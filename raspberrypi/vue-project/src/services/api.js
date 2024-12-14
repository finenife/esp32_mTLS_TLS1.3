import axios from "axios";
import keycloak from "../plugins/keycloak"; // Adjust the path as necessary

const api = axios.create({
  baseURL: "http://localhost:5000/api", // Your backend API base URL
});

// Add an interceptor to include the Keycloak token in requests
api.interceptors.request.use(
  (config) => {
    const token = sessionStorage.getItem("keycloakToken"); // Get token from local storage
    console.log(sessionStorage.getItem("keycloakToken"));
    if (token) {
      config.headers.Authorization = `Bearer ${token}`; // Set the Authorization header
    }
    return config;
  },
  (error) => {
    return Promise.reject(error);
  }
);

export default api;
