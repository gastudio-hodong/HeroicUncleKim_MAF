//
//  UIAccordionItem.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/13.
//
#include "UIAccordionItemRelicSpec.h"

#include "GameUIPopup/Other/Adventure/Relic/UIAccordion/UIAccordionLayoutRelicSpec.h"

USING_NS_CC;

UIAccordionItemRelicSpec* UIAccordionItemRelicSpec::create(const Size size, InfoAccordionUI* const infoAccodion)
{
    UIAccordionItemRelicSpec* pRet = new(std::nothrow) UIAccordionItemRelicSpec();
    if ( pRet && pRet->init(size, infoAccodion) )
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

UIAccordionItemRelicSpec::UIAccordionItemRelicSpec(void)
{
}

UIAccordionItemRelicSpec::~UIAccordionItemRelicSpec(void)
{
}
#pragma mark - ui
void UIAccordionItemRelicSpec::drawContainer()
{
    UIAccordionItemBase::drawContainer();
    
    auto lbEffect = Label::createWithTTF(_infoAccodion->getText("EFFECT"), GAME_FONT, 24);
    lbEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    lbEffect->setPosition(getContentSize().width - 15, getContentSize().height / 2);
    lbEffect->setDimensions(getContentSize().width / 3, getContentSize().height - 10);
    lbEffect->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
    lbEffect->setOverflow(Label::Overflow::SHRINK);
    lbEffect->setColor(Color3B(225,220,95));
    addChild(lbEffect);
    
    if(!_infoAccodion->isActive() || _infoAccodion->getValueDouble("EFFECT") <= 0)
        lbEffect->setColor(Color3B(120,120,120));
}
