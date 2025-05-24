#include <SoftwareSerial.h>

// Define os pinos RX e TX para o módulo HC-06
// Conectar HC-06 TX ao pino 10 do Arduino (RX)
// Conectar HC-06 RX ao pino 11 do Arduino (TX)
SoftwareSerial bluetooth(10, 11); // RX, TX

// Define os pinos para o controle do alarme
const int alarmSensorPin = 7; // Pino de entrada para o seu sensor de alarme
const int alarmOutputPin = 4; // Pino de saída para controlar o alarme (ex: uma sirene ou LED)

// Variável para armazenar o estado do alarme (ligado/desligado)
bool alarmEnabled = true; // O alarme começa ativado por padrão

void setup() {
  Serial.begin(9600);    // Inicializa a comunicação serial de hardware para depuração
  bluetooth.begin(9900); // Inicializa a comunicação serial de software para o Bluetooth

  pinMode(alarmSensorPin, INPUT);
  pinMode(alarmOutputPin, OUTPUT);

  // Estado inicial da saída do alarme
  digitalWrite(alarmOutputPin, HIGH); // Assumindo que HIGH significa que o alarme está DESLIGADO (sem 0V)
                                      // baseado na lógica do seu código original
  Serial.println("Arduino com HC-06 pronto!");
  Serial.println("Alarme está inicialmente LIGADO.");
}

void loop() {
  // Lê dados do módulo Bluetooth
  if (bluetooth.available()) {
    char receivedChar = bluetooth.read();
    Serial.print("Recebido via Bluetooth: ");
    Serial.println(receivedChar);

    if (receivedChar == '1') { // Assumindo que '1' LIGA o alarme
      alarmEnabled = true;
      Serial.println("Alarme ATIVADO via Bluetooth.");
      bluetooth.println("Alarme ON"); // Envia confirmação de volta para o aplicativo
    } else if (receivedChar == '0') { // Assumindo que '0' DESLIGA o alarme
      alarmEnabled = false;
      Serial.println("Alarme DESATIVADO via Bluetooth.");
      bluetooth.println("Alarme OFF"); // Envia confirmação de volta para o aplicativo
      digitalWrite(alarmOutputPin, HIGH); // Garante que o alarme esteja desligado quando desativado
    }
  }

  // Verifica o sensor do alarme somente se o alarme estiver ativado
  if (alarmEnabled) {
    if (digitalRead(alarmSensorPin) == HIGH) { // Se o sensor estiver HIGH (sem gatilho)
      digitalWrite(alarmOutputPin, LOW); // Alarme DESLIGADO (sem 0V)
    } else { // Se o sensor estiver LOW (acionado)
      digitalWrite(alarmOutputPin, HIGH); // Alarme LIGADO (5V no pino 4, baseado no comentário do seu código original)
      Serial.println("Alarme acionado!");
      bluetooth.println("ALARM TRIGGERED!"); // Notifica o aplicativo
    }
  } else {
    // Se o alarme estiver desativado, garante que a saída permaneça desligada
    digitalWrite(alarmOutputPin, HIGH); // Alarme DESLIGADO (sem 0V)
  }
}