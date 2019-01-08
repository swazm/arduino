#ifndef ESPIDF_SWAZM_CONTRACT_H
#define ESPIDF_SWAZM_CONTRACT_H

#include "Log.h"
#include "Swazm.h"
#include <vector>

using namespace std;

class Contract {

public:
    typedef struct {
        char from[80];
        char to[80];
        char gasPrice[20];
        long gas;
    } Options;
    Options options;

public:
    Contract(Swazm* _swazm, const string* address);
    void SetPrivateKey(const uint8_t *key);
    string SetupContractData(const string *func, ...);
    string Call(const string* param);
    string SendTransaction(uint32_t nonceVal, uint32_t gasPriceVal, uint32_t gasLimitVal,
                           string *toStr, string *valueStr, string *dataStr);

private:
    Log Debug;
    #define LOG(x) Debug.println(x)

    Swazm* swazm;
    const string * contractAddress;
    const uint8_t * privateKey;

private:
    string GenerateContractBytes(const string *func);
    string GenerateBytesForInt(const int32_t value);
    string GenerateBytesForUint(const uint32_t value);
    string GenerateBytesForAddress(const string *value);
    string GenerateBytesForString(const string *value);
    string GenerateBytesForBytes(const char* value, const int len);

    void GenerateSignature(uint8_t* signature, int* recid, uint32_t nonceVal, uint32_t gasPriceVal, uint32_t  gasLimitVal,
                           string* toStr, string* valueStr, string* dataStr);
    vector<uint8_t> RlpEncode(
            uint32_t nonceVal, uint32_t gasPriceVal, uint32_t  gasLimitVal,
            string* toStr, string* valueStr, string* dataStr);
    vector<uint8_t> RlpEncodeForRawTransaction(
            uint32_t nonceVal, uint32_t gasPriceVal, uint32_t  gasLimitVal,
            string* toStr, string* valueStr, string* dataStr, uint8_t* sig, uint8_t recid);
    void Sign(uint8_t* hash, uint8_t* sig, int* recid);
};


#endif //ESPIDF_SWAZM_CONTRACT_H
