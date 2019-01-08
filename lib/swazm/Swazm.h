#ifndef ESPIDF_SWAZM_H
#define ESPIDF_SWAZM_H

#include "stdint.h"
#include <string>

using namespace std;

class Swazm {
public:
    Swazm(const string* _host);
    string SwazmClientVersion();
    string SwazmSha3(const string* data);
    int NetVersion();
    bool NetListening();
    int NetPeerCount();
    double ProtocolVersion();
    bool Syncing();
    bool Mining();
    double HashRate();
    long long int GasPrice();
    void Accounts(char **array, int size);
    int BlockNumber();
    long long int GetBalance(const string *address);
    int GetTransactionCount(const string *address);

    string Call(const string *from, const string *to, long gas, long gasPrice, const string *value, const string *data);
    string SendSignedTransaction(const string *data, const uint32_t dataLen);

private:
    string exec(const string* data);
    string generateJson(const string* method, const string* params);
    int getInt(const string* json);
    long getLong(const string* json);
    long long int getLongLong(const string* json);
    double getDouble(const string* json);
    bool getBool(const string* json);
    string getString(const string* json);

private:
    const string* host;
    const string* path;


};

#endif //ESPIDF_SWAZM_H
