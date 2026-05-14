#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ECG pins
int ecgPin = A0;
int LOplus = 10;
int LOminus = 11;

// LEDs
#define GREEN_LED 4
#define YELLOW_LED 5
#define RED_LED 6

// GSM
SoftwareSerial gsmSerial(8, 7);

// ECG processing
int rawValue = 0;
int filteredValue = 0;
int threshold = 520;

unsigned long lastBeat = 0;
int bpm = 0;

// BPM averaging
#define BPM_SAMPLES 5
int bpmBuffer[BPM_SAMPLES];
int indexBPM = 0;

// Peak detection
bool peakDetected = false;
unsigned long debounceTime = 250;

String lastStress = "";

// ---------- GSM ----------
void sendSMS(String msg) {
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  gsmSerial.println("AT+CMGS=\"+917708459352\"");
  delay(1000);
  gsmSerial.print(msg);
  delay(500);
  gsmSerial.write(26);
  delay(5000);
}

// ---------- SETUP ----------
void setup() {
  Serial.begin(9600);
  gsmSerial.begin(9600);

  pinMode(LOplus, INPUT);
  pinMode(LOminus, INPUT);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  // Initialize BPM buffer
  for (int i = 0; i < BPM_SAMPLES; i++) {
    bpmBuffer[i] = 0;
  }
}

// ---------- LOOP ----------
void loop() {

  rawValue = analogRead(ecgPin);

  // Check electrode connection
  if (digitalRead(LOplus) == 1 || digitalRead(LOminus) == 1) {
    Serial.println(0);
    return;
  }

  // -------- SIGNAL FILTER (Moving Average) --------
  static int prev1 = 0, prev2 = 0;
  filteredValue = (rawValue + prev1 + prev2) / 3;
  prev2 = prev1;
  prev1 = rawValue;

  // -------- PEAK DETECTION --------
  unsigned long currentTime = millis();

  if (filteredValue > threshold && !peakDetected &&
      (currentTime - lastBeat > debounceTime)) {

    peakDetected = true;

    if (lastBeat != 0) {
      int newBPM = 60000 / (currentTime - lastBeat);

      // Store in buffer
      bpmBuffer[indexBPM] = newBPM;
      indexBPM = (indexBPM + 1) % BPM_SAMPLES;

      // Average BPM
      int sum = 0;
      for (int i = 0; i < BPM_SAMPLES; i++) {
        sum += bpmBuffer[i];
      }
      bpm = sum / BPM_SAMPLES;
    }

    lastBeat = currentTime;
  }

  if (filteredValue < threshold) {
    peakDetected = false;
  }

  // -------- STRESS CLASSIFICATION --------
  String stress;

  if (bpm < 60 && bpm > 0) {
    stress = "LOW";
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
  }
  else if (bpm <= 100) {
    stress = "MEDIUM";
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  }
  else {
    stress = "HIGH";
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);

    if (lastStress != "HIGH") {
      sendSMS("ALERT! High Stress\nBPM: " + String(bpm));
    }
  }

  lastStress = stress;

  // -------- OLED DISPLAY --------
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("ECG Monitor (PRO)");

  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print("BPM:");
  display.println(bpm);

  display.setTextSize(1);
  display.setCursor(0, 50);
  display.print("Stress:");
  display.println(stress);

  display.display();

  // -------- SERIAL PLOTTER --------
  Serial.print(filteredValue);
  Serial.print(",");
  Serial.println(bpm);

  delay(5);
}
