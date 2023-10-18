#include "TInteger.h"

#include "cocos2d.h"
TInteger::TInteger() :
_value(0),
_valueEncrypt("0")
{
    srand((unsigned int)time(0));
    _xorKey = rand() % (std::numeric_limits<int>::max() - 1000000000 + 1) + 1000000000;
    
    encrypt();
}

TInteger::TInteger(int64_t value) : TInteger()
{
    _value = value;
    encrypt();
}

TInteger::TInteger(const TInteger& t) : TInteger()
{
    _value = t._value;
    _valueEncrypt = t._valueEncrypt;
    
    _xorKey = t._xorKey;
    _xorValue = t._xorValue;
}

TInteger::~TInteger()
{

}

int64_t TInteger::value()
{
    decrypt();
    return _value;
}

int TInteger::valueInt()
{
    decrypt();
    return (int)_value;
}

TInteger::operator int64_t()
{
    decrypt();
    return _value;
}

bool TInteger::operator!()
{
    decrypt();
    return !_value;
}

TInteger TInteger::operator++(int32_t)
{
    TInteger ret = *this;
    
    decrypt();
    ++_value;
    encrypt();
    
    return ret;
}

TInteger TInteger::operator--(int32_t)
{
    TInteger ret = *this;
    
    decrypt();
    --_value;
    encrypt();
    
    return ret;
}

void TInteger::encrypt()
{
    _valueEncrypt = std::to_string(_value);
    _xorValue = _value ^ _xorKey;

    char* encodedData = nullptr;
    cocos2d::base64Encode((unsigned char*)_valueEncrypt.c_str(), (unsigned int)strlen(_valueEncrypt.c_str()), &encodedData);
    if ( encodedData )
    {
        _valueEncrypt = encodedData;
        free(encodedData);
    }
    else
    {
        _valueEncrypt.insert(0, "$");
    }
}

void TInteger::decrypt()
{
    int64_t nTemp = _xorValue ^ _xorKey;
    if ( _value != nTemp )
    {
        std::string encrypt = _valueEncrypt;
        
        if ( encrypt.find_first_of("$") == 0 )
        {
            encrypt.erase(0, 1);
            _value = std::stoll(encrypt);
        }
        else
        {
            unsigned char * decodedData = nullptr;
            int decodedDataLen = cocos2d::base64Decode((unsigned char*)encrypt.c_str(), (unsigned int)strlen(encrypt.c_str()), &decodedData);
            if ( decodedData )
            {
                std::string strData = std::string((char*)decodedData, decodedDataLen);
                _value = std::stoll(strData);
                free(decodedData);
            }
        }
    }
}


