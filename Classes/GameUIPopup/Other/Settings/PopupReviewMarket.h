//
//  PopupReviewMarket.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupReviewMarket_h
#define PopupReviewMarket_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupReviewMarket : public PopupBase
{    
public:
    static PopupReviewMarket* create();
    
    PopupReviewMarket(void);
    virtual ~PopupReviewMarket(void);
    virtual bool init();
    
public:
    // set, get
    void setCallbackYes(const std::function<void(bool)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContent();
    
    // set, get
    
    
    // click
    void onClickReviewNo(cocos2d::Ref* sender);
    void onClickReviewOk(cocos2d::Ref* sender);
    
private:
    
    // widget
    cocos2d::Layer* _layerContainerContent;
 
};

#endif /* PopupReviewMarket_h */
