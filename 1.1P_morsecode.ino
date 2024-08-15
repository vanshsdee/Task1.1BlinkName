const int ledPin = LED_BUILTIN; // LED pin
const int buttonPin = 2;        // Button pin
const int dotDuration = 200;     // Duration of a dot. Consider this 200ms as 'T'.
//dotDuration is also same for the time gap between two dots or a dot & a dash of a single character.
const int dashDuration = 600;   // Duration of a dash. it will be '3T'
const int characterDuration = 600;    // Pause between characters of same word. it equals the time '3T'

bool isBlinking = false;
volatile bool resetFlag = false; // Volatile because it is used in the ISR

void blinkDot() {
  digitalWrite(ledPin, HIGH); 
  delay(dotDuration);
  digitalWrite(ledPin, LOW); //this will OFF led for a time interval of 'T' or dotduration representing time gap after which led for the next dot or dash will get ON
  delay(dotDuration);
  Serial.print(".");
}

void blinkDash() {
  digitalWrite(ledPin, HIGH);
  delay(dashDuration);
  digitalWrite(ledPin, LOW);
  delay(dotDuration);
  Serial.print("-");
}

//Only morse code of V,A,N,S,H is added. it can be extended to include all other alphabets
void blinkCharacter(char c) 
{
  if (c == 'V') 
  {
    blinkDot(); blinkDot(); blinkDot(); blinkDash(); // V: ...-
  } 
  else if (c == 'A') 
  {
    blinkDot(); blinkDash(); // A: .-
  } 
  else if (c == 'N') 
  {
    blinkDash(); blinkDot(); // N: -.
  } 
  else if (c == 'S') 
  {
    blinkDot(); blinkDot(); blinkDot(); // S: ...
  } 
  else if (c == 'H') 
  {
    blinkDot(); blinkDot(); blinkDot(); blinkDot(); // H: ....
  }
  delay(characterDuration); // Pause between letters
  Serial.print(" ");   // Print a space between the characters of the name
}

void Name(char name[], int length) 
{
  for (int i = 0; i < length; i++) 
  {
    blinkCharacter(name[i]);
    if (resetFlag) break; // Check for reset flag after each character. this make the morse code to start again when button is pressed.
  }
}

// Interrupt service routine (ISR) to set the reset flag
void handleButtonPress() {
  resetFlag = true;
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);

  // Attach interrupt to the button pin
  attachInterrupt(digitalPinToInterrupt(buttonPin), handleButtonPress, FALLING);
}

void loop() {
  if (resetFlag) {
    resetFlag = false; // Clear the reset flag
    isBlinking = true;
  }

  if (isBlinking) {
    Serial.println("VANSH in Morse code");
    char name[] = {'V', 'A', 'N', 'S', 'H'}; // Name to blink
    int length = sizeof(name); //  / sizeof(name[0]);
    Name(name, length);
    isBlinking = false;
  }
}

