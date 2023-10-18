//
//  PopupStat.h
//  FantasyClicker
//
//  Created by MAF_Mac on 07/01/2020.
//

#ifndef PopupStat_h
#define PopupStat_h

#include "GameUIPopup/Base/PopupBase.h"

#include "Common/Observer/IRefresh.h"

#include "GameUnit/UnitBehaviorHero.h"

#include "GameUIPopup/Other/Stat/Layer/LayerStatFirstPage.h"
#include "GameUIPopup/Other/Stat/Layer/LayerStatCostume.h"
#include "GameUIPopup/Other/Stat/Layer/LayerStatTreasure.h"

class PopupStat : public PopupBase, public IRefresh
{
public:
    static PopupStat* create(E_PLACE ePlace);
    
    PopupStat(void);
    virtual ~PopupStat(void);
    virtual bool init(E_PLACE ePlace);
    virtual void onEnter() override;

    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    
protected:
    // init
    void initVar();
    void initUi();
    void initBuffIcon();
    void initHero();
    void initPet();
    void initButton();
    
    void drawHero();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickButton(cocos2d::Ref* sender);
    void onClickNickChange(cocos2d::Ref* sender);
    void onClickSwitchLayer(cocos2d::Ref* sender);
    
    //
    void scheduleStatDraw(float dt);
    
    //
    void callbackNickName();
    
    
protected:
    E_PLACE _ePlace;
    
    StatFirstPage* _statNormal;
    StatFirstPage* _statDevil;
    StatCostume* _statCostume;
    StatTreasure* _statTreasure;
    
    cocos2d::Layer* _layerContainerParent;
    cocos2d::Layer* _layerContainerStatNormal;
    cocos2d::Layer* _layerContainerTreasure;
    cocos2d::Layer* _layerContainerCostume;
    cocos2d::Layer* _layerContainerStatDevil;
    
    UnitBehaviorHero* _hero;
    
    cocos2d::Sprite *_sprPrincessBuff;
    E_PRINCESS_BUFF _eSaveBuff;
    
    cocos2d::Vector<cocos2d::Animate*> _vecAnimatePrincessBuff;
    cocos2d::Vector<MafNode::MafMenuItemSprite*> _vecButton;
    cocos2d::Vector<cocos2d::Sprite*> _vecBuffIcon;
    
    cocos2d::Label* _labelNick;
    
    
};
#endif /* PopupStat_h */
