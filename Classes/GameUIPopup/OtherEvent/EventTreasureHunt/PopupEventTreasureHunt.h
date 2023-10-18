//
//  PopupEventTreasureHunt.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#ifndef PopupEventTreasureHunt_hpp
#define PopupEventTreasureHunt_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupEventTreasureHunt : public PopupBase
{
public:
    static PopupEventTreasureHunt* create();
    
    PopupEventTreasureHunt();
    virtual ~PopupEventTreasureHunt(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
protected:
    void initVar();
    void initUI();
    
    //ui
    void uiContainer();
    void uiInfo();
    void uiBoards();
    void uiRewards();
    
    // ui : draw
    void drawInfo();
    void drawBoards();
    void drawRewards();
    
    //
    void setUnitState(E_HERO_STATE eState);
    
    //
    void onClickClose(Ref* sender);
    void onClickHelp(Ref* sender);
    void onClickBoard(Ref* sender);
    void onClickRefresh(Ref* sender);
    void onClickCompass(Ref* sender);
    void onClickMissioinRewardSo(Ref* sender);
    void onClickMissioinRewardCo(Ref* sender);
    void onClickAD(Ref* sender);
    
    //callback
    void callbackInfo(bool bResult, int nResult);
    void callbackRefresh(bool bResult, int nResult);
    void callbackCompass(bool bResult, int nResult);
    void callbackBoard(bool bResult, int nResult);
    void callbackMissionReward(bool bResult, int nResult);
    void callbackADResult(ADS_RESULT result);
    void callbackAD(bool bResult, int nResult);
     
private:
    bool _bInitUI;    
    
    //
    cocos2d::ui::Layout* _uiContentsInfo;
    cocos2d::ui::Layout* _uiContentsBoards;
    cocos2d::ui::Layout* _uiContentsRewards;
    
    //
    cocos2d::Sprite* _unitPet;
};

#endif /* PopupEventTreasureHunt_hpp */
