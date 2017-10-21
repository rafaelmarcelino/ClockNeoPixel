/******************************************************************************************************************************************************************
***************************************************************************Bibliotecas*****************************************************************************
*******************************************************************************************************************************************************************/
//Inclusão de bibliotecas
#include <ToolsGeneral.h>
#include <ClocksGeneral.h>
#include <Adafruit_NeoPixel.h>
/******************************************************************************************************************************************************************
***********************************************************************Estruturas de dados*************************************************************************
*******************************************************************************************************************************************************************/
//Definições de estruturas
struct Inputs
{
  bool changeSeconds;
  bool changeMinutes;
  bool changeHour;
  bool changeColor;
  bool changeLuminosity;
};
struct Outputs
{
};

struct IOs
{
  Inputs inputs;
  Outputs outputs;
};

struct Controller_Command
{
};

struct Controller_Status
{
  bool firstScan;
  bool _1s;
  bool Logging;
  long startCycleTime;
  long lastCycleTime;
};
struct Controller
{
  ToolsGeneral controlIOs;
  ClocksGeneral controlClocks;
  IOs ios;
  Controller_Command _command;
  Controller_Status _status;
};
/******************************************************************************************************************************************************************
****************************************************************************Constantes*****************************************************************************
*******************************************************************************************************************************************************************/
//Definições das constantes
long BASE_TIME_MS = 1000;
long MAX_SECONDS_AND_MINUTES = 59;
long OFFSET_SECONDS_FOR_PIXELS = 150;
long MAX_HOURS = 23;
int PIN_NEOPIXEL = 2;
int NUMBER_OF_PIXELS = 60;
byte RED = 255;
byte GREEN = 255;
byte BLUE = 255;
/******************************************************************************************************************************************************************
*****************************************************************************Variáveis*****************************************************************************
*******************************************************************************************************************************************************************/
//Definição de variáveis globais
Controller myController;
/*int pinsOfInputsOfSystem [];
int pinsOfOutputsOfSystem [];*/
long currentTime;
long currentTime_OLD;
long actualValueOfSeconds;
long actualValueOfMinutes;
long actualValueOfHours;
long maskForPixelMinutes;

int myPixelOfHour;
int myPixelOfMinute;

