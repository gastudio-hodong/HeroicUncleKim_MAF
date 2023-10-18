//
//  InfoRaidMileageLv.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/27.
//

#ifndef InfoRaidMileageLv_hpp
#define InfoRaidMileageLv_hpp

#include "Common/ConfigDefault.h"

class InfoRaidMileageLv : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoRaidMileageLv);
    
    InfoRaidMileageLv();
    virtual ~InfoRaidMileageLv();
    virtual bool init();
    
    
public:
    int getLv();
    void setLv(int value);
    
    int getValue();
    void setValue(int value);
    
private:
    int _nILv;
    float _nValue;
};

#endif /* InfoRaidMileageLv_hpp */
