//
//  UIRaceNumber.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/26.
//

#ifndef UIRaceNumber_hpp
#define UIRaceNumber_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/EventRace/InfoEventRaceHorse.h"

class UIRaceNumber : public cocos2d::LayerColor
{
public:
    static UIRaceNumber* create(const int nIdx, const bool isCurrent);
    
    UIRaceNumber(void);
    virtual ~UIRaceNumber(void);
    virtual bool init(const int nIdx, const bool isCurren);
    
protected:
    // init
    void initVar();
    void initUI();
    
    //utils
    void hide();
    
    void onClick(cocos2d::Ref* sender);
    //event
    void onEventRaceRaedy();
    void onEventClickStart();
    void onEventClickStatus(const int nIdx);
    void onEvnetInvestment(const int nIdx);
    
private:
    bool _isCurrent;
    int _nIdx;
    InfoEventRaceHorse* _infoHorse;
    cocos2d::Sprite* _sprCoinIcon;
};

#endif /* UIRaceNumber_hpp */
