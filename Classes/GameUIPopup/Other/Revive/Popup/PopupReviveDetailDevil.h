//
//  PopupReviveDetailDevil.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 28/06/2019.
//

#ifndef PopupReviveDetailDevil_h
#define PopupReviveDetailDevil_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupReviveDetailDevil : public PopupBase
{
public:
    static PopupReviveDetailDevil* create(int nType);
    
    PopupReviveDetailDevil(void);
    virtual ~PopupReviveDetailDevil(void);
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
    void uiButton();
    void uiAutoSave();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickYes(cocos2d::Ref* sender);
    void onClickAds(cocos2d::Ref* sender);
    
    
    void onClickAutoSaveCheck(cocos2d::Ref* sender);
    
    // callback
    
    
private:
    int _nType;
    bool _bAutoSave;
    
    std::function<void(int, int)> _onCallbackResult;
    
    // widget
    cocos2d::Layer* _layerContainerContent;
    cocos2d::Layer* _layerContainerButton;
};

#endif /* PopupReviveDetailDevil_h */
