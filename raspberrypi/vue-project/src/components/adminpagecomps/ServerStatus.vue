<template>  

<!-- Server Status -->
    <div class="bg-white shadow rounded-lg p-6">
      <h2 class="text-2xl font-bold mb-4">Server Status</h2>
      <div class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-4 gap-4">
        <div v-for="feature in serverFeatures" :key="feature.name" class="border p-4 rounded">
          <h3 class="font-semibold">{{ feature.name }}</h3>
          <div class="flex items-center mt-2">
            <span 
              :class="[feature.status === 'online' ? 'bg-green-500' : 'bg-red-500']"
              class="w-3 h-3 rounded-full mr-2"
            ></span>
            {{ feature.status }}
          </div>
        </div>
      </div>
    </div>
</template>

<script>
export default {
    props:{
      serverFeatures: {
      type: Array,
      required: true,
    },
    }
}
</script>