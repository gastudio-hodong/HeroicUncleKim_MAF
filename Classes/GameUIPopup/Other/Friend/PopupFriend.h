//
//  PopupFriend.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#ifndef PopupFriend_h
#define PopupFriend_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupFriend : public PopupBase
{
private :
    enum eType {
        type_list = 1,
        type_invite = 2,
    };
    
public:
    static PopupFriend* create();
    
    PopupFriend(void);
    virtual ~PopupFriend(void);
    virtual bool init() override;
    
protected:
    
    void initVar();
    void initUi();
    
    // ui
    void uiMenu();
    
    //
    void drawMenu();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickMenu(cocos2d::Ref* sender);
    
    // callback
    void callbackHide();
    
private:
    //
    PopupFriend::eType _eType;

    // widget
    cocos2d::Layer* _layerContainerMenu;
    
    cocos2d::Layer* _layerContainerNow;
    cocos2d::Layer* _layerContainerFriendList;
    cocos2d::Layer* _layerContainerFriendInvite;
};

#endif /* PopupFriend_h */
