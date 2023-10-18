//
//  PopupPetEquipBoxBuyCount.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 31/07/2019.
//


#include "PopupPetEquipBoxBuyCount.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIReddot.h"

#include "ManagerGame/PetEquipBoxManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupPetEquipBoxBuyCount* PopupPetEquipBoxBuyCount::create(E_PET_EQUIP_BUY eType, bool bEvent)
{
    PopupPetEquipBoxBuyCount* pRet = new(std::nothrow) PopupPetEquipBoxBuyCount();
    if ( pRet && pRet->init(eType, bEvent) )
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

PopupPetEquipBoxBuyCount::PopupPetEquipBoxBuyCount(void) :
_bInitUI(false),

//
_eType(E_PET_EQUIP_BUY::COIN),
_bEvent(false),

_countNow(1),
_countMax(0),

_callbackResult(nullptr),

//
_uiContainerInfo(nullptr)
{
    
}

PopupPetEquipBoxBuyCount::~PopupPetEquipBoxBuyCount(void)
{
    
}

bool PopupPetEquipBoxBuyCount::init(E_PET_EQUIP_BUY eType, bool bEvent)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    _eType = eType;
    _bEvent = bEvent;
    
    //
    initVar();
    initUi();
    
    return true;
}

void PopupPetEquipBoxBuyCount::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiInfo();
        uiButtons();
    }
    
    //
    drawInfo();
    drawButtons();
}

#pragma mark - init
void PopupPetEquipBoxBuyCount::initVar()
{
    _countMax = 0;
    
    int itemNeed = PetEquipBoxManager::getInstance()->getCost(_eType, _bEvent);
    int itemNow = 0;
    int itemMax = 10;
    
    _countMax = itemMax;
    if ( _eType == E_PET_EQUIP_BUY::HONOR )
    {
        itemNow = std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::HONOR).c_str());

        for (int i = 0; i < itemMax; i++ )
        {
            unsigned long long result = PetEquipBoxManager::getInstance()->getCostData(_eType, _bEvent) * std::pow(2, PetEquipBoxManager::getInstance()->getEquipBoxOpenHonor() + i);
            if ( result > 2560 )
            {
                result = 2560;
            }
            
            if ( itemNow < result )
            {
                break;
            }
            
            itemNow -= result;
            _countMax = i + 1;
        }
    }
    else
    {
        if ( _eType == E_PET_EQUIP_BUY::BONG )
        {
            itemNow = std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::MATERIAL).c_str());
        }
        else if ( _eType == E_PET_EQUIP_BUY::COIN )
        {
            itemNow = std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str());
        }
        
        int countTemp = itemNow / itemNeed;
        if ( _countMax > countTemp )
        {
            _countMax = countTemp;
        }

        if ( _eType == E_PET_EQUIP_BUY::COIN && _countMax > PetEquipBoxManager::getInstance()->getEquipBoxOpenDevilMax() )
        {
            _countMax = PetEquipBoxManager::getInstance()->getEquipBoxOpenDevilMax();
        }
    }
    
}

void PopupPetEquipBoxBuyCount::initUi()
{
    // size 736
    _spriteContainer->setContentSize( Size(486, 642) );
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(25);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_pet_equip_randombox"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    //
    std::vector<E_ITEMS> list;
    if ( _eType == E_PET_EQUIP_BUY::HONOR )
        list.push_back(E_ITEMS::HONOR);
    else if ( _eType == E_PET_EQUIP_BUY::BONG )
        list.push_back(E_ITEMS::MATERIAL);
    else if ( _eType == E_PET_EQUIP_BUY::COIN )
        list.push_back(E_ITEMS::COSTUME_COIN);

    auto uiCurrency = UICurrency::create(list, Size(_spriteContainer->getContentSize().width - 8, 50));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiCurrency->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(uiCurrency);
    {
        auto uiBG = ui::ImageView::create("Assets/ui_common/base_box_1_1.png");
        uiBG->setScale9Enabled(true);
        uiBG->setContentSize(uiCurrency->getContentSize());
        uiBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiBG->setPosition(Vec2(0, 0));
        uiCurrency->addChild(uiBG, -1);
    }
}

