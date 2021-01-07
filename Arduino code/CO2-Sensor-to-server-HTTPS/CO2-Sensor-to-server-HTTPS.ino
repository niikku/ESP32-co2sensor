#include <HTTPClient.h>
#include <Wire.h>
#include "SparkFun_SCD30_Arduino_Library.h"

const char* root_ca= \
  "-----BEGIN CERTIFICATE-----\n" \
  "MIIFFjCCAv6gAwIBAgIRAJErCErPDBinU/bWLiWnX1owDQYJKoZIhvcNAQELBQAw\n" \
  "TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n" \
  "cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMjAwOTA0MDAwMDAw\n" \
  "WhcNMjUwOTE1MTYwMDAwWjAyMQswCQYDVQQGEwJVUzEWMBQGA1UEChMNTGV0J3Mg\n" \
  "RW5jcnlwdDELMAkGA1UEAxMCUjMwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\n" \
  "AoIBAQC7AhUozPaglNMPEuyNVZLD+ILxmaZ6QoinXSaqtSu5xUyxr45r+XXIo9cP\n" \
  "R5QUVTVXjJ6oojkZ9YI8QqlObvU7wy7bjcCwXPNZOOftz2nwWgsbvsCUJCWH+jdx\n" \
  "sxPnHKzhm+/b5DtFUkWWqcFTzjTIUu61ru2P3mBw4qVUq7ZtDpelQDRrK9O8Zutm\n" \
  "NHz6a4uPVymZ+DAXXbpyb/uBxa3Shlg9F8fnCbvxK/eG3MHacV3URuPMrSXBiLxg\n" \
  "Z3Vms/EY96Jc5lP/Ooi2R6X/ExjqmAl3P51T+c8B5fWmcBcUr2Ok/5mzk53cU6cG\n" \
  "/kiFHaFpriV1uxPMUgP17VGhi9sVAgMBAAGjggEIMIIBBDAOBgNVHQ8BAf8EBAMC\n" \
  "AYYwHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsGAQUFBwMBMBIGA1UdEwEB/wQIMAYB\n" \
  "Af8CAQAwHQYDVR0OBBYEFBQusxe3WFbLrlAJQOYfr52LFMLGMB8GA1UdIwQYMBaA\n" \
  "FHm0WeZ7tuXkAXOACIjIGlj26ZtuMDIGCCsGAQUFBwEBBCYwJDAiBggrBgEFBQcw\n" \
  "AoYWaHR0cDovL3gxLmkubGVuY3Iub3JnLzAnBgNVHR8EIDAeMBygGqAYhhZodHRw\n" \
  "Oi8veDEuYy5sZW5jci5vcmcvMCIGA1UdIAQbMBkwCAYGZ4EMAQIBMA0GCysGAQQB\n" \
  "gt8TAQEBMA0GCSqGSIb3DQEBCwUAA4ICAQCFyk5HPqP3hUSFvNVneLKYY611TR6W\n" \
  "PTNlclQtgaDqw+34IL9fzLdwALduO/ZelN7kIJ+m74uyA+eitRY8kc607TkC53wl\n" \
  "ikfmZW4/RvTZ8M6UK+5UzhK8jCdLuMGYL6KvzXGRSgi3yLgjewQtCPkIVz6D2QQz\n" \
  "CkcheAmCJ8MqyJu5zlzyZMjAvnnAT45tRAxekrsu94sQ4egdRCnbWSDtY7kh+BIm\n" \
  "lJNXoB1lBMEKIq4QDUOXoRgffuDghje1WrG9ML+Hbisq/yFOGwXD9RiX8F6sw6W4\n" \
  "avAuvDszue5L3sz85K+EC4Y/wFVDNvZo4TYXao6Z0f+lQKc0t8DQYzk1OXVu8rp2\n" \
  "yJMC6alLbBfODALZvYH7n7do1AZls4I9d1P4jnkDrQoxB3UqQ9hVl3LEKQ73xF1O\n" \
  "yK5GhDDX8oVfGKF5u+decIsH4YaTw7mP3GFxJSqv3+0lUFJoi5Lc5da149p90Ids\n" \
  "hCExroL1+7mryIkXPeFM5TgO9r0rvZaBFOvV2z0gp35Z0+L4WPlbuEjN/lxPFin+\n" \
  "HlUjr8gRsI3qfJOQFy/9rKIJR0Y/8Omwt/8oTWgy1mdeHmmjk7j1nYsvC9JSQ6Zv\n" \
  "MldlTTKB3zhThV1+XWYp6rjd5JW1zbVWEkLNxE7GJThEUG3szgBVGP7pSWTUTsqX\n" \
  "nLRbwHOoq7hHwg==\n" \
  "-----END CERTIFICATE-----\n";

/////////////////////////////////////////////////
////////////// WIFI ACCESS POINT ////////////////
const char *ssid = "LAB";
const char *password = "QWERTYUIOP";
/////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
////////////////////////////// API INFORMATION ////////////////////////////////////
const String deviceID = "51c38256-6fcd-4113-afa5-0d07a6782a93"; //Device ID of your device
const String username = "admin";                             //Your username
const String serverURL = "https://co2.nikku.nl/api"; //The IP address of the API
///////////////////////////////////////////////////////////////////////////////////

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
        HTTPClient http;
      
        uint16_t co2 = airSensor.getCO2();
        String temperature = String(airSensor.getTemperature(),1);
        String humidity = String(airSensor.getHumidity(),0);

        http.begin(serverURL, root_ca);
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

        Serial.println(request);

        int http_response_code = http.POST(request);

        if (http_response_code < 0)
        {
            Serial.println("Error in sending sensor value");
            String response = http.getString();
            Serial.print("Response (/sensordata): ");
        }
        else
        {
            String response = http.getString();
            Serial.print("Response (/sensordata): ");
            Serial.println(response);
        }
        http.end();
    }

    delay(10000);
}
