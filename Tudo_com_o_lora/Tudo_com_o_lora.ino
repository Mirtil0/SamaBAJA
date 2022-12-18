
//BIBLIOTECAS
#include <DallasTemperature.h>
#include <OneWire.h>
#include "Nextion.h"
#include <SPI.h>
#include <SoftwareSerial.h>

//LORA
//SoftwareSerial loraSerial(8, 7); // TX, RX //inicializa o lora

//CONTROLE DE TEMPO DE FUNCIONAMENTO GLOBAL
unsigned long tempoAtual;
unsigned long tempoAnterior_tft = 0;
unsigned long tempoAnterior_combustivel = 0;
unsigned long tempoAnterior_lora = 0;
unsigned long tempoAnterior_crono = 0;
unsigned long tempoAnterior_vel = 0;
const long intervalo_vel = 500;
unsigned long tempo1 = 0, tempo2 = 0;
const long attTEMP = 5000;

//CRONOMETRO
int tempo_hor;
int tempo_min;
int tempo_seg;

//OBJETOS NEXTION
//NexNumber RPM = NexNumber(0, 9, "n1");
NexNumber temp = NexNumber(0, 10, "n2");
NexNumber hora = NexNumber(0, 11, "n3");
NexNumber minuto = NexNumber(0, 12, "n4");
NexNumber segundo = NexNumber(0, 13, "n5");
NexProgressBar j0 = NexProgressBar(0, 2, "j0"); // Sinal combustivel
NexProgressBar j1 = NexProgressBar(0, 7, "j1"); // Visualização da temperatura em barra
NexNumber velocidade = NexNumber(0, 9, "n0");
NexPicture p1 = NexPicture(0, 14, "p1");
NexPicture p2 = NexPicture(0, 15, "p2");


//CONVERSÂO PARA O DISPLAY
//char    txt[10], txt2[10], txt3[10], txt4[10], txt5[10], txt6[10], txt7[10];

//VARIAVEIS VELOCIDADE
int contador_vel = 0;
int rotacao = 0;
int vel = 0;
int exibir = 0;
int bar_rota = 0;

//VARIAVEIS RPM
/*unsigned long tempoAnterior_vel = 0;
  const long intervalo_vel = 500;
  int contador_vel = 0;
  int rotacao_ = 0;
  int vel = 0;
  int exibir = 0;
  int bar_rota = 0;*/

//VARIVEIS TEMPERATURA
#define ONE_WIRE_BUS 6// Porta do pino de sinal do DS18B20
OneWire ourWire(ONE_WIRE_BUS);// Define uma instancia do oneWire para comunicacao com o sensor
DallasTemperature sensors(&ourWire);
float tempCVT = 0;
int bar_temp = 0;
int bar_alerta = 0;
int alertaTEMP = 0;

//VARIAVEIS NIVEL COMBUSTIVEL
//#define nivel_alto 4 // Porta sensor superior
#define nivel_baixo 5 // Porta sensor inferior
//bool leitura_superior;
bool leitura_inferior;
int estado_nivel = 0;
int bar_nivel = 0;
bool estado;
int pisca = 8;

//SETUP//
void setup() { //Início Void Setup//
  pinMode(5, INPUT_PULLUP);
  nexInit();  //inicializa o tft
  //  loraSerial.begin(9600); //inicializa o lora
  sensors.begin(); //INICIA O SENSOR
  sensors.requestTemperatures();//SOLICITA QUE A FUNÇÃO INFORME A TEMPERATURA DO SENSOR

  //INTERRUPÇÃO DA ROTAÇÃO PORTA DIGITAL 2 (VELOCIDADE)
  attachInterrupt(0, interrupt_vel , FALLING);

  //INTERRUPÇÃO DA ROTAÇÃO PORTA DIGITAL 3 (RPM)
  //attachInterrupt(1, interrupt_vel , RISING);

} //Fim Void Setup//

void loop() { //Início Void Loop//

  tempoAtual = millis();

  // *******    VELOCIDADE   ******** //
  if ((tempoAtual - tempoAnterior_vel) > intervalo_vel) cal_vel();

  // *******  LORA  ******** //
  //if (tempoAtual - tempoAnterior_lora > 1000) lora();

  // *******  TEMPERTURA  ******** //
  //cal_temperatura();

  // *******  NIVEL COMBUSTIVEL  ******** //
  if (tempoAtual - tempoAnterior_combustivel > 1000) combustivel();

  // *******  CRONOMETRO  ******** //
  if (tempoAtual - tempoAnterior_crono > 1500) cronometro();

  // ******  EXIBIÇÃO DISPLAY  ******* //
  if (tempoAtual - tempoAnterior_tft > 500) mostrartft();

} //Fim Void Loop//

