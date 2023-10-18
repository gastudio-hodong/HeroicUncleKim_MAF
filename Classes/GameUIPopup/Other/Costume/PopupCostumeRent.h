//
//  PopupCostumeRent.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupCostumeRent_h
#define PopupCostumeRent_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupCostumeRent : public PopupBase
{    
public:
    static PopupCostumeRent* create(E_COSTUME eType, E_COSTUME_IDX eIdx);
    
    PopupCostumeRent(void);
    virtual ~PopupCostumeRent(void);
    virtual bool init(E_COSTUME eType, E_COSTUME_IDX eIdx);
    
public:
    // set, get
    void setCallbackYes(const std::function<void(int, int, bool)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiBottom();
    void uiContent();
    
    // click
    void onClickNo(cocos2d::Ref* sender);
    void onClickCoupon(cocos2d::Ref* sender);
    void onClickCoin(cocos2d::Ref* sender);
    
    
private:
    E_COSTUME _eType;
    E_COSTUME_IDX _eIdx;
    
    std::function<void(int, int, bool)> _onCallbackYes;
    
    // widget
    cocos2d::Layer* _layerContainerBottom;
    cocos2d::Layer* _layerContainerContent;
 
};

#endif /* PopupCostumeRent_h */
