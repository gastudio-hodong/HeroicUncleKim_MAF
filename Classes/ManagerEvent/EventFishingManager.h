//
//  EventFishingManager.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#ifndef EventFishingManager_h
#define EventFishingManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoBoard.h"

class InfoQuest;
class EventFishingManager : public MafSingleton<EventFishingManager>
{    
public:
    EventFishingManager(void);
    virtual ~EventFishingManager(void);
    virtual bool init();
    
public:
    // load
    void setLoadQuest();
    void setLoadBoard();
    
    // set, get : prison
    cocos2d::Vector<InfoQuest*> getListQuest();
    cocos2d::Vector<InfoBoard*> getListBoard();
    
    // set, get : other
    int getPointCount();
    void setPointCount(int nValue);
    
    int getBaitCount();
    void setBaitCount(int nValue);
    
    int getPearlCount();
    void setPearlCount(int nValue);
    
    int getBaitBuyRemaining();
    int getBaitBuyMax();
    int getBaitBuyPrice();
    
    int getBoardPosition();
    int getBoardDicePrice();
    
     void onEventActivate(int nIdx);
    
    // network : info
    void requestInfo(const std::function<void(bool, int)>& callback);
    void requestBuyBait(int nCount, const std::function<void(bool, int)>& callback);
    void requestFishing(int nCount, const std::function<void(bool, int, std::string)>& callback);
    void requestBoardInfo(const std::function<void(bool, int)>& callback);
    void requestBoardDice(const std::function<void(bool, int, int, std::string)>& callback);
   
    
protected:
    // network : prison info
    void responseInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseBuyBait(cocos2d::network::HttpResponse* response,std::string &data);
    void responseFishing(cocos2d::network::HttpResponse* response,std::string &data);
    void responseBoardInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseBoardDice(cocos2d::network::HttpResponse* response,std::string &data);
     

private:
    bool _isEventActivate;
    TInteger _nTotalPoint;
    TInteger _nTotalTicket;
    TInteger _nTotalBait;
    
    TInteger _nBaitBuyNow;
    TInteger _nBaitBuyMax;
    TInteger _nBaitBuyPrice;
    
    TInteger _nBoardPosition;
    TInteger _nBoardDicePrice;
    
    
    //
    cocos2d::Vector<InfoQuest*> _listQuest;
    cocos2d::Vector<InfoBoard*> _listBoard;
    
    
    //
    std::function<void(bool, int)> _callbackInfo;
    std::function<void(bool, int)> _callbackBuyBait;
    std::function<void(bool, int, std::string)> _callbackFishing;
    
    std::function<void(bool, int)> _callbackBoardInfo;
    std::function<void(bool, int, int, std::string)> _callbackBoardDice;
    
    
};

#endif /* EventFishingManager_h */
