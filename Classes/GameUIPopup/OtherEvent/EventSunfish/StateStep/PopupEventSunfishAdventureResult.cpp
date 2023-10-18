//
//  PopupEventSunfishAdventureResult.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "PopupEventSunfishAdventureResult.h"

#include "ManagerEvent/EventSunfishManager.h"

USING_NS_CC;

PopupEventSunfishAdventureResult::PopupEventSunfishAdventureResult(void) :
_bInitUI(false),

//
_bResult(false),
_levelIdx(0),
_adventureIdx(0),

//
_uiContentsInfo(nullptr),
_uiContentsButton(nullptr),

_uiTitle(nullptr)
{
    
}

PopupEventSunfishAdventureResult::~PopupEventSunfishAdventureResult(void)
{
    
}

bool PopupEventSunfishAdventureResult::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    // init
    initVar();
    initUi();
    
    return true;
}

void PopupEventSunfishAdventureResult::onEnter()
{
    PopupBase::onEnter();
 
    if ( _bInitUI == false )
    {
        uiContainer();
        uiInfo();
        uiButtons();
        
        _bInitUI = true;
    }
    
    //
    drawInfo();
    drawButtons();
}


#pragma mark - init
void PopupEventSunfishAdventureResult::initVar()
{
    
}

void PopupEventSunfishAdventureResult::initUi()
{
    // size
    _spriteContainer->setContentSize( Size(700, 818) );
    
    // label title
    _uiTitle = ui::CText::create(GAME_TEXT("t_ui_event_sunfish_popup_title_1"), GAME_FONT, 32);
    _uiTitle->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5));
    _uiTitle->setTextAreaSize(Size(_spriteContainer->getContentSize().width - 200, _uiTitle->getContentSize().height));
    _uiTitle->setTextOverflow(Label::Overflow::SHRINK);
    _uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    _uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _uiTitle->setTextColor(Color4B(COLOR_TITLE));
    _spriteContainer->addChild(_uiTitle);
    
    // item close
    auto uiClose = ui::Button::create("Assets/ui_common/btn_x.png");
    uiClose->setPosition(Vec2(_spriteContainer->getContentSize().width - uiClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5));
    uiClose->addClickEventListener(CC_CALLBACK_1(PopupEventSunfishAdventureResult::onClickClose, this));
    _spriteContainer->addChild(uiClose);
}

#pragma mark - ui
void PopupEventSunfishAdventureResult::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 10;
    size.height = size.height - 75 - 25;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(size.width / 2, 25));
    _spriteContainer->addChild(layout);
    
    //
    _uiContentsButton = ui::Layout::create();
    _uiContentsButton->setContentSize(Size(size.width, 78));
    _uiContentsButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsButton->setPosition(Vec2(size.width / 2, 0));
    layout->addChild(_uiContentsButton);
    
    _uiContentsInfo = ui::Layout::create();
    _uiContentsInfo->setContentSize(Size(size.width, size.height - _uiContentsButton->getContentSize().height));
    _uiContentsInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContentsInfo->setPosition(Vec2(size.width / 2, size.height));
    layout->addChild(_uiContentsInfo);
}

void PopupEventSunfishAdventureResult::uiInfo()
{
    auto layout = _uiContentsInfo;
    auto size = layout->getContentSize();
    
    //
    auto uiTop = ui::Layout::create();
    uiTop->setContentSize(Size(size.width, 536));
    uiTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiTop->setPosition(Vec2(size.width / 2, size.height));
    layout->addChild(uiTop);
    {
        //
        auto uiInfoTitle = ui::Layout::create();
        uiInfoTitle->setContentSize(Size(uiTop->getContentSize().width, 248));
        uiInfoTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiInfoTitle->setPosition(Vec2(uiTop->getContentSize().width / 2, uiTop->getContentSize().height - 5));
        uiInfoTitle->setName("UI_INFO_TITLE");
        uiTop->addChild(uiInfoTitle);
        {
            auto uiBG = ui::ImageView::create("Assets/ui/event/lobster_bg_3_1.png");
            uiBG->setPosition(Vec2(uiInfoTitle->getContentSize().width / 2, uiInfoTitle->getContentSize().height / 2));
            ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiBG->setScale(3);
            uiBG->setName("UI_BG");
            uiInfoTitle->addChild(uiBG);
        }
        
        //
        auto uiInfoDesc = ui::Layout::create();
        uiInfoDesc->setContentSize(Size(uiTop->getContentSize().width, 278));
        uiInfoDesc->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiInfoDesc->setPosition(Vec2(uiTop->getContentSize().width / 2, 0));
        uiInfoDesc->setName("UI_INFO_DESC");
        uiTop->addChild(uiInfoDesc);
        {
            auto uiBG = ui::ImageView::create("Assets/ui_common/box_05_1.png");
            uiBG->setScale9Enabled(true);
            uiBG->setCapInsets(Rect(12, 12, 8, 8));
            uiBG->setContentSize(uiInfoDesc->getContentSize());
            uiBG->setPosition(Vec2(uiInfoDesc->getContentSize().width / 2, uiInfoDesc->getContentSize().height / 2));
            uiInfoDesc->addChild(uiBG);
            
            auto uiDesc = ui::CText::create("", GAME_FONT, 24);
            uiDesc->setPosition(Vec2(uiInfoDesc->getContentSize().width / 2, uiInfoDesc->getContentSize().height / 2));
            uiDesc->setTextAreaSize(Size(uiInfoDesc->getContentSize().width - 20, uiInfoDesc->getContentSize().height - 20));
            uiDesc->setTextOverflow(Label::Overflow::SHRINK);
            uiDesc->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiDesc->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiDesc->setName("UI_DESC");
            uiInfoDesc->addChild(uiDesc);
        }
    }
    
    //
    auto uiInfo = ui::Layout::create();
    uiInfo->setContentSize(Size(size.width, size.height - uiTop->getContentSize().height));
    uiInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiInfo->setPosition(Vec2(size.width / 2, 0));
    uiInfo->setName("UI_INFO");
    layout->addChild(uiInfo);
    {
        
    }
    
}

