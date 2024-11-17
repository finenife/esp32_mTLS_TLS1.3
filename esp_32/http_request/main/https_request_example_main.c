/*
 * HTTPS GET Example using mTLS to Locally hosted nginx server. 
 * This is based on the esp-idf/examples/protocols/https_request
 *
 * - set-target esp32-s3
 * - look at sdkconfig diff
 * - Requires 3 files to be loaded:
 *      - ca-chain.cert.pem - this is the certificate chain the esp32-s3 will utilize to validate the server's self-signed certificate.
 *      - client.cert.pem - this is the certificate that was issued by the server, signed by the intermediate certificate. 
 *      - client.plain.pem - this is the private key to be utilized by the client, must be the private key that was used in the crt for the server. 
 * 
 * You must have TLS1.3 Support setup in menuconfig. 
 * 
 * 11/15/2024 - Building passing, IDF.py build
 * 
 * Dependencies: 
 * - Another server to hit on local network (I used a raspberry pi)
 * - Self-signed Certificate creation: completed using this guide:  https://jamielinux.com/docs/openssl-certificate-authority/create-the-root-pair.html
 * 
 * ------------------------------------------------------------
 * - a python flask application - ~/nginx/myproject.py
 * from flask import Flask
 * app = Flask(__name__)

 * @app.route("/")
 * def hello():
 *     return "<h1 style='color:blue'>Hello There!</h1>"
 * 
 * if __name__ == "__main__":
 *     app.run(host='0.0.0.0')
 * ------------------------------------------------------------
 * - a uwsgi application for managing the flask app - ~/nginx/wsgi.py
 *   from myproject import app
 *   if __name__ == "__main__":
 *      app.run()
 * ------------------------------------------------------------
 * - a uwsgi configuration file: ~/nginx/myproject.ini
 * [uwsgi]
 * module = wsgi:app
 * 
 * master = true
 * processes = 5
 * 
 * socket = myproject.sock
 * uid = www-data
 * gid = www-data
 * chown-socket = www-data:www-data
 * chmod-socket = 660
 * vacuum = true
 * 
 * die-on-term = true
 * ------------------------------------------------------------
 * - a systemd service for the flask application through uswgi - /etc/systemd/system/myproject.service
 * [Unit]
 * Description=uWSGI instance to serve myproject
 * After=network.target
 * 
 * [Service]
 * ProtectHome=false
 * WorkingDirectory=/home/hayden/nginx
 * Environment="PATH=/home/hayden/nginx/myprojectenv/bin"
 * ExecStart=/home/hayden/nginx/myprojectenv/bin/uwsgi --ini myproject.ini
 * 
 * [Install]
 * WantedBy=multi-user.target
 * 
 * ------------------------------------------------------------
 * - NGINX server configuration on raspberry pi: - /etc/nginx/sites-available/myproject
 *              server {
 *                  listen 443 ssl;
 *                   include snippets/self-signed.conf;
 *                   include snippets/ssl-params.conf;
 *
 *                   location / {
 *                       if ($ssl_client_verify != SUCCESS){
 *                            return 401 'blocked access to mTLS-protected resource';
 *                        }
 *                       include uwsgi_params;
 *                       uwsgi_pass unix:/home/hayden/nginx/myproject.sock;
 *                   }
 *               }
 *               server {
 *                   listen 80;
 *
 *                   return 302 https://192.168.1.175;
 *
 *               }
 *
 *   
 * - /etc/nginx/snippets/self-signed.conf:
 *       ssl_certificate /etc/ssl/certs/intermediate/server.cert.pem;
 *       ssl_certificate_key /etc/ssl/private/intermediate/server.key.pem;
 *       ssl_client_certificate /etc/ssl/certs/intermediate/ca-chain.cert.pem;
 *
 * - /etc/nginx/snippets/ssl-params.conf:
 *       ssl_protocols TLSv1.3;
 *       ssl_prefer_server_ciphers on;
 *       ssl_dhparam /etc/nginx/dhparam.pem;
 *       ssl_ciphers EECDH+AESGCM:EDH+AESGCM;
 *       ssl_ecdh_curve secp384r1;
 *       ssl_session_timeout  10m;
 *       ssl_session_cache shared:SSL:10m;
 *       ssl_session_tickets off;
 *       ssl_stapling on;
 *       ssl_stapling_verify on;
 *       ssl_verify_client optional;
 *       resolver 8.8.8.8 8.8.4.4 valid=300s;
 *       resolver_timeout 5s;
 *       # Disable strict transport security for now. You can uncomment the following
 *       # line if you understand the implications.
 *       #add_header Strict-Transport-Security "max-age=63072000; includeSubDomains; preload";
 *       add_header X-Frame-Options DENY;
 *       add_header X-Content-Type-Options nosniff;
 *       add_header X-XSS-Protection "1; mode=block";
 * 
 * ------------------------------------------------------------
 * - a nginx systemd service - /lib/systemd/system/nginx.service
 * # Stop dance for nginx
 * # =======================
 * #
 * # ExecStop sends SIGQUIT (graceful stop) to the nginx process.
 * # If, after 5s (--retry QUIT/5) nginx is still running, systemd takes control
 * # and sends SIGTERM (fast shutdown) to the main process.
 * # After another 5s (TimeoutStopSec=5), and if nginx is alive, systemd sends
 * # SIGKILL to all the remaining processes in the process group (KillMode=mixed).
 * #
 * # nginx signals reference doc:
 * # http://nginx.org/en/docs/control.html
 * #
 * [Unit]
 * Description=A high performance web server and a reverse proxy server
 * Documentation=man:nginx(8)
 * After=network-online.target remote-fs.target nss-lookup.target
 * Wants=network-online.target
 * 
 * [Service]
 * ProtectHome=false
 * Type=forking
 * PIDFile=/run/nginx.pid
 * ExecStartPre=/usr/sbin/nginx -t -q -g 'daemon on; master_process on;'
 * ExecStart=/usr/sbin/nginx -g 'daemon on; master_process on;'
 * ExecReload=/usr/sbin/nginx -g 'daemon on; master_process on;' -s reload
 * ExecStop=-/sbin/start-stop-daemon --quiet --stop --retry QUIT/5 --pidfile /run/nginx.pid
 * TimeoutStopSec=5
 * KillMode=mixed
 * 
 * [Install]
 * WantedBy=multi-user.target
 * 
 * ------------------------------------------------------------
 * Once the server is running, and the appropriate certificates are integrated here you can build with 
 *  - idf.py build
 *  - idf.py flash
 *  - idf.py monitor
 * 
 * 
 * After running you will see the following: 
 * 
 * A TON of debug information, and then finally:
 * 
 * HTTP/1.1 200 OK
 * Server: nginx/1.22.1
 * Date: Fri, 15 Nov 2024 20:20:29 GMT
 * Content-Type: text/html; charset=utf-8
 * Content-Length: 40
 * Connection: keep-alive
 * X-Frame-Options: DENY
 * X-Content-Type-Options: nosniff
 * X-XSS-Protection: 1; mode=block
 * 
 * <h1 style='color:blue'>Hello There!</h1>
 * 
 * This is the response we expected to see from our flask mTLS protected endpoint!
 * 
 *
 */

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "protocol_examples_common.h"
#include "esp_sntp.h"
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "esp_tls.h"
#include "sdkconfig.h"
#if CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
#include "esp_crt_bundle.h"
#endif
#include "time_sync.h"

