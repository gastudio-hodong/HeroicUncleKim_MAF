//
//  InfoEventDraw.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 14/02/2020.
//

#ifndef InfoEventDraw_h
#define InfoEventDraw_h

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoEventDraw : public cocos2d::Ref
{
    
public:
    CREATE_FUNC(InfoEventDraw);
    
    InfoEventDraw();
    virtual ~InfoEventDraw();
    virtual bool init();
    
public:
    int getIdx();
    void setIdx(int value);
    
    int getType();
    void setType(int value);
    
    int getCountLimit();
    void setCountLimit(int value);
    
    int getRate();
    void setRate(int value);
    
    InfoItems* getReward();
    void setReward(int idx, int params, int count);
    
private:
    TInteger _idx;
    TInteger _type;
    TInteger _countLimit;
    TInteger _rate;
    
    InfoItems* _reward;
    
};
#endif /* InfoEventDraw_h */
