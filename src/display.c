// ================================================================================================================================
// ================================================================================================================================
// ================================================================================================================================

#include <stdarg.h>

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "display.h"

// ================================================================================================================================
// ================================================================================================================================

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define DISPLAY_WIDTH                                                       80
#define DISPLAY_HEIGHT                                                      24

#define DISPLAY_ACTUAL_WIDTH                                                160
#define DISPLAY_ACTUAL_HEIGHT                                               24

#define DISPLAY_CHARACTER_WIDTH                                             2

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define DISPLAY_STATUS_BAR_LINE                                             24

#define DISPLAY_STATUS_BAR_START                                            0xB8F00

// --------------------------------------------------------------------------------------------------------------------------------

#define DISPLAY_KEYBOARD_NUM_LOCK_START                                     0xB8F05
#define DISPLAY_KEYBOARD_NUM_LOCK_END                                       0xB8F13

#define DISPLAY_KEYBOARD_CAPS_LOCK_START                                    0xB8F1f
#define DISPLAY_KEYBOARD_CAPS_LOCK_END                                      0xB8F2f

#define DISPLAY_KEYBOARD_SHIFT_START                                        0xB8F3b
#define DISPLAY_KEYBOARD_SHIFT_END                                          0xB8F43

#define DISPLAY_KEYBOARD_CTRL_START                                         0xB8F4f
#define DISPLAY_KEYBOARD_CTRL_END                                           0xB8F55

#define DISPLAY_KEYBOARD_ALT_START                                          0xB8F61
#define DISPLAY_KEYBOARD_ALT_END                                            0xB8F65

// --------------------------------------------------------------------------------------------------------------------------------

#define DISPLAY_HOUR_MINS_SPACER_TEXT                                       0xB8F8C
#define DISPLAY_HOUR_MINS_SPACER_COLOR                                      0xB8F8D

#define DISPLAY_MINS_SECS_SPACER_TEXT                                       0xB8F96
#define DISPLAY_MINS_SECS_SPACER_COLOR                                      0xB8F97

#define DISPLAY_TIME_HOURS                                                  0xb8f86
#define DISPLAY_TIME_MINUTES                                                0xb8f90
#define DISPLAY_TIME_SECONDS                                                0xb8f9a

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ================================================================================================================================
// ================================================================================================================================

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    asmClearScreen (void);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

QWORD   _getCurrentdisplayLocation (void);

void    _tick (void);
void    _tock (void);

void    _advanceCursor(void);

void    _new_line (void);

void    _print (BYTE color, const char *format, va_list argument_list);

void    _printByteD (BYTE number, BYTE color);
void    _printWordD (WORD number, BYTE color);
void    _printDWordD (DWORD number, BYTE color);
void    _printQWordD (QWORD number, BYTE color);

void    _printByteH (BYTE number, BYTE color);
void    _printWordH (WORD number, BYTE color);
void    _printDWordH (DWORD number, BYTE color);
void    _printQWordH (QWORD number, BYTE color);

void    _putChar (char character, BYTE color);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    __putChar (char character, BYTE color);

void    __chnageAreaColor (int start, int end, BYTE color);

void    __writeString (const char *string, BYTE color);

void    __printByteD (BYTE number, BYTE color);
void    __printWordD (WORD number, BYTE color);
void    __printDWordD (DWORD number, BYTE color);
void    __printQWordD (QWORD number, BYTE color);

void    __printByteH (BYTE number, BYTE color, int call_number);
void    __printWordH (WORD number, BYTE color, int call_number);
void    __printDWordH (DWORD number, BYTE color, int call_number);
void    __printQWordH (QWORD number, BYTE color, int call_number);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ================================================================================================================================
// ================================================================================================================================

