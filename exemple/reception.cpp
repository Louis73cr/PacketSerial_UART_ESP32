#include <PacketSerial.h>

PacketSerial pkt(Serial1);

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, 16, 17); // RX=GPIO16, TX=GPIO17
    Serial.println("Receiver ready");
}

void loop() {
    ReceivedPacket rp;

    if (pkt.readPacket(rp)) {
        Serial.print("CMD=0x"); Serial.print(rp.cmd, HEX);
        Serial.print(" TYPE="); Serial.print(rp.type);
        Serial.print(" LEN="); Serial.println(rp.len);

        switch (rp.type) {
            case TYPE_BOOL:
                {
                    bool v = (rp.data[0] != 0);
                    Serial.print("  BOOL = "); Serial.println(v ? "true" : "false");
                }
                break;

            case TYPE_INT32:
                if (rp.len >= 4) {
                    int32_t v;
                    memcpy(&v, rp.data, 4);
                    Serial.print("  INT32 = "); Serial.println(v);
                } else {
                    Serial.println("  ERROR: Invalid INT32 length");
                }
                break;

            case TYPE_FLOAT:
                if (rp.len >= 4) {
                    float f;
                    memcpy(&f, rp.data, 4);
                    Serial.print("  FLOAT = "); Serial.println(f, 6);
                } else {
                    Serial.println("  ERROR: Invalid FLOAT length");
                }
                break;

            case TYPE_STRING:
                {
                    // Protection buffer overflow
                    char str[256];
                    uint8_t copyLen = (rp.len < 255) ? rp.len : 255;
                    memcpy(str, rp.data, copyLen);
                    str[copyLen] = '\0';
                    Serial.print("  STRING = \""); Serial.print(str); Serial.println("\"");
                }
                break;

            default:
                Serial.println("  ERROR: Unknown type");
                break;
        }
        Serial.println();
    }
}