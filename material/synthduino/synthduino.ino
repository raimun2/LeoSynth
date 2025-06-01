#define PWM_VALUE     OCR2B
#define LED_PORT      PORTB
#define LED_BIT       5
#define PWM_INTERRUPT TIMER2_OVF_vect

void audioOn() {
  TCCR2A = _BV(COM2B1) | _BV(WGM20);
  TCCR2B = _BV(CS20);
  TIMSK2 = _BV(TOIE2);
}


const int audio_out = 3;  
const int button01 = 2;  
const int button02 = A5;  
const int button03 = 4;  
const int button04 = 5;  
const int button05 = 6;  
const int button06 = 7;  
const int button07 = 8;  
const int button08 = 9;  
const int button09 = 10;  
const int button10 = 11;  
const int button11 = 12;  
const int button12 = 13;  
const int perilla1 = A0;  
const int perilla2 = A1;  
const int perilla3 = A2;  
const int perilla4 = A3;  


void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(audio_out, OUTPUT);
    audioOn();
    pinMode(button01, INPUT_PULLUP);
    pinMode(button02, INPUT_PULLUP);
    pinMode(button03, INPUT_PULLUP);
    pinMode(button04, INPUT_PULLUP);
    pinMode(button05, INPUT_PULLUP);
    pinMode(button06, INPUT_PULLUP);
    pinMode(button07, INPUT_PULLUP);
    pinMode(button08, INPUT_PULLUP);
    pinMode(button09, INPUT_PULLUP);
    pinMode(button10, INPUT_PULLUP);
    pinMode(button11, INPUT_PULLUP);
    pinMode(button12, INPUT_PULLUP);
    pinMode(perilla1, INPUT);
    pinMode(perilla2, INPUT);
    pinMode(perilla3, INPUT);
    pinMode(perilla4, INPUT);
}




// Smooth logarithmic mapping
//
uint16_t antilogTable[] = {
  64830,64132,63441,62757,62081,61413,60751,60097,59449,58809,58176,57549,56929,56316,55709,55109,
  54515,53928,53347,52773,52204,51642,51085,50535,49991,49452,48920,48393,47871,47356,46846,46341,
  45842,45348,44859,44376,43898,43425,42958,42495,42037,41584,41136,40693,40255,39821,39392,38968,
  38548,38133,37722,37316,36914,36516,36123,35734,35349,34968,34591,34219,33850,33486,33125,32768
};
uint16_t mapPhaseInc(uint16_t input) {
  return (antilogTable[input & 0x3f]) >> (input >> 6);
}

// Stepped chromatic mapping
//
uint16_t midiTable[] = {
  17,18,19,20,22,23,24,26,27,29,31,32,34,36,38,41,43,46,48,51,54,58,61,65,69,73,
  77,82,86,92,97,103,109,115,122,129,137,145,154,163,173,183,194,206,218,231,
  244,259,274,291,308,326,346,366,388,411,435,461,489,518,549,581,616,652,691,
  732,776,822,871,923,978,1036,1097,1163,1232,1305,1383,1465,1552,1644,1742,
  1845,1955,2071,2195,2325,2463,2610,2765,2930,3104,3288,3484,3691,3910,4143,
  4389,4650,4927,5220,5530,5859,6207,6577,6968,7382,7821,8286,8779,9301,9854,
  10440,11060,11718,12415,13153,13935,14764,15642,16572,17557,18601,19708,20879,
  22121,23436,24830,26306
};
uint16_t mapMidi(uint16_t input) {
  return (midiTable[(1023-input) >> 3]);
}

// Stepped Pentatonic mapping
//
uint16_t pentatonicTable[54] = {
  0,19,22,26,29,32,38,43,51,58,65,77,86,103,115,129,154,173,206,231,259,308,346,
  411,461,518,616,691,822,923,1036,1232,1383,1644,1845,2071,2463,2765,3288,
  3691,4143,4927,5530,6577,7382,8286,9854,11060,13153,14764,16572,19708,22121,26306
};

uint16_t mapPentatonic(uint16_t input) {
  uint8_t value = (1023-input) / (1024/53);
  return (pentatonicTable[value]);
}

// all piano notes
//
uint16_t pianoNotes[87] = { 
  27.50, 29.14, 30.87, 32.70, 34.65, 36.71, 38.89, 41.20, 43.65, 46.25, 49.00, 51.91, 
  55.00, 58.27, 61.74, 65.41, 69.30, 73.42, 77.78, 82.41, 87.31, 92.50, 98.00, 103.83, 
  110.00, 116.54, 123.47, 130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.00, 196.00, 207.65, 
  220.00, 233.08, 246.94, 261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.00, 415.30, 
  440.00, 466.16, 493.88, 523.25, 554.37, 587.33, 622.25, 659.26, 698.46, 739.99, 783.99, 830.61, 
  880.00, 932.33, 987.77, 1046.50, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98, 1661.22, 
  1760.00, 1864.66, 1975.53, 2093.00, 2217.46, 2349.32, 2489.02, 2637.02, 2793.83, 2959.96, 3135.96, 3322.44, 
  3520.00, 3729.31, 3951.07
};

