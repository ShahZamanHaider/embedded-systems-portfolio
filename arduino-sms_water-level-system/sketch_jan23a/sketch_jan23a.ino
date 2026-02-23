#include <SoftwareSerial.h>
#include<string.h>


SoftwareSerial SIM900A(2, 3);

struct smss {
  const char* num1 = "+91XXXXXXXXXX_1";
  const char* num2 = "+91XXXXXXXXXX_2";
  const char* num3 = "+91XXXXXXXXXX_3";
};

struct calll {
  const char* num1 = "+91XXXXXXXXXX_1";
  const char* num2 = "+91XXXXXXXXXX_2";
  const char* num3 = "+91XXXXXXXXXX_3";
};

int sensor_for_Full = 7;
int sensor_for_Empty = 8;
int ResetButtonPin = 9;



void setup() {
  SIM900A.begin(9600);
  Serial.begin(9600);
  Serial.println("System Is ready to work");
  delay(100);

  pinMode(sensor_for_Full, INPUT);
  pinMode(sensor_for_Empty, INPUT);


}




void loop() {
  smss sms;
  calll call;


     delay(1000);


  if (Serial.available() > 0) {

    if (digitalRead(ResetButtonPin) == HIGH) {
      hangup_Call();
      delay(1000);
    }

    if (digitalRead(sensor_for_Empty) == HIGH) {
      SendMessage_Tank_Empty();
      Make_Call();
    } 
    else if (digitalRead(sensor_for_Full) == HIGH) {
      SendMessage_Tank_Full();
      Make_Call();
    } 
    else {
      Serial.println("Something went wrong, please insert Sim again or Restart your IDE or inspect your water sensors...");
    }
  } 
  
  else {
    Serial.println("Error: Try Inserting Sim again or Restart your IDE...");
  }
  
}




void SendMessage_Tank_Empty() {
   smss sms;


  sendSMS(sms.num1, "TANK IS EMPTY");
  sendSMS(sms.num2, "TANK IS EMPTY");
  sendSMS(sms.num3, "TANK IS EMPTY");
}

void SendMessage_Tank_Full() {
  smss sms;

  sendSMS(sms.num1, "TANK IS FULL");
  sendSMS(sms.num2, "TANK IS FULL");
  sendSMS(sms.num3, "TANK IS FULL");
}

void sendSMS(const char* phone_Number, const char* message) {

  SIM900A.println("AT+CMGF=1");
  delay(1000);

  Serial.print("Sending sms to: ");
  Serial.println(phone_Number);

  SIM900A.print("AT+CMGS=\"");
  SIM900A.print(phone_Number);
  SIM900A.println("\"");
  delay(1000);

  SIM900A.println(message);
  delay(100);
  SIM900A.println((char)26);
  Serial.println("Message Successfully Sent to the three numbers.");
  delay(2000);
}

void Make_Call() {
calll call;
  
  callPhoneNumber(call.num1);
  delay(3000);
  callPhoneNumber(call.num2);
  delay(2000);
  callPhoneNumber(call.num3);
  delay(3000);
}

void callPhoneNumber(const char* phoneNumber) {
  Serial.print("Calling to: ");
  Serial.println(phoneNumber);

  SIM900A.println("ATD+ phoneNumber");
  delay(1000);
}


void hangup_Call(){

Serial.println("Call Successfuly Hanged Up");
SIM900A.println("ATH");
delay(1000);

}
