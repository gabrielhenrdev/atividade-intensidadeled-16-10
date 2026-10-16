
//Ex 1

// #include <Arduino.h>

 #define pinverd  4
 //#define pinamar  2
 #define pinverm  15

// unsigned long tempoAnterior = 0;
// const unsigned long intervalo = 20;

// int intensidade = 0;
// const int passo = 5;

// bool aumentando = true;
// int ledAtual = 0;

// int leds[] = {
//     pinverd,
//     pinamar,
//     pinverm
// };

// void setup() {
//     pinMode(pinverd, OUTPUT);
//     pinMode(pinamar, OUTPUT);
//     pinMode(pinverm, OUTPUT);

//     for (int i = 0; i < 3; i++) {
//         analogWrite(leds[i], 0);
//     }
// }

// void loop() {
//     unsigned long tempoAtual = millis();

//     if (tempoAtual - tempoAnterior >= intervalo) {
//         tempoAnterior = tempoAtual;

//         analogWrite(leds[ledAtual], intensidade);

//         if (aumentando) {
//             intensidade += passo;

//             if (intensidade >= 255) {
//                 intensidade = 255;
//                 aumentando = false;
//             }
//         } else {
//             intensidade -= passo;

//             if (intensidade <= 0) {
//                 intensidade = 0;
//                 analogWrite(leds[ledAtual], 0);

//                 ledAtual++;

//                 if (ledAtual >= 3) {
//                     ledAtual = 0;
//                 }

//                 aumentando = true;
//             }
//         }
//     }
// }







//Ex 2, 3 e 4

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define pinverd 4
#define pinverm 15

#define pindht 27
#define tipodht DHT22

DHT mao(pindht, tipodht);
LiquidCrystal_I2C lcd(0x27, 20, 4);

unsigned long tempoAnterior = 0;
const unsigned long intervalo = 3000;

int intensidadeVerm = 0;
int intensidadeVerd = 0;

void setup() {
    pinMode(pinverd, OUTPUT);
    pinMode(pinverm, OUTPUT);

    Serial.begin(115200);
    mao.begin();

    Wire.begin(21, 22);
    lcd.init();
    lcd.backlight();

    Serial.println("Sistema DHT22 iniciado!");
}

void loop() {
    unsigned long tempoAtual = millis();

    if (tempoAtual - tempoAnterior >= intervalo) {
        tempoAnterior = tempoAtual;

        float temperaturaC = mao.readTemperature();
        float temperaturaF = mao.readTemperature(true);
        float umidade = mao.readHumidity();

        if (isnan(temperaturaC) || isnan(umidade)) {
            Serial.println("Erro ao ler o sensor DHT22!");

            analogWrite(pinverm, 0);
            analogWrite(pinverd, 0);

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Erro ao ler DHT22!");
        } else {

            if ((temperaturaC >= 24) * (temperaturaC <= 30)) {
                intensidadeVerm = (temperaturaC - 24) * 255 / 6;
            } else if (temperaturaC > 30) {
                intensidadeVerm = 255;
            } else {
                intensidadeVerm = 0;
            }

            if ((umidade >= 24) * (umidade <= 30)) {
                intensidadeVerd = (umidade - 24) * 255 / 6;
            } else if (umidade > 30) {
                intensidadeVerd = 255;
            } else {
                intensidadeVerd = 0;
            }

            analogWrite(pinverm, intensidadeVerm);
            analogWrite(pinverd, intensidadeVerd);

            Serial.println("----------------------------");

            Serial.print("Temperatura: ");
            Serial.print(temperaturaC);
            Serial.println(" C");

            Serial.print("Temperatura: ");
            Serial.print(temperaturaF);
            Serial.println(" F");

            Serial.print("Umidade: ");
            Serial.print(umidade);
            Serial.println(" %");

            Serial.print("Brilho vermelho: ");
            Serial.println(intensidadeVerm);

            Serial.print("Brilho verde: ");
            Serial.println(intensidadeVerd);

            Serial.println("----------------------------");

            lcd.clear();

            lcd.setCursor(0, 0);
            lcd.print("Monitor DHT22");

            lcd.setCursor(0, 1);
            lcd.print("Temp C: ");
            lcd.print(temperaturaC, 1);
            lcd.print(" C");

            lcd.setCursor(0, 2);
            lcd.print("Temp F: ");
            lcd.print(temperaturaF, 1);
            lcd.print(" F");

            lcd.setCursor(0, 3);
            lcd.print("Umidade: ");
            lcd.print(umidade, 1);
            lcd.print("%");
        }
    }
}
