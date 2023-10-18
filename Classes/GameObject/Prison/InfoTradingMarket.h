//
//  InfoTradingMarket.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 15/07/2019.
//

#ifndef InfoTradingMarket_h
#define InfoTradingMarket_h

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoTradingMarket : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoTradingMarket);
    
    InfoTradingMarket();
    virtual ~InfoTradingMarket();
    virtual bool init();
    
public:
    int getType();
    void setType(int value);
    
    int getIdx();
    void setIdx(int value);
    
    int getCountMax();
    void setCountMax(int value);
    
    InfoItems* getItemFrom();
    void setItemFrom(std::string value);
    
    InfoItems* getItemTo();
    void setItemTo(std::string value);
    
    InfoItems*  getOpenCost();
    void setOpenCost(int value);
    
    int getOpenStep();
    void setOpenStep(int value);
    
private:
    int _type;
    int _idx;

    int _countMax;
    
    InfoItems* _itemFrom;
    InfoItems* _itemTo;
    InfoItems* _openCost;
    
    int _openStep;
};

#endif /* InfoTradingMarket_h */
