/* **************************************************************************** */
/*                                                                              */
/*                                                  ::::    :::     ::::::::    */
/*   PacketSerial.h                                 :+:+:   :+:    :+:    :+:   */
/*                                                  :+:+:+  +:+    +:+          */
/*   By: Louis Croci <louis.croci@epitech.eu>       +#+ +:+ +#+    +#++:++#++   */
/*                                                  +#+  +#+#+#           +#+   */
/*   Created: 2025/11/15 15:18:05 by Louis Croci    #+#   #+#+#    #+#    #+#   */
/*   Updated: 2025/11/15 15:18:05 by Louis Croci    ###    ####     ########    */
/*                                                                              */
/* **************************************************************************** */
/*
PacketSerial.h / PacketSerial.cpp
Bibliothèque simple pour envoyer/recevoir des paquets binaires sur UART
Format de paquet : [START=0xAA][CMD][TYPE][LEN][DATA...][CHECKSUM]


- Conçue pour ESP32 (Arduino) mais portable sur d'autres MCU.
- Fichiers : PacketSerial.h, PacketSerial.cpp, example.ino
- Usage : placer dans libraries/PacketSerial et inclure <PacketSerial.h>


*/


#ifndef PACKET_SERIAL_H
#define PACKET_SERIAL_H


#include <Arduino.h>


#define PACKET_START_BYTE 0xAA


enum PacketType : uint8_t {
TYPE_INVALID = 0,
TYPE_BOOL = 1,
TYPE_INT32 = 2,
TYPE_FLOAT = 3,
TYPE_STRING = 4
};


struct ReceivedPacket {
uint8_t cmd;
uint8_t type;
uint8_t len;
uint8_t data[255];
};


class PacketSerial {
public:
PacketSerial(HardwareSerial &serial);
void begin(uint32_t baud = 115200);


// Envoi générique
void sendPacket(uint8_t cmd, uint8_t type, const uint8_t* data, uint8_t len);


// Envoi helpers
void sendBool(uint8_t cmd, bool v);
void sendInt32(uint8_t cmd, int32_t v);
void sendFloat(uint8_t cmd, float v);
void sendString(uint8_t cmd, const char* s);


// Réception : retourne true si un paquet valide a été lu et rempli into rp
bool readPacket(ReceivedPacket &rp);


// Optionnel : vider le buffer
void flushRx();


private:
HardwareSerial &_serial;


enum ParseState { WAIT_START, READ_CMD, READ_TYPE, READ_LEN, READ_DATA, READ_CHECKSUM };
ParseState state;
uint8_t checksum_acc;
uint8_t data_index;
ReceivedPacket cur;
};


#endif // PACKET_SERIAL_H