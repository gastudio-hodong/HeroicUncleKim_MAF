//
//  UIAccordionTableRelicSpec.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/13.
//

#ifndef UIAccordionTableRelicSpec_hpp
#define UIAccordionTableRelicSpec_hpp

#include "GameUI/UI/AccordionTable/UIAccordionTableBase.h"

class UIAccordionTableRelicSpec : public UIAccordionTableBase
{
public:
    static UIAccordionTableRelicSpec* create(const cocos2d::Size size, const std::vector<std::pair<std::string, cocos2d::Vector<InfoAccordionUI*>>> list);
    
    UIAccordionTableRelicSpec(void);
    virtual ~UIAccordionTableRelicSpec(void);
    virtual bool init(const cocos2d::Size size, const std::vector<std::pair<std::string, cocos2d::Vector<InfoAccordionUI*>>> list) override;
    
protected:
    //interface
    virtual void uiContainer() override;
    
protected:
};

#endif /* UIAccordionTableRelicSpec_hpp */
