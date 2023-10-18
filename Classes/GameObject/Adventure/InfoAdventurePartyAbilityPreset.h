//
//  InfoAdventurePartyAbilityPreset.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/23.
//

#ifndef InfoAdventurePartyAbilityPreset_hpp
#define InfoAdventurePartyAbilityPreset_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoAdventurePartyAbility;
class InfoAdventurePartyAbilityPreset : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoAdventurePartyAbilityPreset);
    InfoAdventurePartyAbilityPreset();
    virtual ~InfoAdventurePartyAbilityPreset();
    virtual bool init();
    
public:
    const int getIdx() const;
    void setIdx(const int value);
    
    const cocos2d::Vector<InfoAdventurePartyAbility*>& getListAblity() const;
    void setListAblity(const cocos2d::Vector<InfoAdventurePartyAbility*> value);
    
    const std::vector<bool> getListSeal() const;
    void setListSeal(const std::vector<bool> value);
    
    const int getAbilityOpenCount() const;
    const int getSealCount() const;
    
    void checkAvilityError();
private:
    int _nIdx;
    
    cocos2d::Vector<InfoAdventurePartyAbility*> _listAbility;
    std::vector<bool> _listSeal;
};

#endif /* InfoAdventurePartyAbilityPreset_hpp */
