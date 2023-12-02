#include <SoftwareSerial.h>

// message settings
String message_opening = "Hi! Purple Yam Processing Machine here. Thanks for using our Text Reminder System! Expect messages every 15 minutes.";
String message_reminder = "Reminder: Ongoing cooking in the Machine. Check now to prevent accidents. Thank you!";
String message_current = "";

// add contact below {"+639123412","+639123123"}
String contacts[] = {"+639452271655","+639668449000"};

// time settings
// unsigned long interval_time = 1; // in minutes
// formula
// minutes * 60 * 1000
unsigned long delay_duration = 60000;
unsigned long previousMillis = 0;

int counter = 0; 

// Create a SoftwareSerial object for communication with SIM900
SoftwareSerial sim900(7, 8); // RX, TX
const int pin_out_gsm = 9;
const int pin_in_gsm = 10;
int state_pin_in_gsm;


void setup() {
  pinMode(pin_out_gsm, OUTPUT);
  pinMode(pin_in_gsm, INPUT_PULLUP);
  // Start communication with Arduino Serial Monitor
  Serial.begin(9600);
  init_gsm();
}

void loop(){
  state_pin_in_gsm = digitalRead(pin_in_gsm); 
  Serial.println(state_pin_in_gsm);
  
  // if on
  if(state_pin_in_gsm==0){
    if(counter < 1 ){
      Serial.println("SENDING WELCOME MESSAGE");
      previousMillis = millis();
      send_sms_init();
    }else{
      // Check if the desired delay duration has passed
      unsigned long currentMillis = millis();  // Get the current time
      Serial.println("WAITING");
      if (currentMillis - previousMillis >= delay_duration) {
        // Update the previous time to the current time for the next iteration
        previousMillis = currentMillis;
        send_sms_init();
      }
    }
  }else{
    counter = 0;
  }
  delay(2000);
}

void init_gsm(){  
  // Start communication with SIM900
  sim900.begin(9600);

  Serial.println("Initializing...");
  delay(1000);

  // Set the SIM900 to text mode
  sim900.print("AT+CMGF=1\r");
  delay(1000);

  Serial.println("Ready to send SMS");

  delay(2000);
  digitalWrite(pin_out_gsm,HIGH);
  delay(2000);
  digitalWrite(pin_out_gsm,LOW);
  
}
void send_sms_init() {
  // Send SMS
  if(counter < 1){
    message_current = message_opening;
  }else{
    message_current = message_reminder;
  }

  // send sms to all contacts
  for (String contact : contacts){
    sendSMS(contact, message_current);
    // Serial.println(message_current);
    // Serial.println(contact);
    // Serial.println(delay_duration);
    delay(5000);
  }
  counter++;
}

void sendSMS(String phoneNumber, String message) {
  // Set the recipient phone number
  sim900.print("AT+CMGS=\"");
  sim900.print(phoneNumber);
  sim900.println("\"");

  delay(1000);

  // Set the SMS message
  sim900.print(message);

  delay(1000);

  // Send the Ctrl+Z to indicate the end of the message
  sim900.write(26);

  delay(1000);

  Serial.println("SMS Sent to "+phoneNumber);
  Serial.println(message);
}

