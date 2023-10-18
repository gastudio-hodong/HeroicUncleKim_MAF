//
//  TradingMarketManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/06/07.
//

#ifndef TradingMarketManager_hpp
#define TradingMarketManager_hpp

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoLoot.h"
#include "GameObject/Prison/InfoTradingMarket.h"

class TradingMarketManager : public MafSingleton<TradingMarketManager>
{
public:
    TradingMarketManager(void);
    virtual ~TradingMarketManager(void);
    virtual bool init();
    
public:
    
    // load
    void setLoad();
    void setLoadTrade();
    
    // info
    cocos2d::Vector<InfoTradingMarket*> getListInfoTradingMarketAll();
    cocos2d::Vector<InfoTradingMarket*> getListInfoTradingMarketFromType(int type);
    InfoTradingMarket* getInfoTradingMarket(int type, int idx);
    
    
    // set, get :
    bool isOpen(int type, int idx);
    int getCount(int type, int idx);
    
    int getHellPointNow();
    int getHellPointMax();
    
    // network
    void requestInfo(bool bPopupLoading, int type, const std::function<void(bool)>& callback);
    void requestOpen(bool bPopupLoading, int type, int idx, const std::function<void(bool)>& callback);
    void requestTrade(bool bPopupLoading, int type, int idx, int count, const std::function<void(bool)>& callback);
    
    
    // event
    int onTradeOpen(int type, int idx, const std::function<void(bool)>& callback);
    int onTrade(int type, int idx, int count, const std::function<void(bool)>& callback);
    
protected:
    // network
    void responseInfo(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseOpen(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseTrade(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    
    
private:
    cocos2d::Vector<InfoTradingMarket*> _listInfoTradingMarket;
    
    //
    std::map<std::pair<int, int>, bool> _listOpen;
    std::map<std::pair<int, int>, int> _listCount;
    
    TInteger _hellPointNow;
    TInteger _hellPointMax;
    
    
    //
    std::function<void(bool)> _callbackInfo;
    std::function<void(bool)> _callbackOpen;
    std::function<void(bool)> _callbackTrade;

};

#endif /* TradingMarketManager_hpp */
