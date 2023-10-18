//
//  PopupEventSunfish.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#ifndef PopupEventSunfish_hpp
#define PopupEventSunfish_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupEventSunfish : public PopupBase
{
public:
    static PopupEventSunfish* create();
    
    PopupEventSunfish();
    virtual ~PopupEventSunfish(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
protected:
    void initVar();
    void initUI();
    
    //ui
    void uiContainer();
    void uiInfo();
    void uiReward();
    
    // ui : draw
    void drawInfo();
    void drawReward();
    
    //
    
    //
    void onClickClose(Ref* sender);
    void onClickHelp(Ref* sender);
    void onClickAdventure(Ref* sender);
    void onClickReinforce(Ref* sender);
    void onClickDex(Ref* sender);
    void onClickReward(Ref* sender);
    
    //callback
    void callbackInfo(bool bResult, int nResult);
    void callbackReward(bool bResult, int nResult);
    void callbackInfoUpdate();
     
private:
    bool _bInitUI;    
    
    //
    cocos2d::ui::Layout* _uiContentsInfo;
    cocos2d::ui::Layout* _uiContentsGame;
    cocos2d::ui::Layout* _uiContentsReward;
    
    cocos2d::ui::CText* _uiTime;
    cocos2d::ui::CText* _uiLife;
    cocos2d::ui::CText* _uiWeight;
};

#endif /* PopupEventSunfish_hpp */
