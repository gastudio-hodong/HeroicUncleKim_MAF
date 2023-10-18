//
//  UIAdsCooldown.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/04/01.
//

#include "UIAdsCooldown.h"

#include "Common/ConfigDefault.h"

USING_NS_CC;

UIAdsCooldown* UIAdsCooldown::create()
{
    UIAdsCooldown* pRet = new(std::nothrow) UIAdsCooldown();
    if ( pRet && pRet->init() )
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

UIAdsCooldown::UIAdsCooldown():
_fTimer(0),
_lbMsg(nullptr),
_isUIActive(false)
{
}

UIAdsCooldown::~UIAdsCooldown()
{
}

bool UIAdsCooldown::init()
{
    auto sprBox = ui::Scale9Sprite::create("Assets/ui/offline/offline_item_box1.png");
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setContentSize(Size(sprBox->getContentSize().width * 1.8f, sprBox->getContentSize().height * 1.5f));
    sprBox->setOpacity(230);
    addChild(sprBox);
    
    auto sprIcon = Sprite::create("Assets/ui/offline/icon_offline_time.png");
    sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprIcon->setScale(0.7f);
    sprIcon->setPosition(10, sprBox->getContentSize().height/2);
    sprBox->addChild(sprIcon);
    
    _lbMsg = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_ads_cooldown_1", AdsUtilsManager::getInstance()->getAdsCooldown()), GAME_FONT, 22);
    _lbMsg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _lbMsg->setPosition(sprIcon->getPosition() + Vec2::RIGHT * (sprIcon->getContentSize().height + 10));
    _lbMsg->setDimensions(sprBox->getContentSize().width - sprIcon->getContentSize().width - 30, sprBox->getContentSize().height);
    _lbMsg->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    _lbMsg->setOverflow(Label::Overflow::SHRINK);
    sprBox->addChild(_lbMsg);
    
    setContentSize(sprBox->getContentSize());
    sprBox->setPosition(sprBox->getContentSize()/2);
    
    return true;
}

void UIAdsCooldown::update(float dt)
{
    _fTimer += dt;
    
    _lbMsg->setString(GAME_TEXTFORMAT("t_ui_ads_cooldown_1", (int)AdsUtilsManager::getInstance()->getAdsCooldown()));
    
    if(_fTimer >= 1.f)
    {
        _fTimer = 0;
        
        if(AdsUtilsManager::getInstance()->getAdsCooldown() <= 0)
            hide();
    }
    
}

void UIAdsCooldown::hide()
{
    setVisible(false);
    _isUIActive = false;
    
    _fTimer = 0.f;
    
    unscheduleUpdate();
    
    
    if(getParent() != nullptr)
        removeFromParent();
}

void UIAdsCooldown::show()
{
    setVisible(true);
    
    _isUIActive = true;
    
    _fTimer = 0.f;
    
    unscheduleUpdate();
    scheduleUpdate();
}

bool UIAdsCooldown::isActiveUI()
{
    return _isUIActive;
}
