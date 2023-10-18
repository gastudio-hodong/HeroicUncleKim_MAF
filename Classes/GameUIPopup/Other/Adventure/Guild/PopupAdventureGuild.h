//
//  PopupAdventureGuild.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/15.
//

#ifndef PopupAdventureGuild_hpp
#define PopupAdventureGuild_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

class PopupAdventureGuild : public PopupBase, public IRefresh
{
public:
    static PopupAdventureGuild* create();
    
    PopupAdventureGuild(void);
    virtual ~PopupAdventureGuild(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData)  override;
    // refresh
    virtual void hide() override;
    virtual void show() override;
public:
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiTop();
    void uiBottom();
    
    // ui draw
    
    // click
    void onClickClose(const Ref* sender);
    
    // callback
    void callbackInfo(bool bResult, int nResult);
    void callbackHide();
private:
    
    // widget
    cocos2d::Layer* _lyContainerParent;
    cocos2d::Layer* _lyContainerTop;
    cocos2d::Layer* _lyContainerBottom;
};

#endif /* PopupAdventureGuild_hpp */
