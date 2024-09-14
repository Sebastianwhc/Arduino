#include <SoftwareSerial.h> // libreria para seriales

SoftwareSerial hc06(2,3); // definir el hc06 en los pines 2 y 3, ver archivo para esquema del RX y TX (pilas con los 3.3v, puede que requiera divisor de tensión)

String cmd=""; //variable para bytes que llegan
String respuesta=""; //variable para bytes que llegan

// Pines para el L298N
int PinIN1 = 5;
int PinIN2 = 6;
int PinIN3 = 9;
int PinIN4 = 10;

// Para controlar movimiento
int E1 = 0;
int E2 = 0;
int E3 = 0;
int E4 = 0;

int Max_Vel = 153; // Maxima Velocidad
int Min_Vel = 0; // Minima Velocidad
int P_Ramp = 10; // Pausa para rampa de aceleración y desaceleración

void setup(){
  //Initialize Bluetooth Serial Port
  hc06.begin(9600); // arrancar bluetooth
}

void loop(){
  //Read data from HC06
  
  if(hc06.available()>0){ // es mayor que 0 cuando llega algo al buffer
    delay(10); // esperar un poco par que teminen de cargar los datos
    while(hc06.available()>0){ //ejecutar ciclo para leer todo lo que tenga el buffer
      cmd+=(char)hc06.read();
      }
  }
  //Select function with cmd
  if(cmd!=""){
    if(cmd=="ADE"){
        Responder();
        MoverADE();
    }
    if(cmd=="ATR"){
        Responder();
        MoverATR();
    }
    if(cmd=="DER"){
        Responder();
        MoverDER();
    }
    if(cmd=="IZQ"){
        Responder();
        MoverIZQ();
    }
  }
}


void Responder()
{
    respuesta=cmd+"_ERRE"+char(13)+char(10);
    hc06.print(respuesta); // regresa lo que recibe + cadena
    // Write the terminator "Carriage Return"=13 and "Linefeed"=10.
    cmd=""; //reset cmd
    respuesta=""; //reset respuesta
}

void MoverADE()
{
  E1=1; E2=0; E3=1; E4=0;
  Mover();
  Parar();
}
void MoverATR()
{
  E1=0; E2=1; E3=0; E4=1;
  Mover();
  Parar();
}

void MoverIZQ()
{
  E1=1; E2=0; E3=0; E4=1;
  Mover();
  Parar();
}

void MoverDER()
{
  E1=0; E2=1; E3=1; E4=0;
  Mover();
  Parar();
}

void Parar()
{
  analogWrite (PinIN1, 0);
  analogWrite (PinIN2, 0);
  analogWrite (PinIN3, 0);
  analogWrite (PinIN4, 0);
}

void Mover()
{
  for (int i=Min_Vel;i<=Max_Vel;i++){
    analogWrite (PinIN1, i*E1);
    analogWrite (PinIN2, i*E2);
    analogWrite (PinIN3, i*E3);
    analogWrite (PinIN4, i*E4);
    delay(P_Ramp);
  }
  for (int i=Max_Vel;i>=Min_Vel;i--){
    analogWrite (PinIN1, i*E1);
    analogWrite (PinIN2, i*E2);
    analogWrite (PinIN3, i*E3);
    analogWrite (PinIN4, i*E4);
    delay(P_Ramp);
  }
}
