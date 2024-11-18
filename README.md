# ESP32 mTLS Server Connection Guide
This document outlines the steps to set up an mTLS server connection using an ESP32 and Flask on a Raspberry Pi.
I'm using this repo to house some code for version control


## Dependencies: 
1. Another server to hit on local network (I used a raspberry pi 5 running latest pios lite)
2. Self-signed Certificate creation: completed using this guide:  https://jamielinux.com/docs/openssl-certificate-authority/create-the-root-pair.html
    - These certificates will ultimately be stored in a place where nginx and tests can access them. 
    - For a production application (which this is definitely not), follow your enterprise PKI procedures when creating/handling/updating any keys, and store your root key/cert offline. 
3. a super simple bare-bones python flask application - `Git: raspberrypi/flask/*` -> `Raspberrypi: ~/nginx/*`
```python
from flask import Flask
app = Flask(__name__)

@app.route("/")
def hello():
    return "<h1 style='color:blue'>Hello There!</h1>"

if __name__ == "__main__":
    app.run(host='0.0.0.0')
```

4. a simple uwsgi application for managing the flask app - `Git: raspberrypi/flask/wsgi.py`-> `Raspberrypi: ~/nginx/wsgi.py`
```python
  from myproject import app
  if __name__ == "__main__":
     app.run()
```
5. a uwsgi configuration file - `Git: raspberrypi/flask/myproject.ini`-> `Raspberrypi: ~/nginx/myproject.ini`
```
[uwsgi]
module = wsgi:app

master = true
processes = 5

socket = myproject.sock
uid = www-data
gid = www-data
chown-socket = www-data:www-data
chmod-socket = 660
vacuum = true

die-on-term = true
```
6. a systemd service for the flask application through uswgi - `Git: raspberrypi/flask/myproject.service`-> `Raspberrypi: /etc/systemd/system/myproject.service`
    - Do not forget to enable the service with `sudo systemctl enable myproject`
