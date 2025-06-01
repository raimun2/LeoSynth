//#include "Volume.h" // Include the Volume library
#include <SensorToButton.h>  //https://github.com/nathanRamaNoodles/SensorToButton  acquired by JChristensen's popular button library
#include <NoodleSynth.h>
synthEngine mixer(20E3);
//const char note1[] PROGMEM = "C Scale:d=4:c,d,e,f,g,a,b,c1";   //plays c scale,  You can add more notes to get creative. //d=4 means that every note without a number in front of the letter is assumed to be a quarter note.
//const char note2[] PROGMEM = ":d=4:e,f,g,a,b,c1,d1,e1";   //plays c scale starting at e
const char note1[] PROGMEM = {"::c"};   //plays c ,  You can add more notes to get creative.
const char note2[] PROGMEM = {"::d"};  //plays e
const char note3[] PROGMEM = {"::e"};  //plays f
const char note4[] PROGMEM = {"::f"};  //plays g
const char note5[] PROGMEM = {"::g"};  //plays g
const char note6[] PROGMEM = {"::a"};  //plays g
const char note7[] PROGMEM = {"::b"};  //plays g
const char note8[] PROGMEM = {"::c1"};  //plays g

MusicWithoutDelay pianoKey(note1);
MusicWithoutDelay pianoKey2(note2);
MusicWithoutDelay pianoKey3(note3);
MusicWithoutDelay pianoKey4(note4);
MusicWithoutDelay pianoKey5(note5);
MusicWithoutDelay pianoKey6(note6);
MusicWithoutDelay pianoKey7(note7);
MusicWithoutDelay pianoKey8(note8);

#define BUTTON_PIN 10       //Connect a tactile button switch (or something similar) from Arduino pin 2 to ground.
#define BUTTON_PIN2 8       //Connect a tactile button switch (or something similar) from Arduino pin 3 to ground.
#define BUTTON_PIN3 5
#define BUTTON_PIN4 1
#define BUTTON_PIN5 12       //Connect a tactile button switch (or something similar) from Arduino pin 2 to ground.
#define BUTTON_PIN6 7       //Connect a tactile button switch (or something similar) from Arduino pin 3 to ground.
#define BUTTON_PIN7 6
#define BUTTON_PIN8 0

#define DEBOUNCE_MS 20     //A debounce time of 70 milliseconds usually works well for noisy button switches. if not, try 20.
SensorToButton firstKey(BUTTON_PIN, DEBOUNCE_MS);    //Declare the button
SensorToButton secondKey(BUTTON_PIN2, DEBOUNCE_MS);
SensorToButton thirdKey(BUTTON_PIN3, DEBOUNCE_MS);
SensorToButton fourthKey(BUTTON_PIN4, DEBOUNCE_MS);
SensorToButton fifthKey(BUTTON_PIN5, DEBOUNCE_MS);
SensorToButton sixthKey(BUTTON_PIN6, DEBOUNCE_MS);
SensorToButton seventhKey(BUTTON_PIN7, DEBOUNCE_MS);
SensorToButton eigthKey(BUTTON_PIN8, DEBOUNCE_MS);

const int knob1 = A3; // nota base (pitch)
const int knob2 = A2; // timbre
const int knob3 = A1; // decay
const int knob4 = A0; // freq osciladores 

const int button1 = 13; // record pattern
const int button2 = 9;  // record sample
const int button3 = 4;  // sustain notes hold
const int button4 = 2;  // octavador arpegiador

const int switch1 = A4; // synth
const int switch2 = A5; // granular

const int audio_in = 11;  // mic in
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

   // pinMode(key1, INPUT_PULLUP);
   // pinMode(key2, INPUT_PULLUP);
   // pinMode(key3, INPUT_PULLUP);
   // pinMode(key4, INPUT_PULLUP);
   // pinMode(key5, INPUT_PULLUP);
   // pinMode(key6, INPUT_PULLUP);
   // pinMode(key7, INPUT_PULLUP);
   // pinMode(key8, INPUT_PULLUP);
    pianoKey.begin(CHB, TRIANGLE, ENVELOPE0, 0);
    pianoKey2.begin(TRIANGLE, ENVELOPE0, 0);
    pianoKey3.begin(TRIANGLE, ENVELOPE0, 0);
    pianoKey4.begin(TRIANGLE, ENVELOPE0, 0);
    pianoKey5.begin(TRIANGLE, ENVELOPE0, 0);
    pianoKey6.begin(TRIANGLE, ENVELOPE0, 0);
    pianoKey7.begin(TRIANGLE, ENVELOPE0, 0);
    pianoKey8.begin(TRIANGLE, ENVELOPE0, 0);

    pianoKey.pause(true); pianoKey2.pause(true); pianoKey3.pause(true); pianoKey4.pause(true);
    pianoKey5.pause(true); pianoKey6.pause(true); pianoKey7.pause(true); pianoKey8.pause(true);

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

int valorBPM = 100; 
int Kick;
int Snare;
int Crash;
int freqKick = 70;
int freqSnare = 170;
int freqCrash = 440;
int octavador;

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

  float factor_nronotas = ((float) (12 * 7 + 3 - 17) / 1024);

  indicenotas = (int) (nota_base*factor_nronotas);

//MusicWithoutDelay pianoKey(note1);  
  pianoKey.update();
 // pianoKey.setFrequency(notes[indicenotas + escalas[0]]);

  pianoKey2.update();
  pianoKey3.update();
  pianoKey4.update();
  pianoKey5.update();
  pianoKey6.update();
  pianoKey7.update();
  pianoKey8.update();

  firstKey.read();
  secondKey.read();
  thirdKey.read();
  fourthKey.read();
  fifthKey.read();
  sixthKey.read();
  seventhKey.read();
  eigthKey.read();

  buttonToPiano(firstKey, pianoKey);
  buttonToPiano(secondKey, pianoKey2);
  buttonToPiano(thirdKey, pianoKey3);
  buttonToPiano(fourthKey, pianoKey4);
  buttonToPiano(fifthKey, pianoKey5);
  buttonToPiano(sixthKey, pianoKey6);
  buttonToPiano(seventhKey, pianoKey7);
  buttonToPiano(eigthKey, pianoKey8);

  

  
}

void buttonToPiano(SensorToButton b, MusicWithoutDelay &m) {  //Must put & before object, because we must point to the address of the Object in the Arduino Memory
  if (b.isPressed()) {
    m.play(1);    //play one time

  }
}
