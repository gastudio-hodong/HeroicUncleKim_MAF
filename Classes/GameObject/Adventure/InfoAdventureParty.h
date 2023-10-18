//
//  InfoAdventureParty.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/18.
//

#ifndef InfoAdventureParty_hpp
#define InfoAdventureParty_hpp

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoAdventureMember;
class InfoAdventureStage;
class InfoAdventurePartyAbility;
class InfoAdventurePartyAbilityPreset;
class InfoAdventureParty : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoAdventureParty);
    InfoAdventureParty();
    virtual ~InfoAdventureParty();
    virtual bool init();
    
public:
    const int getIdx() const;
    void setIdx(const int value);
    
    const int getPresetIdx() const;
    void setPresetIdx(const int value);
    
    InfoAdventureStage* const getInfoStage() const;
    void setInfoStage(InfoAdventureStage* const value);
    
    void resetStageData();
    
    const int getArea() const;
    
    const bool isRunning() const;
    
    const int getStage() const;
    
    const int getEndTime() const;
    const int getExp() const;
    
    const cocos2d::Vector<InfoAdventureMember*>& getListMemeber() const;
    
    InfoAdventureMember* const getMember(const int nIdx) const;
    void setMember(const cocos2d::Vector<InfoAdventureMember*> value);
    void pushBackMember(InfoAdventureMember* const value);
    
    const cocos2d::Vector<InfoItems*> getReward() const;
    const cocos2d::Vector<InfoItems*> getRewardBoss() const;
    
    InfoAdventurePartyAbilityPreset* const getAblityPreset(int nPresetIdx = -1) const;
    void setListAblityPreset(const cocos2d::Vector<InfoAdventurePartyAbilityPreset*> value);
    const cocos2d::Vector<InfoAdventurePartyAbilityPreset*>& getListAblityPreset() const;
    const cocos2d::Vector<InfoAdventurePartyAbility*> getListAblity(int nPresetIdx = -1) const;
    
    const std::vector<bool> getListSeal(int nPresetIdx = -1) const;
    const int getAbilityOpenCount(int nPresetIdx = -1) const;
    const int getSealCount(int nPresetIdx = -1) const;
    
    const bool isOpen() const;
    
    void checkPresetError();
    void checkLocalCompleteUpgrade();
private:
    int _nIdx;
    int _nPresetIdx;
    InfoAdventureStage* _infoStage;
    
    cocos2d::Vector<InfoAdventureMember*> _listMember;
    
    cocos2d::Vector<InfoAdventurePartyAbilityPreset*> _listAbilityPreset;
};
#endif /* InfoAdventureParty_hpp */
