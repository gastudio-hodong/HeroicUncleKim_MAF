#ifndef TInteger_h
#define TInteger_h

#include <stdio.h>
#include <string>

class TInteger
{
public:
    TInteger();
    TInteger(int64_t value);
    TInteger(const TInteger& t);
    virtual ~TInteger();

public:
    TInteger& operator=(int64_t t) {
        _value = t;
        encrypt();
        
        return *this;
    }
    
    TInteger& operator+=(int64_t t) {
        decrypt();
        _value += t;
        encrypt();
        
        return *this;
    }
    
    TInteger& operator%=(int64_t t) {
        decrypt();
        _value %= t;
        encrypt();
        
        return *this;
    }
    
    template<typename T> TInteger& operator-=(T t) {
        decrypt();
        _value -= t;
        encrypt();
        
        return *this;
    }
    
    template<typename T> TInteger& operator*=(T t) {
        decrypt();
        _value *= t;
        encrypt();
        
        return *this;
    }
    
    template<typename T> TInteger& operator/=(T t) {
        decrypt();
        _value /= t;
        encrypt();
        
        return *this;
    }
    
    int64_t value();
    int valueInt();
    operator int64_t();
    bool        operator!();
    TInteger&  operator++();
    TInteger&  operator--();
    TInteger   operator++(int32_t);
    TInteger   operator--(int32_t);
    
    void encrypt();
    void decrypt();
    
    
private:
    int64_t _value;
    std::string _valueEncrypt;
    
    int _xorKey;
    int64_t _xorValue;
    
};



#endif /* TInteger_h */
