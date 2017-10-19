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
  bool SI_firstScan;
  bool SI_1s;
};
struct Outputs
{
};

struct IOs
{
  Inputs inputs;
  Outputs outputs;
};

struct Controller
{
  ToolsGeneral controlIOs;
  ClocksGeneral controlClocks;
  IOs ios;
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
  //Atualiza em todos os ciclos
  updateInAllCicles();

  //Atualiza o status do anel de leds
  updateMyRingLeds(myPixelOfHour, myPixelOfMinute);

  /*Rotina que é executada todo scan, porém só é atualizada no primeiro scan.
  OBS:. Sempre deixar esse método como sendo a última tarefa a ser executada pelo controlador.*/
  onlyOnFirstScan();
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
  pinsOfInputsOfSystem = {11};
  currentTime = 0;
  currentTime_OLD = 0;  */
  actualValueOfSeconds = 0;
  actualValueOfMinutes = 35;
  actualValueOfHours = 23;
  myPixelOfHour = 0;
  myPixelOfMinute = 0;
}

//Configuração dos IOs
void configMyIOs()
{
  /* myController.controlIOs.defThesePinsAsInputs(pinsOfInputsOfSystem);
  myController.controlIOs.defThesePinsAsOutputs(pinsOfOutputsOfSystem);*/
}

//Executa em todos os scans
void updateInAllCicles()
{
  //getStateOfInputs();

  //Gera o clock para hora, minuto e segundo
  generateMyClockHour();

  //Atualiza o anel de leds
  updateMyRingLeds(myPixelOfHour,myPixelOfMinute);
}

//Coleta informação do estado das entradas digitais
void getStateOfInputs()
{
  /*  myController.ios.inputs.changeSeconds = myController.controlIOs.getStatusInput(pinsOfInputsOfSystem[0]);
  myController.ios.inputs.changeMinutes = myController.controlIOs.getStatusInput(pinsOfInputsOfSystem[1]);
  myController.ios.inputs.changeHour = myController.controlIOs.getStatusInput(pinsOfInputsOfSystem[2]);
  myController.ios.inputs.changeLuminosity = myController.controlIOs.getStatusInput(pinsOfInputsOfSystem[3]);
  myController.ios.inputs.changeColor = myController.controlIOs.getStatusInput(pinsOfInputsOfSystem[4]);*/
}

//Gera o clock para o relógio
void generateMyClockHour()
{
  if (myController.ios.inputs.SI_1s)
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
    if (myController.ios.inputs.SI_1s)
    {
      ringPixels.show();
    }
  }
}
//Exibe a hora no monitor Serial
void showHourInSerialMonitor()
{
  Serial.print("Horario atual = ");
  Serial.print(actualValueOfHours);
  Serial.print(":");
  Serial.print(actualValueOfMinutes);
  Serial.print(":");
  Serial.println(actualValueOfSeconds);
}

//Executa apenas no primeiro scan do controlador
void onlyOnFirstScan()
{
  //Caso o primeiro scan ainda não foi executado
  if (!myController.ios.inputs.SI_firstScan)
  {
    ringPixels.show();
    showHourInSerialMonitor();
  }
  myController.ios.inputs.SI_firstScan = true;
}

void testStuffs()
{
}
