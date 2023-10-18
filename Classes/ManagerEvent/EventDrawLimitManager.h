//
//  EventDrawLimitManager.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#ifndef EventDrawLimitManager_h
#define EventDrawLimitManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoItems.h"

class InfoEventDraw;
class InfoEventDrawBonus;
class EventDrawLimitManager : public MafSingleton<EventDrawLimitManager>
{
public:
    EventDrawLimitManager(void);
    virtual ~EventDrawLimitManager(void);
    virtual bool init();
    
public:
    // load
    void setLoad();
    void setLoadFixed();
    
    // set, get : info
    cocos2d::Vector<InfoEventDraw*> getListInfoItemsAll();
    cocos2d::Vector<InfoEventDraw*> getListInfoItemsLimit();
    InfoEventDraw* getInfoItems(int idx);
    cocos2d::Vector<InfoEventDrawBonus*> getListInfoBonusAll();
    InfoEventDrawBonus* getInfoBonus(int idx);
    
    int64_t getEndTime();
    int64_t getResetTime();
    int getTokenNow();
    int getTokenPieceNow();
    int getTokenPieceMax();
    
    int getBonusIdx();
    int getBonusCountNow();
    int getBonusCountMax();
    bool isBonusFinish();
    
    bool isExtraTime();
    
    std::vector<int> getListDraw();
    
    // network
    void requestInfo(bool bPopupLoading, const std::function<void(bool, int)>& pCallback);
    void requestDraw(bool bPopupLoading, int nCount, const std::function<void(bool, int)>& pCallback);
    void requestDrawBonus(bool bPopupLoading, int nCount, const std::function<void(bool, int)>& pCallback);
    void requestTokenPieceExchange(bool bPopupLoading, const std::function<void(bool, int)>& pCallback);
    void requestTokenExchange(bool bPopupLoading, const std::function<void(bool, int)>& pCallback);
    
    // event
    int onDraw(int count, const std::function<void(bool, int)>& callback);
    int onDrawBonus(const std::function<void(bool, int)>& callback);
    int onTokenPieceExchange(const std::function<void(bool, int)>& callback);
    int onTokenExchange(const std::function<void(bool, int)>& callback);
    
    void onEventActivate(int nIdx);
protected:
    // network
    void responseInfo(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseDraw(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseDrawBonus(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseTokenPieceExchange(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseTokenExchange(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    
    void convertInfoDraw(const rapidjson::Value& json);
    
private:
    bool _isEventActivate;
    int64_t _endTime;
    int64_t _resetTime;
    
    
    TInteger _tokenNow; // 코인 수
    TInteger _tokenPieceNow; // 코인 조각 수
    TInteger _bonusIdx; // 뽑기 보너스 IDX
    TInteger _bonusCount;
    
    bool _bExtraTime;
    
    cocos2d::Vector<InfoEventDraw*> _listInfoDraw;
    cocos2d::Vector<InfoEventDrawBonus*> _listInfoDrawBonus;
  
    std::vector<int> _listDraw;
   
    //
    std::function<void(bool, int)> _callbackInfo;
    std::function<void(bool, int)> _callbackDraw;
    std::function<void(bool, int)> _callbackDrawBonus;
    std::function<void(bool, int)> _callbackTokenPieceExchange;
    std::function<void(bool, int)> _callbackTokenExchange;
};

#endif /* EventDrawLimitManager_h */
