//
//  PopupEventSunfishAdventureResult.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef PopupEventSunfishAdventureResult_h
#define PopupEventSunfishAdventureResult_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupEventSunfishAdventureResult : public PopupBase
{
public:
    CREATE_FUNC(PopupEventSunfishAdventureResult);

    PopupEventSunfishAdventureResult(void);
    virtual ~PopupEventSunfishAdventureResult(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    // set, get
    void setLevelIdx(int idx);
    void setAdventureIdx(int idx);
    void setResult(bool bResult);
    
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
    
    // callback
    
    // schedule
    
    //

private:
    bool _bInitUI;
    
    //
    bool _bResult;
    TInteger _levelIdx;
    TInteger _adventureIdx;
    
    
    // widget
    cocos2d::ui::Layout* _uiContentsInfo;
    cocos2d::ui::Layout* _uiContentsButton;

    cocos2d::ui::CText* _uiTitle;
    
};
#endif /* PopupEventSunfishAdventureResult_h */
