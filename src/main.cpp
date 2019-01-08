#include <WiFi.h>
#include <Swazm.h>

#define USE_SERIAL Serial

#define ENV_SSID     "YOUR_SSID"
#define ENV_WIFI_KEY "YOUR_PASSWORD"
const string SWAZM_HOST = "rpc.swazm.com";


Swazm swazm(&SWAZM_HOST);

void example();

void setup() {
    USE_SERIAL.begin(9600);

    for (uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFi.begin(ENV_SSID, ENV_WIFI_KEY);

    // attempt to connect to Wifi network:
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        // wait 1 second for re-trying
        delay(1000);
    }

    USE_SERIAL.println("Connected");

    example();
}

void loop() {
    // put your main code here, to run repeatedly:
}

void example() {
    char tmp[32];

    double version = swazm.ProtocolVersion();
    USE_SERIAL.println("-[Protocol Version]");
    USE_SERIAL.println(version);

    bool listening = swazm.Syncing();

    long long int gasPrice = swazm.GasPrice();
    USE_SERIAL.println("-[Compute Price]");
    memset(tmp, 0, 32);
    sprintf(tmp, "%lld", gasPrice);
    USE_SERIAL.println(tmp);

    int blockNumber = swazm.BlockNumber();
    USE_SERIAL.println("-[Block Number]");
    memset(tmp, 0, 32);
    sprintf(tmp, "%d", blockNumber);
    USE_SERIAL.println(tmp);

    string address = "0x0f07616c0213F0EC90E96233FcD8548A145b9986";
    long long int balance = swazm.GetBalance(&address);
    USE_SERIAL.println("-[Balance]");
    memset(tmp, 0, 32);
    sprintf(tmp, "%lld", balance);
    USE_SERIAL.println(tmp);

    int txcount = swazm.GetTransactionCount(&address);
    USE_SERIAL.println("-[Transaction Count]");
    memset(tmp, 0, 32);
    sprintf(tmp, "%d", txcount);
    USE_SERIAL.println(tmp);
}
