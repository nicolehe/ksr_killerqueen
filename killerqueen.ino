/*
 * Send tweet from @ksr_killerqueen when the giant button connected to pin 2 is pressed
 * uses api.thingspeak.com  as a Twitter proxy
 * see: http://community.thingspeak.com/documentation/apps/thingtweet/
 */ 

#include <SPI.h>       
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte server[]  = { xx, xx, xx, x };  // IP Address for the ThingSpeak API

// your ThingTweet API key
char *thingtweetAPIKey = "xxxxxxx";

char* messages[]={ // all the silly messages that will be tweeted in order when the button is pushed

"ALERT: Once more into the BEEch dear friends, once more!",
"ALERT: pic.twitter.com/zM31C1bSdc",
"ALERT: Veni, Vidi, BEEci",
"ALERT: Forget the roof - come collect berries downstairs.",
"ALERT: Cry “havoc!”, and let slip the bees of war.",
"ALERT: Go full military.",
"ALERT: pic.twitter.com/PNrLdclcL8",
"ALERT: It's time to ride the snail.",
"ALERT: BZZZZZZZZ!",
"ALERT: pic.twitter.com/WEXRJ87Xpp",
"ALERT: OMG why aren't you playing Killer Queen right now?",
"ALERT: Once more unto the hive dear friends, once more - or close the goal up with our dronish dead.",
"ALERT: Hey. You. Drop whatever useless thing you're doing right now and come play Killer Queen.",
"ALERT: Come give a Hive-Five!",
"ALERT: Are you a queen or a drone?",
"ALERT: Military. Economic. Snail.",
"ALERT: Are you blue or gold?",
"ALERT: pic.twitter.com/dychE9ki1J",
"ALERT: Family photo finish?",
"ALERT: Come down and play. Your teammates need you."



};

int curMes = 0; // current message number
int totMes = 0; // total number of messages


EthernetClient client;

boolean MsgSent = false;
const int Sensor = 2; // button is connected to pin 2

void setup()
{
  Serial.begin(9600);    
  if (Ethernet.begin(mac) == 0) { // start ethernet using mac & DHCP address
    Serial.println("Failed to configure Ethernet using DHCP");  
    while(true)   // no point in carrying on, so stay in endless loop:
      ;
  } 
  pinMode(Sensor, INPUT);
  digitalWrite(Sensor, HIGH);  //turn on  pull-up resistors
  delay(1000);
  Serial.println("Ready");  
  
  
  totMes = sizeof(messages)/sizeof(char*)-1;
  Serial.print("There are ");
    Serial.print(totMes+1);
  Serial.print(" messages ");
}

void loop(){
  if(digitalRead(Sensor) == HIGH) // if the button is pushed 
  { 
       MsgSent = sendMessage(messages[curMes]); // send tweet
   
       if(MsgSent)
           Serial.println(messages[curMes]);
       else    
           Serial.println("Unable tweet");

    if(curMes<totMes){ // if the current message number is less than the total number of messages
    curMes++; // add one to the message number
    Serial.print("curMes =");
    Serial.println(curMes);
    }
    else{ 
    curMes=0; // or else bring it back to the first message
    Serial.print("curMes =");
    Serial.println(curMes);
    }
   
   
  }
  else{
      MsgSent = false;  
  }
  delay(100);
}

boolean sendMessage( char *message)
{
boolean result = false;

  const int tagLen = 16; // the number of tag character used to frame the message
  int msgLen = strlen(message) + tagLen + strlen(thingtweetAPIKey);
  Serial.println("connecting ...");
  if (client.connect(server, 80) ) {
    Serial.println("making POST request...");
    client.print("POST /apps/thingtweet/1/statuses/update HTTP/1.1\r\n");
    client.print("Host: api.thingspeak.com\r\n");
    client.print("Connection: close\r\n");
    client.print("Content-Type: application/x-www-form-urlencoded\r\n");
    client.print("Content-Length: ");
    client.print(msgLen);
    client.print("\r\n\r\n");
    client.print("api_key=");          // msg tag
    client.print(thingtweetAPIKey);    // api key 
    client.print("&status=");          // msg tag 
    client.print(message);             // the message
    client.println("\r\n");
  }
  else {
    Serial.println("Connection failed");
  }
  // response string
  if (client.connected()) {
    Serial.println("Connected");
    if(client.find("HTTP/1.1") && client.find("200 OK") ){
       result = true;
    }
    else
       Serial.println("Dropping connection - no 200 OK");
  }
  else {
    Serial.println("Disconnected"); 
  }
  client.stop();
  client.flush();
  
  return result;
}