uint16_t mapPiano(uint16_t input) {
  uint8_t value = (1023-input) / (1024/86);
  return (pianoNotes[value]);
}

// Fibonacci Sequence, low-frequency
//
uint16_t fibonacciBeatTable[20] = {
  1, 1, 2 , 3, 5, 8, 13, 21, 34, 55, 89, 144
};


uint16_t mapFibonacciBeat(uint16_t input) {
  uint8_t value = (1023 - input) / (1024 / 20);
  return (fibonacciBeatTable[value]);
}

// Linear, low-frequency, such as for danceable rhythms or an LFO.
//
uint16_t bpmTable[16] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
};

uint16_t mapBPM(uint16_t input) {
  uint8_t value = (1023 - input) / (1024 / 16);
  return (bpmTable[value]);
}



// hiperparametros synth
uint16_t syncPhaseAcc;
uint16_t syncPhaseInc = 923;
uint16_t grainPhaseAcc;
uint16_t grainPhaseInc;
uint16_t grainAmp;
uint8_t grainDecay = 0;
uint16_t grain2PhaseAcc;
uint16_t grain2PhaseInc;
uint16_t grain2Amp;
uint8_t grain2Decay = 0;

int indicenotas = 20;
int estadoprevio = 0;
// arreglos grabadora
int pattarray[128];
int timearray[128];
int contador = 0;
int whiles = 0;
int maxcontador = 0;



void grainStart () {
  grainPhaseAcc = 0;
  grainAmp = 0x7fff;
  grain2PhaseAcc = 0;
  grain2Amp = 0x7fff;
  LED_PORT ^= 1 << LED_BIT; // Faster than using digitalWrite. You can use this for a Gate signal.
}

void grainBuild () {
  // Smooth frequency mapping
  //syncPhaseInc = mapPhaseInc(analogRead(SYNC_CONTROL)) / 4;

  // Stepped mapping to MIDI notes: C, Db, D, Eb, E, F...
  //syncPhaseInc = mapMidi(analogRead(SYNC_CONTROL));

  // Stepped pentatonic mapping: D, E, G, A, B
  // syncPhaseInc = mapPentatonic(analogRead(SYNC_CONTROL));

  // Stepped mapping to low-Hz Fibonacci beats: 1, 1, 2, 3, 5, 8, 13...
  // syncPhaseInc = mapFibonacciBeat(analogRead(SYNC_CONTROL));

  // Linear, low-Hz mapping: 1, 2, 3, 4...
  /*syncPhaseInc = mapBPM(analogRead(SYNC_CONTROL));

  // Read the grain inputs and send them to the timers
  grainPhaseInc  = mapPhaseInc(analogRead(GRAIN_FREQ_CONTROL)) / 2;
  grainDecay     = analogRead(GRAIN_DECAY_CONTROL) / 8;
  grain2PhaseInc = mapPhaseInc(analogRead(GRAIN2_FREQ_CONTROL)) / 2;
  grain2Decay    = analogRead(GRAIN2_DECAY_CONTROL) / 4;*/

  grainPhaseInc  = mapPhaseInc(analogRead(perilla1)) / 2;
  grainDecay     =  0; 
  grain2PhaseInc = mapPhaseInc(analogRead(perilla2)) / 4;
  grain2Decay    = 0; 
}