/* Constants that aren't configurable in menuconfig */
#define WEB_SERVER "192.168.1.175"
#define WEB_PORT "443"
#define WEB_URL "https://192.168.1.175"

#define SERVER_URL_MAX_SZ 256

static const char *TAG = "example";

/* Timer interval once every day (24 Hours) */
#define TIME_PERIOD (86400000000ULL)

static const char HOWSMYSSL_REQUEST[] = "GET " WEB_URL " HTTP/1.1\r\n"
                             "Host: "WEB_SERVER"\r\n"
                             "User-Agent: esp-idf/1.0 esp32\r\n"
                             "\r\n";

//setup for how to verify the server
extern const uint8_t server_root_cert_pem_start[] asm("_binary_ca_chain_cert_pem_start");
extern const uint8_t server_root_cert_pem_end[]   asm("_binary_ca_chain_cert_pem_end");

//setup for presenting client cert
extern const uint8_t client_cert_pem_start[] asm("_binary_client_cert_pem_start");
extern const uint8_t client_cert_pem_end[]   asm("_binary_client_cert_pem_end");

//setup for client private key
extern const uint8_t client_key_pem_start[] asm("_binary_client_plain_pem_start");
extern const uint8_t client_key_pem_end[]   asm("_binary_client_plain_pem_end");

//setup for client key password
//extern const uint8_t clientkey_password_start[] asm("_binary_clientkey_password_start");
//extern const uint8_t clientkey_password_end[]   asm("_binary_clientkey_password_end");

static const int server_supported_ciphersuites[] = {MBEDTLS_TLS1_3_AES_256_GCM_SHA384, 0};

