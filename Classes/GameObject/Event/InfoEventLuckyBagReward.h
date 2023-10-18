//
//  InfoEventLuckyBagReward.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/12/21.
//

#ifndef InfoEventLuckyBagReward_hpp
#define InfoEventLuckyBagReward_hpp

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoEventLuckyBagReward : public cocos2d::Ref
{
public:
    static InfoEventLuckyBagReward* create();
    
    InfoEventLuckyBagReward();
    virtual ~InfoEventLuckyBagReward();
    virtual bool init();
    
    
public:
    const int getIdx()const;
    void setIdx(const int value);
    
    const int getLevel() const;
    void setLevel(const int value);
    
    const int getRate() const;
    void setRate(const int value);
    
    void setItemIdx(const int value);
    
    void setItemCount(const int value);
    
    void setItemParam(const int value);
    
    InfoItems* getItem();
    
private:
    int _nIdx;
    int _nLevel;
    int _nRate;
    int _nItemIdx;
    int _nItemCount;
    int _nItemParams;
};

#endif /* InfoEventLuckyBagReward_hpp */
