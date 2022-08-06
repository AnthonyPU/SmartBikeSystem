//Define GPIOs
//LEDs
#define ledIzq    26
#define ledDer    33 
#define ledStop   32
#define ledFront  25

//Components
#define encoder   15  
#define buzzer    23

//Structura de boton
typedef struct {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
}BTN;
//Buttons 
#define btnIzq (BTN){2, 0, false}
#define btnDer (BTN){4, 0, false}
#define btnLock (BTN){5, 0, false}