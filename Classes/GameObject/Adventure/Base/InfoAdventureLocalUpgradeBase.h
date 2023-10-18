//
//  InfoAdventureLocalUpgradeBase.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/06/10.
//

#ifndef InfoAdventureLocalUpgradeBase_hpp
#define InfoAdventureLocalUpgradeBase_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoAdventureUpgradeAbility;
class InfoAdventurePropertyType;
class InfoAdventureLocalUpgradeBase : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoAdventureLocalUpgradeBase);
    InfoAdventureLocalUpgradeBase();
    virtual ~InfoAdventureLocalUpgradeBase();
    virtual bool init();
    
public:
    virtual const int getIdx() const;
    virtual void setIdx(const int value);
    
    virtual const E_ADVENTURE_PROPERTY_TYPE getType() const;
    virtual void setType(const E_ADVENTURE_PROPERTY_TYPE value);
    virtual InfoAdventurePropertyType* const getInfoType(int nLv = -1) const;
    
    virtual const int getCurrentLv() const;
    virtual void setCurrentLv(const int value);
    virtual const int getMaxLv() const;
    virtual const int getImageGrade() const;
    
    virtual InfoAdventureUpgradeAbility* const getInfoUpgradeAbility(int nLv = -1) const;
    virtual void setInfoUpgradeAbility(const cocos2d::Vector<InfoAdventureUpgradeAbility*> value);
    
    virtual const int requestUpgrade();
    void completeUpgrade();
protected:
    bool _isDispatch;
    int _nIdx;
    int _nCurrentLv;
    E_ADVENTURE_PROPERTY_TYPE _nPropertyType;
    
    cocos2d::Vector<InfoAdventureUpgradeAbility*> _listProperty;
};

#endif /* InfoAdventureLocalUpgradeBase_hpp */
