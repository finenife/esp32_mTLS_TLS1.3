<template>
  <div class="bg-white shadow rounded-lg p-6 mb-6">
    <h2 class="text-2xl font-bold mb-4">Force Assign</h2>
    <button 
      @click="forceAssign" 
      class="bg-red-500 hover:bg-red-600 text-white px-4 py-2 rounded"
    >
      Force Assign
    </button>
  </div>
</template>

<script>
export default {
  methods: {
    forceAssign() {
      // Implement your force assign logic here
      console.log("Force assign action triggered");
      // You can add any additional logic needed for the force assign action
    }
  }
}
</script>