Creator: Griffin Smith

References: None

IDE: All code was written in Ubuntu Linux using Visual Studio Code as a text editor.

Instructions to run code:
Code was developed on Linux Ubuntu. Program is not gaurnteed to run on any other operating system.

Ensure g++ is installed on your linux machine.
If it is not, open your terminal and run the follwing command:
$ sudo apt install g++

Also ensure that the make utility is installed. If not, run:
$ sudo apt install make

Once the above steps are completed, 
open a terminal in the folder containing the source code memAlloc.cpp

In this terminal enter:
$ make

This will compile the source code into an executeable called memAlloc

The useage for this program is:
Useage $ ./memAlloc [-h] -f <first/best> -l <implicit/explicit> -i <input file>

For example:
$ ./memAlloc -f first -l implicit -i examples/1.in

Will run the simulation on the file 1.in with first fit and an implicit list. It will then give the output:

0, 0x00000001
1, 0x00000F98
2, 
3, 
4, 0x00000011
5, 0x00000F88
6, 0x00000018
7, 0x00000021
8, 
9, 
10, 0x00000011
11, 0x00000F88
12, 
13, 
14, 0x00000021
15, 0x00000F60
16, 

...

996, 
997, 
998, 0x00000F98
999, 0x00000001
