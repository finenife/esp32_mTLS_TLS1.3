<template>
  <div class="container mx-auto px-4 py-8">
    <h1 class="text-3xl font-bold mb-6">Account Settings</h1>

    <!-- Profile Picture Section -->
    <div class="bg-white rounded-lg shadow-md p-6 mb-6 relative">
      <h2 class="text-xl font-semibold mb-4">Profile Picture</h2>
      <div class="flex items-center relative">
        <img :src="currentProfilePicture" alt="Profile Picture" class="w-28 h-28 rounded-full mr-4 object-center" />
        <button @click="showAvatarOptions" class="bottom-0 right-0 transform translate-x-1/2 translate-y-1/2 bg-white rounded-full p-1 shadow">
          <img :src="editIcon" alt="Edit" class="w-6 h-6 z-20" />
        </button>
      </div>
      
      <!-- Avatar Options Modal -->
      <div v-if="isAvatarOptionsVisible" class="avatar-options-modal absolute top-full left-0 mt-2 bg-white border rounded-md shadow-lg p-4 z-10">
        <h3 class="font-semibold mb-2 relative">Select an Avatar</h3>
        <div class="grid grid-cols-3 gap-2">
          <img 
            v-for="avatar in avatars" 
            :key="avatar" 
            :src="getAvatarPath(avatar)" 
            alt="Avatar" 
            @click="selectAvatar(avatar)" 
            class="cursor-pointer w-16 h-16 rounded-full border hover:border-blue-500"
          />
        </div>
        <button @click="isAvatarOptionsVisible = false" class="mt-2 text-blue-600">Close</button>
      </div>
    </div>

    <!-- Other sections remain unchanged -->
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
import editIcon from '@/assets/edit-icon.png'; // Assuming you have an edit icon
import avatar1 from '@/assets/1.png';
import avatar2 from '@/assets/2.png';
import avatar3 from '@/assets/3.png';
import avatar4 from '@/assets/4.png';
import avatar5 from '@/assets/5.png';
import avatar6 from '@/assets/6.png';

export default {
  data() {
    return {
      currentProfilePicture: avatar1, // Default profile picture
      editIcon: editIcon, 
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
      },
      isAvatarOptionsVisible: false,
     avatars: [avatar1, avatar2, avatar3, avatar4, avatar5, avatar6], // Avatar options
    };
  },
  methods: {
    updateProfile() {
      console.log('Profile updated:', this.user);
    },
    changePassword() {
      console.log('Password changed:', this.passwords);
    },
    updateNotifications() {
      console.log('Notification preferences updated:', this.notifications);
    },
    confirmDeleteAccount() {
      if (confirm('Are you sure you want to delete your account? This action cannot be undone.')) {
        console.log('Account deleted');
      }
    }, 
    getAvatarPath(avatar) {
      return avatar; // Dynamically load avatar images
    },
    showAvatarOptions() {
      this.isAvatarOptionsVisible = true;
    },
    selectAvatar(avatar) {
      this.currentProfilePicture = avatar;
      this.isAvatarOptionsVisible = false;
      this.storeAvatarInDatabase(avatar);
    },
    async storeAvatarInDatabase(avatar) {
      try {
        const response = await fetch('/api/store-avatar', {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({ avatarName: `${avatar}.png` }),
        });

        if (!response.ok) throw new Error('Failed to store avatar');
        
        console.log('Avatar stored successfully:', avatar);
      } catch (error) {
        console.error('Error storing avatar:', error);
      }
    }
  }
};
</script>

<style scoped>
.container {
  max-width: 800px; /* Optional max width */
}
.avatar-options-modal {
  position: absolute;
  top: 100%; /* Adjust as needed */
}
</style>
