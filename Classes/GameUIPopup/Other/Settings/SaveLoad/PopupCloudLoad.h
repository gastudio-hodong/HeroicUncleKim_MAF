//
//  PopupCloudLoad.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupCloudLoad_h
#define PopupCloudLoad_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupCloudLoad : public PopupBase
{
public:
    static PopupCloudLoad* create();
    
    PopupCloudLoad(void);
    virtual ~PopupCloudLoad(void);
    virtual bool init();
    
public:
    // set
    void setCallbackResult(const std::function<void(void)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiTopMessage();
    void uiContentsLeft();
    void uiContentsRight();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickLoad(cocos2d::Ref* sender);
    void onClickNow(cocos2d::Ref* sender);
    
private:
    //
    std::function<void(void)> _callbackResult;
    
    // widget
    cocos2d::Layer* _layerContainerTopMessage;
    cocos2d::Layer* _layerContainerContentsLeft;
    cocos2d::Layer* _layerContainerContentsRight;
};

#endif /* PopupCloudLoad_h */
