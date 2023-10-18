//
//  PopupAdventureTimeReduction.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/06/08.
//

#ifndef PopupAdventureTimeReduction_hpp
#define PopupAdventureTimeReduction_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

class PopupAdventureTimeReduction : public PopupBase, public IRefresh
{
public:
    static PopupAdventureTimeReduction* create(const int nAdventureID);
    
    PopupAdventureTimeReduction(void);
    virtual ~PopupAdventureTimeReduction(void);
    virtual bool init(int nAdventureID);
    virtual void update(float dt) override;
    virtual void show() override;
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData)  override;
    
public:
    // set
    void setCallbackResult(const std::function<void(bool, int)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    void uiButton();
    void uiCurrency();
    
    void refreshTimer();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickArrow(cocos2d::Ref* sender);
    void onClickReduction(cocos2d::Ref* sender);
    
    //callback
    void callbakcReduction(bool bResult, int nResult);
    
private:
    int _nCount;
    int _nDefaultPrice;
    int _nReductionTime;
    int _nAdventureID;
    std::function<void(bool, int)> _callbackResult;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerButton;
    cocos2d::Layer* _layerContainerCurrency;
    cocos2d::Label* _labelNowTime;
    cocos2d::Label* _labelReductionTime;
    cocos2d::Label* _labelPrice;
    
    
};


#endif /* PopupAdventureTimeReduction_hpp */
