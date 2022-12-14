#include "../inc/socket.h"

typedef int bool;
#define true 1
#define false (!true)

long lastInterruptTime = 0;
int delayTime = 10000;
bool pressedButtonOnGreen1 = false;
bool pressedButtonOnGreen2 = false;
bool pressedButtonOnRed = false;
bool carSensorButton1 = false;
bool carSensorButton2 = false;

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

int secondsSensorVelocidadeA = 0;
int secondsSensorVelocidadeB = 0;

int reportsLength = 0;
int countVelocidade = 0;
int countCarros = 0;
int countFluxo = 0;

pthread_t threadA;
pthread_t threadB;

int countRed = 0;

typedef struct numberOfInfractions
{
  int redLightAdvance;
  int aboveTheAllowedSpeed;
  int mediaCarro;
} numberOfInfractions;

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
numberOfInfractions principalInfractions;

void passengerPass()
{
  long interruptTime = millis();

  if (interruptTime - lastInterruptTime > 300)
  {
    if (auxiliary.green.state && digitalRead(BOTAO_PEDESTRE_1))
    {
      pressedButtonOnGreen1 = true;
    }

    else if (principal.green.state && digitalRead(BOTAO_PEDESTRE_2))
    {
      pressedButtonOnGreen2 = true;
    }
  }
  lastInterruptTime = interruptTime;
}

void carPassSensor()
{
  long interruptTime = millis();

  if (interruptTime - lastInterruptTime > 300)
  {
    reportsLength += 1;
    if (auxiliary.red.state)
    {
      countRed += 1;
      if (countRed == 2)
      {
        principalInfractions.redLightAdvance += 1;
        countRed = 0;
      }
    }
    carSensorButton1 = digitalRead(SENSOR_PASSAGEM_1);
    carSensorButton2 = digitalRead(SENSOR_PASSAGEM_2);
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
    auxiliary.red.state = true;
    principal.green.state = true;
    principal.red.state = false;
    pressedButtonOnRed = true;
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
    auxiliary.red.state = true;
    principal.red.state = false;
    pressedButtonOnGreen2 = false;
    pressedButtonOnRed = false;
    break;
  case TERCEIRO_ESTADO_SEMAFORO:
    digitalWrite(principal.green.pin, 0);
    digitalWrite(principal.yellow.pin, 0);
    digitalWrite(principal.red.pin, 1);
    digitalWrite(auxiliary.green.pin, 1);
    digitalWrite(auxiliary.yellow.pin, 0);
    digitalWrite(auxiliary.red.pin, 0);
    auxiliary.red.state = false;
    auxiliary.green.state = true;
    principal.green.state = false;
    principal.red.state = true;
    pressedButtonOnRed = false;
    break;
  case QUARTO_ESTADO_SEMAFORO:
    digitalWrite(principal.green.pin, 0);
    digitalWrite(principal.yellow.pin, 0);
    digitalWrite(principal.red.pin, 1);
    digitalWrite(auxiliary.green.pin, 0);
    digitalWrite(auxiliary.yellow.pin, 1);
    digitalWrite(auxiliary.red.pin, 0);
    auxiliary.green.state = false;
    auxiliary.red.state = false;
    principal.green.state = false;
    principal.red.state = true;
    pressedButtonOnGreen1 = false;
    pressedButtonOnRed = false;
    break;
  case ESTADO_VERMELHO_SEMAFORO:
    digitalWrite(principal.green.pin, 0);
    digitalWrite(principal.yellow.pin, 0);
    digitalWrite(principal.red.pin, 1);
    digitalWrite(auxiliary.green.pin, 0);
    digitalWrite(auxiliary.yellow.pin, 0);
    digitalWrite(auxiliary.red.pin, 1);
    principal.red.state = true;
    auxiliary.red.state = true;
    pressedButtonOnRed = false;
    break;
  }
}

void checkTimeTraffic(int num)
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
    else if (carSensorButton1) // checa o vermelho
    {
      return;
    }
    else if (carSensorButton2) // checa o vermelho
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
  checkTimeTraffic(8);
  setTimerLeds(SEGUNDO_ESTADO_SEMAFORO);
  delay(3000);
  setTimerLeds(ESTADO_VERMELHO_SEMAFORO);
  delay(1000);
  setTimerLeds(TERCEIRO_ESTADO_SEMAFORO);
  delay(5000);
  checkTimeTraffic(4);
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
  principal.yellow = setLed(2, SEMAFORO_2_AMARELO); // 2 = amarelo
  principal.red = setLed(3, SEMAFORO_2_VERMELHO);   // 3 = vermelho

  auxiliary.green = setLed(1, SEMAFORO_1_VERDE);    // 1 = verde
  auxiliary.yellow = setLed(2, SEMAFORO_1_AMARELO); // 2 = amarelo
  auxiliary.red = setLed(3, SEMAFORO_1_VERMELHO);   // 3 = vermelho
}

int current_timestamp_to_seconds()
{
  int hours, minutes, seconds, mseconds;
  struct timeval tv;
  struct tm *tm;

  time_t now;
  time(&now);

  gettimeofday(&tv, NULL);

  tm = localtime(&tv.tv_sec);

  hours = tm->tm_hour;
  minutes = tm->tm_min;
  seconds = tm->tm_sec;
  mseconds = (int)(tv.tv_usec / 1000);

  int msec = ((hours * 3600000) + (minutes * 60000) + (seconds * 1000) + (mseconds)); // transforma em ms
  return msec;
}

void carPassSpeedSensor()
{
  long interruptTime = millis();

  if (interruptTime - lastInterruptTime > 50)
  {
    secondsSensorVelocidadeB = current_timestamp_to_seconds();
  }
  lastInterruptTime = interruptTime;
}

