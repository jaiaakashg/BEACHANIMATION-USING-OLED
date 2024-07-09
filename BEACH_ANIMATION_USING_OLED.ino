#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET    -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


int wave1Offset = 0;
int wave2Offset = 0;

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.display();
}

void loop() {
  simulateBeachMorning();
  delay(30); 
}

void simulateBeachMorning() {
  static int sunY = SCREEN_HEIGHT; 
  static int sunRadius = 10;
  static int personX = SCREEN_WIDTH - 30; 
  static int personY = SCREEN_HEIGHT - 20;
  display.clearDisplay();
  drawWaves();
  drawRisingSun(sunY, sunRadius);
  drawPerson(personX, personY);
  display.display();

  if (sunY > SCREEN_HEIGHT / 2) {
    sunY--;
  }

  
  wave1Offset += 2;
  wave2Offset += 1;
  if (wave1Offset >= SCREEN_WIDTH) wave1Offset = 0;
  if (wave2Offset >= SCREEN_WIDTH) wave2Offset = 0;
}

void drawWaves() {
  for (int i = 0; i < SCREEN_WIDTH; i += 2) {
    int x = (i + wave1Offset) % SCREEN_WIDTH;
    int y = SCREEN_HEIGHT - 10 + sin((i + wave1Offset) * 0.1) * 4;
    display.drawPixel(x, y, SSD1306_WHITE);
  }
  for (int i = 0; i < SCREEN_WIDTH; i += 2) {
    int x = (i + wave2Offset) % SCREEN_WIDTH;
    int y = SCREEN_HEIGHT - 15 + sin((i + wave2Offset) * 0.1) * 4;
    display.drawPixel(x, y, SSD1306_WHITE);
  }
  for (int i = 0; i < SCREEN_WIDTH; i += 2) {
    int x = (i + static_cast<int>(wave2Offset * 1.5)) % SCREEN_WIDTH;
    int y = SCREEN_HEIGHT - 20 + sin((i + static_cast<int>(wave2Offset * 1.5)) * 0.1) * 4;
    display.drawPixel(x, y, SSD1306_WHITE);
  }
}

void drawRisingSun(int y, int radius) {
  int sunX = SCREEN_WIDTH / 2;
  for (int r = 0; r < radius; r++) {
    int brightness = map(r, 0, radius, 0, 255);
    if (brightness > 127) {
      display.drawCircle(sunX, y, r, SSD1306_WHITE);
    } else if (brightness > 64) {
      display.drawCircle(sunX, y, r, SSD1306_WHITE * ((brightness % 2) == 0));
    } else {
      display.drawCircle(sunX, y, r, SSD1306_WHITE * ((brightness % 4) == 0));
    }
  }
  for (int angle = 0; angle < 360; angle += 30) {
    int rayLength = radius + 5;
    int x1 = sunX + radius * cos(radians(angle));
    int y1 = y + radius * sin(radians(angle));
    int x2 = sunX + rayLength * cos(radians(angle));
    int y2 = y + rayLength * sin(radians(angle));
    display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
  }
}

void drawPerson(int x, int y) {
  display.fillCircle(x, y - 10, 3, SSD1306_WHITE);
  display.drawLine(x, y - 7, x, y + 2, SSD1306_WHITE);
  display.drawLine(x, y - 5, x - 5, y - 2, SSD1306_WHITE);
  display.drawLine(x, y - 5, x + 5, y - 2, SSD1306_WHITE);
  display.drawLine(x, y + 2, x - 2, y + 10, SSD1306_WHITE);
  display.drawLine(x, y + 2, x + 2, y + 10, SSD1306_WHITE);
  display.drawLine(x - 3, y - 13, x + 3, y - 13, SSD1306_WHITE); 
  display.drawLine(x - 1, y - 13, x - 1, y - 16, SSD1306_WHITE); 
  display.drawLine(x + 1, y - 13, x + 1, y - 16, SSD1306_WHITE);
  display.drawLine(x - 1, y - 16, x + 1, y - 16, SSD1306_WHITE); 
}

