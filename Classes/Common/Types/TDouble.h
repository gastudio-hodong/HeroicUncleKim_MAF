#ifndef TDouble_h
#define TDouble_h

#include <stdio.h>
#include <string>
#include <cmath>

class TDouble
{
public:
    TDouble();
    TDouble(double value);
    TDouble(const TDouble& t);
    virtual ~TDouble();

public:
    TDouble& operator=(double t) {
        _value = t;
        encrypt();
        
        return *this;
    }
    
    TDouble& operator+=(double t) {
        decrypt();
        _value += t;
        encrypt();
        
        return *this;
    }
    
    TDouble& operator%=(double t) {
        decrypt();
        _value = std::fmod(_value, t);
        encrypt();

        return *this;
    }
    
    template<typename T> TDouble& operator-=(T t) {
        decrypt();
        _value -= t;
        encrypt();
        
        return *this;
    }
    
    template<typename T> TDouble& operator*=(T t) {
        decrypt();
        _value *= t;
        encrypt();
        
        return *this;
    }
    
    template<typename T> TDouble& operator/=(T t) {
        decrypt();
        _value /= t;
        encrypt();
        
        return *this;
    }
    
    
    double value();
    operator double();
    bool        operator!();
    TDouble&  operator++();
    TDouble&  operator--();
    TDouble   operator++(int32_t);
    TDouble   operator--(int32_t);
    
    void encrypt();
    void decrypt();
    
    
private:
    double _value;
    std::string _encrypt;
};



#endif /* Double_h */
