//
//  Event7thManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF on 2022/05/12.
//

#ifndef Event7thManager_h
#define Event7thManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/Event/InfoEvent7th.h"
#include "GameObject/Event/InfoEvent7thPerson.h"

class Event7thManager : public MafSingleton<Event7thManager>
{
public:
    Event7thManager(void);
    virtual ~Event7thManager(void);
    virtual bool init();
    
public:
    // load
    void setLoad();
    void setLoadPerson();
    
    // get, set : 협동
    cocos2d::Vector<InfoEvent7th*> getListCoopInfo(int nType);
    cocos2d::Vector<InfoEvent7th*> getListCoopInfo();
    
    int getCoopCount(int nType);
    int getCoopLevel(int nType);
    int getCoopPersonalCount(int nType);
    
    // get, set : 개인
    cocos2d::Vector<InfoEvent7thPerson*> getPersonalInfo();
    InfoEvent7thPerson* getPersonalInfoCurrent();
    
    int getPersonalCount();
    int getPersonalLevel();
    
    // network : request
    void requestInfo(const std::function<void(bool, int)>& callback);
    void requestUpdate(int nType, const std::function<void(bool, int)>& callback);
    void requestReward(int nType, int nLevel, const std::function<void(bool, int, std::string)>& callback);
    
     void onEventActivate(int nIdx);
protected:
    // network : respons
    void responseInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseUpdate(cocos2d::network::HttpResponse* response,std::string &data);
    void responseReward(cocos2d::network::HttpResponse* response,std::string &data);
    
private:
    bool _isEventActivate;
    // 개인
    TInteger _personalInfo;
    TInteger _personalLevel;
    
    // 협동
    std::map<int, int> _listCoopInfo;
    std::map<int, int> _listCoopLevel;
    std::map<int, int> _listCoopPersonalCount;
    
    
    cocos2d::Vector<InfoEvent7th*> _listEvent7th;
    cocos2d::Vector<InfoEvent7thPerson*> _listEvent7thPerson;
    
    
    // callback
    std::function<void(bool, int)> _callbackInfo;
    std::function<void(bool, int)> _callbackUpdate;
    std::function<void(bool, int, std::string)> _callbackReward;
    
    
};


#endif /* Event7thManager_hpp */
