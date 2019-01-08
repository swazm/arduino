#ifndef ESPIDF_SWAZM_LOG_H
#define ESPIDF_SWAZM_LOG_H

#define DEBUGLOG

class Log {
public:
    void print(const char* s);
    void println(const char* s);
};

#endif //ESPIDF_SWAZM_LOG_H
