// Load Wi-Fi library
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid = "Communism";
const char* password = "00001111";

// Set web server port number to 80
WiFiServer server (80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output5State = "off";
String output4State = "off";

// Assign output variables to GPIO pins
const int output5 = 5;
const int output4 = 4;

void setup () {
    Serial.begin (115200);
    // Initialize the output variables as outputs
    pinMode (output5, OUTPUT);
    pinMode (output4, OUTPUT);
    // Set outputs to LOW
    digitalWrite (output5, LOW);
    digitalWrite (output4, LOW);

    // Connect to Wi-Fi network with SSID and password
    Serial.print ("Connecting to ");
    Serial.println (ssid);
    WiFi.begin (ssid, password);
    while (WiFi.status () != WL_CONNECTED) {
        delay (500);
        Serial.print (".");
        }
    // Print local IP address and start web server
    Serial.println ("");
    Serial.println ("WiFi connected.");
    Serial.println ("IP address: ");
    Serial.println (WiFi.localIP ());
    server.begin ();
    }

void loop () 
    {
    // Listen for incoming clients
    WiFiClient client = server.available (); 

    // If a new client connects
    if (client) 
        {
        // print a message out in the serial port
        Serial.println ("New Client.");
        
        // make a String to hold incoming data from the client
        String currentLine = "";                
        
        // while the client's connected
        while (client.connected ()) 
            {            
            // if there's bytes to read from the client,
            if (client.available ()) 
                {
                // read a byte, then
                char c = client.read ();
                // print it out the serial monitor
                Serial.write (c);   

                header += c;
                // if the byte is a newline character
                if (c == '\n') 
                    {
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length () == 0) 
                        {
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        client.println ("HTTP/1.1 200 OK");
                        client.println ("Content-type:text/html");
                        client.println ("Connection: close");
                        client.println ();

                        // turns the GPIOs on and off
                        if (header.indexOf ("GET /5/on") >= 0) 
                            {
                            Serial.println ("GPIO 5 on");
                            output5State = "on";
                            digitalWrite (output5, HIGH);
                            }
                        else if (header.indexOf ("GET /5/off") >= 0) 
                            {
                            Serial.println ("GPIO 5 off");
                            output5State = "off";
                            digitalWrite (output5, LOW);
                            }
                        else if (header.indexOf ("GET /4/on") >= 0) 
                            {
                            Serial.println ("GPIO 4 on");
                            output4State = "on";
                            digitalWrite (output4, HIGH);
                            }
                        else if (header.indexOf ("GET /4/off") >= 0) 
                            {
                            Serial.println ("GPIO 4 off");
                            output4State = "off";
                            digitalWrite (output4, LOW);
                            }

                        // Display the HTML web page
                        client.println ("<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'><style>html { font-family: Helvetica;display: inline-block;margin: 0px auto;text-align: center;background-color: #151515;color: white; } .sidenav { height: 100%; width: 0; position: fixed; z-index: 1; top: 0; left: 0;background-color: #111;overflow-x: hidden;padding-top: 60px;transition: 0.5s; }.sidenav a{padding: 8px 8px 8px 32px;text-decoration: none;font-size: 25px;color: #818181;display: block;transition: 0.3s;}.sidenav a:hover {color: #f1f1f1;}.sidenav .closebtn {position: absolute;top: 0;right: 25px;font-size: 36px;margin-left: 50px;}</style></head><body><div id ='sideNav' class='sidenav':><a href ='javascript:void(0)'class='closebtn' onClick ='closeNav ()'>&times</a><a href ='light.html'>Light</a><a href ='light.html'>Light</a><a href ='light.html'>Light</a><a href ='light.html'>Light</a></div><div id ='main'><h1>Room remote</h1><span style='font-size:30px; cursor:pointer' onclick='openNav()'>&#9776; Menu</span></div><script>function openNav() {document.getElementById('sideNav').style.width = '400px';}</script><script>function closeNav() {document.getElementById('sideNav').style.width = '0';}</script></body></html>");

                        // The HTTP response ends with another blank line
                        client.println ();
                        // Break out of the while loop
                        break;
                        }
                    // if you got a newline, then clear currentLine
                    else 
                        currentLine = "";   
                    }
                // if you got anything else but a carriage return character,
                // add it to the end of the currentLine
                else if (c != '\r')
                    currentLine += c;

                }
            }
        // Clear the header variable
        header = "";
        // Close the connection
        client.stop ();
        Serial.println ("Client disconnected.");
        Serial.println ("");
        }
    }