//
//  PopupChatInfo.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef PopupChatInfo_h
#define PopupChatInfo_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoChat;
class PopupChatInfo : public PopupBase
{
public:
    static PopupChatInfo* create(InfoChat* infoChat);
    
    PopupChatInfo(void);
    virtual ~PopupChatInfo(void);
    virtual bool init(InfoChat* infoChat);

public:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiButton();
    
    // ui draw
    void drawButton();
    
    //callback
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickDetail(cocos2d::Ref* sender);
    void onClickReport(cocos2d::Ref* sender);
    void onClickBlock(cocos2d::Ref* sender);
    
 
 
    
private:
    InfoChat* _infoChat;
    
    // widget
    cocos2d::Layer* _layerContainerButton;
 
};
#endif /* PopupChatInfo_h */
