//#include "Volume.h" // Include the Volume library

const int knob1 = A3; // nota base (pitch)
const int knob2 = A2; // timbre
const int knob3 = A1; // decay
const int knob4 = A0; // freq osciladores 

const int button1 = 13; // record pattern
const int button2 = 9;  // record sample
const int button3 = 4;  // sustain notes hold
const int button4 = 2;  // octavador arpegiador

const int key1 = 10; 
const int key2 = 8; 
const int key3 = 5; 
const int key4 = 1; 
const int key5 = 12;
const int key6 = 7;
const int key7 = 6;
const int key8 = 0;

const int switch1 = A4; 
const int switch2 = A5; 

const int audio_in = 11;  // line in
const int audio_out = 3; // audio out


void setup() {
    Serial.begin(9600);

    pinMode(knob1, INPUT);
    pinMode(knob2, INPUT);
    pinMode(knob3, INPUT);
    pinMode(knob4, INPUT);
    
    pinMode(button1, INPUT_PULLUP);
    pinMode(button2, INPUT_PULLUP);
    pinMode(button3, INPUT_PULLUP);
    pinMode(button4, INPUT_PULLUP);

    pinMode(key1, INPUT_PULLUP);
    pinMode(key2, INPUT_PULLUP);
    pinMode(key3, INPUT_PULLUP);
    pinMode(key4, INPUT_PULLUP);
    pinMode(key5, INPUT_PULLUP);
    pinMode(key6, INPUT_PULLUP);
    pinMode(key7, INPUT_PULLUP);
    pinMode(key8, INPUT_PULLUP);

    pinMode(switch1, INPUT_PULLUP);
    pinMode(switch2, INPUT_PULLUP);

    pinMode(audio_in, INPUT);
    pinMode(audio_out, OUTPUT);
}


//Variables
int keyboardValue; //Stores analog value when button is pressed
float notes[] = { 
  27.50, 29.14, 30.87, 32.70, 34.65, 36.71, 38.89, 41.20, 43.65, 46.25, 49.00, 51.91, 
  55.00, 58.27, 61.74, 65.41, 69.30, 73.42, 77.78, 82.41, 87.31, 92.50, 98.00, 103.83, 
  110.00, 116.54, 123.47, 130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.00, 196.00, 207.65, 
  220.00, 233.08, 246.94, 261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.00, 415.30, 
  440.00, 466.16, 493.88, 523.25, 554.37, 587.33, 622.25, 659.26, 698.46, 739.99, 783.99, 830.61, 
  880.00, 932.33, 987.77, 1046.50, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98, 1661.22, 
  1760.00, 1864.66, 1975.53, 2093.00, 2217.46, 2349.32, 2489.02, 2637.02, 2793.83, 2959.96, 3135.96, 3322.44, 
  3520.00, 3729.31, 3951.07
};

int escala_cromatica[8] = {0, 1, 2, 3,  4,  5,  6, 7};
int escala_mayor[8] =     {0, 2, 4, 5,  7,  9, 11, 12};
int escala_menor[8] =     {0, 2, 3, 5,  7,  8, 10, 12};
int escala_pentma[8] =    {0, 3, 4, 7, 11, 12, 14, 16};
int escala_pentme[8] =    {0, 3, 5, 7, 10, 12, 14, 17};
int escala_blues[8] =     {0, 3, 5, 6,  7, 10, 12, 15};


int indicenotas = 39;
int escalas[8];

int nota_base;
int escala;
int bpm;
int master;

  
int record_pattern;
int record_sound;
int sustain;
int octave;

int note1;
int note2;
int note3;
int note4;
int note5;
int note6;
int note7;
int note8;

int valorBPM = 100; 
int Kick;
int Snare;
int Crash;
int freqKick = 70;
int freqSnare = 170;
int freqCrash = 440;
int octavador;

int Kickarray[64];
int Snarearray[64];
int Crasharray[64];

int contadorSnare = 0;
int contadorKick = 0;
int contadorCrash = 0;

int contador = 0;
int maxcontador;
int mincontador;


/* mientras se aprieta el amarillo se graba
 * se guardan 2 arreglos de 32 posiciones
 * ambos arreglos parten en 0
 * cada vez que se lea 1 se almacena y se aumenta el contador
 */


