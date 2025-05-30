#include <SoftwareSerial.h>

// Define os pinos RX e TX para o módulo HC-06
// Conectar HC-06 TX ao pino 10 do Arduino (RX)
// Conectar HC-06 RX ao pino 11 do Arduino (TX)
SoftwareSerial bluetooth(10, 11); // RX, TX

// Define os pinos para o controle do alarme
const int pinoEntrada = 7; 
const int pinoSaida = 4; 

bool alarmeAcionado = true; // O alarme começa ativado por padrão
bool SensorDetectou = false;
void setup() {
  Serial.begin(9600);    // Inicializa a comunicação serial de hardware para depuração
  bluetooth.begin(9900); // Inicializa a comunicação serial de software para o Bluetooth

  pinMode(pinoEntrada, INPUT);
  pinMode(pinoSaida, OUTPUT);

  // Estado inicial da saída do alarme
  digitalWrite(pinoSaida, HIGH); 
  Serial.println("Arduino com HC-06 pronto!");
  Serial.println("Alarme está inicialmente LIGADO.");
}

void loop() {
  // Lê dados do módulo Bluetooth
  if (bluetooth.available()) {
    char sinal = bluetooth.read();
    Serial.print("Recebido via Bluetooth: ");
    Serial.println(sinal);

    if (sinal == '0') { 
      alarmeAcionado = true;
      Serial.println("Alarme ATIVADO via Bluetooth.");
      bluetooth.println("Alarme ON"); // Envia confirmação de volta para o aplicativo
    } else if (sinal == '1') { 
      alarmeAcionado = false;
      Serial.println("Alarme DESATIVADO via Bluetooth.");
      bluetooth.println("Alarme OFF");
      
    }
  }

  // Verifica o sensor do alarme somente se o alarme estiver ativado
  if (alarmeAcionado) {
    if ((digitalRead(pinoEntrada) == HIGH) && !SensorDetectou ) { // Se o sensor estiver HIGH (sem gatilho)
      digitalWrite(pinoSaida, LOW); // Alarme DESLIGADO (sem 0V)
      
    } else { // Se o sensor estiver LOW (acionado)
      digitalWrite(pinoSaida, HIGH); // Alarme LIGADO (5V no pino 4)
      SensorDetectou = true;
      Serial.println("Alarme acionado!");
    }
  } else {
    // Se o alarme estiver desativado, garante que a saída permaneça desligada
    digitalWrite(pinoSaida, LOW); // Alarme DESLIGADO
    SensorDetectou = false;
  }
}
