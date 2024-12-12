<template>
  <div class="container mx-auto px-4 py-8">
    <h1 class="text-3xl font-bold mb-8">Admin Dashboard</h1>

     <ActivePlayersList :players="activePlayers" />

     <DeviceStatus :devices="devices" />

    <DeviceProvisioning 
      :certificateComponents="certificateComponents" 
      :intermediateCertificates="intermediateCertificates" 
      :signingAlgorithms="signingAlgorithms"
    />

    <ServerStatus :serverFeatures="serverFeatures" />
  </div>
</template>

<script>
import ActivePlayersList from './adminpagecomps/ActivePlayersList.vue';
import DeviceProvisioning from './adminpagecomps/DeviceProvisioning.vue';
import DeviceStatus from './adminpagecomps/DeviceStatus.vue';
import ServerStatus from './adminpagecomps/ServerStatus.vue';


export default {
    components: {
        ActivePlayersList, 
        DeviceStatus, 
        DeviceProvisioning, 
        ServerStatus
    },
  data() {
    return {
      // Active Players Data
      activePlayers: [
        { id: 1, name: "Alice Johnson", status: "Online", lastActive: "5 minutes ago" },
        { id: 2, name: "Bob Smith", status: "Offline", lastActive: "1 hour ago" },
        { id: 3, name: "Carol Williams", status: "Online", lastActive: "10 minutes ago" }
      ],

      // Devices Data
      devices: [
        { id: 1, name: "Device A", registrationStatus: "Registered", online: true },
        { id: 2, name: "Device B", registrationStatus: "Pending", online: false },
        { id: 3, name: "Device C", registrationStatus: "Registered", online: true }
      ],

      certificateComponents: {
        deviceId: "device12345", // Example Device ID
        publicKey: "ABCD1234...", // Example Public Key
        issuerInfo: "CA Authority", // Example Issuer Information
        validityPeriod: "2023-12-01 to 2024-12-01" // Example Validity Period
      },
      
      // Certificate URL for download
     
      //selectedCertificate: null,
      //selectedAlgorithm: null,
      showCertDropdown: false,
      showAlgoDropdown: false,
      intermediateCertificates: [
        { id: 1, name: 'Cert A' },
        { id: 2, name: 'Cert B' },
        { id: 3, name: 'Cert C' },
      ],
      signingAlgorithms: ['RSA', 'ECDSA', 'Ed25519'],

      // Server Features Data
      serverFeatures: [
        { name: "Authentication Service", status: "online" },
        { name: "Database Service", status: "online" },
        { name: "API Gateway", status: "offline" },
        { name: "Notification Service", status: "online" }
      ]
    };
  },
  methods: {
    toggleCertDropdown() {
      this.showCertDropdown = !this.showCertDropdown;
      this.showAlgoDropdown = false;
    },
    toggleAlgoDropdown() {
      this.showAlgoDropdown = !this.showAlgoDropdown;
      this.showCertDropdown = false;
    },
    selectCertificate(cert) {
      this.selectedCertificate = cert;
      this.showCertDropdown = false;
    },
    selectAlgorithm(algo) {
      this.selectedAlgorithm = algo;
      this.showAlgoDropdown = false;
    },
  }
};
</script>

<style scoped>
.container {
  max-width: 1200px;
}
</style>
