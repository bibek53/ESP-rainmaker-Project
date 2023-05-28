#include "RMaker.h"
#include "WiFi.h"
#include "WiFiProv.h"
#include <Preferences.h>
#include <RBDdimmer.h>
Preferences pref;

const char *service_name = "PROV_THAPA_HOME";
const char *pop = "1234";

// define the Chip Id
uint32_t espChipId = 0;

// define the Node Name
char nodeName[] = "ESP32_Relay_8B";

// define the Device Names
char deviceName_1[] = "TubeLight";
char deviceName_2[] = "Light1";
char deviceName_3[] = "Light2";
char deviceName_4[] = "Light3";
char deviceName_5[] = "fan";

// define the GPIO connected with Relays and switches
static uint8_t RelayPin1 = 23;  //D23
static uint8_t RelayPin2 = 22;  //D22
static uint8_t RelayPin3 = 21;  //D21
static uint8_t RelayPin4 = 19;  //D19

static uint8_t SwitchPin1 = 13;  //D13
static uint8_t SwitchPin2 = 15;  //D15
static uint8_t SwitchPin3 = 14;  //D14
static uint8_t SwitchPin4 = 27;  //D27
static uint8_t SwitchPin5 = 33;  //D33

// static uint8_t SwitchPin6 = 32;  //D32
// static uint8_t SwitchPin7 = 15;  //D15
// static uint8_t SwitchPin8 = 4;   //D4

#define outputPin 12
#define zerocross 5  // for boards with CHANGEBLE input pins

dimmerLamp dimmer(outputPin, zerocross);  //initialase port for dimmer for ESP8266, ESP32, Arduino due boards

int outVal = 0;
int Slider_Value = 0;

static uint8_t gpio_reset = 0;  // Press BOOT for reset WiFi

/* Variable for reading pin status*/
// Relay State
bool toggleState_1 = LOW;  //Define integer to remember the toggle state for relay 1
bool toggleState_2 = LOW;  //Define integer to remember the toggle state for relay 2
bool toggleState_3 = LOW;  //Define integer to remember the toggle state for relay 3
bool toggleState_4 = LOW;  //Define integer to remember the toggle state for relay 4
bool toggleState_5 = LOW;  //Define integer to remember the toggle state for relay 4

// Switch State
bool SwitchState_1 = LOW;
bool SwitchState_2 = LOW;
bool SwitchState_3 = LOW;
bool SwitchState_4 = LOW;
bool SwitchState_5 = LOW;


//The framework provides some standard device types like switch, lightbulb, fan, temperature sensor.
static Switch my_switch1(deviceName_1, &RelayPin1);
static Switch my_switch2(deviceName_2, &RelayPin2);
static Switch my_switch3(deviceName_3, &RelayPin3);
static Switch my_switch4(deviceName_4, &RelayPin4);
static Fan my_fan(deviceName_5);

void sysProvEvent(arduino_event_t *sys_event) {
  switch (sys_event->event_id) {
    case ARDUINO_EVENT_PROV_START:
#if CONFIG_IDF_TARGET_ESP32
      Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on BLE\n", service_name, pop);
      printQR(service_name, pop, "ble");
#else
      Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on SoftAP\n", service_name, pop);
      printQR(service_name, pop, "softap");
#endif
      break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      Serial.printf("\nConnected to Wi-Fi!\n");
      // digitalWrite(wifiLed, true);
      break;
  }
}

