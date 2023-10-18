#include "TDouble.h"

#include "cocos2d.h"
TDouble::TDouble()
{
    _value = 0;
    encrypt();
}

TDouble::TDouble(double value)
{
    _value = value;
    encrypt();
}

TDouble::TDouble(const TDouble& t)
{
    _value = t._value;
    _encrypt = t._encrypt;
}

TDouble::~TDouble()
{

}

double TDouble::value()
{
    decrypt();
    return _value;
}

TDouble::operator double()
{
    decrypt();
    return _value;
}

bool TDouble::operator!()
{
    decrypt();
    return !_value;
}

TDouble& TDouble::operator++()
{
    decrypt();
    ++_value;
    encrypt();
    
    return *this;
}

TDouble& TDouble::operator--()
{
    decrypt();
    --_value;
    encrypt();
    
    return *this;
}

TDouble TDouble::operator++(int32_t)
{
    TDouble ret = *this;
    
    decrypt();
    ++_value;
    encrypt();
    
    return ret;
}

TDouble TDouble::operator--(int32_t)
{
    TDouble ret = *this;
    
    decrypt();
    --_value;
    encrypt();
    
    return ret;
}

void TDouble::encrypt()
{
    _encrypt = std::to_string(_value);

    char* encodedData = nullptr;
    cocos2d::base64Encode((unsigned char*)_encrypt.c_str(), (unsigned int)strlen(_encrypt.c_str()), &encodedData);
    if ( encodedData )
    {
        _encrypt = encodedData;
        free(encodedData);
    }
    else
    {
        _encrypt.insert(0, "$");
    }
}

void TDouble::decrypt()
{
    std::string encrypt = _encrypt;
    
    if ( encrypt.find_first_of("$") == 0 )
    {
        encrypt.erase(0, 1);
        _value = std::stod(encrypt);
    }
    else
    {
        unsigned char * decodedData = nullptr;
        int decodedDataLen = cocos2d::base64Decode((unsigned char*)encrypt.c_str(), (unsigned int)strlen(encrypt.c_str()), &decodedData);
        if ( decodedData )
        {
            std::string strData = std::string((char*)decodedData, decodedDataLen);
            _value = std::stod(strData);
            free(decodedData);
        }
    }
}


