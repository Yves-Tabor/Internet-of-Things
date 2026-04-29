








#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

const char* ssid = "wifi";
const char* password = "password";

#define BOT_TOKEN "API_Key"
#define CHAT_ID "000000000"

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

#define LED 4

void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n Connected to WiFi");

  client.setInsecure();

  bot.sendMessage(CHAT_ID, "Bot is online! Use /on or /off", "");
}

void loop() {
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

  while (numNewMessages) {
    handleMessages(numNewMessages);
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }

  delay(1000);
}

void handleMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    
    String text = bot.messages[i].text;
    String chat_id = bot.messages[i].chat_id;

    if (chat_id != CHAT_ID) continue;

    if (text == "/on") {
      digitalWrite(LED, HIGH);
      bot.sendMessage(CHAT_ID, "LED ON", "");
    }

    if (text == "/off") {
      digitalWrite(LED, LOW);
      bot.sendMessage(CHAT_ID, "LED OFF", "");
    }
  }
}
