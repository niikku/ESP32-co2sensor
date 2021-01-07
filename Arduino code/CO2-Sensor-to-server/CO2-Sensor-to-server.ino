#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include "SparkFun_SCD30_Arduino_Library.h"

/////////////////////////////////////////////////
////////////// WIFI ACCESS POINT ////////////////
const char *ssid = "LAB";
const char *password = "QWERTYUIOP";
/////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
////////////////////////////// API INFORMATION ////////////////////////////////////
const String deviceID = "51c38256-6fcd-4113-afa5-0d07a6782a93"; //Device ID of your device
const String username = "admin";                                //Your username
const String serverURL = "http://192.168.0.4:3000/api";         //The IP address of the API
///////////////////////////////////////////////////////////////////////////////////

HTTPClient http;
SCD30 airSensor;

void setup()
{
    Serial.begin(115200);

    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());

    Wire.begin();

    if (airSensor.begin(Wire, false) == false)
    {
        Serial.println("Air sensor not detected. Please check wiring. Freezing...");
        while(1){
          ;
        }
    }

      float offset = airSensor.getTemperatureOffset();
      Serial.print("Current temp offset: ");
      Serial.print(offset, 2);
      Serial.println("C");

    //Temperature offset due to self-heating
    airSensor.setTemperatureOffset(3.5);
}

void loop()
{
    if (airSensor.dataAvailable())
    {
        uint16_t co2 = airSensor.getCO2();
        String temperature = String(airSensor.getTemperature(),1);
        String humidity = String(airSensor.getHumidity(),0);

        http.begin(serverURL);
        http.addHeader("Content-Type", "application/json");

        String request = "{";
        request += "\"deviceid\":";
        request += "\"" + deviceID + "\",";
        request += "\"username\":";
        request += "\"" + username + "\",";
        request += "\"co2\":";
        request += "\"" + String(co2) + "\",";
        request += "\"temperature\":";
        request += "\"" + temperature + "\",";
        request += "\"humidity\":";
        request += "\"" + humidity + "\"}";

//        String request = "{";
//        request += "\"co2\":";
//        request += "\"" + String(co2) + "\"";
//        request += ",\"uuid\":";
//        request += "\"" + uuid + "\"}";
        Serial.print("Sending: ");
        Serial.println(request);

        int http_response_code = http.POST(request);

        if (http_response_code < 0)
        {
            Serial.println("Did not get a reply from the server, perhaps the URL is wrong or a firewall is blocking requests.");
        }
        else
        {
            String response = http.getString();
            Serial.print("Response: ");
            Serial.println(response);
        }
        http.end();
    }

    delay(10000);
}
