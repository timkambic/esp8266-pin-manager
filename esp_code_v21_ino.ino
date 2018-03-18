#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "website.h"
#include "javascript.h"

ESP8266WebServer server(80);
const char* ssid="HP404";
const char* password="brezgesla";
String XML;
String XML_multi;

void buildXMLresponse(int data){
  XML="<?xml version='1.0'?>";
  XML+="<response>";
  XML+=String(data);
  XML+="</response>";
}
void buildXML_multiresponse(int data[10]){
	XML_multi="<?xml version='1.0'?>";
	XML_multi+="<response>";
	for(int i=0;i<10;i++){
		XML_multi+="<val>";
		XML_multi+=String(data[i]);
		XML_multi+="</val>";
	}
	XML_multi+="</response>";
}


void handleWebsite(){
	Serial.println("serving index");
	server.send(200,"text/html",html_page);
}

void serve_javascript(){
	server.send(200, "text/javascript",javascript_file);
	Serial.println("serving js");
}

void readPins(){
	// mogoce najd neki k bi poslal samo ko se spremeni kerkor od pinov 
	String pin_str = server.arg(0);
	int pin = pin_str.toInt();
	int state = digitalRead(pin);
	Serial.print("Reading pin:");Serial.print(pin);Serial.print("  - pin is:");Serial.println(state);
	buildXMLresponse(state);
	server.send(200,"text/xml",XML);
}


void readMultiPins(){
	String pin_str = server.arg(0);
	char pin[12];
	int response[10];
	pin_str.toCharArray(pin,12);
	Serial.print("MULTIREAD received:"); Serial.println(pin);
		
	response[0]=3;response[1]=3;response[2]=3;response[3]=3;
	if(pin[4] == '1'){
		response[4] = digitalRead(4);
	}
	else {response[4]=2;}
	
	if(pin[5] == '1'){
		response[5] = digitalRead(5);
	}
	else {response[5]=2;}
	
	if(pin[6] == '1'){
		response[6] = digitalRead(12);
	}
	else {response[6]=2;}
	
	if(pin[7] == '1'){
		response[7] = digitalRead(13);
	}
	else {response[7]=2;}
	
	if(pin[8] == '1'){
		response[8] = digitalRead(14);
	}
	else {response[8]=2;}
	
	if(pin[9] == '1'){
		response[9] = digitalRead(16);
	}
	else {response[9]=2;}
	
	Serial.print("response:");
	for(int a=0;a<10;a++){
		Serial.print(response[a]);
		Serial.print(",");
	}
	
	buildXML_multiresponse(response);
	Serial.println(XML_multi);
	server.send(200,"text/xml",XML_multi);
}

void togglePins(){ // togglePins?pin=1&state=1
	String pin_str = server.arg(0);
	int pin = pin_str.toInt();
	String state_str = server.arg(1);
	int state;
	if(state_str == "true"){state=1;} else if(state_str =="false"){state=0;}
	Serial.print("Toggling pin:");Serial.print(pin);Serial.print(" to:");Serial.println(state);
	
	buildXMLresponse(state);
	digitalWrite(pin, state);
	server.send(200,"text/xml",XML);
	Serial.print("(togglePins)response sent: "); Serial.println(state);
}

void setPins(){ //setPins?pin=1&state=INPUT
	String pin_str = server.arg(0);
	int pin = pin_str.toInt();

	Serial.print("Changing pin:");Serial.print(pin);Serial.print(" to:");Serial.println(server.arg(1));
	//pinMode();
	int response;
	if( server.arg(1) == "INPUT"){ // send back the oposite of received 
		response = 1;
		pinMode(pin, INPUT);
	}
	else if(server.arg(1) == "OUTPUT"){
		response = 0;
		pinMode(pin,OUTPUT);
	}
	else{
		response = 3;
	}
	buildXMLresponse(response); 
	server.send(200,"text/xml",XML);
	Serial.print("(setPins)response sent: "); Serial.println(response);
}

void setup() {
	pinMode(3,INPUT);
	pinMode(4,INPUT);
	pinMode(5,INPUT);
	pinMode(12,INPUT);
	pinMode(13,INPUT);
	pinMode(14,INPUT);
	pinMode(16,INPUT);
	
	Serial.begin(115200);  
	Serial.println("\n\nBOOTING ESP8266 and connecting to WiFi");
	WiFi.begin(ssid,password);
	while(WiFi.status()!=WL_CONNECTED){
	  delay(500);
	  Serial.print(".");
	}  
	WiFi.mode(WIFI_STA);
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
	Serial.print("free heap:");Serial.println(ESP.getFreeHeap());
	
	server.on("/",handleWebsite);
	server.on("/script.js",serve_javascript);
	server.on("/readPins",readPins);
	server.on("/togglePins",togglePins);
	server.on("/setPins",setPins);
	server.on("/readMultiPins", readMultiPins);
	
	server.begin(); 
	Serial.println("server started");
}

void loop() {
  server.handleClient();
}
