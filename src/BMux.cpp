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

// by default we a sampling interval of 50Hz

#include "BMux.h"

Button::Button(uint16_t value, void (*clickFunction)(void), void (*holdFunction)(void), uint16_t holdDuration) {
    this->value = value;
    this->duration = holdDuration;
    this->clickFunction = clickFunction;
    this->holdFunction = holdFunction;
    this->counter = 0;
    this->isHeld = false;
}

void BMux::init(uint8_t pin, uint8_t debounce, uint8_t margin) {
    this->pin = pin;
    this->debounce = debounce;
    this->margin = margin;
}

void BMux::add(Button button) {
    if (buttonsCount < BUTTONS_COUNT) {
        buttons[buttonsCount++] = button;
    }
}

void BMux::check() {
    // In case this function gets called very frequently avoid sampling the
    // analog pin too often: default (max) frequency is 50Hz
    if (millis() - time > 1000 / BMUX_SAMPLING) {
        time = millis();
        uint16_t reading = analogRead(pin);
        uint16_t msec = 0;
        uint16_t min = 0;
        uint16_t max = 0;

        // we changed the strategy here, all events will be emitted on button release
        for (uint8_t i = 0; i < buttonsCount; i++) {
            // low limit checking (this led to overflows in the past)
            if (buttons[i].value <= margin) {
                min = 0;
            } else {
                min = buttons[i].value - margin;
            }

            // max limit check
            max = buttons[i].value + margin;
            if (max > 1023) max = 1023;

            // checking
            if (reading >= min && reading <= max) {
                // just increment the count of the pressed buttons
                buttons[i].counter += 1;

                // check for button held & still pressed
                msec = buttons[i].counter;
                msec *= 1000 / BMUX_SAMPLING;
                if (msec >= buttons[i].duration) {
                    buttons[i].held();
                    buttons[i].counter = 0;
                    buttons[i].isHeld = true;
                }
            } else {
                // check for overshoots after a held
                if (buttons[i].isHeld) {
                    // discard, it's a overshoot or noise
                    buttons[i].isHeld = false;
                    buttons[i].counter = 0;
                    return;
                }

                if (buttons[i].counter > 0) {
                    if (buttons[i].counter >= debounce) {
                        buttons[i].pressed();
                    }

                    buttons[i].counter = 0;
                }
            }
        }
    }
}
