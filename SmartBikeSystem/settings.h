//Define GPIOs
//LEDs pinout
#define LED_DER   33
#define LED_IZQ   26
#define LED_STOP  32
#define LED_FRONT 25

//Buttons
#define PIN_BTN_DER   4
#define PIN_BTN_IZQ   2
#define PIN_BTN_BLZ   18
#define PIN_BTN_LOCK  15

//Encoder
#define PIN_ENCODER   35

//Buzzer
#define BUZZER_PIN        23
#define BUZZER_CHANNEL    0
#define BUZZER_RESOLUTION 8
//Notas musicales
#define CANT_NOTES 8
#define NOTAS_MUSIC (note_t[]){NOTE_C,NOTE_D,NOTE_E,NOTE_F,NOTE_G,NOTE_A,NOTE_B,NOTE_C}
#define OCTAVA_NOTA (uint8_t[]){4,4,4,4,4,4,4,5}
#define TIEMPO_NOTA (uint8_t[]){500,500,500,500,500,500,500,500}
#define GAP 200

//Tiempo de espera para evitar el efecto rebote en los pulsadores
#define TIEMPO_REBOTE 200

//Flags
bool flag_der     = false;
bool flag_izq     = false;
bool flag_blz     = false;
bool flag_lock    = false;
bool flag_encoder = false;

//States
bool state_led        = false;
bool state_led_der    = false;
bool state_led_izq    = false;
bool state_led_stop   = false;
bool state_led_front  = false;
bool state_lock       = false;
bool state_sirena     = false;

//Variables para la funcion encoder
long t_lastPulse;
float dt;
float diametro = 26;  //diámetro de la rueda de la bicicleta
float dist = 0, vel = 0, vel_last = 0;

//Dicección I2C del MPU-6050
const int mpuAddress = 0x68;

//Configuración del timer
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
