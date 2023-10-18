//
//  InfoEventTreasureHuntMap.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/24.
//

#ifndef InfoEventTreasureHuntMap_hpp
#define InfoEventTreasureHuntMap_hpp

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoEventTreasureHuntMap : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoEventTreasureHuntMap);
    
    InfoEventTreasureHuntMap();
    virtual ~InfoEventTreasureHuntMap();
    virtual bool init();
        
public:
    const int getIdx();
    void setIdx(int value);
    
    cocos2d::Vector<InfoItems*> getReward();
    void setReward(const std::string value);
private:
    TInteger _idx;
    
    std::string _reward;
    
};

#endif /* InfoEventTreasureHuntMap_hpp */
