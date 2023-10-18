//
//  BadgeManager.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#ifndef BadgeManager_h
#define BadgeManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoBadge.h"

class BadgeManager : public MafSingleton<BadgeManager>
{    
public:
    BadgeManager(void);
    virtual ~BadgeManager(void);
    virtual bool init();
    
public:
    //
    void setLoad();
    
    //
    cocos2d::Vector<InfoBadge*> getListAll();
    cocos2d::Vector<InfoBadgeHave*> getListHave();
    cocos2d::Vector<InfoBadgeHave*> getListSelect();
    std::vector<std::string> getListGroup();
    
    cocos2d::Vector<InfoBadge*> getListFromGroup(std::string strGroup);
    cocos2d::Vector<InfoBadgeHave*> getListHaveFromBadgeIdx(int nBadgeIdx);
    cocos2d::Vector<InfoBadgeHave*> getListHaveFromBuffType(E_BADGE_BUFF type);
    
    void setRewardBadge(int nIdx);
    
    InfoBadge* getBadge(int nIdx);
    InfoBadgeHave* getBadgeHave(int nIdx);
    float getBadgeBuffCount(E_BADGE_BUFF type, int nBadgeIdx = 0);
    
    // network
    void requestInfo(const std::function<void(bool)>& pCallback);
    void requestUpdate(std::vector<int> listBadge, const std::function<void(bool)>& pCallback);
    
    
protected:
    // network
    void responseInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseUpdate(cocos2d::network::HttpResponse* response,std::string &data);
    
    
private:
    std::map<int, int> _listBadgeSelect;
    cocos2d::Vector<InfoBadge*> _listBadgeAll;
    cocos2d::Vector<InfoBadgeHave*> _listBadgeHave;
  
   
    //
    std::function<void(bool)> _callbackInfo;
    std::function<void(bool)> _callbackUpdate;
};

#endif /* BadgeManager_h */
