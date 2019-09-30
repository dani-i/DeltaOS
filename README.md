# DeltaOS

## What is this?

 A naive IA-32e Operating System (my first OS dev project :D), originally [hosted](https://gitlab.com/i_daniel/DeltaOS) in a GitLab repo, decided to move it here since the class for which it was done has finished.

## Features:

*  Multiboot (grub)
*  CPU checks
*  Enabling A20 line
*  4 level paging
*  Enabling Long Mode
*  Setting GDT64
*  Entering Long Mode
*  Routines for handling interrupts and exceptions
*  PIC programming
*  Displaying the trap frame
*  Timer
*  Keyboard

## Screen captures of the OS in various states

| Moment of the screen capture | Screen capture |
| ------ | ------ |
| Successful run upon jumping into Long Mode | ![Upon the Long Mode jump.](https://github.com/dani-i/DeltaOS/blob/master/prt0.png?raw=true) |
| Successful OS load and setup | ![Running.](https://github.com/dani-i/DeltaOS/blob/master/prt.png?raw=true) |
| Example of a trap frame dump | ![Running.](https://github.com/dani-i/DeltaOS/blob/master/prt1.png?raw=true) |

