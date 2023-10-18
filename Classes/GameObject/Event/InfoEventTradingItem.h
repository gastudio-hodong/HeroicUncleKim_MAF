//
//  InfoEventTradingItem.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/01/11.
//

#ifndef InfoEventTradingItem_hpp
#define InfoEventTradingItem_hpp

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoEventTradingItem : public cocos2d::Ref
{
public:
    static InfoEventTradingItem* create();
    
    InfoEventTradingItem();
    virtual ~InfoEventTradingItem();
    virtual bool init();
    
    
public:
    int getOrder();
    void setOrder(int value);
    
    int getIdx();
    void setIdx(int value);
    
    int getBuyCount();
    void setBuyCount(int value);
    
    int getMaxBuyCount();
    void setMaxBuyCount(int value);
    
    std::string getType();
    void setType(std::string value);
    
    InfoItems* getItem();
    void setItem(InfoItems* value);
    
    InfoItems* getPrice();
    void setPrice(InfoItems* value);
    
private:
    int _nOrder;
    int _nIdx;
    int _nBuyCount;
    int _nMaxBuyCount;
    std::string _strType;
    
    InfoItems* _item;
    InfoItems* _itemPrice;
};


#endif /* InfoEventTradingItem_hpp */
