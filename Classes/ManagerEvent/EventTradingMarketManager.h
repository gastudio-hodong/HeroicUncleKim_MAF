//
//  EventTradingMarketManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/01/11.
//

#ifndef EventTradingMarketManager_h
#define EventTradingMarketManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/Event/InfoEventTradingItem.h"
#include "GameObject/InfoSetting.h"

class EventTradingMarketManager : public MafSingleton<EventTradingMarketManager>
{
public:
    EventTradingMarketManager(void);
    virtual ~EventTradingMarketManager(void);
    virtual bool init();
    
public:
    //load
    void setLoadTradingItem();
    void setLoadSetting();
    
    //get
    cocos2d::Vector<InfoEventTradingItem*> getListInfoEventTradingItem();
    
    InfoEventTradingItem* getInfoEventTradingItem(int idx);
    InfoSetting* getRaidSetting(std::string strKey);
    
    int getTotalCoin();
    void setTotalCoin(int value);
    int getTodayCoin();
    void setTodayCoin(int value);
    int64_t getEndTime();
    
    void onEventActivate(int nIdx);
    
    //network : request
    void requestInfo(const std::function<void(bool, int)>& callback);
    void requestBuyItem(const std::function<void(bool, int)>& callback, int nIdx);
    
protected:
    // network : respons
    void responseInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseBuyItem(cocos2d::network::HttpResponse* response,std::string &data);
    
private:
    bool _isEventActivate;
    int _nTotalCoin;
    int _nTodayCoin;
    int64_t _nEndTime;
    
    cocos2d::Vector<InfoEventTradingItem*> _listInfoEventTradingItem;
    cocos2d::Vector<InfoSetting*> _listInfoSetting;
    
    std::function<void(bool, int)> _callbackInfo;
    std::function<void(bool, int)> _callbackBuyItem;
    
};


#endif /* EventTradingMarketManager_hpp */