const   char            _STATUS_BAR_TEXT [] =   "  Num Lock  |  Caps Lock  |  Shift  |  Ctrl  |  Alt                   :    :    ";
const   QWORD           _DISPLAY_ADDRESS    =   0xB8000;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static  BYTE            first_run           =   true;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static  void    _display_time_section (BYTE time_section, QWORD section_offset)
{
    BYTE        *display = (BYTE *)(section_offset);
    char        array [2];

    array [0] = (char)((time_section / 10) + '0');
    array [1] = (char)((time_section % 10) + '0');

    *display = (BYTE)array[0];
    ++display;
    ++display;

    *display = (BYTE)array[1];
    ++display;
    ++display;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

DWORD                   current_line        =   0;
DWORD                   current_column      =   0;

// ================================================================================================================================
// ================================================================================================================================
// ================================================================================================================================

// ================================================================================================================================
// ==== Public functions. =========================================================================================================
// ================================================================================================================================

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    clearScreen (void)
{
    BYTE        *display = (BYTE *)_DISPLAY_ADDRESS;

    current_line = 0;
    current_column = 0;

    for (current_line = 0; current_line < DISPLAY_ACTUAL_HEIGHT - 1; ++current_line)
    {
        for (current_column = 0; current_column < DISPLAY_WIDTH; ++current_column)
        {
            *display = (BYTE)'\0';
            ++display;
            *display = DISPLAY_COLOR;
            ++display;
        }
    }

    if (first_run)
    {
        // ------------------------------------------------------------------------------------------------------------------------

        first_run = false;

        current_line = DISPLAY_STATUS_BAR_LINE;
        current_column = 0;

        __writeString (_STATUS_BAR_TEXT, DISPLAY_STATUS_BAR);

        unsetNumLock ();
        unsetCapsLock ();
        unsetShift ();
        unsetCtrl ();
        unsetAlt ();

        displayTime (0);
    }

    current_line = 0;
    current_column = 0;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    unsetNumLock (void)
{
    __chnageAreaColor (DISPLAY_KEYBOARD_NUM_LOCK_START,    DISPLAY_KEYBOARD_NUM_LOCK_END,     DISPLAY_KEY_MODE_UNSET);
}

// --------------------------------------------------------------------------------------------------------------------------------

void    setNumLock (void)
{
    __chnageAreaColor (DISPLAY_KEYBOARD_NUM_LOCK_START,    DISPLAY_KEYBOARD_NUM_LOCK_END,     DISPLAY_KEY_MODE_SET);
}

// --------------------------------------------------------------------------------------------------------------------------------

void    unsetCapsLock (void)
{
    __chnageAreaColor (DISPLAY_KEYBOARD_CAPS_LOCK_START,   DISPLAY_KEYBOARD_CAPS_LOCK_END,    DISPLAY_KEY_MODE_UNSET);
}

// --------------------------------------------------------------------------------------------------------------------------------

void    setCapsLock (void)
{
    __chnageAreaColor (DISPLAY_KEYBOARD_CAPS_LOCK_START,   DISPLAY_KEYBOARD_CAPS_LOCK_END,    DISPLAY_KEY_MODE_SET);
}

// --------------------------------------------------------------------------------------------------------------------------------

void    unsetShift (void)
{
    __chnageAreaColor (DISPLAY_KEYBOARD_SHIFT_START,       DISPLAY_KEYBOARD_SHIFT_END,        DISPLAY_KEY_MODE_UNSET);
}

// --------------------------------------------------------------------------------------------------------------------------------

void    setShift (void)
{

    __chnageAreaColor (DISPLAY_KEYBOARD_SHIFT_START,       DISPLAY_KEYBOARD_SHIFT_END,        DISPLAY_KEY_MODE_SET);
}

// --------------------------------------------------------------------------------------------------------------------------------

void    unsetCtrl (void)
{

    __chnageAreaColor (DISPLAY_KEYBOARD_CTRL_START,        DISPLAY_KEYBOARD_CTRL_END,         DISPLAY_KEY_MODE_UNSET);
}

// --------------------------------------------------------------------------------------------------------------------------------

void    setCtrl (void)
{
    __chnageAreaColor (DISPLAY_KEYBOARD_CTRL_START,        DISPLAY_KEYBOARD_CTRL_END,         DISPLAY_KEY_MODE_SET);
}

// --------------------------------------------------------------------------------------------------------------------------------

void    unsetAlt (void)
{
    __chnageAreaColor (DISPLAY_KEYBOARD_ALT_START,         DISPLAY_KEYBOARD_ALT_END,          DISPLAY_KEY_MODE_UNSET);
}

// --------------------------------------------------------------------------------------------------------------------------------

void    setAlt (void)
{
    __chnageAreaColor (DISPLAY_KEYBOARD_ALT_START,         DISPLAY_KEYBOARD_ALT_END,          DISPLAY_KEY_MODE_SET);
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    displayTime (QWORD time)
{
    BYTE       hours;
    BYTE       minutes;
    BYTE       seconds;

    seconds = (BYTE)(time % 60);
    minutes = (BYTE)(time / 60 % 60);
    hours = (BYTE)(time / 3600 % 24);

    if (seconds % 2 == 0)
    {
        _tick ();
    }
    else
    {
        _tock ();
    }

    _display_time_section (hours, DISPLAY_TIME_HOURS);
    _display_time_section (minutes, DISPLAY_TIME_MINUTES);
    _display_time_section (seconds, DISPLAY_TIME_SECONDS);
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    printNewLine (void)
{
    _putChar ('\n', DISPLAY_COLOR);
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    print (const char *format, ...)
{
    va_list     argument_list;

    va_start (argument_list, format);

    _print (DISPLAY_COLOR, format, argument_list);
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    println (const char *format, ...)
{
    va_list     argument_list;

    va_start (argument_list, format);

    _print (DISPLAY_COLOR, format, argument_list);

    _new_line ();
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    printSuccess (const char *format, ...)
{
    va_list     argument_list;

    va_start (argument_list, format);

    _print (DISPLAY_VALID, format, argument_list);
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    printError (const char *format, ...)
{
    va_list     argument_list;

    va_start (argument_list, format);

    _print (DISPLAY_ERROR, format, argument_list);
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    printHexDump (BYTE *buffer, DWORD buffer_size)
{
    // TODO - printHexDump

    UNREFERENCED_PARAMETER (buffer);
    UNREFERENCED_PARAMETER (buffer_size);
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ================================================================================================================================
// ==== Tier 1 helpers. ===========================================================================================================
// ================================================================================================================================

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    _tick (void)
{
    // ----------------------------------------------------------------------------------------------------------------------------

    BYTE        *display;

    // ----------------------------------------------------------------------------------------------------------------------------
    // Hours <-> Minutes spacer.

    display = (BYTE *)(DISPLAY_HOUR_MINS_SPACER_COLOR);

    *display = DISPLAY_TIME_TICK;

    // ----------------------------------------------------------------------------------------------------------------------------
    // Minutes <-> Seconds spacer.

    display = (BYTE *)(DISPLAY_MINS_SECS_SPACER_COLOR);

    *display = DISPLAY_TIME_TICK;

    // ----------------------------------------------------------------------------------------------------------------------------
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    _tock (void)
{
    // ----------------------------------------------------------------------------------------------------------------------------

    BYTE        *display;

    // ----------------------------------------------------------------------------------------------------------------------------
    // Hours <-> Minutes spacer.

    display = (BYTE *)(DISPLAY_HOUR_MINS_SPACER_COLOR);

    *display = DISPLAY_TIME_TOCK;

    // ----------------------------------------------------------------------------------------------------------------------------
    // Minutes <-> Seconds spacer.

    display = (BYTE *)(DISPLAY_MINS_SECS_SPACER_COLOR);

    *display = DISPLAY_TIME_TOCK;

    // ----------------------------------------------------------------------------------------------------------------------------
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    _advanceCursor(void)
{
    ++current_column;

    if (current_column >= DISPLAY_WIDTH)
    {
        _new_line ();
    }
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    _new_line (void)
{
    if (current_line + 1 < DISPLAY_HEIGHT)
    {
        ++current_line;
    }
    else
    {
        // TODO Add a scroll functionality.

        current_line = 0;
    }

    current_column = 0;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    _putChar (
    char                character,
    BYTE                color
)
{
    if (character == '\n')
    {
        _new_line ();
    }
    else if (character == '\t')
    {
        do {
            __putChar (' ', color);
        }   while (current_column % DISPLAY_TAB_SIZE != 0);
    }
    else if (character == '\b')
    {
        if (0 == current_line && 0 == current_column)
        {
            // Nothing to do here.
        }
        else
        {
            QWORD       position = _getCurrentdisplayLocation ();
            BYTE        *display = (BYTE *)position;

            if (current_column != 0)
            {
                --current_column;
                position = _getCurrentdisplayLocation ();

                display = (BYTE *)(position);
                *display = ' ';
            }
            else
            {
                if (0 < current_line)
                {
                    --current_line;
                    current_column = DISPLAY_WIDTH;

                    position = _getCurrentdisplayLocation ();

                    display = (BYTE *)(position);
                    *display = ' ';
                }
            }
        }
    }
    else if (character == '\r')
    {
        current_column = 0;
    }
    else
    {
        __putChar (character, color);
    }
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    __writeString (const char *string, BYTE color)
{
    char    *str = (char *)string;

    while (*str != '\0')
    {
        _putChar (*str, color);

        ++str;
    }
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    _print (BYTE color, const char *format, va_list argument_list)
{
    DWORD       index = 0;
    char        character;

    while (format [index] != '\0')
    {
        character = format [index];

        if (character == '%')
        {
            character = charToLower (format [++index]);

            if (character == '%')
            {
                _putChar ('%', color);
            }
            else if (character == 's')
            {
                char    *string = va_arg (argument_list, char *);

                __writeString (string, color);
            }
            else if (character == 'c')
            {
                BYTE    *output_character_ptr = va_arg (argument_list, BYTE*);
                char    output_character = output_character_ptr;

                _putChar ((char)output_character, color);
            }
            else
            {
                char    size = character;
                char    display_mode = charToLower (format [++index]);
                if (size == 'b')
                {
                    BYTE    number = (BYTE)va_arg (argument_list, QWORD);

                    if (display_mode == 'b')
                    {
                        // TODO Display numbers as bynary.
                    }
                    else if (display_mode == 'd')
                    {
                        _printByteD (number, color);
                    }
                    else if (display_mode == 'h')
                    {
                        _printByteH (number, color);
                    }
                }
                else if (size == 'w')
                {
                    WORD    number = (WORD)va_arg (argument_list, QWORD);

                    if (display_mode == 'b')
                    {
                        // TODO Display numbers as bynary.
                    }
                    else if (display_mode == 'd')
                    {
                        _printWordD (number, color);
                    }
                    else if (display_mode == 'h')
                    {
                        _printWordH (number, color);
                    }
                }
                else if (size == 'd')
                {
                    DWORD    number = (DWORD)va_arg (argument_list, QWORD);

                    if (display_mode == 'b')
                    {
                        // TODO Display numbers as bynary.
                    }
                    else if (display_mode == 'd')
                    {
                        _printDWordD (number, color);
                    }
                    else if (display_mode == 'h')
                    {
                        _printDWordH (number, color);
                    }
                }
                else if (size == 'q')
                {
                    QWORD    number = va_arg (argument_list, QWORD);

                    if (display_mode == 'b')
                    {
                        // TODO Display numbers as bynary.
                    }
                    else if (display_mode == 'd')
                    {
                        _printQWordD (number, color);
                    }
                    else if (display_mode == 'h')
                    {
                        _printQWordH (number, color);
                    }
                }
            }
        }
        else
        {
            _putChar (format [index], color);
        }

        ++index;
    }
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// --------------------------------------------------------------------------------------------------------------------------------

void    _printByteD (BYTE number, BYTE color)
{
    if (number > 0)
    {
        __printByteD (number, color);
    }
    else
    {
        __writeString ("0", color);
    }
}

// --------------------------------------------------------------------------------------------------------------------------------

void    _printWordD (WORD number, BYTE color)
{
    if (number > 0)
    {
        __printWordD (number, color);
    }
    else
    {
        __writeString ("0", color);
    }
}

// --------------------------------------------------------------------------------------------------------------------------------

void    _printDWordD (DWORD number, BYTE color)
{
    if (number > 0)
    {
        __printDWordD (number, color);
    }
    else
    {
        __writeString ("0", color);
    }
}

// --------------------------------------------------------------------------------------------------------------------------------

void    _printQWordD (QWORD number, BYTE color)
{
    if (number > 0)
    {
        __printQWordD (number, color);
    }
    else
    {
        __writeString ("0", color);
    }
}

// --------------------------------------------------------------------------------------------------------------------------------

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// --------------------------------------------------------------------------------------------------------------------------------

void    _printByteH (BYTE number, BYTE color)
{
    __writeString ("0x", color);

    if (number > 0)
    {
        __printByteH (number, color, 0);
    }
    else
    {
        __writeString ("00", color);
    }
}

// --------------------------------------------------------------------------------------------------------------------------------

void    _printWordH (WORD number, BYTE color)
{
    __writeString ("0x", color);

    if (number > 0)
    {
        __printWordH (number, color, 0);
    }
    else
    {
        __writeString ("0000", color);
    }
}

// --------------------------------------------------------------------------------------------------------------------------------

void    _printDWordH (DWORD number, BYTE color)
{
    __writeString ("0x", color);

    if (number > 0)
    {
        __printDWordH (number, color, 0);
    }
    else
    {
        __writeString ("00000000", color);
    }
}

// --------------------------------------------------------------------------------------------------------------------------------

void    _printQWordH (QWORD number, BYTE color)
{
    __writeString ("0x", color);

    if (number > 0)
    {
        __printQWordH (number, color, 0);
    }
    else
    {
        __writeString ("0000000000000000", color);
    }
}

// --------------------------------------------------------------------------------------------------------------------------------

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

QWORD   _getCurrentdisplayLocation (void)
{
    return _DISPLAY_ADDRESS + current_line * DISPLAY_ACTUAL_WIDTH + current_column * DISPLAY_CHARACTER_WIDTH;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ================================================================================================================================
// ==== Tier 2 helpers. ===========================================================================================================
// ================================================================================================================================

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    __putChar (
    char                character,
    BYTE                color
)
{
    QWORD       position = _getCurrentdisplayLocation ();
    BYTE        *display = (BYTE *)position;

    *display = (BYTE)character;

    display = (BYTE *)(position + 1);

    *display = color;

    _advanceCursor ();
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void    __chnageAreaColor (int start, int end, BYTE color)
{
    BYTE        *display = (BYTE *)_DISPLAY_ADDRESS;

    for (display = (BYTE *)((QWORD)start); display <= (BYTE *)((QWORD)end); display += 2)
    {
        *display = color;
    }
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// --------------------------------------------------------------------------------------------------------------------------------

void    __printByteD (BYTE number, BYTE color)
{
    if (number > 0)
    {
        if (number >= 10)
        {
            __printByteD (number / 10, color);
        }

        number = (BYTE)(number % 10 + (BYTE)'0');

        __putChar ((char)number, color);
    }
}

// --------------------------------------------------------------------------------------------------------------------------------

void    __printWordD (WORD number, BYTE color)
{
    if (number > 0)
    {
        if (number >= 10)
        {
            __printWordD (number / 10, color);
        }

        number = (WORD)(number % 10 + (WORD)'0');

        __putChar ((char)number, color);
    }
}

// --------------------------------------------------------------------------------------------------------------------------------

void    __printDWordD (DWORD number, BYTE color)
{
    if (number > 0)
    {
        if (number >= 10)
        {
            __printDWordD (number / 10, color);
        }

        number = (DWORD)(number % 10 + (DWORD)'0');

        __putChar ((char)number, color);
    }
}

// --------------------------------------------------------------------------------------------------------------------------------

void    __printQWordD (QWORD number, BYTE color)
{
    if (number > 0)
    {
        if (number >= 10)
        {
            __printQWordD (number / 10, color);
        }

        number = (QWORD)(number % 10 + (QWORD)'0');

        __putChar ((char)number, color);
    }
}

// --------------------------------------------------------------------------------------------------------------------------------

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// --------------------------------------------------------------------------------------------------------------------------------

void    __printByteH (BYTE number, BYTE color, int call_number)
{
    if (number > 0)
    {
        if (number >= 16)
        {
            __printByteH (number / 16, color, call_number + 1);
        }
        else
        {
            while (call_number < 1)
            {
                _putChar ('0', color);
                ++call_number;
            }
        }

        number = number % 16;

        if (number >= 10)
        {
            number = (BYTE)(number - 10 + (BYTE)'a');
        }
        else
        {
            number = (BYTE)(number + (BYTE)'0');
        }

        __putChar ((char)number, color);
    }
}

// --------------------------------------------------------------------------------------------------------------------------------

void    __printWordH (WORD number, BYTE color, int call_number)
{
    if (number > 0)
    {
        if (number >= 16)
        {
            __printWordH (number / 16, color, call_number + 1);
        }
        else
        {
            while (call_number < 3)
            {
                _putChar ('0', color);
                ++call_number;
            }
        }

        number = number % 16;

        if (number >= 10)
        {
            number = (WORD)(number - 10 + (WORD)'a');
        }
        else
        {
            number = (WORD)(number + (WORD)'0');
        }

        __putChar ((char)number, color);
    }
}

// --------------------------------------------------------------------------------------------------------------------------------

void    __printDWordH (DWORD number, BYTE color, int call_number)
{
    if (number > 0)
    {
        if (number >= 16)
        {
            __printDWordH (number / 16, color, call_number + 1);
        }
        else
        {
            while (call_number < 7)
            {
                _putChar ('0', color);
                ++call_number;
            }
        }

        number = number % 16;

        if (number >= 10)
        {
            number = (DWORD)(number - 10 + (DWORD)'a');
        }
        else
        {
            number = (DWORD)(number + (DWORD)'0');
        }

        __putChar ((char)number, color);
    }
}

// --------------------------------------------------------------------------------------------------------------------------------

void    __printQWordH (QWORD number, BYTE color, int call_number)
{
    if (number > 0)
    {
        if (number >= 16)
        {
            __printQWordH (number / 16, color, call_number + 1);
        }
        else
        {
            while (call_number < 15)
            {
                _putChar ('0', color);
                ++call_number;
            }
        }

        number = number % 16;

        if (number >= 10)
        {
            number = (QWORD)(number - 10 + (QWORD)'a');
        }
        else
        {
            number = (QWORD)(number + (QWORD)'0');
        }

        __putChar ((char)number, color);
    }
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ================================================================================================================================
// ================================================================================================================================
// ================================================================================================================================
