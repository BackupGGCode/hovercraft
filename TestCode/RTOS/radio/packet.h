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

enum packetType { INIT=1, PING, PONG, INFO, MOVE, ACK };

typedef 
struct initPacket 
{
    uint8_t init:1;
} initPacket_t;

struct pingPacket
{
    uint8_t ping:1;
};

struct pongPacket
{
    uint8_t pong:1;
};

struct infoPacket 
{
    uint8_t resend:1;
    uint8_t frontSonar;
    uint8_t rightSonar;
    uint8_t leftSonar;
    uint8_t rightMotor;
    uint8_t leftMotor;
};

struct movePacket 
{
    uint8_t rightMotor;
    uint8_t leftMotor;
};

struct ackPacket 
{
    uint8_t ack:1;
};

