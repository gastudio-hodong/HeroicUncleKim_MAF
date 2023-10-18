//
//  InfoRaidTradingMarketReward.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/02/15.
//

#ifndef InfoRaidTradingMarketReward_hpp
#define InfoRaidTradingMarketReward_hpp

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoRaidTradingMarketReward : public cocos2d::Ref
{
public:
    enum class E_TYPE
    {
        FIXED = 0,
        RAND
    };
    static InfoRaidTradingMarketReward* create();
    
    InfoRaidTradingMarketReward();
    virtual ~InfoRaidTradingMarketReward();
    virtual bool init();
    
    
public:
    void setActivate(bool value);
    
    int getIdx();
    void setIdx(int value);
    
    E_TYPE getType();
    void setType(E_TYPE value);
    
    int getBuyCount();
    void setBuyCount(int value);
    
    int getMaxBuyCount();
    void setMaxBuyCount(int value);
    
    int getStartTime();
    void setStartTime(int value);
    
    int getEndTime();
    void setEndTime(int value);
    
    bool isSaleCondition();
    
    InfoItems* getItem();
    void setItem(InfoItems* value);
    
    InfoItems* getPrice();
    void setPrice(InfoItems* value);
    
private:
    bool isActivate;
    E_TYPE _eType;
    int _nIdx;
    int _nBuyCount;
    int _nMaxBuyCount;
    
    int _nStartTime;
    int _nEndTime;
    
    InfoItems* _item;
    InfoItems* _itemPrice;
};

#endif /* InfoRaidTradingMarketReward_hpp */
