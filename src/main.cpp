#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// SSD1306設定
#define SDA_PIN 25
#define SCL_PIN 24
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// I2Cスキャナ関数
void scanI2C() {
  Serial.println("\n=== I2C Scanner ===");
  byte count = 0;
  
  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    uint8_t error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("Found device at 0x");
      if (addr < 16) Serial.print("0");
      Serial.print(addr, HEX);
      Serial.println(" !");
      count++;
    } else if (error == 4) {
      Serial.print("Unknown error at 0x");
      if (addr < 16) Serial.print("0");
      Serial.println(addr, HEX);
    }
  }
  
  if (count == 0) {
    Serial.println("No I2C devices found");
  } else {
    Serial.print("Found ");
    Serial.print(count);
    Serial.println(" device(s)");
  }
  Serial.println("==================\n");
}

// SSD1306テスト描画
void testDisplay() {
  // テスト1: 画面全体を白にして表示
  Serial.println("Test 1: Fill screen white");
  display.clearDisplay();
  display.fillScreen(SSD1306_WHITE);
  display.display();
  delay(1000);
  
  // テスト2: 画面クリア
  Serial.println("Test 2: Clear screen");
  display.clearDisplay();
  display.display();
  delay(500);
  
  // テスト3: テキスト表示
  Serial.println("Test 3: Display text");
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("SSD1306 Test");
  display.println("I2C Address: 0x3C");
  display.println("");
  display.println("SDA: D25 (Pin 25)");
  display.println("SCL: D24 (Pin 24)");
  display.display();
  delay(2000);
  
  // テスト4: 図形描画
  Serial.println("Test 4: Draw shapes");
  display.clearDisplay();
  display.drawRect(10, 10, 40, 30, SSD1306_WHITE);
  display.fillRect(60, 10, 40, 30, SSD1306_WHITE);
  display.drawCircle(30, 50, 10, SSD1306_WHITE);
  display.fillCircle(80, 50, 10, SSD1306_WHITE);
  display.display();
  delay(2000);
  
  // テスト5: ライン描画
  Serial.println("Test 5: Draw lines");
  display.clearDisplay();
  for (int16_t i = 0; i < SCREEN_WIDTH; i += 4) {
    display.drawLine(0, 0, i, SCREEN_HEIGHT - 1, SSD1306_WHITE);
  }
  for (int16_t i = 0; i < SCREEN_HEIGHT; i += 4) {
    display.drawLine(0, 0, SCREEN_WIDTH - 1, i, SSD1306_WHITE);
  }
  display.display();
  delay(2000);
  
  // テスト6: アニメーション
  Serial.println("Test 6: Animation");
  for (int i = 0; i < 3; i++) {
    for (int x = 0; x < SCREEN_WIDTH - 16; x += 4) {
      display.clearDisplay();
      display.fillRect(x, 24, 16, 16, SSD1306_WHITE);
      display.display();
      delay(20);
    }
  }
  
  Serial.println("All tests completed!\n");
}

void setup() {
  // シリアル初期化
  Serial.begin(115200);
  while (!Serial && millis() < 3000) { delay(10); }
  Serial.println("\n\n=== SSD1306 Test Start ===");
  
  // LED設定（エラー表示用）
  pinMode(LED_BUILTIN, OUTPUT);
  
  // I2C初期化（SDA=D25, SCL=D24）
  Wire.begin();
  Wire.setSDA(SDA_PIN);
  Wire.setSCL(SCL_PIN);
  Wire.setClock(400000); // 400kHz
  
  Serial.println("I2C initialized");
  Serial.print("SDA: D");
  Serial.println(SDA_PIN);
  Serial.print("SCL: D");
  Serial.println(SCL_PIN);
  
  delay(100);
  
  // I2Cスキャン実行
  scanI2C();
  
  // SSD1306初期化
  Serial.println("Initializing SSD1306...");
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("ERROR: SSD1306 initialization failed!");
    Serial.println("Check connections:");
    Serial.println("  - VCC to 3.3V or 5V");
    Serial.println("  - GND to GND");
    Serial.println("  - SDA to D25 (Pin 25)");
    Serial.println("  - SCL to D24 (Pin 24)");
    Serial.println("  - Pull-up resistors (4.7k) if needed");
    
    // エラーLED点滅
    while (true) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
  }
  
  Serial.println("SSD1306 initialized successfully!");
  
  // 初期表示
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println("SSD1306");
  display.setCursor(10, 40);
  display.println("  OK!");
  display.display();
  delay(2000);
  
  // テスト実行
  testDisplay();
  
  Serial.println("Setup complete. Entering loop...\n");
}

void loop() {
  // カウンタ表示（動作確認用）
  static unsigned long lastUpdate = 0;
  static int counter = 0;
  
  if (millis() - lastUpdate > 1000) {
    lastUpdate = millis();
    counter++;
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("SSD1306 Running");
    display.println("");
    display.print("Counter: ");
    display.println(counter);
    display.println("");
    display.print("Uptime: ");
    display.print(millis() / 1000);
    display.println(" sec");
    display.display();
    
    Serial.print("Counter: ");
    Serial.print(counter);
    Serial.print(" | Uptime: ");
    Serial.print(millis() / 1000);
    Serial.println(" sec");
  }
}
