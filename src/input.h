#ifndef INPUT_H
#define CONTROL_LEFT          0b00000001
#define CONTROL_RIGHT         0b00000010
#define CONTROL_UP            0b00000100
#define CONTROL_DOWN          0b00001000
#define CONTROL_JUMP          0b00010000
#define CONTROL_SPRINT        0b00100000
#define CONTROL_ATTACK_MELEE  0b01000000
#define CONTROL_ATTACK_RANGED 0b10000000

void input_init(void);
unsigned char input_collect(void);
#define INPUT_H
#else
#warning "Multiple inclusions of \"input.h\""
#endif
