//
//  LayerEventDrawLimitAni.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "LayerEventDrawLimitAni.h"

#include "GameObject/EventDraw/InfoEventDraw.h"

#include "GameUnit/UnitBehaviorHeroFrost.h"
#include "GameUnit/UnitBehaviorMonsterFrost.h"

#include "ManagerEvent/EventDrawLimitManager.h"

USING_NS_CC;

LayerEventDrawLimitAni* LayerEventDrawLimitAni::create(cocos2d::Size size)
{
    LayerEventDrawLimitAni* pRet = new(std::nothrow) LayerEventDrawLimitAni();
    if ( pRet && pRet->init(size) )
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

LayerEventDrawLimitAni::LayerEventDrawLimitAni(void) :
_bInitUI(false),

//
_machineBallNow(0),
_machineBallMax(0),

//
_uiContentsDefault(nullptr),
_uiContentsButtons(nullptr),

_uiMachineBG(nullptr),
_uiMachine(nullptr)
{
    
}

LayerEventDrawLimitAni::~LayerEventDrawLimitAni(void)
{
    
}

bool LayerEventDrawLimitAni::init(cocos2d::Size size)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    setContentSize(size);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setTouchEnabled(true);
    setBackGroundColor(Color3B::BLACK);
    setBackGroundColorType(BackGroundColorType::SOLID);
    setOpacity(200);
    
    // init
    initVar();
    initUi();
    
    return true;
}

void LayerEventDrawLimitAni::onEnter()
{
    ui::Layout::onEnter();
 
    if ( _bInitUI == false )
    {
        uiContainer();
        uiDefault();
        uiButtons();
        
        _bInitUI = true;
    }
    
    //
    drawDefault();
    drawButtons();
}


#pragma mark - init
void LayerEventDrawLimitAni::initVar()
{
    
}

void LayerEventDrawLimitAni::initUi()
{
    
}

#pragma mark - ui
void LayerEventDrawLimitAni::uiContainer()
{
    auto size = getContentSize();
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(size.width / 2, 0));
    addChild(layout);
    
    //
    _uiContentsDefault = ui::Layout::create();
    _uiContentsDefault->setContentSize(Size(size.width, size.height));
    _uiContentsDefault->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _uiContentsDefault->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(_uiContentsDefault);
    
    _uiContentsButtons = ui::Layout::create();
    _uiContentsButtons->setContentSize(Size(size.width, 150));
    _uiContentsButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsButtons->setPosition(Vec2(size.width / 2, 0));
    layout->addChild(_uiContentsButtons);
}

