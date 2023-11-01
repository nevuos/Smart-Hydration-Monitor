#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // RX, TX

// Configurações do sensor e leituras
const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;           
int total = 0;

// Estados e parâmetros do Filtro de Kalman
float kalman_gain = 0.0;
float estimate = 0.0;
float estimate_error = 2.0; // Erro de estimativa
float measurement_error = 3.0; // Erro de medição do sensor

// Parâmetros do controle da bomba
bool pumpState = false;
int lowerThreshold = 60;
int upperThreshold = 65;

unsigned long lastPumpTime = 0;
const unsigned long pumpInterval = 3600000; // Intervalo para controle baseado em tempo (1 hora)

void setup() {
  pinMode(3, OUTPUT);
  Serial.begin(9600);
  BTSerial.begin(9600);
  initializeReadings();
}

void loop() {
  handleBluetoothCommands();

  int percentage = readAndFilterSoilMoisture();
  if (percentage == -1) {
    sendAlert("Sensor error!");
    return;
  }

  updateStatus(percentage); // Enviar status atual para Serial e Bluetooth
  controlPump(percentage);
  timeBasedPumpControl();
  delay(1000);
}

void initializeReadings() {
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
}

void handleBluetoothCommands() {
  if (BTSerial.available()) {
    char c = BTSerial.read();
    // Ajuste dos limites de umidade via Bluetooth
    if (c == 'L') lowerThreshold = BTSerial.parseInt();
    if (c == 'U') upperThreshold = BTSerial.parseInt();
  }
}

int readAndFilterSoilMoisture() {
  int reading = analogRead(A0);
  if (reading < 0 || reading > 1023) return -1; // Verificação de erro de leitura

  return applyKalmanFilter(reading);
}

int applyKalmanFilter(int reading) {
  float measurement = map(reading, 355, 1023, 100, 0);
  kalman_gain = estimate_error / (estimate_error + measurement_error);
  estimate = estimate + kalman_gain * (measurement - estimate);
  estimate_error = (1.0 - kalman_gain) * estimate_error;
  return (int)estimate;
}

void controlPump(int moisturePercentage) {
  if (!pumpState && moisturePercentage < lowerThreshold) {
    pumpOn();
  } else if (pumpState && moisturePercentage > upperThreshold) {
    pumpOff();
  }
}

void pumpOn() {
  digitalWrite(3, LOW);
  pumpState = true;
  Serial.println(" - Pump on");
  BTSerial.println(" - Pump on");
}

void pumpOff() {
  digitalWrite(3, HIGH);
  pumpState = false;
  Serial.println(" - Pump off");
  BTSerial.println(" - Pump off");
}

void timeBasedPumpControl() {
  if (millis() - lastPumpTime > pumpInterval && !pumpState) {
    pumpOn();
    lastPumpTime = millis();
  }
}

void updateStatus(int moisturePercentage) {
  Serial.print("Soil Moisture: ");
  Serial.println(moisturePercentage);
  BTSerial.print("Soil Moisture: ");
  BTSerial.println(moisturePercentage);
}

void sendAlert(String message) {
  Serial.println(message);
  BTSerial.println(message);
}
