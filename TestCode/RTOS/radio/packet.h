/*
 * =====================================================================================
 *
 *       Filename:  packet.h
 *
 *    Description:  Radio Packet Structures.
 *
 *         Author:  Marco Yuen <marcoy@gmail.com>
 *
 * =====================================================================================
 */

#include "../motor/DualMotors.h"

typedef enum { INIT=1, PING, PONG, INFO, MOVE, ACK } packet_t;

typedef
struct genericPacket
{
    packet_t type;
} genericPacket_t;

typedef 
struct initPacket 
{
    packet_t type;
    uint8_t init:1;
} initPacket_t;

typedef
struct pingPacket
{
    packet_t type;
    uint8_t ping:1;
} pingPacket_t;

typedef
struct pongPacket
{
    packet_t type;
    uint8_t pong:1;
} pongPacket_t;

typedef
struct infoPacket 
{
    packet_t type;
    uint8_t resend:1;
    uint8_t frontSonar;
    uint8_t rightSonar;
    uint8_t leftSonar;
    uint8_t rightMotor;
    uint8_t leftMotor;
} infoPacket_t;

typedef
struct movePacket 
{
    packet_t type;
    uint8_t rightMotor;
    uint8_t leftMotor;
    direction_t rightDirection;
    direction_t leftDirection;
} movePacket_t;

typedef
struct ackPacket 
{
    packet_t type;
    uint8_t ack:1;
} ackPacket_t;

struct resendPacket 
{
    packet_t type;
};