void LayerEventDrawLimitAni::uiDefault()
{
    auto layout = _uiContentsDefault;
    auto size = layout->getContentSize();
    
    _uiMachineBG = ui::ImageView::create("Assets/ui/event/gacha_machine_1_0.png");
    _uiMachineBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiMachineBG->setPosition(Vec2(size.width / 2, size.height));
    ((ui::Scale9Sprite*)_uiMachineBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    _uiMachineBG->setScale(3);
    layout->addChild(_uiMachineBG);
    
    _uiMachine = ui::ImageView::create("Assets/ui/event/gacha_machine_1_1.png");
    _uiMachine->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiMachine->setPosition(Vec2(size.width / 2, size.height));
    ((ui::Scale9Sprite*)_uiMachine->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    _uiMachine->setScale(3);
    layout->addChild(_uiMachine, 1000);
}

void LayerEventDrawLimitAni::uiButtons()
{
    auto layout = _uiContentsButtons;
    auto size = layout->getContentSize();
    
    auto uiSkip = ui::Button::create("Assets/ui_common/btn_2_4_on.png");
    uiSkip->setCapInsets(Rect(15,25,2,2));
    uiSkip->setScale9Enabled(true);
    uiSkip->setContentSize(Size(128, 50));
    uiSkip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    uiSkip->setPosition(Vec2(size.width - 50, size.height));
    uiSkip->addClickEventListener(CC_CALLBACK_1(LayerEventDrawLimitAni::onClickSkip, this));
    uiSkip->setVisible(false);
    uiSkip->setName("UI_SKIP");
    layout->addChild(uiSkip);
    
    auto uiText = ui::CText::create("SKIP", GAME_FONT, 24);
    uiText->setPosition(Vec2(uiSkip->getContentSize().width / 2, uiSkip->getContentSize().height / 2));
    uiText->setTextColor(Color4B(160, 148, 109, 255));
    uiSkip->addChild(uiText);
}

#pragma mark - ui : draw
void LayerEventDrawLimitAni::drawDefault()
{
    auto layout = _uiContentsDefault;
    
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.1);
    for ( int i = 1; i <= 6; i++ )
    {
        auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/event/gacha_machine_1_%d.png",i).c_str());
        spriteAni->getTexture()->setTexParameters(texParams);
        animation->addSpriteFrame(spriteAni->getSpriteFrame());
    }
    
    auto action1 = DelayTime::create(0.3);
    auto action2 = CallFunc::create([=](){
        
        if ( _machineBallMax > 0 )
        {
            if ( _machineBallNow < _machineBallMax )
            {
                _uiMachineBG->runAction(Sequence::create(ScaleTo::create(0.12, 3.1), ScaleTo::create(0.12, 3.0), NULL));
                _uiMachine->runAction(Sequence::create(ScaleTo::create(0.12, 3.1), ScaleTo::create(0.12, 3.0), CallFunc::create(CC_CALLBACK_0(LayerEventDrawLimitAni::actionBall, this)), NULL));
            }
            else
            {
                _uiMachineBG->stopAllActions();
                _uiMachine->getVirtualRenderer()->stopAllActions();
                _uiMachine->stopAllActions();
                
                //
                auto action1 = DelayTime::create(1.0);
                auto action2 = CallFunc::create(CC_CALLBACK_0(LayerEventDrawLimitAni::onResult, this));
                _uiMachine->runAction(Sequence::create(action1, action2, NULL));
            }
        }
    });
    
    _uiMachine->getVirtualRenderer()->stopAllActions();
    _uiMachine->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
    _uiMachine->getVirtualRenderer()->runAction(RepeatForever::create(Sequence::create(action1, action2, nullptr)));
}

void LayerEventDrawLimitAni::drawButtons()
{
    auto layout = _uiContentsButtons;
    
    if ( auto uiSkip = layout->getChildByName<ui::Button*>("UI_SKIP"); uiSkip != nullptr )
    {
        uiSkip->setVisible(false);
        if ( _machineBallMax > 0 )
        {
            uiSkip->setVisible(true);
        }
    }
}

#pragma mark - ui : action
void LayerEventDrawLimitAni::actionBall()
{
    auto layout = _uiContentsDefault;
    auto size = layout->getContentSize();
    
    if ( _machineBallNow >= _listDraw.size() )
    {
        return;
    }
    
    
    int idx = _listDraw.at(_machineBallNow);
    auto objItems = EventDrawLimitManager::getInstance()->getInfoItems(idx);
    
    int ballIdx = random(1, 3);
    int type = objItems->getType();
    if ( type == 1 )        ballIdx = 5;
    else if ( type == 2 )   ballIdx = 4;
    
    double moveY = random(600.0 - 30 * _machineBallNow, 600.0 - 25 * _machineBallNow) * -1;
    double moveX = random(0.0, 120.0);
    if ( _machineBallNow % 2 == 0 )
    {
        moveX *= -1;
    }
    
    auto rotate = random(0, 120);
    if ( random(0, 1) == 0 )
    {
        rotate *= -1;
    }
    
    //
    auto uiBG = ui::Layout::create();
    uiBG->setContentSize(Size(40, 40));
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiBG->setPosition(Vec2(size.width / 2, size.height - 350));
    layout->addChild(uiBG, moveY * -1);
    
    if ( type == 1 )
    {
        auto uiEffect = ui::ImageView::create("Assets/ui_common/box_item_fx1_2.png");
        uiEffect->setPosition(Vec2(uiBG->getContentSize().width / 2, uiBG->getContentSize().height / 2));
        uiEffect->runAction(RepeatForever::create(RotateBy::create(2, 180)));
        ((ui::Scale9Sprite*)uiEffect->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiEffect->setScale(1.5);
        uiBG->addChild(uiEffect, -1);
    }
    
    auto uiBall = ui::ImageView::create(MafUtils::format("Assets/ui/event/gacha_ball_%d_0.png", ballIdx));
    uiBall->setPosition(Vec2(uiBG->getContentSize().width / 2, uiBG->getContentSize().height / 2));
    ((ui::Scale9Sprite*)uiBall->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiBall->setScale(1 + (moveY * -1) / 300.0);
    uiBG->addChild(uiBall);
    
    auto action1 = EaseBounceOut::create(MoveBy::create(0.4, Vec2(moveX, moveY)));
    auto action2 = RotateBy::create(0.4, rotate);
    auto spawn = Spawn::create(action1, action2, NULL);
    uiBG->runAction(spawn);
    
    //
    auto scene = UtilsScene::getInstance()->getRunningScene();
    
    if ( type == 1 )
    {
        auto uiEffectWhite = ui::Layout::create();
        uiEffectWhite->setContentSize(UtilsRect::getVisibleSize());
        uiEffectWhite->setBackGroundColor(Color3B::WHITE);
        uiEffectWhite->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
        uiEffectWhite->setOpacity(0);
        uiEffectWhite->runAction(Sequence::create(FadeIn::create(0.05), DelayTime::create(0.1), FadeOut::create(0.05), RemoveSelf::create(), NULL));
        scene->addChild(uiEffectWhite, TAG_SCENE_TOAST);
    }
    
    
    //
    _machineBallNow++;
}

#pragma mark - ui : click
void LayerEventDrawLimitAni::onClickSkip(cocos2d::Ref* sender)
{
    onResult();
}

#pragma mark - set, get
void LayerEventDrawLimitAni::setCallbackFinish(const std::function<void(void)>& callback)
{
    _callbackFinish = callback;
}

#pragma mark - callback

#pragma mark - schedule

#pragma mark - game
void LayerEventDrawLimitAni::onDrawStart()
{
    _listDraw = EventDrawLimitManager::getInstance()->getListDraw();
    
    _machineBallNow = 0;
    _machineBallMax = (int)_listDraw.size();
    
    //
    drawButtons();
}

void LayerEventDrawLimitAni::onResult()
{
    if ( _callbackFinish != nullptr )
    {
        _callbackFinish();
        _callbackFinish = nullptr;
    }
        
    onHide();
        
}

void LayerEventDrawLimitAni::onHide()
{
    removeFromParent();
}
