<template>
  <div class="container mx-auto px-4 py-8">
    <h1 class="text-3xl font-bold mb-6">Friends</h1>
    
    <!-- Friend Requests Section -->
    <div class="bg-gray-100 rounded-lg p-4 mb-6">
      <h2 class="text-xl font-semibold mb-4">Friend Requests</h2>
      <div v-if="friendRequests.length === 0" class="text-gray-500">
        No new friend requests.
      </div>
      <div v-for="request in friendRequests" :key="request.id" class="flex items-center mb-4">
        <img :src="request.avatar" :alt="request.name" class="w-12 h-12 rounded-full border border-gray-300">
        <div class="ml-4 flex-grow">
          <h3 class="text-lg font-semibold text-gray-800">{{ request.name }}</h3>
          <p class="text-sm text-gray-500">{{ request.title }}</p>
        </div>
        <div>
          <button @click="acceptRequest(request.id)" class="bg-blue-600 hover:bg-blue-700 text-white font-bold py-1 px-3 rounded mr-2">
            Accept
          </button>
          <button @click="declineRequest(request.id)" class="bg-red-600 hover:bg-red-700 text-white font-bold py-1 px-3 rounded">
            Decline
          </button>
        </div>
      </div>
    </div>

    <!-- Friends List Section -->
    <div class="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 gap-6">
      <div v-for="friend in friends" :key="friend.id" class="bg-white rounded-lg shadow-md p-4 flex flex-col">
        <div class="flex items-center mb-4">
          <img :src="friend.avatar" :alt="friend.name" class="w-16 h-16 rounded-full border border-gray-300">
          <div class="ml-4">
            <h2 class="text-xl font-semibold text-gray-800">{{ friend.name }}</h2>
            <p class="text-sm text-gray-500">{{ friend.title }}</p>
            <p class="text-sm text-gray-400">{{ friend.status }}</p>
          </div>
        </div>
        <div class="flex justify-between items-center mt-auto">
          <span class="text-sm text-gray-500">Last seen: {{ friend.lastSeen }}</span>
          <button class="bg-blue-600 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded">
            Message
          </button>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  data() {
    return {
      friendRequests: [
        {
          id: 1,
          name: "David Brown",
          title: "Marketing Specialist",
          avatar: "https://randomuser.me/api/portraits/men/45.jpg"
        },
        {
          id: 2,
          name: "Emma Wilson",
          title: "Data Analyst",
          avatar: "https://randomuser.me/api/portraits/women/32.jpg"
        },
        // Add more requests as needed
      ],
      friends: [
        {
          id: 1,
          name: "Alice Johnson",
          title: "Software Engineer at Company A",
          avatar: "https://randomuser.me/api/portraits/women/12.jpg",
          status: "Online",
          lastSeen: "Just now"
        },
        {
          id: 2,
          name: "Bob Smith",
          title: "Product Manager at Company B",
          avatar: "https://randomuser.me/api/portraits/men/32.jpg",
          status: "Offline",
          lastSeen: "2 hours ago"
        },
        {
          id: 3,
          name: "Carol Williams",
          title: "UX Designer at Company C",
          avatar: "https://randomuser.me/api/portraits/women/45.jpg",
          status: "Away",
          lastSeen: "Yesterday"
        },
        // Add more friends as needed
      ]
    };
  },
  methods: {
    acceptRequest(id) {
      // Logic to accept the friend request
      console.log(`Accepted request from user with ID ${id}`);
      // Remove from friendRequests or add to friends list as needed
    },
    declineRequest(id) {
      // Logic to decline the friend request
      console.log(`Declined request from user with ID ${id}`);
      // Remove from friendRequests as needed
    }
  }
};
</script>

<style scoped>
.container {
  max-width: 1200px; /* Optional max width */
}
</style>