void PopupEventSunfishAdventureResult::uiButtons()
{
    auto layout = _uiContentsButton;
    auto size = layout->getContentSize();
    
    auto uiButtons = ui::Layout::create();
    uiButtons->setIgnoreAnchorPointForPosition(false);
    uiButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiButtons->setPosition(Vec2(size.width / 2, size.height / 2));
    uiButtons->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiButtons->setName("UI_BUTTONS");
    layout->addChild(uiButtons);
    {
        auto uiOk = ui::Button::create("Assets/ui_common/btn_ok.png");
        uiOk->setName("UI_BUTTON_OK");
        uiOk->addClickEventListener(CC_CALLBACK_1(PopupEventSunfishAdventureResult::onClickClose, this));
        uiButtons->addChild(uiOk);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_confirm"), GAME_FONT, 24);
            uiText->setPosition(uiOk->getContentSize()/2);
            uiText->setTextAreaSize(Size(uiOk->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiOk->addChild(uiText);
        }
    }
}

#pragma mark - ui : draw
void PopupEventSunfishAdventureResult::drawInfo()
{
    auto layout = _uiContentsInfo;
    
    auto levelIdx = _levelIdx.valueInt();
    auto adventureIdx = _adventureIdx.valueInt();
    
    auto objSunfishLevel = EventSunfishManager::getInstance()->getInfoSunfishLevel(levelIdx);
    auto objSunfishAdventure = EventSunfishManager::getInstance()->getInfoSunfishAdventure(adventureIdx);
    
    double weight = objSunfishAdventure->getWeight();
    double weightBonus = objSunfishAdventure->getWeight() * (objSunfishAdventure->getBonusWeight() * MIN(10, EventSunfishManager::getInstance()->getDie(EventSunfishManager::E_TYPE::ADVENTURE, adventureIdx)) / 1000.0f);
    weight += weightBonus;
    
    //
    if ( auto uiInfoTitle = utils::findChild<ui::Layout*>(layout, "UI_INFO_TITLE"); uiInfoTitle != nullptr )
    {
        auto uiBG = uiInfoTitle->getChildByName<ui::ImageView*>("UI_BG");
        if ( _bResult == true )
            uiBG->loadTexture("Assets/ui/event/lobster_bg_3_1.png");
        else
            uiBG->loadTexture("Assets/ui/event/lobster_bg_3_2.png");
        
        if ( _bResult == true )
        {
            auto uiIconLobster = ui::ImageView::create(objSunfishLevel->getSkinPath());
            uiIconLobster->setPosition(Vec2(uiInfoTitle->getContentSize().width / 2, 99));
            ((ui::Scale9Sprite*)uiIconLobster->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiIconLobster->setScale(MIN((uiInfoTitle->getContentSize().height * 0.6) / uiIconLobster->getContentSize().width, (uiInfoTitle->getContentSize().height * 0.6) / uiIconLobster->getContentSize().height));
            uiIconLobster->setName("UI_ICON_LOBSTER");
            uiInfoTitle->removeChildByName("UI_ICON_LOBSTER");
            uiInfoTitle->addChild(uiIconLobster);
            
            auto uiIconResult = ui::ImageView::create("Assets/ui/event/icon_success.png");
            uiIconResult->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiIconResult->setPosition(Vec2(uiInfoTitle->getContentSize().width / 2, uiInfoTitle->getContentSize().height + 10));
            ((ui::Scale9Sprite*)uiIconResult->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiIconResult->setScale(3);
            uiIconResult->setName("UI_ICON_RESULT");
            uiInfoTitle->removeChildByName("UI_ICON_RESULT");
            uiInfoTitle->addChild(uiIconResult);
        }
        else
        {
            auto uiIconLobster = ui::ImageView::create(objSunfishLevel->getSkinPath());
            uiIconLobster->setPosition(Vec2(uiInfoTitle->getContentSize().width / 2, 99));
            ((ui::Scale9Sprite*)uiIconLobster->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiIconLobster->setScale(MIN((uiInfoTitle->getContentSize().height * 0.6) / uiIconLobster->getContentSize().width, (uiInfoTitle->getContentSize().height * 0.6) / uiIconLobster->getContentSize().height));
            uiIconLobster->setFlippedY(true);
            uiIconLobster->setName("UI_ICON_LOBSTER");
            uiInfoTitle->removeChildByName("UI_ICON_LOBSTER");
            uiInfoTitle->addChild(uiIconLobster);
            
            auto uiIconResult = ui::ImageView::create("Assets/ui/event/icon_youdie.png");
            uiIconResult->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiIconResult->setPosition(Vec2(uiInfoTitle->getContentSize().width / 2, uiInfoTitle->getContentSize().height + 10));
            ((ui::Scale9Sprite*)uiIconResult->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiIconResult->setScale(3);
            uiIconResult->setName("UI_ICON_RESULT");
            uiInfoTitle->removeChildByName("UI_ICON_RESULT");
            uiInfoTitle->addChild(uiIconResult);
        }
    }
    
    //
    if ( auto uiInfoDesc = utils::findChild<ui::Layout*>(layout, "UI_INFO_DESC"); uiInfoDesc != nullptr )
    {
        std::string desc = "";
        if ( _bResult == true )
        {
            desc = objSunfishAdventure->getSurvivalText();
            desc = MafUtils::format(desc.c_str(), weight, objSunfishAdventure->getPoint());
        }
        else
        {
            desc = objSunfishAdventure->getDieText();
            desc = MafUtils::format(desc.c_str(), objSunfishAdventure->getPoint());
        }
            
        
        auto uiDesc = uiInfoDesc->getChildByName<ui::CText*>("UI_DESC");
        uiDesc->setString(desc);
    }
}

void PopupEventSunfishAdventureResult::drawButtons()
{
    auto layout = _uiContentsButton;
    
    auto uiButtons = layout->getChildByName<ui::Layout*>("UI_BUTTONS");

    //
    if ( auto uiOk = uiButtons->getChildByName<ui::Button*>("UI_BUTTON_OK"); uiOk != nullptr )
    {
        
    }
    
    auto sizeButtons = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiButtons);
    uiButtons->setContentSize(sizeButtons);
    uiButtons->requestDoLayout();
}

#pragma mark - ui : click
void PopupEventSunfishAdventureResult::onClickClose(Ref* sender)
{
    //
    if ( _bResult == true )
    {
        auto adventureIdx = _adventureIdx.valueInt();
        
        auto objSunfishAdventure = EventSunfishManager::getInstance()->getInfoSunfishAdventure(adventureIdx);
        
        double weight = objSunfishAdventure->getWeight();
        double weightBonus = objSunfishAdventure->getWeight() * (objSunfishAdventure->getBonusWeight() * MIN(10, EventSunfishManager::getInstance()->getDie(EventSunfishManager::E_TYPE::ADVENTURE, adventureIdx)) / 1000.0f);
        weight += weightBonus;
        
        //
        EventSunfishManager::getInstance()->onDelegateGameWeight(weight);
    }
    else
    {
        EventSunfishManager::getInstance()->onDelegateGameDie();
    }
    
    //
    hide();
}


#pragma mark - callback

#pragma mark - schedule

#pragma mark - set, get
void PopupEventSunfishAdventureResult::setLevelIdx(int idx)
{
    _levelIdx = idx;
    
    //
    if ( _bInitUI == true )
    {
        drawInfo();
        drawButtons();
    }
}

void PopupEventSunfishAdventureResult::setAdventureIdx(int idx)
{
    _adventureIdx = idx;
    
    //
    if ( _bInitUI == true )
    {
        drawInfo();
        drawButtons();
    }
}

void PopupEventSunfishAdventureResult::setResult(bool bResult)
{
    _bResult = bResult;
    
    //
    if ( _bInitUI == true )
    {
        drawInfo();
        drawButtons();
    }
}

#pragma mark - event
