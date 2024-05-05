<template>
  <q-page class="flex flex-center">
    <q-input v-model="monitor" filled type="textarea" rows="20" style="width: 500px" readonly />
  </q-page>
</template>

<script>
import mqtt from 'mqtt'
import { mqttConfig, urlMQTT } from 'src/lib/mqtt'
import { defineComponent, onMounted, ref } from 'vue'

export default defineComponent({
  name: 'IndexPage',
  setup() {
    // inital variabel client mqtt
    const client = mqtt.connect(urlMQTT, mqttConfig);

    // initial variable monitor
    const monitor = ref('')

    onMounted(() => {
      client.on('connect', () => {
        monitor.value = 'Berhasil terhubung ke RabbitMQ\n'
      })

      client.subscribe("routing_edp", (err) => {
        if (!err) { }
      })

      client.on("message", (topic, payload) => {
        monitor.value += `${payload.toString()}\n`
      })
    })

    return {
      monitor,
      client
    }
  }
})
</script>
