#include <PacketSerial.h>

HardwareSerial mySerial(1);
PacketSerial pkt(mySerial);

void setup() {
    Serial.begin(115200);
    mySerial.begin(115200, SERIAL_8N1, 16, 17);
    Serial.println("Broadcast ready");
}

void loop() {
    static int32_t counter = 0;
    static unsigned long lastSend = 0;
    
    if (millis() - lastSend >= 1000) {
        pkt.sendInt32(10, counter);
        pkt.sendFloat(11, 3.1415);
        pkt.sendBool(12, (counter % 2) == 0);
        pkt.sendString(13, "Hello esp32");
        
        Serial.println("Sent broadcast #" + String(counter));
        counter++;
        lastSend = millis();
    }
    
    ReceivedPacket rp;
    if (pkt.readPacket(rp)) {
        Serial.printf("Received CMD=0x%02X\n", rp.cmd);
    }
}