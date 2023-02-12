const int DETECT_PIN = 12;
unsigned long pre_time;
unsigned long now_time;
// String command;

// put your setup code here, to run once:
void setup() {
  setupCommunication();
  setupDisplay();
  pinMode(DETECT_PIN, OUTPUT);
  writeDisplay("Start", 0, true);
  pre_time = millis();
  now_time = millis();
}

// put your main code here, to run repeatedly:
void loop() {
  now_time = millis();

  String command = receiveMessage();
  // if (command != "")
  //   sendMessage(command.c_str());

  //if(now_time - pre_time >= 1000){
  
    if(command == "undetected"){
      digitalWrite(DETECT_PIN, LOW);
      writeDisplay("undetected", 1, true);    
      pre_time = now_time;
    }

    else if(command == "detected"){
      digitalWrite(DETECT_PIN, HIGH);
      writeDisplay("detected", 1, true);    
      pre_time = now_time;
    }
  //}
}
