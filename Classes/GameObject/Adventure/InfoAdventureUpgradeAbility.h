//
//  InfoAdventureProperty.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/18.
//

#ifndef InfoAdventureProperty_hpp
#define InfoAdventureProperty_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoItems;
class InfoAdventurePropertyType;
class InfoAdventureUpgradeAbility : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoAdventureUpgradeAbility);
    InfoAdventureUpgradeAbility();
    virtual ~InfoAdventureUpgradeAbility();
    virtual bool init();
    
public:
    const int getLv() const;
    void setLv(const int value);
    
    const double getEffect() ;
    void setEffect(const double value);
    
    InfoAdventurePropertyType* const getInfoType();
    
    const E_ADVENTURE_PROPERTY_TYPE getType() const;
    void setType(const E_ADVENTURE_PROPERTY_TYPE value);
    
    const cocos2d::Vector<InfoItems*>& getListCost() const;
    InfoItems* const getInfoItems(const int nIdx) const;
    void setCost(const cocos2d::Vector<InfoItems*> value);
    
    const bool isPartyOption();
    
    const bool isUpgradeConditionMine() const;
    const bool isUpgradeConditionCurrency(E_ITEMS eType) const;
    const bool isUpgradeCondition() const;
    const int getUpgradeCost(const int nIdx) const;
    const bool tryUpgradePayment();
private:
    int _nLv;
    E_ADVENTURE_PROPERTY_TYPE _eType;
    TDouble _dEffect;
    cocos2d::Vector<InfoItems*> _listCost;
};

#endif /* InfoAdventureProperty_hpp */
