//
//  PopupEventSunfishLevel.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "PopupEventSunfishLevel.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/OtherEvent/EventSunfish/StateStep/PopupEventSunfishLevelResult.h"

#include "ManagerEvent/EventSunfishManager.h"

USING_NS_CC;

PopupEventSunfishLevel::PopupEventSunfishLevel(void) :
_bInitUI(false),

//
_levelIdx(0),

//
_uiContentsInfo(nullptr),
_uiContentsButton(nullptr),

_uiTitle(nullptr)
{
    
}

PopupEventSunfishLevel::~PopupEventSunfishLevel(void)
{
    
}

bool PopupEventSunfishLevel::init()
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

void PopupEventSunfishLevel::onEnter()
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
void PopupEventSunfishLevel::initVar()
{
    
}

void PopupEventSunfishLevel::initUi()
{
    // size
    _spriteContainer->setContentSize( Size(700, 818) );
    
    // label title
    _uiTitle = ui::CText::create("", GAME_FONT, 32);
    _uiTitle->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5));
    _uiTitle->setTextAreaSize(Size(_spriteContainer->getContentSize().width - 200, _uiTitle->getContentSize().height));
    _uiTitle->setTextOverflow(Label::Overflow::SHRINK);
    _uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    _uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _uiTitle->setTextColor(Color4B(COLOR_TITLE));
    _spriteContainer->addChild(_uiTitle);
}

#pragma mark - ui
void PopupEventSunfishLevel::uiContainer()
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

void PopupEventSunfishLevel::uiInfo()
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
            auto uiBG = ui::ImageView::create("Assets/ui/event/lobster_event_bg_2_1.png");
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
        auto uiRate = ui::CText::create(GAME_TEXTFORMAT("t_ui_event_sunfish_survival_rate", 0), GAME_FONT, 28);
        uiRate->setPosition(Vec2(uiInfo->getContentSize().width / 2, uiInfo->getContentSize().height / 2));
        uiRate->setTextAreaSize(Size(uiInfo->getContentSize().width - 10, uiRate->getContentSize().height));
        uiRate->setTextOverflow(Label::Overflow::SHRINK);
        uiRate->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiRate->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiRate->setTextColor(Color4B::YELLOW);
        uiRate->setName("UI_RATE");
        uiInfo->addChild(uiRate);
    }
}

void PopupEventSunfishLevel::uiButtons()
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
        // ad
        auto uiCheck = ui::Button::create("Assets/ui_common/btn_ok.png");
        uiCheck->setName("UI_BUTTON_CHECK");
        uiCheck->addClickEventListener(CC_CALLBACK_1(PopupEventSunfishLevel::onClickCheck, this));
        uiButtons->addChild(uiCheck);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_confirm"), GAME_FONT, 24);
            uiText->setPosition(uiCheck->getContentSize()/2);
            uiText->setTextAreaSize(Size(uiCheck->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiCheck->addChild(uiText);
        }
    }
}