#pragma mark - ui
void PopupPetEquipBoxBuyCount::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = 406;
    size.height = 465;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 38));
    _spriteContainer->addChild(layout);

    _uiContainerButtons = ui::Layout::create();
    _uiContainerButtons->setContentSize( Size(size.width, 88) );
    _uiContainerButtons->setIgnoreAnchorPointForPosition(false);
    _uiContainerButtons->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContainerButtons->setPosition( Vec2(size.width / 2, 0) );
    layout->addChild(_uiContainerButtons);
    
    _uiContainerInfo = ui::Layout::create();
    _uiContainerInfo->setContentSize( Size(size.width, size.height - _uiContainerButtons->getContentSize().height) );
    _uiContainerInfo->setIgnoreAnchorPointForPosition(false);
    _uiContainerInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerInfo->setPosition( Vec2(size.width / 2, size.height) );
    layout->addChild(_uiContainerInfo);
}

void PopupPetEquipBoxBuyCount::uiInfo()
{
    auto layout = _uiContainerInfo;
    Size size = layout->getContentSize();
    

    //
    {
        auto uiChild = ui::Layout::create();
        uiChild->setContentSize(Size(66 * 3, 86 * 3));
        uiChild->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiChild->setPosition(Vec2(size.width / 2, size.height));
        uiChild->setTag(enumToInt(E_PET_EQUIP_BUY::COIN));
        uiChild->setVisible(false);
        layout->addChild(uiChild);
        
        auto uiBG = ui::ImageView::create("Assets/ui/pet/pet_equipment_shop_1_1.png");
        uiBG->setPosition(Vec2(uiChild->getContentSize().width / 2, uiChild->getContentSize().height / 2));
        uiBG->setScale(uiChild->getContentSize().width / uiBG->getContentSize().width);
        ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiChild->addChild(uiBG);
        
        auto uiTitle = ui::CText::create(GAME_TEXT("t_pet_equip_randombox_buy_3"), GAME_FONT, 24);
        uiTitle->setPosition(Vec2(uiChild->getContentSize().width / 2, uiChild->getContentSize().height - 20));
        uiTitle->setTextAreaSize(Size(uiChild->getContentSize().width - 10, uiTitle->getContentSize().height));
        uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiTitle->setTextOverflow(Label::Overflow::SHRINK);
        uiTitle->setTextColor(Color4B(COLOR_TITLE));
        uiChild->addChild(uiTitle);
        
        auto uiCount = ui::CText::create("(0/0)", GAME_FONT, 21);
        uiCount->setPosition(Vec2(uiChild->getContentSize().width / 2, uiChild->getContentSize().height - 40));
        uiCount->setName("UI_COUNT");
        uiChild->addChild(uiCount);
        
        auto uiIcon = ui::ImageView::create("Assets/icon/icon_coin_2.png");
        uiIcon->setPosition(Vec2(uiChild->getContentSize().width * 0.24, uiChild->getContentSize().height * 0.13));
        uiChild->addChild(uiIcon);
        
        auto uiCost = ui::CText::create("x 0", GAME_FONT, 22);
        uiCost->setPosition(Vec2(uiChild->getContentSize().width * 0.54, uiChild->getContentSize().height * 0.13));
        uiCost->setName("UI_COST");
        uiChild->addChild(uiCost);
    }
    
    //
    {
        auto uiChild = ui::Layout::create();
        uiChild->setContentSize(Size(66 * 3, 86 * 3));
        uiChild->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiChild->setPosition(Vec2(size.width / 2, size.height));
        uiChild->setTag(enumToInt(E_PET_EQUIP_BUY::HONOR));
        uiChild->setVisible(false);
        layout->addChild(uiChild);
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(15, 0, 0, 0));
            uiChild->setLayoutParameter(params);
        }
        
        auto uiBG = ui::ImageView::create("Assets/ui/pet/pet_equipment_shop_1_2.png");
        uiBG->setPosition(Vec2(uiChild->getContentSize().width / 2, uiChild->getContentSize().height / 2));
        uiBG->setScale(uiChild->getContentSize().width / uiBG->getContentSize().width);
        ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiChild->addChild(uiBG);
        
        auto uiTitle = ui::CText::create(GAME_TEXT("t_pet_equip_randombox_buy_1"), GAME_FONT, 24);
        uiTitle->setPosition(Vec2(uiChild->getContentSize().width / 2, uiChild->getContentSize().height - 20));
        uiTitle->setTextAreaSize(Size(uiChild->getContentSize().width - 10, uiTitle->getContentSize().height));
        uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiTitle->setTextOverflow(Label::Overflow::SHRINK);
        uiTitle->setTextColor(Color4B(COLOR_TITLE));
        uiChild->addChild(uiTitle);
        
        auto uiIcon = ui::ImageView::create("Assets/icon/menu_honor.png");
        uiIcon->setPosition(Vec2(uiChild->getContentSize().width * 0.24, uiChild->getContentSize().height * 0.13));
        uiChild->addChild(uiIcon);
        
        auto uiCost = ui::CText::create("x 0", GAME_FONT, 22);
        uiCost->setPosition(Vec2(uiChild->getContentSize().width * 0.54, uiChild->getContentSize().height * 0.13));
        uiCost->setName("UI_COST");
        uiChild->addChild(uiCost);
    }
    
    //
    {
        auto uiChild = ui::Layout::create();
        uiChild->setContentSize(Size(66 * 3, 86 * 3));
        uiChild->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiChild->setPosition(Vec2(size.width / 2, size.height));
        uiChild->setTag(enumToInt(E_PET_EQUIP_BUY::BONG));
        uiChild->setVisible(false);
        layout->addChild(uiChild);
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(15, 0, 0, 0));
            uiChild->setLayoutParameter(params);
        }
        
        auto uiBG = ui::ImageView::create("Assets/ui/pet/pet_equipment_shop_1_3.png");
        uiBG->setPosition(Vec2(uiChild->getContentSize().width / 2, uiChild->getContentSize().height / 2));
        uiBG->setScale(uiChild->getContentSize().width / uiBG->getContentSize().width);
        ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiChild->addChild(uiBG);
        
        auto uiTitle = ui::CText::create(GAME_TEXT("t_pet_equip_randombox_buy_2"), GAME_FONT, 24);
        uiTitle->setPosition(Vec2(uiChild->getContentSize().width / 2, uiChild->getContentSize().height - 20));
        uiTitle->setTextAreaSize(Size(uiChild->getContentSize().width - 10, uiTitle->getContentSize().height));
        uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiTitle->setTextOverflow(Label::Overflow::SHRINK);
        uiTitle->setTextColor(Color4B(COLOR_TITLE));
        uiChild->addChild(uiTitle);
        
        auto uiIcon = ui::ImageView::create("Assets/icon/evilcrystal.png");
        uiIcon->setPosition(Vec2(uiChild->getContentSize().width * 0.24, uiChild->getContentSize().height * 0.13));
        uiChild->addChild(uiIcon);
        
        auto uiCost = ui::CText::create("x 0", GAME_FONT, 22);
        uiCost->setPosition(Vec2(uiChild->getContentSize().width * 0.54, uiChild->getContentSize().height * 0.13));
        uiCost->setName("UI_COST");
        uiChild->addChild(uiCost);
    }
   
    
    //
    auto uiCountBG = ui::ImageView::create("Assets/ui/prison/bafometz/baphomet_numbox.png");
    uiCountBG->setPosition(Vec2(layout->getContentSize().width / 2, 59.5));
    uiCountBG->setName("UI_COUNT_BG");
    layout->addChild(uiCountBG);
    {
        auto uiCount = ui::CText::create("0", GAME_FONT, 28);
        uiCount->setPosition(Vec2(uiCountBG->getContentSize().width / 2, uiCountBG->getContentSize().height / 2));
        uiCount->setName("UI_COUNT");
        uiCountBG->addChild(uiCount);
        
        auto uiArrowL = ui::Button::create("Assets/ui/prison/bafometz/baphomet_arrow_l.png");
        uiArrowL->setPosition(Vec2(uiCountBG->getContentSize().width / 2 - 90, uiCountBG->getContentSize().height / 2));
        uiArrowL->addClickEventListener(CC_CALLBACK_1(PopupPetEquipBoxBuyCount::onClickArrow, this));
        uiArrowL->setTag(1);
        uiCountBG->addChild(uiArrowL);
        
        auto uiArrowR = ui::Button::create("Assets/ui/prison/bafometz/baphomet_arrow_r.png");
        uiArrowR->setPosition(Vec2(uiCountBG->getContentSize().width / 2 + 90, uiCountBG->getContentSize().height / 2));
        uiArrowR->addClickEventListener(CC_CALLBACK_1(PopupPetEquipBoxBuyCount::onClickArrow, this));
        uiArrowR->setTag(2);
        uiCountBG->addChild(uiArrowR);
    }
    
    
}

