//
//  EventBingoManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/07/23.
//

#ifndef EventBingoManager_hpp
#define EventBingoManager_hpp

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoBingoBlock.h"

class InfoItems;
class EventBingoManager : public MafSingleton<EventBingoManager>
{
public:
    EventBingoManager(void);
    virtual ~EventBingoManager(void);
    virtual bool init();
    
public:
    //get
    
    cocos2d::Vector<InfoBingoBlock*> getListBoardBlock();
    cocos2d::Vector<InfoBingoBlock*> getListLineRewardBlock();
    cocos2d::Vector<InfoBingoBlock*> getListBingoQuestBlock();
    
    InfoBingoBlock* getBoardBlock(int nNum);
    InfoBingoBlock* getLineRewardBlock(int nNum);
    InfoBingoBlock* getQuestRewardBlock(int nNum);
    
    cocos2d::Vector<InfoBingoBlock*> getListBoardBlockLine(std::vector<int> vecIdx);
    
    bool isRewardPossible(int idx);
    bool isQuestRewardPossible(int idx);
    bool isRoundEnd();
    
    int getBingoCount();
    int getBingoCompleteCount();
    int getBingoTicketCount();
    void setBingoTicketCount(int nValue);
    
    int getEventIdx();
    int getEventStartTime();
    int getEventEndTime();
    int getEventTotalEndTime();
    
    // network : info
    void requestInfo(const std::function<void(bool, int)>& callback);
    void requestUseTicket(const std::function<void(bool, int, int, cocos2d::Vector<InfoItems*>)>& callback);
    void requestReward(const std::function<void(bool, int)>& callback, int nNum);
    void requestQuestReward(const std::function<void(bool, int)>& callback, int nIdx);
    void requestNextBoard(const std::function<void(bool, int)>& callback);
   
    void onEventActivate(int nIdx);
protected:
    // network : prison info
    void responseInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseUseTicket(cocos2d::network::HttpResponse* response,std::string &data);
    void responseReward(cocos2d::network::HttpResponse* response,std::string &data);
    void responseQuestReward(cocos2d::network::HttpResponse* response,std::string &data);
    void responseNextBoard(cocos2d::network::HttpResponse* response,std::string &data);
     

private:
    bool _isEventActivate;
    int _nBingoCompleteCount;
    int _nBingoTicketCount;
    
    int _nEventIdx;
    int _nEventStartTime;
    int _nEventEndTime;
    
    int _nTotalEndTime;
    
    std::function<void(bool, int)> _callbackInfo;
    std::function<void(bool, int, int, cocos2d::Vector<InfoItems*>)> _callbackUseTicket;
    std::function<void(bool, int)> _callbackReward;
    std::function<void(bool, int)> _callbackQuestReward;
    std::function<void(bool, int)> _callbackNextBoard;
    
    cocos2d::Vector<InfoBingoBlock*> _listBoardBlock;
    cocos2d::Vector<InfoBingoBlock*> _listLineRewardBlock;
    cocos2d::Vector<InfoBingoBlock*> _listBingoQuestBlock;
};

#endif /* EventBingoManager_hpp */
