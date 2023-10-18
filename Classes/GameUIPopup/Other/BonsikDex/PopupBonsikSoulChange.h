//
//  PopupBonsikSoulChange.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupBonsikSoulChange_h
#define PopupBonsikSoulChange_h

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoTradeItems.h"
#include "GameObject/InfoItems.h"

class PopupBonsikSoulChange : public PopupBase
{
public:
    static PopupBonsikSoulChange* create();
    
    PopupBonsikSoulChange(void);
    virtual ~PopupBonsikSoulChange(void);
    virtual bool init();
    
public:
    // set
    void setCallbackResult(const std::function<void(void)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickArrowL(cocos2d::Ref* sender);
    void onClickArrowR(cocos2d::Ref* sender);
    void onClickTrade(cocos2d::Ref* sender);
    
     
private:
    cocos2d::Vector<InfoTradeItems*> _listTrade;
    std::vector<std::function<void(void)>> _listNetworkFunc;
    
    std::function<void(void)> _onCallbackResult;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
};

#endif /* PopupBonsikSoulChange_h */
