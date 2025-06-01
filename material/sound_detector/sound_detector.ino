#define MICROFONO (5)
#define PWM_INTERRUPT TIMER2_OVF_vect
#define PWM_VALUE     OCR2B
#define PWM_PIN       3
#define LED_PIN       13

const int pinLED = 13;
const int pinMicrophone = 9;
uint8_t microfono;



void audioOn() {
  // Set up PWM to 31.25kHz, phase accurate
  TCCR2A = _BV(COM2B1) | _BV(WGM20);
  TCCR2B = _BV(CS20);
  TIMSK2 = _BV(TOIE2);
}


void setup ()
{
  pinMode (pinLED, OUTPUT);
  pinMode (pinMicrophone, INPUT);
  pinMode(PWM_PIN,OUTPUT);
  audioOn();
  pinMode(LED_PIN,OUTPUT);
  Serial.begin(9600);
}

void loop ()
{
  //bool soundDetected = digitalRead(pinMicrophone);

  microfono    = analogRead(MICROFONO);  
  Serial.println(microfono);

}


SIGNAL(PWM_INTERRUPT)
{
  uint8_t value;
  uint16_t output;

  output = value;
  
    // Output to PWM (this is faster than using analogWrite)  
  PWM_VALUE = output;
}
