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

int PWM = 255; // Maximo PWM
int cont = 0; //para controlar cuantos pulsos medir

const int l_v = 70; //longitud del vector de muestras

const byte interrupt_Pin = 2;

float tiempo_anterior = 0.0, tiempo_actual = 0.0, delta_t = 0.0;
float vel_motor[l_v];
float tiempo[l_v];

void setup() {
  pinMode(interrupt_Pin, INPUT); // pin para leer pulsos del encoder por interrupcion
  attachInterrupt(digitalPinToInterrupt(interrupt_Pin), pulso,  RISING); //LOW CHANGE RISING FALLING

  Serial.begin(9600); //velocidad del serial
  Serial.println("Velocidad Motor DC");

  delay (1000); // tiempo de espera antes de empezar ensayo. 
  MoverADE(); //comienza a mover los motores
}
void loop() {
}

void pulso() {
  // Caracteristicas del motor: 220 RPM, Encoder 20 pulsos/Rev

  if (cont > 0) {
    tiempo_actual = micros() * 1.0; // toma tiempo al entrar a la interrupción
    delta_t = tiempo_actual - tiempo_anterior; //calcula tiempo transcurrido
    if (delta_t > 3409) //13636 us es el maximo periodo a máxima velocidad (220rev/mim*20pulsos/rev*1min/60s)^-1, 6818 es la mitad, 3409 es 1/4
    {
      //[(1 pulso)/(20 pulsos/Rev)]/(delta_tiempo_us)*((1000000 us/1 s)*(60 s/1 min))=Rev/Min.
      vel_motor[cont - 1] = (((1.0 / 20.0) / (delta_t)) * (60 * 1.0e6)); // almacena velocidad instantanea del motor
      tiempo[cont - 1] = delta_t; //almacena tiempo
      cont++; //incremento para posicion en los vectores
      tiempo_anterior = tiempo_actual; // nuevo tiempo anterior
    }
  }

  if (cont > l_v) { //una vez supere la cantidad de muestras deseadas (l_v) para e imprime
    Parar();
    for (int i = 0; i <= l_v; i++) {
      Serial.print(vel_motor[i], 1);
      Serial.print(" - ");
      Serial.print(tiempo[i], 1);
      Serial.print(" - ");
      Serial.println(i);
    }
    cont = 0;
  }
  if (cont == 0) {
    tiempo_anterior = micros() * 1.0;  // tomar tiempo inicial. Solo entra una vez
    cont++;
  }
}

void MoverADE()
{
  E1 = 1; E2 = 0; E3 = 1; E4 = 0;
  Mover();
}

void Parar()
{
  E1 = 0; E2 = 0; E3 = 0; E4 = 0;
  Mover();
}

void Mover()
{
  analogWrite (PinIN1, PWM * E1);
  analogWrite (PinIN2, PWM * E2);
  analogWrite (PinIN3, PWM * E3);
  analogWrite (PinIN4, PWM * E4);
}
