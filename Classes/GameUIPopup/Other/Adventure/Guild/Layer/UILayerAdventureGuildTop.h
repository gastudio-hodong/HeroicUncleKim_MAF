//
//  UILayerAdventureGuildTop.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/15.
//

#ifndef UILayerAdventureGuildTop_hpp
#define UILayerAdventureGuildTop_hpp

#include "GameUIPopup/Other/Adventure/Layer/UILayerAdventureParent.h"

#include "Common/Observer/IRefresh.h"

#include "GameObject/Raid/InfoRaidBongStat.h"

class UILayerAdventureGuildTop : public UILayerAdventureParent, public IRefresh
{
private:
    struct tagDecorateInfo
    {
        cocos2d::Vec2 pos;
        int nZorder;
    };
public:
    static UILayerAdventureGuildTop* create(const cocos2d::Size size);
    
    UILayerAdventureGuildTop(void);
    virtual ~UILayerAdventureGuildTop(void);
    virtual bool init(const cocos2d::Size size);
    
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData)  override;
    virtual void refreshUI() override;
public:
    
    void setCallbackHide(const std::function<void(void)>& callback);
    
    void showButtonForceTutorial();
    void showTutorial();
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiTopUI();
    void uiDecorative();
    void uiMember();
    void uiManagerCat();
    void uiFurniture();
    void uiGummie();
    void uiCurrency();
    void uiButton();
    
    // click
    void onClickHide(Ref* const pSender);
    void onClickHelp(Ref* const pSender);
    void onClickBoss(Ref* const pSender);
    void onClickRelic(Ref* const pSender);
    void onClickOfficeUpgrade(Ref* const pSender);
    void onClickPartyUpgrade(Ref* const pSender);
    void onClickGummie(Ref* const pSender);
    void onClickMission(Ref* const pSender);
    
    //callback
    void callbackGummie(bool bResult, int nResult);
private:
    
    cocos2d::Layer* _lyUI;
    cocos2d::Layer* _lyDecorative;
    cocos2d::Layer* _lyButton;
    
    cocos2d::Sprite* _sprManagerCat;
    cocos2d::ui::Button* _btnTutorial;
    
    std::function<void(void)> _callbackHide;
};


#endif /* UILayerAdventureGuildTop_hpp */
