<template>
  <div class="bg-white shadow rounded-lg p-6 mb-6">
    <h2 class="text-2xl font-bold mb-4">Device Statuses</h2>
    <div class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4">
      <div v-for="device in devices" :key="device.id" class="border p-4 rounded">
        <h3 class="font-semibold">{{ device.name }}</h3>
        <p>Registration Status: {{ device.registrationStatus }}</p>
        <p>Online: 
          <span :class="device.online ? 'text-green-500' : 'text-red-500'">
            {{ device.online ? 'Yes' : 'No' }}
          </span>
        </p>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'DeviceStatus',
  props: {
    devices: {
      type: Array,
      required: true,
    },
  },
};
</script>