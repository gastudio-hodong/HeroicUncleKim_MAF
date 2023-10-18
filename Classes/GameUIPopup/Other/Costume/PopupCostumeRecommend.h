//
//  PopupCostumeRent.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupCostumeRecommend_h
#define PopupCostumeRecommend_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupCostumeRecommend : public PopupBase
{    
public:
    static PopupCostumeRecommend* create(E_COSTUME eType, E_COSTUME_IDX eIdx);
    
    PopupCostumeRecommend(void);
    virtual ~PopupCostumeRecommend(void);
    virtual bool init(E_COSTUME eType, E_COSTUME_IDX eIdx);
    
public:
    // set, get
    void setCallbackYes(const std::function<void(E_COSTUME, E_COSTUME_IDX)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiBottom();
    void uiContent();
    
    void onClickCoupon(cocos2d::Ref* sender);
    
    
private:
    E_COSTUME _eType;
    E_COSTUME_IDX _eIdx;
    std::function<void(E_COSTUME, E_COSTUME_IDX)> _onCallbackYes;
    
    // widget
    cocos2d::Layer* _layerContainerBottom;
    cocos2d::Layer* _layerContainerContent;
 
};

#endif /* PopupCostumeRent_h */
