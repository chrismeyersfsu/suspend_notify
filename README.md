suspend_notify
==============

Do you have a laptop, run linux, and love pop-tarts ?!

If so, this repo is for you!

Play a sound when your laptop suspends or wakes up. Doesn't fix the root of the problem, but alerts you when your pop-tarts are done.

* Place poptart atop your keyboard
* Close your laptop
* Wait for Linux to randomly wakeup, you will be alerted by the sound of your choice.
* Enjoy your pop-tart

**tested on dell xps 15 w/ ubuntu 14.04**

##Install
Disregard the C code in this repo. Just copy `etc/pm/sleep.d/01_playsound` to `/etc/pm/sleep.d/01_playsound` and edit the path to the sounds you wish to play.
