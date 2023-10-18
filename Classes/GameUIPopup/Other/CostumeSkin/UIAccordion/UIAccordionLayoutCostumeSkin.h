//
//  UIAccordionLayoutCostumeSkin.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/13.
//

#ifndef UIAccordionLayoutCostumeSkin_hpp
#define UIAccordionLayoutCostumeSkin_hpp

#include "GameUI/UI/AccordionTable/UIAccordionLayer/UIAccordionLayerBase.h"

#include "Common/Observer/IRefresh.h"

#include "GameObject/InfoAccordionUI.h"

class UIAccordionLayoutCostumeSkin : public UIAccordionLayerBase
{
public:
    static UIAccordionLayoutCostumeSkin* create(const cocos2d::Vector<InfoAccordionUI*> listInfoAccodion, bool isActive = false);
    
    UIAccordionLayoutCostumeSkin(void);
    virtual ~UIAccordionLayoutCostumeSkin(void);
    
protected:
    //ui
    virtual void uiContainer() override;
    
    // click
    void onClickSkin(cocos2d::Ref* sender);
    
public:
    
private:
    
};

#endif /* UIAccordionLayoutCostumeSkin_hpp */
