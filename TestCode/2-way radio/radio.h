/**
 * @file   radio.h
 *
 * @brief    QKits TXRX24G transceiver module
 *
 * CSC 460/560 Real Time Operating Systems - Mantis Cheng
 *
 * @author Scott Craig
 * @author Justin Tanner
 *
 * Modifications made by Leanne Ross in collaboration
 * with Mantis Cheng - January 2008
 */

#ifndef __RADIO_H__
#define __RADIO_H__

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

/** We want this driver to be interrupt driven when data arrives. */
#define INTERRUPT_DRIVEN
#define ENABLE_DEFAULT_INTERRUPT_HANDLER

/**
 * Depending on what other things the processor is doing packets may
 * be dropped at the receiver. 500 us is too short. 1 ms works.
 * 2 ms gives more time
 */
#define DELAY_MS_BETWEEN_PACKETS   2

/** Configuration data */
#define CFG_VECT_LEN    15
/** PAYLOAD_BYTES: 28 = (32 bytes - 2 bytes_address - 2 bytes_CRC) */
#define PAYLOAD_BYTES   28
#define RADIO_BUF_LEN   256

/** Configuration values */
#define DATA2_W      0                   // not used
#define DATA1_W      (PAYLOAD_BYTES*8)
#define ADDR2        0,0,0,0,0           // not used
#define ADDR1        0,0,0,0xAC,0xDC     // address can be up to 5 bytes

/** 16-bit addr, 16-bit CRC, CRC_EN */
#define ADDRW_CRC    ((16 << 2) | _BV(1) | _BV(0))

/** RX2_EN=0, CM=1(ShockBurst), RFDR=1(1 Mbps), XOF=011 (16 MHz crystal), RF_PWR=11 (HIGH) */
#define MODES        (_BV(6) | _BV(5) | _BV(3) | _BV(2) | _BV(1) | _BV(0))

/** Choose a channel between 101 and 119
 *@n (or 0 to 82, which conflicts with wireless LAN)
 *@n eg. channel 110 == 2400 MHz + 110 MHz = 2510 MHz
*/
#define DEFAULT_CHANNEL   0
#define RXEN         	  1	// defaulted to receive mode
#define RFCH_RXEN   	  ((DEFAULT_CHANNEL << 1) | RXEN)


/* Port assignments */
#define DR_PORT         PORTE
#define CE_PORT         PORTA
#define CS_PORT         PORTA
#define CLK1_PORT       PORTA
#define DATA_PORT       PORTA
#define DATA_PIN        PINA

/* Port direction */
#define DR_DDR          DDRE
#define CE_DDR          DDRA
#define CS_DDR          DDRA
#define CLK1_DDR        DDRA
#define DATA_DDR        DDRA

/* Pin number assignments */
#define DR_PINNUM       PORTE4
#define CE_PINNUM       PORTA3
#define CS_PINNUM       PORTA2
#define CLK1_PINNUM     PORTA1
#define DATA_PINNUM     PORTA0

/* Address and Channel Definitions */
#define RADIO_CHANNEL   105      // 2400 MHz + 110 MHz = 2510 MHz
#define BLIMP_ADDRESS   0xACDC   // Address for Blimp
#define BASE_ADDRESS    0xDCAC   // Address for Base Station

#define TX_MODE   0   // Transmit Mode
#define RX_MODE   1   // Receive Mode

/* Pin operations */
#define CLK1_HIGH()   CLK1_PORT |= _BV(CLK1_PINNUM)
#define CLK1_LOW()    CLK1_PORT &= ~_BV(CLK1_PINNUM)

#define CE_HIGH()     CE_PORT |= _BV(CE_PINNUM)
#define CE_LOW()      CE_PORT &= ~_BV(CE_PINNUM)

#define CS_HIGH()     CS_PORT |= _BV(CS_PINNUM)
#define CS_LOW()      CS_PORT &= ~_BV(CS_PINNUM)

#define DATA_HIGH()   DATA_PORT |= _BV(DATA_PINNUM)
#define DATA_LOW()    DATA_PORT &= ~_BV(DATA_PINNUM)

/** Buffer to hold message contents. Volatile so accessible outside ISR. */
extern volatile uint8_t radio_buf[PAYLOAD_BYTES];
/** Length of buffer holding radio packet. 1 signifies packet received. */
extern volatile uint8_t packet_available;


/** Routine for initializing the radio. */
int radio_init(uint8_t channel, uint16_t address, uint8_t rx_enable);

/** Routine for sending a radio packet. */
void radio_send(uint16_t const addr, uint8_t * const arr);
/** Routine to switch radio into transmit mode, to send a packet. */
void radio_set_transmit(void);
/** Routine to switch radio into receive mode. */
void radio_set_receive(void);
/** Routine to switch radio into standby mode, turning the radio off. */
void set_standby_mode(void);

#endif
