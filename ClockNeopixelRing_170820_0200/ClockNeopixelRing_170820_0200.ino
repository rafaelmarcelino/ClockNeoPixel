//Inclusão de bibliotecas
#include <ToolsGeneral.h>
#include <ClocksGeneral.h>

//Definições de estruturas
struct Inputs{
  bool changeSeconds;
  bool changeMinutes;
  bool changeHour;
  bool changeColor;
  bool changeLuminosity;
};
struct Outputs{
  
};

struct IOs {
  Inputs inputs;
  Outputs outputs;
};

struct Controller{
  ToolsGeneral controlIOs;
  ClocksGeneral controlClocks;
  IOs ios;
};

//Definições das constantes
long BASE_TIME = 1000;
long MAX_SECONDS_AND_MINUTES = 59;
long MAX_HOURS = 23;
//Definição de variáveis globais
Controller myController;
/*int pinsOfInputsOfSystem [];
int pinsOfOutputsOfSystem [];*/
long currentTime;
long currentTime_OLD;
long actualValueOfSeconds;
long actualValueOfMinutes;
long actualValueOfHours;

void setup() {
  // put your setup code here, to run once:
  //configMyIOs();
  setDefaultValues();  
}

void loop() {
  // put your main code here, to run repeatedly:
  //currentTime = millis();
  //getStateOfInputs();
  generateMyClockHour();   
}

//Definição dos valores setDefaultValues
void setDefaultValues(){
  Serial.begin(9600);
  
  /*pinsOfInputsOfSystem = {10};
  pinsOfInputsOfSystem = {11};
  currentTime = 0;
  currentTime_OLD = 0;  */
  actualValueOfSeconds = 0;
  actualValueOfMinutes = 0;
  actualValueOfHours = 0;
}

//Configuração dos IOs
void configMyIOs(){
  /* myController.controlIOs.defThesePinsAsInputs(pinsOfInputsOfSystem);
  myController.controlIOs.defThesePinsAsOutputs(pinsOfOutputsOfSystem);*/
  
}

//Coleta informação do estado das entradas digitais
void getStateOfInputs(){
  /*  myController.ios.inputs.changeSeconds = myController.controlIOs.getStatusInput(pinsOfInputsOfSystem[0]);
  myController.ios.inputs.changeMinutes = myController.controlIOs.getStatusInput(pinsOfInputsOfSystem[1]);
  myController.ios.inputs.changeHour = myController.controlIOs.getStatusInput(pinsOfInputsOfSystem[2]);
  myController.ios.inputs.changeLuminosity = myController.controlIOs.getStatusInput(pinsOfInputsOfSystem[3]);
  myController.ios.inputs.changeColor = myController.controlIOs.getStatusInput(pinsOfInputsOfSystem[4]);*/
}

void generateMyClockHour(){
  if(myController.controlClocks.getClockInThisTime(BASE_TIME)){
    //Contando os segundos
    if(actualValueOfSeconds <= MAX_SECONDS_AND_MINUTES){
      actualValueOfSeconds ++;
    }
    //Contando os minutos e redefinindo os segundos
    if(actualValueOfSeconds > MAX_SECONDS_AND_MINUTES){
      actualValueOfSeconds = 0;
      actualValueOfMinutes ++;
    }
    //Contando as horas e redefinindo os minutos
    if(actualValueOfMinutes > MAX_SECONDS_AND_MINUTES){
      actualValueOfMinutes = 0;
      actualValueOfHours ++;
    }
    //Redefinindo as horas
    if(actualValueOfHours > MAX_HOURS){
      actualValueOfHours = 0;
    }
    showHourInSerialMonitor();
  }
}

//Exibe a hora no monitor Serial
void showHourInSerialMonitor(){
  Serial.print("Horario atual = ");
  Serial.print(actualValueOfHours);
  Serial.print(":");
  Serial.print(actualValueOfMinutes);
  Serial.print(":");
  Serial.println(actualValueOfSeconds);
  
}