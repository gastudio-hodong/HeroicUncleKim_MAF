//
//  LayerEventDrawLimitResult.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "LayerEventDrawLimitResult.h"

#include "GameObject/EventDraw/InfoEventDraw.h"

#include "GameUnit/UnitBehaviorHeroFrost.h"
#include "GameUnit/UnitBehaviorMonsterFrost.h"

#include "ManagerEvent/EventDrawLimitManager.h"

USING_NS_CC;

LayerEventDrawLimitResult* LayerEventDrawLimitResult::create(cocos2d::Size size)
{
    LayerEventDrawLimitResult* pRet = new(std::nothrow) LayerEventDrawLimitResult();
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

LayerEventDrawLimitResult::LayerEventDrawLimitResult(void) :
_bInitUI(false),

//

//
_uiContentsTitle(nullptr),
_uiContentsReward(nullptr),
_uiContentsButtons(nullptr)
{
    
}

LayerEventDrawLimitResult::~LayerEventDrawLimitResult(void)
{
    log("LayerEventDrawLimitResult::~LayerEventDrawLimitResult");
}

bool LayerEventDrawLimitResult::init(cocos2d::Size size)
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

void LayerEventDrawLimitResult::onEnter()
{
    ui::Layout::onEnter();
 
    if ( _bInitUI == false )
    {
        uiContainer();
        uiTitle();
        uiReward();
        uiButtons();
        
        _bInitUI = true;
    }
    
    //
    drawReward();
    drawButtons();
}


#pragma mark - init
void LayerEventDrawLimitResult::initVar()
{
    auto listDraw = EventDrawLimitManager::getInstance()->getListDraw();
    
    _listEventDraw.clear();
    for ( auto idx : listDraw )
    {
        auto obj = EventDrawLimitManager::getInstance()->getInfoItems(idx);
        if ( obj == nullptr )
            continue;
        
        _listEventDraw.pushBack(obj);
    }
}

void LayerEventDrawLimitResult::initUi()
{
    
}

#pragma mark - ui
void LayerEventDrawLimitResult::uiContainer()
{
    auto size = getContentSize();
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(size.width / 2, 0));
    addChild(layout);
    
    //
    _uiContentsTitle = ui::Layout::create();
    _uiContentsTitle->setContentSize(Size(size.width, 110));
    _uiContentsTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContentsTitle->setPosition(Vec2(size.width / 2, size.height));
    layout->addChild(_uiContentsTitle);
    
    _uiContentsButtons = ui::Layout::create();
    _uiContentsButtons->setContentSize(Size(size.width, 150));
    _uiContentsButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsButtons->setPosition(Vec2(size.width / 2, 0));
    layout->addChild(_uiContentsButtons);
    
    _uiContentsReward = ui::Layout::create();
    _uiContentsReward->setContentSize(Size(size.width, size.height - _uiContentsTitle->getContentSize().height - _uiContentsButtons->getContentSize().height));
    _uiContentsReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsReward->setPosition(Vec2(size.width / 2, _uiContentsButtons->getContentSize().height));
    layout->addChild(_uiContentsReward);
}