```
[Unit]
Description=uWSGI instance to serve myproject
After=network.target

[Service]
ProtectHome=false
WorkingDirectory=/home/hayden/nginx
Environment="PATH=/home/hayden/nginx/myprojectenv/bin"
ExecStart=/home/hayden/nginx/myprojectenv/bin/uwsgi --ini myproject.ini

[Install]
WantedBy=multi-user.target
```
7. Install `nginx` with `apt install nginx`
8. Run `sudo systemctl enable nginx` to make sure nginx starts up
9. NGINX server configuration on raspberry pi - `Git: raspberrypi/nginx/etc/*`-> `Raspberrypi: /etc/nginx/*` - /etc/nginx/sites-available/myproject
``` 
            server {
                 listen 443 ssl;
                  include snippets/self-signed.conf;
                  include snippets/ssl-params.conf;
                  location / {
                      if ($ssl_client_verify != SUCCESS){   // ssl_client_verify holds the result of ssl_verify_client
                           return 401 'blocked access to mTLS-protected resource';
                       }
                      include uwsgi_params;
                      uwsgi_pass unix:/home/hayden/nginx/myproject.sock;
                  }
              }
              server {
                  listen 80;
                  return 302 https://192.168.1.175;
              }
```
10. A small snippet file to allow nginx to know where to look for certificates, and where the private key it should use for TLS exchanges associated with the server-cert `Raspberrypi: /etc/nginx/snippets/self-signed.conf`
```
      ssl_certificate /etc/ssl/certs/intermediate/server.cert.pem;
      ssl_certificate_key /etc/ssl/private/intermediate/server.key.pem;
      ssl_client_certificate /etc/ssl/certs/intermediate/ca-chain.cert.pem;
```
11. A SSL/TLS configuration, SSL is used by nginx, but rest assured, it is TLS v1.3 `Raspberrypi: /etc/nginx/snippets/ssl-params.conf`
    - This was taken from [cipherlist.eu](https://cipherlist.eu/)
    - This only allows TLS1.3 connections, and only offers EECDH+AESGCM:EDH+AESGCM ciphers. 
```
      ssl_protocols TLSv1.3;
      ssl_prefer_server_ciphers on;
      ssl_dhparam /etc/nginx/dhparam.pem;
      ssl_ciphers EECDH+AESGCM:EDH+AESGCM;
      ssl_ecdh_curve secp384r1;
      ssl_session_timeout  10m;
      ssl_session_cache shared:SSL:10m;
      ssl_session_tickets off;
      ssl_stapling on;
      ssl_stapling_verify on;
      ssl_verify_client optional;               #This is where we tell nginx that we want to verify the client, setting optional allows some endpoints to not require mTLS. 
      resolver 8.8.8.8 8.8.4.4 valid=300s;
      resolver_timeout 5s;
      # Disable strict transport security for now. You can uncomment the following
      # line if you understand the implications.
      #add_header Strict-Transport-Security "max-age=63072000; includeSubDomains; preload";
      add_header X-Frame-Options DENY;
      add_header X-Content-Type-Options nosniff;
      add_header X-XSS-Protection "1; mode=block";
```
12. a nginx systemd service - `Git:raspberrypi/nginx/nginx.service`-> `Raspberrypi: /lib/systemd/system/nginx.service`
```
# Stop dance for nginx
# =======================
#
# ExecStop sends SIGQUIT (graceful stop) to the nginx process.
# If, after 5s (--retry QUIT/5) nginx is still running, systemd takes control
# and sends SIGTERM (fast shutdown) to the main process.
# After another 5s (TimeoutStopSec=5), and if nginx is alive, systemd sends
# SIGKILL to all the remaining processes in the process group (KillMode=mixed).
#
# nginx signals reference doc:
# http://nginx.org/en/docs/control.html
#
[Unit]
Description=A high performance web server and a reverse proxy server
Documentation=man:nginx(8)
After=network-online.target remote-fs.target nss-lookup.target
Wants=network-online.target

[Service]
ProtectHome=false
Type=forking
PIDFile=/run/nginx.pid
ExecStartPre=/usr/sbin/nginx -t -q -g 'daemon on; master_process on;'
ExecStart=/usr/sbin/nginx -g 'daemon on; master_process on;'
ExecReload=/usr/sbin/nginx -g 'daemon on; master_process on;' -s reload
ExecStop=-/sbin/start-stop-daemon --quiet --stop --retry QUIT/5 --pidfile /run/nginx.pid
TimeoutStopSec=5
KillMode=mixed
 
[Install]
WantedBy=multi-user.target
```

## Confirm Server is functioning
### Disable mTLS
- To start, lets disable mTLS checking and ensure that you have nginx setup correctly, comment out the following two lines in the files below: 
```
/etc/nginx/sites-available/myproject:
...
Line 6:                    if ($ssl_client_verify != SUCCESS){ 
```
and
```
/etc/nginx/snippets/ssl-params.conf:
...
Line 11:                   ssl_verify_client optional;
```

- **Note:** *With the certificates being self-signed certificates, you will want to ensure the ca-chain where the intermediate and root certificates are included is installed as a certificate on your machine. How to do this will be operating system specific and will not be included in this.*
- test your nginx configuration for any syntax errors with `sudo nginx -t`
- restart nginx with `sudo systemctl restart nginx`
- Attempt to connect to your raspberry pi by a web browser on the LAN and  you should see the following in your browser:
<h1 style='color:blue'>Hello There!</h1>

### Re-enable mTLS and install certs on the test client. 
- Now that you have confirmed the connection, ensure your client certificate, client private key, and the ca-chain are installed on your test machine correctly. Revert the nginx service change from above and readd the line to ensure mTLS verification. 
- test your nginx configuration for any syntax errors with `sudo nginx -t`
- restart nginx with `sudo systemctl restart nginx`
- Attempt to connect to your raspberry pi by a web browser on the same LAN as Raspberry Pi, and this time you should be prompted  and  you should see the following in your browser:
<h1 style='color:blue'>Hello There!</h1>

## Test with ESP32-s3
### Configure ESP-IDF Environment correctly.
The default env in espressif idf is looking for the following certs in the following path, all files should be pem format as output by guide noted above in pre-req #2
1. Server Validation CA Certificate Chain - /http-request/main/ca-chain.cert.pem 
2. The client issued certificate - /http-request/main/client.cert.pem
3. The client issed PEM Private Key (**Unencrypted**) - /http-request/main/client.plain.pem
    - This can be done by `openssl rsa -in {Path to encrypted_private_key.pem} -out {Path to decrypted_private_key.pem}`
    - You will be prompted for the original passphrase that was used when creating the cient key the first time. 
    - **Note:** *Your private key is very sensitive, and should only be known by the client. Follow appropriate best practices when handling private key material. ESP-IDF offers a method to store this with the protected password, but I was not able to get this function working before my initial commit. It is dangerous to have a plain text private key, and this should be avoided in production.*

### Build the image
Once the server is running, and the appropriate certificates are integrated here you can build with 
 - `idf.py build` - a configuration is provided for utilizing esp32-s3 in sdkconfig. 
 - `idf.py flash` - the sdkconfig is setup for flashing over uart, this can be changed with `idf.py menuconfig`
 - `idf.py monitor`
    - The serial console will prompt the user for wifi ssid and password from standard in, always pass it directly as `{SSID} {PWD}` with the space between. You wil not be able to see reply in the console. Press enter/return to submit. 

### Verify the connection
After running you will see the following: 

A TON of debug information, and then finally:

```
HTTP/1.1 200 OK
Server: nginx/1.22.1
Date: Fri, 15 Nov 2024 20:20:29 GMT
Content-Type: text/html; charset=utf-8
Content-Length: 40
Connection: keep-alive
X-Frame-Options: DENY
X-Content-Type-Options: nosniff
X-XSS-Protection: 1; mode=block

<h1 style='color:blue'>Hello There!</h1>
```

### This is the response we expected to see from our flask mTLS protected endpoint!

## Conclusion
This guide provides an overview of setting up an mTLS1.3 connection with ESP32, Nginx, and Flask. Follow each step carefully to ensure successful configuration. At time of writing this, I was able to see the output. This code is not intended, nor deployed in a production environment. I used this for my own learning and thought I would share openly here. 
