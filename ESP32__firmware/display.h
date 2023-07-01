#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define WIDTH 128
#define HEIGHT 64
#define BAR_HEIGHT 16
#define I2C_MUX_ADDRESS 0x70
#define DISPLAY_I2C_ADDRESS 0x3C


// global variables
Adafruit_SSD1306 display(WIDTH, HEIGHT, &Wire, -1);
bool wireInitialized = false;


// Class for each display. Make sure to call setup_I2C before declaring any of this class
class Display {
  private:
    int port;  // Port on I2C Mux
    const char* text; // text to display
    int barWidth; // Width of bar

    void initiate_mux_port(uint8_t bus){
      Wire.beginTransmission(I2C_MUX_ADDRESS);  // TCA9548A address
      Wire.write(1 << bus);          // send byte to select bus
      Wire.endTransmission();
    }

  public:
    Display(int port, const char* text) : port(port), text(text){
      
      // select port
      initiate_mux_port(port);
      
      // begin the display
      if(!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
      }

      // reset the display
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
    } 

    void update(int value){

      // Connect to the display
      initiate_mux_port(port);

      // Reset
      display.clearDisplay();

      // Display Text
      display.setCursor(0, 0);
      display.println(text);

      // Display Bar
      int potVal = value;
      barWidth = map(potVal, 0, 4095, 0, WIDTH);
      display.fillRect(0, HEIGHT - BAR_HEIGHT, barWidth, BAR_HEIGHT, SSD1306_WHITE);
      
      // Update changes
      display.display();
    }
};
