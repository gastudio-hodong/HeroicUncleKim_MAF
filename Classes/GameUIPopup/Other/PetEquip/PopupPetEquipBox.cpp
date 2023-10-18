//
//  PopupPetEquipBox.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 31/07/2019.
//


#include "PopupPetEquipBox.h"

#include "GameObject/InfoPetEquip.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/PetEquip/PopupPetEquipUseMaxWarning.h"
#include "GameUIPopup/Other/PetEquip/PopupPetEquipBoxReward.h"
#include "GameUIPopup/Other/PetEquip/PopupPetEquipBoxBuyCount.h"
#include "GameUIPopup/Other/PetEquip/PopupPetEquipBoxBuyPurchase.h"
#include "GameUIPopup/Other/PurchaseCash/PopupCash.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/PetEquipBoxManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupPetEquipBox* PopupPetEquipBox::create()
{
    PopupPetEquipBox* pRet = new(std::nothrow) PopupPetEquipBox();
    if ( pRet && pRet->init() )
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

PopupPetEquipBox::PopupPetEquipBox(void) :
_bInitUI(false),

_boxBuyType(E_PET_EQUIP_BUY::COIN),
_boxBuyCount(0),

//
_uiContainerInfo(nullptr),
_uiContainerBuy(nullptr),
_uiContainerEvent(nullptr),

_uiBox(nullptr),
_uiBoxBoost(nullptr),
_uiBoxSpecialCount(nullptr),
_uiBoxNormalCount(nullptr),
_uiBoxButtons(nullptr)
{
    
}

PopupPetEquipBox::~PopupPetEquipBox(void)
{
    
}

bool PopupPetEquipBox::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    
    //
    initVar();
    initUi();
    
    //
    RefreshManager::getInstance()->addUI(E_REFRESH::PET_EQUIP, this);
    
    return true;
}

void PopupPetEquipBox::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiInfo();
        uiBuy();
        uiEvent();
    }
    
    //
    drawInfo();
    drawBuy();
    drawEvent();
    
    //
    PopupLoading::show();
    PetEquipBoxManager::getInstance()->requestEquipBoxInfo(CC_CALLBACK_1(PopupPetEquipBox::callbackInfo, this));
}

#pragma mark - refresh
void PopupPetEquipBox::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::PET_EQUIP )
    {
        drawInfo();
        drawBuy();
        drawEvent();
    }
}

#pragma mark - init
void PopupPetEquipBox::initVar()
{
    
}

void PopupPetEquipBox::initUi()
{
    // size 736
    _spriteContainer->setContentSize( Size(744, 976) );
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(25);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_pet_equip_randombox"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupPetEquipBox::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::COSTUME_COIN);
    list.push_back(E_ITEMS::HONOR);
    list.push_back(E_ITEMS::MATERIAL);

    auto uiCurrency = UICurrency::create(list, Size(_spriteContainer->getContentSize().width, 50));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiCurrency->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(uiCurrency);
}

#pragma mark - ui
void PopupPetEquipBox::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = 736;
    size.height = size.height - 75 - 50 - 25;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 25));
    _spriteContainer->addChild(layout);

    _uiContainerInfo = ui::Layout::create();
    _uiContainerInfo->setContentSize( Size(size.width, 496) );
    _uiContainerInfo->setIgnoreAnchorPointForPosition(false);
    _uiContainerInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerInfo->setPosition( Vec2(size.width / 2, size.height) );
    layout->addChild(_uiContainerInfo);
    
    _uiContainerBuy = ui::Layout::create();
    _uiContainerBuy->setContentSize(Size(705, size.height - _uiContainerInfo->getContentSize().height));
    _uiContainerBuy->setIgnoreAnchorPointForPosition(false);
    _uiContainerBuy->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContainerBuy->setPosition( Vec2(size.width / 2, 0) );
    layout->addChild(_uiContainerBuy);
    
    // popup top
    _uiContainerEvent = ui::Layout::create();
    _uiContainerEvent->setContentSize(Size(617.9, 199.8));
    _uiContainerEvent->setIgnoreAnchorPointForPosition(false);
    _uiContainerEvent->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContainerEvent->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height) );
    _uiContainerEvent->setVisible(false);
    _spriteContainer->addChild(_uiContainerEvent);
}

