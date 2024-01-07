#define BLYNK_PRINT Serial
#include <WiFiManager.h>
#include <BlynkSimpleEsp32.h>
#include <BH1750.h>
#include <Wire.h>
BH1750 luxMeter;

#define BLYNK_TEMPLATE_ID "TMPL6t_8BEKuQ"
#define BLYNK_TEMPLATE_NAME "Artificial Lighting"
#define BLYNK_AUTH_TOKEN "gKGzcwfk2xtIkbMcF-UcdKScrUhShira"

const int neon1 = 26;
const int neon2 = 25;
const int led1 = 33;
const int led2 = 32;

int pwmValue=0;
int modeNeon;
int modeLED;
// PWM channel 0 parameter
const int freq = 10000; // 5000 Hz
//const int pwmChannel = 0;
const int pwmRes = 8; // 8-bit resolution
float luxData;
unsigned long timer = 0;

void setup() {
  Serial.begin(115200);
  // Configure the channel 
  ledcSetup(0, freq, pwmRes);
  ledcSetup(1, freq, pwmRes);
  ledcSetup(2, freq, pwmRes);
  ledcSetup(3, freq, pwmRes);

  // Attach the channel 0 on the 3 pins
  ledcAttachPin(neon1, 0); //neon1 channel 0
  ledcAttachPin(neon2, 1); //neon2 channel 1
  ledcAttachPin(led1, 2); //led1 channel 2
  ledcAttachPin(led2, 3); //led2 channel 3

  WiFiManager WM;
  WM.setConnectTimeout(10);
  while (!WM.autoConnect("ArtificialLighting","password")) { //looping untuk mencoba mengkoneksikan ke ap tersimpan/ konfigurasi ulang
    Serial.println("Gagal Terhubung");
  }
  Serial.println("WiFi Terhubung");
  // put your setup code here, to run once:
  Blynk.config(BLYNK_AUTH_TOKEN, "blynk.cloud", 8080);
  while (!Blynk.connected()) {
    delay(500);
    Serial.print("Menghubungkan ulang...");
    Serial.println();
    Blynk.connect();
  }
  Wire.begin();
  luxMeter.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  luxData = luxMeter.readLightLevel();
  if(millis()-timer >= 1000){
    Blynk.virtualWrite(V0, luxData);
    Serial.print("Light: ");
    Serial.print(luxData);
    Serial.println(" lx");
    timer=millis();
  }
  if(modeNeon==5){
    ledcWrite(0, pwmValue);
    ledcWrite(1, 0);
  }
  else if(modeNeon==10){
    ledcWrite(0, pwmValue);
    ledcWrite(1, pwmValue);
  }
  else if(modeNeon==0){
    ledcWrite(0, 0);
    ledcWrite(1, 0);
  }

  if(modeLED==5){
    ledcWrite(2, pwmValue);
    ledcWrite(3, 0);
  }
  else if(modeLED==10){
    ledcWrite(2, pwmValue);
    ledcWrite(3, pwmValue);
  }
  else if(modeLED==0){
    ledcWrite(2, 0);
    ledcWrite(3, 0);
  }
}

BLYNK_WRITE(V1)
{
  modeNeon = param.asInt(); // assigning incoming value from pin V4 to a variable
  // process received value
}
BLYNK_WRITE(V2)
{
  modeLED = param.asInt(); // assigning incoming value from pin V4 to a variable
  // process received value
}
BLYNK_WRITE(V3)
{
  pwmValue = map(param.asInt(), 0, 100, 0, 255);
  // process received value
}
