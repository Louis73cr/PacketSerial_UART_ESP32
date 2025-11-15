#include <PacketSerial.h>


PacketSerial pkt(Serial1);

void setup() {
    Serial.begin(115200); // USB debug
    pkt.begin(115200);
    Serial.println("Broadcast ready");
}


void loop() {
    static int32_t counter = 0;
    pkt.sendInt32(10, counter);
    pkt.sendFloat(11, 3.1415);
    pkt.sendBool(12, (counter % 2) == 0);
    pkt.sendString(13, "Hello esp32");

    Serial.println("Sent broadcast packet with counter=" + String(counter));
    counter++;
    delay(1000);
}
