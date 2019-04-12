// ENERGY MONITOR INITIAL
#include "EmonLib.h"             // Include Emon Library
EnergyMonitor emon1,emon2;       // Create an instance

// WEBSERVER INITIAL
#include <SPI.h>
#include <Ethernet.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xAA, 0xBB, 0xCC };   //physical mac address
byte ip[] = { 192, 168, 1, 244 };                      // ip in lan (that's what you need to use in your browser. ("192.168.1.244")
byte gateway[] = { 192, 168, 1, 1 };                   // internet access via router
byte subnet[] = { 255, 255, 255, 0 };                  //subnet mask
EthernetServer server(80);

// VARIABLES
float realpowerA;
float apparentpowerA;
float powerfactorA;
float supplyvoltageA;
float supplycurrentA;
float realpowerB;
float apparentpowerB;
float powerfactorB;
float supplyvoltageB;
float supplycurrentB;
float supplyvoltageC;
float supplyrealC;
float supplyapparentC;


void setup(){  
  Serial.begin(9600);

  // Energy Monitor Calibrations
  emon1.voltage(0, 115, 1.4);    // Voltage: input pin, calibration, phase_shift
  emon1.current(1, 455);          // Current: input pin, calibration.
  emon2.voltage(2, 115, 1.4);    // Voltage: input pin, calibration, phase_shift
  emon2.current(3, 455);          // Current: input pin, calibration.

  // Start Ethernet HOST
  Ethernet.begin(mac, ip);              // Initialize Hardware Settings 
  server.begin();                       // Start HTTP Server
  Serial.print("Server is at ");        // Report IP to serial
  Serial.println(Ethernet.localIP());


}

void loop(){

  emon1.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
  emon1.serialprint();           // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)
  
  float realpowerA       = emon1.realPower;        //extract Real Power into variable
  float apparentpowerA   = emon1.apparentPower;    //extract Apparent Power into variable
  float powerfactorA     = emon1.powerFactor;      //extract Power Factor into Variable
  float supplyvoltageA   = emon1.Vrms;             //extract Vrms into Variable
  float supplycurrentA   = emon1.Irms;             //extract Irms into Variable

  emon2.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
  emon2.serialprint();           // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)
  
  float realpowerB       = emon2.realPower;        //extract Real Power into variable
  float apparentpowerB   = emon2.apparentPower;    //extract Apparent Power into variable
  float powerfactorB     = emon2.powerFactor;      //extract Power Factor into Variable
  float supplyvoltageB   = emon2.Vrms;             //extract Vrms into Variable
  float supplycurrentB   = emon2.Irms;             //extract Irms into Variable

  float supplyvoltageC   = ((supplyvoltageA)+(supplyvoltageB));
  float supplyrealC      = ((realpowerA)+(realpowerB));
  float supplyapparentC  = ((apparentpowerA)+(apparentpowerB));

  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 60");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");

          client.println("Main Leg A");
          client.print("Amps: ");
          client.print(supplycurrentA);
          client.print("A      Volts:");
          client.print(supplyvoltageA);
          client.println("<br />");
          client.println("Main Leg B");
          client.print("Amps: ");
          client.print(supplycurrentB);
          client.print("A      Volts:");
          client.print(supplyvoltageB);
          client.println("<br />");
          client.println("<br />");
          client.println("<br />");
          client.println("<br />");
          client.println("<br />");
          client.println("<br />");

          client.print("<realpowerA>");
          client.print(realpowerA);
          client.print("</realpowerA>");
          client.println("<br />");
          client.print("<realpowerB>");
          client.print(realpowerB);
          client.print("</realpowerB>");
          client.println("<br />");
          client.print("<apparentpowerA>");
          client.print(apparentpowerA);
          client.print("</apparentpowerA>");
          client.println("<br />");
          client.print("<apparentpowerB>");
          client.print(apparentpowerB);
          client.print("</apparentpowerB>");
          client.println("<br />");
          client.print("<powerfactorA>");
          client.print(powerfactorA);
          client.print("</powerfactorA>");
          client.println("<br />");
          client.print("<powerfactorB>");
          client.print(powerfactorB);
          client.print("</powerfactorB>");
          client.println("<br />");
          client.print("<supplyvoltageA>");
          client.print(supplyvoltageA);
          client.print("</supplyvoltageA>");
          client.println("<br />");
          client.print("<supplyvoltageB>");
          client.print(supplyvoltageB);
          client.print("</supplyvoltageB>");
          client.println("<br />");
          client.print("<supplycurrentA>");
          client.print(supplycurrentA);
          client.print("</supplycurrentA>");
          client.println("<br />");
          client.print("<supplycurrentB>");
          client.print(supplycurrentB);
          client.print("</supplycurrentB>");
          client.println("<br />");
          client.print("<supplyvoltageC>");
          client.print(supplyvoltageC);
          client.print("</supplyVoltageC>");
          client.println("<br />");
          client.print("<supplyrealC>");
          client.print(supplyrealC);
          client.print("</supplyrealC>");
          client.println("<br />");
          client.print("<supplyapparentC>");
          client.print(supplyapparentC);
          client.print("</supplyapparentC>");
          client.println("<br />");

          client.println("</html>");
          break;
        }


        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }

    delay(10);        // give the web browser time to receive the data
    client.stop();    // close the connection:
    Serial.println("client disconnected");
 

  float supplycurrentM (supplycurrentA+supplycurrentB);
  Serial.println(realpowerA);
  Serial.println(realpowerB);
  Serial.println(apparentpowerA);
  Serial.println(apparentpowerB);
  Serial.println(powerfactorA);
  Serial.println(powerfactorB);
  Serial.println(supplyvoltageA);
  Serial.println(supplyvoltageB);
  Serial.println(supplycurrentA);
  Serial.println(supplycurrentB);
  Serial.println(supplycurrentM);
  Serial.println("......................");
}
}
