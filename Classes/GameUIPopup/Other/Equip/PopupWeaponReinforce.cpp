//
//  PopupWeaponReinforce.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 23/09/2019.
//

#include "PopupWeaponReinforce.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoWeapon.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/WeaponManager.h"

USING_NS_CC;

PopupWeaponReinforce::PopupWeaponReinforce(void) :
_bInitUI(false),

//
_uiContentsInfo(nullptr),
_uiContentsButton(nullptr)
{
    
}

PopupWeaponReinforce::~PopupWeaponReinforce(void)
{
    
}

bool PopupWeaponReinforce::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    return true;
}

void PopupWeaponReinforce::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        initVar();
        initUi();
        
        //
        _bInitUI = true;
    }
    
    //
    drawInfo();
    drawButtons();
}

#pragma mark - init
void PopupWeaponReinforce::initVar()
{
    
}

void PopupWeaponReinforce::initUi()
{
    Size size = Size(500, 632);
    
    //
    _spriteContainer->setContentSize( size );
    
    // label title
    auto uiTitle = ui::CText::create(GAME_TEXT("t_ui_bonsikdex_weapon_enhance_popup_1"), GAME_FONT, 32);
    uiTitle->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5));
    uiTitle->setTextAreaSize(Size(_spriteContainer->getContentSize().width - 200, uiTitle->getContentSize().height));
    uiTitle->setTextOverflow(Label::Overflow::SHRINK);
    uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiTitle->setTextColor(Color4B(COLOR_TITLE));
    _spriteContainer->addChild(uiTitle);
    
    // menu
    auto uiClose = ui::Button::create("Assets/ui_common/btn_x.png");
    uiClose->setPosition(Vec2(_spriteContainer->getContentSize().width - uiClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5));
    uiClose->addClickEventListener(CC_CALLBACK_1(PopupWeaponReinforce::onClickClose, this));
    _spriteContainer->addChild(uiClose);
    
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::GEM);
    list.push_back(E_ITEMS::MYTHRIL);
    
    auto uiCurrency = UICurrency::create(list, Size(_spriteContainer->getContentSize().width - 10, 58));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiCurrency->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(uiCurrency);
    
    //
    uiContainer();
    uiInfo();
    uiButtons();
}

#pragma mark - ui
void PopupWeaponReinforce::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 10;
    size.height = size.height - 75 - 58 - 25;
    
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

