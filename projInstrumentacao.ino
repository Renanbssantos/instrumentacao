#include <PubSubClient.h>
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>

#define DHTTYPE DHT11   // DHT 11

//defines - mapeamento de pinos do NodeMCU
#define D0    16
#define D1    5
#define D2    4
#define D3    0
#define D4    2
#define D5    14
#define D6    12
#define D7    13
#define D8    15
#define D9    3
#define D10   1

//#define TOPICO_SUB_TEMP "MQTTEnviaTemp" //topico de escuta
#define TOPICO_PUB_TEMP "MQTTEnviaTemp" //envio de informações para broker mqtt
#define TOPICO_SUBSCRIBE "MQTTRecebe"
#define TOPICO_PUB_UMAR "MQTTEnviaUmid"
#define ID_MQTT "RENAN2-PC 6476"



const char* BROKER_MQTT = "test.mosquitto.org";
int BROKER_PORT = 1883;
WiFiClient espClient; //cria objeto espClient
PubSubClient MQTT(espClient); //variável armazena estado atual da saída

// nome e senha wifi pra roteamento pelo notebook
//const char* ssid = "RENAN2-PC 6476";
//const char* password = ";P16850u";

//const char* ssid = "motore";
//const char* password = "12345678";

const char* ssid = "Renan";
const char* password = "1234567890";

// Web Server on port 80
WiFiServer server(80);

// DHT Sensor
const int DHTPin = 5;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);



// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];


//protótopos
void VerificaConexoesWiFIEMQTT(void);
void mqtt_callback(char* topic,byte* payload, unsigned int length);
void initMQTT();
void reconnectMQTT();
void EnviaEstadoOutputMQTT();
void initSerial();
void initWiFi();
void reconnectWiFi();
void enviaTemperatura();

// only runs once on boot
void setup() {
  

  //inicializações:
    initSerial();
    initWiFi();
    initMQTT();
    
  // Initializing serial port for debugging purposes
  /*
  Serial.begin(9600);
  delay(10);
  
  dht.begin();
  
  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);
  
  // Printing the ESP IP address
  Serial.println(WiFi.localIP());
  */
}


//Função: inicializa comunicação serial com baudrate 115200 (para fins de monitorar no terminal serial 
//        o que está acontecendo.
//Parâmetros: nenhum
//Retorno: nenhum
void initSerial() 
{
    Serial.begin(9600);
}


//Função: inicializa e conecta-se na rede WI-FI desejada
//Parâmetros: nenhum
//Retorno: nenhum
void initWiFi() 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(ssid);
    Serial.println("Aguarde");
    
    reconnectWiFi();
}


void initMQTT(){
  MQTT.setServer(BROKER_MQTT, BROKER_PORT); //informa broker e porta a ser conectado
  MQTT.setCallback(mqtt_callback); //atribui função de chamada
}

void mqtt_callback(char* topic, byte* payload, unsigned int length){
  String msg;

  //obtem string do payload recebido
  for(int i = 0; i < length; i++){
    char c = (char)payload[i];
    msg += c;
  }
}

//Função: verifica o estado das conexões WiFI e ao broker MQTT. 
//        Em caso de desconexão (qualquer uma das duas), a conexão
//        é refeita.
//Parâmetros: nenhum
//Retorno: nenhum
void VerificaConexoesWiFIEMQTT(void)
{
    if(!MQTT.connected()){ 
        reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
    }
    if(WiFi.status() != WL_CONNECTED){
      reconnectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
    }
     
}


//Função: reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair)
//        em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito.
//Parâmetros: nenhum
//Retorno: nenhum
void reconnectMQTT() 
{
    while (!MQTT.connected()) 
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) 
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE); 
        } 
        else 
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Nova tentativa de conexao em 2s...");
            delay(2000);
        }
    }
}


//Função: reconecta-se ao WiFi
//Parâmetros: nenhum
//Retorno: nenhum
void reconnectWiFi() 
{
    //se já está conectado a rede WI-FI, nada é feito. 
    //Caso contrário, são efetuadas tentativas de conexão
    if (WiFi.status() == WL_CONNECTED)
        return;
        
    WiFi.begin(ssid, password); // Conecta na rede WI-FI
    
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
  
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");
    Serial.print(ssid);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}


