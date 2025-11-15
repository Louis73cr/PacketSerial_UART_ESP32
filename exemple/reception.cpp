#include <PacketSerial.h>


PacketSerial pkt(Serial1);


void setup() {
    Serial.begin(115200);
    pkt.begin(115200);
    Serial.println("Receiver ready");
}


void loop() {
    ReceivedPacket rp;

    if (pkt.readPacket(rp)) {
        Serial.print("CMD="); Serial.print(rp.cmd);
        Serial.print(" TYPE="); Serial.print(rp.type);
        Serial.print(" LEN="); Serial.println(rp.len);


        if (rp.type == TYPE_BOOL) {
            bool v = rp.data[0] != 0;
            Serial.print("BOOL= "); Serial.println(v);
        } else if (rp.type == TYPE_INT32 && rp.len >= 4) {
            int32_t v; memcpy(&v, rp.data, 4);
            Serial.print("INT32= "); Serial.println(v);
        } else if (rp.type == TYPE_FLOAT && rp.len >= 4) {
            float f; memcpy(&f, rp.data, 4);
            Serial.print("FLOAT= "); Serial.println(f);
        } else if (rp.type == TYPE_STRING) {
            rp.data[rp.len] = 0; // terminate
            Serial.print("STR= "); Serial.println((char*)rp.data);
        }
    }
}
