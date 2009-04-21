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

struct pingPacket
{
    packet_t type;
    uint8_t ping:1;
};

typedef
struct pongPacket
{
    packet_t type;
    uint8_t pong:1;
} pongPacket_t;

struct infoPacket 
{
    packet_t type;
    uint8_t resend:1;
    uint8_t frontSonar;
    uint8_t rightSonar;
    uint8_t leftSonar;
    uint8_t rightMotor;
    uint8_t leftMotor;
};

typedef
struct movePacket 
{
    packet_t type;
    uint8_t rightMotor;
    uint8_t leftMotor;
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
