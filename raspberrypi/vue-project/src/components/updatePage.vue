<template>
  <div class="container mx-auto px-4 py-8">
    <h1 class="text-3xl font-bold mb-6">Software Update</h1>

    <!-- Update Status Section -->
    <div class="bg-gray-100 rounded-lg p-4 mb-6">
      <h2 class="text-xl font-semibold mb-4">Update Status</h2>
      <p v-if="updateAvailable" class="text-green-600">
        A new update (Version {{ latestVersion }}) is available!
      </p>
      <p v-else class="text-red-600">
        Your software is up to date (Version {{ currentVersion }}).
      </p>
    </div>

    <!-- Actions Section -->
    <div class="flex space-x-4 mb-6">
      <button 
        v-if="updateAvailable" 
        @click="applyUpdate" 
        class="bg-blue-600 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded"
      >
        Apply Update
      </button>
      <button 
        @click="showReleaseNotes" 
        class="bg-gray-600 hover:bg-gray-700 text-white font-bold py-2 px-4 rounded"
      >
        View Release Notes
      </button>
    </div>

    <!-- Release Notes Modal -->
    <div v-if="showNotes" class="fixed inset-0 flex items-center justify-center z-50">
      <div class="bg-white rounded-lg shadow-lg p-6 w-full max-w-md">
        <h3 class="text-xl font-semibold mb-4">Release Notes</h3>
        <ul class="list-disc list-inside mb-4">
          <li v-for="note in releaseNotes" :key="note">{{ note }}</li>
        </ul>
        <button @click="showNotes = false" class="bg-red-600 hover:bg-red-700 text-white font-bold py-2 px-4 rounded">
          Close
        </button>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  data() {
    return {
      currentVersion: '1.0.0',
      latestVersion: '1.1.0',
      updateAvailable: true,
      showNotes: false,
      releaseNotes: [
        "Added new feature X.",
        "Improved performance of Y.",
        "Fixed bug Z.",
        "Updated user interface for better usability."
      ]
    };
  },
  methods: {
    applyUpdate() {
      // Logic to apply the update
      console.log('Applying update...');
      // Simulate update process
      this.currentVersion = this.latestVersion;
      this.updateAvailable = false;
    },
    showReleaseNotes() {
      this.showNotes = true;
    }
  }
};
</script>

<style scoped>
.container {
  max-width: 800px; /* Optional max width */
}
</style>