void PopupWeaponReinforce::uiInfo()
{
    auto layout = _uiContentsInfo;
    Size size = layout->getContentSize();
    
    //
    auto uiName = ui::CText::create(" ", GAME_FONT, 28);
    uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiName->setPosition(Vec2(size.width / 2, size.height - 80));
    uiName->setTextAreaSize(Size(size.width - 10, uiName->getContentSize().height));
    uiName->setTextOverflow(Label::Overflow::SHRINK);
    uiName->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiName->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiName->enableOutline(Color4B::BLACK, 2);
    uiName->setName("UI_NAME");
    layout->addChild(uiName);
    
    //
    auto uiInfoBG = ui::ImageView::create("Assets/ui_common/base_box_1_20.png");
    uiInfoBG->setScale9Enabled(true);
    uiInfoBG->setContentSize(Size(458, 56));
    uiInfoBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiInfoBG->setPosition(Vec2(size.width / 2, size.height - 90));
    layout->addChild(uiInfoBG);
    {
        auto uiPrevInfo = ui::Layout::create();
        uiPrevInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiPrevInfo->setPosition(Vec2(uiInfoBG->getContentSize().width / 2 - 40, uiInfoBG->getContentSize().height / 2));
        uiPrevInfo->setName("UI_INFO_PREV");
        uiPrevInfo->setLayoutType(ui::Layout::Type::HORIZONTAL);
        uiInfoBG->addChild(uiPrevInfo);
        {
            auto uiReinforceBG = ui::ImageView::create("Assets/ui/bonsikdex/book_rating_icon_1_1.png");
            uiReinforceBG->setName("UI_REINFORCE_BG");
            uiPrevInfo->addChild(uiReinforceBG);
            
            auto uiReinforce = ui::CText::create("0", GAME_FONT, 20);
            uiReinforce->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiReinforce->setPosition(Vec2(uiReinforceBG->getContentSize().width - 10, uiReinforceBG->getContentSize().height / 2));
            uiReinforce->setTextColor(Color4B::YELLOW);
            uiReinforce->setName("UI_REINFORCE");
            uiReinforceBG->addChild(uiReinforce);
            
            auto uiEffect = ui::CText::create("(0.00%)", GAME_FONT, 24);
            uiEffect->setTextColor(Color4B::YELLOW);
            uiEffect->setName("UI_EFFECT");
            uiPrevInfo->addChild(uiEffect);
            
            auto sizeInfo = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiPrevInfo);
            uiPrevInfo->setContentSize(sizeInfo);
            uiPrevInfo->requestDoLayout();
        }
        
        auto uiNextInfo = ui::Layout::create();
        uiNextInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiNextInfo->setPosition(Vec2(uiInfoBG->getContentSize().width / 2 + 40, uiInfoBG->getContentSize().height / 2));
        uiNextInfo->setName("UI_INFO_NEXT");
        uiNextInfo->setLayoutType(ui::Layout::Type::HORIZONTAL);
        uiInfoBG->addChild(uiNextInfo);
        {
            auto uiReinforceBG = ui::ImageView::create("Assets/ui/bonsikdex/book_rating_icon_1_1.png");
            uiReinforceBG->setName("UI_REINFORCE_BG");
            uiNextInfo->addChild(uiReinforceBG);
            
            auto uiReinforce = ui::CText::create("0", GAME_FONT, 20);
            uiReinforce->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiReinforce->setPosition(Vec2(uiReinforceBG->getContentSize().width - 10, uiReinforceBG->getContentSize().height / 2));
            uiReinforce->setTextColor(Color4B::YELLOW);
            uiReinforce->setName("UI_REINFORCE");
            uiReinforceBG->addChild(uiReinforce);
            
            auto uiEffect = ui::CText::create("(0.00%)", GAME_FONT, 24);
            uiEffect->setTextColor(Color4B::YELLOW);
            uiEffect->setName("UI_EFFECT");
            uiNextInfo->addChild(uiEffect);
            
            auto sizeInfo = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiNextInfo);
            uiNextInfo->setContentSize(sizeInfo);
            uiNextInfo->requestDoLayout();
        }
        
        auto uiArrow = ui::ImageView::create("Assets/ui_common/btn_shortcuts_4.png");
        uiArrow->setPosition(Vec2(uiInfoBG->getContentSize().width / 2, uiInfoBG->getContentSize().height / 2));
        uiInfoBG->addChild(uiArrow);
        
        // 1+0.01*(int(무기 단계*0.1)+1)*강화레벨
    }

    // desc
    auto uiDesc = ui::CText::create(GAME_TEXT("t_ui_bonsikdex_weapon_enhance_popup_2"), GAME_FONT, 24);
    uiDesc->setPosition(Vec2(size.width / 2, size.height - 220));
    uiDesc->setTextAreaSize(Size(size.width - 10, uiDesc->getContentSize().height));
    uiDesc->setTextOverflow(Label::Overflow::SHRINK);
    uiDesc->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiDesc->setTextHorizontalAlignment(TextHAlignment::CENTER);
    layout->addChild(uiDesc);

    //
    auto uiCostBG = ui::Layout::create();
    uiCostBG->setContentSize(Size(size.width - 10, 38));
    uiCostBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiCostBG->setPosition(Vec2(size.width / 2, 65));
    uiCostBG->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiCostBG->setName("UI_COST");
    layout->addChild(uiCostBG);

    
}

