//
//  InfoEventTreasureHuntReward.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/24.
//

#ifndef InfoEventTreasureHuntReward_hpp
#define InfoEventTreasureHuntReward_hpp

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoEventTreasureHuntReward : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoEventTreasureHuntReward);
    
    InfoEventTreasureHuntReward();
    virtual ~InfoEventTreasureHuntReward();
    virtual bool init();
        
public:
    const int getType();
    void setType(int value);
    
    const int getLevel();
    void setLevel(int value);
    
    const int getValue();
    void setValue(int value);
    
    cocos2d::Vector<InfoItems*> getReward();
    void setReward(const std::string value);
private:
    TInteger _type;
    TInteger _level;
    TInteger _value;
    
    std::string _reward;
    
};

#endif /* InfoEventTreasureHuntReward_hpp */
