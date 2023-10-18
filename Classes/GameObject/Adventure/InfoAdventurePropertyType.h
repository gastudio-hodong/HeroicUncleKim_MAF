//
//  InfoAdventurePropertyType.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/20.
//

#ifndef InfoAdventurePropertyType_hpp
#define InfoAdventurePropertyType_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoAdventurePropertyType : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoAdventurePropertyType);
    InfoAdventurePropertyType();
    virtual ~InfoAdventurePropertyType();
    virtual bool init();
    
public:
    const int getIdx() const;
    void setIdx(const int value);
    const E_ADVENTURE_PROPERTY_TYPE getType() const;
    
    const bool isPartyOption() const;
    void setIsPartyOption(const bool value);
    
    const bool isRewardOption() const;
    void setIsRewardOption(const bool value);
private:
    int _nIdx;
    bool _isPartyOption;
    bool _isRewardOption;
};


#endif /* InfoAdventurePropertyType_hpp */
