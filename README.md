\# UnoSynth



A tiny \*\*Arduino Uno\*\* framework for DIY synth patches.  

\*\*Goal:\*\* separate hardware layout from musical logic, so anyone can write patches without touching pin wiring, matrix scanning, or Charlieplexing.



\- 12 buttons via 4x3 matrix (7 pins total)

&nbsp; - 8 \*\*keys\*\* + 4 \*\*function\*\* buttons

\- 3-position \*\*page switch\*\* (two inputs, both pull-up)

\- 4 \*\*potentiometers\*\*

\- 4 \*\*LEDs\*\* via \*\*Charlieplexing\*\* (3 pins)

\- \*\*Audio out\*\* on D5 (tone-based) and \*\*CV/audio in\*\* on A5



> MIT-licensed — go hack, remix, and publish your own patches!



---



\## Hardware Layout (fixed)



| Feature            | Pins                   |

|--------------------|------------------------|

| LEDs (Charlieplex) | D2, D3, D4             |

| Audio out          | D5                     |

| Button rows        | D6, D7, D8, D9         |

| Button columns     | D10, D11, D12          |

| 3-pos switch       | D13 (digital), A0 (analog), both INPUT\_PULLUP |

| Pots               | A1, A2, A3, A4         |

| Audio/CV in        | A5                     |

| USB Serial         | D0, D1 (reserved)      |



Buttons are scanned as a 4x3 matrix (first \*\*8\*\* are \*\*keys\*\*, last \*\*4\*\* are \*\*function\*\* buttons).



---



\## API (what patches use)



```cpp

\#include <UnoSynth.h>



UnoSynth.begin();         // call in setup()

UnoSynth.update();        // call each loop()



bool UnoSynth::getKey(uint8\_t i);   // i = 0..7

bool UnoSynth::getFn(uint8\_t i);    // i = 0..3

int  UnoSynth::getPot(uint8\_t i);   // i = 0..3, returns 0..1023

int  UnoSynth::getPage();           // 0..2 (page switch)



void UnoSynth::setLED(uint8\_t id, bool on); // id = 0..3



void UnoSynth::playFreq(uint16\_t hz); // tone() on D5

void UnoSynth::stopAudio();