void loop() {

  nota_base = 1024 - analogRead(knob1);
  escala = 1024 - analogRead(knob2);
  bpm = 1024 - analogRead(knob3);
  master = 1024 - analogRead(knob4);
  
  if(escala < 170){
    for(int i=1; i<=8; i++){
      escalas[i] = escala_cromatica[i];
    }
  } else if(escala < 341){
    for(int i=1; i<=8; i++){
      escalas[i] = escala_mayor[i];
    }
  } else if(escala < 512){
    for(int i=1; i<=8; i++){
      escalas[i] = escala_menor[i];
    }
  } else if(escala < 692){
    for(int i=1; i<=8; i++){
      escalas[i] = escala_pentma[i];
    }
  } else if(escala < 862){
    for(int i=1; i<=8; i++){
      escalas[i] = escala_pentme[i];
    }
  } else if(escala > 862){
    for(int i=1; i<=8; i++){
      escalas[i] = escala_blues[i];
    }
  }

  record_pattern = 1 - digitalRead(button1);
  record_sound = 1 - digitalRead(button2);
  sustain = 1 - digitalRead(button3);
  octave = 1 - digitalRead(button4);

  note1 = 1 - digitalRead(key1);
  note2 = 1 - digitalRead(key2);
  note3 = 1 - digitalRead(key3);
  note4 = 1 - digitalRead(key4);
  note5 = 1 - digitalRead(key5);
  note6 = 1 - digitalRead(key6);
  note7 = 1 - digitalRead(key7);
  note8 = 1 - digitalRead(key8);

  float factor_nronotas = ((float) (12 * 7 + 3 - 17) / 1024);
  

  indicenotas = (int) (nota_base*factor_nronotas);

//  Serial.println(indicenotas);


  if(note1 == 1){
      tone(audio_out, notes[indicenotas + escalas[0]], 100); 
  }
  if(note2 == 1){
      tone(audio_out, notes[indicenotas + escalas[1]], 100); 
  }  
  if(note3 == 1){
      tone(audio_out, notes[indicenotas + escalas[2]], 100); 
  }
  if(note4 == 1){
      tone(audio_out, notes[indicenotas + escalas[3]], 100); 
  }
  if(note5 == 1){
      tone(audio_out, notes[indicenotas + escalas[4]], 100); 
  }  
  if(note6 == 1){
      tone(audio_out, notes[indicenotas + escalas[5]], 100); 
  }
  if(note7 == 1){
      tone(audio_out, notes[indicenotas + escalas[6]], 100); 
  }
  if(note8 == 1){
      tone(audio_out, notes[indicenotas + escalas[7]], 100); 
  }  



  // elimino grabacion previa
  if(record_pattern == LOW){ 
    contador = 0;
    maxcontador = 0;
    mincontador = 0; 
    contadorKick = 0;
    contadorSnare = 0;
    contadorCrash = 0;
    for(int pin = 0; pin < 64; pin++){ 
      Kickarray[pin] = 0;
      Snarearray[pin] = 0;
      Crasharray[pin] = 0;
    }
  }
  /* 
  // mientras apreto boton amarillo, luego de borrar todo, graba los primeros 64 momentos de cada pieza
  while(record_pattern == LOW){ 
     record_pattern = digitalRead(button1);
     if(contadorKick < 64 && contadorSnare < 64 && contadorCrash < 64){
      contador += 1;
      Kick = digitalRead(key1);
      Snare = digitalRead(key2);
      Crash = digitalRead(key3);
      if(Kick ==  0){
       // tone(audio_out, freqKick, 100);
        Kickarray[contadorKick] =  contador;
        // Serial.println(Kick);

        if(mincontador==0){
          mincontador = contador;
        }
        contadorKick += 1;
      }
      if(Snare ==  0){
       // tone(audio_out, freqSnare, 100);
        Snarearray[contadorSnare] =  contador;
        if(mincontador==0){
          mincontador = contador;
        }
        // Serial.println(contadorSnare);

        contadorSnare += 1;
      }
      if(Crash ==  0){
       // tone(audio_out, freqCrash, 100);
        Crasharray[contadorCrash] =  contador;
        if(mincontador==0){
          mincontador = contador;
        }
        contadorCrash += 1;
        // Serial.println(contadorCrash);
      }
      maxcontador = contador;
    }
    delay(500); // cada 50 milisegundos evalua si se graba alguna nota mas
  }
*/
// con "delay" (reproduccion)
 /*
 valorBPM = 10 + analogRead(knob1) / 13; 
 if(millis() % valorBPM == 0){
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
      tone(audio_out, freqKick, valorBPM + 10);
      contadorKick += 1;
    }
    if(Snarearray[contadorSnare] == contador) {
      tone(audio_out, freqSnare, valorBPM + 10);
      contadorSnare += 1;
    }
    if(Crasharray[contadorCrash] == contador) {
      tone(audio_out, freqCrash, valorBPM + 10);
      contadorCrash += 1;
    }  
  }
 }
   */
  // con delay (teclado)
  /*     
  if(millis() % 10 == 0){

    keyboardValue = analogRead(A0); // Leo la tecla
  
    indicenotas = analogRead(knob2)*0.0839; // Mapeo la primera nota

    octavador = digitalRead(button2); 

    if(octavador == LOW){
       indicenotas = indicenotas + 12; // subo 1 octava si uso ultrasonido
    }
    
    // codigo para calibrar las resistencias
    maxbut = max(maxbut, keyboardValue);
    minbut = min(minbut, keyboardValue);
    
    if(keyboardValue == 0){
      maxbut = 0;
      minbut = 1023;
    }
    
    //Serial.println(maxbut);
    //Serial.println(minbut);
   

    }
  }  */

}
