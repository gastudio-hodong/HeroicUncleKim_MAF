//
//  InfoAdventureRelicCollect.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/12.
//

#ifndef InfoAdventureRelicCollect_hpp
#define InfoAdventureRelicCollect_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoItems;
class InfoAdventurePropertyType;
class InfoAdventureRelicCollect : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoAdventureRelicCollect);
    InfoAdventureRelicCollect();
    virtual ~InfoAdventureRelicCollect();
    virtual bool init();
    
public:
    const bool isActive() const;
    void setIsActive(const bool value);
    const int getIdx() const;
    void setIdx(const int value);
    
    const E_ADVENTURE_PROPERTY_TYPE getType() const;
    void setType(const E_ADVENTURE_PROPERTY_TYPE value);
    
    InfoAdventurePropertyType* const getInfoType() const;
    
    void setListCondition(const std::vector<std::pair<int,int>> list);
    const std::vector<std::pair<int,int>> getListCondition() const;
    const std::pair<int,int> getCondition(int idx) const;
    
    const double getAbilityEffect();
    void setAbilityEffect(const double value);
    
    const cocos2d::Vector<InfoItems*>& getListReward() const;
    void setListReward(const cocos2d::Vector<InfoItems*> list);
    
    const bool isReward() const;
private:
    bool _isActive;
    int _nIdx;
    TDouble _dAvlilityEffect;
    
    std::vector<std::pair<int,int>> _listCondition;
    
    E_ADVENTURE_PROPERTY_TYPE _eType;
    cocos2d::Vector<InfoItems*> _listReward;
};


#endif /* InfoAdventureRelicCollect_hpp */
