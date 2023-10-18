//
//  UIAccordionLayoutRelicSpec.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/13.
//

#ifndef UIAccordionLayoutRelicSpec_hpp
#define UIAccordionLayoutRelicSpec_hpp

#include "GameUI/UI/AccordionTable/UIAccordionLayer/UIAccordionLayerBase.h"

#include "GameObject/InfoAccordionUI.h"

class UIAccordionLayoutRelicSpec : public UIAccordionLayerBase
{
public:
    static UIAccordionLayoutRelicSpec* create(const cocos2d::Vector<InfoAccordionUI*> listInfoAccodion);
    
    UIAccordionLayoutRelicSpec(void);
    virtual ~UIAccordionLayoutRelicSpec(void);
    
protected:
    //ui
    virtual void uiContainer() override;
    
private:
    static bool sort(InfoAccordionUI* const a, InfoAccordionUI* const b);
};

#endif /* UIAccordionLayoutRelicSpec_hpp */
