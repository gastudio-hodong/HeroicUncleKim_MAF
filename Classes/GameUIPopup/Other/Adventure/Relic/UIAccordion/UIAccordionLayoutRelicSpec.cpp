//
//  UIAccordionLayoutRelicSpec.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/13.
//

#include "UIAccordionLayoutRelicSpec.h"

#include "GameUIPopup/Other/Adventure/Relic/UIAccordion/UIAccordionItemRelicSpec.h"

USING_NS_CC;

bool UIAccordionLayoutRelicSpec::sort(InfoAccordionUI* const a, InfoAccordionUI* const b)
{
    if(a == nullptr)
        return true;
    else if(b == nullptr)
        return false;
    
    bool result = false;
    
    bool isActiveA = a->isActive();
    
    bool isActiveB = b->isActive();
    
    if(isActiveA && !isActiveB)
        result = true;
    else if(!isActiveA && isActiveB)
        result = false;
    else
        result = a->getIdx() < b->getIdx();
    
    return result;
}

UIAccordionLayoutRelicSpec* UIAccordionLayoutRelicSpec::create(const Vector<InfoAccordionUI*> listInfoAccodion)
{
    UIAccordionLayoutRelicSpec* pRet = new(std::nothrow) UIAccordionLayoutRelicSpec();
    if ( pRet && pRet->init(listInfoAccodion) )
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

UIAccordionLayoutRelicSpec::UIAccordionLayoutRelicSpec(void)
{
}

UIAccordionLayoutRelicSpec::~UIAccordionLayoutRelicSpec(void)
{
}

#pragma mark - ui
void UIAccordionLayoutRelicSpec::uiContainer()
{
    _uiContainer->removeAllChildren();
    
    if(!_isActive)
    {
        _uiContainer->setContentSize(Size::ZERO);
        updateLayout();
        return;
    }
    
    float subItemHeight = 42;
    float subItemInterval = 5;
    
    Size size = Size(710, subItemHeight * _listInfoAccodion.size() + subItemInterval * _listInfoAccodion.size());
    _uiContainer->setContentSize(size);
    Vec2 pos = Vec2(size.width/2, size.height);
    std::sort(_listInfoAccodion.begin(), _listInfoAccodion.end(), UIAccordionLayoutRelicSpec::sort);
    for(int i = 0; i < _listInfoAccodion.size(); ++i)
    {
        auto obj = _listInfoAccodion.at(i);
        auto uiItem = UIAccordionItemRelicSpec::create(Size(710, 42), obj);
        uiItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiItem->setPosition(pos);
        _uiContainer->addChild(uiItem);
        
        pos -= Vec2(0, subItemHeight + subItemInterval);
    }
    
    updateLayout();
}
