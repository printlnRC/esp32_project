#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <DFRobot_HX711.h>

// Configuration WiFi
const char* ssid = "Xiaomi 11T Pro";
const char* password = "123456789";

// Partie batterie
const int batterie = 34;
int bat = 0; // Pourcentage de la batterie

WebServer server(80); // Serveur web sur le port 80

void setup() {

  // Initialisation du système de fichiers SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connexion au réseau WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  /* Configuration des routes pour les fichiers statiques
  server.on("/batterie.png", HTTP_GET, []() {
    File file = SPIFFS.open("/batterie.png", "r");
    server.streamFile(file, "image/png");
    file.close();
  });*/


  server.on("/", HTTP_GET, []() {
    File file = SPIFFS.open("/index.html", "r");
    if (!file) {
      Serial.println("File not found");
      return;
    }
    server.streamFile(file, "text/html");
    file.close();
  });

  server.on("/style.css", HTTP_GET, []() {
    File file = SPIFFS.open("/style.css", "r");
    server.streamFile(file, "text/css");
    file.close();
  });

  server.on("/script.js", HTTP_GET, []() {
    File file = SPIFFS.open("/script.js", "r");
    server.streamFile(file, "application/javascript");
    file.close();
  });

  /* Configuration des routes pour les données dynamiques
  server.on("/status", HTTP_GET, []() {
    server.send(200, "text/plain", String(pourcentage));
  });*/
}

void loop() {
  server.handleClient(); // Gestion des requêtes clients

  int analogValue = analogRead(batterie); // Lecture de la valeur analogique de la batterie
  bat = map(analogValue, 2740, 4095, 0, 100); // Conversion de la valeur analogique en pourcentage
 
// affichage niveau batterie
  Serial.print("Pourcentage de batterie: ");
  Serial.print(bat);
  Serial.println("%");

  delay(500); // Attente de 500ms avant la prochaine mesure
}
