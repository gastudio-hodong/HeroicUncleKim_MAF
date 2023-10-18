//
//  InfoAdventureFriendAbility.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/16.
//

#ifndef InfoAdventureFriendAbility_hpp
#define InfoAdventureFriendAbility_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoAdventurePropertyType;
class InfoAdventureStage;
class InfoAdventureFriendAbility : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoAdventureFriendAbility);
    InfoAdventureFriendAbility();
    virtual ~InfoAdventureFriendAbility();
    virtual bool init();
    
public:
    const int getIdx() const;
    void setIdx(const int value);
    
    const int getMinLv() const;
    void setMinLv(const int value);
    
    const int getAdventurePoint() const;
    void setAdventurePoint(const int value);
    
    InfoAdventurePropertyType* const getInfoType(E_ADVENTURE_PROPERTY_TYPE eType) const;
    
    void setListEffect(const std::vector<std::pair<E_ADVENTURE_PROPERTY_TYPE, TDouble>> list);
    const std::vector<std::pair<E_ADVENTURE_PROPERTY_TYPE, TDouble>> getListEffect() const;
    
    const double getAbilityEffect(E_ADVENTURE_PROPERTY_TYPE eType);
private:
    int _nIdx;
    int _nLv;
    int _nAdventurePoint;
    
    std::vector<std::pair<E_ADVENTURE_PROPERTY_TYPE, TDouble>> _listEffect;
};


class InfoAdventureFriend : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoAdventureFriend);
    InfoAdventureFriend();
    virtual ~InfoAdventureFriend();
    virtual bool init();
    
public:
    const bool isAdventure() const;
    void setIsAdventure(const bool value);
    
    const int getUseridx() const;
    void setUseridx(const int value);
    void setUseridxFromPartyIdx(const int value);
    const int getPartyIdx() const;
    
    const int getLevel() const;
    void setLevel(const int value);
    
    const int getGrade() const;
    
    const std::string getPlatform() const;
    void setPlatform(const std::string value);
    
    const std::string getNickName() const;
    void setNickName(const std::string value);
    
    InfoAdventureStage* const getInfoStage() const;
    void setInfoStage(InfoAdventureStage* const value);
    
    const double getAbilityEffect(E_ADVENTURE_PROPERTY_TYPE eType);
    const std::vector<std::pair<E_ADVENTURE_PROPERTY_TYPE, TDouble>> getListEffect() const;
private:
    bool _isAdventure;
    int _nUseridx;
    int _nLevel;
    std::string _strPlatform;
    std::string _strNickName;
    InfoAdventureStage* _infoStage;
};
#endif /* InfoAdventureFriendAbility_hpp */
