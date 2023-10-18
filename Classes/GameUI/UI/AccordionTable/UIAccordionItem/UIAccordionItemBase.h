//
//  UIAccordionItemBase.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/13.
//

#ifndef UIAccordionItemBase_hpp
#define UIAccordionItemBase_hpp

#include "Common/ConfigDefault.h"
#include "Common/Observer/IRefresh.h"

#include "GameObject/InfoAccordionUI.h"

class UIAccordionItemBase : public cocos2d::LayerColor
{
public:
    static UIAccordionItemBase* create(const cocos2d::Size size,  InfoAccordionUI* const infoAccodion);
    
    UIAccordionItemBase(void);
    virtual ~UIAccordionItemBase(void);
    virtual bool init(const cocos2d::Size size, InfoAccordionUI* const infoAccodion);
    
public:
    
protected:
    // init
    virtual void initVar();
    virtual void initUi();
    
    //ui
    virtual void drawContainer();
    
protected:
    InfoAccordionUI* _infoAccodion;
};


#endif /* UIAccordionItemBase_hpp */
