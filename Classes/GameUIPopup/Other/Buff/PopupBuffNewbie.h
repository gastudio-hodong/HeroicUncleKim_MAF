//
//  PopupBuffNewbie.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupBuffNewbie_h
#define PopupBuffNewbie_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupBuffNewbie : public PopupBase
{    
public:
    static PopupBuffNewbie* create(void);
    
    PopupBuffNewbie(void);
    virtual ~PopupBuffNewbie(void);
    virtual bool init(void);
    virtual void onEnter();
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContent();
    
    // schedule
    void scheduleClose(float dt);
    
    // click
    
private:
    float _timeNowClose;
    float _timeMaxClose;
    
    E_NEWBIE_STEP _eNewbie;
    
    
    
    // widget
    cocos2d::Layer* _layerContainerContent;
 
};

#endif /* PopupBuffNewbie_h */
