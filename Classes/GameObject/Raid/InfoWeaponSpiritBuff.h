//
//  InfoWeaponSpiritBuff.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/01/24.
//

#ifndef InfoWeaponSpiritBuff_h
#define InfoWeaponSpiritBuff_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoWeaponSpiritBuff : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoWeaponSpiritBuff);
    
    InfoWeaponSpiritBuff();
    virtual ~InfoWeaponSpiritBuff();
    virtual bool init();
    
    
public:
    E_SPIRIT_BUFF_TYPE getType();
    void setType(E_SPIRIT_BUFF_TYPE value);
    
    int getIdx();
    void setIdx(int value);
    
    int getIncreases();
    void setIncreases(int value);
    
    int getCoolTime();
    void setCoolTime(int value);
    
    int getDurationTime();
    void setDurationTime(int value);
    
    cocos2d::Color4B getEffectColor();
    void setEffectColor(cocos2d::Color4B value);
    
private:
    E_SPIRIT_BUFF_TYPE _eType;
    int _nIdx;
    TInteger _nIncreases;
    TInteger _nCoolTime;
    TInteger _nDurationTime;
    cocos2d::Color4B _effectColor;
};


#endif /* InfoWeaponSpiritBuff_hpp */
