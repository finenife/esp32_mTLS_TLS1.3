<!-- AdminPanel.vue -->
<template>
  <div class="flex h-screen bg-gray-100">
    <!-- Sidebar -->
    <div class="hidden md:flex md:flex-shrink-0">
      <div class="flex flex-col w-64">
        <div class="flex items-center h-16 px-4 bg-gray-900">
          <span class="text-lg font-semibold text-white">Menu</span>
        </div>
        <div class="flex-1 flex flex-col overflow-y-auto bg-gray-800">
          <nav class="flex-1 px-2 py-4 space-y-1">
            <button @click="currentComponent = 'dashboard'" class="relative flex items-center px-2 py-2 text-sm font-medium text-white rounded-md hover:bg-gray-700">
              Dashboard
              <div class="absolute -center -right-5 w-5 h-5 bg-red-500 rounded-full text-center">10</div>
            </button>
            <button @click="currentComponent = 'friends'" class="flex items-center px-2 py-2 text-sm font-medium text-gray-300 rounded-md hover:bg-gray-700 hover:text-white">
              Friends
            </button>
            <button @click="currentComponent = 'candump'" class="flex items-center px-2 py-2 text-sm font-medium text-gray-300 rounded-md hover:bg-gray-700 hover:text-white">
              CanDump
            </button>
            <button @click="currentComponent = 'UpdatePage'" class="flex items-center px-2 py-2 text-sm font-medium text-gray-300 rounded-md hover:bg-gray-700 hover:text-white">
              Check for Update
            </button>
          </nav>
        </div>
      </div>
    </div>

    <!-- Main content -->
    <div class="flex flex-col w-0 flex-1 overflow-hidden">
      <!-- Top bar -->
      <div class="relative z-10 flex-shrink-0 flex h-16 bg-white shadow">
        <div class="flex-1 px-4 flex justify-between">
          <div class="flex-1 flex">
            <div class="w-full flex md:ml-0">
              <label for="search_field" class="sr-only">Search</label>
              <div class="relative w-full text-gray-400 focus-within:text-gray-600">
                <div class="absolute inset-y-0 left-0 flex items-center pointer-events-none">
                  <svg class="h-5 w-5" fill="currentColor" viewBox="0 0 20 20">
                    <path fill-rule="evenodd" clip-rule="evenodd" d="M8 4a4 4 0 100 8 4 4 0 000-8zM2 8a6 6 0 1110.89 3.476l4.817 4.817a1 1 0 01-1.414 1.414l-4.816-4.816A6 6 0 012 8z" />
                  </svg>
                </div>
                <input id="search_field" class="block w-full h-full pl-8 pr-3 py-2 border-transparent text-gray-900 placeholder-gray-500 focus:outline-none focus:placeholder-gray-400 focus:ring-0 focus:border-transparent sm:text-sm" placeholder="Search" type="search">
              </div>
            </div>
          </div>
          <div class="ml-4 flex items-center md:ml-6">
            <!-- Profile dropdown -->
            <div class="ml-3 relative">
              <div>
                <button @click="toggleUserMenu" class="max-w-xs bg-white flex items-center text-sm rounded-full focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-indigo-500" id="user-menu" aria-haspopup="true">
                  <span class="sr-only">Open user menu</span>
                  <img class="h-8 w-8 rounded-full" src="https://images.unsplash.com/photo-1472099645785-5658abf4ff4e?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=facearea&facepad=2&w=256&h=256&q=80" alt="">
                </button>
              </div>
              <transition enter-active-class="transition ease-out duration-100" enter-from-class="transform opacity-0 scale-95" enter-to-class="transform opacity-100 scale-100" leave-active-class="transition ease-in duration-75" leave-from-class="transform opacity-100 scale-100" leave-to-class="transform opacity-0 scale-95">
                <div v-if="isUserMenuOpen" class="origin-top-right absolute right-0 mt-2 w-48 rounded-md shadow-lg py-1 bg-white ring-1 ring-black ring-opacity-5" role="menu" aria-orientation="vertical" aria-labelledby="user-menu">
                  <button @click="loadAccountScreen" class="block px-4 py-2 text-sm text-gray-700 hover:bg-gray-100" role="menuitem">Your Profile</button>
                  <a href="#" class="block px-4 py-2 text-sm text-gray-700 hover:bg-gray-100" role="menuitem">Settings</a>
                  <a href="#" class="block px-4 py-2 text-sm text-gray-700 hover:bg-gray-100" role="menuitem">Sign out</a>
                </div>
              </transition>
            </div>
          </div>
        </div>
      </div>

      <!-- Main content area -->
      <main class="flex-1 relative overflow-y-auto focus:outline-none">
        <div class="py-6">
          <component :is="currentComponent" />
        </div>
      </main>
    </div>
  </div>
</template>

<script>

import dashboard from './dashboard.vue';
import candump from './candump.vue';
import friends from './friends.vue';
import UpdatePage from './updatePage.vue';
import AccountSettings from './accountSettings.vue';

export default {
  components: { 
    dashboard,
    friends,
    candump, 
    UpdatePage, 
    AccountSettings
   },
  data() {
    return {
      currentComponent: 'dashboard', //Default component
      isUserMenuOpen: false,
    }
  },
  methods: {
    toggleUserMenu() {
      this.isUserMenuOpen = !this.isUserMenuOpen
    },
    loadAccountScreen(){
      this.toggleUserMenu();
      this.currentComponent = 'AccountSettings'
    }
  }
}
</script>
