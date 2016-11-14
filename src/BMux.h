/*************************************************************************
 * Button Muxer Library, by Pavel Milanes, CO7WT, pavelmc@gmail.com
 *
 * This library is a derived work from the AnalogButtons library from
 * rlogiacco https://github.com/rlogiacco/AnalogButtons library.
 *
 * The main reason of this spin off in the buttons behavior. In almost all
 * interfaces when you press and hold a button you expect a hold event
 * to rise after a preset time, with the former library you will get a
 * click event and some time after that, the hold event. That's not a
 * desired behavior, and this library born to solve that.
 *
 * Thanks to rlogiacco and the others that have worked on that code, and mainly
 * for make it available under a free software license.
 *
 * You can always found the last version of this library in the github page:
 * https://github.com/pavelmc/BMux/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * **************************************************************************/

/* HOW to calculate the values for each individual button?
 *
 * Take a voltmeter and measure the +5V (V) and the voltage at the KEY_PIN
 * with each button pressed (UV) and simply use this formula to calculate each
 * button value:
 *
 * Value = UV/V * 1023
 *
 * Try to use resistors with values over 1K ohms to minimize the power drain
 * and quality ones (at least 10% tolerance).
 *
 * Alternatively you can load the example on this lib and hook up a serial port
 * to your sketch and simple READ directly the value on the serial port.
 *
 */


// Take into account that by default we use a sampling interval of 50Hz
// but you can change that setting by declaring a external var before
// loading the lib in your code, BMUX_SAMPLING is the var

#include "Arduino.h"

#ifndef bmux_h
#define bmux_h

#ifndef BUTTONS_COUNT
    #define BUTTONS_COUNT 8
#endif

#ifndef BMUX_SAMPLING
    #define BMUX_SAMPLING 50    // how many times per second will be sampled
#endif

class Button {
public:
    uint16_t value;         // to hold a max of 1023
    uint16_t duration;      // in msecs, max is 65.5 seconds
    uint8_t counter;        // in sample intervals of BMUX_SAMPLING
    boolean isHeld;         // flag to sign that the button is held

    Button() {};
    Button(uint16_t value, void (*clickFunction)(void), void (*holdFunction)(void) = 0, uint16_t holdDuration = 1000);

    // Override these function if you want
    inline void pressed() {
        (*clickFunction)();
    }

    // Override these function if you want
    inline void held() {
        if (holdFunction) {
            (*holdFunction)();
        }
    }
private:
    void (*clickFunction)(void);
    void (*holdFunction)(void);
};

class BMux {
private:
    uint8_t debounce;       // in sample intervals of BMUX_SAMPLING
    uint32_t time;          // time stamp to define the sampling interval
    uint8_t margin;         // max is 255, which is bigger than we need
    uint8_t pin;            // the analog pin
    uint8_t buttonsCount;
    Button buttons[BUTTONS_COUNT];

public:
    void init(uint8_t pin, uint8_t debounce = 5, uint8_t margin = 10);
    void add(Button button);
    void check();
};

#endif
