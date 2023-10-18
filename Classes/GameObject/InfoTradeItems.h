//
//  InfoTradeItems.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 15/07/2019.
//

#ifndef InfoTradeItems_h
#define InfoTradeItems_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoTradeItems : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoTradeItems);
    
    InfoTradeItems();
    virtual ~InfoTradeItems();
    virtual bool init();
    
public:
    int getIdx();
    void setIdx(int nIdx);
    
    //
    E_ITEMS getTradeForType();
    void setTradeForType(E_ITEMS eType);
    int getTradeForCount();
    void setTradeForCount(int nCount);
    
    //
    E_ITEMS getTradeToType();
    void setTradeToType(E_ITEMS eType);
    int getTradeToCount();
    void setTradeToCount(int nCount);
    
    int getTradeLimit();
    void setTradeLimit(int nLimit);
    
    int getTradeCount();
    void setTradeCount(int nCount);
    
    int getTradeProceedingCount();
    void setTradeProceedingCount(int nCount);
    
    
private:
    int _nIdx;
    
    E_ITEMS _eTradeForType;
    int _nTradeForCount;
    
    E_ITEMS _eTradeToType;
    int _nTradeToCount;
    
    
    int _nTradeLimit;   //거래 제한 수
    int _nTradeCount;   //거래 했는 수
    int _nTradeProceedingCount; //현재 거래 할려는 수
    
};

#endif /* InfoTradeItems_h */
