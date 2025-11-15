/**
 * @file PacketSerial.cpp
 * @brief Simple library for sending/receiving structured binary packets over UART (ESP32/Arduino).
 *
 * Packet format: [START=0xAA][CMD][TYPE][LEN][DATA...][CHECKSUM]
 *
 * - Designed for ESP32 (Arduino), portable to other MCUs.
 * - Files: PacketSerial.h, PacketSerial.cpp
 * - Usage: Place in libraries/PacketSerial and include <PacketSerial.h>
 *
 * @author Louis Croci <louis.croci@icloud.com>
 * @date 2025-11-15
 * @version 1.0.0
 */

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
    uint8_t len = strlen(s) + 1;
    sendPacket(cmd, TYPE_STRING, (const uint8_t*)s, len);
}

bool PacketSerial::readPacket(ReceivedPacket &rp) {
    while (_serial.available()) {
        uint8_t byte = _serial.read();

        switch (state) {
            case WAIT_START:
                if (byte == PACKET_START_BYTE) {
                    state = READ_CMD;
                    checksum_acc = 0;
                }
                break;

            case READ_CMD:
                cur.cmd = byte;
                checksum_acc += byte;
                state = READ_TYPE;
                break;

            case READ_TYPE:
                cur.type = byte;
                checksum_acc += byte;
                state = READ_LEN;
                break;

            case READ_LEN:
                cur.len = byte;
                checksum_acc += byte;
                data_index = 0;
                
                if (cur.len > 255) {
                    state = WAIT_START;
                    break;
                }
                
                if (cur.len > 0) {
                    state = READ_DATA;
                } else {
                    state = READ_CHECKSUM;
                }
                break;

            case READ_DATA:
                cur.data[data_index++] = byte;
                checksum_acc += byte;
                if (data_index >= cur.len) {
                    state = READ_CHECKSUM;
                }
                break;

            case READ_CHECKSUM:
                if (checksum_acc == byte) {
                    rp = cur;
                    state = WAIT_START;
                    return true;
                } else {
                    state = WAIT_START;
                }
                break;
        }
    }
    return false;
}

void PacketSerial::flushRx() {
    while (_serial.available()) {
        _serial.read();
    }
}