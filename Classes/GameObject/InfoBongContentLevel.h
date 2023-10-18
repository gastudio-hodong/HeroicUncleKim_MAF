//
//  InfoRaidBongLevel.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/27.
//

#ifndef InfoRaidBongLevel_hpp
#define InfoRaidBongLevel_hpp

#include "Common/ConfigDefault.h"

class InfoBongContentLevel : public cocos2d::Ref
{
public:
    
    CREATE_FUNC(InfoBongContentLevel);
    
    InfoBongContentLevel();
    virtual ~InfoBongContentLevel();
    virtual bool init();
    
    
public:
    int getLevel();
    void setLevel(int value);
    
    double getTotalEXP();
    void setTotalEXP(double value);
    
private:
    TInteger _nLevel;
    TDouble _fTotalEXP;
};


#endif /* InfoRaidBongLevel_hpp */