void PopupWeaponReinforce::uiButtons()
{
    auto layout = _uiContentsButton;
    Size size = layout->getContentSize();
    
    auto uiButtons = ui::Layout::create();
    uiButtons->setIgnoreAnchorPointForPosition(false);
    uiButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiButtons->setPosition(Vec2(size.width / 2, size.height / 2));
    uiButtons->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiButtons->setName("UI_BUTTONS");
    layout->addChild(uiButtons);
    {
        // check
        auto uiReinforce = ui::Button::create("Assets/ui_common/btn_ok.png");
        uiReinforce->addClickEventListener(CC_CALLBACK_1(PopupWeaponReinforce::onClickReinforce, this));
        uiButtons->addChild(uiReinforce);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_upgrade"), GAME_FONT, 24);
            uiText->setPosition(uiReinforce->getContentSize()/2);
            uiText->setTextAreaSize(Size(uiReinforce->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiReinforce->addChild(uiText);
        }
        
        auto uiCancel = ui::Button::create("Assets/ui_common/btn_ok.png");
        uiCancel->addClickEventListener(CC_CALLBACK_1(PopupWeaponReinforce::onClickClose, this));
        uiButtons->addChild(uiCancel);
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(20, 0, 0, 0));
            uiCancel->setLayoutParameter(params);
            
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_cancel"), GAME_FONT, 24);
            uiText->setPosition(uiCancel->getContentSize()/2);
            uiText->setTextAreaSize(Size(uiCancel->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiCancel->addChild(uiText);
        }
    }
}

#pragma mark - draw
void PopupWeaponReinforce::drawInfo()
{
    auto layout = _uiContentsInfo;
    
    auto objWeapon = WeaponManager::getInstance()->getInfoWeapon(_idxWeapon.valueInt());
    if ( objWeapon == nullptr )
    {
        return;
    }
    
    int reinforcePrev = WeaponManager::getInstance()->getReinforce(objWeapon->getIdx());
    int reinforceNext = reinforcePrev + 1;
    int reinforceMax = objWeapon->getEnhanceMax();
    
    int effectPrev = WeaponManager::getInstance()->getReinforceEffect(objWeapon->getIdx(), reinforcePrev) * 100;
    int effectNext = WeaponManager::getInstance()->getReinforceEffect(objWeapon->getIdx(), reinforceNext) * 100;
    
    //
    if ( auto uiName = utils::findChild<ui::CText*>(layout, "UI_NAME"); uiName != nullptr )
    {
        uiName->setString(MafUtils::format("%d.%s", objWeapon->getIdx(), objWeapon->getName().c_str()));
    }
    
    if ( auto uiPrevInfo = utils::findChild<ui::Layout*>(layout, "UI_INFO_PREV"); uiPrevInfo != nullptr )
    {
        auto uiReinforceBG = utils::findChild<ui::ImageView*>(uiPrevInfo, "UI_REINFORCE_BG");
        uiReinforceBG->loadTexture("Assets/ui/bonsikdex/book_rating_icon_1_1.png");
        if ( reinforcePrev >= reinforceMax )
        {
            uiReinforceBG->loadTexture("Assets/ui/bonsikdex/book_rating_icon_1_2.png");
        }
        
        auto uiReinforce = utils::findChild<ui::CText*>(uiPrevInfo, "UI_REINFORCE");
        uiReinforce->setString(MafUtils::toString(reinforcePrev));
        
        auto uiEffect = utils::findChild<ui::CText*>(uiPrevInfo, "UI_EFFECT");
        uiEffect->setString(MafUtils::format("%d%%", effectPrev));
        
        auto sizeInfo = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiPrevInfo);
        uiPrevInfo->setContentSize(sizeInfo);
        uiPrevInfo->requestDoLayout();
    }
    
    if ( auto uiNextInfo = utils::findChild<ui::Layout*>(layout, "UI_INFO_NEXT"); uiNextInfo != nullptr )
    {
        auto uiReinforceBG = utils::findChild<ui::ImageView*>(uiNextInfo, "UI_REINFORCE_BG");
        uiReinforceBG->loadTexture("Assets/ui/bonsikdex/book_rating_icon_1_1.png");
        if ( reinforceNext >= reinforceMax )
        {
            uiReinforceBG->loadTexture("Assets/ui/bonsikdex/book_rating_icon_1_2.png");
        }
        
        auto uiReinforce = utils::findChild<ui::CText*>(uiNextInfo, "UI_REINFORCE");
        uiReinforce->setString(MafUtils::toString(reinforceNext));
        
        auto uiEffect = utils::findChild<ui::CText*>(uiNextInfo, "UI_EFFECT");
        uiEffect->setString(MafUtils::format("%d%%", effectNext));
        
        auto sizeInfo = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiNextInfo);
        uiNextInfo->setContentSize(sizeInfo);
        uiNextInfo->requestDoLayout();
    }
    
   
    
    //
    if ( auto uiCost = utils::findChild<ui::Layout*>(layout, "UI_COST"); uiCost != nullptr )
    {
        uiCost->removeAllChildren();
        
        auto listCost = objWeapon->getEnhanceCost();
        
        for ( int i = 0; i < listCost.size(); i++ )
        {
            auto objItem = listCost.at(i);
            
            auto uiItemBG = ui::Layout::create();
            uiItemBG->setContentSize(Size(100, uiCost->getContentSize().height));
            uiItemBG->setLayoutType(ui::Layout::Type::HORIZONTAL);
            uiCost->addChild(uiItemBG);
            {
                if ( i != 0 )
                {
                    auto params = ui::LinearLayoutParameter::create();
                    params->setMargin(ui::Margin(20, 0, 0, 0));
                    uiItemBG->setLayoutParameter(params);
                }
                
                auto uiItemIcon = ui::ImageView::create(objItem->getPath());
                uiItemBG->addChild(uiItemIcon);
                {
                    auto params = ui::LinearLayoutParameter::create();
                    params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
                    uiItemIcon->setLayoutParameter(params);
                }
                
                std::string rewardCount = objItem->getCount();
                if ( objItem->isString() == true )
                {
                    MafUtils::convertNumberToShort(rewardCount);
                }
                
                auto uiItemCount = ui::CText::create(rewardCount, GAME_FONT, 20);
                uiItemCount->enableOutline(Color4B::BLACK, 2);
                uiItemBG->addChild(uiItemCount);
                {
                    auto params = ui::LinearLayoutParameter::create();
                    params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
                    uiItemCount->setLayoutParameter(params);
                }
            }
            
            auto sizeItem = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiItemBG);
            uiItemBG->setContentSize(sizeItem);
            uiItemBG->requestDoLayout();
        }
        
        auto sizeCost = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiCost);
        uiCost->setContentSize(sizeCost);
        uiCost->requestDoLayout();
    }
    
    
    
}

