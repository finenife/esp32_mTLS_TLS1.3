<template>
  <div id="app">
    <nav>
      <router-link to="/login">Login</router-link> |
      <router-link to="/register">Register</router-link>|
      <router-link to="/">Dashboard</router-link> | 
      <router-link to="/candump">Candump</router-link> |
      <router-link to="/admin">Admin</router-link>
    </nav>
    <router-view/>
  </div>
</template>

<script>
export default {
  name: 'App'
}
</script>