/*static void https_get_request(esp_tls_cfg_t cfg, const char *WEB_SERVER_URL, const char *REQUEST)
{
    char buf[512];
    int ret, len;

    esp_tls_t *tls = esp_tls_init();
    if (!tls) {
        ESP_LOGE(TAG, "Failed to allocate esp_tls handle!");
        goto exit;
    }

    if (esp_tls_conn_http_new_sync(WEB_SERVER_URL, &cfg, tls) == 1) {
        ESP_LOGI(TAG, "Connection established...");
    } else {
        ESP_LOGE(TAG, "Connection failed...");
        goto cleanup;
    }

#ifdef CONFIG_EXAMPLE_CLIENT_SESSION_TICKETS
     The TLS session is successfully established, now saving the session ctx for reuse 
    if (save_client_session) {
        esp_tls_free_client_session(tls_client_session);
        tls_client_session = esp_tls_get_client_session(tls);
    }
#endif

    size_t written_bytes = 0;
    do {
        ret = esp_tls_conn_write(tls,
                                 REQUEST + written_bytes,
                                 strlen(REQUEST) - written_bytes);
        if (ret >= 0) {
            ESP_LOGI(TAG, "%d bytes written", ret);
            written_bytes += ret;
        } else if (ret != ESP_TLS_ERR_SSL_WANT_READ  && ret != ESP_TLS_ERR_SSL_WANT_WRITE) {
            ESP_LOGE(TAG, "esp_tls_conn_write  returned: [0x%02X](%s)", ret, esp_err_to_name(ret));
            goto cleanup;
        }
    } while (written_bytes < strlen(REQUEST));

    ESP_LOGI(TAG, "Reading HTTP response...");
    do {
        len = sizeof(buf) - 1;
        memset(buf, 0x00, sizeof(buf));
        ret = esp_tls_conn_read(tls, (char *)buf, len);

        if (ret == ESP_TLS_ERR_SSL_WANT_WRITE  || ret == ESP_TLS_ERR_SSL_WANT_READ) {
            continue;
        } else if (ret < 0) {
            ESP_LOGE(TAG, "esp_tls_conn_read  returned [-0x%02X](%s)", -ret, esp_err_to_name(ret));
            break;
        } else if (ret == 0) {
            ESP_LOGI(TAG, "connection closed");
            break;
        }

        len = ret;
        ESP_LOGD(TAG, "%d bytes read", len);
        Print response directly to stdout as it is read
        for (int i = 0; i < len; i++) {
            putchar(buf[i]);
        }
        putchar('\n'); // JSON output doesn't have a newline at end
    } while (1);

cleanup:
    esp_tls_conn_destroy(tls);
exit:
    for (int countdown = 10; countdown >= 0; countdown--) {
        ESP_LOGI(TAG, "%d...", countdown);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}*/

static esp_err_t https_send_request(esp_tls_t *tls, const char *REQUEST)
{
    int ret;
    size_t written_bytes = 0;
    do {
        ret = esp_tls_conn_write(tls,
                                 REQUEST + written_bytes,
                                 strlen(REQUEST) - written_bytes);
        if (ret >= 0) {
            ESP_LOGI(TAG, "%d bytes written", ret);
            written_bytes += ret;
        } else if (ret != ESP_TLS_ERR_SSL_WANT_READ && ret != ESP_TLS_ERR_SSL_WANT_WRITE) {
            ESP_LOGE(TAG, "esp_tls_conn_write returned: [0x%02X](%s)", ret, esp_err_to_name(ret));
            return ESP_FAIL;
        }
    } while (written_bytes < strlen(REQUEST));

    return ESP_OK;
}

static esp_err_t https_read_response(esp_tls_t *tls)
{
    char buf[512];
    int ret, len;

    ESP_LOGI(TAG, "Reading HTTP response...");
    do {
        len = sizeof(buf) - 1;
        memset(buf, 0x00, sizeof(buf));
        ret = esp_tls_conn_read(tls, (char *)buf, len);

        if (ret == ESP_TLS_ERR_SSL_WANT_WRITE || ret == ESP_TLS_ERR_SSL_WANT_READ) {
            continue;
        } else if (ret < 0) {
            ESP_LOGE(TAG, "esp_tls_conn_read returned [-0x%02X](%s)", -ret, esp_err_to_name(ret));
            return ESP_FAIL;
        } else if (ret == 0) {
            ESP_LOGI(TAG, "connection closed");
            break;
        }

        len = ret;
        ESP_LOGD(TAG, "%d bytes read", len);
        /* Print response directly to stdout as it is read */
        for (int i = 0; i < len; i++) {
            putchar(buf[i]);
        }
        putchar('\n'); // JSON output doesn't have a newline at end
        break;
    } while (1);

    return ESP_OK;
}

