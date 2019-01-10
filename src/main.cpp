#include <WiFi.h>
#include <Swazm.h>
#include <Contract.h>

#define USE_SERIAL Serial
#define ENV_SSID     "SWAZM"
#define ENV_WIFI_KEY "zomp9neleli"


const string SWAZM_HOST = "rpc.swazm.com";

const uint8_t PRIVATE_KEY[] = {0xe9, 0x61, 0xb6, 0x52, 0x7c, 0x6f, 0xd0, 0x27, 0x33, 0x2a, 0xd7, 0x77, 0x2c, 0x9a, 0x33,
                               0xb2, 0xb4, 0x0b, 0xca, 0x6b, 0xf6, 0x62, 0x6a, 0x3a, 0x75, 0xb0, 0xed, 0x3d, 0xf3, 0x33,
                               0xc9, 0x8e};
string PUBLIC_KEY = "0x0f07616c0213F0EC90E96233FcD8548A145b9986";
string METER_CONTRACT = "0xf6A2c3D46339e1Cb736D5D241fbfB3B0E9B82a6F";

string CONTRACT_CONSUME_FUNCTION = "RegisterConsume(uint,uint,uint,uint)";
string CONTRACT_PRODUCE_FUNCTION = "RegisterProduce(uint,uint,uint,uint)";


Swazm swazm(&SWAZM_HOST);
Contract contract(&swazm, &METER_CONTRACT);

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
    uint32_t nonceVal = (uint32_t) swazm.GetTransactionCount(&PUBLIC_KEY);
    uint32_t gasPriceVal = 2000000000;
    uint32_t gasLimitVal = 3100000;
    string valueStr = "0x01";
    string p = contract.SetupContractData(&CONTRACT_CONSUME_FUNCTION, 1, 2,3,4);
    string result = contract.SendTransaction(nonceVal, gasPriceVal, gasLimitVal, &METER_CONTRACT, &valueStr, &p);

    USE_SERIAL.println(result.c_str());
    delay(10000);
}

void example() {
    char tmp[32];

    double version = swazm.ProtocolVersion();
    USE_SERIAL.println("-[Protocol Version]");
    USE_SERIAL.println(version);

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

    // transaction
    contract.SetPrivateKey(PRIVATE_KEY);


}
