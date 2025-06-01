int valorSensor = 100; // Definimos una variable para las lecturas del sensor
int Kick;
int Snare;
int Crash;
int freqKick = 70;
int freqSnare = 170;
int freqCrash = 440;
int boton = 0;
const int buttonPin = 13; 
const int KickPin = 10;  
const int SnarePin = 9;  
const int CrashPin = 8;  
const int audioPin = 2;  

int Kickarray[64];
int Snarearray[64];
int Crasharray[64];
int contadorSnare = 0;
int contadorKick = 0;
int contadorCrash = 0;

int contador = 0;
int maxcontador;
int mincontador;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(KickPin, INPUT);
  pinMode(SnarePin, INPUT);
  pinMode(CrashPin, INPUT);
  pinMode(audioPin, OUTPUT);
}

/* mientras se aprieta el amarillo se graba
 * se guardan 2 arreglos de 32 posiciones
 * ambos arreglos parten en 0
 * cada vez que se lea 1 se almacena y se aumenta el contador
 */

void loop() {
  valorSensor = 10 + analogRead(A0) / 13; // Leemos el valor del sensor a través del pin A0
  
    
  boton = digitalRead(buttonPin);
  


  // si apreto borro grabacion previa y parto de cero
  if(boton == HIGH){ 
    contador = 0;
    maxcontador = 0;
    mincontador = 0;
    contadorKick = 0;
    contadorSnare = 0;
    contadorCrash = 0;
    Serial.println("hola");

    for(int pin = 0; pin<32; pin++){
      Kickarray[pin] = 0;
      Snarearray[pin] = 0;
      Crasharray[pin] = 0;
    }
  }

  // grabacion
  while(boton == HIGH){ 
     boton = digitalRead(buttonPin);
     if(contadorKick < 64 && contadorSnare < 64 && contadorCrash < 64){
      contador += 1;
      Kick = digitalRead(KickPin);
      Snare = digitalRead(SnarePin);
      Crash = digitalRead(CrashPin);
      if(Kick ==  1){
        tone(audioPin, freqKick, 100);
        Kickarray[contadorKick] =  contador;
        //maxcontador = contador;
        if(mincontador==0){
          mincontador = contador;
        }
        contadorKick += 1;
        //Serial.println("Kick");
      }
      if(Snare ==  1){
        tone(audioPin, freqSnare, 100);
        Snarearray[contadorSnare] =  contador;
        if(mincontador==0){
          mincontador = contador;
        }
        contadorSnare += 1;
        //Serial.println("Snare");
      }
      if(Crash ==  1){
        tone(audioPin, freqCrash, 100);
        Crasharray[contadorCrash] =  contador;
        //maxcontador = contador;
        if(mincontador==0){
          mincontador = contador;
        }
        contadorCrash += 1;
       // Serial.println("Crash");
      }
      maxcontador = contador;

    }
    delay(50);     
  }  
  
  // reproduccion
  contador += 1;
  if(contador > maxcontador) {
    contador = mincontador;
    contadorKick = 0;
    contadorSnare = 0;
    contadorCrash = 0;
  }
  if(maxcontador > 0 && contador <= maxcontador && contador > 0){
    if(Kickarray[contadorKick] == contador) {
      tone(audioPin, freqKick, valorSensor + 10);
      contadorKick += 1;
    }
    if(Snarearray[contadorSnare] == contador) {
      tone(audioPin, freqSnare, valorSensor + 10);
      contadorSnare += 1;
    }
    if(Crasharray[contadorCrash] == contador) {
      tone(audioPin, freqCrash, valorSensor + 10);
      contadorCrash += 1;
    }  
    delay(valorSensor);      
  }
}
