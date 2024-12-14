import './assets/main.css'
import { createApp } from 'vue'
import App from './App.vue'
import router from './router'

import keycloak from './plugins/keycloak'

const app = createApp(App)

app.use(keycloak)

keycloak.init()
  .then((status) => {
    if (status === keycloak.LoginStatus.AUTHENTICATED) {
      console.log("User is authenticated");
      console.log("Stored token:", keycloak.token); // Log the token
      localStorage.setItem("keycloakToken", keycloak.token); // Store token
      console.log("Stored token:", keycloak.token); // Log the token
    } else {
      console.log("User is not authenticated");
    }
    app.use(router);
    app.mount("#app");
  })
  .catch((error) => {
    console.error("Failed to initialize Keycloak", error);
  });