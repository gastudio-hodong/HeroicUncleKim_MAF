//
//  UIAccordionItem.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/13.
//

#ifndef UIAccordionItem_hpp
#define UIAccordionItem_hpp

#include "GameUI/UI/AccordionTable/UIAccordionItem/UIAccordionItemBase.h"

#include "GameObject/InfoAccordionUI.h"

class UIAccordionItemRelicSpec : public UIAccordionItemBase
{
public:
    static UIAccordionItemRelicSpec* create(const cocos2d::Size size,  InfoAccordionUI* const infoAccodion);
    
    UIAccordionItemRelicSpec(void);
    virtual ~UIAccordionItemRelicSpec(void);
    
protected:
    //ui
    virtual void drawContainer() override;
private:
    
};


#endif /* UIAccordionItem_hpp */
