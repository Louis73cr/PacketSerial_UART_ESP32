# PacketSerial - ESP32 UART Communication Library

## Description
PacketSerial is a lightweight library for sending and receiving structured binary packets over UART on ESP32 (Arduino). It provides reliable serial communication between microcontrollers or with a PC using a robust packet format with checksum validation.

## Features
- ✅ Send and receive binary packets with error detection
- ✅ Packet format: `[START=0xAA][CMD][TYPE][LEN][DATA...][CHECKSUM]`
- ✅ Supports multiple data types: boolean, int32, float, string
- ✅ Built-in checksum for data integrity
- ✅ State machine parser for robust reception
- ✅ Easily portable to other Arduino-compatible MCUs

## Installation
1. Copy `PacketSerial.h` and `PacketSerial.cpp` into your Arduino libraries folder:
```
   Arduino/libraries/PacketSerial/
```
2. Restart Arduino IDE
3. Include the library in your sketch:
```cpp
   #include <PacketSerial.h>
```

## Quick Start

### Initialization
```cpp
#include <PacketSerial.h>

HardwareSerial mySerial(1); // Use Serial1 (RX=16, TX=17 on ESP32)
PacketSerial packet(mySerial);

void setup() {
    packet.begin(115200); // Initialize at 115200 baud
}
```

### Sending Data
```cpp
// Send a boolean
packet.sendBool(0x01, true);

// Send an integer
packet.sendInt32(0x02, 12345);

// Send a float
packet.sendFloat(0x03, 3.14159);

// Send a string
packet.sendString(0x04, "Hello ESP32");

// Send raw bytes
uint8_t data[4] = {0x01, 0x02, 0x03, 0x04};
packet.sendPacket(0x10, TYPE_INT32, data, 4);
```

### Receiving Data
```cpp
void loop() {
    ReceivedPacket rp;
    
    if (packet.readPacket(rp)) {
        // Packet received successfully
        Serial.printf("CMD: 0x%02X, Type: %d, Len: %d\n", 
                      rp.cmd, rp.type, rp.len);
        
        // Handle different data types
        switch (rp.type) {
            case TYPE_BOOL:
                bool value = (rp.data[0] != 0);
                break;
                
            case TYPE_INT32:
                int32_t num;
                memcpy(&num, rp.data, 4);
                break;
                
            case TYPE_FLOAT:
                float fval;
                memcpy(&fval, rp.data, 4);
                break;
                
            case TYPE_STRING:
                char str[256];
                memcpy(str, rp.data, rp.len);
                str[rp.len] = '\0'; // Add null terminator
                break;
        }
    }
}
```

## Packet Format

| Byte      | Description                    | Size    |
|-----------|--------------------------------|---------|
| 0xAA      | Start byte (constant)          | 1 byte  |
| CMD       | Command identifier             | 1 byte  |
| TYPE      | Data type (see PacketType)     | 1 byte  |
| LEN       | Length of DATA field           | 1 byte  |
| DATA...   | Payload (0-255 bytes)          | N bytes |
| CHECKSUM  | Sum of CMD+TYPE+LEN+DATA       | 1 byte  |

### Data Types
- `TYPE_INVALID = 0` - Invalid/unused
- `TYPE_BOOL = 1` - Boolean (1 byte)
- `TYPE_INT32 = 2` - 32-bit integer (4 bytes)
- `TYPE_FLOAT = 3` - Float (4 bytes)
- `TYPE_STRING = 4` - String (variable length)

## Methods
- `void begin(uint32_t baud)` - Initialize serial communication
- `void sendPacket(uint8_t cmd, uint8_t type, const uint8_t* data, uint8_t len)` - Send raw packet
- `void sendBool(uint8_t cmd, bool v)` - Send boolean value
- `void sendInt32(uint8_t cmd, int32_t v)` - Send 32-bit integer
- `void sendFloat(uint8_t cmd, float v)` - Send float value
- `void sendString(uint8_t cmd, const char* s)` - Send string
- `bool readPacket(ReceivedPacket &rp)` - Read received packet (returns true if packet available)
- `void flushRx()` - Clear receive buffer

## Example Projects
See `examples/` folder for complete sketches.

## Notes
- Maximum packet size: 260 bytes (5 header bytes + 255 data bytes)
- Checksum is a simple 8-bit sum (not CRC)
- String type does NOT include null terminator - add it manually on reception
- **Important**: `readPacket()` must be called frequently in `loop()` to avoid buffer overflow

## Troubleshooting
- **No data received**: Check TX/RX pin connections and baud rate
- **Invalid checksum**: Verify both devices use same baud rate and packet format
- **Buffer overflow**: Call `readPacket()` more frequently or increase baud rate

## Author
Louis Croci <louis.croci@icloud.com>

## Version
1.0.4 (2025-11-15)

## License
MIT License - Feel free to use in your projects!
