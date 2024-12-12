<template>
  <div class="bg-white shadow rounded-lg p-6 mb-6">
    <h2 class="text-2xl font-bold mb-4">Device Provisioning</h2>
    <!-- Certificate Components -->
    <div class="grid grid-cols-1 md:grid-cols-2 gap-4 mb-4">
      <!-- Device ID -->
      <div class="border p-4 rounded">
        <h3 class="font-semibold mb-2">Device ID</h3>
        <input type="text" v-model="certificateComponents.deviceId" placeholder="Enter Device ID" class="w-full border rounded p-2" />
      </div>
      <!-- Public Key -->
      <div class="border p-4 rounded">
        <h3 class="font-semibold mb-2">Public Key</h3>
        <input type="text" v-model="certificateComponents.publicKey" placeholder="Enter Public Key" class="w-full border rounded p-2" />
      </div>
      <!-- Issuer Information -->
      <div class="border p-4 rounded">
        <h3 class="font-semibold mb-2">Issuer Information</h3>
        <input type="text" v-model="certificateComponents.issuerInfo" placeholder="Enter Issuer Info" class="w-full border rounded p-2" />
      </div>
      <!-- Validity Period -->
      <div class="border p-4 rounded">
        <h3 class="font-semibold mb-2">Validity Period</h3>
        <input type="text" v-model="certificateComponents.validityPeriod" placeholder="Enter Validity Period" class="w-full border rounded p-2" />
      </div>
    </div>

    <!-- Intermediate Certificate Dropdown -->
    <div class="border p-4 rounded relative">
      <h3 class="font-semibold mb-2">Intermediate Certificate</h3>
      <div class="relative">
        <button @click="toggleCertDropdown" class="w-full bg-white border border-gray-300 rounded-md shadow-sm px-4 py-2 inline-flex justify-between items-center text-sm font-medium text-gray-700 hover:bg-gray-50 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-blue-500">
          {{ selectedCertificate ? selectedCertificate.name : 'Select Certificate' }}
          <svg class="h-5 w-5 text-gray-400" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 20 20" fill="currentColor" aria-hidden="true">
            <path fill-rule="evenodd" d="M5.293 7.293a1 1 0 011.414 0L10 10.586l3.293-3.293a1 1 0 111.414 1.414l-4 4a1 1 0 01-1.414 0l-4-4a1 1 0 010-1.414z" clip-rule="evenodd"/>
          </svg>
        </button>
        <div v-if="showCertDropdown" class="origin-top-right absolute right-0 mt-2 w-full rounded-md shadow-lg bg-white ring=1 ring-black ring-opacity-5 focus:outline-none z-20">
          <div class='py-1' role='menu' aria-orientation='vertical' aria-labelledby='options-menu'>
            <button v-for='cert in intermediateCertificates' :key='cert.id' @click.prevent='selectCertificate(cert)' class='block w-full text-left px-4 py-2 text-sm text-gray700 hover:bg-gray100 hover:text-gray900' role='menuitem'>
              {{ cert.name }}
            </button>
          </div>
        </div>
      </div>
    </div>

    <!-- Signing Algorithm Dropdown -->
    <div class="border p-4 rounded relative">
      <h3 class='font-semibold mb-2'>Signing Algorithm</h3>
      <div class='relative'>
        <button @click='toggleAlgoDropdown' class='w-full bg-white border border-gray-300 rounded-md shadow-sm px-4 py-2 inline-flex justify-between items-center text-sm font-medium text-gray-700 hover:bg-gray-50 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-blue-500'>
          {{ selectedAlgorithm || 'Select Algorithm' }}
          <svg class="h-5 w-5 text-gray-400" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 20 20" fill="currentColor" aria-hidden="true">
            <path fill-rule="evenodd" d="M5.293 7.293a1 1 0 011.414 0L10 10.586l3.293-3.293a1 1 0 111.414 1.414l-4 4a1 1 0 01-1.414 0l-4-4a1 1 0 010-1.414z" clip-rule="evenodd"/>
          </svg>
        </button>
        <div v-if='showAlgoDropdown' class='origin-top-right absolute right-0 mt-2 w-full rounded-md shadow-lg bg-white ring-1 ring-black ring-opacity-5 focus:outline-none z-20'>
          <div class='py-11' role='menu' aria-orientation='vertical' aria-labelledby='options-menu'>
            <button v-for='algo in signingAlgorithms' :key='algo' @click.prevent='selectAlgorithm(algo)' class='block w-full text-left px-4 py-2 text-sm text-gray700 hover:bg-gray100 hover:text-gray900' role='menuitem'>
              {{ algo }}
            </button>
          </div>
        </div>
      </div>
    </div>

    <!-- Generate Certificate Button -->
    <button @click='generateCertificate' class="bg-blue-500 hover:bg-blue-600 text-white px-4 mt-4 py-2 rounded">Generate New Certificate</button>

    <!-- Download Certificate Link -->
    <div v-if="certificateUrl" class="mt-4">
        <a 
          :href="certificateUrl" 
          download 
          class="text-blue-500 underline"
        >
          Download Certificate
        </a>
      </div>
 </div>
</template>

<script>
export default {
   name: 'DeviceProvisioning',
   props: {
     certificateComponents: {
       type: Object,
       required: true,
     },
     intermediateCertificates: {
       type: Array,
       required: true,
     },
     signingAlgorithms: {
       type: Array,
       required: true,
     }
   },
   data() {
     return {
       selectedCertificate: null,
       selectedAlgorithm: null,
       showCertDropdown: false,
       showAlgoDropdown: false,
       certificateUrl: null,
     };
   },
   methods: {
     toggleCertDropdown() {
       this.showCertDropdown = !this.showCertDropdown;
       this.showAlgoDropdown = false; // Close other dropdowns if open
     },
     toggleAlgoDropdown() {
         console.log("Algo toggle");
       this.showAlgoDropdown = !this.showAlgoDropdown;
       this.showCertDropdown = false; // Close other dropdowns if open
     },
     selectCertificate(cert) {
        console.log("You chose this cert:", cert);
       this.selectedCertificate = cert;
       this.showCertDropdown = false; // Close dropdown after selection
     },
     selectAlgorithm(algo) {
        console.log("You chose this algo:", algo);
       this.selectedAlgorithm = algo;
       this.showAlgoDropdown = false; // Close dropdown after selection
     },
     generateCertificate() {
    // Check if selectedCertificate is null before accessing its properties
    if (this.selectedCertificate) {
      console.log("Generating certificate with the following details:", this.certificateComponents, this.selectedCertificate.name, this.selectedAlgorithm);
    } else {
      console.error("No certificate selected. Please select a certificate before generating.");
      return; // Exit the method if no certificate is selected
    }

    // Simulate providing a download link for the generated certificate
    this.certificateUrl = "/path/to/certificate.pem"; // Replace with actual logic
    }
   }
};
</script>

<style scoped>
/* Add any specific styles for the Device Provisioning component here */
</style>
