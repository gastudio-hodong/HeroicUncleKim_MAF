#ifndef MafEncrypt_h
#define MafEncrypt_h

#include <stdio.h>
#include <string>

class MafEncrypt
{
public:
    static MafEncrypt* getInstance();
    
    MafEncrypt(void);
    virtual ~MafEncrypt(void);
    virtual bool init();
    
public:
    ///ctr encrypt
    void init_ctr(struct ctr_state *state, const unsigned char ivec[16]);
    
    void ctr_encrypt(unsigned char *indata, unsigned char *outdata ,int32_t bytes_read);
    void ctr_decrypt(unsigned char *indata, unsigned char *outdata ,int32_t bytes_read);
    
    ///암호화
    std::string encrypt(int32_t data);
    std::string encrypt(const std::string& data, bool isBase64Encode = true);
    
    ///복호화
    std::string decrypt(const std::string& data, bool isBase64Encode = true);

	
private:
    unsigned char _key[32] = {0x00, };
    unsigned char _iv[16] = {0x00, };
    
};

#endif
