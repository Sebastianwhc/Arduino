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

int Max_Vel = 180; // Maxima Velocidad del motor
int Max_PWM = 255; // Maximo PWM
int Min_PWM = 115; // Mínimo PWM, sacado a prueba y error para romper la inercia del motor-reducción 115?
int PWM;
int c_int = 0; // Contador ingreso a interrupciones
int T_DES;//para controlar timer

const byte interrupt_Pin = 2;

float tiempo_anterior = 0.0, tiempo_actual = 0.0, delta_t = 0.0; // para medir tiempo del cálculo de velocidad
float con_t1 = 0.0, con_t2 = 0.0, t2_t1 = 0.0; // para medir tiempo de interrupción
float vel_ref = 180, vel_motor = 0.0, vel_motor_s = 0.0; // para velocidades de motor
float error = 0.0, error_P = 0.0, error_I = 0.0, error_mod = 0.0, int_error = 0.0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // pin del led

  ///// Configuración timer1
  TCCR1A = 0;                        // El registro de control A queda todo en 0, pines OC1A y OC1B deshabilitados
  TCCR1B = 0;                        //limpia el registrador
  TCCR1B |= (1 << CS10) | (1 << CS12); // configura prescaler para 1024: CS12 = 1 e CS10 = 1
  T_DES = 65144;                    //0.025 seg -> 65536-(16MHz/1024*0.025 - 1) = 65144 //0.05 seg -> 65536-(16MHz/1024*0.05 - 1) = 64755 //0.1 seg -> 65536-(16MHz/1024*0.1 - 1) = 63975
  TCNT1 = T_DES;                    // inicia timer para desbordamiento

  TIMSK1 |= (1 << TOIE1);           // habilita la interrupcion del TIMER1
  /////

  pinMode(interrupt_Pin, INPUT); // pin para leer pulsos del encoder por interrupcion

  attachInterrupt(digitalPinToInterrupt(interrupt_Pin), pulso, RISING);// configurar interrupción en el pin externo

  Serial.begin(9600); //velocidad del serial
  Serial.println("Velocidad Motor DC");

  con_t1 = micros() * 1.0; // tomar tiempo inicial
}

void loop() {
  Serial.print(vel_ref, 1);
  Serial.print(" - ");
  Serial.print(vel_motor, 1);
  Serial.print(" - ");
  Serial.print(error, 1);
  Serial.print(" - ");
  Serial.print(error_mod, 1);
  Serial.print(" - ");
  Serial.print(PWM);
  Serial.print(" - ");
  Serial.println(t2_t1, 3);
}

void pulso() {    //interrupción por pin externo
  // Caracteristicas del motor: 220 RPM, Encoder 20 pulsos/Rev

  if (c_int > 0) {
    tiempo_actual = micros() * 1.0; // toma tiempo al entrar a la interrupción
    delta_t = tiempo_actual - tiempo_anterior; //calcula tiempo transcurrido
  }
  if (delta_t > 6818 && c_int > 0) //13636 us es el maximo periodo a máxima velocidad (220rev/mim*20pulsos/rev*1min/60s)^-1, 6818 es la mitad, 3409 es 1/4
  {
    //[(1 pulso)/(20 pulsos/Rev)]/(delta_tiempo_us)*((1000000 us/1 s)*(60 s/1 min))=Rev/Min.
    vel_motor_s = vel_motor_s * 1.0 + (((1.0 / 20.0) / (delta_t)) * (60 * 1.0e6)); // sumar velocidad del motor para posteriormente promediar
    c_int++; // contador para promediar
    tiempo_anterior = tiempo_actual; // nuevo tiempo anterior
  }
  if (c_int == 0) {
    tiempo_anterior = micros() * 1.0;  // tomar tiempo inicial. Solo entra una vez
    c_int = 1;
  }
}


ISR(TIMER1_OVF_vect)                              //interrupcion del TIMER1
{
  TCNT1 = T_DES - 1; // Renicia TIMER
  con_t2 = micros() * 1.0; //toma tiempo al ingresar a la interrupción
  t2_t1 = (1.0 * con_t2 - 1.0 * con_t1) * 1.0e-6; // calcula el tiempo transcurrido entre interruciones
  con_t1 = con_t2; //Nuevo tiempo anterior
  calcular_control();
}

void calcular_control() { // calcula y controla la velocidad del motor e imprime

  if (c_int <= 1 && (micros() * 1.0 - tiempo_actual * 1.0) > 100000.0) //c_int solo es mayor que 1 cuando ha entrado dos veces a la interrupción pulso. El motor está detenido. Se reinician variables
  {
    vel_motor = 0.0;
    vel_motor_s = 0.0;
  }
  if (c_int > 1) //c_int solo es mayor que uno cuando ha entrado a la interrupción pulso
  {
    vel_motor = 1.0 * vel_motor_s / (c_int - 1); //saca el promedio de todas las muestras en función pulso(-1 porque contador comenzó en 1)
    vel_motor_s = 0.0; //reinicia sumador de velocidad
    c_int = 1; //reinicia contador de muestras
  }
  control(); //ejecuta la acción de control
}

void control()
{
  error = 1.0 * vel_ref - 1.0 * vel_motor; //calcular el error
  error_P = error; //calcula parte proporcional
  error_I = error_I * 1.0 + 1.0 * error * t2_t1; //calcular parte integral. Integración aprox. con lapso de interrupción
  error_mod = 0.0 * error_P + 1.0 * error_I; //calcula error modificado

  PWM = 1.0 * Max_PWM * error_mod / Max_Vel; // escala el error modificado para convertirlo en PWM
  if (PWM < Min_PWM) {
    PWM = Min_PWM; //saturación inferior, menor que eso no se mueve
  }
  if (PWM > Max_PWM) {
    PWM = Max_PWM; //saturación superior,
  }
  //PWM=Max_PWM; //en caso de que se quiera a maxima velocidad

  MoverADE();// mueve el motor
}


void MoverADE()
{
  E1 = 1; E2 = 0; E3 = 1; E4 = 0;
  Mover();
}
void Mover()
{
  analogWrite (PinIN1, PWM * E1);
  analogWrite (PinIN2, PWM * E2);
  analogWrite (PinIN3, PWM * E3);
  analogWrite (PinIN4, PWM * E4);
}
