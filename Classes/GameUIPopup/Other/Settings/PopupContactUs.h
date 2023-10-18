//
//  PopupContactUs.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#ifndef PopupContactUs_h
#define PopupContactUs_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupContactUs : public PopupBase
{
public:
    static PopupContactUs* create();
    
    PopupContactUs(void);
    virtual ~PopupContactUs(void);
    virtual bool init() override;
    
protected:
    
    void initVar();
    void initUi();
    
    // ui
    void uiDefaultFAQ();
    void uiDefaultContactUs();
     
    // draw
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickFAQ(cocos2d::Ref* sender);
    void onClickContactUs(cocos2d::Ref* sender);
    
    // callback
    void callbackAlarm();
    
    // game

    
private:
    struct element
    {
        std::string strTextKey;
        std::string strLink;
    };
    
    //
    std::vector<element> _list;
    
    // widget
    cocos2d::Layer* _layerContainerFAQ;
    cocos2d::Layer* _layerContainerContactUs;
};

#endif /* PopupContactUs_h */
