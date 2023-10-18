//
//  PopupAdventureTimeReduction.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/06/08.
//

#include "PopupAdventureTimeReduction.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoSetting.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupAdventureTimeReduction* PopupAdventureTimeReduction::create(int nAdventureID)
{
    PopupAdventureTimeReduction* pRet = new(std::nothrow) PopupAdventureTimeReduction();
    if ( pRet && pRet->init(nAdventureID) )
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

PopupAdventureTimeReduction::PopupAdventureTimeReduction(void) :
_nAdventureID(0),
_nCount(1),
_nDefaultPrice(0),
_nReductionTime(0),
_callbackResult(nullptr),

_layerContainerContents(nullptr),
_layerContainerButton(nullptr),

_labelNowTime(nullptr),
_labelReductionTime(nullptr),
_labelPrice(nullptr)
{
    
}

PopupAdventureTimeReduction::~PopupAdventureTimeReduction(void)
{
    
}

bool PopupAdventureTimeReduction::init(int nAdventureID)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    _nAdventureID = nAdventureID;
    
    _nDefaultPrice = 20;
    auto objSetting = AdventureManager::getInstance()->getSetting("advt_time_skip_cost");
    if(objSetting != nullptr)
        _nDefaultPrice = objSetting->getValueToInt();
    
    //분 단위
    _nReductionTime = 30;
    
    if(std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str()) < _nDefaultPrice)
    {
        _nCount = 0;
    }
    
    // init
    initVar();
    initUi();
    
    scheduleUpdate();
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE, this);
    
    return true;
}
#pragma mark - Override
void PopupAdventureTimeReduction::update(float dt)
{
    auto objParty = AdventureManager::getInstance()->getInfoParty(_nAdventureID);
    
    if(objParty == nullptr)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        
        hide();
        return;
    }
    refreshTimer();
    
}
void PopupAdventureTimeReduction::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if(eType == E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE)
        this->setLocalZOrder(ZORDER_SCENE_POPUP);
}
void PopupAdventureTimeReduction::show()
{
    PopupBase::show();
    
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        this->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
}
#pragma mark - init
void PopupAdventureTimeReduction::initVar()
{

}

void PopupAdventureTimeReduction::initUi()
{
    Size size = Size(500, 450);
    
    //
    _spriteContainer->setContentSize( size );
    
    //
    _layerContainerButton = Layer::create();
    _layerContainerButton->setContentSize( Size(size.width - 50, 89) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_layerContainerButton);
    
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, size.height - 75 - 25 - 25 - _layerContainerButton->getContentSize().height - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25) );
    _spriteContainer->addChild(_layerContainerContents);
    
    _layerContainerCurrency = Layer::create();
    _layerContainerCurrency->setContentSize( Size(_spriteContainer->getContentSize().width, 60) );
    _layerContainerCurrency->setIgnoreAnchorPointForPosition(false);
    _layerContainerCurrency->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerCurrency->setPosition( _spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height + 5);
    _spriteContainer->addChild(_layerContainerCurrency);
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_expedition_cat_skill_title_1"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAdventureTimeReduction::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    uiContents();
    uiButton();
    uiCurrency();
}

