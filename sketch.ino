#define BLYNK_TEMPLATE_ID "TMPL6o7AjDhm7"
#define BLYNK_TEMPLATE_NAME "GasSafe"
#define BLYNK_AUTH_TOKEN "xlPqvWlxynIFrmz5nVF5rqwFWwRiUOCE"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// WiFi
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

// Pin Konfigurasi
#define GAS_PIN 34      
#define LED_PIN 2        
#define RELAY_PIN 23     
#define BUZZER_PIN 5    


bool sistemAktif = false;

BlynkTimer timer;

void setup() {
  Serial.begin(115200);

  pinMode(GAS_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(RELAY_PIN, HIGH); // relay ON
  digitalWrite(BUZZER_PIN, LOW);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(1000L, cekGas);
}

BLYNK_WRITE(V1) {
  sistemAktif = param.asInt();

  if (sistemAktif) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Sistem AKTIF");
  } else {
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Sistem NON-AKTIF");
  }
}

void cekGas() {
  if (!sistemAktif) return;

  int gasValue = analogRead(GAS_PIN);
  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  Blynk.virtualWrite(V0, gasValue);

  if (gasValue > 900) {
    digitalWrite(RELAY_PIN, HIGH);
    alarmOn(); // gunakan fungsi
    Blynk.logEvent("gas_detected", "Gas berbahaya terdeteksi di dapur!");
    Serial.println("!!! GAS BERBAHAYA TERDETEKSI !!!");
  } else {
    
    digitalWrite(RELAY_PIN, LOW);
    noTone(BUZZER_PIN);
  }
}

void alarmOn() {
  tone(BUZZER_PIN, 500);
  delay(300);
  tone(BUZZER_PIN, 800);
  delay(300);
  tone(BUZZER_PIN, 1000);
  delay(300);
}

void loop() {
  Blynk.run();
  timer.run();
}


