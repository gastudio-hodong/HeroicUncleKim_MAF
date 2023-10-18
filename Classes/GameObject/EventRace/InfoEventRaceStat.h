//
//  InfoEventRaceStat.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/22.
//

#ifndef InfoEventRaceStat_hpp
#define InfoEventRaceStat_hpp

#include "Common/ConfigDefault.h"

class InfoEventRaceStat : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoEventRaceStat);
    InfoEventRaceStat();
    virtual ~InfoEventRaceStat();
    virtual bool init();
    
public:
    const bool isHidden() const;
    void setIsHidden(const bool isHidden);
    
    const bool isAbilityActive() const;
    void setIsAbilityActive(const bool isActive);
    
    const int getIdx() const;
    void setIdx(const int value);
    
    const int getGrade();
    void setGrade(const int value);
    
    const double getValue();
    void setValue(const double value);
    
    const std::string getTypeName() const;
    void setTypeName(const std::string value);
    
    const std::string getTendency() const;
    void setTendency(const std::string value);
    
    const std::string getText();
    void setTextKey(const std::string value);
protected:
    bool _isHidden;
    bool _isAbilityActive;
    int _nIdx;
    TInteger _nGrade;
    TDouble _dValue;
    std::string _strTypeName;
    std::string _strTendency;
    std::string _strTextKey;
};


#endif /* InfoEventRaceStat_hpp */
