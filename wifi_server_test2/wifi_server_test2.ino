/*
 Another silly test

 This one presents a configuration page, and saves the content to EEPROM.
 The format saved is exactly what the Azure IOTHUB sample code wants.
 The idea would be to load this if a reset button is held in.
 We could also do a few other things, like be able to clear the EEPROM.
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

const char WiFiAPPSK[] = "sparkfun";

const char HTTP_ROOT[] = "<html>\
  <head>\
    <title>Scintilla Configuration</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Scintilla configuration page!</h1>\
<form action='/configure' method='POST'>Wifi Details:<br>\
  SSID:<input type='text' name='SSID' placeholder='MyWifiSSID'><br>\
  Password:<input type='password' name='PASSWORD' placeholder='password'><br>\
  Azure Host:<input type='text' name='AZUREHOST' placeholder='Go get it from Azure!'><br>\
  Azure Key:<input type='text' name='AZUREKEY' placeholder='Go get it from Azure!'><br>\
  Azure EventHub ID:<input type='text' name='AZUREID' placeholder='Go get it from Azure!'><br>\
  Location:<input type='text' name='GEO' placeholder='Go get it from Azure!'><br>\
  <input type='submit' name='SUBMIT' value='Save'></form>\
    <img src=\"/test.svg\" />\
  </body>\
</html>";


const char *ssid = "DLab";
const char *password = "endmill1";

ESP8266WebServer server ( 80 );

const int led = 13;

void handleRoot() {
	digitalWrite ( led, 1 );
	int sec = millis() / 1000;
	int min = sec / 60;
	int hr = min / 60;

	server.send ( 200, "text/html", HTTP_ROOT );
	digitalWrite ( led, 0 );
}


int numWifi = 1;  // number of defined wifi ssid/pwd pairs
const char* ssidi;
const char* pwdi;
const char* geo = "-33.8745828,151.2067465";  // Sydney Town Hall
// Event Hub
const char* host = "makerdenww-ns.servicebus.windows.net";
const char* key = "Z2aUC4jbHD3aphn8pIMmOhiqf9/ns7o/SvPSixKKCEY=";
const char* id = "D1";



const int BufferLen = 510;  // max eprom length, minus 2 for data length
char buffer[BufferLen];
int length;
void saveSettingsToEEPROM(String ssid, String password, String host, String key, String id, String geo) {
  // TODO FIXME: Somebody do some validation! :D
  Serial.println("Saving settings to eeprom...");
  EEPROM.begin(512);

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  // EDG says: The MS code does multiple APs.  We can do this later with a more complex UI
  root["wifi"] = numWifi;

  JsonArray& _ssid = root.createNestedArray("ssid");
  for (int i = 0; i < numWifi; i++)
  {
    _ssid.add(ssid);
  }

  JsonArray& _password = root.createNestedArray("pwd");
  for (int i = 0; i < numWifi; i++)
  {
    _password.add(password);
  }


  root["host"] = host;
  root["key"] = key;
  root["id"] = id;
  root["geo"] = geo;

  length = root.printTo(buffer, BufferLen);
  root.prettyPrintTo(Serial);

  EEPROM.write(0, highByte(length));
  EEPROM.write(1, lowByte(length));
  for (int address = 2; address < length + 2; address++) {
    EEPROM.write(address, buffer[address - 2]);
  }
  EEPROM.commit();
}

void handleNotFound() {
	digitalWrite ( led, 1 );
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";

	for ( uint8_t i = 0; i < server.args(); i++ ) {
		message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
	}

	server.send ( 404, "text/plain", message );
	digitalWrite ( led, 0 );
}

void initWifi() {
  WiFi.begin ( ssid, password );
  Serial.println ( "" );

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );
  
  // Setup AP
  WiFi.mode(WIFI_AP_STA);

  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "ESP8266 Thing " + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
}

void doConfigure(){
  String msg;
  if (server.hasArg("SSID") && 
      server.hasArg("PASSWORD") && 
      server.hasArg("AZUREHOST") &&
      server.hasArg("AZUREKEY") &&
      server.hasArg("AZUREID") &&
      server.hasArg("GEO")){
    // TODO: Try a connection.
    // TODO: If it's good, save it to EEPROM
    saveSettingsToEEPROM(server.arg("SSID"),server.arg("PASSWORD"),server.arg("AZUREHOST"),server.arg("AZUREKEY"),server.arg("AZUREID"),server.arg("GEO"));
    msg = "Settings saved!\n";
  }
  else {  
  msg = "Failed to save settings!";
  Serial.println("Failed to save settings!");
  }
  String content = "<html><body>";
  content += msg;
  content += "</body></html>";
  server.send(200, "text/html", content);
}

void setup ( void ) {
	pinMode ( led, OUTPUT );
	digitalWrite ( led, 0 );
	Serial.begin ( 115200 );
	initWifi();
	// EDG says: This is for automatically finding these things later, say, in an app
	if ( MDNS.begin ( "scintilla" ) ) {
		Serial.println ( "MDNS responder started" );
	}

	server.on ( "/setup", handleRoot );
	server.on ( "/test.svg", drawGraph );
	server.on("/configure", doConfigure );
	server.on("/", doStatus);
	server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println ( "HTTP server started" );
}

void loop ( void ) {
	server.handleClient();
}

void drawGraph() {
	String out = "";
	char temp[100];
	out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
 	out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
 	out += "<g stroke=\"black\">\n";
 	int y = rand() % 130;
 	for (int x = 10; x < 390; x+= 10) {
 		int y2 = rand() % 130;
 		sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
 		out += temp;
 		y = y2;
 	}
	out += "</g>\n</svg>\n";

	server.send ( 200, "image/svg+xml", out);
}


void doStatus() {
  
  String out = "<html><head><title>Scintilla Status</title>";
  out += "<style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style></head><body>";
  out += "<h1>Scintilla Configuration</h1>\n";
  
  StaticJsonBuffer<1000> jsonBuffer;
  int address = 2;
  length = word(EEPROM.read(0), EEPROM.read(1));
  for (address = 2; address < length + 2; address++) {
    buffer[address - 2] = EEPROM.read(address);
  }
  buffer[address - 2] = '\0';
  Serial.println(buffer);

  JsonObject& root = jsonBuffer.parseObject(buffer);
  if (!root.success())
  {
    Serial.println("parseObject() failed");
    out += "Current configuration invalid! Please set the configuration.\r\n";
    
  }
  else {
    numWifi = root["wifi"];
    host = root["host"];
    key = root["key"];
    id = root["id"];
    geo = root["geo"];
  
    Serial.println(numWifi);
    for (size_t i = 0; i < numWifi; i++)
    {
      ssidi = root["ssid"][i];
      pwdi = root["pwd"][i];
      Serial.print("["); Serial.print(i); Serial.print("]");
      Serial.print(ssidi); Serial.print(" - ");
      out += "SSID: " + (String)ssidi + "<br>";
      out += "Password" + (String)"*********" + "<br>";
      if (WiFi.status() != WL_CONNECTED) 
        out += "Status: DISCONNECTED!!!\r\n";
      else {
        out += "Status: CONNECTED!!!\r\n";
        out += "IP Address: " + (String)WiFi.localIP() + "\r\n";
        
      }
      Serial.println(pwdi);
    }
    Serial.println(host);
    Serial.println(key);
    Serial.println(id);
    Serial.println(geo);
    out += "Azure Info:<br>";
    out += "Host: " + (String)host + "<br";
    out += "Key: " + (String)key + "<br>";
    out += "id: " + (String)id + "<br>";
    out += "Geo: " + (String)geo + "<br>";
  }
  out += "<a href=\"/setup\">Edit configuration</a>";
  out += "</body></html>";
  server.send ( 200, "text/html", out);
  
}


