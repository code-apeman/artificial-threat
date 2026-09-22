#include <allegro.h>

void keyboard_handler(int scancode) {
    // evil bitwise fuckery
    if (scancode & 0x80) switch (scancode & 0x7f) {
        case KEY_LEFT:  control_state &= ~CONTROL_LEFT;          break;
        case KEY_RIGHT: control_state &= ~CONTROL_RIGHT;         break;
        case KEY_UP:    control_state &= ~CONTROL_UP;            break;
        case KEY_DOWN:  control_state &= ~CONTROL_DOWN;          break;
        case KEY_Z:     control_state &= ~CONTROL_JUMP;          break;
        case KEY_X:     control_state &= ~CONTROL_SPRINT;        break;
        case KEY_C:     control_state &= ~CONTROL_ATTACK_MELEE;  break;
        case KEY_A:     control_state &= ~CONTROL_ATTACK_RANGED; break;
    } else switch (scancode & 0x7f) {
        case KEY_ESC: game_exit_flag = true; break;

        case KEY_LEFT:  control_state |=  CONTROL_LEFT;          break;
        case KEY_RIGHT: control_state |=  CONTROL_RIGHT;         break;
        case KEY_UP:    control_state |=  CONTROL_UP;            break;
        case KEY_DOWN:  control_state |=  CONTROL_DOWN;          break;
        case KEY_Z:     control_state |=  CONTROL_JUMP;          break;
        case KEY_X:     control_state |=  CONTROL_SPRINT;        break;
        case KEY_C:     control_state |=  CONTROL_ATTACK_MELEE;  break;
        case KEY_A:     control_state |=  CONTROL_ATTACK_RANGED; break;
    }
}
END_OF_FUNCTION(keyboard_handler)

void frame_timer_callback(){
    frame_flag = true;
}
END_OF_FUNCTION(frame_timer_callback)

void input_init(void){
    install_timer();
    install_keyboard();

    LOCK_VARIABLE(control_state);
    LOCK_FUNCTION(keyboard_handler);
    keyboard_lowlevel_callback = keyboard_handler;

    LOCK_VARIABLE(frame_flag);
    LOCK_FUNCTION(frame_timer_callback);
    install_int_ex(frame_timer_callback, FRAMETIME_HWTICKS);
}
