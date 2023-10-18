//
//  PopupEventSunfishLevel.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef PopupEventSunfishLevel_h
#define PopupEventSunfishLevel_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupEventSunfishLevel : public PopupBase
{
public:
    CREATE_FUNC(PopupEventSunfishLevel);

    PopupEventSunfishLevel(void);
    virtual ~PopupEventSunfishLevel(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    // set, get
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
    
    // callback
    void callbackCheck(bool bResult, int nResult, bool bSuccess);
    
    // schedule
    
    //

private:
    bool _bInitUI;
    
    //
    TInteger _levelIdx;
    
    // widget
    cocos2d::ui::Layout* _uiContentsInfo;
    cocos2d::ui::Layout* _uiContentsButton;

    cocos2d::ui::CText* _uiTitle;
    
};
#endif /* PopupEventSunfishLevel_h */
