//
//  PopupNotic.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#ifndef PopupNotic_h
#define PopupNotic_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupNotic : public PopupBase
{
public:
    static PopupNotic* create();
    
    PopupNotic(void);
    virtual ~PopupNotic(void);
    virtual bool init() override;
    
public:
    virtual void hide() override;
    void show(bool bCheckIgnore = false);
    
protected:
    void initVar();
    void initUi();
    
    // ui
    void uiDefaultWebView();
    void uiDefaultCheck();
    
    // draw
    
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickCheck(cocos2d::Ref* sender);
    void onClickBack(cocos2d::Ref* sender);
    

    // callback
    
    // game
    void setCheckSave();

    
private:
    //
    std::string _strUrl;
    bool _bCheck;

    // widget
    cocos2d::Layer* _layerContainer;
    cocos2d::Layer* _layerContainerCheck;
    
    cocos2d::experimental::ui::WebView* _nodeWebView;
    MafNode::MafMenuItemSprite* _itemCheck;
    MafNode::MafMenuItemSprite* _itemBack;
};

#endif /* PopupNotic_h */
