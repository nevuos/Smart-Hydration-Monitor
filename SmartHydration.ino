#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // RX, TX - Cria uma porta serial para comunicação Bluetooth nos pinos 10 (RX) e 11 (TX)

const int numReadings = 10;  // Número de leituras para calcular a média
int readings[numReadings];   // Array para armazenar as leituras do sensor
int readIndex = 0;           // Índice atual no array de leituras
int total = 0;               // Total acumulado das leituras para calcular a média
int average = 0;             // Média das leituras

int soilMoistureValue = 0;   // Valor atual de umidade do solo
int percentage = 0;          // Umidade do solo em percentagem baseada na média
int instantPercentage = 0;   // Umidade do solo em percentagem baseada na leitura instantânea
int displayedPercentage = -1;  // Última percentagem exibida. Inicializada com -1 para garantir a primeira atualização
const int threshold = 10;    // Limiar para considerar uma mudança significativa na umidade

void setup() {
  pinMode(3, OUTPUT);        // Configura o pino 3 como saída (para a bomba)
  Serial.begin(9600);        // Inicia comunicação serial com o computador
  BTSerial.begin(9600);      // Inicia comunicação serial Bluetooth

  // Inicializa o array de leituras com 0
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
}

void loop() {
  // Lê a umidade atual do solo
  soilMoistureValue = analogRead(A0);
  total = total - readings[readIndex];
  readings[readIndex] = soilMoistureValue;
  total = total + readings[readIndex];
  readIndex = (readIndex + 1) % numReadings;
  average = total / numReadings;

  // Mapeia a média e a leitura instantânea para percentagens
  percentage = map(average, 610, 1023, 100, 0);
  instantPercentage = map(soilMoistureValue, 610, 1023, 100, 0);

  // Atualiza a percentagem exibida se houver uma mudança significativa
  if (abs(instantPercentage - displayedPercentage) > threshold || displayedPercentage == -1) {
    displayedPercentage = instantPercentage;
  }

  // Envia a umidade do solo para o Serial Monitor e via Bluetooth
  Serial.print("Soil Moisture: ");
  Serial.print(displayedPercentage);
  BTSerial.print("Soil Moisture: ");
  BTSerial.print(displayedPercentage);

  // Controla a bomba e envia status via Serial e Bluetooth
  if (instantPercentage < 10) {
    Serial.println(" - Pump on");
    BTSerial.println(" - Pump on");
    digitalWrite(3, LOW);  // Liga a bomba
  } else if (instantPercentage > 80) {
    Serial.println(" - Pump off");
    BTSerial.println(" - Pump off");
    digitalWrite(3, HIGH); // Desliga a bomba
  } else {
    Serial.println();
    BTSerial.println();
  }

  delay(1000);  // Espera 1 segundo antes da próxima leitura
}
