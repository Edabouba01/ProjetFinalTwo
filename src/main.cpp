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
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}