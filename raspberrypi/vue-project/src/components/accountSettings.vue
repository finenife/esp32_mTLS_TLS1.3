<template>
  <div class="container mx-auto px-4 py-8">
    <h1 class="text-3xl font-bold mb-6">Account Settings</h1>

    <!-- Personal Information Section -->
    <div class="bg-white rounded-lg shadow-md p-6 mb-6">
      <h2 class="text-xl font-semibold mb-4">Personal Information</h2>
      <form @submit.prevent="updateProfile">
        <div class="mb-4">
          <label for="name" class="block text-sm font-medium text-gray-700">Full Name</label>
          <input type="text" id="name" v-model="user.name" required class="mt-1 block w-full border-gray-300 rounded-md shadow-sm focus:border-blue-500 focus:ring focus:ring-blue-200 focus:ring-opacity-50">
        </div>
        <div class="mb-4">
          <label for="email" class="block text-sm font-medium text-gray-700">Email Address</label>
          <input type="email" id="email" v-model="user.email" required class="mt-1 block w-full border-gray-300 rounded-md shadow-sm focus:border-blue-500 focus:ring focus:ring-blue-200 focus:ring-opacity-50">
        </div>
        <button type="submit" class="bg-blue-600 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded">
          Update Profile
        </button>
      </form>
    </div>

    <!-- Change Password Section -->
    <div class="bg-white rounded-lg shadow-md p-6 mb-6">
      <h2 class="text-xl font-semibold mb-4">Change Password</h2>
      <form @submit.prevent="changePassword">
        <div class="mb-4">
          <label for="current-password" class="block text-sm font-medium text-gray-700">Current Password</label>
          <input type="password" id="current-password" v-model="passwords.current" required class="mt-1 block w-full border-gray-300 rounded-md shadow-sm focus:border-blue-500 focus:ring focus:ring-blue-200 focus:ring-opacity-50">
        </div>
        <div class="mb-4">
          <label for="new-password" class="block text-sm font-medium text-gray-700">New Password</label>
          <input type="password" id="new-password" v-model="passwords.new" required class="mt-1 block w-full border-gray-300 rounded-md shadow-sm focus:border-blue-500 focus:ring focus:ring-blue-200 focus:ring-opacity-50">
        </div>
        <button type="submit" class="bg-blue-600 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded">
          Change Password
        </button>
      </form>
    </div>

    <!-- Notification Preferences Section -->
    <div class="bg-white rounded-lg shadow-md p-6 mb-6">
      <h2 class="text-xl font-semibold mb-4">Notification Preferences</h2>
      <form @submit.prevent="updateNotifications">
        <div class="flex items-center mb-4">
          <input type="checkbox" id="email-notifications" v-model="notifications.email" class="mr-2 leading-tight">
          <label for="email-notifications" class="text-sm">Email Notifications</label>
        </div>
        <div class="flex items-center mb-4">
          <input type="checkbox" id="sms-notifications" v-model="notifications.sms" class="mr-2 leading-tight">
          <label for="sms-notifications" class="text-sm">SMS Notifications</label>
        </div>
        <button type="submit" class="bg-blue-600 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded">
          Save Preferences
        </button>
      </form>
    </div>

    <!-- Delete Account Section -->
    <div class="bg-red-100 rounded-lg shadow-md p-6 mb-6">
      <h2 class="text-xl font-semibold mb-4 text-red-600">Delete Account</h2>
      <p class="mb-4 text-red-800">Are you sure you want to delete your account? This action cannot be undone.</p>
      <button @click.prevent="confirmDeleteAccount()" class="bg-red-600 hover:bg-red-700 text-white font-bold py-2 px-4 rounded">
        Delete My Account
      </button>
    </div>

  </div>
</template>

<script>
export default {
  data() {
    return {
      user: {
        name: 'John Doe',
        email: 'john.doe@example.com'
      },
      passwords: {
        current: '',
        new: ''
      },
      notifications: {
        email: true,
        sms: false
      }
    };
  },
  methods: {
    updateProfile() {
      // Logic to update the user's profile
      console.log('Profile updated:', this.user);
    },
    changePassword() {
      // Logic to change the user's password
      console.log('Password changed:', this.passwords);
    },
    updateNotifications() {
      // Logic to update notification preferences
      console.log('Notification preferences updated:', this.notifications);
    },
    confirmDeleteAccount() {
      // Logic to confirm and delete the user's account
      if (confirm('Are you sure you want to delete your account? This action cannot be undone.')) {
        console.log('Account deleted');
      }
    }
  }
};
</script>

<style scoped>
.container {
  max-width: 800px; /* Optional max width */
}
</style>