//
//  EventManager.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#ifndef EventManager_h
#define EventManager_h

#include "Libs/maf/MafSingleton.h"

#include "GameObject/Event/InfoActivatedEvent.h"
#include "GameObject/Event/InfoEventOriginData.h"

class EventManager : public MafSingleton<EventManager>
{
public:
    EventManager(void);
    virtual ~EventManager(void);
    virtual bool init();
    
public:
    void setLoadOriginData();
    // set, get
    void addActivatedEvent(InfoActivatedEvent* obj);
    InfoActivatedEvent* getActivatedEvent(int nIdx);
    bool isActivatedEvent(int nIdx);
    
    InfoEventOriginData* getEventOriginDataByEventIdx(int nIdx);
    std::string getEventOhterDataByEventIdx(int nIdx);
    // network
    void requestInfo(const std::function<void(bool)>& pCallback);
    
    //비활성화된 이벤트를 활성화 시킬 때 사용
    int getActivateRequestCount() const;
    void AddActivateRequestCount();
protected:
    // network
    void responseInfo(cocos2d::network::HttpResponse* response,std::string &data);
    
    
private:
    int _nRequestCount;
    cocos2d::Vector<InfoActivatedEvent*> _listActivatedEvent;
    cocos2d::Vector<InfoEventOriginData*> _listEventOriginData;
   
    //
    std::function<void(bool)> _callbackInfo;
public:
    MafDelegate<void(int)> _onRequestInfo;
    MafDelegate<void()> _onEventIconShow;
};

#endif /* EventManager_h */
