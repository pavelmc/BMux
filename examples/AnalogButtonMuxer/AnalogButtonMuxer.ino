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


#include <BMux.h>

#define ANALOG_PIN A2

// First define the callback functions for the buttons action handling,
// in this example just a Serial notification

void b1Click() {
    Serial.print("button 1 clicked");
}

void b2Click() {
    Serial.print("button 2 clicked");
}
void b2Hold() {
    Serial.print("button 2 held");
}

void b3Click() {
    Serial.print("button 3 clicked");
}
void b3Hold() {
    Serial.print("button 3 held");
}

void b4Click() {
    Serial.print("button 4 clicked");
}
void b4Hold() {
    Serial.print("button 4 held");
}

void b5Click() {
    Serial.print("button 5 clicked");
}
void b5Hold() {
    Serial.print("button 5 held");
}

// Creating the AnalogButtons2 instance;
// 5 msec of debounce and 20 units of tolerance
BMux abm;

// Adding the buttons the the instance
Button b1 = Button(1013, &b1Click);
Button b2 = Button(1002, &b2Click, &b2Hold);
Button b3 = Button(970, &b3Click, &b3Hold);
Button b4 = Button(929, &b4Click, &b4Hold);

// Default hold duration is 1 second, lets make it 5 seconds for button5
Button b5 = Button(860, &b5Click, &b5Hold, 5000);

void setup() {
    Serial.begin(9600);
    Serial.println("Testing your Analog Button Muxer Library");

    abm.init(ANALOG_PIN, 5, 20);
    abm.add(b1);
    abm.add(b2);
    abm.add(b3);
    abm.add(b4);
    abm.add(b5);
}

void loop() {
    // To check values when button are pressed
    abm.check();

    // To configure the MAX/Min values for each button: uncomment the following
    // line and attach a serial terminal to see the data and note down the values

    configure();
}

void configure() {
    unsigned int value = analogRead(ANALOG_PIN);
    Serial.println(value);
    delay(250);
}
