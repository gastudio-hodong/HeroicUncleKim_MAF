//
//  MafNative.h
//  TapRed
//
//  Created by SanHeo on 2014. 8. 2..
//
//

#ifndef __TapRed__MafAes256__
#define __TapRed__MafAes256__

#include "cocos2d.h"

class MafAes256
{
public:
    
    static std::string Encrypt(std::string data);
    static std::string Decrypt(std::string data);

//    static const char* AESEncoding(const char* data);
//    static const char* AESDecoding(const char* data);
    
    static std::string URLEncode(std::string data);
    
    
    static std::string XorEncrypt(std::string data);
    static std::string XorEncrypt(int data)         {return XorEncrypt(cocos2d::StringUtils::toString(data));}
    static std::string XorEncrypt(long long data)   {return XorEncrypt(cocos2d::StringUtils::toString(data));}
    static std::string XorEncrypt(float data)       {return XorEncrypt(cocos2d::StringUtils::toString(data));}
    static std::string XorEncrypt(double data)      {return XorEncrypt(cocos2d::StringUtils::toString(data));}
    
    static std::string XorDecryptStr(std::string data)      {return XorEncrypt(data);}
    static int XorDecryptInt(std::string data)              {return atoi(XorEncrypt(data).c_str());}
    static long long XorDecryptLong(std::string data)       {return atoll(XorEncrypt(data).c_str());}
    static float XorDecryptFloat(std::string data)          {return atof(XorEncrypt(data).c_str());}
    static double XorDecryptDouble(std::string data)        {return atof(XorEncrypt(data).c_str());}
    
};


#endif /* defined(__TapRed__MafAes256__) */



/*
 JNI 
 type 정리
 http://stnguyen.com/cocos2d-x/call-java-functions-from-cpp.html
 
 사용법
 http://202psj.tistory.com/278
 
 */
