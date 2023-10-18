//
//  InfoAdventureCore.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/09.
//

#ifndef InfoAdventureCore_hpp
#define InfoAdventureCore_hpp

#include "Common/ConfigDefault.h"

class InfoAdventureCore : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoAdventureCore);
    InfoAdventureCore();
    virtual ~InfoAdventureCore();
    virtual bool init();
    
public:
    const bool isActive() const;
    void setIsActice(const bool value);
    const int getIdx() const;
    void setIdx(const int value);
    const int getCurrentLv();
    void setCurrentLv(const int value);
    const int getExp();
    void setExp(const int value);
    const int getCount();
    void setCount(const int value);
    
private:
    bool _isActive;
    int _nIdx;
    TInteger _nCurrentLv;
    TInteger _nExp;
    TInteger _nCount;
};
#endif /* InfoAdventureCore_hpp */
