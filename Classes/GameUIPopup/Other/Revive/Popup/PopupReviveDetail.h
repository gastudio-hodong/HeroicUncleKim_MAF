//
//  PopupReviveDetail.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 28/06/2019.
//

#ifndef PopupReviveDetail_h
#define PopupReviveDetail_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupReviveDetail : public PopupBase
{
public:
    static PopupReviveDetail* create(int nType);
    
    PopupReviveDetail(void);
    virtual ~PopupReviveDetail(void);
    virtual bool init(int type);
    
public:
    // set
    void setCallbackResult(const std::function<void(int, int)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContent();
    void uiContentAds();
    void uiButton();
    void uiAutoSave();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickYes(cocos2d::Ref* sender);
    void onClickAds(cocos2d::Ref* sender);
    void onClickFree(cocos2d::Ref* sender);
    
    void onClickAutoSaveCheck(cocos2d::Ref* sender);
    
    // callback
    
    
private:
    int _nType;
    bool _bAutoSave;
    
    std::function<void(int, int)> _onCallbackResult;
    
    // widget
    cocos2d::Layer* _layerContainerContent;
    cocos2d::Layer* _layerContainerContentAds;
    cocos2d::Layer* _layerContainerButton;
};

#endif /* PopupReviveDetail_h */
