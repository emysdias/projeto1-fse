#include <wiringPi.h>
#include <stdio.h>

typedef int bool;
#define true 1
#define false (!true)

long lastInterruptTime = 0;
int test = 500;

#define PRIMEIRO_ESTADO_SEMAFORO 1
#define SEGUNDO_ESTADO_SEMAFORO 2
#define TERCEIRO_ESTADO_SEMAFORO 3
#define QUARTO_ESTADO_SEMAFORO 4

int SEMAFORO_1_VERDE = 31;
int SEMAFORO_1_AMARELO = 25;
int SEMAFORO_1_VERMELHO = 29;
int SEMAFORO_2_VERDE = 28;
int SEMAFORO_2_AMARELO = 27;
int SEMAFORO_2_VERMELHO = 26;
int BOTAO_PEDESTRE_1 = 10;
int BOTAO_PEDESTRE_2 = 11;
int SENSOR_PASSAGEM_1 = 15;
int SENSOR_PASSAGEM_2 = 16;
int SENSOR_VELOCIDADE_1_A = 1;
int SENSOR_VELOCIDADE_1_B = 4;
int SENSOR_VELOCIDADE_2_A = 5;
int SENSOR_VELOCIDADE_2_B = 6;

typedef struct led
{
    int pin;
    bool state;
    int color;
} led;

typedef struct trafficLights
{
    led red;
    led yellow;
    led green;
} trafficLights;

trafficLights principal;
trafficLights auxiliary;

// void callback_function(void)
// {
//     long interruptTime = millis();

//     if (interruptTime - lastInterruptTime > 200)
//     {
//         // Perform your logic here
//         test = 100;
//         delay(3000);
//     }
//     lastInterruptTime = interruptTime;
//     test = 1000;
// }

void setTimerLeds(int num)
{
    switch (num)
    {
    case PRIMEIRO_ESTADO_SEMAFORO:
        digitalWrite(principal.green.pin, 1);
        digitalWrite(principal.yellow.pin, 0);
        digitalWrite(principal.red.pin, 0);
        digitalWrite(auxiliary.green.pin, 0);
        digitalWrite(auxiliary.yellow.pin, 0);
        digitalWrite(auxiliary.red.pin, 1);
        break;
    case SEGUNDO_ESTADO_SEMAFORO:
        digitalWrite(principal.green.pin, 0);
        digitalWrite(principal.yellow.pin, 1);
        digitalWrite(principal.red.pin, 0);
        digitalWrite(auxiliary.green.pin, 0);
        digitalWrite(auxiliary.yellow.pin, 0);
        digitalWrite(auxiliary.red.pin, 1);
        break;
    case TERCEIRO_ESTADO_SEMAFORO:
        digitalWrite(principal.green.pin, 0);
        digitalWrite(principal.yellow.pin, 0);
        digitalWrite(principal.red.pin, 1);
        digitalWrite(auxiliary.green.pin, 1);
        digitalWrite(auxiliary.yellow.pin, 0);
        digitalWrite(auxiliary.red.pin, 0);
        break;
    case QUARTO_ESTADO_SEMAFORO:
        digitalWrite(principal.green.pin, 0);
        digitalWrite(principal.yellow.pin, 0);
        digitalWrite(principal.red.pin, 1);
        digitalWrite(auxiliary.green.pin, 0);
        digitalWrite(auxiliary.yellow.pin, 1);
        digitalWrite(auxiliary.red.pin, 0);
        break;
    }
}

void setTimer()
{
    setTimerLeds(PRIMEIRO_ESTADO_SEMAFORO); // primeiro estado semaforo
    delay(20000);
    setTimerLeds(SEGUNDO_ESTADO_SEMAFORO);
    delay(3000);
    setTimerLeds(TERCEIRO_ESTADO_SEMAFORO);
    delay(10000);
    setTimerLeds(QUARTO_ESTADO_SEMAFORO);
    delay(3000);
}

led setLed(int num, int pin)
{
    led l;
    l.color = num;
    l.pin = pin;
    l.state = false;
    pinMode(pin, OUTPUT);
    return l;
}

void setTrafficLights()
{
    principal.green = setLed(1, SEMAFORO_1_VERDE);    // 1 = verde
    principal.yellow = setLed(2, SEMAFORO_1_AMARELO); // 2 = amareuxiliarylo
    principal.red = setLed(3, SEMAFORO_1_VERMELHO);   // 3 = vermelho

    auxiliary.green = setLed(1, SEMAFORO_2_VERDE);    // 1 = verde
    auxiliary.yellow = setLed(2, SEMAFORO_2_AMARELO); // 2 = amarelo
    auxiliary.red = setLed(3, SEMAFORO_2_VERMELHO);   // 3 = vermelho
}

int main(void)
{

    printf("asasas\n");
    wiringPiSetup();

    setTrafficLights();

    while (1)
    {
        setTimer();
    }

    return 0;
}