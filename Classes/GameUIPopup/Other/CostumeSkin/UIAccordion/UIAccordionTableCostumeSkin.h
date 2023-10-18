//
//  UIAccordionTableCostumeSkin.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/13.
//

#ifndef UIAccordionTableCostumeSkin_hpp
#define UIAccordionTableCostumeSkin_hpp

#include "GameUI/UI/AccordionTable/UIAccordionTableBase.h"

class UIAccordionTableCostumeSkin : public UIAccordionTableBase
{
public:
    static UIAccordionTableCostumeSkin* create(const cocos2d::Size size, const std::vector<std::pair<std::string, cocos2d::Vector<InfoAccordionUI*>>> list);
    
    UIAccordionTableCostumeSkin(void);
    virtual ~UIAccordionTableCostumeSkin(void);
    virtual bool init(const cocos2d::Size size, const std::vector<std::pair<std::string, cocos2d::Vector<InfoAccordionUI*>>> list) override;
    
protected:
    ///interface
    virtual void uiContainer() override;
    
protected:
    
};

#endif /* UIAccordionTableCostumeSkin_hpp */
