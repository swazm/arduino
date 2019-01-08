#include "Swazm.h"
#include <WiFiClientSecure.h>
#include "CaCert.h"
#include "Log.h"
#include "Util.h"
#include "../cJSON/cJSON.h"
#include <iostream>
#include <sstream>

WiFiClientSecure client;
Log debug;
#define LOG(x) void(x)

Swazm::Swazm(const string* _host) {
    client.setCACert(swazm_ca_cert);
    host = _host;
}

string Swazm::SwazmClientVersion() {
    string m = "web3_clientVersion";
    string p = "[]";
    string input = generateJson(&m, &p);
    string output = exec(&input);
    return getString(&output);
}

string Swazm::SwazmSha3(const string* data) {
    string m = "web3_sha3";
    string p = "[\"" + *data + "\"]";
    string input = generateJson(&m, &p);
    string output = exec(&input);
    return getString(&output);
}

int Swazm::NetVersion() {
    string m = "net_version";
    string p = "[]";
    string input = generateJson(&m, &p);
    string output = exec(&input);
    return getInt(&output);
}

bool Swazm::NetListening() {
    string m = "net_listening";
    string p = "[]";
    string input = generateJson(&m, &p);
    string output = exec(&input);
    return getBool(&output);
}

int Swazm::NetPeerCount() {
    string m = "net_peerCount";
    string p = "[]";
    string input = generateJson(&m, &p);
    string output = exec(&input);
    return getInt(&output);
}

double Swazm::ProtocolVersion() {
    string m = "eth_protocolVersion";
    string p = "[]";
    string input = generateJson(&m, &p);
    string output = exec(&input);
    return getDouble(&output);
}

bool Swazm::Syncing() {
    string m = "eth_syncing";
    string p = "[]";
    string input = generateJson(&m, &p);
    string result = exec(&input);

    cJSON *root, *value;
    root = cJSON_Parse(result.c_str());
    value = cJSON_GetObjectItem(root, "result");
    bool ret;
    if (cJSON_IsBool(value)) {
        ret = false;
    } else{
        ret = true;
    }
    cJSON_free(root);
    return ret;
}

bool Swazm::Mining() {
    string m = "eth_mining";
    string p = "[]";
    string input = generateJson(&m, &p);
    string output = exec(&input);
    return getBool(&output);
}

double Swazm::HashRate() {
    string m = "eth_hashrate";
    string p = "[]";
    string input = generateJson(&m, &p);
    string output = exec(&input);
    return getDouble(&output);
}

long long int Swazm::GasPrice() {
    string m = "eth_gasPrice";
    string p = "[]";
    string input = generateJson(&m, &p);
    string output = exec(&input);
    return getLongLong(&output);
}

void Swazm::Accounts(char** array, int size) {
     // TODO
}

int Swazm::BlockNumber() {
    string m = "eth_blockNumber";
    string p = "[]";
    string input = generateJson(&m, &p);
    string output = exec(&input);
    return getInt(&output);
}

long long int Swazm::GetBalance(const string* address) {
    string m = "eth_getBalance";
    string p = "[\"" + *address + "\",\"latest\"]";
    string input = generateJson(&m, &p);
    string output = exec(&input);
    return getLongLong(&output);
}

int Swazm::GetTransactionCount(const string* address) {
    string m = "eth_getTransactionCount";
    string p = "[\"" + *address + "\",\"latest\"]";
    string input = generateJson(&m, &p);
    string output = exec(&input);
    return getInt(&output);
}

string Swazm::Call(const string* from, const string* to, long gas, long gasPrice,
                     const string* value, const string* data) {
    // TODO use gas, gasprice and value
    string m = "eth_call";
    string p = "[{\"from\":\"" + *from + "\",\"to\":\""
               + *to + "\",\"data\":\"" + *data + "\"}, \"latest\"]";
    string input = generateJson(&m, &p);
    return exec(&input);
}

string Swazm::SendSignedTransaction(const string* data, const uint32_t dataLen) {
    string m = "eth_sendRawTransaction";
    string p = "[\"" + *data + "\"]";
    string input = generateJson(&m, &p);
#if 0
    LOG(input);
#endif
    return exec(&input);
}

// -------------------------------
// Private

string Swazm::generateJson(const string* method, const string* params) {
    return "{\"jsonrpc\":\"2.0\",\"method\":\"" + *method + "\",\"params\":" + *params + ",\"id\":0}";
}

string Swazm::exec(const string* data) {
    string result;

    // start connection
    LOG("\nStarting connection to server...");
    int connected = client.connect(host->c_str(), 443);
    if (!connected) {
        return "";
    }

    LOG("Connected to server!");
    // Make a HTTP request:
    int l = data->size();
    stringstream ss;
    ss << l;
    string lstr = ss.str();

    string strPost = "POST / HTTP/1.1";
    string strHost = "Host: " + *host;
    string strContentLen = "Content-Length: " + lstr;

    client.println(strPost.c_str());
    client.println(strHost.c_str());
    client.println("Content-Type: application/json");
    client.println(strContentLen.c_str());
    client.println("Connection: close");
    client.println();
    client.println(data->c_str());

    while (client.connected()) {
        String line = client.readStringUntil('\n');
        LOG(line.c_str());
        if (line == "\r") {
            break;
        }
    }
    // if there are incoming bytes available
    // from the server, read them and print them:
    while (client.available()) {
        char c = client.read();
        result += c;
    }
    LOG(result.c_str());

    client.stop();

    return result;
}

int Swazm::getInt(const string* json) {
    int ret = -1;
    cJSON *root, *value;
    root = cJSON_Parse(json->c_str());
    value = cJSON_GetObjectItem(root, "result");
    if (cJSON_IsString(value)) {
        ret = strtol(value->valuestring, nullptr, 16);
    }
    cJSON_free(root);
    return ret;
}

long Swazm::getLong(const string* json) {
    long ret = -1;
    cJSON *root, *value;
    root = cJSON_Parse(json->c_str());
    value = cJSON_GetObjectItem(root, "result");
    if (cJSON_IsString(value)) {
        ret = strtol(value->valuestring, nullptr, 16);
    }
    cJSON_free(root);
    return ret;
}

long long int Swazm::getLongLong(const string* json) {
    long long int ret = -1;
    cJSON *root, *value;
    root = cJSON_Parse(json->c_str());
    value = cJSON_GetObjectItem(root, "result");
    if (cJSON_IsString(value)) {
        ret = strtoll(value->valuestring, nullptr, 16);
    }
    cJSON_free(root);
    return ret;
}

double Swazm::getDouble(const string* json) {
    double ret = -1;
    cJSON *root, *value;
    root = cJSON_Parse(json->c_str());
    value = cJSON_GetObjectItem(root, "result");
    if (cJSON_IsString(value)) {
        LOG(value->valuestring);
        ret = strtof(value->valuestring, nullptr);
    }
    cJSON_free(root);
    return ret;
}

bool Swazm::getBool(const string* json) {
    bool ret = false;
    cJSON *root, *value;
    root = cJSON_Parse(json->c_str());
    value = cJSON_GetObjectItem(root, "result");
    if (cJSON_IsBool(value)) {
        ret = (bool)value->valueint;
    }
    cJSON_free(root);
    return ret;
}

string Swazm::getString(const string* json) {
    cJSON *root, *value;
    root = cJSON_Parse(json->c_str());
    value = cJSON_GetObjectItem(root, "result");
    if (cJSON_IsString(value)) {
        cJSON_free(root);
        return string(value->valuestring);
    }
    cJSON_free(root);
    return nullptr;
}
