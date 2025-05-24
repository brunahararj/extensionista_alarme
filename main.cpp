void setup() {
  Serial.begin(9600);
  pinMode(7, INPUT);
  pinMode(4, OUTPUT);
}

void loop() {
  if (digitalRead(7)){
    digitalWrite(4,0);
    }
    else {digitalWrite(4,1);
    Serial.println("Alarme acionado");
}
}