//
//  UIAccordionTableCostumeSkin.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/13.
//

#include "UIAccordionTableCostumeSkin.h"

#include "GameUIPopup/Other/CostumeSkin/UIAccordion/UIAccordionLayoutCostumeSkin.h"

USING_NS_CC;

UIAccordionTableCostumeSkin* UIAccordionTableCostumeSkin::create(const Size size, const std::vector<std::pair<std::string, Vector<InfoAccordionUI*>>> list)
{
    UIAccordionTableCostumeSkin* pRet = new(std::nothrow) UIAccordionTableCostumeSkin();
    if ( pRet && pRet->init(size, list))
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

UIAccordionTableCostumeSkin::UIAccordionTableCostumeSkin(void)
{
}

UIAccordionTableCostumeSkin::~UIAccordionTableCostumeSkin(void)
{
}

bool UIAccordionTableCostumeSkin::init(const Size size, const std::vector<std::pair<std::string, Vector<InfoAccordionUI*>>> list)
{
    if ( !UIAccordionTableBase::init(size, list) )
    {
        return false;
    }
    
    _dLayerInterval = 0;
    
    updateLayout();
    
    return true;
}
#pragma mark - ui
void UIAccordionTableCostumeSkin::uiContainer()
{
    _table->getInnerContainer()->removeAllChildren();
    _listLayer.clear();
    
    for(auto obj : _listInfoAccordionUI)
    {
        std::string key = obj.first;
        std::string strText = "";
        std::string strPath = "";
        if(key.compare("own") == 0)
        {
            strText = GAME_TEXT("t_ui_buy_yes");
            strPath = "Assets/ui/adventure/adventure_relic_info_listname_box1.png";
        }
        else if(key.compare("non_own") == 0)
        {
            strText = GAME_TEXT("t_ui_buy_no");
            strPath = "Assets/ui/adventure/adventure_relic_info_listname_box2.png";
        }

        auto layout = UIAccordionLayoutCostumeSkin::create(obj.second, true);
        layout->setButtonText(strText);
        layout->setButtonImage(strPath, Size(700, 54));
        layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        layout->setName(key);
        _table->addChild(layout);
        
        _listLayer.pushBack(layout);
    }
    
    updateLayout();
    linkLayerCallback();
}
