/*

    Firmware Update using github repo

    Using :-
    
    ->  HTTP over TLS (HTTPS) example sketch 
        Created by Ivan Grokhotkov, 2015.
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266httpUpdate.h>
#include "config.h"
#include "certs.h"


const char *ssid = SSID;
const char *password = PASS;

#define HTTPS_TIMEOUT 10000
#define HTTPS_BUFFER_SIZE 1024


const String FirmwareVer = {"0.0.2"};

X509List cert(cert_DigiCert_Global_Root_G2);

void onProgress(int progress, int total)
{
  float percentage = ((float)progress / total) * 100;
  Serial.print("Firmware update progress: ");
  Serial.print(percentage);
  Serial.println("%");
}

void onStart()
{
  Serial.println("Firmware update started...");
}

void onEnd()
{
  Serial.println("Firmware update completed!");
  delay(2000);
  ESP.restart();
}

void onError(int error)
{
  Serial.print("Firmware update error: ");
  Serial.println(error);
}

void setup()
{
  Serial.begin(74880); // default baudate for nodemcu (ESP8266MOD)
  while (!Serial)
  {
  };
  Serial.println("\n");
  
  Serial.println("Starting for Firmware Ver : " + String(FirmwareVer));

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Set time via NTP, as required for x.509 validation
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("Connecting to ");
  Serial.println(github_host);

  Serial.printf("Using certificate: %s\n", cert_DigiCert_Global_Root_G2);
  client.setTrustAnchors(&cert);

  if (!client.connect(github_host, github_port))
  {
    Serial.println("Connection failed");
    return;
  }

  
  Serial.print("Requesting URL: ");
  Serial.println(URL_VERSION_FILE_PATH);

  client.print(String("GET ") + URL_VERSION_FILE_PATH + " HTTP/1.1\r\n" + "Host: " + github_host + "\r\n" + "User-Agent: ESP8266HTTPClient\r\n" + "Connection: close\r\n\r\n");

  Serial.println("Request sent");

  unsigned long int _t = millis();

  while ((!client.connected() || !client.available()) && millis() - _t < HTTPS_TIMEOUT)
  {
    delay(250);
    // Serial.println("Waiting for data");
  }

  Serial.println("timespend : " + String(millis() - _t) + String(" milli seconds"));

  String responseBody = "";
  bool headerEnded = false;

  while (client.available() > 0)
  {
    String line = client.readStringUntil('\n');
    if (line == "\r")
    {
      headerEnded = true;
    }
    else if (headerEnded)
    {
      line.trim();
      responseBody += line + "\n";
    }
  }
  responseBody.trim();
  int atidx = responseBody.indexOf("@");
  String payload_bin = responseBody.substring(0, atidx);
  String payload = responseBody.substring(atidx + 1);
  Serial.println("payoad : " + payload);
  Serial.println("payoad_bin : " + payload_bin);
  if (payload.equals(FirmwareVer))
  {
    Serial.println("Firmware already Uptodate");
  }
  else
  {
    Serial.println("Firmware update needed!");
    Serial.println("New firmware detected");
    ESPhttpUpdate.setLedPin(LED_PIN, LOW);
    ESPhttpUpdate.onStart(onStart);
    ESPhttpUpdate.onEnd(onEnd);
    ESPhttpUpdate.onError(onError);
    ESPhttpUpdate.onProgress(onProgress);
    ESPhttpUpdate.rebootOnUpdate(false);
    ESPhttpUpdate.update(client, URL_fw_Bin);
  }
}

void loop() {}