//Função: envia ao Broker o estado atual do output 
//Parâmetros: nenhum
//Retorno: nenhum
void EnviaEstadoOutputMQTT(void){
    /*
    if (EstadoSaida == '0')
      MQTT.publish(TOPICO_PUBLISH, "D");
 
    if (EstadoSaida == '1')
      MQTT.publish(TOPICO_PUBLISH, "L");
 
    Serial.println("- Estado da saida D0 enviado ao broker!");
    delay(1000);
    */
}

void enviaTemperatura(void){
  MQTT.publish("hello","Hello CRL");
  float t = dht.readTemperature();
  dtostrf(t,6,2,celsiusTemp);
  MQTT.publish(TOPICO_PUB_TEMP,celsiusTemp);
    Serial.print("%\t Temperatura lida: ");
    Serial.println(t);
    
  float ht = dht.readHumidity();
  dtostrf(ht,6,2,humidityTemp);
  MQTT.publish(TOPICO_PUB_UMAR,humidityTemp);
  Serial.print("%ht Umidade lida: ");
  Serial.println(ht);
  
  delay(2000);
}

// runs over and over again
void loop() {


  //garante funcionamento das conexões WiFi e ao broker MQTT
  VerificaConexoesWiFIEMQTT();

  enviaTemperatura();
  
  //keep-alive da comunicação com broker MQTT
  MQTT.loop();
  
  // Listenning for new clients
  WiFiClient client = server.available();
  /*
  if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if (c == '\n' && blank_line) {
            // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
            float h = dht.readHumidity();
            // Read temperature as Celsius (the default)
            float t = dht.readTemperature();
            // Read temperature as Fahrenheit (isFahrenheit = true)
            float f = dht.readTemperature(true);
            // Check if any reads failed and exit early (to try again).
            if (isnan(h) || isnan(t) || isnan(f)) {
              Serial.println("Failed to read from DHT sensor!");
              strcpy(celsiusTemp,"Failed");
              strcpy(fahrenheitTemp, "Failed");
              strcpy(humidityTemp, "Failed");         
            }
            else{
              // Computes temperature values in Celsius + Fahrenheit and Humidity
              float hic = dht.computeHeatIndex(t, h, false);       
              dtostrf(hic, 6, 2, celsiusTemp);             
              float hif = dht.computeHeatIndex(f, h);
              dtostrf(hif, 6, 2, fahrenheitTemp);         
              dtostrf(h, 6, 2, humidityTemp);
              // You can delete the following Serial.print's, it's just for debugging purposes
              Serial.print("Humidity: ");
              Serial.print(h);
              Serial.print(" %\t Temperature: ");
              Serial.print(t);
              Serial.print(" *C ");
              Serial.print(f);
              Serial.print(" *F\t Heat index: ");
              Serial.print(hic);
              Serial.print(" *C ");
              Serial.print(hif);
              Serial.print(" *F");
              Serial.print("Humidity: ");
              Serial.print(h);
              Serial.print(" %\t Temperature: ");
              Serial.print(t);
              Serial.print(" *C ");
              Serial.print(f);
              Serial.print(" *F\t Heat index: ");
              Serial.print(hic);
              Serial.print(" *C ");
              Serial.print(hif);
              Serial.println(" *F");
            }
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            // your actual web page that displays temperature and humidity
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head></head><body><h1>ESP8266 - Temperature and Humidity</h1><h3>Temperature in Celsius: ");
            client.println(celsiusTemp);
            client.println("*C</h3><h3>Temperature in Fahrenheit: ");
            client.println(fahrenheitTemp);
            client.println("*F</h3><h3>Humidity: ");
            client.println(humidityTemp);
            client.println("%</h3><h3>");
            client.println("</body></html>");     
            break;
        }
        if (c == '\n') {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r') {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }  
    // closing the client connection
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }
  */
}   