void write_callback(Device *device, Param *param, const param_val_t val, void *priv_data, write_ctx_t *ctx) {
  const char *device_name = device->getDeviceName();
  const char *param_name = param->getParamName();

  if (strcmp(device_name, deviceName_1) == 0) {

    Serial.printf("Lightbulb = %s\n", val.val.b ? "true" : "false");

    if (strcmp(param_name, "Power") == 0) {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      toggleState_1 = val.val.b;
      (toggleState_1 == false) ? digitalWrite(RelayPin1, HIGH) : digitalWrite(RelayPin1, LOW);
      param->updateAndReport(val);
      pref.putBool("Relay1", toggleState_1);
    }

  } else if (strcmp(device_name, deviceName_2) == 0) {

    Serial.printf("Switch value = %s\n", val.val.b ? "true" : "false");

    if (strcmp(param_name, "Power") == 0) {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      toggleState_2 = val.val.b;
      (toggleState_2 == false) ? digitalWrite(RelayPin2, HIGH) : digitalWrite(RelayPin2, LOW);
      param->updateAndReport(val);
      pref.putBool("Relay2", toggleState_2);
    }

  } else if (strcmp(device_name, deviceName_3) == 0) {

    Serial.printf("Switch value = %s\n", val.val.b ? "true" : "false");

    if (strcmp(param_name, "Power") == 0) {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      toggleState_3 = val.val.b;
      (toggleState_3 == false) ? digitalWrite(RelayPin3, HIGH) : digitalWrite(RelayPin3, LOW);
      param->updateAndReport(val);
      pref.putBool("Relay3", toggleState_3);
    }

  } else if (strcmp(device_name, deviceName_4) == 0) {

    Serial.printf("Switch value = %s\n", val.val.b ? "true" : "false");

    if (strcmp(param_name, "Power") == 0) {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      toggleState_4 = val.val.b;
      (toggleState_4 == false) ? digitalWrite(RelayPin4, HIGH) : digitalWrite(RelayPin4, LOW);
      param->updateAndReport(val);
      pref.putBool("Relay4", toggleState_4);
    }

  } else if (strcmp(device_name, deviceName_5) == 0) {
    if (strcmp(param_name, "Power") == 0) {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      toggleState_5 = val.val.b;
      (toggleState_5 == false) ? dimmer.setPower(0) : dimmer.setPower(88);
      param->updateAndReport(val);
      pref.putInt("fan1", toggleState_5);
    }
    if (strcmp(param_name, "Speed") == 0) {
      if (toggleState_5) {
        Serial.printf("\nReceived value = %d for %s - %s\n", val.val.i, device_name, param_name);
        Slider_Value = val.val.i;
        outVal = Slider_Value;
        dimmer.setPower(outVal);  // name.setPower(0%-100%)
        param->updateAndReport(val);
        pref.putInt("speed1", Slider_Value);
      }
    }
  }
}


void all_SwitchOff() {
  toggleState_1 = 0;
  digitalWrite(RelayPin1, HIGH);
  pref.putBool("Relay1", toggleState_1);
  my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_1);
  delay(100);
  toggleState_2 = 0;
  digitalWrite(RelayPin2, HIGH);
  pref.putBool("Relay2", toggleState_2);
  my_switch2.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_2);
  delay(100);
  toggleState_3 = 0;
  digitalWrite(RelayPin3, HIGH);
  pref.putBool("Relay3", toggleState_3);
  my_switch3.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_3);
  delay(100);
  toggleState_4 = 0;
  digitalWrite(RelayPin4, HIGH);
  pref.putBool("Relay4", toggleState_4);
  my_switch4.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_4);
  delay(100);
  toggleState_5 = 0;
  dimmer.setPower(0);
  pref.putInt("fan1", toggleState_5);
  my_fan.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_5);
  delay(100);
}

void all_SwitchOn() {
  toggleState_1 = 1;
  digitalWrite(RelayPin1, LOW);
  pref.putBool("Relay1", toggleState_1);
  my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_1);
  delay(100);
  toggleState_2 = 1;
  digitalWrite(RelayPin2, LOW);
  pref.putBool("Relay2", toggleState_2);
  my_switch2.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_2);
  delay(100);
  toggleState_3 = 1;
  digitalWrite(RelayPin3, LOW);
  pref.putBool("Relay3", toggleState_3);
  my_switch3.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_3);
  delay(100);
  toggleState_4 = 1;
  digitalWrite(RelayPin4, LOW);
  pref.putBool("Relay4", toggleState_4);
  my_switch4.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_4);
  delay(100);
  toggleState_5 = 1;
  dimmer.setPower(100);
  pref.putInt("fan1", toggleState_5);
  my_fan.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_5);
  delay(100);
}

