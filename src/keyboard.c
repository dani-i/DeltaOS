// ================================================================================================================================
// ================================================================================================================================
// ================================================================================================================================

#include "keyboard.h"

// ================================================================================================================================
// ================================================================================================================================

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define KEY_PRESSED_ESC                                                     0x01
#define KEY_PRESSED_1                                                       0x02
#define KEY_PRESSED_2                                                       0x03
#define KEY_PRESSED_3                                                       0x04
#define KEY_PRESSED_4                                                       0x05
#define KEY_PRESSED_5                                                       0x06
#define KEY_PRESSED_6                                                       0x07
#define KEY_PRESSED_7                                                       0x08
#define KEY_PRESSED_8                                                       0x09
#define KEY_PRESSED_9                                                       0x0A
#define KEY_PRESSED_0                                                       0x0B
#define KEY_PRESSED_MINUS                                                   0x0C
#define KEY_PRESSED_EQUAL                                                   0x0D
#define KEY_PRESSED_BACKSPACE                                               0x0E
#define KEY_PRESSED_TAB                                                     0x0F
#define KEY_PRESSED_Q                                                       0x10
#define KEY_PRESSED_W                                                       0x11
#define KEY_PRESSED_E                                                       0x12
#define KEY_PRESSED_R                                                       0x13
#define KEY_PRESSED_T                                                       0x14
#define KEY_PRESSED_Y                                                       0x15
#define KEY_PRESSED_U                                                       0x16
#define KEY_PRESSED_I                                                       0x17
#define KEY_PRESSED_O                                                       0x18
#define KEY_PRESSED_P                                                       0x19
#define KEY_PRESSED_OPEN_BRACKET                                            0x1A
#define KEY_PRESSED_CLOSED_BRACKET                                          0x1B
#define KEY_PRESSED_ENTTER                                                  0x1C
#define KEY_PRESSED_LEFT_CTRL                                               0x1D
#define KEY_PRESSED_A                                                       0x1E
#define KEY_PRESSED_S                                                       0x1F
#define KEY_PRESSED_D                                                       0x20
#define KEY_PRESSED_F                                                       0x21
#define KEY_PRESSED_G                                                       0x22
#define KEY_PRESSED_H                                                       0x23
#define KEY_PRESSED_J                                                       0x24
#define KEY_PRESSED_K                                                       0x25
#define KEY_PRESSED_L                                                       0x26
#define KEY_PRESSED_SEMICOLON                                               0x27
#define KEY_PRESSED_QUOTE                                                   0x28
#define KEY_PRESSED_BACK_TICK                                               0x29
#define KEY_PRESSED_LEFT_SHIFT                                              0x2A
#define KEY_PRESSED_BACKSLASH                                               0x2B
#define KEY_PRESSED_Z                                                       0x2C
#define KEY_PRESSED_X                                                       0x2D
#define KEY_PRESSED_C                                                       0x2E
#define KEY_PRESSED_V                                                       0x2F
#define KEY_PRESSED_B                                                       0x30
#define KEY_PRESSED_N                                                       0x31
#define KEY_PRESSED_M                                                       0x32
#define KEY_PRESSED_COMA                                                    0x33
#define KEY_PRESSED_DOT                                                     0x34
#define KEY_PRESSED_SLASH                                                   0x35
#define KEY_PRESSED_RIGHT_SHIFT                                             0x36
#define KEY_PRESSED_KEYPAD_STAR                                             0x37
#define KEY_PRESSED_LEFT_ALT                                                0x38
#define KEY_PRESSED_SPACE                                                   0x39
#define KEY_PRESSED_CAPS_LOCK                                               0x3A
#define KEY_PRESSED_F1                                                      0x3B
#define KEY_PRESSED_F2                                                      0x3C
#define KEY_PRESSED_F3                                                      0x3D
#define KEY_PRESSED_F4                                                      0x3E
#define KEY_PRESSED_F5                                                      0x3F
#define KEY_PRESSED_F6                                                      0x40
#define KEY_PRESSED_F7                                                      0x41
#define KEY_PRESSED_F8                                                      0x42
#define KEY_PRESSED_F9                                                      0x43
#define KEY_PRESSED_F10                                                     0x44
#define KEY_PRESSED_NUM_LOCK                                                0x45
#define KEY_PRESSED_SCROLL_LOCK                                             0x46
#define KEY_PRESSED_KEYPAD_7                                                0x47
#define KEY_PRESSED_KEYPAD_8                                                0x48
#define KEY_PRESSED_KEYPAD_9                                                0x49
#define KEY_PRESSED_KEYPAD_MINUS                                            0x4A
#define KEY_PRESSED_KEYPAD_4                                                0x4B
#define KEY_PRESSED_KEYPAD_5                                                0x4C
#define KEY_PRESSED_KEYPAD_6                                                0x4D
#define KEY_PRESSED_KEYPAD_PLUS                                             0x4E
#define KEY_PRESSED_KEYPAD_1                                                0x4F
#define KEY_PRESSED_KEYPAD_2                                                0x50
#define KEY_PRESSED_KEYPAD_3                                                0x51
#define KEY_PRESSED_KEYPAD_0                                                0x52
#define KEY_PRESSED_KEYPAD_DOT                                              0x53
#define KEY_PRESSED_F11                                                     0x57
#define KEY_PRESSED_F12                                                     0x58

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define KEY_RELEASED_ESC                                                    0x81
#define KEY_RELEASED_1                                                      0x82
#define KEY_RELEASED_2                                                      0x83
#define KEY_RELEASED_3                                                      0x84
#define KEY_RELEASED_4                                                      0x85
#define KEY_RELEASED_5                                                      0x86
#define KEY_RELEASED_6                                                      0x87
#define KEY_RELEASED_7                                                      0x88
#define KEY_RELEASED_8                                                      0x89
#define KEY_RELEASED_9                                                      0x8A
#define KEY_RELEASED_0                                                      0x8B
#define KEY_RELEASED_MINUS                                                  0x8C
#define KEY_RELEASED_EQUAL                                                  0x8D
#define KEY_RELEASED_BACKSPACE                                              0x8E
#define KEY_RELEASED_TAB                                                    0x8F
#define KEY_RELEASED_Q                                                      0x90
#define KEY_RELEASED_W                                                      0x91
#define KEY_RELEASED_E                                                      0x92
#define KEY_RELEASED_R                                                      0x93
#define KEY_RELEASED_T                                                      0x94
#define KEY_RELEASED_Y                                                      0x95
#define KEY_RELEASED_U                                                      0x96
#define KEY_RELEASED_I                                                      0x97
#define KEY_RELEASED_O                                                      0x98
#define KEY_RELEASED_P                                                      0x99
#define KEY_RELEASED_OPEN_BRACKET                                           0x9A
#define KEY_RELEASED_CLOSED_BRACKET                                         0x9B
#define KEY_RELEASED_ENTTER                                                 0x9C
#define KEY_RELEASED_LEFT_CTRL                                              0x9D
#define KEY_RELEASED_A                                                      0x9E
#define KEY_RELEASED_S                                                      0x9F
#define KEY_RELEASED_D                                                      0xA0
#define KEY_RELEASED_F                                                      0xA1
#define KEY_RELEASED_G                                                      0xA2
#define KEY_RELEASED_H                                                      0xA3
#define KEY_RELEASED_J                                                      0xA4
#define KEY_RELEASED_K                                                      0xA5
#define KEY_RELEASED_L                                                      0xA6
#define KEY_RELEASED_SEMICOLON                                              0xA7
#define KEY_RELEASED_QUOTE                                                  0xA8
#define KEY_RELEASED_BACK_TICK                                              0xA9
#define KEY_RELEASED_LEFT_SHIFT                                             0xAA
#define KEY_RELEASED_BACKSLASH                                              0xAB
#define KEY_RELEASED_Z                                                      0xAC
#define KEY_RELEASED_X                                                      0xAD
#define KEY_RELEASED_C                                                      0xAE
#define KEY_RELEASED_V                                                      0xAF
#define KEY_RELEASED_B                                                      0xB0
#define KEY_RELEASED_N                                                      0xB1
#define KEY_RELEASED_M                                                      0xB2
#define KEY_RELEASED_COMA                                                   0xB3
#define KEY_RELEASED_DOT                                                    0xB4
#define KEY_RELEASED_SLASH                                                  0xB5
#define KEY_RELEASED_RIGHT_SHIFT                                            0xB6
#define KEY_RELEASED_KEYPAD_STAR                                            0xB7
#define KEY_RELEASED_LEFT_ALT                                               0xB8
#define KEY_RELEASED_SPACE                                                  0xB9
#define KEY_RELEASED_CAPS_LOCK                                              0xBA
#define KEY_RELEASED_F1                                                     0xBB
#define KEY_RELEASED_F2                                                     0xBC
#define KEY_RELEASED_F3                                                     0xBD
#define KEY_RELEASED_F4                                                     0xBE
#define KEY_RELEASED_F5                                                     0xBF
#define KEY_RELEASED_F6                                                     0xC0
#define KEY_RELEASED_F7                                                     0xC1
#define KEY_RELEASED_F8                                                     0xC2
#define KEY_RELEASED_F9                                                     0xC3
#define KEY_RELEASED_F10                                                    0xC4
#define KEY_RELEASED_NUM_LOCK                                               0xC5
#define KEY_RELEASED_SCROLL_LOCK                                            0xC6
#define KEY_RELEASED_KEYPAD_7                                               0xC7
#define KEY_RELEASED_KEYPAD_8                                               0xC8
#define KEY_RELEASED_KEYPAD_9                                               0xC9
#define KEY_RELEASED_KEYPAD_MINUS                                           0xCA
#define KEY_RELEASED_KEYPAD_4                                               0xCB
#define KEY_RELEASED_KEYPAD_5                                               0xCC
#define KEY_RELEASED_KEYPAD_6                                               0xCD
#define KEY_RELEASED_KEYPAD_PLUS                                            0xCE
#define KEY_RELEASED_KEYPAD_1                                               0xCF
#define KEY_RELEASED_KEYPAD_2                                               0xD0
#define KEY_RELEASED_KEYPAD_3                                               0xD1
#define KEY_RELEASED_KEYPAD_0                                               0xD2
#define KEY_RELEASED_KEYPAD_DOT                                             0xD3
#define KEY_RELEASED_F11                                                    0xD7
#define KEY_RELEASED_F12                                                    0xD8

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ================================================================================================================================
// ================================================================================================================================

