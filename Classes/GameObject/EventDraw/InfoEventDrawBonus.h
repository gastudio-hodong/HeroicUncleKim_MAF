//
//  InfoEventDrawBonus.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 14/02/2020.
//

#ifndef InfoEventDrawBonus_h
#define InfoEventDrawBonus_h

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoEventDrawBonus : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoEventDrawBonus);
    
    InfoEventDrawBonus();
    virtual ~InfoEventDrawBonus();
    virtual bool init();
    
public:
    int getIdx();
    void setIdx(int value);
    
    int getCountMax();
    void setCountMax(int value);
    
    InfoItems* getReward();
    void setReward(int idx, int params, int count);
    
private:
    TInteger  _idx;
    TInteger _countMax;
    
    InfoItems* _reward;
    
};
#endif /* InfoEventDrawBonus_h */
