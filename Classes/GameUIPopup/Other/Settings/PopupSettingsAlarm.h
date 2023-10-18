//
//  PopupSettingsAlarm.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#ifndef PopupSettingsAlarm_h
#define PopupSettingsAlarm_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupSettingsAlarm : public PopupBase
{
public:
    static PopupSettingsAlarm* create();
    
    PopupSettingsAlarm(void);
    virtual ~PopupSettingsAlarm(void);
    virtual bool init() override;
    
public:
    void setCallbackResult(const std::function<void(void)>& callback);
    
protected:
    
    void initVar();
    void initUi();
    
    // ui
    void uiDefaultAll();
    void uiDefaultAlarm();
     
    // draw
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    
    // callback
    void callbackFunctionAll(cocos2d::Ref* sender, cocos2d::ui::CheckBox::EventType type);
    void callbackFunction(cocos2d::Ref* sender, cocos2d::ui::CheckBox::EventType type);
    
    // game
    std::string getAlarmText(int nTag);
    void setAlarm(int nTag, bool bCheck);
    
private:
    std::function<void(void)> _callbackResult;

    // widget
    cocos2d::Layer* _layerContainerAll;
    cocos2d::Layer* _layerContainerAlarm;
};

#endif /* PopupSettingsAlarm_h */