void manual_control() {
  if (digitalRead(SwitchPin1) == LOW && SwitchState_1 == LOW) {
    digitalWrite(RelayPin1, LOW);
    toggleState_1 = 1;
    SwitchState_1 = HIGH;
    pref.putBool("Relay1", toggleState_1);
    my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_1);
    Serial.println("TubeLight on");
  }
  if (digitalRead(SwitchPin1) == HIGH && SwitchState_1 == HIGH) {
    digitalWrite(RelayPin1, HIGH);
    toggleState_1 = 0;
    SwitchState_1 = LOW;
    pref.putBool("Relay1", toggleState_1);
    my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_1);
    Serial.println("TubeLight off");
  }
  if (digitalRead(SwitchPin2) == LOW && SwitchState_2 == LOW) {
    digitalWrite(RelayPin2, LOW);
    toggleState_2 = 1;
    SwitchState_2 = HIGH;
    pref.putBool("Relay2", toggleState_2);
    my_switch2.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_2);
    Serial.println("Light1 on");
  }
  if (digitalRead(SwitchPin2) == HIGH && SwitchState_2 == HIGH) {
    digitalWrite(RelayPin2, HIGH);
    toggleState_2 = 0;
    SwitchState_2 = LOW;
    pref.putBool("Relay2", toggleState_2);
    my_switch2.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_2);
    Serial.println("Light1 off");
  }
  if (digitalRead(SwitchPin3) == LOW && SwitchState_3 == LOW) {
    digitalWrite(RelayPin3, LOW);
    toggleState_3 = 1;
    SwitchState_3 = HIGH;
    pref.putBool("Relay3", toggleState_3);
    my_switch3.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_3);
    Serial.println("Light2 on");
  }
  if (digitalRead(SwitchPin3) == HIGH && SwitchState_3 == HIGH) {
    digitalWrite(RelayPin3, HIGH);
    toggleState_3 = 0;
    SwitchState_3 = LOW;
    pref.putBool("Relay3", toggleState_3);
    my_switch3.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_3);
    Serial.println("Light2 off");
  }
  if (digitalRead(SwitchPin4) == LOW && SwitchState_4 == LOW) {
    digitalWrite(RelayPin4, LOW);
    toggleState_4 = 1;
    SwitchState_4 = HIGH;
    pref.putBool("Relay4", toggleState_4);
    my_switch4.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_4);
    Serial.println("Light3 on");
  }
  if (digitalRead(SwitchPin4) == HIGH && SwitchState_4 == HIGH) {
    digitalWrite(RelayPin4, HIGH);
    toggleState_4 = 0;
    SwitchState_4 = LOW;
    pref.putBool("Relay4", toggleState_4);
    my_switch4.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_4);
    Serial.println("Light3 off");
  }
  if (digitalRead(SwitchPin5) == LOW && SwitchState_5 == LOW) {
     dimmer.setPower(100);
    toggleState_5 = 1;
    SwitchState_5 = HIGH;
    pref.putInt("fan1", toggleState_5);
    my_fan.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_5);
    Serial.println("Fan on");
  }
  if (digitalRead(SwitchPin5) == HIGH && SwitchState_5 == HIGH) {
     dimmer.setPower(0);
    toggleState_5 = 0;
    SwitchState_5 = LOW;
    pref.putBool("fan1", toggleState_5);
    my_fan.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_5);
    Serial.println("fan off");
  }
}

void getRelayState() {
  toggleState_1 = pref.getBool("Relay1", 0);
  digitalWrite(RelayPin1, !toggleState_1);
  my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_1);
  delay(200);
  toggleState_2 = pref.getBool("Relay2", 0);
  digitalWrite(RelayPin2, !toggleState_2);
  my_switch2.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_2);
  delay(200);
  toggleState_3 = pref.getBool("Relay3", 0);
  digitalWrite(RelayPin3, !toggleState_3);
  my_switch3.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_3);
  delay(200);
  toggleState_4 = pref.getBool("Relay4", 0);
  digitalWrite(RelayPin4, !toggleState_4);
  my_switch4.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_4);
  delay(200);
  toggleState_5 = pref.getInt("fan1", 0);
  dimmer.setPower(toggleState_5 ? 100 : 0);
  toggleState_5 = !toggleState_5;
  my_fan.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_5);
  delay(200);
  Slider_Value = pref.getInt("speed1", 0);
  dimmer.setPower(Slider_Value);
  Slider_Value = !Slider_Value;
  my_fan.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, Slider_Value);
  delay(200);
}

