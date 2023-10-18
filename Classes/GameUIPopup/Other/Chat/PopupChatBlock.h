//
//  PopupChatBlock.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef PopupChatBlock_h
#define PopupChatBlock_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoChat;
class PopupChatBlock : public PopupBase
{
public:
    static PopupChatBlock* create(InfoChat* infoChat);
    
    PopupChatBlock(void);
    virtual ~PopupChatBlock(void);
    virtual bool init(InfoChat* infoChat);

public:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContents();
    void uiButton();
    
    // ui draw
    void drawButton();
    
    //callback
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickList(cocos2d::Ref* sender);
    void onClickReport(cocos2d::Ref* sender);
    
 
 
    
private:
    InfoChat* _infoChat;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerButton;
    
    cocos2d::Label* _labelBlockCount;
 
};
#endif /* PopupChatBlock_h */
