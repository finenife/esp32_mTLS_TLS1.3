import { createRouter, createWebHistory } from "vue-router";
import Login from "@/components/login.vue";
import Registration from "@/components/registration.vue";
import UserDashboard from "@/components/UserDashboard.vue";
import Candump from "@/components/candump.vue";
import Adminpage from "@/components/adminpage.vue";

const routes = [
  { path: "/login", name: "Login", component: Login },
  { path: "/register", name: "Registration", component: Registration },
  { path: "/", name: "Index", component: UserDashboard}, 
  { path: "/candump", name: "Candump", component: Candump},
  { path: "/admin", name: "Admin", component: Adminpage}
];

const router = createRouter({
  history: createWebHistory(),
  routes,
});

export default router;
