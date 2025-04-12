#include "LedController.hpp"

//ESP32
//#define DIN 23
//#define CS 5
//#define CLK 18

//UNO
#define DIN 11
#define CS 12
#define CLK 13

LedController<1, 1> display;

int efeitoDesce = 0;  //0=Desligado 1-Barra Superior 2-Barra do Meio 3-Barra Inferior
int efeitoSobe = 0;   //0=Desligado 1-Barra Inferior 2-Barra do Meio 3-Barra Superior
int estado = 0;       //0=Parado 1=Descendo 2=Subindo
int estadoAnt = 9;
int andar = 0;
int andarAnt = 9;
unsigned long delayEfeito;
int tempoFase[] = { 200, 150, 150, 500 };
String comando;

void setup() {

  display = LedController<1, 1>(DIN, CLK, CS);

  display.init(CS);
  display.setIntensity(15);
  display.clearMatrix();

  //display.setDigit(0, 4, 1, false);
  //display.setChar(0, 0, ' ', false);
  //display.setLed(0, 0, 0, true);

  Serial.begin(115200);

  Serial.println("Elevador montTUDO");
  Serial.println("-----------------");
  Serial.println("Comandos disponiveis via serial:");
  Serial.println("a<numero_do_andar> - seleciona o andar.  Ex: a18");
  Serial.println("p - elevador parado");
  Serial.println("d - elevador descendo");
  Serial.println("s - elevador subindo");
  Serial.println("Digite o comando e pressione enter...");
  Serial.println();
  Serial.println("Skecth Iniciado!");
}

void loop() {
  if (Serial.available()) {
    comando = Serial.readString();
    comando.trim();
    Serial.print("Comando: ");
    Serial.println(comando);

    if (comando[0] == 'a' || comando[0] == 'A') {
      String andarSel = comando.substring(1);
      andar = andarSel.toInt();
    }

    if (comando[0] == 'p' || comando[0] == 'P') {
      estado = 0;
    }

    if (comando[0] == 'd' || comando[0] == 'D') {
      estado = 1;
    }

    if (comando[0] == 's' || comando[0] == 'S') {
      estado = 2;
    }
  }

  displayElevadorLoop();
}

void displayElevadorLoop() {

  //Elevador Parado
  if (estado == 0 && estadoAnt != 0) {
    display.setChar(0, 0, ' ', false);
    display.setChar(0, 7, ' ', false);
  }

  //Mostra o andar
  if (andar != andarAnt) {
    if (andar < 10) {
      display.setChar(0, 5, ' ', false);
      display.setChar(0, 4, ' ', false);
      display.setDigit(0, 3, andar, false);
      display.setChar(0, 2, ' ', false);
    } else {
      if (andar < 100) {
        display.setChar(0, 5, ' ', false);
        display.setDigit(0, 4, andar / 10, false);
        display.setDigit(0, 3, andar % 10, false);
        display.setChar(0, 2, ' ', false);
      } else {
        if (andar < 1000) {
          display.setChar(0, 5, ' ', false);
          display.setDigit(0, 4, andar / 100, false);
          display.setDigit(0, 3, ((andar % 100) / 10), false);
          display.setDigit(0, 2, ((andar % 100)) % 10, false);
        } else {
          display.setDigit(0, 5, andar / 1000, false);
          display.setDigit(0, 4, ((andar % 1000) / 100), false);
          display.setDigit(0, 3, (((andar % 1000) % 100) / 10), false);
          display.setDigit(0, 2, (((andar % 1000) % 100)) % 10, false);
        }
      }
    }
  }

  //Elevador Descendo
  if (estado == 1) {

    if ((millis() - delayEfeito) > tempoFase[efeitoDesce]) {
      delayEfeito = millis();
      efeitoDesce++;
      if (efeitoDesce > 3) efeitoDesce = 0;

      switch (efeitoDesce) {
        case 0:
          {
            display.setChar(0, 0, ' ', false);
            display.setChar(0, 7, ' ', false);
            break;
          }
        case 1:
          {
            display.setLed(0, 0, 6, true);
            display.setLed(0, 7, 6, true);
            break;
          }
        case 2:
          {
            display.setLed(0, 0, 0, true);
            display.setLed(0, 7, 0, true);
            break;
          }
        case 3:
          {
            display.setLed(0, 0, 3, true);
            display.setLed(0, 7, 3, true);
            break;
          }
      }
    }
  }

  //Elevador Subindo
  if (estado == 2) {
    if ((millis() - delayEfeito) > tempoFase[efeitoSobe]) {
      delayEfeito = millis();
      efeitoSobe++;
      if (efeitoSobe > 3) efeitoSobe = 0;

      switch (efeitoSobe) {
        case 0:
          {
            display.setChar(0, 0, ' ', false);
            display.setChar(0, 7, ' ', false);
            break;
          }
        case 1:
          {
            display.setLed(0, 0, 3, true);
            display.setLed(0, 7, 3, true);
            break;
          }
        case 2:
          {
            display.setLed(0, 0, 0, true);
            display.setLed(0, 7, 0, true);
            break;
          }
        case 3:
          {
            display.setLed(0, 0, 6, true);
            display.setLed(0, 7, 6, true);
            break;
          }
      }
    }
  }

  estadoAnt = estado;
  andarAnt = andar;
}
