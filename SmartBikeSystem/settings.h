//Define GPIOs
//LEDs
#define ledIzq    26
#define ledDer    33 
#define ledStop   32
#define ledFront  25

//Buttons    
#define btn_ledIzq  2
#define btn_ledDer  4
#define btn_alarm   5

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