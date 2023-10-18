//
//  AdventureRelicManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/04.
//

#ifndef AdventureRelicManager_hpp
#define AdventureRelicManager_hpp

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoBongContentLevel.h"
#include "GameObject/Adventure/InfoAdventurePartyAbilityOrigin.h"
#include "GameObject/Adventure/InfoAdventurePartyAbilityGradeRate.h"
#include "GameObject/Adventure/InfoAdventureFurniture.h"
#include "GameObject/Adventure/InfoAdventureParty.h"
#include "GameObject/Adventure/InfoAdventurePropertyType.h"
#include "GameObject/Adventure/InfoAdventureMap.h"
#include "GameObject/Adventure/InfoAdventureStage.h"
#include "GameObject/Adventure/InfoAdventureRelic.h"
#include "GameObject/Adventure/InfoAdventureRelicCollect.h"
#include "GameObject/Adventure/InfoAdventureCore.h"

class AdventureRelicManager : public MafSingleton<AdventureRelicManager>
{
private:
    AdventureRelicManager(void);
    virtual ~AdventureRelicManager(void);
    friend class MafSingleton;
    
public:
    virtual bool init();
    
    const cocos2d::Vector<InfoAdventureRelic*>& getListInfoRelic() const;
    const cocos2d::Vector<InfoAdventureRelic*> getListInfoRelicFromArea(const int nArea) const;
    InfoAdventureRelic* const getInfoRelic(const int nIdx) const;
    
    const cocos2d::Vector<InfoAdventureCore*>& getListInfoCore() const;
    InfoAdventureCore* const getInfoCore(const int nIdx) const;
    
    InfoBongContentLevel* const getInfoCoreLevelOrigin(const int nLv) const;
    InfoBongContentLevel* const getInfoRelicLevel(const int nLv) const;
    
    const cocos2d::Vector<InfoAdventureRelicCollect*> getListCollect()const;
    InfoAdventureRelicCollect* const getInfoCollect(const int nIdx) const;
    
    const int getCoreMaxLv();
    const int getRelicMaxLv();
    static const int getCraftPrice(const int nIdx, const int nCount);
    const int getRelicLevelUpPrice(const int nIdx, const int nCount);
    const int getRelicLevelupMax(const int nIdx);
    const double getRelicCollectEffect(const E_ADVENTURE_PROPERTY_TYPE eType);
    const int getTotalRelicLvFromArea(const int nArea) const;
    const int getTotalRelicLv() const;
    const double getRelicTotalEffectCoefficient(const E_ADVENTURE_PROPERTY_TYPE eType) const;
    const double getRelicTotalEffect(const E_ADVENTURE_PROPERTY_TYPE eType) const;
    const double getRelicEffectBuffCalculator(const E_ADVENTURE_PROPERTY_CALCULATE_TYPE eType) const;
    const bool isRelicReddotCondition() const;
    const bool isRelicCollectReddotCondition() const;
    const bool isRelicReddotConditionFromArea(const int nArea) const;
    const int getCountActiveAreaCore() const;
    const bool isActivateRelicEffect(const E_ADVENTURE_PROPERTY_TYPE eType) const;
    
    //load table
    void setLoadAll();
    void setLoadAdventureRelic();
    void setLoadCoreLevelOrigin();
    void setLoadRelicLevel();
    void setLoadRelicCollect();
    
    //network : request
    void requestInfoCore(const std::function<void(bool, int)>& callback);
    void requestInfoRelic(const std::function<void(bool, int)>& callback);
    void requestCraftRelic(const std::function<void(bool, int, cocos2d::Vector<InfoItems*>)>& callback, int nCoreIdx, int nCount);
    void requestRewardRelicCollect(const std::function<void(bool, int)>& callback, int nIdx);
    void requestInfoRelicCollect(const std::function<void(bool, int)>& callback);
    
    void requestRelicLevelUp(const std::function<void(bool, int)>& callback, int nIdx, int nLv);
    void requestRelicGrind(const std::function<void(bool, int)>& callback, int nIdx, int nCount);
protected:
    // network : respons
    void responseInfoCore(cocos2d::network::HttpResponse* const response, const std::string &data);
    void responseInfoRelic(cocos2d::network::HttpResponse* const response, const std::string &data);
    void responseCraftRelic(cocos2d::network::HttpResponse* const response, const std::string &data);
    void responseRewardRelicCollect(cocos2d::network::HttpResponse* const response, const std::string &data);
    void responseInfoRelicCollect(cocos2d::network::HttpResponse* const response, const std::string &data);
    void responseRelicLevelUp(cocos2d::network::HttpResponse* const response, const std::string &data, const int nIdx);
    void responseRelicGrind(cocos2d::network::HttpResponse* const response, const std::string &data);
    
    void setInitCore();
protected:
    cocos2d::Vector<InfoAdventureRelic*> _listInfoRelic;
    cocos2d::Vector<InfoAdventureCore*> _listInfoCore;
    cocos2d::Vector<InfoBongContentLevel*> _listInfoCoreLevelOrigin;
    cocos2d::Vector<InfoBongContentLevel*> _listInfoRelicLevel;
    cocos2d::Vector<InfoAdventureRelicCollect*> _listInfoCollect;
    
    std::function<void(bool, int)> _callbackInfoCore;
    std::function<void(bool, int)> _callbackInfoRelic;
    std::function<void(bool, int)> _callbackInfoRelicCollect;
    std::function<void(bool, int)> _callbackRewardRelicCollect;
    std::function<void(bool, int)> _callbackRewardRelicLevelUp;
    std::function<void(bool, int)> _callbackRewardRelicGrind;
    std::function<void(bool, int, cocos2d::Vector<InfoItems*>)> _callbackCraftRelic;
};

#endif /* AdventureRelicManager_hpp */
