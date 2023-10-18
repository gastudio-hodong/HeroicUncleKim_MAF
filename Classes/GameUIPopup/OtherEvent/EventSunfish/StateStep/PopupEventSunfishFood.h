//
//  PopupEventSunfishFood.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef PopupEventSunfishFood_h
#define PopupEventSunfishFood_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupEventSunfishFood : public PopupBase
{
public:
    enum class E_STEP {
        CHECK,
        RESULT,
    };
    
public:
    CREATE_FUNC(PopupEventSunfishFood);

    PopupEventSunfishFood(void);
    virtual ~PopupEventSunfishFood(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    // set, get
    void setFoodIdx(int idx);
    void setLevelIdx(int idx);
    
    //
    
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
    void onClickCheck(Ref* sender);
    void onClickConfirm(Ref* sender);
    
    // callback
    void callbackCheck(bool bResult, int nResult, bool bSuccess);
    
    // schedule
    
    //

private:
    bool _bInitUI;
    
    //
    E_STEP _eStep;
    TInteger _foodIdx;
    TInteger _levelIdx;
    
    // widget
    cocos2d::ui::Layout* _uiContentsInfo;
    cocos2d::ui::Layout* _uiContentsButton;

    cocos2d::ui::CText* _uiTitle;
    
};
#endif /* PopupEventSunfishFood_h */
