//a única parte que será utilizada no código final é a referente a impressão na serial
//ainda será implementadi a tranmissão de dados

bool alertaTemp = 0;
int nivel = 2;
int pitch = 0;
int roll = 0;
int vel = 0;
int temp = 0;
unsigned long tempoAnterior1;
unsigned long tempoAnterior2;
unsigned long cronometroAtual;
int tempoHor;
int tempoMin;
int tempoSeg;

void setup() {
  Serial.begin(57600);
  delay(5000);
}

void loop() {

  cronometroAtual = millis(); //armazena o tempo atual em milissegundos
  tempoHor = cronometroAtual / 3600000; //armazena o tempo em horas
  tempoMin = (cronometroAtual % 3600000) / (60000); //armazena o tempo em minutos
  tempoSeg = ((cronometroAtual % 3600000) % 60000) / (1000); //armazena o tempo em segundos

  if (millis() - tempoAnterior1 > 10) {
    String dados = String(vel) + "," + String(nivel) + "," + String(temp) + "," + String(alertaTemp) + "," + String(tempoHor) + "," + String(tempoMin) + "," + String(tempoSeg) + "," + String(pitch) + "," + String(roll);
    Serial.println(dados);
    tempoAnterior1 = millis();
  }
  if (millis() - tempoAnterior2 > 20) {
    vel = random(0, 50);
    temp = random(-10, 140);
    pitch = random(-360, 360);
    roll = random(-360, 360);
    alertaTemp = !alertaTemp;
    if (nivel == 2) nivel = 0; else nivel = 2;
    tempoAnterior2 = millis();
  }
}
