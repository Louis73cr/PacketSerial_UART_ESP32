
/**
 * @file PacketSerial.h
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

#ifndef PACKET_SERIAL_H
#define PACKET_SERIAL_H


#include <Arduino.h>

/**
 * @def PACKET_START_BYTE
 * @brief Start byte indicating the beginning of a packet.
 */
#define PACKET_START_BYTE 0xAA

/**
 * @enum PacketType
 * @brief Types of data that can be sent in a packet.
 */
enum PacketType : uint8_t {
    TYPE_INVALID = 0,
    TYPE_BOOL = 1,
    TYPE_INT32 = 2,
    TYPE_FLOAT = 3,
    TYPE_STRING = 4
};

/**
 * @struct ReceivedPacket
 * @brief Structure to hold a received packet's data.
 */
struct ReceivedPacket {
    uint8_t cmd;
    uint8_t type;
    uint8_t len;
    uint8_t data[255];
};

/**
 * @class PacketSerial
 * @brief Class to handle sending and receiving structured packets over UART.
 */
class PacketSerial {
    public:
        /**
         * @brief Construct a new Packet Serial object.
         * @param serial Reference to the HardwareSerial instance to use.
         */
        PacketSerial(HardwareSerial &serial);

        /**
         * @brief Initialize the serial communication.
         * @param baud Baud rate for serial communication.
         */
        void begin(uint32_t baud = 115200);

        /**
         * @brief Send a packet with specified command, type, data, and length.
         * @param cmd Command identifier.
         * @param type Data type.
         * @param data Pointer to the data to send.
         * @param len Length of the data.
         */
        void sendPacket(uint8_t cmd, uint8_t type, const uint8_t* data, uint8_t len);

        /**
         * @brief Send a boolean value with specified command.
         * @param cmd Command identifier.
         * @param v Boolean value to send.
         */
        void sendBool(uint8_t cmd, bool v);

        /**
         * @brief Send a 32-bit integer with specified command.
         * @param cmd Command identifier.
         * @param v 32-bit integer value to send.
         */
        void sendInt32(uint8_t cmd, int32_t v);

        /**
         * @brief Send a float value with specified command.
         * @param cmd Command identifier.
         * @param v Float value to send.
         */
        void sendFloat(uint8_t cmd, float v);

        /**
         * @brief Send a string with specified command.
         * @param cmd Command identifier.
         * @param s Null-terminated string to send.
         */
        void sendString(uint8_t cmd, const char* s);

        /**
         * @brief Read a received packet if available.
         * @param rp Reference to a ReceivedPacket structure to fill with received data.
         * @return true if a packet was successfully read, false otherwise.
         */
        bool readPacket(ReceivedPacket &rp);


        /**
         * @brief Flush the receive buffer.
         */
        void flushRx();


    private:
        /**
         * @brief Reference to the HardwareSerial instance used for communication.
         */
        HardwareSerial &_serial;

        /**
         * @brief Internal state for parsing incoming packets.
         */
        enum ParseState { WAIT_START, READ_CMD, READ_TYPE, READ_LEN, READ_DATA, READ_CHECKSUM };
        
        /**
         * @brief Current state of the packet parser.
         */
        ParseState state;
        
        /**
         * @brief Accumulated checksum for the current packet being received.
         */
        uint8_t checksum_acc;

        /**
         * @brief Current index in the data array while receiving a packet.
         */
        uint8_t data_index;

        /**
         * @brief Current received packet being constructed.
         */
        ReceivedPacket cur;
};


#endif // PACKET_SERIAL_H