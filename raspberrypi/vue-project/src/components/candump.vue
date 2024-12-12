<template>
  <div class="py-6">
    <div class="max-w-7xl mx-auto px-4 sm:px-6 md:px-8">
      <h1 class="text-2xl font-semibold text-gray-900">Device Information</h1>
    </div>
    <div class="max-w-7xl mx-auto px-4 sm:px-6 md:px-8">
      <!-- Key Metrics -->
      <div class="mt-4 grid grid-cols-1 gap-5 sm:grid-cols-1 lg:grid-cols-1">
        <div class="bg-white overflow-hidden shadow rounded-lg">
          <div class="p-5">
            <div class="flex items-center">
              <div class="flex-shrink-0">
                <img src="https://camo.githubusercontent.com/2de3a05829e345f228269b48dcf6c20647c232eaa479ffe0b4da73d3630dfa12/68747470733a2f2f72616d6e2e72656164746865646f63732e696f2f656e2f6c61746573742f5f696d616765732f72616d6e5f73696d706c655f73657475702e6a7067" alt="Description of the image" width="150" height="150">
              </div>
              <div class="ml-5 w-full flex-auto flex-col">
                <div class="flex justify-between">
                <div class="w-1/2 pr-2">
                  <h3 class="text-lg font-medium text-gray-700">Device Status</h3>
                  <div class="flex items-center">
                        <!-- Green Status Circle -->
                        <div class="w-5 h-5 bg-green-500 ring-1 ring-gray-500 rounded-full mr-2"></div>
                        
                        <!-- Text Next to Circle -->
                        <span class="text-gray-700">Connection Successful</span>
                  </div>
                </div>
                <div class="items-center pr-2 text-left">
                    <div class="flex-initial justify-between items-center">
                        <h3 class=" text-lg font-medium text-gray-700">Device Information</h3>
                        <!-- Make it so that the device information populates here -->
                        <span class="text-gray-600">MAC Address: <b>1c:23:1a:67:fc</b></span>
                  </div>
                </div>
              </div>
              </div>
            </div>
          </div>
        </div>
        
        <!-- Add more devive details here here -->
          <div class="bg-black p-4 rounded-lg shadow-lg">
            <h2 class="text-xl text-white font-bold mb-4">Device Command Interface</h2>
            <div class="flex items-center">
              <span class="text-green-500 mr-2">$</span>
                <input
                  type="text"
                  v-model="inputText"
                  @keyup.enter="handleEnter"
                  class="bg-transparent text-white w-full focus:outline-none font-mono"
                  placeholder="Type your command..."
                />
            </div>
           </div>
          <div class="bg-black p-4 rounded-lg shadow-lg">
            <h2 class="text-xl text-white font-bold mb-4">Output</h2>
             <div class="bg-transparent text-white w-full focus:outline-none font-mono overflow-y-auto h-96">
              <div v-for="(message, index) in canMessages" :key="index" class="mb-1">
                {{ formatMessage(message) }}
              </div>
            </div>
          </div>
      </div>
    </div>
</div>
  
</template>

<script>
import { ref, onMounted } from 'vue';

export default {
    data() {
    return {
      inputText: ''
    }
  },
  methods: {
    handleEnter() {
      console.log('Command entered:', this.inputText)
      this.inputText = ''
    }
  },
  setup() {
    const canMessages = ref([]);

    const formatMessage = (message) => {
      return `${message.timestamp.toFixed(6)}  ${message.arbitrationId.toString(16).padStart(3, '0')}   [${message.dlc}]  ${message.data.map(byte => byte.toString(16).padStart(2, '0')).join(' ')}`;
    };

    const generateMockData = () => {
      const timestamp = Date.now() / 1000;
      const arbitrationId = Math.floor(Math.random() * 2048); // 11-bit identifier
      const dlc = Math.floor(Math.random() * 9); // 0-8 bytes of data
      const data = Array.from({length: dlc}, () => Math.floor(Math.random() * 256));

      return {
        timestamp,
        arbitrationId,
        dlc,
        data
      };
    };

    const addMockMessage = () => {
      const newMessage = generateMockData();
      canMessages.value.unshift(newMessage);
      if (canMessages.value.length > 100) {
        canMessages.value.pop();
      }
    };

    onMounted(() => {
      // Generate initial mock data
      for (let i = 0; i < 20; i++) {
        addMockMessage();
      }

      // Simulate incoming messages every second
      setInterval(addMockMessage, 250);
    });

    return { canMessages, formatMessage };
  }
}
</script>