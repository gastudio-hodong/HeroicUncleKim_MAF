//
//  PopupBuffNewbie.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupBuffNewbie.h"

#include "ManagerGame/UserInfoConfigManager.h"

USING_NS_CC;

PopupBuffNewbie* PopupBuffNewbie::create(void)
{
    PopupBuffNewbie* pRet = new(std::nothrow) PopupBuffNewbie();
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

PopupBuffNewbie::PopupBuffNewbie(void) :
_timeMaxClose(2.0f),
_timeNowClose(2.0f),

_eNewbie(E_NEWBIE_STEP::STEP_NONE),

//
_layerContainerContent(nullptr)
{
    
}

PopupBuffNewbie::~PopupBuffNewbie(void)
{
    
}

bool PopupBuffNewbie::init(void)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
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
    listener->onTouchEnded = [=](cocos2d::Touch *touch, cocos2d::Event *event)->void {
        hide();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    // init
    initVar();
    initUi();
    

    return true;
}

void PopupBuffNewbie::onEnter()
{
    PopupBase::onEnter();
    
    //
    this->schedule(schedule_selector(PopupBuffNewbie::scheduleClose), 1.0f);
}

#pragma mark - init
void PopupBuffNewbie::initVar()
{
    auto config = UserInfoConfigManager::getInstance();
    _eNewbie = config->getNewbieStep();
    _eNewbie = (E_NEWBIE_STEP)MAX(1, (int)_eNewbie);
}

void PopupBuffNewbie::initUi()
{
    Size size = Size(497, 500);
    
    //
    _spriteContainer->setContentSize( size );
    
    //
    
    auto labelTitle = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_nebew_buff_popup_title_%d", (int)_eNewbie)), GAME_FONT, 32);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    labelTitle->setColor(COLOR_TITLE);
    _spriteContainer->addChild(labelTitle);
    
    uiContent();
}

#pragma mark - ui
void PopupBuffNewbie::uiContent()
{
    if ( _layerContainerContent == nullptr )
    {
        _layerContainerContent = Layer::create();
        _layerContainerContent->setContentSize( Size(_spriteContainer->getContentSize().width, _spriteContainer->getContentSize().height - 75) );
        _layerContainerContent->setIgnoreAnchorPointForPosition(false);
        _layerContainerContent->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
        _layerContainerContent->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75) );
        _spriteContainer->addChild(_layerContainerContent);
    }
    _layerContainerContent->removeAllChildren();
    
    
    
    std::string text = MafUtils::format("t_ui_nebew_buff_popup_msg_%d", (int)_eNewbie);
    
    auto labelContent = Label::createWithTTF(GAME_TEXT(text), GAME_FONT, 28);
    labelContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelContent->setPosition(_layerContainerContent->getContentSize().width / 2, _layerContainerContent->getContentSize().height - 10);
    labelContent->setDimensions(_layerContainerContent->getContentSize().width * 0.9, labelContent->getContentSize().height);
    labelContent->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelContent->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContent->addChild(labelContent);
}

#pragma mark - schedule
void PopupBuffNewbie::scheduleClose(float dt)
{
    // time
    _timeNowClose -= dt;
    
    if ( _timeNowClose < 0 )
    {
        hide();
    }
}



#pragma mark - set, get, sort, add

#pragma mark - click
