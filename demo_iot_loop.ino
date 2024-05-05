#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Redmi 9";
const char* password = "12345678";
const char* mqtt_server = "103.167.112.188";

WiFiClient espClient;
PubSubClient client(espClient);
#define MSG_BUFFER_SIZE	(150)
char msg[MSG_BUFFER_SIZE];

// Fungsi Untuk Menghubungkan ke Wifi
void setup_wifi() {
  delay(10);
  // Memulai untuk menghubungkan ke Wifi
  Serial.println();
  Serial.print("Menghubungkan ke ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi terhubung");
  Serial.println("Alamat IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Jalankan perulangan pada fungsi ini jika Gagal terhubung ke RabbitMQ
  while (!client.connected()) {
    Serial.print("Menguhubungkan kembali MQTT ...");

    // Buat Random ClientId untuk RabbitMQ
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    // Hubungkan kredensial untuk terhubung ke mqtt
    if (client.connect(clientId.c_str(), "/iot:kelas_edp", "kelas_edp")) {
      Serial.println("terhubung");
      // Jika terhubung coba publish satu pesan ke mqtt
      client.publish("routing_edp", "Init");
      // ... disini jika tanpa web quasar bisa di subsribe langsung.
      // client.subscribe("routing_edp");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" mencoba kembali dalam 5 detik");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Loopong publish ke RabbitMQ
  for (int i = 1; i <= 20; i++) {
    snprintf(msg, MSG_BUFFER_SIZE, "Perulangan Ke: %ld", i);
    Serial.println(msg);
    client.publish("routing_edp", msg);
    delay(1000);
  }
}
