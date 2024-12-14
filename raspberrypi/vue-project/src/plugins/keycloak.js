import Keycloak from 'keycloak-js';

const keycloakConfig = {
  url: 'http://192.168.1.175:8080',
  realm: 'Test',
  clientId: 'frontend'
};

const keycloak = new Keycloak(keycloakConfig);

const LoginStatus = {
  NOT_AUTHENTICATED: 'NOT_AUTHENTICATED',
  AUTHENTICATED: 'AUTHENTICATED',
  FAILED: 'FAILED'
};

export default {
  install: (app) => {
    app.config.globalProperties.$keycloak = keycloak;
    app.provide('keycloak', keycloak);
  },
  keycloak,
  LoginStatus,
  init: () => {
    return new Promise((resolve, reject) => {
      keycloak.init({
        onLoad: 'login-required',
        silentCheckSsoRedirectUri: window.location.origin + '/silent-check-sso.html',
        checkLoginIframe: false,
        promiseType: 'native'
      })
        .then((authenticated) => {
          if (authenticated) {
            console.log("Storing token:");
            console.log(keycloak.token)
            sessionStorage.setItem("keycloakToken", keycloak.token);
            resolve(LoginStatus.AUTHENTICATED);
            startTokenRefresh();
          } else {
            resolve(LoginStatus.NOT_AUTHENTICATED);
          }
        })
        .catch(() => {
          reject(LoginStatus.FAILED);
        });
    });
  },
  login: () => keycloak.login(),
  logout: () => keycloak.logout(),
  getToken: () => keycloak.token,
};

// Function to start the token refresh process
function startTokenRefresh() {
  setInterval(() => {
    keycloak.updateToken(200) // Refresh token if it's valid for less than 70 seconds
      .then(refreshed => {
        if (refreshed) {
          console.log('Token refreshed');
          sessionStorage.setItem('keycloakToken', keycloak.token); // Update stored token
        } else {
          console.warn('Token not refreshed, valid for ' + Math.round(keycloak.tokenParsed.exp + keycloak.timeSkew - new Date().getTime() / 1000) + ' seconds');
        }
      })
      .catch(() => {
        console.error('Failed to refresh token');
        // Optionally log out user or handle session expiration
        keycloak.logout();
      });
  }, 8000); // Refresh every 170 seconds
}