// ******************* //
// ******  EXIBIÇÃO DISPLAY  ******* //
// ******************* //

void mostrartft() {
  // Atualização dos valores do Nextion
  velocidade.setValue(vel);
  //RPM.setValue(rotacao);
  temp.setValue(tempCVT);
  hora.setValue(tempo_hor);
  minuto.setValue(tempo_min);
  segundo.setValue(tempo_seg);
  //j2.setValue(bar_rota);
  j1.setValue(bar_temp);
  j0.setValue(bar_nivel);

  //-----------------------//

  // Atualização dos valores do Nextion
  velocidade.setValue(vel);
  //RPM.setValue(rotacao);
  temp.setValue(tempCVT);
  hora.setValue(tempo_hor);
  minuto.setValue(tempo_min);
  segundo.setValue(tempo_seg);
  //j2.setValue(bar_rota);
  j1.setValue(bar_temp);
  j0.setValue(bar_nivel);
  if (estado_nivel == 2) estado = !estado;
  if (estado == true) pisca = 8; else pisca = 9;
  if (estado == false) pisca = 9;
  p1.setPic(pisca);
  p1.setPic(pisca);

  /*Converte inteiros para string necessario apenas quando utilizar texto ao invez de numero
    Conversão segundo
    memset(txt6, 0, sizeof(txt6));
    itoa(tempo_seg, txt6, 10);
  */
  //Normalização de barras
  //Valor da temperatura
  bar_temp = map(tempCVT, -10, 140, 0, 100);
  //Sinal de alta temperatura
  // bar_alerta = map(alertaTEMP, 0, 1, 0, 100);
  //Sinal de alta rotação
  //bar_rota = map(rotacao, 0, 4000, 0, 100);
  //Sinal de nível
  bar_nivel = map(estado_nivel, 0, 2, 100, 0);
  tempCVT++;
}

// ******************* //
// *******  ROTAÇÃO  ******** //
// ******************* //

void cal_vel() {

  detachInterrupt(0);
  rotacao = ((contador_vel / ((tempoAtual - tempoAnterior_vel) * 4 * 0.001))) * 60; //verificar isso
  vel = (rotacao / 60) * 2 * 3.141592 * 0.24 * 3.6; //verificar isso
  tempoAnterior_vel = millis();
  //Serial.print(rotacao);
  //Serial.print(" ");
  //Serial.print(vel);
  contador_vel = 0;
  //Serial.println(rotacao);
  attachInterrupt(0, interrupt_vel, FALLING);

}

// ******************* //
// *******  TEMPERATURA  ******** //
// ******************* //

void cal_temperatura() {

  tempo1 = millis();
  tempCVT = sensors.getTempCByIndex(0);
  //Serial.print(tempCVT);
  //Serial.print("\t");
  //Serial.println((tempo1 - tempo2));
  if (tempCVT >= 32)
  {
    alertaTEMP = 1;
  }
  else
  {
    alertaTEMP = 0;
  }

  if ((tempo1 - tempo2) > attTEMP)
  {
    sensors.requestTemperatures();//SOLICITA QUE A FUNÇÃO INFORME A TEMPERATURA DO SENSOR
    tempCVT = sensors.getTempCByIndex(0);
    tempo2 = tempo1;
  }

}

// ******************* //
// *******  CRONOMETRO ******** //
// ******************* //

void cronometro() {

  tempoAnterior_crono = millis();
  tempo_hor = tempoAtual / 3600000; //armazena o tempo em horas
  tempo_min = (tempoAtual % 3600000) / (60000); //armazena o tempo em minutos
  tempo_seg = ((tempoAtual % 3600000) % 60000) / (1000); //armazena o tempo em segundos

}

// ******************* //
// *******  NIVEL COMBUSTIVEL ******** //
// ******************* //

void combustivel() {

  tempoAnterior_combustivel = millis();
  leitura_inferior = digitalRead(nivel_baixo);
  if (leitura_inferior == HIGH) {
    estado_nivel = 2;

  } else {

    estado_nivel = 0;

  }

}

// ******************* //
// *******  LORA ******** //
// ******************* //

void lora() {

  tempoAnterior_lora = millis();
  String dados = String(tempCVT) + "," + String(tempoAtual) + "," + String(rotacao) + "," + String(vel); //Salva todos os dados em uma única string
  //  loraSerial.print(dados);

}

//INTERUPÇÃO PARA ROTAÇÃO
void interrupt_vel()
{
  contador_vel++;
}
