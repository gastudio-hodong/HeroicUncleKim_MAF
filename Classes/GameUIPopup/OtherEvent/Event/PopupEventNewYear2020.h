//
//  PopupEventNewYear2020.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupEventNewYear2020_h
#define PopupEventNewYear2020_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupEventNewYear2020 : public PopupBase
{
public:
    static PopupEventNewYear2020* create();
    
    PopupEventNewYear2020(void);
    virtual ~PopupEventNewYear2020(void);
    virtual bool init();
    virtual void onEnter();
public:
    // set
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    
    // draw
    void drawContents();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickReward(cocos2d::Ref* sender);
    
    
    // network
    void requestInfo();
    void responseInfo(cocos2d::network::HttpResponse* response,std::string &data);
    
private:
    //
    bool _bReward;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Sprite* _spriteReward;
};

#endif /* PopupEventNewYear2020_h */
