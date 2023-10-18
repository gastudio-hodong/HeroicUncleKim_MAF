//
//  PopupEventSunfishFood.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "PopupEventSunfishFood.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/OtherEvent/EventSunfish/StateStep/PopupEventSunfishFoodResult.h"


#include "ManagerEvent/EventSunfishManager.h"

USING_NS_CC;

PopupEventSunfishFood::PopupEventSunfishFood(void) :
_bInitUI(false),

//
_eStep(E_STEP::CHECK),
_foodIdx(0),
_levelIdx(0),

//
_uiContentsInfo(nullptr),
_uiContentsButton(nullptr),

_uiTitle(nullptr)
{
    
}

PopupEventSunfishFood::~PopupEventSunfishFood(void)
{
    
}

bool PopupEventSunfishFood::init()
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

void PopupEventSunfishFood::onEnter()
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
void PopupEventSunfishFood::initVar()
{
    
}

void PopupEventSunfishFood::initUi()
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
void PopupEventSunfishFood::uiContainer()
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

void PopupEventSunfishFood::uiInfo()
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
        uiInfoTitle->setClippingEnabled(true);
        uiTop->addChild(uiInfoTitle);
        {
            auto uiBG = ui::ImageView::create("Assets/ui/event/lobster_event_bg_1_1.png");
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

void PopupEventSunfishFood::uiButtons()
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
        // check
        auto uiCheck = ui::Button::create("Assets/ui_common/btn_ok.png");
        uiCheck->setName("UI_BUTTON_CHECK");
        uiCheck->addClickEventListener(CC_CALLBACK_1(PopupEventSunfishFood::onClickCheck, this));
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
        
        auto uiConfirm = ui::Button::create("Assets/ui_common/btn_ok.png");
        uiConfirm->setName("UI_BUTTON_CONFIRM");
        uiConfirm->addClickEventListener(CC_CALLBACK_1(PopupEventSunfishFood::onClickConfirm, this));
        uiButtons->addChild(uiConfirm);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_confirm"), GAME_FONT, 24);
            uiText->setPosition(uiConfirm->getContentSize()/2);
            uiText->setTextAreaSize(Size(uiConfirm->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiConfirm->addChild(uiText);
        }
    }
}

#pragma mark - ui : draw
void PopupEventSunfishFood::drawInfo()
{
    auto layout = _uiContentsInfo;
    
    auto levelIdx = _levelIdx.valueInt();
    auto foodIdx = _foodIdx.valueInt();
    
    auto objSunfishFood = EventSunfishManager::getInstance()->getInfoSunfishFood(foodIdx);

    //
    _uiTitle->setString(objSunfishFood->getName());
        
    //
    if ( auto uiInfoTitle = utils::findChild<ui::Layout*>(layout, "UI_INFO_TITLE"); uiInfoTitle != nullptr )
    {
        auto uiBG = uiInfoTitle->getChildByName<ui::ImageView*>("UI_BG");
        uiBG->loadTexture(MafUtils::format("Assets/ui/event/lobster_event_bg_1_%d.png", levelIdx));
        
        auto uiIconFood = ui::ImageView::create(objSunfishFood->getSkinPath());
        uiIconFood->setPosition(Vec2(150, uiInfoTitle->getContentSize().height / 2));
        ((ui::Scale9Sprite*)uiIconFood->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiIconFood->setScale(MIN((uiInfoTitle->getContentSize().height * 0.6) / uiIconFood->getContentSize().width, (uiInfoTitle->getContentSize().height * 0.6) / uiIconFood->getContentSize().height));
        uiIconFood->setName("UI_ICON_FOOD");
        uiInfoTitle->removeChildByName("UI_ICON_FOOD");
        uiInfoTitle->addChild(uiIconFood);
    }
    
    //
    if ( auto uiInfoDesc = utils::findChild<ui::Layout*>(layout, "UI_INFO_DESC"); uiInfoDesc != nullptr )
    {
        std::string desc = "";
        switch (_eStep) {
            case E_STEP::CHECK :    desc = objSunfishFood->getDieDialogue(); break;
            case E_STEP::RESULT :
                desc = objSunfishFood->getSurvivalText();
                desc = MafUtils::format(desc.c_str(), objSunfishFood->getPoint());
                break;
                
            default:
                break;
        }
        
        auto uiDesc = uiInfoDesc->getChildByName<ui::CText*>("UI_DESC");
        uiDesc->setString(desc);
    }
}

void PopupEventSunfishFood::drawButtons()
{
    auto layout = _uiContentsButton;
    auto uiButtons = layout->getChildByName<ui::Layout*>("UI_BUTTONS");

    //
    auto uiCheck = uiButtons->getChildByName<ui::Button*>("UI_BUTTON_CHECK");
    uiCheck->setVisible(false);
    
    auto uiConfirm = uiButtons->getChildByName<ui::Button*>("UI_BUTTON_CONFIRM");
    uiConfirm->setVisible(false);
    
    switch (_eStep) {
        case E_STEP::CHECK :    uiCheck->setVisible(true); break;
        case E_STEP::RESULT :   uiConfirm->setVisible(true); break;
            
        default:
            break;
    }
     
    //
    auto sizeButtons = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiButtons);
    uiButtons->setContentSize(sizeButtons);
    uiButtons->requestDoLayout();
}

#pragma mark - ui : click
void PopupEventSunfishFood::onClickCheck(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    //
    auto foodIdx = _foodIdx.valueInt();
    EventSunfishManager::getInstance()->requestFoodCheck(true, foodIdx, CC_CALLBACK_3(PopupEventSunfishFood::callbackCheck, this));
}

void PopupEventSunfishFood::onClickConfirm(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    //
    auto objSunfish = EventSunfishManager::getInstance()->getInfoSunfishFood(_foodIdx.valueInt());
    if ( objSunfish != nullptr )
    {
        double weight = objSunfish->getWeight();
        double weightBonus = objSunfish->getWeight() * (objSunfish->getBonusWeight() * MIN(10, EventSunfishManager::getInstance()->getDie(EventSunfishManager::E_TYPE::FOOD, _foodIdx.valueInt())) / 1000.0f);
        EventSunfishManager::getInstance()->onDelegateGameWeight(weight + weightBonus);
    }
    
    //
    hide();
}

#pragma mark - callback
void PopupEventSunfishFood::callbackCheck(bool bResult, int nResult, bool bSuccess)
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
    
    
    if ( bSuccess == true )
    {// 생존
        //
        _eStep = E_STEP::RESULT;
        
        //
        drawInfo();
        drawButtons();
    }
    else
    {// 사망
        auto popup = PopupEventSunfishFoodResult::create();
        popup->setFoodIdx(_foodIdx.valueInt());
        popup->setLevelIdx(_levelIdx.valueInt());
        popup->setResult(false);
        popup->setHideBackKey(false);
        popup->show();
        
        hide();
    }
}
#pragma mark - schedule

#pragma mark - set, get
void PopupEventSunfishFood::setFoodIdx(int idx)
{
    _foodIdx = idx;
    
    if ( _bInitUI == true )
    {
        drawInfo();
        drawButtons();
    }
}

void PopupEventSunfishFood::setLevelIdx(int idx)
{
    _levelIdx = idx;
    
    if ( _bInitUI == true )
    {
        drawInfo();
        drawButtons();
    }
}

#pragma mark - event