void LayerEventDrawLimitResult::uiTitle()
{
    auto layout = _uiContentsTitle;
    auto size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/base_text_1_4.png");
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(uiBG);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_token_event_msg2"), GAME_FONT, 24);
        uiText->setPositionType(PositionType::PERCENT);
        uiText->setPositionPercent(Vec2::ANCHOR_MIDDLE);
        uiText->setTextAreaSize(Size(uiBG->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiBG->addChild(uiText);
    }
    
    auto uiClose = ui::Button::create("Assets/ui_common/btn_x.png");
    uiClose->setPosition(Vec2(size.width - 50, size.height / 2));
    uiClose->addClickEventListener(CC_CALLBACK_1(LayerEventDrawLimitResult::onClickClose, this));
    layout->addChild(uiClose);
}

void LayerEventDrawLimitResult::uiReward()
{
    auto layout = _uiContentsReward;
    auto size = layout->getContentSize();
    
    auto uiReward = ui::Layout::create();
    uiReward->setContentSize(Size(size.width, size.height));
    uiReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiReward->setPosition(Vec2(size.width / 2, size.height / 2));
    uiReward->setName("UI_REWARD");
    layout->addChild(uiReward);
}

void LayerEventDrawLimitResult::uiButtons()
{
    auto layout = _uiContentsButtons;
    auto size = layout->getContentSize();
    
    auto uiBtns = ui::Layout::create();
    uiBtns->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiBtns->setPositionType(ui::Widget::PositionType::PERCENT);
    uiBtns->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    uiBtns->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiBtns->setName("UI_BTNS");
    layout->addChild(uiBtns);
    
    auto uiDrawOnce = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
    uiDrawOnce->setCapInsets(Rect(33,33,2,2));
    uiDrawOnce->setScale9Enabled(true);
    uiDrawOnce->setContentSize(Size(290, 100));
    uiDrawOnce->addClickEventListener(CC_CALLBACK_1(LayerEventDrawLimitResult::onClickDraw, this));
    uiDrawOnce->setTag(1);
    uiDrawOnce->setName("UI_DRAW_ONCE");
    uiBtns->addChild(uiDrawOnce);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_token_btn_buy_single_1"), GAME_FONT, 28);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiText->setPosition(Vec2(uiDrawOnce->getContentSize().width / 2, uiDrawOnce->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(uiDrawOnce->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiDrawOnce->addChild(uiText);
    }
    
    auto uiDrawMany = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
    uiDrawMany->setCapInsets(Rect(33,33,2,2));
    uiDrawMany->setScale9Enabled(true);
    uiDrawMany->setContentSize(Size(290, 100));
    uiDrawMany->addClickEventListener(CC_CALLBACK_1(LayerEventDrawLimitResult::onClickDraw, this));
    uiDrawMany->setTag(10);
    uiDrawMany->setName("UI_DRAW_MANY");
    uiBtns->addChild(uiDrawMany);
    {
        auto params = ui::LinearLayoutParameter::create();
        params->setMargin(ui::Margin(10, 0, 0, 0));
        uiDrawMany->setLayoutParameter(params);
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_token_btn_buy_multiple_1"), GAME_FONT, 28);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiText->setPosition(Vec2(uiDrawMany->getContentSize().width / 2, uiDrawMany->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(uiDrawMany->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiDrawMany->addChild(uiText);
    }
    
    // skip
    auto uiSkip = ui::Button::create("Assets/ui_common/btn_2_4_on.png");
    uiSkip->setCapInsets(Rect(15,25,2,2));
    uiSkip->setScale9Enabled(true);
    uiSkip->setContentSize(Size(128, 50));
    uiSkip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    uiSkip->setPosition(Vec2(size.width - 50, size.height));
    uiSkip->addClickEventListener(CC_CALLBACK_1(LayerEventDrawLimitResult::onClickSkip, this));
    uiSkip->setVisible(false);
    uiSkip->setName("UI_SKIP");
    layout->addChild(uiSkip);
    {
        auto uiText = ui::CText::create("SKIP", GAME_FONT, 24);
        uiText->setPosition(Vec2(uiSkip->getContentSize().width / 2, uiSkip->getContentSize().height / 2));
        uiText->setTextColor(Color4B(160, 148, 109, 255));
        uiSkip->addChild(uiText);
    }
}

