//
//  PopupPartyMiniSpec.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/06.
//

#ifndef PopupPartyMiniSpec_hpp
#define PopupPartyMiniSpec_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

class PopupPartyMiniSpec : public PopupBase
{
public:
    static PopupPartyMiniSpec* create(const int nParty);
    
    PopupPartyMiniSpec(void);
    virtual ~PopupPartyMiniSpec(void);
    virtual bool init(const int nParty);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    
private:
    int _nParty;
    cocos2d::Layer* _layerContainerContents;
};

#endif /* PopupPartyMiniSpec_hpp */
