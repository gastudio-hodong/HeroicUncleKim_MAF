//
//  PopupPurchaseDailyReward.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupPurchaseDailyReward.h"

USING_NS_CC;

PopupPurchaseDailyReward* PopupPurchaseDailyReward::create(std::string strMsg)
{
    PopupPurchaseDailyReward* pRet = new(std::nothrow) PopupPurchaseDailyReward();
    if ( pRet && pRet->init(strMsg) )
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

PopupPurchaseDailyReward::PopupPurchaseDailyReward(void) :
_strMsg(""),

_layerContainerContent(nullptr)
{
    
}

PopupPurchaseDailyReward::~PopupPurchaseDailyReward(void)
{
    
}

bool PopupPurchaseDailyReward::init(std::string strMsg)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    _strMsg = strMsg;
    
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

#pragma mark - init
void PopupPurchaseDailyReward::initVar()
{
    
}

void PopupPurchaseDailyReward::initUi()
{
    Size size = Size(497, 250);
    
    //
    _spriteContainer->setContentSize( size );

    //
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_shop_daily_reward_title"), GAME_FONT, 32);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    labelTitle->setColor(COLOR_TITLE);
    _spriteContainer->addChild(labelTitle);
    
    uiContent();
}

#pragma mark - ui
void PopupPurchaseDailyReward::uiContent()
{
    if ( _layerContainerContent == nullptr )
    {
        _layerContainerContent = Layer::create();
        _layerContainerContent->setContentSize( Size(_spriteContainer->getContentSize().width, _spriteContainer->getContentSize().height  - 75) );
        _layerContainerContent->setIgnoreAnchorPointForPosition(false);
        _layerContainerContent->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
        _layerContainerContent->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75) );
        _spriteContainer->addChild(_layerContainerContent);
    }
    _layerContainerContent->removeAllChildren();
    
    auto labelContent = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_shop_daily_reward_msg_1", _strMsg.c_str()), GAME_FONT, 28);
    labelContent->setPosition(_layerContainerContent->getContentSize().width / 2, _layerContainerContent->getContentSize().height / 2 + 10);
    labelContent->setDimensions(_layerContainerContent->getContentSize().width * 0.9, labelContent->getContentSize().height);
    labelContent->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelContent->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContent->addChild(labelContent);
    
    auto labelInfo = Label::createWithTTF(GAME_TEXT("t_ui_shop_daily_reward_msg_2"), GAME_FONT, 20);
    labelInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    labelInfo->setPosition(_layerContainerContent->getContentSize().width / 2, 10);
    _layerContainerContent->addChild(labelInfo);
}