void PopupPetEquipBoxBuyCount::uiButtons()
{
    auto layout = _uiContainerButtons;
    Size size = layout->getContentSize();
    
    {
        auto uiBtnBuy = ui::Button::create("Assets/ui_common/btn_ok.png");
        uiBtnBuy->addClickEventListener(CC_CALLBACK_1(PopupPetEquipBoxBuyCount::onClickBuy, this));
        layout->addChild(uiBtnBuy);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_buy"), GAME_FONT, 28);
            uiText->setPositionType(ui::Widget::PositionType::PERCENT);
            uiText->setPositionPercent(Vec2::ANCHOR_MIDDLE);
            uiText->setTextAreaSize(Size(uiBtnBuy->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiBtnBuy->addChild(uiText);
        }
    }
    
    {
        auto uiBtnCancel = ui::Button::create("Assets/ui_common/btn_ok.png");
        uiBtnCancel->addClickEventListener(CC_CALLBACK_1(PopupPetEquipBoxBuyCount::onClickClose, this));
        layout->addChild(uiBtnCancel);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_cancel"), GAME_FONT, 28);
            uiText->setPositionType(ui::Widget::PositionType::PERCENT);
            uiText->setPositionPercent(Vec2::ANCHOR_MIDDLE);
            uiText->setTextAreaSize(Size(uiBtnCancel->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiBtnCancel->addChild(uiText);
        }
        
        auto params = ui::LinearLayoutParameter::create();
        params->setMargin(ui::Margin(10, 0, 0, 0));
        uiBtnCancel->setLayoutParameter(params);
    }
    
}

#pragma mark - draw
void PopupPetEquipBoxBuyCount::drawInfo()
{
    auto layout = _uiContainerInfo;
    Size size = layout->getContentSize();
    
   
    auto uiChildCoin = layout->getChildByTag<ui::Layout*>(enumToInt(E_PET_EQUIP_BUY::COIN));
    if ( uiChildCoin != nullptr )
    {
        uiChildCoin->setVisible(false);
        
        if ( _eType == E_PET_EQUIP_BUY::COIN )
        {
            uiChildCoin->setVisible(true);
            
            //
            auto uiCost = uiChildCoin->getChildByName<ui::CText*>("UI_COST");
            if ( uiCost != nullptr )
            {
                uiCost->setString(MafUtils::format("x %d", PetEquipBoxManager::getInstance()->getCost(E_PET_EQUIP_BUY::COIN, _bEvent) * _countNow));
            }
            
            auto uiCount = uiChildCoin->getChildByName<ui::CText*>("UI_COUNT");
            if ( uiCount != nullptr )
            {
                int countNow = PetEquipBoxManager::getInstance()->getEquipBoxOpenDevil();
                int countMax = PetEquipBoxManager::getInstance()->getEquipBoxOpenDevilMax();
                
                uiCount->setString(MafUtils::format("(%d/%d)", countNow, countMax));
            }
        }
    }
    
    auto uiChildHonor = layout->getChildByTag<ui::Layout*>(enumToInt(E_PET_EQUIP_BUY::HONOR));
    if ( uiChildHonor != nullptr )
    {
        uiChildHonor->setVisible(false);
        
        if ( _eType == E_PET_EQUIP_BUY::HONOR )
        {
            uiChildHonor->setVisible(true);
            
            //
            auto uiCost = uiChildHonor->getChildByName<ui::CText*>("UI_COST");
            if ( uiCost != nullptr )
            {
                int itemTotal = 0;

                for (int i = 0; i < _countNow; i++ )
                {
                    unsigned long long result = PetEquipBoxManager::getInstance()->getCostData(_eType, _bEvent) * std::pow(2, PetEquipBoxManager::getInstance()->getEquipBoxOpenHonor() + i);
                    if ( result > 2560 )
                    {
                        result = 2560;
                    }
                    
                    itemTotal += result;
                }
                
                uiCost->setString(MafUtils::format("x %d", itemTotal));
            }
        }
    }
    
    auto uiChildBong = layout->getChildByTag<ui::Layout*>(enumToInt(E_PET_EQUIP_BUY::BONG));
    if ( uiChildBong != nullptr )
    {
        uiChildBong->setVisible(false);
        
        if ( _eType == E_PET_EQUIP_BUY::BONG )
        {
            uiChildBong->setVisible(true);
            
            //
            auto uiCost = uiChildBong->getChildByName<ui::CText*>("UI_COST");
            if ( uiCost != nullptr )
            {
                uiCost->setString(MafUtils::format("x %d", PetEquipBoxManager::getInstance()->getCost(E_PET_EQUIP_BUY::BONG, _bEvent) * _countNow));
            }
        }
    }
    
    //
    auto uiCountBG = layout->getChildByName<ui::ImageView*>("UI_COUNT_BG");
    {
        auto uiCount = uiCountBG->getChildByName<ui::CText*>("UI_COUNT");
        uiCount->setString(MafUtils::toString(_countNow));
    }
}

void PopupPetEquipBoxBuyCount::drawButtons()
{
    auto layout = _uiContainerButtons;
    Size size = layout->getContentSize();
    
    // align
    double padding = 10;
    double widthButtons = -padding;
    for ( auto& uiBtn : layout->getChildren() )
    {
        widthButtons += uiBtn->getContentSize().width * uiBtn->getScaleX() + padding;
    }
    
    float posX = (size.width / 2.0f) - (widthButtons / 2.0f);
    float posY = size.height / 2.0f;
    for (const auto &child : layout->getChildren())
    {
        child->setPosition(posX + child->getContentSize().width * child->getScaleX() / 2.0f, posY);
        posX += child->getContentSize().width * child->getScaleX() + padding;
    }
}

#pragma mark - get, set
void PopupPetEquipBoxBuyCount::setCallback(const std::function<void(E_PET_EQUIP_BUY, int)>& callback)
{
    _callbackResult = callback;
}

#pragma mark - callback

#pragma mark - click
void PopupPetEquipBoxBuyCount::onClickClose(Ref* sender)
{
    hide();
}

void PopupPetEquipBoxBuyCount::onClickArrow(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    if ( uiBtn->getTag() == 1 )
    {
        _countNow -= 1;
        if ( _countNow <= 0 )
        {
            _countNow = _countMax;
        }
    }
    else if ( uiBtn->getTag() == 2 )
    {
        _countNow += 1;
        if ( _countNow > _countMax )
        {
            _countNow = 1;
        }
    }
    
    if ( _countNow > _countMax )
    {
        _countNow = _countMax;
    }
    
    //
    drawInfo();
}

void PopupPetEquipBoxBuyCount::onClickBuy(Ref* sender)
{
    if ( _callbackResult != nullptr )
    {
        _callbackResult(_eType, _countNow);
    }
    
    hide();
}
