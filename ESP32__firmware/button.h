#include <Arduino.h>

class Button {
  private:
    int id;                   // identification number
    int pin;                  // attached pin
    int threshold;            // analog value where press is registered
    mutable int val;          // analog value
    mutable bool touched;     // val is above threshold
    mutable bool lastTouched; // pin was touched last read
    
  public:
      Button(int id, int pin, int threshold)
        : id(id), pin(pin), threshold(threshold), val(0), touched(false), lastTouched(false) {
      pinMode(this->pin, INPUT);
    }

    // Retunrs a bool representing if the button is touched
    bool read() const {
      val = touchRead(pin);
      if(val < threshold){
        touched = true;
      }
      else{
        touched = false;
      }  
      return touched;
    }

    // returns val if the button changes value (0 false, 1 true), -1 if not. Calls read()
    int update(){
      bool currentTouched = read();
      if(currentTouched != lastTouched){
        lastTouched = currentTouched;
        if (currentTouched){
          return 1; // push
        }
        else{
          return 0; // release
        }
      }
      else{
        return -1;
      }
    }

    // Assume we only send serial data on button update, thus nothing is needed except for the id
    void outputToSerial(){
      Serial.println("button");  //anounce to client we are sending button data
      Serial.println(id);
    }
};
