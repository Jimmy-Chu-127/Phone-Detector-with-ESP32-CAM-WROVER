#include "WifiCam.hpp"
#include <WiFi.h>

<<<<<<< Updated upstream
static const char* WIFI_SSID = "ATTk29Y8s2";
static const char* WIFI_PASS = "pf+xb36frqbg";
=======
// static const char* WIFI_SSID = "NETGEAR46";
// static const char* WIFI_PASS = "roundchair972";
// static const char* WIFI_SSID = "io";
// static const char* WIFI_PASS = "girishkrishnan";
static const char* WIFI_SSID = "phonedetect";
static const char* WIFI_PASS = "12345678";
>>>>>>> Stashed changes

const int BUTTON_PIN = 14;                              //map BUTTON_PIN to the GPIO pin 14

//state == 0 represents the OFF state
//state == 1 represents the ON state with no phone detected
//state == 2 represents the ON state with phone detected
int state = 0;                                          //initializing state

int button_now_state;                                   //current state of the button
int button_pre_state = HIGH;                            //previous state of the button

unsigned long current_time;
unsigned long pre_time;

esp32cam::Resolution initialResolution;
WebServer server(80);

//The press_checker() function detects button state transition
//returns 1 for a button press
//returns 0 for no button press
int press_checker() {
  if (button_pre_state == button_now_state) {           //no transition occurs
    if (button_now_state == HIGH){                     
    }
    else { //button_now_state == LOW                    //if the button stays LOW there is a possible chance to reach 3s worth resetting
    }
    button_pre_state = button_now_state;                //updating the button state
    return 0;
  }
  else {  //button_pre_state != button_now_state        //transition occurs
    if (button_now_state == HIGH){                      //transition from LOW to HIGH consider pressed
      button_pre_state = button_now_state;              //updating the button state
      return 1;
    }
    else { //button_now_state == LOW                    //transition from HIGH to LOW not consider pressed
      if(button_pre_state == HIGH){
      }
      button_pre_state = button_now_state;              //updating the button state
      return 0;
    }
  }
}

void setup(){
  setupCommunication();                                 //setting up communication
  setupMotor();                                         //setting up buzzer
<<<<<<< Updated upstream
=======
  pinMode(DETECT_PIN, INPUT_PULLDOWN);                  //detect pin
>>>>>>> Stashed changes
  pinMode(BUTTON_PIN, INPUT_PULLUP);                    //pin Button
  delay(2000);
  // Serial.begin(115200);

  // WiFi.persistent(false);
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(WIFI_SSID, WIFI_PASS);
  // if (WiFi.waitForConnectResult() != WL_CONNECTED) {
  //   Serial.println("WiFi failure");
  //   delay(5000);
  //   ESP.restart();
  // }

  WiFi.softAP(WIFI_SSID, WIFI_PASS);  
  Serial.println("WiFi connected");

  {
    using namespace esp32cam;
    initialResolution = Resolution::find(1024, 768);

    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(initialResolution);
    cfg.setJpeg(80);

    bool ok = Camera.begin(cfg);
    if (!ok) {
      Serial.println("camera initialize failure");
      delay(5000);
      ESP.restart();
    }
    Serial.println("camera initialize success");
  }

  Serial.println("camera starting");
  Serial.print("http://");
  // Serial.println(WiFi.localIP());
  Serial.println(WiFi.softAPIP());

  addRequestHandlers();
  server.begin();
  pre_time = millis();
}

void loop(){
  server.handleClient();

  current_time = millis();
  if (current_time - pre_time > 1000){
    sendMessage("Hello");
    pre_time = current_time;
  }

  button_now_state = digitalRead(BUTTON_PIN);           //getting the current button state

  String command = receiveMessage();                    //receive message from python
  
  if(state == 0){                                       //phone detector OFF state
    if(press_checker()){
      // Serial.println("testasdf");
      state = 1;
    }
  }

  if(state == 1){                                       //phone detector ON not detected state
    if(press_checker()){
      state = 0;
    }
    if(command == "undetected"){
      //stay in state 1
    }
    if(command == "detected"){
      state = 2;
    }
  }

  if(state == 2){                                        //phone detector ON detected state
    activateMotor(125);

    if(press_checker()){
      state = 0;
      deactivateMotor();
    }
    if(command == "undetected"){
      state = 1;
      deactivateMotor();
    }
    if(command == "detected"){
      //stay in state 2
    }
  }
}
