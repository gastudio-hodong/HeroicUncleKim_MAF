//
//  InfoRaidStatIncrease.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/10.
//

#ifndef InfoRaidStatIncrease_hpp
#define InfoRaidStatIncrease_hpp

#include "Common/ConfigDefault.h"

class InfoRaidStatIncrease : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoRaidStatIncrease);
    
    InfoRaidStatIncrease();
    virtual ~InfoRaidStatIncrease();
    virtual bool init();
    
    
public:
    int getLv();
    void setLv(int value);
    int getDmg();
    void setDmg(int value);
    
    double getCriDmg();
    void setCriDmg(double value);
    double getCriRate();
    void setCriRate(double value);
    double getDoubleRate();
    void setDoubleRate(double value);
private:
    int _nLv;
    int _nDmg;
    TDouble _fCriDmg;
    TDouble _fCriRate;
    TDouble _fDoubleRate;
    
};



#endif /* InfoRaidStatIncrease_hpp */
