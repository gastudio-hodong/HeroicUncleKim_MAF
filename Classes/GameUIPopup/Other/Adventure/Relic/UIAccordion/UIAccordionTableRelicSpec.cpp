//
//  UIAccordionTableRelicSpec.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/13.
//

#include "UIAccordionTableRelicSpec.h"

#include "GameUIPopup/Other/Adventure/Relic/UIAccordion/UIAccordionLayoutRelicSpec.h"

USING_NS_CC;

UIAccordionTableRelicSpec* UIAccordionTableRelicSpec::create(const Size size, const std::vector<std::pair<std::string, Vector<InfoAccordionUI*>>> list)
{
    UIAccordionTableRelicSpec* pRet = new(std::nothrow) UIAccordionTableRelicSpec();
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

UIAccordionTableRelicSpec::UIAccordionTableRelicSpec(void)
{
}

UIAccordionTableRelicSpec::~UIAccordionTableRelicSpec(void)
{
}

bool UIAccordionTableRelicSpec::init(const Size size, const std::vector<std::pair<std::string, Vector<InfoAccordionUI*>>> list)
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
void UIAccordionTableRelicSpec::uiContainer()
{
    for(auto obj : _listInfoAccordionUI)
    {
        std::string key = obj.first;
        std::string strText = "";
        std::string strPath = "";
        if(key.compare("atk") == 0)
        {
            strText = GAME_TEXT("t_ui_advt_property_type_17");
            strPath = "Assets/ui/adventure/adventure_relic_info_listname_box1.png";
        }
        else if(key.compare("cri") == 0)
        {
            strText = GAME_TEXT("t_ui_advt_property_type_18");
            strPath = "Assets/ui/adventure/adventure_relic_info_listname_box2.png";
        }
        else if(key.compare("spirit_bonus") == 0)
        {
            strText = GAME_TEXT("t_ui_advt_property_type_19");
            strPath = "Assets/ui/adventure/adventure_relic_info_listname_box3.png";
        }
        else if(key.compare("spirit_collect") == 0)
        {
            strText = GAME_TEXT("t_ui_advt_property_type_20");
            strPath = "Assets/ui/adventure/adventure_relic_info_listname_box4.png";
        }
        else if(key.compare("etc") == 0)
        {
            strText = GAME_TEXT("t_ui_advt_property_type_etc");
            strPath = "Assets/ui/adventure/adventure_relic_info_listname_box5.png";
        }
        auto layout = UIAccordionLayoutRelicSpec::create(obj.second);
        
        layout->setButtonText(strText);
        layout->setButtonImage(strPath, Size(710, 52));
        layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _table->addChild(layout);
        _listLayer.pushBack(layout);
    }
    
    updateLayout();
    linkLayerCallback();
}
