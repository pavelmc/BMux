# Analog Button Muxer (BMux) #

An Analog Buttons handling library for Arduino with improvements from many sources, with the normal behavior you expect for the held & click actions.

## Feaures ##

Acctually we have the following features implemented:

* Up to 8 buttons in a single Analog pin _(by default limited to 8 to keep firmware footprint low, it's tunable in software)_.
* Secondary functions (press & hold) in each button, configured on demand.
* Intuitive behavior in the case you held a button pressed (_on the original AnalogButtons library when you held a button you got two events, first a click and then a hold one, that's not acceptable for some uses._)

## Inspiration ##

This library is based in the following source codes:

* rlogiacco work: [AnalogButtons](https://github.com/rlogiacco/AnalogButtons) _(This lib is in fact is a derived work from this code)_
* My own work with [AnaButtons](https://github.com/pavelmc/AnaButtons) **Which is deprecated in favor of this lib**.

## How it works? ##

The first thing to do is determine how many buttons you will manage in the analog pin. By default max number of buttons per pin is limited to 8 to limit memory consumption (you can declare less or more buttons as your need commands), it can be controlled defining the `BUTTONS_COUNT` macro **before** including this library.

You then create an instance of the lib to be configured and user later in our code; like this:

```
BMux abm;
```

The library needs a group of callback functions for each of the click or hold event for each of the buttons you will use. Then you have to instantiate the buttons associating the callback functions and set at least a value and a click function to it. This value is the mean analog value of the ADC for the particular resistor combination you use, it must be distinct and far away from the other buttons resistors combination.

On the simplest form you will have a function named btnClick() and an analog ADC value of 512:

```
void btnClick() {
  // do something
}

Button aButton = Button(512, &btnClick);
```

On the complex for you will have all this parameters to choose from:

* an associated `value` (required) an unsigned 10 bits integer in the [0-1023] range matching the voltage drop for that particular resistor combination.
* a `click function` (required) executed upon button click.
* a `hold function` (optional) executed once the button is identified as **being held** (it will do nothing if not declared)
* an `hold duration` (optional) determining the number of milliseconds the button must remain pressed before being identified as held down (defaults to 1 second, max 65534 msecs or ~65.5 seconds), this is the interval at which the held event will repeat it self if the button is held longer.

An example of a complex button definition with 5000 msecs (5 seconds) of held detection and repeat.

```
void btnClick() {
  // do something
}

void btnHold() {
  // do something else
}

Button aButton = Button(512, &btnClick, &btnHold, 5000);
```

Then you need to configure your instance in the setup() part of your code, and for that you have the following options:

* the `analog pin` the buttons will be attached to (required)
* the `debounce interval` which determines the minimum duration a button must remain pressed to be considered being **clicked** in order to avoid false positives and ADC noise (expressed in terms of samples at 50Hz, defaults to 5 samples aka ~100 msecs, see the note and example in the library folder)
* the `analog value margin` which takes into account slight resistance fluctuations and ADC errors transforming the button *value* into a range (expressed in units of ADC error, defaults to 10)

For example in it's simpler way it's just this. (assuming all defaults, and using the analog A2 input)

```
abm.init(A2);
```

A more complex definition can be this, with a tight debounce interval of 2 and a wide margin of 20 ADC units for super fast trigger but with possible false positives.

```
abm.init(A2, 2, 20);
```

**Note:** _The library has a sampling limit to avoid to use the ADC in an intensive way, the default sampling is 50 Hz (50 samples per second; aka 20 msecs 1/50 = 0.02 = 20 msecs) So a `debounce interval` of 5 is the same to say that a click will be detected after 100 msecs of being held_

Now it's time to links the buttons with the Analog interface, is as simple as this (still in the setup() part of your code):

```
abm.add(aButton);
abm.add(anotherButton);
```

**Tip:** remember that you have a default of 8 button maximum per analog pin, but you can use less or more that just read the paragraph after the "How it Works?" title.

It's time to put it to work, just include this line in your loop() and you are done:

```
abm.check();
```

## Author & contributors ##

The only author is Pavel Milanes, CO7WT, reachable at pavelmc@gmail.com, Until now I have no contributors or sponsors.

## Where to download the latest version? ##

Always download the latest version from the [github repository](https://github.com/pavelmc/BMux/)

## If you like to give thanks... ##

Please take a look at the Donations and Funding file to keep me creating/updating/sharing my work with you.
