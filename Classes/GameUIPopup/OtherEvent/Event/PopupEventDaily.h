//
//  PopupEventDaily.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupEventDaily_h
#define PopupEventDaily_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupEventDaily : public PopupBase
{
public:
    enum eTag {
        tag_ui_contents_box = 100,  // 100~ 내용 수까지 사용
        tag_ui_contents_box_day = 10,
        tag_ui_contents_box_icon = 11,
        tag_ui_bottom_box_coin = 100,
        tag_ui_bottom_box_ticket = 101,
        tag_ui_bottom_box_in_label = 10,
    };
    
public:
    static PopupEventDaily* create();
    
    PopupEventDaily(void);
    virtual ~PopupEventDaily(void);
    virtual bool init();
    virtual void onEnter();
    
public:
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    void uiBottom();
    void drawContents();
    void drawBottom();

    // set, get
    
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    
    // network
    void requestInfo();
    void responseInfo(cocos2d::network::HttpResponse* response,std::string &data);
    
private:
    
    int _nRewardCoin;
    int _nRewardPoint;
    
    int _nRewardDay;
    bool _bRewardNew;
    

    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::LayerColor* _layerContainerBottom;
};

#endif /* PopupEventDaily_h */
