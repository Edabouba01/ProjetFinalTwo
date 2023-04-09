#include <Arduino.h>

/*Pour Bme*/

#include <AsyncElegantOTA.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>



#include <WiFi.h>
/*Pour Ota*/
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

#include <Wire.h>
#include <SPI.h>
// Replace with your network credentials
const char *ssid = "BELL382";
const char *password = "E9991FC25CF7";

Adafruit_BME280 bme; // I2C

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap-icons@1.10.3/font/bootstrap-icons.css">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-rbsA2VBKQhggwzxH7pPCaAqO46MgnOM80zW1RWuH61DGLwZJEdK2Kadq2F9CUG65" crossorigin="anonymous">

  <style>
  
  .container
{
    margin-top: 150px;
}
 
#temp{
    color: rgb(190, 149, 37);
    font-size: 25px;
}
#Humid{
    color: aqua;
    font-size: 25px;
}
#Press{
    color: rgb(19, 148, 105);
    font-size: 25px;
}
#Aliti{
    color: rgba(61, 16, 103, 0.442);
    font-size: 25px;
}

.temp-icon{
    color: rgb(190, 173, 11);
}
.humid-icon {
    color: aqua;
}
.pressure-icon{
    color: rgb(19, 148, 105);

}
.altitude-icon{
 color:    rgba(31, 5, 56, 0.94);
}
sup{
    font-size: 15px;;
}
  </style>
</head>
<body>
    <div class="container ">
        <div class="row text-center ">
          <div class="col bordure">
            <h1> ESP32 Weather Station </h1>
          </div>
        </div><br><br>
        <div class="row">
          
          <div class="col-8 bordure text-center">
            <h2>   <i class="bi bi-thermometer-half temp-icon"></i>        Temperature   </h2>
          </div>
          <div class="col-4 bordure ">
            <span id="temp"> </span><sup>°C</sup>
          </div><br>
          
        </div>
            <div class="row">
             <div class="col-8 bordure text-center">
                <h2>  <i class="bi bi-cloud-drizzle humid-icon "></i>        Humidity</h2>
                
             </div>
             <div class="col-4 bordure">
                <span id="Humid">  </span><sup>°F</sup>
              </div><br>
              
         </div>
        <div class="row">


          <div class="col-8 bordure text-center">
            <h2> <i class="bi bi-clock-history pressure-icon"></i>       Pressure</h2>
            
              </div>

              <div class="col-4 bordure">
                <span id="Press">  </span><sup>hpa</sup>
                  </div><br>
        </div>

        <div class="row">
        
          <div class="col-8 bordure text-center">
            <h2><i  class="bi bi-triangle altitude-icon"></i>         Altitude</h2>
              </div>

              <div class="col-4 bordure">
                <span id="Aliti">  </span><sup>m</sup>
                  </div>
        </div>
      </div>

   <script src="/js/contenu.js"></script>
      <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.6.3/jquery.min.js"></script>
      


    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-kenU1KFdBIe4zVF0s0G1M5b4hcpxyD9F7jL+jjXkk+Q2h455rYXK/7HAuoJl+0I4" crossorigin="anonymous"></script>

</body>

</html>)rawliteral";

/*contenu Js */
const char *js_contenu = R"rawliteral(  
const url = window.location.protocol + '//' + window.location.hostname + '/Renvoi'; // URL du fichier JSON

function meteo() {
  fetch(url)
    .then((response) => response.json()) 
    .then((Renvoi) => {
      
      document.querySelector("#temp").textContent = Renvoi.VlTemperature;
      document.querySelector("#Humid").textContent = Renvoi.VlHumidite;
      document.querySelector("#Press").textContent = Renvoi.VlPression;
      document.querySelector("#Aliti").textContent = Renvoi.VlAltitude;
    })
    .catch((error) => console.error(error));
    
} 
meteo();//changer les donnee de base
// Mettez à jour les données toutes les 3 secondes


// rafraîchit les données toutes les 3 secondes
setInterval(meteo, 3000);
   
)rawliteral";

void setup() {
  
  // Serial port for debugging purposes
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html); });

  // Route for root / Requet JS
  server.on("/js/contenu.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "application/javascript", js_contenu); });

  // Créez une route pour envoyer les données du capteur au format JSON
  server.on("/Renvoi", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String json = "{";
    json += "\"VlTemperature\": " + String(bme.readTemperature()) + ",";
    json += "\"VlHumidite\": " + String(bme.readHumidity()) + ",";
    json += "\"VlPression\": " + String(bme.readPressure() / 100.0F) + ",";
    json += "\"VlAltitude\": " + String(bme.readAltitude(1013.25));
    json += "}";
    
    request->send(200, "application/json", json); });

  // Start ElegantOTA
  AsyncElegantOTA.begin(&server);
  // Start server
  server.begin();
  Serial.println("HTTP server started");
  
  //Test BME
  Serial.println(F("BME280 test"));
  if (!bme.begin())
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }

  bool status; // Pour prendre l etat des status

  // Voir si le Bme280 est prise en charge
  status = bme.begin(0x76);
  if (!status)
  {
    Serial.println("BME non disponible ");
  }

  // lecture des valeurs de temperature, pression, altitude et humidite depuis le bme
  float Temperature = bme.readTemperature();
  float Humidite = bme.readHumidity();
  float Pression = bme.readPressure() / 100.0F;
  float Altitude = bme.readAltitude(1013.25);
  
  //Affichage dans le  Serial

  Serial.print(F("Temperature = "));
  Serial.print(Temperature);
  Serial.println(" *C");
  Serial.println("La valeur de lhumidite");
  Serial.println(Humidite);
  Serial.println(" *F");
  Serial.print(F("Pression = "));
  Serial.println(Pression);
  Serial.println(" Pa");
  Serial.print(F("Approx altitude = "));
  Serial.print(Altitude);
  Serial.println(" m");
  Serial.println();
  delay(3000);

  // Lancer le Web
  server.begin();
}

void loop() {
  
}