void setup() {
  Serial.begin(115200);
  pref.begin("Relay_State", false);

  // Set the Relays GPIOs as output mode
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  // Configure the input GPIOs
  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);
  pinMode(SwitchPin5, INPUT_PULLUP);
  pinMode(gpio_reset, INPUT);

  // Write to the GPIOs the default state on booting
  digitalWrite(RelayPin1, !toggleState_1);
  digitalWrite(RelayPin2, !toggleState_2);
  digitalWrite(RelayPin3, !toggleState_3);
  digitalWrite(RelayPin4, !toggleState_4);

  Node my_node;
  my_node = RMaker.initNode(nodeName);

  //Standard switch device
  my_switch1.addCb(write_callback);
  my_switch2.addCb(write_callback);
  my_switch3.addCb(write_callback);
  my_switch4.addCb(write_callback);
  my_fan.addCb(write_callback);

  //Add switch device to the node
  my_node.addDevice(my_switch1);
  my_node.addDevice(my_switch2);
  my_node.addDevice(my_switch3);
  my_node.addDevice(my_switch4);
  my_node.addDevice(my_fan);

  dimmer.begin(NORMAL_MODE, ON);  // Initialize the dimmer object


  //Custom Fan device
  Param speed("Speed", ESP_RMAKER_PARAM_RANGE, value(0), PROP_FLAG_READ | PROP_FLAG_WRITE);
  speed.addBounds(value(0), value(100), value(1));
  speed.addUIType(ESP_RMAKER_UI_SLIDER);
  my_fan.addParam(speed);
  my_fan.updateAndReportParam("Speed", 0);
  my_fan.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME,toggleState_5);
  delay(500);

  //This is optional
  RMaker.enableOTA(OTA_USING_PARAMS);
  //If you want to enable scheduling, set time zone for your region using setTimeZone().
  //The list of available values are provided here https://rainmaker.espressif.com/docs/time-service.html
  // RMaker.setTimeZone("Asia/Shanghai");
  // Alternatively, enable the Timezone service and let the phone apps set the appropriate timezone
  RMaker.enableTZService();
  RMaker.enableSchedule();

  //Service Name
  for (int i = 0; i < 17; i = i + 8) {
    espChipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }

  Serial.printf("\nChip ID:  %d Service Name: %s\n", espChipId, service_name);

  Serial.printf("\nStarting ESP-RainMaker\n");
  RMaker.start();

  WiFi.onEvent(sysProvEvent);
#if CONFIG_IDF_TARGET_ESP32
  WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, WIFI_PROV_SECURITY_1, pop, service_name);
#else
  WiFiProv.beginProvision(WIFI_PROV_SCHEME_SOFTAP, WIFI_PROV_SCHEME_HANDLER_NONE, WIFI_PROV_SECURITY_1, pop, service_name);
#endif

  getRelayState();  // Get the last state of Relays

  //    my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
  //    my_switch2.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
  //    my_switch3.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
  //    my_switch4.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
  //    my_switch5.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
  //    my_switch6.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
  //    my_switch7.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
  //    my_switch8.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
}

void loop() {
  // Read GPIO0 (external button to reset device
  if (digitalRead(gpio_reset) == LOW) {  //Push button pressed
    Serial.printf("Reset Button Pressed!\n");
    // Key debounce handling
    delay(100);
    int startTime = millis();
    while (digitalRead(gpio_reset) == LOW) delay(50);
    int endTime = millis();

    if ((endTime - startTime) > 10000) {
      // If key pressed for more than 10secs, reset all
      Serial.printf("Reset to factory.\n");
      RMakerFactoryReset(2);
    } else if ((endTime - startTime) > 3000) {
      Serial.printf("Reset Wi-Fi.\n");
      // If key pressed for more than 3secs, but less than 10, reset Wi-Fi
      RMakerWiFiReset(2);
    }
  }
  delay(100);
  manual_control();
}
