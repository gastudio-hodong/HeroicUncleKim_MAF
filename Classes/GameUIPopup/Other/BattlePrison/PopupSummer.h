//
//  PopupSummer.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupSummer_h
#define PopupSummer_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupSummer : public PopupBase
{
public:
    static PopupSummer* create();
    
    PopupSummer(void);
    virtual ~PopupSummer(void);
    virtual bool init();
    virtual void onEnter();
    
public:
    // set
    void setCallbackShortcuts(const std::function<void(void)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    
    // draw
    void drawPearl();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickShortcuts(cocos2d::Ref* sender);
    
    // network
    void requestInfo();
    void responseInfo(cocos2d::network::HttpResponse* response,std::string &data);
private:
    int _nPearl;
    std::function<void(void)> _onCallbackShortcuts;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Label* _labelPearl;
};

#endif /* PopupSummer_h */
