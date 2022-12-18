//bibliciotecas
import processing.serial.*; //serial
Serial porta;

//arquivos de imagem
PImage logo;
PImage galao;
PImage termometro;
PImage tempBarra;
PImage rollImage;
PImage pitchImage;

//variáveis recebidas pela serial
int vel = 0; //feito
int nivel = 0; //feito
int tempCVT = 0; //feito
int hora = 0; //feito
int min = 0; //feito
int seg = 0; //feito
int alertaTemp = 1; //feito
float roll = 0; //feito
float pitch = 0; //feito

//variáveis não recebidas pela serial
boolean piscaNivel = false;
boolean piscaTemp = false;

void setup() {
  frameRate(100);
  //carrega as imagens
  logo = loadImage("logo.png");
  galao = loadImage("galao.png");
  termometro = loadImage("termometro.png");
  tempBarra = loadImage("tempBarra.png");
  rollImage = loadImage("roll.png");
  pitchImage = loadImage("pitch.png");

  size(1200, 600); //tamanho da janela (ainda precisa ajustar o código para se adaptar a qualquer tamanho de janela)
  textAlign(CENTER);
  textFont(loadFont("fontePadrao.vlw"), 60);
  background(0);
  imageMode(CENTER);
  text("Km/h", 600, 350);
  image(logo, 600, 470);
  noStroke();
  textFont(loadFont("fontePadrao.vlw"), 40);
  fill(255);

  //salva a porta que está sendo usada
  String nomePorta = Serial.list()[0];
  porta = new Serial(this, nomePorta, 57600); //atentar ao baud rate, tem que ser igual ao da serial do arduino
  while(porta.available() < 0); 
}

void draw() {
  if ((frameCount % 10) == 0) {
    velocidade();
    mostrarNivel();
    temperatura();
    cronometro();
    mostrarRoll();
    mostrarPitch();
  }
  lerSerial();
}

void lerSerial() {
  if (porta.available() > 0) {
    String valorRecebido = porta.readStringUntil('\n');
    valorRecebido = trim(valorRecebido);
    if (valorRecebido != null) {
      int dado[] = int(split(valorRecebido, ','));
      vel = dado[0];
      nivel = dado[1];
      tempCVT = dado[2];
      alertaTemp = dado [3];
      hora = dado[4];
      min = dado[5];
      seg = dado[6];
      roll = dado[7];
      pitch = dado[8];
    } 
  } 
}

void velocidade() {
  rectMode(CENTER);
  fill(0);
  rect(600, 250, 275, 275);
  fill(255);
  textAlign(CENTER);
  textFont(loadFont("velocidade.vlw"), 150);
  text(vel, 600, 250);
  textFont(loadFont("fontePadrao.vlw"), 60);
  text("Km/h", 600, 350);
}

void mostrarNivel() {
  if (nivel == 0) {
    if (piscaNivel) {
      imageMode(CENTER);
      image(galao, 1050, 470, 100, 100);
      piscaNivel = false;
    } else {
      rectMode(CENTER);
      fill(0);
      rect(1050, 470, 100, 100);
      piscaNivel = true;
    }
  } else {
    rectMode(CENTER);
    fill(0);
    rect(1050, 470, 100, 100);
  }
}

void temperatura() {
  if (alertaTemp == 1) {
    if (piscaTemp) {
      imageMode(CENTER);
      image(termometro, 100, 470, 50, 100);
      piscaTemp = false;
    } else {
      rectMode(CENTER);
      fill(0);
      rect(100, 470, 100, 100);
      piscaTemp = true;
    }
  } else {
    imageMode(CENTER);
    image(termometro, 100, 470, 50, 100);
  }
  fill(0);
  rectMode(CORNER);
  rect(130, 390, 240, 220);
  fill(255);
  textAlign(CORNER);
  textFont(loadFont("fontePadrao.vlw"), 60);
  String tempText = tempCVT + "°";
  text(tempText, 140, 505);
  textAlign(CENTER);
  float barra = map(tempCVT, -10, 140, 0, 300);
  rectMode(CORNER);
  imageMode(CORNER);
  fill(0);
  image(tempBarra, 100, 100, 100, 300);
  rect(100, 100, 100, 300 - barra);
}


void cronometro() {
  fill(0);
  rectMode(CORNER);
  rect(890, 40, 300, 50);
  fill( 255, 0, 0);
  textFont(loadFont("fontePadrao.vlw"), 40);
  textAlign(LEFT);
  text(hora + ":" + min + ":" + seg, 900, 80);
  textAlign(CENTER);
}

void mostrarRoll() {
  textAlign(CORNER);
  rectMode(CORNER);
  textFont(loadFont("fontePadrao.vlw"), 40);
  fill(0);
  rect(900, 105, 400, 55);
  fill(255);
  text("Roll:   " + roll + "°", 900, 150);
  pushMatrix();
  translate(1040, 210);
  fill(0);
  ellipse(0, 0, 97, 97);
  rotate(radians(roll));
  imageMode(CENTER);
  image(rollImage, 0, 0);
  popMatrix();
}

void mostrarPitch() {
  textAlign(CORNER);
  rectMode(CORNER);
  textFont(loadFont("fontePadrao.vlw"), 40);
  fill(0);
  rect(900, 255, 400, 55);
  fill(255);
  text("Pitch: " + pitch + "°", 900, 300);
  pushMatrix();
  translate(1040, 360);
  fill(0);
  ellipse(0, 0, 97, 97);
  rotate(radians(pitch));
  imageMode(CENTER);
  image(pitchImage, 0, 0);
  popMatrix();
}