void loop() {

  /*int grabando = digitalRead(button01);

  // si grabando y antes no grabando
  // reinicia arreglos; contadores en cero; grabando = TRUE
  if((grabando == LOW) & (estadoprevio == 0)){
    contador = 0;
    whiles = 0;
    for(int pin = 0; pin<128; pin++){ 
      pattarray[pin] = 0;
      timearray[pin] = 0;
    }
    estadoprevio == 1;
  }
   // si grabando y antes grabando
  // almacena; contador+1
  if((grabando == LOW) & estadoprevio == 1){
    whiles += 1;
    if (digitalRead(button05) == LOW) {
      timearray[contador] = whiles;
      pattarray[contador] = 3;
      contador += 1;
    }
    else if (digitalRead(button06) == LOW) {
      timearray[contador] = whiles;
      pattarray[contador] = 2;
      contador += 1;
    }
    else if (digitalRead(button07) == LOW) {
      timearray[contador] = whiles;
      pattarray[contador] = 1;
      contador += 1;
    }
    else if (digitalRead(button08) == LOW) {
      timearray[contador] = whiles;
      pattarray[contador] = 0;
      contador += 1;
    }
    else if (digitalRead(button09) == LOW) {
      timearray[contador] = whiles;
      pattarray[contador] = 7;
      contador += 1;
    }
    else if (digitalRead(button10) == LOW) {
      timearray[contador] = whiles;
      pattarray[contador] = 6;
      contador += 1;
    }
    else if (digitalRead(button11) == LOW) {
      timearray[contador] = whiles;
      pattarray[contador] = 5;
      contador += 1;
    }
    else if (digitalRead(button12) == LOW) {
      timearray[contador] = whiles;
      pattarray[contador] = 4;
      contador += 1;
    }
  } 

  // recien termina de grabar ajusta parametros de reproduccion
  if(grabando == HIGH && estadoprevio == 1){
    estadoprevio = 0;
    maxcontador = contador;
    contador = 0;
    whiles = 0;
  }

  // bucle de reproduccion
  whiles += 1;
  if(grabando == HIGH && estadoprevio == 0 && whiles == timearray[contador]){

    syncPhaseInc = pentatonicTable[indicenotas+pattarray[contador]]; 
    contador += 1;
    if(contador > maxcontador){
      contador = 0;
      whiles = 0;
    }
  }*/
  
  int pinValue3 = analogRead(perilla3);
  int pinValue4 = analogRead(perilla4);


  if (digitalRead(button05) == LOW) {
    syncPhaseInc = pentatonicTable[indicenotas+3]; // tone(AUDIO_PIN, , 100); // Hacemos que el buzzer ejecute la nota
  }
  else if (digitalRead(button06) == LOW) {
    syncPhaseInc = pentatonicTable[indicenotas+2]; // Hacemos que el buzzer ejecute la nota
  }
  else if (digitalRead(button07) == LOW) {
    syncPhaseInc = pentatonicTable[indicenotas+1]; // Hacemos que el buzzer ejecute la nota
  }
  else if (digitalRead(button08) == LOW) {
    syncPhaseInc = pentatonicTable[indicenotas+0]; // Hacemos que el buzzer ejecute la nota
  }
  else if (digitalRead(button09) == LOW) {
    syncPhaseInc = pentatonicTable[indicenotas+7]; // Hacemos que el buzzer ejecute la nota
  }
  else if (digitalRead(button10) == LOW) {
    syncPhaseInc = pentatonicTable[indicenotas+6]; // Hacemos que el buzzer ejecute la nota
  }
  else if (digitalRead(button11) == LOW) {
    syncPhaseInc = pentatonicTable[indicenotas+5]; // Hacemos que el buzzer ejecute la nota
  }
  else if (digitalRead(button12) == LOW) {
    syncPhaseInc = pentatonicTable[indicenotas+4]; // Hacemos que el buzzer ejecute la nota
  } else{
   // syncPhaseInc = 0;
   // grainPhaseAcc = 0;
    
  }

  
  // The loop is pretty simple - it just updates the parameters for the oscillators.
  //
  // Avoid using any functions that make extensive use of interrupts, or turn interrupts off.
  // They will cause clicks and poops in the audio.
  
  // Smooth frequency mapping
  //syncPhaseInc = mapPhaseInc(analogRead(SYNC_CONTROL)) / 4;
  
  // Stepped mapping to MIDI notes: C, Db, D, Eb, E, F...
  //syncPhaseInc = mapMidi(analogRead(SYNC_CONTROL));
  
  // Stepped pentatonic mapping: D, E, G, A, B
  // syncPhaseInc = mapPentatonic(analogRead(SYNC_CONTROL));

    grainBuild();


}


SIGNAL(PWM_INTERRUPT){
  uint8_t value;
  uint16_t output;

  syncPhaseAcc += syncPhaseInc;
  if (syncPhaseAcc < syncPhaseInc) {
    // Time to start the next grain
      grainStart();
  }
  
  // Increment the phase of the grain oscillators
  grainPhaseAcc += grainPhaseInc;
  grain2PhaseAcc += grain2PhaseInc;

  // Convert phase into a triangle wave
  value = (grainPhaseAcc >> 7) & 0xff;
  if (grainPhaseAcc & 0x8000) value = ~value;
  // Multiply by current grain amplitude to get sample
  output = value * (grainAmp >> 8);

  // Repeat for second grain
  value = (grain2PhaseAcc >> 7) & 0xff;
  if (grain2PhaseAcc & 0x8000) value = ~value;
  output += value * (grain2Amp >> 8);

  // Make the grain amplitudes decay by a factor every sample (exponential decay)
  grainAmp -= (grainAmp >> 8) * grainDecay;
  grain2Amp -= (grain2Amp >> 8) * grain2Decay;

  // Scale output to the available range, clipping if necessary
  output >>= 9;
  if (output > 255) output = 255;

  // Output to PWM (this is faster than using analogWrite)  
  PWM_VALUE = output;
}
