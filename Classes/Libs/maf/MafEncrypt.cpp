#include "Libs/maf/MafEncrypt.h"

#include <cocos2d.h>

#include <openssl/aes.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/crypto.h>

USING_NS_CC;

#define KEY_SIZE 256

struct ctr_state {
    unsigned char ivec[AES_BLOCK_SIZE];
    unsigned int num;
    unsigned char ecount[AES_BLOCK_SIZE];
};

static MafEncrypt* s_instance = nullptr;
MafEncrypt* MafEncrypt::getInstance()
{
    if ( s_instance == nullptr )
    {
        s_instance = new (std::nothrow) MafEncrypt();
        s_instance->init();
    }
    
    return s_instance;
}

MafEncrypt::MafEncrypt()
{
    
}

MafEncrypt::~MafEncrypt()
{
    
}

bool MafEncrypt::init()
{
#if TARGET_IPHONE_SIMULATOR
    OPENSSL_cleanse(nullptr, 0);
#endif
    
    strncpy((char*)_key, "abcdefghijklmnopqrstu123456vwxyz", sizeof(_key)/sizeof(unsigned char));
    strcpy((char*)_iv, "");
    
    return true;
}

void MafEncrypt::init_ctr(struct ctr_state *state, const unsigned char ivec[16])
{
    state->num = 0;
    memset(state->ecount, 0, AES_BLOCK_SIZE);
    memcpy(state->ivec, ivec, AES_BLOCK_SIZE);
}

void MafEncrypt::ctr_encrypt(unsigned char *indata, unsigned char *outdata ,int32_t bytes_read)
{
    struct ctr_state state;
    init_ctr(&state, _iv);
    
    AES_KEY     key;
    AES_set_encrypt_key(_key, KEY_SIZE, &key);
    
    AES_cbc_encrypt((unsigned char *)indata, (unsigned char *)outdata, bytes_read, &key, state.ivec, AES_ENCRYPT);
}

void MafEncrypt::ctr_decrypt(unsigned char *indata, unsigned char *outdata ,int32_t bytes_read)
{
    struct ctr_state state;
    init_ctr(&state, _iv);
    
    AES_KEY     key;
    AES_set_decrypt_key(_key, KEY_SIZE, &key);
    
    AES_cbc_encrypt((unsigned char *)indata, (unsigned char *)outdata, bytes_read, &key, state.ivec, AES_DECRYPT);
}

std::string MafEncrypt::encrypt(int32_t data)
{
    std::ostringstream oss;
    oss << data;
    return encrypt(oss.str());
}

std::string MafEncrypt::encrypt(const std::string& data, bool isBase64Encode)
{
    size_t encryptLen = data.length();
    size_t blockLen = (int)(encryptLen / AES_BLOCK_SIZE + 1);
    
    std::vector<unsigned char> tmpIn(blockLen * AES_BLOCK_SIZE + 1, 0x00);
    tmpIn.assign(data.begin(), data.end());
    
    std::vector<unsigned char> tmpOut(blockLen * AES_BLOCK_SIZE + 1, 0x00);
    
    int k = encryptLen % AES_BLOCK_SIZE;
    int j = (int)(encryptLen / AES_BLOCK_SIZE);
    int padding = AES_BLOCK_SIZE - k;
    for ( int i = 0; i < padding; i++ )
    {
        tmpIn[j * AES_BLOCK_SIZE + k + i] = padding;
    }
    tmpIn[blockLen * AES_BLOCK_SIZE] = '\0';
    
    
    // 암호화
    ctr_encrypt((unsigned char*)tmpIn.data(), (unsigned char*)tmpOut.data(), (unsigned int)blockLen * AES_BLOCK_SIZE);
    
    std::string result;
    if ( isBase64Encode )
    {
        // 인코딩
        char* encode_text;
        
        base64Encode((const unsigned char*)tmpOut.data(), (unsigned int)blockLen * AES_BLOCK_SIZE, (char**)&encode_text);
        result = std::string(encode_text);

        // free
        free(encode_text);
    }
    else
    {
        result = std::string((char*)tmpOut.data());
    }
    
    return result;
}

std::string MafEncrypt::decrypt(const std::string& data, bool isBase64Encode)
{
    std::string strData = "";
    if ( isBase64Encode )
    {
        unsigned char* decode_text;
        
        int len = base64Decode((unsigned char*)data.c_str(), (unsigned int)data.length(), &decode_text);
        strData = std::string((char*)decode_text, len);
        
        // free
        free(decode_text);
    }
    else
    {
        strData = std::string((char*)data.c_str());
    }
    
    size_t decryptLen = strData.length();
    std::vector<unsigned char> tmpOut(decryptLen + 1, 0x00);
    
    //복호화
    ctr_decrypt((unsigned char*)strData.c_str(), (unsigned char*)tmpOut.data(), (unsigned int)decryptLen);
    
    if (0x00 < tmpOut[decryptLen - 1] && tmpOut[decryptLen - 1] <= 0x16)
    {
        int tmp = tmpOut[decryptLen - 1];
        for (int i = (int)(decryptLen - 1); i >= decryptLen - tmp && i >= 0; i--)
        {
            if (tmpOut[i] != tmp)
            {
                tmpOut.assign(decryptLen, 0x00);
                break;
            }
            else
                tmpOut[i] = 0x00;
        }
    }
    
    std::string result = std::string((char*)tmpOut.data());
    return result;
}
