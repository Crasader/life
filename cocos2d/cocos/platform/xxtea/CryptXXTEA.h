//
//  CryptXXTEA.hpp
//  encodeResource
//
//  Created by ff on 16/6/3.
//
//

#ifndef CryptXXTEA_hpp
#define CryptXXTEA_hpp

#include "cocos2d.h"


class CryptXXTEA
{
public:
    CryptXXTEA();
    ~CryptXXTEA();
    static CryptXXTEA* getInstance();
    void setXXTEAKeyAndSign(const char *key, int keyLen, const char *sign, int signLen);
    size_t decryptData(unsigned char* chunk, size_t chunkSize);
    bool decryptData(cocos2d::Data &data);
protected:
    bool _xxteaEnabled;
    char* _xxteaKey;
    int _xxteaKeyLen;
    char* _xxteaSign;
    int _xxteaSignLen;
    
private:
    void cleanupXXTEAKeyAndSign();
    
    static CryptXXTEA* instance;
private:
    //bool _xxteaEnabled;
public:
    inline bool getXxteaEnabled(void) const { return _xxteaEnabled; }
    inline char* getXxteaKey(void) const { return _xxteaKey; }
    inline int getXxteaKeyLen(void) const { return _xxteaKeyLen; }
    inline char* getXxteaSign(void) const { return _xxteaSign; }
    inline int getXxteaSignLen(void) const { return _xxteaSignLen; }
};


#endif /* CryptXXTEA_hpp */