static void https_get_request(esp_tls_cfg_t cfg, const char *WEB_SERVER_URL, const char *REQUEST)
{
    esp_tls_t *tls = esp_tls_init();
    if (!tls) {
        ESP_LOGE(TAG, "Failed to allocate esp_tls handle!");
        goto exit;
    }

    if (esp_tls_conn_http_new_sync(WEB_SERVER_URL, &cfg, tls) == 1) {
        ESP_LOGI(TAG, "Connection established...");
    } else {
        ESP_LOGE(TAG, "Connection failed...");
        goto cleanup;
    }

#ifdef CONFIG_EXAMPLE_CLIENT_SESSION_TICKETS
    /* The TLS session is successfully established, now saving the session ctx for reuse */
    if (save_client_session) {
        esp_tls_free_client_session(tls_client_session);
        tls_client_session = esp_tls_get_client_session(tls);
    }
#endif

    if (https_send_request(tls, REQUEST) != ESP_OK) {
        goto cleanup;
    }

    if (https_read_response(tls) != ESP_OK) {
        goto cleanup;
    }

cleanup:
    esp_tls_conn_destroy(tls);
exit:
    for (int countdown = 10; countdown >= 0; countdown--) {
        ESP_LOGI(TAG, "%d...", countdown);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}




#if CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
static void https_get_request_using_crt_bundle(void)
{
    ESP_LOGI(TAG, "https_request using crt bundle");
    //ESP_LOGI(TAG, "ClientKey_Password @ %p: %s",clientkey_password_start,(const unsigned char *) clientkey_password_start);
 
    esp_tls_cfg_t cfg = {
        //TODO:consider utilizing the heap instead with malloc(),  I had to make the stack 2X larger for the task to be able to access everything.
        //.crt_bundle_attach = esp_crt_bundle_attach,
        .cacert_buf = (const unsigned char *) server_root_cert_pem_start,
        .cacert_bytes = server_root_cert_pem_end - server_root_cert_pem_start,
        .clientcert_buf = (const unsigned char *) client_cert_pem_start,
        .clientcert_bytes = client_cert_pem_end - client_cert_pem_start, 
        .clientkey_buf = (const unsigned char *) client_key_pem_start,
        .clientkey_bytes = client_key_pem_end - client_key_pem_start,
        .ciphersuites_list = server_supported_ciphersuites
        //TODO: I can load an encrypted key using .clientkey_password
        // I tried below while loading client.key.pem and got E (7815) esp-tls-mbedtls: mbedtls_pk_parse_keyfile returned -0x2E80 - MBEDTLS_ERR_PKCS5_FEATURE_UNAVAILABLE
        /*.clientkey_password = (const unsigned char *) clientkey_password_start,
          .clientkey_password_len = clientkey_password_end - clientkey_password_start, 
          .clientkey_bytes = client_key_pem_end - client_key_pem_start*/
        
    };
    https_get_request(cfg, WEB_URL, HOWSMYSSL_REQUEST);
}
#endif // CONFIG_MBEDTLS_CERTIFICATE_BUNDLE

static void https_request_task(void *pvparameters)
{
    ESP_LOGI(TAG, "Start https_request example");

#if CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
    https_get_request_using_crt_bundle();
    https_get_request_using_crt_bundle();
    https_get_request_using_crt_bundle();
    https_get_request_using_crt_bundle();
#endif
    ESP_LOGI(TAG, "Minimum free heap size: %" PRIu32 " bytes", esp_get_minimum_free_heap_size());
    ESP_LOGI(TAG, "Finish https_request example");
    vTaskDelete(NULL);
}

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

    if (esp_reset_reason() == ESP_RST_POWERON) {
        ESP_LOGI(TAG, "Updating time from NVS");
        ESP_ERROR_CHECK(update_time_from_nvs());
    }

    const esp_timer_create_args_t nvs_update_timer_args = {
            .callback = (void *)&fetch_and_store_time_in_nvs,
    };

    esp_timer_handle_t nvs_update_timer;
    ESP_ERROR_CHECK(esp_timer_create(&nvs_update_timer_args, &nvs_update_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(nvs_update_timer, TIME_PERIOD));

    xTaskCreate(&https_request_task, "https_get_task", 16384, NULL, 5, NULL);
}
