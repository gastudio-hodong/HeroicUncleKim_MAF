//
//  PopupBadge.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/13.
//

#ifndef PopupBadge_h
#define PopupBadge_h

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoBadge.h"

class PopupBadge : public PopupBase
{
public:
    static PopupBadge* create();
    
    PopupBadge(void);
    virtual ~PopupBadge(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiBadgeSelect();
    void uiBadgeHave();
    void uiButton();
    cocos2d::Layer* uiBadgeDetail(double nWidth, double nHeight, std::string strGroup);
    
    // draw
    void drawBadgeSelect();
    void drawBadgeHave();

    // set, get
    
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickEdit(cocos2d::Ref* sender);
    void onClickEditApply(cocos2d::Ref* sender);
    void onClickSelect(cocos2d::Ref* sender);
    void onClickUnSelect(cocos2d::Ref* sender);
    
    
    
    // network
    
    // callback
    void callbackInfo(bool bResult);
    void callbackUpdate(bool bResult);

    
private:
    bool _bEdit;
    
    cocos2d::Vector<InfoBadgeHave*> _listBadgeSelect;
    cocos2d::Vector<InfoBadgeHave*> _listBadgeSelectTemp;
    
    
    // widget
    cocos2d::Layer* _layerContainerBadgeSelect;
    cocos2d::Layer* _layerContainerBadgeSelectInBox;
    cocos2d::Layer* _layerContainerBadgeHave;
    cocos2d::Layer* _layerContainerButton;
    
    cocos2d::Vector<MafNode::MafMenuItemSprite*> _listBadgeHaveItem;
    cocos2d::Label* _labelTotalBadgeAtkBuff;
};

#endif /* PopupBadge_hpp */
