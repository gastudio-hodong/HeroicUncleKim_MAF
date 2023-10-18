//
//  PopupAuthority.h
//  FantasyClicker-mobile
//
//  Created by TK on 2020/06/03.
//

#ifndef PopupAuthority_h
#define PopupAuthority_h

#include "GameUIPopup/Base/PopupBase.h"

#include "ManagerGame/PrisonManager.h"

class PopupAuthority : public PopupBase
{
public:
    static PopupAuthority* create(PrisonManager::PrisonType type);
    
    PopupAuthority(void);
    virtual ~PopupAuthority(void);
    virtual bool init(PrisonManager::PrisonType type);
    
public:
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiBottom();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    
private:
    
    // widget
    cocos2d::Layer* _layerContainerTop;
    cocos2d::Layer* _layerContainerBottom;
    cocos2d::Layer* _layerContainerList;
    cocos2d::Sprite* _spriteIcon;
    cocos2d::Label* _labelContentTitle;
    cocos2d::Label* _labelContentInfo;
    
    PrisonManager::PrisonType _type;
};


#endif /* PopupAuthority_h */
