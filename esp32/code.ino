#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <HTTPClient.h>

#define button 3
#define redLed 41
#define greenLed 42
#define buzzer 18
#define pir 17


int sistemState = false;
int tonebuzzer, i;
int buttonAnterior = 0;


const char* ssid = "Wokwi-GUEST";
const char* password = "";

#define BOT_TOKEN "9230251904:AAHalE3FgSUe701ZM8P8A56JmPehzJB7I-U"
#define CHAT_ID "-1004364203934"

const char* FIREBASE_URL =
"https://alarme-c1a35-default-rtdb.firebaseio.com/sistema";

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);


void setup() {
  pinMode(button, INPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(pir, INPUT);



  Serial.begin(115200);

  WiFi.begin(ssid, password, 6);

  Serial.print("Conectando ao Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado!");

  client.setInsecure();

  bot.sendMessage(CHAT_ID, "Sistema de alarme conectado!", "");

}

void loop() {
  static unsigned long ultimaLeitura = 0;

  if (millis() - ultimaLeitura >= 1000) {
    sistemState = lerArmado();
    ultimaLeitura = millis();
  }


  int buttonValue = digitalRead(button);
  int pirValue = digitalRead(pir);
  

  if (buttonValue == 1 && buttonAnterior == 0) {
    sistemState  = !sistemState;
    atualizarAtivo(sistemState);
    delay(10);
  }
  buttonAnterior = buttonValue;


  if (sistemState == true) {
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
  

    if (pirValue == 1) {
      enviarTelegram("Movimento detectado!");
      atualizarMovimento(true);
      tocarBuzzer();
  

    }

  } else {
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    
  }


  delay(100);
}

void tocarBuzzer() {
  i = 0;
  tonebuzzer  = 40;
  while (i < 10) {
    while (tonebuzzer < 801) {
      tone(buzzer, tonebuzzer);
      Serial.println(tonebuzzer);
      tonebuzzer = tonebuzzer + 10;
      delay(10);
      noTone(buzzer);
    }
    while (tonebuzzer > 40) {
      tone(buzzer, tonebuzzer);
      Serial.println(tonebuzzer);
      tonebuzzer = tonebuzzer - 10;
      delay(10);
      noTone(buzzer);
    }
    tone(buzzer, 760, 100);
    tone(buzzer, 300, 100);
    tone(buzzer, 760, 100);
    tone(buzzer, 300, 100);
    tone(buzzer, 760, 100);
    tone(buzzer, 300, 100);

    i = i + 1;
  }
}





void enviarTelegram(String mensagem) {
  if (WiFi.status() == WL_CONNECTED) {
    bool enviado = bot.sendMessage(CHAT_ID, mensagem, "");

    if (enviado) {
      Serial.println("Mensagem enviada ao Telegram!");
    } else {
      Serial.println("Erro ao enviar mensagem!");
    }
  }
}



void atualizarMovimento(bool movimento){
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(client,String(FIREBASE_URL) + "/movimento.json");
    http.addHeader("Content-Type", "application/json");

    if(movimento){
      http.PUT("true");
    }else{
      http.PUT("false");
    }

    http.end();
  }

}

void atualizarAtivo(bool ativo){
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(client,String(FIREBASE_URL) + "/ativo.json");
    http.addHeader("Content-Type", "application/json");

    if(ativo){
      http.PUT("true");
    }else{
      http.PUT("false");
    }

    http.end();
  }

}

bool lerArmado() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(client, String(FIREBASE_URL) + "/ativo.json");

    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
      String resposta = http.getString();
      http.end();

      resposta.trim();

      return resposta == "true";
    }

    Serial.println("Erro ao ler Firebase: " + String(httpCode));
    http.end();
  }

  return sistemState;
}