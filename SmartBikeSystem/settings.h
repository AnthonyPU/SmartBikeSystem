//Define GPIOs
//LEDs
#define ledIzq    26
#define ledDer    33 
#define ledStop   32
#define ledFront  25

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

//Components
#define encoder   15

//Buzzer
#define BUZZER_PIN        23
#define BUZZER_CHANNEL    0
#define BUZZER_RESOLUTION 8
//Notas musicales
#define CANT_NOTES 8
#define NOTAS_MUSIC (note_t[]){NOTE_C,NOTE_D,NOTE_E,NOTE_F,NOTE_G,NOTE_A,NOTE_B,NOTE_C}
#define OCTAVA_NOTA (uint8_t[]){4,4,4,4,4,4,4,5}
#define TIEMPO_NOTA (uint8_t[]){500,500,500,500,500,500,500,500}
#define gap 200
