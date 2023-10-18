//
//  MailRecievePopup.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 25/06/2019.
//

#ifndef MailReceivePopup_hpp
#define MailReceivePopup_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoItems.h"

class MailReceivePopup : public PopupBase
{
public:
    static MailReceivePopup* create(cocos2d::Vector<InfoItems*> listReward, int nSuccess, int nError);
    
    MailReceivePopup(void);
    virtual ~MailReceivePopup(void);
    virtual bool init(cocos2d::Vector<InfoItems*> listReward, int nSuccess, int nError);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    void uiText();
    void uiButton();
    void uiAutoSave();
    void showAutoSaveToast();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickAutoSaveCheck(cocos2d::Ref* sender);
    
    // callback
    void callbackSave(bool bResult);

private:
    cocos2d::Vector<InfoItems*> _listReward;
    int _nSuccess;
    int _nError;
    cocos2d::Size _sizeContents;
    bool _bAutoSave;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerText;
    cocos2d::Layer* _layerContainerButton;
};

#endif /* MailRecievePopup_hpp */
