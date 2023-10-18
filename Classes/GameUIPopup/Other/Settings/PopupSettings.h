//
//  PopupSettings.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#ifndef PopupSettings_h
#define PopupSettings_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupSettings : public PopupBase
{
public:
    static PopupSettings* create();
    
    PopupSettings(void);
    virtual ~PopupSettings(void);
    virtual bool init() override;
    
protected:
    
    void initVar();
    void initUi();
    
    // ui
    void uiDefaultSound();
    void uiDefaultFunction();
    void uiDefaultButton();
     
    // draw
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickAlarmSetting(cocos2d::Ref* sender);
    
    // callback
    void callbackSound(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
    void callbackFunction(cocos2d::Ref* sender, cocos2d::ui::CheckBox::EventType type);
    void callbackAlarm();
    
    // game
    std::string getFunctionText(int nTag);
    void setFunction(int nTag, bool bCheck);
    
private:
    float _soundBG;
    float _soundEF;
    
    // widget
    cocos2d::Layer* _layerContainerSound;
    cocos2d::Layer* _layerContainerFunction;
    cocos2d::Layer* _layerContainerButton;
};

#endif /* PopupSettings_h */
