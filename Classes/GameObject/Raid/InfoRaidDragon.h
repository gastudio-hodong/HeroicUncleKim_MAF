//
//  InfoRaidDragon.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/27.
//

#ifndef InfoRaidDragon_hpp
#define InfoRaidDragon_hpp

#include "Common/ConfigDefault.h"

class InfoRaidDragon : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoRaidDragon);
    InfoRaidDragon();
    virtual ~InfoRaidDragon();
    virtual bool init();
    
    
public:
    int getLevel();
    void setLevel(int value);
    
    double getHP();
    void setHP(double value);
    
private:
    TInteger _nLevel;
    TDouble _fHP;
};

#endif /* InfoRaidDragon_hpp */