#pragma mark - ui : draw
void LayerEventDrawLimitResult::drawReward()
{
    auto layout = _uiContentsReward;
    
    auto uiReward = layout->getChildByName<ui::Layout*>("UI_REWARD");
    if ( uiReward != nullptr )
    {
        uiReward->removeAllChildren();
        
        //
        for ( int i = 0; i < _listEventDraw.size(); i++ )
        {
            auto objEventDraw = _listEventDraw.at(i);
            auto objItem = objEventDraw->getReward();
            if ( objItem == nullptr )
                continue;
            
            int ballIdx = random(1, 3);
            if ( objEventDraw->getType() == 1 )         ballIdx = 5;
            else if ( objEventDraw->getType() == 2 )    ballIdx = 4;
                
            auto pathBG = "Assets/ui/event/event_gacha_box_01.png";
            if ( objEventDraw->getType() == 1 )
            {
                pathBG = "Assets/ui/event/event_gacha_box_02.png";
            }
            
            auto uiBG = ui::ImageView::create(pathBG);
            uiReward->addChild(uiBG);
            if ( objEventDraw->getType() == 1 )
            {
                auto uiEffect = ui::ImageView::create("Assets/ui_common/box_item_fx1_2.png");
                uiEffect->setPosition(Vec2(uiBG->getContentSize().width / 2, uiBG->getContentSize().height / 2));
                uiEffect->runAction(RepeatForever::create(RotateBy::create(2, 180)));
                ((ui::Scale9Sprite*)uiEffect->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
                uiEffect->setScale(1.5);
                uiBG->addChild(uiEffect, -1);
            }
            
            auto uiIcon = ui::ImageView::create(objItem->getPath());
            uiIcon->setPosition(Vec2(uiBG->getContentSize().width / 2, uiBG->getContentSize().height / 2 + 10));
            uiBG->addChild(uiIcon);
            
            auto uiCount = ui::CText::create(objItem->getCount(), GAME_FONT, 20);
            uiCount->setPosition(Vec2(uiBG->getContentSize().width / 2, 20));
            uiBG->addChild(uiCount);
            
            auto uiBall = ui::ImageView::create(MafUtils::format("Assets/ui/event/gacha_ball_%d_1.png", ballIdx));
            uiBall->setPosition(Vec2(uiBG->getContentSize().width / 2, uiBG->getContentSize().height / 2));
            ((ui::Scale9Sprite*)uiBall->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiBall->setScale(3);
            uiBall->setTouchEnabled(true);
            uiBall->addClickEventListener(CC_CALLBACK_1(LayerEventDrawLimitResult::onClickReward, this));
            uiBall->setName("UI_BALL");
            uiBall->setTag(ballIdx);
            uiBG->addChild(uiBall);
            
            _listBall.pushBack(uiBall);
            
        }
    
        UtilsWidget::getInstance()->sortWidgetGrid(uiReward, 3, uiReward->getContentSize().width, uiReward->getContentSize().height, 100, false, false);
    }
}

void LayerEventDrawLimitResult::drawButtons()
{
    auto layout = _uiContentsButtons;
    
    if ( auto uiBtns = layout->getChildByName<ui::Layout*>("UI_BTNS"); uiBtns != nullptr )
    {
        auto size = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiBtns);
        uiBtns->setContentSize(size);
        uiBtns->requestDoLayout();
    }

    if ( auto uiDrawOnce = utils::findChild<ui::Button*>(layout, "UI_DRAW_ONCE"); uiDrawOnce != nullptr )
    {
        uiDrawOnce->removeChildByName("UI_COST");
        
        auto costText =  GAME_TEXTFORMAT("t_ui_token_btn_buy_single_2", EventDrawLimitManager::getInstance()->getTokenNow());
        
        auto uiCost = UtilsWidget::getInstance()->createIconText("Assets/ui/event/icon_token.png", costText, uiDrawOnce->getContentSize().width - 10, 24, 15);
        uiCost->setPosition(Vec2(uiDrawOnce->getContentSize().width / 2, 3 + 22.5));
        uiCost->setName("UI_COST");
        uiDrawOnce->addChild(uiCost);
        
        if ( auto uiText = utils::findChild<ui::CText*>(uiCost, "WIDGET_TEXT"); uiText != nullptr )
        {
            uiText->setTextColor(Color4B(199, 106, 0, 255));
        }
        
        if ( _listBall.size() > 0 )
        {
            uiDrawOnce->loadTextureNormal("Assets/ui_common/btn_1_off.png");
        }
        else
        {
            uiDrawOnce->loadTextureNormal("Assets/ui_common/btn_1_1_on.png");
        }
    }
    
    if ( auto uiDrawMany = utils::findChild<ui::Button*>(layout, "UI_DRAW_MANY"); uiDrawMany != nullptr )
    {
        uiDrawMany->removeChildByName("UI_COST");
        
        auto costText =  GAME_TEXTFORMAT("t_ui_token_btn_buy_multiple_2", EventDrawLimitManager::getInstance()->getTokenNow());
        
        auto uiCost = UtilsWidget::getInstance()->createIconText("Assets/ui/event/icon_token.png", costText, uiDrawMany->getContentSize().width - 10, 24, 15);
        uiCost->setPosition(Vec2(uiDrawMany->getContentSize().width / 2, 3 + 22.5));
        uiCost->setName("UI_COST");
        uiDrawMany->addChild(uiCost);
        
        if ( auto uiText = utils::findChild<ui::CText*>(uiCost, "WIDGET_TEXT"); uiText != nullptr )
        {
            uiText->setTextColor(Color4B(199, 106, 0, 255));
        }
        
        if ( _listBall.size() > 0 )
        {
            uiDrawMany->loadTextureNormal("Assets/ui_common/btn_1_off.png");
        }
        else
        {
            uiDrawMany->loadTextureNormal("Assets/ui_common/btn_1_1_on.png");
        }
    }
    
    if ( auto uiSkip = layout->getChildByName<ui::Button*>("UI_SKIP"); uiSkip != nullptr )
    {
        if ( _listBall.size() > 0 )
        {
            uiSkip->setVisible(true);
        }
        else
        {
            uiSkip->setVisible(false);
        }
    }
}

#pragma mark - ui : action
void LayerEventDrawLimitResult::actionBall(cocos2d::ui::ImageView* uiBall)
{
    // erase
    _listBall.eraseObject(uiBall);
    
    //
    int ballIdx = uiBall->getTag();
    
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.1);
    for ( int i = 1; i <= 5; i++ )
    {
        auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/event/gacha_ball_%d_%d.png", ballIdx, i).c_str());
        spriteAni->getTexture()->setTexParameters(texParams);
        animation->addSpriteFrame(spriteAni->getSpriteFrame());
    }
    auto animate = Animate::create(animation);
    
    uiBall->getVirtualRenderer()->stopAllActions();
    uiBall->getVirtualRenderer()->runAction(animate);
    
    auto action1 = DelayTime::create(animate->getDuration());
    auto action2 = RemoveSelf::create();
    uiBall->runAction(Sequence::create(action1, action2, NULL));
}

