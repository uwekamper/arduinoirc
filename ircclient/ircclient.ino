#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,2,2);

// Enter the IP address of the server you're connecting to:\\
//IPAddress server(192,168,2,1);
IPAddress server(38,229,70,20);

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 23 is default for telnet;
// if you're using Processing's ChatServer, use  port 10002):
EthernetClient client;

void setup() {
  // start the Ethernet connection:
  Ethernet.begin(mac, ip);
  delay(2000);
  // start the serial library:
  Serial.begin(9600);
}



void loop()
{
  if (!client.connected()) {
    Serial.println("connecting ...");
    if (client.connect(server, 6667)) {
      Serial.println("connected");
      delay(1000);
      client.println("USER  uk 8 *  : Uwe Kamper\r");
      delay(500);
      client.println("NICK arduinoirc\r");
      delay(500);
      client.println("JOIN #c-base\r");
      delay(500);
      handle_irc_connection();
    } 
    else {
      // if you didn't get a connection to the server:
      Serial.println("connection failed");
      delay(2000);
    }
  }
}
  
#define IRC_BUFSIZE  32
char from[IRC_BUFSIZE];
char type[IRC_BUFSIZE];
char to[IRC_BUFSIZE];

void handle_irc_connection() {
  char c;
  // if there are incoming bytes available 
  // from the server, read them and print them:
  while(true) {
    if (!client.connected()) {
      return;
    }
    if(client.available()) {
      c = client.read();
    }
    else {
      continue;
    }
    
    if(c == ':') {
      memset(from, 0, sizeof(from));
      memset(type, 0, sizeof(type));
      memset(to, 0, sizeof(to));
      
      read_until(' ', from);
      read_until(' ', type);
      read_until(' ', to);
     
      if(strcmp(type, "PRIVMSG") == 0) {
        print_nick(from);
        ignore_until(':');
        print_until('\r');
      }
      else {
        
        ignore_until('\r');
      } 
    }
    // could be a PING request by the server.
    else if (c == 'P') {
      char buf[5];
      memset(buf, 0, sizeof(buf));
      buf[0] = c;
      for(int i = 1; i < 4; i++) {
        c = client.read();
        buf[i] = c;
      }
      ignore_until('\r');
      if(strcmp(buf, "PING") == 0) {
        client.println("PONG\r");
        Serial.println("PING->PONG");
      }
    }
  } // end while
   
}

void print_nick(char buffer[]) {
  Serial.print("<");
  for(int i = 0; i < IRC_BUFSIZE - 1; i++) {
    if(buffer[i] == '!') { 
      break; 
    }    
    Serial.print(buffer[i]);
  }
  Serial.print(">");
}

int read_until(char abort_c, char buffer[]) {
  int bytes_read = 0;
  memset(buffer, 0, sizeof(buffer));
  for(int i = 0; i < IRC_BUFSIZE - 1; i++) {
    if (client.available()) {
      char c = client.read();  
      bytes_read++;
      if(c == abort_c) {
        return bytes_read;
      }
      else if (c == '\n') {
        return bytes_read;
      }
      buffer[i] = c;
    }
  }
  ignore_until(abort_c);
  return bytes_read;
}

// reads characters from the connection until
// it hits the given character.
void ignore_until(char c) {
  while(true){
    if (client.available()) {
      char curr_c = client.read();
      if (curr_c == c) {
        return;  
      }
    }
  }
}

// reads characters from the connection until
// it hits the given character.
void print_until(char c) {
  while(true){
    if (client.available()) {
      char curr_c = client.read();
      if (curr_c == c) {
        Serial.println("");
        return;
      }
      Serial.print(curr_c);
    }
  }
}

// reads characters from the connection until
// it hits the given character.
void print_until_endline() {
  while(true){
    if (client.available()) {
      char curr_c = client.read();
      if (curr_c == '\r') {
        curr_c = client.read();
        if (curr_c == '\n') { return; }
      }
      Serial.print(curr_c);
    }
  }
}
