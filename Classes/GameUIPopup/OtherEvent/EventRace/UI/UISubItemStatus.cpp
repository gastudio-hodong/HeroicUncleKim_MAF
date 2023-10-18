//
//  UISubItemStatus.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/26.
//

#include "UISubItemStatus.h"

#include "ManagerEvent/EventRaceManager.h"

USING_NS_CC;

UISubItemStatus* UISubItemStatus::create(const Size size, InfoEventRaceStat*  const infoStat)
{
    UISubItemStatus* pRet = new(std::nothrow) UISubItemStatus();
    if ( pRet && pRet->init(size, infoStat) )
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

UISubItemStatus::UISubItemStatus(void):_infoStat(nullptr)
{
}

UISubItemStatus::~UISubItemStatus(void)
{
}

bool UISubItemStatus::init(const Size size, InfoEventRaceStat*  const infoStat)
{
    if (!LayerColor::init())
    {
        return false;
    }
    _infoStat = infoStat;
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    // init
    initVar();
    initUI();
    
    return true;
}
#pragma mark - init
void UISubItemStatus::initVar()
{
}

void UISubItemStatus::initUI()
{
    auto sprBg = ui::Scale9Sprite::create("Assets/ui/race_event/race_betting_listbox.png",Rect::ZERO, Rect(3,3,10,12));
    sprBg->setContentSize(getContentSize());
    sprBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBg->setPosition(getContentSize()/2);
    addChild(sprBg);
    
    auto sprIcon = Sprite::create(getIconPath());
    sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprIcon->setPosition(15, getContentSize().height/2);
    addChild(sprIcon);
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_race_status_" + _infoStat->getTypeName()), GAME_FONT, 30);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbTitle->setPosition(sprIcon->getPosition() + Vec2::RIGHT * (sprIcon->getContentSize().width + 15));
    lbTitle->setDimensions(getContentSize().width/2 - (sprIcon->getContentSize().width + 15), getContentSize().height - 10);
    lbTitle->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    lbTitle->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbTitle->enableBold();
    addChild(lbTitle);
    
    std::vector<Color3B> listColor = {
        Color3B(255,0,0),
        Color3B(255,120,0),
        Color3B(245,245,245),
        Color3B(255,250,0),
        Color3B(30,255,0),
        Color3B(250,255,205),
    };
    auto lbEffect = Label::createWithTTF(_infoStat->getText(), GAME_FONT, 30);
    lbEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    lbEffect->setPosition(getContentSize().width - 15, getContentSize().height/2);
    lbEffect->setDimensions(getContentSize().width/2, getContentSize().height - 10);
    lbEffect->setAlignment(cocos2d::TextHAlignment::RIGHT, cocos2d::TextVAlignment::CENTER);
    lbEffect->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbEffect->setColor(listColor[5]);
    lbEffect->enableBold();
    addChild(lbEffect);
    
    if(_infoStat->getTypeName().compare("CONDITION") == 0)
    {
        if(EventRaceManager::getInstance()->isHiddenAds())
            lbEffect->setColor(listColor[_infoStat->getGrade()-1]);
        else
            lbEffect->setString("???");
    }
    else if(_infoStat->getTypeName().compare("ABILITY") == 0)
    {
        if(!EventRaceManager::getInstance()->isHiddenAds())
            lbEffect->setString("???");
    }
    else
        lbEffect->setColor(listColor[_infoStat->getGrade()-1]);
}

#pragma mark -ui
const std::string UISubItemStatus::getIconPath() const
{
    std::string result = "";
    
    if(_infoStat->getTypeName().compare("SPEED") == 0)
        result = "Assets/ui/race_event/race_info_icon_00.png";
    else if(_infoStat->getTypeName().compare("STAMINA") == 0)
        result = "Assets/ui/race_event/race_info_icon_01.png";
    else if(_infoStat->getTypeName().compare("POWER") == 0)
        result = "Assets/ui/race_event/race_info_icon_02.png";
    else if(_infoStat->getTypeName().compare("GUTS") == 0)
        result = "Assets/ui/race_event/race_info_icon_03.png";
    else if(_infoStat->getTypeName().compare("WISDOM") == 0)
        result = "Assets/ui/race_event/race_info_icon_04.png";
    else if(_infoStat->getTypeName().compare("CONDITION") == 0)
        result = "Assets/ui/race_event/race_info_icon_05.png";
    else if(_infoStat->getTypeName().compare("ABILITY") == 0)
        result = "Assets/ui/race_event/race_info_icon_06.png";
    
    return result;
}