void carPassSpeedSensorCheck()
{
  long interruptTime = millis();

  if (interruptTime - lastInterruptTime > 50)
  {
    reportsLength += 1;
    if (secondsSensorVelocidadeB != 0)
    {
      secondsSensorVelocidadeA = current_timestamp_to_seconds();

      int resultMSenconds = secondsSensorVelocidadeA - secondsSensorVelocidadeB;
      int velocityms = 1 / (resultMSenconds * 0.001); // converte de ms para s
      int velocitykm = velocityms * 3.6;
      if (velocitykm > 60)
        principalInfractions.aboveTheAllowedSpeed += 1;
      if (principal.red.state)
        principalInfractions.redLightAdvance += 1;
      printf("A velocidade do carro foi de: %d km/h\n", velocitykm);
      countVelocidade += velocitykm;
      countCarros++;
      principalInfractions.mediaCarro = countVelocidade / countCarros;
    }
  }
  lastInterruptTime = interruptTime;
  secondsSensorVelocidadeB = 0;
}

void sendData(int sockfd)
{
  char buffer[MAX];
  int count;

  while (1)
  {
    bzero(buffer, sizeof(buffer));
    count = 0;
    while (count < reportsLength)
    {
      buffer[0] = principalInfractions.aboveTheAllowedSpeed;
      buffer[1] = principalInfractions.redLightAdvance;
      buffer[2] = principalInfractions.mediaCarro;
      buffer[3] = reportsLength;

      write(sockfd, buffer, sizeof(buffer));
      bzero(buffer, sizeof(buffer));
      count++;
      if (count == reportsLength)
      {
        reportsLength = 0;
      }
    }
  }
  delay(2000);
}

void *secondThread(void *arg)
{
  char buff[MAX];
  int n;
  int sockfd = (int)arg;

  while (1)
  {
    read(sockfd, buff, MAX);
  }
}

void *threadFunc(void *arg)
{
  int sockfd, connfd;
  struct sockaddr_in servaddr, cli;

  sockfd = configureSocket();
  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr("164.41.98.26");
  servaddr.sin_port = htons(PORT);

  connectSocket(sockfd, servaddr);

  pthread_create(&threadB, NULL, secondThread, (void *)sockfd);
  sendData(sockfd);
  close(sockfd);
}

void main(int argc, char *argv[])
{
  if (strcmp(argv[1], "placa2") == 0)
  {
    SEMAFORO_1_VERDE = 8;
    SEMAFORO_1_AMARELO = 9;
    SEMAFORO_1_VERMELHO = 14;
    SEMAFORO_2_VERDE = 30;
    SEMAFORO_2_AMARELO = 21;
    SEMAFORO_2_VERMELHO = 22;
    BOTAO_PEDESTRE_1 = 12;
    BOTAO_PEDESTRE_2 = 13;
    SENSOR_PASSAGEM_1 = 7;
    SENSOR_PASSAGEM_2 = 0;
    SENSOR_VELOCIDADE_1_A = 2;
    SENSOR_VELOCIDADE_1_B = 3;
    SENSOR_VELOCIDADE_2_A = 23;
    SENSOR_VELOCIDADE_2_B = 24;
  }

  pthread_create(&threadA, NULL, threadFunc, NULL);
  wiringPiSetup();
  setTrafficLights();

  pinMode(BOTAO_PEDESTRE_1, INPUT);
  pullUpDnControl(BOTAO_PEDESTRE_1, PUD_UP);
  wiringPiISR(BOTAO_PEDESTRE_1, INT_EDGE_RISING, &passengerPass);

  pinMode(BOTAO_PEDESTRE_2, INPUT);
  pullUpDnControl(BOTAO_PEDESTRE_2, PUD_UP);
  wiringPiISR(BOTAO_PEDESTRE_2, INT_EDGE_RISING, &passengerPass);

  pinMode(SENSOR_PASSAGEM_1, INPUT);
  pullUpDnControl(SENSOR_PASSAGEM_1, PUD_UP);
  wiringPiISR(SENSOR_PASSAGEM_1, INT_EDGE_BOTH, &carPassSensor);

  pinMode(SENSOR_PASSAGEM_2, INPUT);
  pullUpDnControl(SENSOR_PASSAGEM_2, PUD_UP);
  wiringPiISR(SENSOR_PASSAGEM_2, INT_EDGE_BOTH, &carPassSensor);

  pinMode(SENSOR_VELOCIDADE_1_A, INPUT);
  pullUpDnControl(SENSOR_VELOCIDADE_1_A, PUD_UP);
  wiringPiISR(SENSOR_VELOCIDADE_1_A, INT_EDGE_RISING, &carPassSpeedSensorCheck);

  pinMode(SENSOR_VELOCIDADE_1_B, INPUT);
  pullUpDnControl(SENSOR_VELOCIDADE_1_B, PUD_UP);
  wiringPiISR(SENSOR_VELOCIDADE_1_B, INT_EDGE_RISING, &carPassSpeedSensor);

  pinMode(SENSOR_VELOCIDADE_2_A, INPUT);
  pullUpDnControl(SENSOR_VELOCIDADE_2_A, PUD_UP);
  wiringPiISR(SENSOR_VELOCIDADE_2_A, INT_EDGE_RISING, &carPassSpeedSensorCheck);

  pinMode(SENSOR_VELOCIDADE_2_B, INPUT);
  pullUpDnControl(SENSOR_VELOCIDADE_2_B, PUD_UP);
  wiringPiISR(SENSOR_VELOCIDADE_2_B, INT_EDGE_RISING, &carPassSpeedSensor);

  while (1)
  {
    setTimer();
  }
}