//
//  MafNative.cpp
//  TapRed
//
//  Created by SanHeo on 2014. 8. 2..
//
//

#include "Libs/maf/MafAes256.h"
#include "Libs/maf/MafConfig.h"
#include "Libs/maf/MafEncrypt.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include <jni.h>
    #include "jni/JniHelper.h"
#endif


std::string MafAes256::URLEncode(std::string data)
{
    const std::string unreserved = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~";
    
    std::string escaped = "";
    for (size_t i = 0; i<data.length(); i++)
    {
        if (unreserved.find_first_of(data[i]) != std::string::npos)
        {
            escaped.push_back(data[i]);
        }
        else
        {
            escaped.append("%");
            char buf[3] = {0,};
    
            sprintf(buf, "%.2X", (unsigned char)data[i]);
            escaped.append(buf);
        }
    }
    
    return escaped;
}

std::string MafAes256::Encrypt(std::string data)
{
    return MafEncrypt::getInstance()->encrypt(data);
}

std::string MafAes256::Decrypt(std::string data)
{
    return MafEncrypt::getInstance()->decrypt(data);
}

/*
char key[8] = {
    static_cast<char>(random(1, 255)),static_cast<char>(random(1, 255)),
    static_cast<char>(random(1, 255)),static_cast<char>(random(1, 255)),
    static_cast<char>(random(1, 255)),static_cast<char>(random(1, 255)),
    static_cast<char>(random(1, 255)),static_cast<char>(random(1, 255))};
*/

std::string MafAes256::XorEncrypt(std::string strValue)
{
    
    /*
    std::string output = data;
    for (int i = 0; i < data.size(); i++)
        output[i] = data[i] ^ key[i % (sizeof(key) / sizeof(char))];
    return output;
     */
    
    std::string strResult = strValue;
    std::string strKey = "abcdefghijklmnopqrstu123456vwxyz";
    size_t klen=strKey.length();
    size_t vlen=strValue.length();
    unsigned int k=0;
    unsigned int v=0;
    
    for ( v=0; v < vlen; v++ )
    {
        strResult[v] = strResult[v] ^ strKey[k];
        k = (++k < klen ? k : 0);
    }
    
    return strResult;
}


