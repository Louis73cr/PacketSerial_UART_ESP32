# PacketSerial_UART_ESP32

## Description
PacketSerial_UART_ESP32 est une bibliothèque simple pour envoyer et recevoir des paquets binaires structurés via UART sur ESP32 (Arduino). Elle facilite la communication série entre microcontrôleurs ou avec un PC, en utilisant un format de paquet robuste.

## Fonctionnalités
- Envoi et réception de paquets binaires
- Format de paquet : [START=0xAA][CMD][TYPE][LEN][DATA...][CHECKSUM]
- Support des types : booléen, int32, float
- Détection d'erreurs via checksum
- Facilement portable sur d'autres MCU

## Installation
1. Copier les fichiers `PacketSerial.h` et `PacketSerial.cpp` dans le dossier `libraries/PacketSerial` de votre projet Arduino.
2. Inclure la bibliothèque dans votre code :
   ```cpp
   #include <PacketSerial.h>
   ```

## Utilisation
Initialisation :
```cpp
HardwareSerial serial(1);
PacketSerial packet(serial);
packet.begin(115200);
```

Envoi d'un paquet :
```cpp
uint8_t data[4] = {0x01, 0x02, 0x03, 0x04};
packet.sendPacket(0x10, TYPE_INT32, data, 4);
```

Réception (dans loop) :
```cpp
packet.update();
if (packet.available()) {
	// Traiter le paquet reçu
}
```

## Format du paquet
| Octet      | Description         |
|------------|--------------------|
| 0xAA       | Start byte         |
| CMD        | Commande           |
| TYPE       | Type de données    |
| LEN        | Longueur DATA      |
| DATA...    | Données            |
| CHECKSUM   | Somme de contrôle  |

## Auteur
- Louis Croci <louis.croci@icloud.com>

## Licence
MIT