//
//  PopupEventSunfishLevelResult.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef PopupEventSunfishLevelResult_h
#define PopupEventSunfishLevelResult_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupEventSunfishLevelResult : public PopupBase
{
public:
    CREATE_FUNC(PopupEventSunfishLevelResult);

    PopupEventSunfishLevelResult(void);
    virtual ~PopupEventSunfishLevelResult(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    // set, get
    void setLevelIdx(int idx);
    void setResult(bool bResult);
    
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
    void onClickClose(Ref* sender);
    
    // callback
    
    // schedule
    
    //

private:
    bool _bInitUI;
    
    //
    bool _bResult;
    TInteger _levelIdx;
    
    // widget
    cocos2d::ui::Layout* _uiContentsInfo;
    cocos2d::ui::Layout* _uiContentsButton;

    cocos2d::ui::CText* _uiTitle;
    
};
#endif /* PopupEventSunfishLevelResult_h */