#pragma mark - ui : click
void LayerEventDrawLimitResult::onClickClose(cocos2d::Ref* sender)
{
    if ( _listBall.size() > 0 )
    {
        for ( auto iter = _listBall.rbegin(); iter != _listBall.rend(); ++iter )
        {
            auto uiBall = *iter;
            actionBall(uiBall);
        }
        
        //
        drawButtons();
        return;
    }
    
    onHide();
}

void LayerEventDrawLimitResult::onClickReward(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBall = dynamic_cast<ui::ImageView*>(sender);
    
    actionBall(uiBall);
    
    //
    drawButtons();
}

void LayerEventDrawLimitResult::onClickDraw(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    int count = uiBtn->getTag();
    
    if ( _listBall.size() > 0 )
    {
        return;
    }
    
    
    //
    if ( _callbackAgain != nullptr )
    {
        _callbackAgain(count);
        _callbackAgain = nullptr;
    }
    
    //
    onHide();
    
}

void LayerEventDrawLimitResult::onClickSkip(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    for ( auto iter = _listBall.rbegin(); iter != _listBall.rend(); ++iter )
    {
        auto uiBall = *iter;
        actionBall(uiBall);
    }
    
    //
    drawButtons();
}

#pragma mark - set, get
void LayerEventDrawLimitResult::setCallbackAgain(const std::function<void(int)>& callback)
{
    _callbackAgain = callback;
}

#pragma mark - callback

#pragma mark - schedule

#pragma mark - game
void LayerEventDrawLimitResult::onHide()
{
    removeFromParent();
}
