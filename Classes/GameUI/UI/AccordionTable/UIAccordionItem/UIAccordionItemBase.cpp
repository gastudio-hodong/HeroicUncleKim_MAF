//
//  UIAccordionItemBase.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/13.
//

#include "UIAccordionItemBase.h"

USING_NS_CC;

UIAccordionItemBase* UIAccordionItemBase::create(const Size size, InfoAccordionUI* const infoAccodion)
{
    UIAccordionItemBase* pRet = new(std::nothrow) UIAccordionItemBase();
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

UIAccordionItemBase::UIAccordionItemBase(void):
_infoAccodion(nullptr)
{
}

UIAccordionItemBase::~UIAccordionItemBase(void)
{
}

bool UIAccordionItemBase::init(const Size size, InfoAccordionUI* const infoAccodion)
{
    if ( !LayerColor::init() )
    {
        return false;
    }
    
    //
    setIgnoreAnchorPointForPosition(false);
    setContentSize(size);
    _infoAccodion = infoAccodion;
    // init
    initVar();
    initUi();
    
    return true;
}
 
#pragma mark - init
void UIAccordionItemBase::initVar()
{
    
}

void UIAccordionItemBase::initUi()
{
    drawContainer();
}

#pragma mark - ui
void UIAccordionItemBase::drawContainer()
{
    auto sprBox = ui::Scale9Sprite::create(_infoAccodion->getBoxPath());
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setPosition(getContentSize()/2);
    sprBox->setContentSize(getContentSize());
    addChild(sprBox);
    
    auto lbTitle = Label::createWithTTF(_infoAccodion->getTitleText(), GAME_FONT, 24);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbTitle->setPosition(15, getContentSize().height / 2);
    lbTitle->setDimensions(getContentSize().width / 2, getContentSize().height - 10);
    lbTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbTitle->setOverflow(Label::Overflow::SHRINK);
    addChild(lbTitle);
    
    if(!_infoAccodion->isActive())
        lbTitle->setColor(Color3B(120,120,120));
}
