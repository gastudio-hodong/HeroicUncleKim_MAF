//
//  UIRaceStaus.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/26.
//

#ifndef UIRaceStaus_hpp
#define UIRaceStaus_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/EventRace/InfoEventRaceHorse.h"

class UIRaceStatus : public cocos2d::LayerColor
{
public:
    static UIRaceStatus* create(const int nIdx);
    
    UIRaceStatus(void);
    virtual ~UIRaceStatus(void);
    virtual bool init(const int nIdx);
    
protected:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiTop();
    void uiBasicStatus();
    void uiHiddenStatus();
    void uiBetting();
    
    void uiBetButtonSet(std::vector<int> listBetCount, float posY);
    //utils
    void hide();
    
    //click
    void onClickClose(cocos2d::Ref* const sender);
    void onClickBet(cocos2d::Ref* const sender);
    void onClickBetReset(cocos2d::Ref* const sender);
    void onClickBetComplete(cocos2d::Ref* const sender);
    
    //callback
    void callbackInvestment(bool bResult, int nResult);
    //event
    void onEventClickReady();
    void onEventClickStatus(const int nIdx);
    void onEventAds(int);
    void onEventInvestment(const int nIdx);
private:
    int _nIdx;
    TDouble _investCount;
    InfoEventRaceHorse* _infoHorse;
    cocos2d::Layer* _lyTop;
    cocos2d::Layer* _lyBasicStatus;
    cocos2d::Layer* _lyHiddenStatus;
    cocos2d::Layer* _lyBetting;
    
    cocos2d::ui::Button* _btnBetting;
    cocos2d::ui::Button* _btnBetReset;
    cocos2d::ui::CText* _lbBetCount;
};

#endif /* UIRaceStaus_hpp */
