//Define GPIOs
//LEDs
#define ledIzq    26
#define ledDer    33 
#define ledStop   32
#define ledFront  25

//Components
#define encoder   15  
#define buzzer    23

//Buttons
#define PIN_BTN_DER 4
#define PIN_BTN_IZQ 2
#define PIN_BTN_LOCK 5

bool FLAG_DER = false;
bool FLAG_IZQ = false;
bool FLAG_LOCK = false;

bool STATE_DER = false;
bool STATE_IZQ = false;
bool STATE_LOCK = false;