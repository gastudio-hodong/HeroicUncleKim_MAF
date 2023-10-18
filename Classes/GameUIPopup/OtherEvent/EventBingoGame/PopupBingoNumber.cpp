//
//  PopupBingoNumber.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/07/27.
//

#include "PopupBingoNumber.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoItems.h"

#include "ManagerEvent/EventBingoManager.h"

USING_NS_CC;

PopupBingoNumber* PopupBingoNumber::create(int nNum, Vector<InfoItems*> listReward)
{
    PopupBingoNumber* pRet = new(std::nothrow) PopupBingoNumber();
    if ( pRet && pRet->init(nNum, listReward) )
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

PopupBingoNumber::PopupBingoNumber(void) :
_nNum(0),
_timer(10),
_lbGuide(nullptr),
_callbackClose(nullptr)
{
    _listReward.clear();
}

PopupBingoNumber::~PopupBingoNumber(void)
{
    if(_callbackClose != nullptr)
    {
        _callbackClose();
        _callbackClose = nullptr;
    }
    
    _listReward.clear();
}

bool PopupBingoNumber::init(int nNum, Vector<InfoItems*> listReward)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    _nNum = nNum;
    _listReward = listReward;
    
    // init
    initVar();
    initUi();
    
    //scheduleUpdate();
    
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
        
//        cocos2d::Rect rect = _spriteContainer->getBoundingBox();
//        if (rect.containsPoint(this->convertToNodeSpace(touch->getLocation())) == false)
//        {
            onHide();
        //}
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}
void PopupBingoNumber::update(float dt)
{
    _timer -= dt;
    
    if(_lbGuide != nullptr)
    {
        _lbGuide->setString(GAME_TEXT("t_ui_shop_daily_reward_msg_2") + "(" + std::to_string((int)_timer) + ")");
    }
    
    if(_timer <= 0)
    {
        onHide();
        return;
    }
}

#pragma mark - init
void PopupBingoNumber::initVar()
{
    _spriteContainer->setContentSize(Size(450, 260));
    
    float height = getContentSize().height;
    
    if(height >= 1327)
        height = 1327;
    
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _spriteContainer->setPositionY(height - 180);
}

void PopupBingoNumber::initUi()
{

    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_bingo_aqnum"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    uiContent();
}

#pragma mark - ui
void PopupBingoNumber::uiContent()
{
    auto lbNumber = Label::createWithTTF(std::to_string(_nNum), GAME_FONT, 80);
    lbNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbNumber->setPosition(_spriteContainer->getContentSize()/2);
    _spriteContainer->addChild(lbNumber);
    
   // _lbGuide = Label::createWithTTF(GAME_TEXT("t_ui_shop_daily_reward_msg_2") + "(" + std::to_string((int)_timer) + ")", GAME_FONT, 24);
    _lbGuide = Label::createWithTTF(GAME_TEXT("t_ui_shop_daily_reward_msg_2"), GAME_FONT, 24);
    _lbGuide->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lbGuide->setPosition(_spriteContainer->getContentSize().width/2, 30);
    _spriteContainer->addChild(_lbGuide);
}

#pragma mark - utills
void PopupBingoNumber::onHide()
{
    
    if(_listReward.size() > 0)
        PopupToastItems::show(_listReward);
    
    if(_callbackClose != nullptr)
    {
        _callbackClose();
        _callbackClose = nullptr;
    }
    
    hide();
}

void PopupBingoNumber::onClickClose(Ref* pSender)
{
    onHide();
}
void PopupBingoNumber::setCallback(const std::function<void(void)>& pCallback)
{
    _callbackClose = pCallback;
}
