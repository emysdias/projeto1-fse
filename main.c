#include <wiringPi.h>

int SEMAFORO_1_VERDE = 8;
int SEMAFORO_1_AMARELO = 9;
int SEMAFORO_1_VERMELHO = 14;
int SEMAFORO_2_VERDE = 30;
int SEMAFORO_2_AMARELO = 21;
int SEMAFORO_2_VERMELHO = 22;
int BOTAO_PEDESTRE_1 = 12;
int BOTAO_PEDESTRE_2 = 13;
int SENSOR_PASSAGEM_1 = 7;
int SENSOR_PASSAGEM_2 = 0;
int SENSOR_VELOCIDADE_1_A = 2;
int SENSOR_VELOCIDADE_1_B = 3;
int SENSOR_VELOCIDADE_2_A = 23;
int SENSOR_VELOCIDADE_2_B = 24;

int main(void)
{
    wiringPiSetup();

    pinMode(SEMAFORO_1_VERDE, OUTPUT);
    while (1)
    {
        digitalWrite(SEMAFORO_1_VERDE, HIGH);
        delay(500);
        digitalWrite(SEMAFORO_1_VERDE, LOW);
        delay(500);
    }
    return 0;
}