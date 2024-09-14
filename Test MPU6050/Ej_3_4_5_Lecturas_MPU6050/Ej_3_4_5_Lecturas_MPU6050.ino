// Librerias I2C para controlar el mpu6050
// la libreria MPU6050.h necesita I2Cdev.h, I2Cdev.h necesita Wire.h
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
// La dirección del MPU6050 puede ser 0x68 o 0x69, dependiendo
// del estado de AD0. Si no se especifica, 0x68 estará implicito
MPU6050 sensor;
// Valores RAW (sin procesar) del acelerometro y giroscopio en los ejes x,y,z
int ax, ay, az;
int gx, gy, gz;

long tiempo_prev, dt;
float girosc_ang_x, girosc_ang_y;
float girosc_ang_x_prev, girosc_ang_y_prev;

void setup() {
    Serial.begin(9600); //Iniciando puerto serial
    Wire.begin(); //Iniciando I2C
    sensor.initialize(); //Iniciando el sensor
    if (sensor.testConnection()) Serial.println("Sensor iniciado correctamente");
    else Serial.println("Error al iniciar el sensor");
    
    tiempo_prev=millis(); // para calcular dt 
}
void loop() {
    // Leer las aceleraciones y velocidades angulares
    sensor.getAcceleration(&ax, &ay, &az);
    sensor.getRotation(&gx, &gy, &gz);

    //Escalando lecturas: pueden ser 2g/4g/8g/16g para el acelerómetro y 250/500/1000/2000(°/s) para el giroscopio
    //Para este ejemplo trabajaremos con los rangos por defecto (2g y 250°/s):
    float ax_m_s2 = ax * (9.81/16384.0);
    float ay_m_s2 = ay * (9.81/16384.0);
    float az_m_s2 = az * (9.81/16384.0);
    float gx_deg_s = gx * (250.0/32768.0);
    float gy_deg_s = gy * (250.0/32768.0);
    float gz_deg_s = gz * (250.0/32768.0);
    
    //Mostrar las lecturas separadas por un [tab]
    Serial.print("a[x y z](m/s^2): ");
    Serial.print(ax_m_s2); Serial.print(" ");
    Serial.print(ay_m_s2); Serial.print(" ");
    Serial.print(az_m_s2);
    Serial.print(" -- g[x y z](deg/s): ");
    Serial.print(gx_deg_s); Serial.print(" ");
    Serial.print(gy_deg_s); Serial.print(" ");
    Serial.print(gz_deg_s);

    //Calcular los angulos de inclinacion:
    float accel_ang_x=atan(ax/sqrt(pow(ay,2) + pow(az,2)))*(180.0/3.14);
    float accel_ang_y=atan(ay/sqrt(pow(ax,2) + pow(az,2)))*(180.0/3.14);
    //Mostrar los angulos separadas por un [tab]
    Serial.print(" -- Ang_X: ");
    Serial.print(accel_ang_x);
    Serial.print(" Ang_Y: ");
    Serial.print(accel_ang_y);

    //Calcular los angulos rotacion:
    dt = millis()-tiempo_prev;
    tiempo_prev=millis();
    girosc_ang_x = (gx/131)*dt/1000.0 + girosc_ang_x_prev;
    girosc_ang_y = (gy/131)*dt/1000.0 + girosc_ang_y_prev;
    girosc_ang_x_prev=girosc_ang_x;
    girosc_ang_y_prev=girosc_ang_y;
    //Mostrar los angulos separadas por un [tab]
    Serial.print(" -- Rot_X: ");
    Serial.print(girosc_ang_x);
    Serial.print(" Rot_Y: ");
    Serial.println(girosc_ang_y);

    delay(1000);

}
