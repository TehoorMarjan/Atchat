#include <Arduino.h>
#include <DigiCDC.h>
#include <Servo_ATTinyCore.h>

#define FIRMWARE_VERSION "0.2.1"
#define CMD_BUFFER_SIZE 32

Servo servo;
char cmdBuffer[CMD_BUFFER_SIZE];
uint8_t cmdIndex = 0;

void processATCommand() {
    cmdBuffer[cmdIndex] = '\0'; // Null terminate
    
    if (strcmp(cmdBuffer, "AT+VER?") == 0) {
        SerialUSB.println(FIRMWARE_VERSION);
    }
    else if (strncmp(cmdBuffer, "AT+S=", 5) == 0) {
        // Extract angle from AT+S=<angle>
        int angle = atoi(&cmdBuffer[5]);
        if (angle >= 0 && angle <= 180) {
            servo.write(angle);
            SerialUSB.print("OK S=");
            SerialUSB.println(angle);
        } else {
            SerialUSB.println("ERROR: Angle must be 0-180");
        }
    }
    else if (strcmp(cmdBuffer, "AT") == 0) {
        SerialUSB.println("OK");
    }
    else {
        SerialUSB.println("ERROR");
    }
    
    cmdIndex = 0; // Reset buffer
}

void setup() {
    SerialUSB.begin();
    servo.attach(0);
    servo.write(0); // Center servo
    cmdIndex = 0;
}

void loop() {
    if (SerialUSB.available()) {
        char input = SerialUSB.read();
        
        if (input == '\r' || input == '\n') {
            if (cmdIndex > 0) {
                processATCommand();
            }
        }
        else if (cmdIndex < CMD_BUFFER_SIZE - 1) {
            cmdBuffer[cmdIndex++] = input;
        }
        else {
            // Buffer overflow, reset
            cmdIndex = 0;
            SerialUSB.println("ERROR: Command too long");
        }
    }
    
    SerialUSB.delay(10); // keep USB alive
}