#pragma mark - ui
void PopupAdventureTimeReduction::uiContents()
{
    //
    _layerContainerContents->removeAllChildren();
    _labelReductionTime = nullptr;
    _labelNowTime = nullptr;
    _labelPrice = nullptr;
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _layerContainerContents->addChild(menu);
    
    auto lbGuide = Label::createWithTTF(GAME_TEXT("t_ui_advt_time_skip_msg"), GAME_FONT, 24);
    lbGuide->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbGuide->setDimensions(_layerContainerContents->getContentSize().width, lbGuide->getContentSize().height);
    lbGuide->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    lbGuide->setPosition(_layerContainerContents->getContentSize().width/2, _layerContainerContents->getContentSize().height);
    lbGuide->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _layerContainerContents->addChild(lbGuide);
    
    //
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setContentSize(Size(82, 82));
    sprBox->setPosition(_layerContainerContents->getContentSize().width/2, _layerContainerContents->getContentSize().height/2 + 15);
    _layerContainerContents->addChild(sprBox);
    {
        auto sprIcon = Sprite::create("Assets/icon/icon_coin_2.png");
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprIcon->setPosition(sprBox->getContentSize()/2);
        sprBox->addChild(sprIcon);
        
        _labelPrice = MafNode::createBmFont(std::to_string(_nCount * _nDefaultPrice), GAME_BM_FONT_AL, 20);
        _labelPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _labelPrice->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        _labelPrice->setDimensions(sprBox->getContentSize().width, _labelPrice->getContentSize().height);
        _labelPrice->setPosition(sprBox->getContentSize().width/2, _labelPrice->getContentSize().height/2);
        sprBox->addChild(_labelPrice);
    }
    
    auto itemDown = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/arrow_L01.png"), nullptr, CC_CALLBACK_1(PopupAdventureTimeReduction::onClickArrow, this));
    itemDown->setTag(-1);
    itemDown->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    itemDown->setPosition(sprBox->getPosition() + Vec2::LEFT * sprBox->getContentSize().width);
    itemDown->setTouchRect(itemDown->getContentSize() * 1.5f);
    menu->addChild(itemDown);
    
    auto itemUp = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/arrow_R01.png"), nullptr, CC_CALLBACK_1(PopupAdventureTimeReduction::onClickArrow, this));
    itemUp->setTag(1);
    itemUp->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    itemUp->setPosition(sprBox->getPosition() + Vec2::RIGHT * sprBox->getContentSize().width);
    itemUp->setTouchRect(itemDown->getContentSize() * 1.5f);
    menu->addChild(itemUp);
    
    auto objParty = AdventureManager::getInstance()->getInfoParty(_nAdventureID);
    
    auto time = objParty->getEndTime() - UtilsDate::getInstance()->getTime();
    
    int hour = (int)time / 3600;
    int min = ((int)time % 3600) / 60;
    int second = (int)time % 3600 % 60;
    
    if(hour < 0)
        hour = 0;
    if(min < 0)
        min = 0;
    if(second < 0)
        second = 0;
    
    std::string strTimer = GAME_TEXT("t_ui_time_now") + " : %02d:%02d:%02d";
    
    _labelNowTime = Label::createWithTTF(MafUtils::format(strTimer.c_str(), hour,min,second), GAME_FONT, 24);
    _labelNowTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _labelNowTime->setPosition(sprBox->getPosition() + Vec2::DOWN * (sprBox->getContentSize().height/2 + 25));
    _labelNowTime->setColor(Color3B(128, 128, 128));
    _layerContainerContents->addChild(_labelNowTime);
    
    time -= (_nReductionTime * 60) * _nCount;
    
    if(time <= 0)
        time = 0;
    
    if(time > 0)
    {
        hour = (int)time / 3600;
        min = ((int)time % 3600) / 60;
        second = (int)time % 3600 % 60;
    }
    else
    {
        hour = 0;
        min = 0;
        second = 0;
    }
    
    strTimer = GAME_TEXT("t_ui_time_reduction") + " : %02d:%02d:%02d";
    
    _labelReductionTime = Label::createWithTTF(MafUtils::format(strTimer.c_str(), hour,min,second), GAME_FONT, 24);
    _labelReductionTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _labelReductionTime->setPosition(_labelNowTime->getPosition() + Vec2::DOWN * (_labelNowTime->getContentSize().height));
    _labelReductionTime->setColor(COLOR_TITLE);
    _layerContainerContents->addChild(_labelReductionTime);
    
}

void PopupAdventureTimeReduction::uiButton()
{
    //
    _layerContainerButton->removeAllChildren();
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);
    
    auto itemConfirm = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr, CC_CALLBACK_1(PopupAdventureTimeReduction::onClickReduction, this));
    menu->addChild(itemConfirm);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_confirm"), GAME_FONT, 24);
        labelText->setPosition(itemConfirm->getContentSize().width / 2, itemConfirm->getContentSize().height / 2);
        itemConfirm->addChild(labelText);
    }
    
    //
    menu->alignItemsHorizontallyWithPadding(10);
}

