//
//  PopupLegendMake.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 16/10/2019.
//

#ifndef PopupLegendMake_h
#define PopupLegendMake_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupLegendMake : public PopupBase
{
public:
    
    static PopupLegendMake* create(E_COSTUME eType, E_COSTUME_IDX eIdx);
    
    PopupLegendMake(void);
    virtual ~PopupLegendMake(void);
    virtual bool init(E_COSTUME eType, E_COSTUME_IDX eIdx);

public:
    // set
    void setCallbackResult(const std::function<void(bool)>& callback);
    
protected:
    
    // init
    void initUi();
    
    // ui
    void uiContents();
  
    //draw
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onMakeLegend(cocos2d::Ref* sender);
    
    // callback
    void callbackBuy(bool bResult);
    
    
private:
    E_COSTUME _eType;
    E_COSTUME_IDX _eIdx;
    std::function<void(bool)> _callbackResult;
    
    // widget
    cocos2d::Layer* _layerContainerParent;
    cocos2d::Layer* _layerContainerContents;
};
#endif /* LegendMake_hpp */
