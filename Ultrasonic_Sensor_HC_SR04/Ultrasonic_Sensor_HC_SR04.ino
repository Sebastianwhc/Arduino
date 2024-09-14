#define echoPin_1 A0 // attach pin A0 Arduino to pin Echo of HC-SR04
#define trigPin_1 A1 //attach pin A1 Arduino to pin Trig of HC-SR04
#define echoPin_2 A2 // attach pin A2 Arduino to pin Echo of HC-SR04
#define trigPin_2 A3 //attach pin A3 Arduino to pin Trig of HC-SR04

// defines variables
long duration_1,duration_2; // variable for the duration of sound wave travel
int distance_1,distance_2; // variable for the distance measurement

void setup() {
  pinMode(trigPin_1, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin_1, INPUT); // Sets the echoPin as an INPUT
  pinMode(trigPin_2, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin_2, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
}
void loop() {
  // Clears the trigPin condition
  digitalWrite(trigPin_1, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_1, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration_1 = pulseIn(echoPin_1, HIGH);
    // pulseIn() Reads a pulse (either HIGH or LOW) on a pin. For example, if value is HIGH, 
    // pulseIn() waits for the pin to go HIGH, starts timing, then waits for the pin 
    // to go LOW and stops timing. Returns the length of the pulse in microseconds 
    // or 0 if no complete pulse was received within the timeout.
    //
    // pulseIn(pin, value)
    // pulseIn(pin, value, timeout)
    
    // * pin: the number of the pin on which you want to read the pulse. (int)
    // * value: type of pulse to read: either HIGH or LOW. (int)
    // * timeout (optional): the number of microseconds to wait for the pulse to be 
    // completed: the function returns 0 if no complete pulse was received within the 
    // timeout. Default is one second (unsigned long).
  // Calculating the distance
  distance_1 = duration_1 * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)


  digitalWrite(trigPin_2, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_2, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration_2 = pulseIn(echoPin_2, HIGH);
  
  // Calculating the distance
  distance_2 = duration_2 * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

  
  // Displays the distance on the Serial Monitor
  Serial.print("Distance S1: ");
  Serial.print(distance_1);
  Serial.print(" cm");
  Serial.print("   -   Distance S2: ");
  Serial.print(distance_2);
  Serial.println(" cm");
}
