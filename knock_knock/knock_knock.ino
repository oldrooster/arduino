
const int micPin = 13;

// variable for the piezo value
int knockVal;
// variable for the switch value
int switchVal;

// variables for the high and low limits of the knock value
const int quietKnock = 10;
const int loudKnock = 4090;

// variable to indicate if locked or not
bool locked = true;
// how many valid knocks you've received
int numberOfKnocks = 0;

void setup() {
  
  pinMode(micPin, INPUT);

  // start serial communication for debugging
  Serial.begin(9600);

}

void loop() {

  // if the box is locked
  if (locked == true) {

    // check the value of the piezo
    knockVal = analogRead(micPin);

    // if there are not enough valid knocks
    if (knockVal > 0) {

      // check to see if the knock is in range
      if (checkForKnock(knockVal) == true) {

        // increment the number of valid knocks
        numberOfKnocks++;
        Serial.print(3 - numberOfKnocks);
        Serial.println(" more knocks to go");
      }

      // print status of knocks
      //Serial.print(3 - numberOfKnocks);
      //Serial.println(" more knocks to go");
      
    }

    // if there are three knocks
    if (numberOfKnocks >= 3) {
      // unlock the box
      //locked = false;
    }
  }
}

// this function checks to see if a detected knock is within max and min range
bool checkForKnock(int value) {
  // if the value of the knock is greater than the minimum, and larger
  // than the maximum
  if (value > quietKnock && value < loudKnock) {
    // turn the status LED on
    delay(50);
    // print out the status
    Serial.print("Valid knock of value ");
    Serial.println(value);
    // return true
    return true;
  }
  // if the knock is not within range
  else {
    // print status
    //Serial.print("Bad knock value ");
    //Serial.println(value);
    // return false
    return false;
  }
}
