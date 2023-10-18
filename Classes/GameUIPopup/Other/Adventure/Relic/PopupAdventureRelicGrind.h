//
//  PopupAdventureRelicGrind.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/13.
//

#ifndef PopupAdventureRelicGrind_hpp
#define PopupAdventureRelicGrind_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

#include "GameObject/Adventure/InfoAdventureRelic.h"

class PopupAdventureRelicGrind : public PopupBase, public IRefresh
{
public:
    static PopupAdventureRelicGrind* create(const int nIdx);
    
    PopupAdventureRelicGrind(void);
    virtual ~PopupAdventureRelicGrind(void);
    virtual bool init(const int nIdx);
    virtual void show() override;
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData)  override;
    
public:
    
    void setCallbackRefresh(const std::function<void(void)>& _callback);
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    void uiButton();
    
    // click
    void onClickClose(cocos2d::Ref* const sender);
    void onClickGrind(cocos2d::Ref* const sender);
    void onClickFluctuation(cocos2d::Ref* const sender);
    // network
    
    void callbackGrind(const bool bResult, const int nResult);
private:
    int _nIdx;
    InfoAdventureRelic* _infoCurrentRelic;
    int _nCount;
    
    std::function<void(void)> _callbackRefresh;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerButton;
    
    cocos2d::Label* _lbGrind;
};

#endif /* PopupAdventureRelicGrind_hpp */
