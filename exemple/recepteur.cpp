/* **************************************************************************** */
/*                                                                              */
/*                                                  ::::    :::     ::::::::    */
/*   recepteur.cpp                                  :+:+:   :+:    :+:    :+:   */
/*                                                  :+:+:+  +:+    +:+          */
/*   By: Louis Croci <louis.croci@epitech.eu>       +#+ +:+ +#+    +#++:++#++   */
/*                                                  +#+  +#+#+#           +#+   */
/*   Created: 2025/11/15 15:35:10 by Louis Croci    #+#   #+#+#    #+#    #+#   */
/*   Updated: 2025/11/15 15:35:10 by Louis Croci    ###    ####     ########    */
/*                                                                              */
/* **************************************************************************** */

#include <PacketSerial.h>


PacketSerial pkt(Serial1); // on utilise Serial1 pour la liaison UART entre cartes

void setup() {
    Serial.begin(115200); // debug USB
    pkt.begin(115200);
    Serial.println("Maître prêt");
}


void loop() {
    static int32_t counter = 0;
    pkt.sendInt32(10, counter);
    pkt.sendFloat(11, 3.1415);
    pkt.sendBool(12, (counter % 2) == 0);
    pkt.sendString(13, "Salut esp32");

    Serial.println("Paquets envoyés");
    counter++;
    delay(1000);
}
