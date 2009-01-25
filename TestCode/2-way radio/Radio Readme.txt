2-way Radio Readme
=============

Author: Leanne Ross
Date: 4 February 2008


To use the radio you need 3 files: radio.c, radio.h, and common.h.

Common.h holds the structure definition of the payload contents.
The first part of the struct is the type of message sent.
The second part consists of:
1. data received when sampling the joystick (control struct)
2. data received from the blimp (status struct)
3. message contents for debugging

(Note: You are allowed to modify the contents and layout of the payload union struct in any way that suits your project, as long as the size of "packet" type is exactly 28 bytes. If this assumption is violated, the radio code will not work. We check for this condition at the beginning of the "base.c" code.)

Radio.h contains all the variables, definitions, and public function declarations with respect to the radio.

Radio.c contains all the functions and the Interrupt Service Routine for the radio.


ASSUMPTIONS:
=========

1. The projects must be compiled with optimization '-O2' otherwise the delay library functions will not work properly.

2. The blimp code/project is saved in the same directory as radio.c, radio.h, and common.h. The base code/project, uart.c and uart.h are saved in a directory inside the folder that holds the blimp code. If you have everything in one directory, you will need to change the #includes in base.c.

3. PAYLOAD_BYTES must equal 28 bytes long. No more, no less.

4. Each PAYLOAD/message sent is exactly 28 bytes long.

5. One packet is sent and dealt with at one time. The radio is put in standby mode (off) while the packet is being processed. By placing the radio in standby mode, we are preventing a race condition from occuring.

6. The base station acts as the controller. It decides when to send a message to the blimp (eg. changes from the joystick). Following the initial ready message, the blimp only sends a message to the base station after it first receives a message from the base station (eg. status report).

7. The radio is initially placed in receive mode for both the blimp and base station. Thereafter, the radio is placed in transmit mode immediately prior to sending a message (radio_send() does this) and into receive mode only when it is ready to receive a packet (ie. in the listen state). At all other times, the radio should be in standby mode (off). The ISR places the radio in standby mode upon exiting. This allows the radio packet to be processed without another INT4 interrupt occurring, overwriting the first message.


FINITE STATE MACHINE of BLIMP
====================

Blimp starts in the INIT state. Here the blimp sends 3 READY messages to the base station. After 100 ms, the blimp moves into the LISTEN state. In the LISTEN state, the blimp is in receive mode, waiting for a message from the base. If the blimp receives a CONTROL message, the radio is placed in standby mode and the blimp deals with the message (eg. setting PWM) and returns a STATUS message to the base station (eg. current height of the blimp). After the STATUS message is sent, the blimp returns to simply listening for another message. If any other message type is received from the base station, the blimp returns a READY message.


FINITE STATE MACHING of BASE STATION
=========================
Base starts in the INIT state. In INIT, it waits for a READY message from the blimp. As soon as it receives a READY message, the base station moves into the LISTEN state where a timer is set and the base then waits for the timeout before moving to the CONTROLLER state. In the CONTROLLER state, the base samples the joystick and sends a CONTROL message to the blimp when a change has occurred. From here, the base enters the LISTEN mode (timer is once again started), waiting for either a return message or a timeout to occur. If a STATUS message is received, the base prints the data via UART and again enters the CONTROLLER state. If a timeout occurs, the base enters the CONTROLLER state (ie. perhaps messages were lost). By having a timeout, we are eliminating the possibility of a deadlock occurring. If the base receives any other message, the message is printed and the base returns to the LISTEN state.


ADAPTATIONS:
=========
You could have the blimp as the controller and the base station in receive mode. In this setup, you would be able to better use the various types of messages.
