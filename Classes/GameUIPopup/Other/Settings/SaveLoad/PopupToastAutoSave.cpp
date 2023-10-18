//
//  PopupToastAutoSave.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/08/18.
//

#include "PopupToastAutoSave.h"

#include "ManagerEvent/EventBingoManager.h"

USING_NS_CC;

PopupToastAutoSave* PopupToastAutoSave::create()
{
    PopupToastAutoSave* pRet = new(std::nothrow) PopupToastAutoSave();
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

PopupToastAutoSave::PopupToastAutoSave(void) :
_timer(3)
{
}

PopupToastAutoSave::~PopupToastAutoSave(void)
{
}

bool PopupToastAutoSave::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    
    scheduleUpdate();
    
    // touch
    _eventDispatcher->removeEventListenersForTarget(this);
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *event)->bool {
        cocos2d::Rect rect = cocos2d::Rect(0, 0, this->getContentSize().width, this->getContentSize().height);
        if (rect.containsPoint(this->convertToNodeSpace(touch->getLocation())))
        {
            return true;
        }
        
        return false;
    };
    listener->onTouchEnded = [=](cocos2d::Touch *touch, cocos2d::Event *event)->void
    {
        hide();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // init
    initUi();
    return true;
}
void PopupToastAutoSave::update(float dt)
{
    _timer -= dt;
    
    if(_timer <= 0)
    {
        hide();
        return;
    }
}

#pragma mark - init
void PopupToastAutoSave::initUi()
{
    _spriteContainer->setVisible(false);
    
    setContentSize(Size(420, 120));
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setIgnoreAnchorPointForPosition(false);
    
    auto sprBg = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale2_3.png");
    sprBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBg->setContentSize(getContentSize());
    sprBg->setPosition(getContentSize()/2);
    addChild(sprBg);
    
    
    auto sprIcon = Sprite::create("Assets/ui/transfer/login_data_icon1.png");
    sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprIcon->setScale(1.5);
    sprIcon->getTexture()->setTexParameters(texParams);
    sprIcon->setPosition(Vec2(15, sprBg->getContentSize().height/2));
    sprBg->addChild(sprIcon);
    
    auto lbContent = Label::createWithTTF(GAME_TEXT("t_ui_menu_msg_4"), GAME_FONT, 25);
    lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbContent->setPosition(sprIcon->getPosition() + Vec2::RIGHT * (sprIcon->getContentSize().width * sprIcon->getScale() + 10));
    lbContent->setDimensions(sprBg->getContentSize().width - 25 - sprIcon->getContentSize().width, sprBg->getContentSize().height);
    lbContent->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    lbContent->setOverflow(cocos2d::Label::Overflow::SHRINK);
    sprBg->addChild(lbContent);
}