int testMainPixelHour;
int testBeforeMainPixelHour;
int testAfterMainPixelHour;
/******************************************************************************************************************************************************************
******************************************************************************Objetos******************************************************************************
*******************************************************************************************************************************************************************/
//Criando instância do anel de led
Adafruit_NeoPixel ringPixels = Adafruit_NeoPixel(NUMBER_OF_PIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
/******************************************************************************************************************************************************************
**************************************************************************Métodos Arduino**************************************************************************
*******************************************************************************************************************************************************************/
void setup()
{
  // put your setup code here, to run once:
  //configMyIOs();
  setDefaultValues();
}

void loop()
{
  // put your main code here, to run repeatedly:
  //Coleta status das entradas
  getStateOfInputs();

  //Gera o clock para hora, minuto e segundo
  generateMyClockHour();

  //Atualiza o anel de leds
  updateMyRingLeds(myPixelOfHour, myPixelOfMinute);

  /*Rotina que é executada todo scan, porém só é atualizada no primeiro scan.
  OBS:. Sempre deixar esse método como sendo a última tarefa a ser executada pelo controlador.*/
  onlyOnFirstScan();

  //Coleta o valor do ciclo do controlador
  myController._status.lastCycleTime = myController.controlClocks.getLastCycleTime(myController._status.startCycleTime);

  //Exibe log no serial monitor
  callLog();
}
/******************************************************************************************************************************************************************
**************************************************************************Métodos Usuário**************************************************************************
*******************************************************************************************************************************************************************/
//Definição dos valores setDefaultValues
void setDefaultValues()
{
  //Incializando anel de leds e atualiza status dos leds
  ringPixels.begin();
  ringPixels.show();
  //Incializando comunicação Serial
  Serial.begin(9600);

  /*pinsOfInputsOfSystem = {10};
  pinsOfInputsOfSystem = {11};*/
  actualValueOfSeconds = 0;
  actualValueOfMinutes = 35;
  actualValueOfHours = 23;
  myPixelOfHour = 0;
  myPixelOfMinute = 0;

  //inicializando valores dos tempos de ciclos
  myController._status.startCycleTime = 0;
  myController._status.lastCycleTime = 0;
}

//Configuração dos IOs
void configMyIOs()
{
  /* myController.controlIOs.defThesePinsAsInputs(pinsOfInputsOfSystem);
  myController.controlIOs.defThesePinsAsOutputs(pinsOfOutputsOfSystem);*/
}

//Coleta informação do estado das entradas digitais
void getStateOfInputs()
{
  //Atualiza valor de início de ciclo
  myController._status.startCycleTime = millis();

  /*  myController.ios.inputs.changeSeconds = myController.controlIOs.getStatusInput(pinsOfInputsOfSystem[0]);
  myController.ios.inputs.changeMinutes = myController.controlIOs.getStatusInput(pinsOfInputsOfSystem[1]);
  myController.ios.inputs.changeHour = myController.controlIOs.getStatusInput(pinsOfInputsOfSystem[2]);
  myController.ios.inputs.changeLuminosity = myController.controlIOs.getStatusInput(pinsOfInputsOfSystem[3]);
  myController.ios.inputs.changeColor = myController.controlIOs.getStatusInput(pinsOfInputsOfSystem[4]);*/

  //Clock de 1 segundo
  myController._status._1s = myController.controlClocks.getClockInThisTime(BASE_TIME_MS);
}

//Gera o clock para o relógio
void generateMyClockHour()
{
  if (myController._status._1s)
  {
    //Contando os segundos e o offset de segundos para atualização dos minutos no anel de led
    if (actualValueOfSeconds <= MAX_SECONDS_AND_MINUTES)
    {
      actualValueOfSeconds++;
    }
    //Contando os minutos e redefinindo os segundos
    if (actualValueOfSeconds > MAX_SECONDS_AND_MINUTES)
    {
      actualValueOfSeconds = 0;
      actualValueOfMinutes++;
    }
    //Contando as horas e redefinindo os minutos
    if (actualValueOfMinutes > MAX_SECONDS_AND_MINUTES)
    {
      actualValueOfMinutes = 0;
      actualValueOfHours++;
    }
    //Redefinindo as horas
    if (actualValueOfHours > MAX_HOURS)
    {
      actualValueOfHours = 0;
    }
  }
  //Atualiza valor do pixel de hora e minuto
  myPixelOfHour = actualValueOfHours;
  myPixelOfMinute = actualValueOfMinutes;
}

//Atualiza os pixels do anel de led
void updateMyRingLeds(int mainPixelHour, int pixelMinutes)
{
  int _beforeMainPixelHour = 0;
  int _afterMainPixelHour = 0;

  //Define os pixels antes e depois do pixel principal
  if ((mainPixelHour >= 0) && (mainPixelHour <= 23))
  {
    if (mainPixelHour == MAX_HOURS)
    {
      _beforeMainPixelHour = mainPixelHour - 1;
      _afterMainPixelHour = 0;
    }
    else if (mainPixelHour == 0)
    {
      _beforeMainPixelHour = MAX_HOURS;
      _afterMainPixelHour = 1;
    }
    else
    {
      _beforeMainPixelHour = mainPixelHour - 1;
      _afterMainPixelHour = mainPixelHour + 1;
    }

    //Define cor, brilho e posição dos pixels
    for (byte i = 0; i <= MAX_HOURS; i++)
    {
      //Definindo cor das horas. Pixel antes do principal
      if ((i == _beforeMainPixelHour) && (i != pixelMinutes))
      {
        ringPixels.setPixelColor(_beforeMainPixelHour, 0, 0, (BLUE / 8));
      }
      else if ((i == _beforeMainPixelHour) && (i == pixelMinutes))
      {
        ringPixels.setPixelColor(_beforeMainPixelHour, RED, 0, (BLUE / 8));
      }

      //Definindo cor das horas. Pixel principal
      if ((i == mainPixelHour) && (i != pixelMinutes))
      {
        ringPixels.setPixelColor(mainPixelHour, 0, 0, BLUE);
      }
      else if ((i == mainPixelHour) && (i == pixelMinutes))
      {
        ringPixels.setPixelColor(mainPixelHour, RED, 0, BLUE);
      }

      //Definindo cor das horas. Pixel depois do principal
      if ((i == _afterMainPixelHour) && (i != pixelMinutes))
      {
        ringPixels.setPixelColor(_afterMainPixelHour, 0, 0, (BLUE / 8));
      }
      else if ((i == _afterMainPixelHour) && (i == pixelMinutes))
      {
        ringPixels.setPixelColor(_afterMainPixelHour, RED, 0, (BLUE / 8));
      }

      //Definindo cor dos minutos
      if ((i == pixelMinutes) &&
          (i != _beforeMainPixelHour) &&
          (i != mainPixelHour) &&
          (i != _afterMainPixelHour))
      {
        ringPixels.setPixelColor(pixelMinutes, RED, 0, 0);
      }

      //Definindo leds apagados
      if ((i != _beforeMainPixelHour) &&
          (i != mainPixelHour) &&
          (i != _afterMainPixelHour) &&
          i != pixelMinutes)
      {
        ringPixels.setPixelColor(i, 0, 0, 0);
      }
    }
    //Taxa de atualização do anel de led
    if (myController._status._1s)
    {
      ringPixels.show();
    }
  }
}
//Exibe a hora no monitor Serial
void showHourInSerialMonitor()
{
}

//Executa apenas no primeiro scan do controlador
void onlyOnFirstScan()
{
  //Caso o primeiro scan ainda não foi executado
  if (!myController._status.firstScan)
  {
    ringPixels.show();
  }else{
    return;
  }
  myController._status.firstScan = true;
}

//Gera mensagens de Log
void callLog()
{
  //Log para o tempo do scan em ms
  Serial.print("Ultimo scan = ");
  Serial.println(myController._status.lastCycleTime);

  //Log do horário atual
  if (myController._status._1s)
  {
    Serial.print("Horario atual = ");
    Serial.print(actualValueOfHours);
    Serial.print(":");
    Serial.print(actualValueOfMinutes);
    Serial.print(":");
    Serial.println(actualValueOfSeconds);
  }
}