void PopupWeaponReinforce::drawButtons()
{
    auto layout = _uiContentsButton;
    
    auto uiButtons = layout->getChildByName<ui::Layout*>("UI_BUTTONS");
     
    //
    auto sizeButtons = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiButtons);
    uiButtons->setContentSize(sizeButtons);
    uiButtons->requestDoLayout();
}

#pragma mark - click
void PopupWeaponReinforce::onClickClose(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    hide();
}

void PopupWeaponReinforce::onClickReinforce(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    int result = WeaponManager::getInstance()->onWeaponReinforce(_idxWeapon.valueInt());
    if ( result != 0 )
    {
        if ( result == 2 )
        {
            auto objWeapon = WeaponManager::getInstance()->getInfoWeapon(_idxWeapon.valueInt());

            auto listCost = objWeapon->getEnhanceCost();
            for ( int i = 0; i < listCost.size(); i++ )
            {
                auto cost = listCost.at(i);
                
                auto itemNeed = cost->getCount();
                auto itemNow = ItemsManager::getInstance()->getItems((E_ITEMS)cost->getIdx());
                if ( MafUtils::compareStr(itemNow, itemNeed) == false )
                {
                    auto popup = PopupCurrencyDetail::create((E_ITEMS)cost->getIdx());
                    popup->show();
                    break;
                }
            }
        }
        return;
    }
    
    hide();
}

#pragma mark - set, get
void PopupWeaponReinforce::setWeaponIdx(int idx)
{
    _idxWeapon = idx;
    
    if ( _bInitUI == true )
    {
        drawInfo();
        drawButtons();
    }
}
