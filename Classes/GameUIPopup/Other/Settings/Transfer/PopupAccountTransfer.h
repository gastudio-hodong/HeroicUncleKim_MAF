//
//  PopupAccountTransfer.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupAccountTransfer_h
#define PopupAccountTransfer_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupAccountTransfer : public PopupBase
{
public:
    static PopupAccountTransfer* create(void);
    
    PopupAccountTransfer(void);
    virtual ~PopupAccountTransfer(void);
    virtual bool init(void);
    
public:
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiPrecautions();
    void uiContent();
    void uiSelect();
    void uiSend();
    void uiReceive();
    
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickTransfer(cocos2d::Ref* sender);
    void onClickSend(cocos2d::Ref* sender);
    void onClickReceive(cocos2d::Ref* sender);
    void onClickCancel(cocos2d::Ref* sender);
    void onClickConfirm(cocos2d::Ref* sender);
    
    
    // network
    void requestAuth();
    void responseAuth(cocos2d::network::HttpResponse* response,std::string &data);
    void requestAuthCheck();
    void responseAuthCheck(cocos2d::network::HttpResponse* response,std::string &data);
    
    // callback
    void callbackSave(bool bResult);
    void callbackLoad(bool bResult, bool bData);
    
private:
    std::string _strUUID;
    std::string _strUUIDConfirm;
    
    // widget
    cocos2d::Layer* _layerContainerPrecautions;
    cocos2d::Layer* _layerContainerContent;
    
    cocos2d::Label* _labelPrecautions;
    
    cocos2d::ui::EditBox* _edit;
 
};

#endif /* PopupAccountTransfer_h */
