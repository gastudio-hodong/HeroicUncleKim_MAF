//
//  PopupUserReport.h
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/09/17.
//

#ifndef PopupUserReport_h
#define PopupUserReport_h

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoProduct.h"

class PopupUserReport : public PopupBase
{
public:
    static PopupUserReport* create(E_REPORT_TYPE eType, int nUseridx, std::string strNickname, std::string strPlatform, std::string strMessage);
        
    PopupUserReport();
    virtual ~PopupUserReport(void);
    virtual bool init(E_REPORT_TYPE eType, int nUseridx, std::string strNickname, std::string strPlatform, std::string strMessage);
    
public:
    
protected:
        
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContents();
    void uiButton();
    
    //
    void drawButton();
    
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickReport(cocos2d::Ref* sender);
    
    // callback
    void callbackReport(bool bResult, int nResult);
    void callbackComboResult(std::string strResult);
    
private:
    E_REPORT_TYPE _eReportType;
    
    int _nUserIdx;
    std::string _strUserNickname;
    std::string _strUserPlatform;
    
    std::string _strMessage;
    
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerButton;
    
    cocos2d::Label* _labelReportCount;
    cocos2d::ui::EditBox* _editBox;
     
};

#endif /* PopupUserReport_h */
