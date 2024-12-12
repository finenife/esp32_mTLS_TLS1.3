<template>
  <div class="bg-white shadow rounded-lg p-6 mb-6">
    <h2 class="text-2xl font-bold mb-4">Active Players</h2>
    <table class="w-full border-collapse">
      <thead>
        <tr class="border-b">
          <th class="text-left py-2">Name</th>
          <th class="text-left py-2">Status</th>
          <th class="text-left py-2">Last Active</th>
        </tr>
      </thead>
      <tbody>
        <tr v-for="player in players" :key="player.id" class="border-b">
          <td class="py-2">{{ player.name }}</td>
          <td class="py-2">{{ player.status }}</td>
          <td class="py-2">{{ player.lastActive }}</td>
        </tr>
      </tbody>
    </table>
    <ForceAssign />
  </div>
</template>

<script>
import ForceAssign from './ForceAssign.vue'

export default {
  name: 'ActivePlayersList',
  components: {ForceAssign},
  props: {
    players: {
      type: Array,
      required: true
    }
  }
}
</script>