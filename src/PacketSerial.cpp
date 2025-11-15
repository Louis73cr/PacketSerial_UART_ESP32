/* **************************************************************************** */
/*                                                                              */
/*                                                  ::::    :::     ::::::::    */
/*   PacketSerial.cpp                               :+:+:   :+:    :+:    :+:   */
/*                                                  :+:+:+  +:+    +:+          */
/*   By: Louis Croci <louis.croci@epitech.eu>       +#+ +:+ +#+    +#++:++#++   */
/*                                                  +#+  +#+#+#           +#+   */
/*   Created: 2025/11/15 15:17:59 by Louis Croci    #+#   #+#+#    #+#    #+#   */
/*   Updated: 2025/11/15 15:17:59 by Louis Croci    ###    ####     ########    */
/*                                                                              */
/* **************************************************************************** */

#include "PacketSerial.h"


PacketSerial::PacketSerial(HardwareSerial &serial) : _serial(serial) {
state = WAIT_START;
checksum_acc = 0;
data_index = 0;
}


void PacketSerial::begin(uint32_t baud) {
_serial.begin(baud);
}


void PacketSerial::sendPacket(uint8_t cmd, uint8_t type, const uint8_t* data, uint8_t len) {
uint8_t checksum = 0;
_serial.write((uint8_t)PACKET_START_BYTE);
_serial.write(cmd);
_serial.write(type);
_serial.write(len);


checksum += cmd;
checksum += type;
checksum += len;


for (uint8_t i = 0; i < len; i++) {
_serial.write(data[i]);
checksum += data[i];
}


_serial.write(checksum);
}


void PacketSerial::sendBool(uint8_t cmd, bool v) {
uint8_t b = v ? 1 : 0;
sendPacket(cmd, TYPE_BOOL, &b, 1);
}


void PacketSerial::sendInt32(uint8_t cmd, int32_t v) {
uint8_t buf[4];
memcpy(buf, &v, 4);
sendPacket(cmd, TYPE_INT32, buf, 4);
}


void PacketSerial::sendFloat(uint8_t cmd, float v) {
uint8_t buf[4];
memcpy(buf, &v, 4);
sendPacket(cmd, TYPE_FLOAT, buf, 4);
}


void PacketSerial::sendString(uint8_t cmd, const char* s) {
}