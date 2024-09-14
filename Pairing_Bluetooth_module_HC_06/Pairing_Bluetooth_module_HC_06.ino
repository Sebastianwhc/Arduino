#include <SoftwareSerial.h> // libreria para seriales

SoftwareSerial hc06(2,3); // definir el hc06 en los pines 2 y 3, ver archivo para esquema del RX y TX (pilas con los 3.3v)

String cmd=""; //variable para bytes que llegan
float sensor_val=0; // variable de prueba aleatoria

void setup(){
  //Initialize Serial Monitor
  Serial.begin(9600); //arrancar serial de arduino para monitorear
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
    hc06.print(cmd); // regresa lo que recibe
    hc06.print("_ERRE"); // agregar este final
    // Write the terminator "Carriage Return" and "Linefeed".
    hc06.write(13);
    hc06.write(10);
  }

  //Select function with cmd
  if(cmd!=""){
//    // Para enviar al Monotor serial de IDE de arduino. ABAJO
//    Serial.print("Command recieved : ");
//    Serial.println(cmd);
//    // We expect ON or OFF from bluetooth
//    if(cmd=="ON"){
//        Serial.println("Function is on");
//
//    }else if(cmd=="OFF"){
//        Serial.println("Function is off");
//    }else{
//        Serial.println("Function is off by default");
//    }
//    //// Para enviar al Monotor serial de IDE de arduino. ARRIBA
    cmd=""; //reset cmd
  }
}