void PopupPetEquipBox::uiInfo()
{
    auto layout = _uiContainerInfo;
    Size size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui/pet/pet_equipment_room_2.png");
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiBG->setPosition(Vec2(size.width/2, 0));
    uiBG->setScale(4);
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    layout->addChild(uiBG);
    
    auto uiDescBG = ui::ImageView::create("Assets/ui_common/base_box_1_2.png");
    uiDescBG->setScale9Enabled(true);
    uiDescBG->setContentSize(Size(size.width, 35));
    uiDescBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiDescBG->setPosition(Vec2(size.width/2, size.height));
    layout->addChild(uiDescBG);
    {
        auto uiDesc = ui::CText::create(GAME_TEXT("t_pet_equip_notice_reset"), GAME_FONT, 24);
        uiDesc->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiDesc->setPosition(Vec2(uiDescBG->getContentSize().width / 2, uiDescBG->getContentSize().height / 2));
        uiDesc->setTextAreaSize(Size(uiDescBG->getContentSize().width - 10, uiDesc->getContentSize().height));
        uiDesc->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiDesc->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiDesc->setTextOverflow(Label::Overflow::SHRINK);
        uiDesc->setTextColor(Color4B::YELLOW);
        uiDescBG->addChild(uiDesc);
    }
    
    // boost
    {
        double posX = size.width / 2;
        double posY = size.height - 75;
        
        _uiBoxBoost = ui::ImageView::create("Assets/ui/pet/boost_progress_1_1.png");
        _uiBoxBoost->setPosition(Vec2(posX, posY));
        layout->addChild(_uiBoxBoost);
        {
            auto uiBoostBar = ui::LoadingBar::create("Assets/ui/pet/boost_progress_1_2.png");
            uiBoostBar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            uiBoostBar->setPosition(Vec2(0, 0));
            uiBoostBar->setPercent(0);
            uiBoostBar->setName("UI_BAR");
            uiBoostBar->setVisible(false);
            _uiBoxBoost->addChild(uiBoostBar);
            
            //
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.1f);
            for ( int i = 1; i <= 22; i++ )
            {
                auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/pet/boost_progress_2_%d.png",i).c_str());
                spriteAni->getTexture()->setTexParameters(texParams);
                animation->addSpriteFrame(spriteAni->getSpriteFrame());
            }
            
            auto uiBoxBoostMax = ui::ImageView::create("Assets/ui/pet/boost_progress_1_1.png");
            uiBoxBoostMax->setPosition(Vec2(_uiBoxBoost->getContentSize().width / 2, _uiBoxBoost->getContentSize().height / 2));
            uiBoxBoostMax->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
            uiBoxBoostMax->setName("UI_BAR_MAX");
            uiBoxBoostMax->setVisible(false);
            _uiBoxBoost->addChild(uiBoxBoostMax);
        }
    }
    
    // box
    {
        double posX = size.width / 2;
        double posY = size.height / 2 + 25;
        
        _uiBox = ui::ImageView::create("Assets/ui/pet/pet_equipment_box_1.png");
        _uiBox->setPosition(Vec2(posX, posY));
        layout->addChild(_uiBox);
        
        auto seq = Sequence::create(FadeOut::create(1.5f), DelayTime::create(2), FadeIn::create(1.5f), DelayTime::create(2), nullptr);
        auto uiBoxSpeical = ui::ImageView::create("Assets/ui/pet/pet_equipment_box2_1.png");
        uiBoxSpeical->setPosition(Vec2(_uiBox->getContentSize().width / 2, _uiBox->getContentSize().height / 2));
        uiBoxSpeical->runAction(RepeatForever::create(seq));
        _uiBox->addChild(uiBoxSpeical);
    }
    
    // right Top
    {
        double posX = size.width - 80;
        double posY = 0;
        
        // help
        {
            posY = size.height - 80;
            
            auto uiHelp = ui::Button::create("Assets/ui_common/icon_help.png");
            uiHelp->setPosition(Vec2(posX, posY));
            uiHelp->addClickEventListener(CC_CALLBACK_1(PopupPetEquipBox::onClickHelp, this));
            layout->addChild(uiHelp);
            {
                auto uiText = ui::CText::create(GAME_TEXT("t_ui_costume_info"), GAME_FONT, 22);
                uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                uiText->setPosition(Vec2(uiHelp->getContentSize().width / 2, 0));
                uiText->setTextAreaSize(Size(uiHelp->getContentSize().width - 10, uiText->getContentSize().height));
                uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
                uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
                uiText->setTextOverflow(Label::Overflow::SHRINK);
                uiText->enableOutline(Color4B::BLACK, 2);
                uiHelp->addChild(uiText);
            }
        }
    }
    
    // left top
    {
        double posX = 80;
        double posY = 0;
        
        // special box count
        {
            posY = size.height - 80;
            
            auto uiBoxSpecial = ui::ImageView::create("Assets/icon/icon_pet_equipment_2.png");
            uiBoxSpecial->setPosition(Vec2(posX, posY));
            uiBoxSpecial->setScale(2);
            ((ui::Scale9Sprite*)uiBoxSpecial->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            layout->addChild(uiBoxSpecial);
            
            _uiBoxSpecialCount = ui::CText::create("x 0", GAME_FONT, 24);
            _uiBoxSpecialCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            _uiBoxSpecialCount->setPosition(Vec2(posX + uiBoxSpecial->getBoundingBox().size.width / 2, posY));
            _uiBoxSpecialCount->enableOutline(Color4B::BLACK, 2);
            layout->addChild(_uiBoxSpecialCount);
        }
        
        // normal box count
        {
            posY = size.height - 160;
            
            auto uiBoxNormal = ui::ImageView::create("Assets/icon/icon_pet_equipment.png");
            uiBoxNormal->setPosition(Vec2(posX, posY));
            uiBoxNormal->setScale(2);
            ((ui::Scale9Sprite*)uiBoxNormal->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            layout->addChild(uiBoxNormal);
            
            _uiBoxNormalCount = ui::CText::create("x 0", GAME_FONT, 24);
            _uiBoxNormalCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            _uiBoxNormalCount->setPosition(Vec2(posX + uiBoxNormal->getBoundingBox().size.width / 2, posY));
            _uiBoxNormalCount->enableOutline(Color4B::BLACK, 2);
            layout->addChild(_uiBoxNormalCount);
        }
        
        // event box count
        {
            posY = size.height - 240;
            
            auto uiBoxEvent = ui::ImageView::create("Assets/icon/icon_treasure.png");
            uiBoxEvent->setPosition(Vec2(posX, posY));
            uiBoxEvent->setScale(2);
            uiBoxEvent->setName("UI_BOX_EVENT_ICON");
            ((ui::Scale9Sprite*)uiBoxEvent->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiBoxEvent->setVisible(false);
            layout->addChild(uiBoxEvent);
            
            auto uiBoxEventCount = ui::CText::create("x 0", GAME_FONT, 24);
            uiBoxEventCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiBoxEventCount->setPosition(Vec2(posX + uiBoxEvent->getBoundingBox().size.width / 2, posY));
            uiBoxEventCount->enableOutline(Color4B::BLACK, 2);
            uiBoxEventCount->setName("UI_BOX_EVENT_COUNT");
            uiBoxEventCount->setVisible(false);
            layout->addChild(uiBoxEventCount);
        }
    }
    
    // button
    {
        double posX = size.width / 2;
        double posY = 62;
        
        _uiBoxButtons = ui::Layout::create();
        _uiBoxButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _uiBoxButtons->setPosition(Vec2(posX, posY));
        _uiBoxButtons->setLayoutType(ui::Layout::Type::HORIZONTAL);
        layout->addChild(_uiBoxButtons);
        
        // 일반상자 1
        {
            auto uiButton = ui::Button::create("Assets/ui_common/btn_1_1_on.png", "", "Assets/ui_common/btn_off.png");
            uiButton->setScale9Enabled(true);
            uiButton->setContentSize(Size(220, 104));
            uiButton->addClickEventListener(CC_CALLBACK_1(PopupPetEquipBox::onClickBox, this));
            uiButton->setTag(1);
            _uiBoxButtons->addChild(uiButton);
            
            auto uiText = ui::CText::create(GAME_TEXT("t_pet_equip_box_name_1") + "\n" + GAME_TEXTFORMAT("t_pet_equip_use", 1), GAME_FONT, 24);
            uiText->setPosition(Vec2(uiButton->getContentSize().width / 2, uiButton->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiButton->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiButton->addChild(uiText);
            
            {
                auto seq = Sequence::create(FadeOut::create(1.f), FadeIn::create(1.f), nullptr);
                
                auto uiMileageEffect = ui::ImageView::create("Assets/ui_common/btn_1_10_on.png");
                uiMileageEffect->setScale9Enabled(true);
                uiMileageEffect->setContentSize(Size(220, 104));
                uiMileageEffect->setPosition(Vec2(uiButton->getContentSize().width / 2, uiButton->getContentSize().height / 2));
                uiMileageEffect->runAction(RepeatForever::create(seq));
                uiMileageEffect->setName("UI_MILEAGE_EFFECT");
                uiMileageEffect->setVisible(false);
                uiButton->addChild(uiMileageEffect);
                
                auto uiMileageIcon = ui::ImageView::create("Assets/ui/pet/boost_1_2.png");
                uiMileageIcon->setPosition(Vec2(15, uiButton->getContentSize().height - 15));
                uiMileageIcon->setScale(0.7);
                uiMileageIcon->setName("UI_MILEAGE_ICON");
                uiMileageIcon->setVisible(false);
                uiButton->addChild(uiMileageIcon);
                
            }
        }
        
        // 일반상자 3
        {
            auto uiButton = ui::Button::create("Assets/ui_common/btn_1_1_on.png", "", "Assets/ui_common/btn_off.png");
            uiButton->setScale9Enabled(true);
            uiButton->setContentSize(Size(220, 104));
            uiButton->addClickEventListener(CC_CALLBACK_1(PopupPetEquipBox::onClickBox, this));
            uiButton->setTag(3);
            _uiBoxButtons->addChild(uiButton);
            {
                auto params = ui::LinearLayoutParameter::create();
                params->setMargin(ui::Margin(10, 0, 0, 0));
                uiButton->setLayoutParameter(params);
            }
            
            auto uiText = ui::CText::create(GAME_TEXT("t_pet_equip_box_name_1") + "\n" + GAME_TEXTFORMAT("t_pet_equip_use", 3), GAME_FONT, 24);
            uiText->setPosition(Vec2(uiButton->getContentSize().width / 2, uiButton->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiButton->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiButton->addChild(uiText);
        }
        
        // 스페셜 상자
        {
            auto uiButton = ui::Button::create("Assets/ui_common/btn_special_box_1_1.png", "", "Assets/ui_common/btn_off.png");
            uiButton->setScale9Enabled(true);
            uiButton->setContentSize(Size(220, 104));
            uiButton->addClickEventListener(CC_CALLBACK_1(PopupPetEquipBox::onClickBox, this));
            uiButton->setTag(999);
            _uiBoxButtons->addChild(uiButton);
            {
                auto params = ui::LinearLayoutParameter::create();
                params->setMargin(ui::Margin(10, 0, 0, 0));
                uiButton->setLayoutParameter(params);
            }
            
            {
                auto uiImpect = ui::ImageView::create("Assets/ui_common/base_text_3_1.png");
                uiImpect->setScale9Enabled(true);
                uiImpect->setContentSize(Size(220, 39));
                uiImpect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiImpect->setPosition(Vec2(uiButton->getContentSize().width / 2, uiButton->getContentSize().height - 5));
                uiButton->addChild(uiImpect);
                
                auto uiText = ui::CText::create(GAME_TEXT("t_pet_equip_box_special_deco"), GAME_FONT, 22);
                uiText->setPosition(Vec2(uiImpect->getContentSize().width / 2, uiImpect->getContentSize().height / 2));
                uiText->setTextAreaSize(Size(uiImpect->getContentSize().width - 10, uiText->getContentSize().height));
                uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
                uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
                uiText->setTextOverflow(Label::Overflow::SHRINK);
                uiImpect->addChild(uiText);
            }
            
            {
                auto seq = Sequence::create(FadeOut::create(1.f), FadeIn::create(1.f), nullptr);
                
                auto uiEffect = ui::ImageView::create("Assets/ui_common/btn_special_box_1_2.png");
                uiEffect->setPosition(Vec2(uiButton->getContentSize().width / 2, uiButton->getContentSize().height / 2));
                uiEffect->runAction(RepeatForever::create(seq));
                uiEffect->setName("UI_EFFECT");
                uiEffect->setVisible(false);
                uiButton->addChild(uiEffect);
            }
            
            {
                auto seq = Sequence::create(FadeOut::create(1.f), FadeIn::create(1.f), nullptr);
                
                auto uiMileageEffect = ui::ImageView::create("Assets/ui_common/btn_1_10_on.png");
                uiMileageEffect->setScale9Enabled(true);
                uiMileageEffect->setContentSize(Size(220, 104));
                uiMileageEffect->setPosition(Vec2(uiButton->getContentSize().width / 2, uiButton->getContentSize().height / 2));
                uiMileageEffect->runAction(RepeatForever::create(seq));
                uiMileageEffect->setName("UI_MILEAGE_EFFECT");
                uiMileageEffect->setVisible(false);
                uiButton->addChild(uiMileageEffect);
                
                auto uiMileageIcon = ui::ImageView::create("Assets/ui/pet/boost_1_2.png");
                uiMileageIcon->setPosition(Vec2(15, uiButton->getContentSize().height - 15));
                uiMileageIcon->setScale(0.7);
                uiMileageIcon->setName("UI_MILEAGE_ICON");
                uiMileageIcon->setVisible(false);
                uiButton->addChild(uiMileageIcon);
            }
            
            auto uiText = ui::CText::create(GAME_TEXT("t_pet_equip_box_name_2") + "\n" + GAME_TEXT("t_ui_prison_use"), GAME_FONT, 24);
            uiText->setPosition(Vec2(uiButton->getContentSize().width / 2, uiButton->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiButton->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiButton->addChild(uiText);
        }
        
        auto sizeButtons = UtilsWidget::getInstance()->getLayoutAccumulatedSize(_uiBoxButtons);
        _uiBoxButtons->setContentSize(sizeButtons);
        _uiBoxButtons->setPositionX(posX);
        _uiBoxButtons->requestDoLayout();
    }
}

void PopupPetEquipBox::uiBuy()
{
    auto layout = _uiContainerBuy;
    Size size = layout->getContentSize();
    
    auto uiTitleBG = ui::ImageView::create("Assets/ui_common/base_text_1_3.png");
    uiTitleBG->setScale9Enabled(true);
    uiTitleBG->setContentSize(Size(size.width, 50));
    uiTitleBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiTitleBG->setPosition(Vec2(size.width/2, size.height));
    layout->addChild(uiTitleBG);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_pet_equip_randombox_purchase"), GAME_FONT, 28);
        uiText->setPosition(Vec2(uiTitleBG->getContentSize().width / 2, uiTitleBG->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(uiTitleBG->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextColor(Color4B(COLOR_TITLE));
        uiTitleBG->addChild(uiText);
    }
    
    // 66(165 > 660 )86(215 > )
    auto uiInfo = ui::Layout::create();
    uiInfo->setContentSize(Size(size.width, 65));
    uiInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiInfo->setPosition(Vec2(size.width / 2, size.height - uiTitleBG->getContentSize().height));
    uiInfo->setName("UI_INFO");
    layout->addChild(uiInfo);
    {
        auto uiFreeBG = ui::ImageView::create("Assets/ui/pet/box_buy_progressbar_1.png");
        uiFreeBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiFreeBG->setPosition(Vec2(0, 10));
        uiInfo->addChild(uiFreeBG);
         
        auto uiFreeBar = ui::LoadingBar::create("Assets/ui/pet/box_buy_progressbar_2.png");
        uiFreeBar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiFreeBar->setPosition(Vec2(0, 0));
        uiFreeBar->setPercent(50);
        uiFreeBar->setName("UI_BAR");
        uiFreeBG->addChild(uiFreeBar);
        
        auto uiIcon = ui::ImageView::create("Assets/icon/icon_pet_equipment.png");
        uiIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiIcon->setPosition(Vec2(-10, 35));
        uiFreeBG->addChild(uiIcon);
        
        auto uiFreeTitle = ui::CText::create(GAME_TEXTFORMAT("t_pet_equip_box_free_count_1", 0, 0), GAME_FONT, 20);
        uiFreeTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiFreeTitle->setPosition(Vec2(50, 35));
        uiFreeTitle->setName("UI_TITLE");
        uiFreeBG->addChild(uiFreeTitle);
        
        auto uiFree = ui::Button::create("Assets/ui_common/btn_2_3_on.png", "", "Assets/ui_common/btn_2_1_off.png");
        uiFree->setScale9Enabled(true);
        uiFree->setContentSize(Size(146, 46));
        uiFree->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiFree->setPosition(Vec2(uiFreeBG->getContentSize().width + 10, 0));
        uiFree->addClickEventListener(CC_CALLBACK_1(PopupPetEquipBox::onClickFree, this));
        uiFree->setName("UI_FREE");
        uiFreeBG->addChild(uiFree);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_raid_event_cooper_msg_7"), GAME_FONT, 22);
            uiText->setPosition(Vec2(uiFree->getContentSize().width / 2, uiFree->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiFree->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setName("UI_FREE_TEXT");
            uiFree->addChild(uiText);
        }
    }
    
    auto uiList = ui::Layout::create();
    uiList->setContentSize(Size(size.width, size.height - uiTitleBG->getContentSize().height - uiInfo->getContentSize().height));
    uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiList->setPosition(Vec2(size.width / 2, 0));
    uiList->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiList->setName("UI_LIST");
    layout->addChild(uiList);
    {
        //
        {
            auto uiChild = ui::Layout::create();
            uiChild->setContentSize(Size(66 * 2.5, 86 * 2.5));
            uiChild->setTag(enumToInt(E_PET_EQUIP_BUY::COIN));
            uiChild->setTouchEnabled(true);
            uiChild->addClickEventListener(CC_CALLBACK_1(PopupPetEquipBox::onClickBuy, this));
            uiList->addChild(uiChild);
            
            auto uiBG = ui::ImageView::create("Assets/ui/pet/pet_equipment_shop_1_1.png");
            uiBG->setPosition(Vec2(uiChild->getContentSize().width / 2, uiChild->getContentSize().height / 2));
            uiBG->setScale(uiChild->getContentSize().width / uiBG->getContentSize().width);
            ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiChild->addChild(uiBG);
            
            auto uiTitle = ui::CText::create(GAME_TEXT("t_pet_equip_randombox_buy_3"), GAME_FONT, 20);
            uiTitle->setPosition(Vec2(uiChild->getContentSize().width / 2, uiChild->getContentSize().height - 20));
            uiTitle->setTextAreaSize(Size(uiChild->getContentSize().width - 10, uiTitle->getContentSize().height));
            uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiTitle->setTextOverflow(Label::Overflow::SHRINK);
            uiTitle->setTextColor(Color4B(COLOR_TITLE));
            uiChild->addChild(uiTitle);
            
            auto uiCount = ui::CText::create("(0/0)", GAME_FONT, 18);
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
            uiChild->setContentSize(Size(66 * 2.5, 86 * 2.5));
            uiChild->setTag(enumToInt(E_PET_EQUIP_BUY::HONOR));
            uiChild->setTouchEnabled(true);
            uiChild->addClickEventListener(CC_CALLBACK_1(PopupPetEquipBox::onClickBuy, this));
            uiList->addChild(uiChild);
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
            
            auto uiTitle = ui::CText::create(GAME_TEXT("t_pet_equip_randombox_buy_1"), GAME_FONT, 20);
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
            uiChild->setContentSize(Size(66 * 2.5, 86 * 2.5));
            uiChild->setTag(enumToInt(E_PET_EQUIP_BUY::BONG));
            uiChild->setTouchEnabled(true);
            uiChild->addClickEventListener(CC_CALLBACK_1(PopupPetEquipBox::onClickBuy, this));
            uiList->addChild(uiChild);
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
            
            auto uiTitle = ui::CText::create(GAME_TEXT("t_pet_equip_randombox_buy_2"), GAME_FONT, 20);
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
        {
            auto uiChild = ui::Layout::create();
            uiChild->setContentSize(Size(66 * 2.5, 86 * 2.5));
            uiChild->setTag(enumToInt(E_PET_EQUIP_BUY::CASH));
            uiChild->setTouchEnabled(true);
            uiChild->addClickEventListener(CC_CALLBACK_1(PopupPetEquipBox::onClickBuy, this));
            uiList->addChild(uiChild);
            {
                auto params = ui::LinearLayoutParameter::create();
                params->setMargin(ui::Margin(15, 0, 0, 0));
                uiChild->setLayoutParameter(params);
            }
            
            auto uiBG = ui::ImageView::create("Assets/ui/pet/pet_equipment_shop_1_4.png");
            uiBG->setPosition(Vec2(uiChild->getContentSize().width / 2, uiChild->getContentSize().height / 2));
            uiBG->setScale(uiChild->getContentSize().width / uiBG->getContentSize().width);
            ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiChild->addChild(uiBG);
            
            auto uiTitle = ui::CText::create(GAME_TEXT("t_pet_equip_randombox_buy_6"), GAME_FONT, 20);
            uiTitle->setPosition(Vec2(uiChild->getContentSize().width / 2, uiChild->getContentSize().height - 20));
            uiTitle->setTextAreaSize(Size(uiChild->getContentSize().width - 10, uiTitle->getContentSize().height));
            uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiTitle->setTextOverflow(Label::Overflow::SHRINK);
            uiTitle->setTextColor(Color4B(COLOR_TITLE));
            uiChild->addChild(uiTitle);
            
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_costume_skin_msg_2"), GAME_FONT, 18);
            uiText->setPosition(Vec2(uiChild->getContentSize().width / 2, 32));
            uiText->setTextAreaSize(Size(130, uiTitle->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiChild->addChild(uiText);
        }
        
        auto sizeList = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiList);
        uiList->setContentSize(sizeList);
        uiList->requestDoLayout();
    }
}

void PopupPetEquipBox::uiEvent()
{
    auto layout = _uiContainerEvent;
    Size size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui/pet/event_pet_box_1_1.png");
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    uiBG->setScale(3.7);
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    layout->addChild(uiBG);
    
    auto uiTime = ui::CText::create("", GAME_FONT, 22);
    uiTime->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    uiTime->setPosition(Vec2(size.width - 10, size.height - 35));
    uiTime->enableOutline(Color4B::BLACK, 2);
    uiTime->setName("UI_TIME");
    layout->addChild(uiTime);
    
    auto uiDay = ui::CText::create("", GAME_FONT, 22);
    uiDay->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    uiDay->setPosition(Vec2(size.width - 10, size.height - 55));
    uiDay->enableOutline(Color4B::BLACK, 2);
    uiDay->setTextColor(Color4B(COLOR_COIN));
    uiDay->setName("UI_DAY");
    layout->addChild(uiDay);
    
    auto uiReddot = UIReddot::create(E_REDDOT_TYPE::EVENT, Vec2::ANCHOR_TOP_RIGHT);
    uiReddot->setPosition(size.width - 20, size.height - 15);
    layout->addChild(uiReddot);
    
    auto uiHelpEvent = ui::Button::create("Assets/ui_common/icon_help.png");
    uiHelpEvent->setPosition(Vec2(size.width - 50, 50));
    uiHelpEvent->addClickEventListener(CC_CALLBACK_1(PopupPetEquipBox::onClickHelpEvent, this));
    layout->addChild(uiHelpEvent);
}

#pragma mark - draw
void PopupPetEquipBox::drawInfo()
{
    auto layout = _uiContainerInfo;
    Size size = layout->getContentSize();
    
    
    int boxSpecialCount = PetEquipBoxManager::getInstance()->getEquipBoxSpecialCount();
    int boxNormalCount = PetEquipBoxManager::getInstance()->getEquipBoxCount();
    int boxEventCount = PetEquipBoxManager::getInstance()->getEquipBoxEventCount();
    
    int mileageNow = PetEquipBoxManager::getInstance()->getEquipBoxMileage();
    int mileageMax = ParameterManager::getInstance()->getParameterInteger("pet_equip_box_mileage");
    
    bool bEvent = EventManager::getInstance()->isActivatedEvent(16);
    
    // box
    {
        _uiBox->loadTexture("Assets/ui/pet/pet_equipment_box_1.png");
        if ( bEvent == true )
        {
            _uiBox->loadTexture("Assets/ui/pet/pet_equipment_event_box_1.png");
        }
    }
    
    // box speical
    {
        _uiBoxSpecialCount->setString(MafUtils::format("x %d", boxSpecialCount));
    }
    
    // box normal
    {
        _uiBoxNormalCount->setString(MafUtils::format("x %d", boxNormalCount));
        _uiBoxNormalCount->setTextColor(Color4B::WHITE);
        _uiBoxNormalCount->disableEffect(LabelEffect::STRIKETHROUGH);
        _uiBoxNormalCount->removeAllChildren();
        
        if ( bEvent == true )
        {
            _uiBoxNormalCount->enableStrikethrough();
            _uiBoxNormalCount->setTextColor(Color4B(104, 104, 104, 255));
            
            auto uiRestrict = ui::CText::create(GAME_TEXT("t_pet_equip_restrict"), GAME_FONT, 28);
            uiRestrict->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiRestrict->setPosition(Vec2(_uiBoxNormalCount->getContentSize().width + 10, _uiBoxNormalCount->getContentSize().height / 2));
            uiRestrict->enableOutline(Color4B::BLACK, 2);
            _uiBoxNormalCount->addChild(uiRestrict);
        }
    }
    
    // box event
    {
        auto uiBoxEventIcon = layout->getChildByName<ui::ImageView*>("UI_BOX_EVENT_ICON");
        auto uiBoxEventCount = layout->getChildByName<ui::CText*>("UI_BOX_EVENT_COUNT");
        
        if ( bEvent == true )
        {
            uiBoxEventIcon->setVisible(true);
            uiBoxEventCount->setVisible(true);
            uiBoxEventCount->setString(MafUtils::format("x %d", boxEventCount));
        }
        else
        {
            uiBoxEventIcon->setVisible(false);
            uiBoxEventCount->setVisible(false);
        }
    }
    
    // boost
    {
        double percent = (double)mileageNow / mileageMax * 100;
        if ( percent > 100 )
            percent = 100;
        
        auto uiBoostBar = _uiBoxBoost->getChildByName<ui::LoadingBar*>("UI_BAR");
        auto uiBoxBoostMax = _uiBoxBoost->getChildByName<ui::ImageView*>("UI_BAR_MAX");
        
        if ( percent >= 100 )
        {
            uiBoostBar->setVisible(false);
            uiBoxBoostMax->setVisible(true);
        }
        else
        {
            uiBoostBar->setVisible(true);
            uiBoostBar->setPercent(percent);
            uiBoxBoostMax->setVisible(false);
        }
    }
    
    
    // button
    {
        auto uiBoxNormal1 = _uiBoxButtons->getChildByTag<ui::Button*>(1);
        if ( uiBoxNormal1 != nullptr )
        {
            int useCount = 1;
            int boxCount = boxNormalCount;
            if ( bEvent == true )
            {
                boxCount = boxEventCount;
            }
            
            if ( mileageNow >= mileageMax )
            {
                uiBoxNormal1->getChildByName("UI_MILEAGE_EFFECT")->setVisible(true);
                uiBoxNormal1->getChildByName("UI_MILEAGE_ICON")->setVisible(true);
            }
            else
            {
                uiBoxNormal1->getChildByName("UI_MILEAGE_EFFECT")->setVisible(false);
                uiBoxNormal1->getChildByName("UI_MILEAGE_ICON")->setVisible(false);
            }
            
            if ( useCount <= boxCount )
            {
                uiBoxNormal1->setEnabled(true);
            }
            else
            {
                uiBoxNormal1->setEnabled(false);
            }
        }
        
        auto uiBoxNormal3 = _uiBoxButtons->getChildByTag<ui::Button*>(3);
        if ( uiBoxNormal3 != nullptr )
        {
            int useCount = 3;
            int boxCount = boxNormalCount;
            if ( bEvent == true )
            {
                boxCount = boxEventCount;
            }
            
            if ( mileageNow >= mileageMax )
            {
                uiBoxNormal3->setEnabled(false);
            }
            else
            {
                uiBoxNormal3->setEnabled(true);
                
                if ( useCount <= boxCount )
                {
                    uiBoxNormal3->setEnabled(true);
                }
                else
                {
                    uiBoxNormal3->setEnabled(false);
                }
            }
        }
        
        auto uiBoxSpecial = _uiBoxButtons->getChildByTag<ui::Button*>(999);
        if ( uiBoxSpecial != nullptr )
        {
            int useCount = 1;
            int boxCount = boxSpecialCount;
            
            if ( boxSpecialCount > 0 )
            {
                uiBoxSpecial->getChildByName("UI_EFFECT")->setVisible(true);
            }
            else
            {
                uiBoxSpecial->getChildByName("UI_EFFECT")->setVisible(false);
            }
            
            if ( mileageNow >= mileageMax )
            {
                uiBoxSpecial->getChildByName("UI_MILEAGE_EFFECT")->setVisible(true);
                uiBoxSpecial->getChildByName("UI_MILEAGE_ICON")->setVisible(true);
            }
            else
            {
                uiBoxSpecial->getChildByName("UI_MILEAGE_EFFECT")->setVisible(false);
                uiBoxSpecial->getChildByName("UI_MILEAGE_ICON")->setVisible(false);
            }
            
            if ( useCount <= boxCount )
            {
                uiBoxSpecial->setEnabled(true);
            }
            else
            {
                uiBoxSpecial->setEnabled(false);
            }
        }
    }
}

void PopupPetEquipBox::drawBuy()
{
    auto layout = _uiContainerBuy;
    Size size = layout->getContentSize();
    
    auto uiInfo = layout->getChildByName<ui::Layout*>("UI_INFO");
    if ( uiInfo != nullptr )
    {
        int countNow = PetEquipBoxManager::getInstance()->getEquipBoxOpenHonor() + PetEquipBoxManager::getInstance()->getEquipBoxOpenBong() + PetEquipBoxManager::getInstance()->getEquipBoxOpenDevil() + PetEquipBoxManager::getInstance()->getEquipBoxOpenCash();
        int countMax = PetEquipBoxManager::getInstance()->getPetEquipFreeBoxCount();
        if ( countNow > countMax )
        {
            countNow = countMax;
        }
        bool bCountMax = countNow >= countMax ? true : false;
        
        double percent = (double)countNow / countMax * 100;
        if ( percent > 100 )
            percent = 100;
        
        auto uiFreeBar = (ui::LoadingBar*)utils::findChild(uiInfo, "UI_BAR");
        uiFreeBar->setPercent(percent);
        
        auto uiFreeTitle = (ui::CText*)utils::findChild(uiInfo, "UI_TITLE");
        uiFreeTitle->setString(GAME_TEXTFORMAT("t_pet_equip_box_free_count_1", countNow, countMax));
        
        auto uiFree = (ui::Button*)utils::findChild(uiInfo, "UI_FREE");
        auto uiFreeText = (ui::CText*)utils::findChild(uiInfo, "UI_FREE_TEXT");
        if ( bCountMax == false )
        {
            uiFree->setEnabled(false);
            uiFreeText->setString(GAME_TEXT("t_ui_raid_event_cooper_msg_7"));
            uiFreeText->setTextColor(Color4B::GRAY);
        }
        else if ( PetEquipBoxManager::getInstance()->getEquipBoxOpenFree() >= 1 )
        {
            uiFree->setEnabled(false);
            uiFreeText->setString(GAME_TEXT("t_ui_raid_event_cooper_msg_6"));
            uiFreeText->setTextColor(Color4B::GRAY);
        }
        else
        {
            uiFree->setEnabled(true);
            uiFreeText->setString(GAME_TEXT("t_ui_raid_event_cooper_msg_7"));
            uiFreeText->setTextColor(Color4B::WHITE);
        }
    }
    
    auto uiList = layout->getChildByName<ui::Layout*>("UI_LIST");
    if ( uiList != nullptr )
    {
        bool bEvent = EventManager::getInstance()->isActivatedEvent(16);
        
        //
        {
            auto uiChild = uiList->getChildByTag<ui::Button*>(enumToInt(E_PET_EQUIP_BUY::COIN));
            
            auto uiCost = uiChild->getChildByName<ui::CText*>("UI_COST");
            if ( uiCost != nullptr )
            {
                uiCost->setString(MafUtils::format("x %d", PetEquipBoxManager::getInstance()->getCost(E_PET_EQUIP_BUY::COIN, bEvent)));
            }
            
            auto uiCount = uiChild->getChildByName<ui::CText*>("UI_COUNT");
            if ( uiCount != nullptr )
            {
                int countNow = PetEquipBoxManager::getInstance()->getEquipBoxOpenDevil();
                int countMax = PetEquipBoxManager::getInstance()->getEquipBoxOpenDevilMax();
                
                uiCount->setString(MafUtils::format("(%d/%d)", countNow, countMax));
            }
        }
        
        //
        {
            auto uiChild = uiList->getChildByTag<ui::Button*>(enumToInt(E_PET_EQUIP_BUY::HONOR));
            auto uiCost = uiChild->getChildByName<ui::CText*>("UI_COST");
            if ( uiCost != nullptr )
            {
                uiCost->setString(MafUtils::format("x %d", PetEquipBoxManager::getInstance()->getCost(E_PET_EQUIP_BUY::HONOR, bEvent)));
            }
        }
        
        //
        {
            auto uiChild = uiList->getChildByTag<ui::Button*>(enumToInt(E_PET_EQUIP_BUY::BONG));
            auto uiCost = uiChild->getChildByName<ui::CText*>("UI_COST");
            if ( uiCost != nullptr )
            {
                uiCost->setString(MafUtils::format("x %d", PetEquipBoxManager::getInstance()->getCost(E_PET_EQUIP_BUY::BONG, bEvent)));
            }
        }
    }
    
}

void PopupPetEquipBox::drawEvent()
{
    auto layout = _uiContainerEvent;
    Size size = layout->getContentSize();
    
    bool bEvent = EventManager::getInstance()->isActivatedEvent(16);
    
    if ( bEvent == false )
    {
        layout->setVisible(false);
    }
    else
    {
        layout->setVisible(true);
        
        auto objEvent = EventManager::getInstance()->getActivatedEvent(16);
        
        auto startTime = UtilsDate::getInstance()->getTimeString(objEvent->getEventStart(), UtilsDate::eType::date_real_Ymd);
        auto endTime = UtilsDate::getInstance()->getTimeString(objEvent->getEventEnd(), UtilsDate::eType::date_real_Ymd);
        auto timer = EventManager::getInstance()->getActivatedEvent(16)->getEventEnd() - UtilsDate::getInstance()->getTime();
        
        int day = timer / (24 * 3600);
        
        auto uiTime = layout->getChildByName<ui::CText*>("UI_TIME");
        uiTime->setString(startTime + " ~ " + endTime);
        
        auto uiDay = layout->getChildByName<ui::CText*>("UI_DAY");
        uiDay->setString("(D-" + MafUtils::toString(day) + ")");
    }
}

#pragma mark - callback
void PopupPetEquipBox::callbackInfo(bool bResult)
{
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    drawInfo();
    drawBuy();
    drawEvent();
}

void PopupPetEquipBox::callbackFree(bool bResult, int nResult, int eType)
{
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 0 )         strMessage = GAME_TEXT("t_ui_error_31");
        else if ( nResult == 2 )    strMessage = GAME_TEXT("t_ui_error_full");
        else if ( nResult == 4 )    strMessage = GAME_TEXT("t_ui_error_37");
        else if ( nResult == -1 )   strMessage = GAME_TEXT("t_ui_error_2");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_pet_equip_notice_freebox"));
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->show();
    
    //
    drawInfo();
    drawBuy();
    drawEvent();
}

void PopupPetEquipBox::callbackBoxBuy(bool bResult, int nResult, int eType)
{
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        _boxBuyCount = 0;
        
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 0 )         strMessage = GAME_TEXT("t_ui_error_31");
        else if ( nResult == 2 )    strMessage = GAME_TEXT("t_ui_error_full");
        else if ( nResult == 4 )    strMessage = GAME_TEXT("t_ui_error_37");
        else if ( nResult == -1 )   strMessage = GAME_TEXT("t_ui_error_2");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    drawInfo();
    drawBuy();
    drawEvent();
    
    //
    if ( _boxBuyCount > 0 )
    {
        onBoxBuyAll();
    }
}


#pragma mark - click
void PopupPetEquipBox::onClickClose(Ref* sender)
{
    hide();
}

void PopupPetEquipBox::onClickHelp(cocos2d::Ref* sender)
{
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_pet_equip_box_guide"));
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->setContentSize(Size(655, 825));
    popup->setContentAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    popup->show();
}

void PopupPetEquipBox::onClickHelpEvent(cocos2d::Ref* sender)
{
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_pet_equip_event_rule_detail"));
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->setContentSize(Size(655, 825));
    popup->setContentAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    popup->show();
}

void PopupPetEquipBox::onClickBox(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    bool bEvent = EventManager::getInstance()->isActivatedEvent(16);
    
    int useCount = 0;
    int useBox = 0;
    int idx = uiBtn->getTag();
    if ( idx == 1 )
    {
        useCount = 1;
        useBox = 1;
        if ( bEvent == true )
            useBox = 2;
    }
    else if ( idx == 3 )
    {
        useCount = 3;
        useBox = 1;
        if ( bEvent == true )
            useBox = 2;
    }
    else if ( idx == 999 )
    {
        useCount = 1;
        useBox = 3;
    }
    
    int equipMaxCount = PetNewManager::getInstance()->getPetEquipMaxUseCount();
    if ( equipMaxCount >= PetNewManager::getInstance()->getListPetEquipAll().size() )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_pet_equip_warn_limit_3"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;;
    }
    else
    {
        auto timeSaveText = UserDefault::getInstance()->getStringForKey("PET_EQUIP_MAX_WARNING", "0");
        auto timeSave = std::atoi(timeSaveText.c_str());
        auto timeNow = UtilsDate::getInstance()->getTime();
        
        int diff = difftime(timeNow, timeSave) / 86400;
        if ( diff <= 0 && equipMaxCount >= 1 )
        {
            auto popup = PopupPetEquipUseMaxWarning::create(useCount, useBox);
            popup->setCallback([=](int userCount, int userBox){
                auto popup = PopupPetEquipBoxReward::create(E_PET_BOX_TYPE::CASH);
                popup->onUseBox(useCount, useBox);
                popup->show();
            });
            popup->show();
            return;
        }
    }
    
    auto popup = PopupPetEquipBoxReward::create(E_PET_BOX_TYPE::CASH);
    popup->onUseBox(useCount, useBox);
    popup->show();
}

void PopupPetEquipBox::onClickBuy(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiSender = dynamic_cast<ui::Layout*>(sender);
    

    E_PET_EQUIP_BUY eType = enumFromInt<E_PET_EQUIP_BUY>(uiSender->getTag());
    
    bool bEvent = EventManager::getInstance()->isActivatedEvent(16);
    

    //
    if ( eType == E_PET_EQUIP_BUY::CASH )
    {
        auto popup = PopupPetEquipBoxBuyPurchase::create();
        popup->show();
    }
    else
    {
        int itemNow = 0;
        int itemNeed = PetEquipBoxManager::getInstance()->getCost(eType, bEvent);
        std::string itemName = "";
        
        int count = 0;
        int countMax = 999;
        
        if ( eType == E_PET_EQUIP_BUY::HONOR )
        {
            itemNow = std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::HONOR).c_str());
            itemName = "t_ui_error_21";
        }
        else if ( eType == E_PET_EQUIP_BUY::BONG )
        {
            itemNow = std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::MATERIAL).c_str());
            itemName = "t_ui_error_19";
        }
        else if ( eType == E_PET_EQUIP_BUY::COIN )
        {
            itemNow = std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str());
            itemName = "t_ui_error_17";
            
            count = PetEquipBoxManager::getInstance()->getEquipBoxOpenDevil();
            countMax = PetEquipBoxManager::getInstance()->getEquipBoxOpenDevilMax();
        }
            
        
        if ( itemNow < itemNeed )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(itemName));
            popup->addButton(GAME_TEXT("t_ui_purchase_bonus_msg_4"), false, [=](Ref* sender){
                auto popup = PopupCash::create();
                popup->show();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
        if ( count >= countMax )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_35"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
        auto popup = PopupPetEquipBoxBuyCount::create(eType, bEvent);
        popup->setCallback([=](E_PET_EQUIP_BUY eType, int count){
            _boxBuyType = eType;
            _boxBuyCount = count;
            
            //
            onBoxBuyAll();
        });
        
        popup->show();
    }
    
}

void PopupPetEquipBox::onClickFree(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    int countNow = PetEquipBoxManager::getInstance()->getEquipBoxOpenFree();
    int countMax = 1;
    
    if ( countNow >= countMax )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_35"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    PopupLoading::show();
    PetEquipBoxManager::getInstance()->requestEquipBoxBuy(enumToInt(E_PET_EQUIP_BUY::FREE), CC_CALLBACK_3(PopupPetEquipBox::callbackFree, this));
    
}

#pragma mark -
void PopupPetEquipBox::onBoxBuyAll()
{
    if ( _boxBuyCount <= 0 )
    {
        return;
    }
    
    _boxBuyCount--;
    
    PopupLoading::show();
    PetEquipBoxManager::getInstance()->requestEquipBoxBuy(enumToInt(_boxBuyType), CC_CALLBACK_3(PopupPetEquipBox::callbackBoxBuy, this));
}
