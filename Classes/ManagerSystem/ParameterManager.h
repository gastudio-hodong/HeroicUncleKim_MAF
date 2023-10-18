#ifndef ParameterManager_h
#define ParameterManager_h

#include "Libs/maf/MafSingleton.h"

class ParameterManager : public MafSingleton<ParameterManager>
{    
public:
    ParameterManager(void);
    virtual ~ParameterManager(void);
    virtual bool init();
    
public:
    // load
    void setLoad();
    
    // set, get
    int getParameterInteger(std::string strKey);
    double getParameterDouble(std::string strKey);
    std::string getParameterString(std::string strKey);
    
protected:

private:
    //
    struct elementValue
    {
        int valueInteger;
        double valueDouble;
        std::string valueString;
    };
    
    std::unordered_map<std::string, elementValue> _listParameter;
    
    
};

#endif /* ParameterManager_h */
