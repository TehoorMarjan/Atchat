
#include <Arduino.h>
#include <ATCommands.h>
#include <Servo8Bit.h>

#ifndef F
#define F(x) x
#endif

#define WORKING_BUFFER_SIZE 64
#define FIRMWARE_VERSION "1.0.0"

ATCommands AT;
Servo8Bit sprayServo;

uint8_t ledState = LOW; // Initial LED state

// --- AT+VER? ---
bool at_read_cmd_ver(ATCommands *sender) {
    sender->serial->println(FIRMWARE_VERSION);
    return true;
}
bool at_test_cmd_ver(ATCommands *sender) {
    sender->serial->println(F("AT+VER? : Returns firmware version (string)"));
    return true;
}

// --- AT+SPRAY ---
bool at_write_cmd_spray(ATCommands *sender) {
    String param = sender->next();
    int sprayCount = param.toInt();
    // Placeholder: implement spray logic here
    sender->serial->println(F("SPRAY command received (placeholder)"));
    return true;
}
bool at_test_cmd_spray(ATCommands *sender) {
    sender->serial->println(F("AT+SPRAY=<x> : Activates spray for <x> x 500ms (2 = 1s)"));
    return true;
}

// --- AT+LED ---
bool at_test_cmd_led(ATCommands *sender) {
    sender->serial->println(F("AT+LED=<0|1> : 1=ON, 0=OFF (LED_BUILTIN)"));
    return true;
}
bool at_read_cmd_led(ATCommands *sender) {
    sender->serial->print(F("LED State: "));
    sender->serial->println(ledState == HIGH ? "ON" : "OFF");
    return true;
}
bool at_write_cmd_led(ATCommands *sender) {
    String param = sender->next();
    int val = param.toInt();
    if (val < 0 || val > 1) {
        sender->serial->println(F("ERROR: Param must be 0 or 1"));
        return false;
    }
    ledState = (val == 1) ? HIGH : LOW; // Set LED state based on input
    digitalWrite(LED_BUILTIN, ledState);
    return true;
}

// Command table
static at_command_t commands[] = {
    {"+VER", nullptr, at_test_cmd_ver, at_read_cmd_ver, nullptr},
    {"+SPRAY", nullptr, at_test_cmd_spray, nullptr, at_write_cmd_spray},
    {"+LED", nullptr, at_test_cmd_led, at_read_cmd_led, at_write_cmd_led},
};


void setup() {
    Serial.begin(9600);
    sprayServo.attach(0); // Attach to pin 0 (change as needed)
    AT.begin(&Serial, commands, sizeof(commands) / sizeof(commands[0]), WORKING_BUFFER_SIZE);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, ledState); // Initialize LED state
}

void loop() {
    AT.update();
}
