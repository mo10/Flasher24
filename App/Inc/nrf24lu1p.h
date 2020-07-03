//
// Created by Mo10 on 2020/07/02.
//

#ifndef FLASHER24_NRF24LU1P_H
#define FLASHER24_NRF24LU1P_H

// nRF24LU1+ SPI commands
#include <stdint.h>

#define NRF24LU1P_WREN       0x06
#define NRF24LU1P_WRDIS      0x04
#define NRF24LU1P_RDSR       0x05
#define NRF24LU1P_WRSR       0x01
#define NRF24LU1P_READ       0x03
#define NRF24LU1P_PROGRAM    0x02
#define NRF24LU1P_ERASE_PAGE 0x52
#define NRF24LU1P_ERASE_ALL  0x62
#define NRF24LU1P_RDFPCR     0x89
#define NRF24LU1P_RDISIP     0x84
#define NRF24LU1P_RDISMP     0x85
#define NRF24LU1P_ENDEBUG    0x86

// nRF24LU1+ Flash Status Register bitmask
#define NRF24LU1P_FSR_DBG    0x80
#define NRF24LU1P_FSR_STP    0x40
#define NRF24LU1P_FSR_WEN    0x20
#define NRF24LU1P_FSR_RDYN   0x10
#define NRF24LU1P_FSR_INFEN  0x08
#define NRF24LU1P_FSR_RDISMB 0x04
#define NRF24LU1P_FSR_RDISIP 0x02

void nrf24_init();

// Read a page of memory (512 bytes) and send
// the bytes over serial as hex characters
void nrf24_read_page(uint16_t page);

// Write a page of memory (512 bytes)
void nrf24_write_page(uint16_t page, uint8_t * data);

// Wait for a flash operation to complete
void nrf24_flash_wait();

#endif //FLASHER24_NRF24LU1P_H