void PopupAdventureTimeReduction::uiCurrency()
{
    _layerContainerCurrency->removeAllChildren();
    
    //
    auto spriteBG = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale2_4.png", Rect::ZERO, Rect(27,27,6,6));
    spriteBG->setContentSize(_layerContainerCurrency->getContentSize());
    spriteBG->setPosition(_layerContainerCurrency->getContentSize().width / 2, _layerContainerCurrency->getContentSize().height / 2);
    _layerContainerCurrency->addChild(spriteBG);
    
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::COSTUME_COIN);
    
    auto uiCurrency = UICurrency::create(list, _layerContainerCurrency->getContentSize());
    uiCurrency->setPosition(Vec2(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height / 2));
    spriteBG->addChild(uiCurrency);
}

void PopupAdventureTimeReduction::refreshTimer()
{
    auto objParty = AdventureManager::getInstance()->getInfoParty(_nAdventureID);
    auto time = objParty->getEndTime() - UtilsDate::getInstance()->getTime();
    
    if(time <= 0 )
    {
        unscheduleUpdate();
        hide();
        return;
    }
    
    int hour = (int)time / 3600;
    int min = ((int)time % 3600) / 60;
    int second = (int)time % 3600 % 60;
    
    if(hour < 0)
        hour = 0;
    if(min < 0)
        min = 0;
    if(second < 0)
        second = 0;
    
    std::string strTimer =  GAME_TEXT("t_ui_time_now") + " : %02d:%02d:%02d";
    
    _labelNowTime->setString(MafUtils::format(strTimer.c_str(), hour,min,second));
    
    time -= (_nReductionTime * 60) * _nCount;
    
    if(time <= 0)
        time = 0;
    
    if(time > 0)
    {
        hour = (int)time / 3600;
        min = ((int)time % 3600) / 60;
        second = (int)time % 3600 % 60;
    }
    else
    {
        hour = 0;
        min = 0;
        second = 0;
    }
    
    strTimer = GAME_TEXT("t_ui_time_reduction") + " : %02d:%02d:%02d";
    
    _labelReductionTime->setString(MafUtils::format(strTimer.c_str(), hour, min, second));
}

#pragma mark - set, get, sort, add
void PopupAdventureTimeReduction::setCallbackResult(const std::function<void(bool, int)>& callback)
{
    _callbackResult = callback;
}

#pragma mark - click
void PopupAdventureTimeReduction::onClickClose(Ref* sender)
{
    hide();
}

void PopupAdventureTimeReduction::onClickArrow(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    int nTag = item->getTag();
    auto objParty = AdventureManager::getInstance()->getInfoParty(_nAdventureID);
    
    if(objParty == nullptr)
        return;
    
    auto time = objParty->getEndTime() - UtilsDate::getInstance()->getTime();
    int min = (int)time / 60;
    int countMax = min / 30 + 1;
    int currentCoin = std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str());
    if(currentCoin < countMax * _nDefaultPrice)
    {
        countMax = currentCoin / _nDefaultPrice;
    }
    
    _nCount += nTag;
    
    if(countMax > 0)
    {
        if(_nCount > countMax)
            _nCount = 1;
        else if(_nCount < 1)
            _nCount = countMax;
    }
    else
        _nCount = 0;
    
    _labelPrice->setString(std::to_string(_nCount * _nDefaultPrice));
}

void PopupAdventureTimeReduction::onClickReduction(cocos2d::Ref* sender)
{
    auto objParty = AdventureManager::getInstance()->getInfoParty(_nAdventureID);
    if(objParty == nullptr)
        return;
    
    PopupLoading::show();
    AdventureManager::getInstance()->requestAdventureTimeReduction(CC_CALLBACK_2(PopupAdventureTimeReduction::callbakcReduction, this), objParty->getInfoStage(), _nCount * _nDefaultPrice);
}

#pragma mark - callback

void PopupAdventureTimeReduction::callbakcReduction(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if(_callbackResult != nullptr)
        _callbackResult(bResult, nResult);
    
    hide();
}
