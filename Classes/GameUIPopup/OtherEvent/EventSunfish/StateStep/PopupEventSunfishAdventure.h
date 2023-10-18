//
//  PopupEventSunfishAdventure.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef PopupEventSunfishAdventure_h
#define PopupEventSunfishAdventure_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupEventSunfishAdventure : public PopupBase
{
public:
    enum class E_STEP {
        USE,
        START,
    };
    
public:
    CREATE_FUNC(PopupEventSunfishAdventure);

    PopupEventSunfishAdventure(void);
    virtual ~PopupEventSunfishAdventure(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    // set, get
    void setLevelIdx(int idx);
    
    // event
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui : defalut
    void uiContainer();
    void uiInfo();
    void uiButtons();
    
    // ui : draw
    void drawInfo();
    void drawButtons();
    
    // ui : action
    
    // ui : click
    void onClickClose(Ref* sender);
    void onClickAD(Ref* sender);
    void onClickUse(Ref* sender);
    void onClickStart(Ref* sender);
    
    // callback
    void callbackAdventureUse(bool bResult, int nResult, int adventureIdx);
    void callbackAdventureStart(bool bResult, int nResult, bool bSuccess);
    void callbackADResult(ADS_RESULT result);
    void callbackAD(bool bResult, int nResult);
    
    // schedule
    
    //

private:
    bool _bInitUI;
    
    //
    E_STEP _eStep;
    TInteger _levelIdx;
    TInteger _adventureIdx;
    
    // widget
    cocos2d::ui::Layout* _uiContentsInfo;
    cocos2d::ui::Layout* _uiContentsButton;
    
    cocos2d::ui::CText* _uiTitle;
};
#endif /* PopupEventSunfishAdventure_h */
