//
//  EventBoardGameManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/15.
//

#ifndef EventBoardGameManager_hpp
#define EventBoardGameManager_hpp

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoBoard.h"

class InfoQuest;
class EventBoardGameManager : public MafSingleton<EventBoardGameManager>
{
public:
    EventBoardGameManager(void);
    virtual ~EventBoardGameManager(void);
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
    
    int getTotalCandy();
    void setTotalCandy(int nValue);
    
    int getTotalPoint();
    void setTotalPoint(int nValue);
    
    int getBoardPosition();
    
    int getAdsNow();
    int getAdsMax();
    
     void onEventActivate(int nIdx);
    
    // network : info
    
    void requestBoardInfo(const std::function<void(bool, int)>& callback);
    void requestBoardReward(const std::function<void(bool, int, int, std::string)>& callback);
    void requestBoardAds(const std::function<void(bool, int)>& callback);
protected:
    // network : prison info
    void responseBoardInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseBoardReward(cocos2d::network::HttpResponse* response,std::string &data);
    void responseBoardAds(cocos2d::network::HttpResponse* response,std::string &data);

private:
    bool _isEventActivate;
    TInteger _nBoardPosition;
    TInteger _nTotalCandy;
    TInteger _nTotalPoint;
    
    TInteger _adsNow;
    TInteger _adsMax;
    
    //
    cocos2d::Vector<InfoQuest*> _listQuest;
    cocos2d::Vector<InfoBoard*> _listBoard;
    
    std::function<void(bool, int)> _callbackBoardInfo;
    std::function<void(bool, int, int, std::string)> _callbackBoardReward;
    std::function<void(bool, int)> _callbackBoardAds;
};


#endif /* EventBoardGameManager_hpp */
