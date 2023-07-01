#include <vector>
#include "potentiometer.h"
#include "button.h"
#include "python_interface.h"
#include "display.h"

#define BAUD_RATE 9600


//global variables
int num_of_pots;
int num_of_buttons;
StaticJsonDocument<MAX_CONFIG_SIZE> config;
std::vector<Pot> pots;
std::vector<Display> displays;
std::vector<Button> buttons;


void setup() {
  // Setup coms
  Serial.begin(BAUD_RATE);
  
  // ask for config untill it is recived
  do {
    config = getConfig();   
  } while(config.isNull());
  
  //update the number of pots
  num_of_pots = config["settings"]["num_of_pots"];

  //fill the list
  for (int i = 0; i < num_of_pots; i++) {

    // Extract the values for pots
    int id = i;
    int pin = config["pots"][i]["pin"];
    float maxError = config["settings"]["change_percent"];

    // Extract the values for displays
    const char* text = config["pots"][i]["disp"];

    // Create a new Pot and Display object and add it to the vector
    pots.push_back(Pot(id, pin, maxError));
    displays.push_back(Display(id, text));
  }

  // update number of buttons
  num_of_buttons = config["settings"]["num_of_buttons"];

  // fill the list
  for(int i = 0; i < num_of_buttons; i++) {

    // Extrack the values for buttons
    int id = i;
    int pin = config["buttons"][id]["pin"];
    int threshold = config["buttons"][id]["threshold"];

    buttons.push_back(Button(id, pin, threshold));
  }
}

void loop() {
  // Pots
  for(int i = 0; i < num_of_pots; i++){ 
    int pot_data = pots[i].update();  // Update the pot value.
    if(pot_data != -1){  // If it changed:
      pots[i].outputToSerial();
      displays[i].update(pot_data);
    }
  }

  // Buttons
  for(int i = 0; i < num_of_buttons; i++){
    int button_data = buttons[i].update();
    if(button_data == 1){
      buttons[i].outputToSerial();
    }
  }

  // Update Delay
  delay(config["settings"]["update_delay"]); 
}
