//
//  InfoAdventureRelic.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/04.
//

#ifndef InfoAdventureRelic_hpp
#define InfoAdventureRelic_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoItems;
class InfoAdventurePropertyType;
class InfoAdventureRelic : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoAdventureRelic);
    InfoAdventureRelic();
    virtual ~InfoAdventureRelic();
    virtual bool init();
    
public:
    const bool isActive() const;
    void setIsActive(const bool value);
    const int getIdx() const;
    void setIdx(const int value);
    const int getArea() const;
    void setArea(const int value);
    const int getGrade() const;
    void setGrade(const int value);
    const int getCount() const;
    void setCount(const int value);
    
    const E_ADVENTURE_PROPERTY_TYPE getType() const;
    void setType(const E_ADVENTURE_PROPERTY_TYPE value);
    
    InfoAdventurePropertyType* const getInfoType() const;
    
    const int getCurrentLv();
    void setCurrentLv(const int value);
    
    const double getAbilityEffect(int nLv = -1);
    void setAbilityEffectDefault(const double value);
    
    void setAbilityEffectIncrease(const double value);
    
    const cocos2d::Vector<InfoItems*>& getListGrindReward() const;
    void setListGrindReward(const cocos2d::Vector<InfoItems*> list);
    
    const bool isReddotCondition();
private:
    bool _isActive;
    int _nIdx;
    int _nArea;
    int _nGrade;
    int _nCount;
    TInteger _nCurrentLv;
    TDouble _dAvlilityDefault;
    TDouble _dAvlilityIncrease;
    
    
    E_ADVENTURE_PROPERTY_TYPE _eType;
    cocos2d::Vector<InfoItems*> _listGrindReward;
};

#endif /* InfoAdventureRelic_hpp */
