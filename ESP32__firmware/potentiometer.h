#include <Arduino.h>

class Pot {
  private:
    int id;                 // identification number
    int pin;                // attached pin
    mutable int val;        // last read value
    mutable int lastVal;    // values from previous read for comparison
    float maxError;         // acceptable error for the pot
  public:
      Pot(int id, int pin, float maxError)
        : id(id), pin(pin), maxError(maxError), val(0), lastVal(0) {
      pinMode(this->pin, INPUT);
    }

    // returns a value between 0 and 4095 representing the position of the pot
    int read() const {
      val = analogRead(pin);
      return val;
    }

    // returns val if the pot changes position, -1 if not. Calls read()
    int update(){
      int currentVal = read();
      if(currentVal < lastVal * (1 - maxError) || currentVal > lastVal * (1 + maxError)){
        lastVal = val;
        return val;
      }
      else{
        return -1;
      }
    }

    // convert the pot value to serial to be accepted by the software on the client
    void outputToSerial(){
      Serial.println("pot");  //anounce to client we are sending pot data
      Serial.print(id);
      Serial.print(",");
      Serial.print(val);
      Serial.println();
    }
};