#pragma mark - ui : draw
void PopupEventSunfishLevel::drawInfo()
{
    auto layout = _uiContentsInfo;
    
    auto levelPrevIdx = _levelIdx.valueInt();
    auto levelNextIdx = levelPrevIdx + 1;
    
    auto objSunfishLevelPrev = EventSunfishManager::getInstance()->getInfoSunfishLevel(levelPrevIdx);
    auto objSunfishLevelNext = EventSunfishManager::getInstance()->getInfoSunfishLevel(levelNextIdx);

    //
    _uiTitle->setString(objSunfishLevelPrev->getName());
    
    //
    if ( auto uiInfoTitle = utils::findChild<ui::Layout*>(layout, "UI_INFO_TITLE"); uiInfoTitle != nullptr )
    {
        auto uiIcon = ui::ImageView::create(objSunfishLevelPrev->getSkinPath());
        uiIcon->setPosition(Vec2(uiInfoTitle->getContentSize().width / 2, uiInfoTitle->getContentSize().height / 2));
        ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiIcon->setScale(4);
        uiIcon->setScale(MIN((uiInfoTitle->getContentSize().height * 0.6) / uiIcon->getContentSize().width, (uiInfoTitle->getContentSize().height * 0.6) / uiIcon->getContentSize().height));
        uiIcon->setName("UI_ICON");
        uiInfoTitle->removeChildByName("UI_ICON");
        uiInfoTitle->addChild(uiIcon);
        {
            auto action1 = CallFuncN::create([=](Node* sender){
                
                auto uiIcon = (ui::ImageView*)sender;
                auto levelIdx = _levelIdx.valueInt();
                
                auto objSunfishLevel = EventSunfishManager::getInstance()->getInfoSunfishLevel(levelIdx);
                uiIcon->loadTexture(objSunfishLevel->getSkinPath());
                ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            });
            auto action2 = DelayTime::create(0.2);
            auto action3 = CallFuncN::create([=](Node* sender){
                
                auto uiIcon = (ui::ImageView*)sender;
                auto levelIdx = _levelIdx.valueInt() + 1;
                
                auto objSunfishLevel = EventSunfishManager::getInstance()->getInfoSunfishLevel(levelIdx);
                uiIcon->loadTexture(objSunfishLevel->getSkinPath());
                ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            });
            auto action4 = DelayTime::create(0.2);
            uiIcon->runAction(RepeatForever::create(Sequence::create(action1, action2, action3, action4, NULL)));
        }
    }
    
    //
    if ( auto uiInfoDesc = utils::findChild<ui::Layout*>(layout, "UI_INFO_DESC"); uiInfoDesc != nullptr )
    {
        auto uiDesc = uiInfoDesc->getChildByName<ui::CText*>("UI_DESC");
        uiDesc->setString(objSunfishLevelNext->getDieDialogue());
    }
    
    //
    if ( auto uiInfo = utils::findChild<ui::Layout*>(layout, "UI_INFO"); uiInfo != nullptr )
    {
        auto rate = (objSunfishLevelNext->getSurvivalRate() + objSunfishLevelNext->getBonusRate() * EventSunfishManager::getInstance()->getDie(EventSunfishManager::E_TYPE::LEVEL, levelNextIdx)) / 10.0f;
        rate = MIN(rate, 99.9f);
        
        auto uiRate = uiInfo->getChildByName<ui::CText*>("UI_RATE");
        uiRate->setString(GAME_TEXTFORMAT("t_ui_event_sunfish_survival_rate", rate));
    }
    
}

void PopupEventSunfishLevel::drawButtons()
{
    auto layout = _uiContentsButton;
    auto uiButtons = layout->getChildByName<ui::Layout*>("UI_BUTTONS");

    //
    if ( auto uiCheck = uiButtons->getChildByName<ui::Button*>("UI_BUTTON_CHECK"); uiCheck != nullptr )
    {
        
    }
     
    //
    auto sizeButtons = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiButtons);
    uiButtons->setContentSize(sizeButtons);
    uiButtons->requestDoLayout();
}

#pragma mark - ui : click
void PopupEventSunfishLevel::onClickCheck(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    //
    auto levelPrevIdx = _levelIdx.valueInt();
    auto levelNextIdx = levelPrevIdx + 1;
    
    // request
    EventSunfishManager::getInstance()->requestLevelCheck(true, levelNextIdx, CC_CALLBACK_3(PopupEventSunfishLevel::callbackCheck, this));
}

#pragma mark - callback
void PopupEventSunfishLevel::callbackCheck(bool bResult, int nResult, bool bSuccess)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 2 )
            strMessage = GAME_TEXT("t_ui_draw_msg_1");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    //
    auto popup = PopupEventSunfishLevelResult::create();
    popup->setLevelIdx(_levelIdx.valueInt());
    popup->setResult(bSuccess);
    popup->setHideBackKey(false);
    popup->show();
    
    //
    hide();
}
#pragma mark - schedule

#pragma mark - set, get
void PopupEventSunfishLevel::setLevelIdx(int idx)
{
    _levelIdx = idx;
    
    //
    if ( _bInitUI == true )
    {
        drawInfo();
        drawButtons();
    }
}

#pragma mark - event
