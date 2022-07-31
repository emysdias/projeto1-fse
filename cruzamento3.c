#include <wiringPi.h>
#include <stdio.h>

typedef int bool;
#define true 1
#define false (!true)

long lastInterruptTime = 0;
bool pressedButtonOnGreen1 = false;
bool pressedButtonOnGreen2 = false;
int delayTime = 10000;

#define ESTADO_VERMELHO_SEMAFORO 0
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

void changeTimerButton()
{
    long interruptTime = millis();

    if (interruptTime - lastInterruptTime > 300)
    {
        if (auxiliary.green.state)
        {
            pressedButtonOnGreen1 = true;
        }
        else if (principal.green.state)
        {
            pressedButtonOnGreen2 = true;
        }
    }
    lastInterruptTime = interruptTime;
}

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
        auxiliary.green.state = false;
        principal.green.state = true;
        break;
    case SEGUNDO_ESTADO_SEMAFORO:
        digitalWrite(principal.green.pin, 0);
        digitalWrite(principal.yellow.pin, 1);
        digitalWrite(principal.red.pin, 0);
        digitalWrite(auxiliary.green.pin, 0);
        digitalWrite(auxiliary.yellow.pin, 0);
        digitalWrite(auxiliary.red.pin, 1);
        auxiliary.green.state = false;
        principal.green.state = false;
        pressedButtonOnGreen2 = false;
        break;
    case TERCEIRO_ESTADO_SEMAFORO:
        digitalWrite(principal.green.pin, 0);
        digitalWrite(principal.yellow.pin, 0);
        digitalWrite(principal.red.pin, 1);
        digitalWrite(auxiliary.green.pin, 1);
        digitalWrite(auxiliary.yellow.pin, 0);
        digitalWrite(auxiliary.red.pin, 0);
        auxiliary.green.state = true;
        principal.green.state = false;
        break;
    case QUARTO_ESTADO_SEMAFORO:
        digitalWrite(principal.green.pin, 0);
        digitalWrite(principal.yellow.pin, 0);
        digitalWrite(principal.red.pin, 1);
        digitalWrite(auxiliary.green.pin, 0);
        digitalWrite(auxiliary.yellow.pin, 1);
        digitalWrite(auxiliary.red.pin, 0);
        auxiliary.green.state = false;
        principal.green.state = false;
        pressedButtonOnGreen1 = false;
        break;
    case ESTADO_VERMELHO_SEMAFORO:
        digitalWrite(principal.green.pin, 0);
        digitalWrite(principal.yellow.pin, 0);
        digitalWrite(principal.red.pin, 1);
        digitalWrite(auxiliary.green.pin, 0);
        digitalWrite(auxiliary.yellow.pin, 0);
        digitalWrite(auxiliary.red.pin, 1);
        break;
    }
}

void checkGreen(int num)
{
    for (int i = 0; i < num; i++)
    {
        if (pressedButtonOnGreen1 == true)
        {
            return;
        }
        else if (pressedButtonOnGreen2 == true)
        {
            return;
        }
        delay(1000);
    }
}

void setTimer()
{
    setTimerLeds(ESTADO_VERMELHO_SEMAFORO);
    delay(1000);
    setTimerLeds(PRIMEIRO_ESTADO_SEMAFORO); // primeiro estado semaforo
    delay(10000);
    checkGreen(9);
    setTimerLeds(SEGUNDO_ESTADO_SEMAFORO);
    delay(3000);
    setTimerLeds(TERCEIRO_ESTADO_SEMAFORO);
    delay(5000);
    checkGreen(5);
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
    principal.green = setLed(1, SEMAFORO_2_VERDE);    // 1 = verde
    principal.yellow = setLed(2, SEMAFORO_2_AMARELO); // 2 = amareuxiliarylo
    principal.red = setLed(3, SEMAFORO_2_VERMELHO);   // 3 = vermelho

    auxiliary.green = setLed(1, SEMAFORO_1_VERDE);    // 1 = verde
    auxiliary.yellow = setLed(2, SEMAFORO_1_AMARELO); // 2 = amarelo
    auxiliary.red = setLed(3, SEMAFORO_1_VERMELHO);   // 3 = vermelho
}

int main(void)
{
    wiringPiSetup();

    setTrafficLights();

    pinMode(BOTAO_PEDESTRE_1, INPUT);
    pullUpDnControl(BOTAO_PEDESTRE_1, PUD_UP);
    wiringPiISR(BOTAO_PEDESTRE_1, INT_EDGE_RISING, &changeTimerButton);

    pinMode(BOTAO_PEDESTRE_2, INPUT);
    pullUpDnControl(BOTAO_PEDESTRE_2, PUD_UP);
    wiringPiISR(BOTAO_PEDESTRE_2, INT_EDGE_RISING, &changeTimerButton);

    while (1)
    {
        setTimer();
    }

    return 0;
}