void    _interpretKey (BYTE code, BYTE key);

// ================================================================================================================================
// ================================================================================================================================

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

const   char    default_layout [] = {
    0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0, 0
};

const   char    shift_layout [] = {
    0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0, 0
};

const   char    caps_lock_layout [] = {
    0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 0, '\\', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0, 0
};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

BYTE    num_lock        =   true;
BYTE    caps_lock       =   false;
BYTE    shift           =   false;
BYTE    ctrl            =   false;
BYTE    alt             =   false;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    handlerKeyboard ()
{
    BYTE    code;
    BYTE    key;

    picSendEOI (PIC1_INTERRUPT_KEYBOARD);

    code = inB (0x64);
    key = inB (0x60);

    switch (key)
    {
        // ------------------------------------------------------------------------------------------------------------------------

        case KEY_PRESSED_NUM_LOCK:
            if (true == num_lock)
            {
                num_lock = false;

                unsetNumLock ();
            }
            else
            {
                num_lock = true;

                setNumLock ();
            }

            break;

        // ------------------------------------------------------------------------------------------------------------------------

        case KEY_PRESSED_CAPS_LOCK:
            if (true == caps_lock)
            {
                caps_lock = false;

                unsetCapsLock ();
            }
            else
            {
                caps_lock = true;

                setCapsLock ();
            }

            break;

        // ------------------------------------------------------------------------------------------------------------------------

        case KEY_PRESSED_RIGHT_SHIFT:
        case KEY_PRESSED_LEFT_SHIFT:

            ++shift;

            setShift ();

            break;

        // ------------------------------------------------------------------------------------------------------------------------

        case KEY_RELEASED_RIGHT_SHIFT:
        case KEY_RELEASED_LEFT_SHIFT:

            --shift;

            if (0 == shift)
            {
                unsetShift ();
            }


            break;

        // ------------------------------------------------------------------------------------------------------------------------

        case KEY_PRESSED_LEFT_CTRL:

            ++ctrl;

            setCtrl ();

            break;

        // ------------------------------------------------------------------------------------------------------------------------

        case KEY_RELEASED_LEFT_CTRL:

            --ctrl;

            if (0 == shift)
            {
                unsetCtrl ();
            }

            break;

        // ------------------------------------------------------------------------------------------------------------------------

        case KEY_PRESSED_LEFT_ALT:

            ++alt;

            setAlt ();

            break;

        // ------------------------------------------------------------------------------------------------------------------------

        case KEY_RELEASED_LEFT_ALT:

            --alt;

            if (0 == shift)
            {
                unsetAlt ();
            }

            break;

        // ------------------------------------------------------------------------------------------------------------------------

        default:

            _interpretKey (code, key);

            break;

        // ------------------------------------------------------------------------------------------------------------------------
    }
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void keyboardInitialization ()
{
    addInterrupt (asmHandlerKeyboard, INTERRUPT_KEYBOARD, INTERRUPT, RING_0);

    irqClear (PIC1_INTERRUPT_KEYBOARD);

    setNumLock ();
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ================================================================================================================================
// ================================================================================================================================

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    _interpretKey (BYTE code, BYTE key)
{
    // print ("%bh %bh\n", code, key);

    UNREFERENCED_PARAMETER (code);

    if (key < 0x80)
    {
        --key;

        if (false == num_lock)
        {
            if (key == 0x37 || (key >= 0x47 && key <= 0x52))
            {
                return;
            }
        }

        if (true == shift)
        {
            if (shift_layout [key])
            {
                print ("%c", shift_layout [key]);
            }
        }
        else if (true == caps_lock)
        {
            if (caps_lock_layout [key])
            {
                print ("%c", caps_lock_layout [key]);
            }
        }
        else if (0 != default_layout [key])
        {
            print ("%c", default_layout [key]);
        }
    }
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ================================================================================================================================
// ================================================================================================================================
// ================================================================================================================================
