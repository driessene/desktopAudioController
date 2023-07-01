#include <Arduino.h>
#include <ArduinoJson.h>

#define MAX_CONFIG_SIZE 1024

// pause until a serial input is recived
void wait_for_serial(int timeout = 10000){
  int wait = 100;       //wait x long between checks
  int total_time = 0;   //tries
  while(Serial.available() == 0 && total_time < timeout){
    delay(wait);
    total_time += wait;
  }
}

StaticJsonDocument<MAX_CONFIG_SIZE> getConfig() {
  
  char configYaml[MAX_CONFIG_SIZE];   // define the config file
  int bytesRead = 0;                  // total bytes recived
  Serial.println("Config request");   // request new config
  wait_for_serial(2000);              // wait for a serial input. Try for 2 seconds

  while (Serial.available() > 0) {      // while the buffer contains content
    char yamlChar = Serial.read();      // read the character
    configYaml[bytesRead++] = yamlChar; // add it to the file
    wait_for_serial(2000);              // wait for the next character. try for 2 seconds
  }
  
  configYaml[bytesRead] = '\0';         // add a null character at the end of the file

  // Parse the config YAML as JSON
  StaticJsonDocument<MAX_CONFIG_SIZE> doc;
  DeserializationError error = deserializeJson(doc, configYaml);

  // Check if there was an error during parsing
  if (error) {
    Serial.print("Failed to parse config YAML: ");
    Serial.println(error.c_str());

    // Print the received config YAML for debugging
    // Serial.println("Config YAML received:");
    // Serial.println(configYaml);

    return StaticJsonDocument<MAX_CONFIG_SIZE>(); // Return an empty document
  }

  Serial.println("Config valid");    // Acknowledge successful config reception
  return doc;                        // Return the resulting config document
}
