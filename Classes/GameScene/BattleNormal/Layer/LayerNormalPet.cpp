//
//  LayerNormalPet.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "LayerNormalPet.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUI/UICell/UILayoutPetCell.h"
#include "GameUIPopup/Other/PetEquip/PopupPetEquipBox.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/PetNewManager.h"

USING_NS_CC;

LayerNormalPet* LayerNormalPet::create()
{
    LayerNormalPet* pRet = new(std::nothrow) LayerNormalPet();
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

LayerNormalPet::LayerNormalPet(void) :
_bInitUI(false),

// widget
_uiContainerInfo(nullptr),
_uiContainerList(nullptr)

{
    
}

LayerNormalPet::~LayerNormalPet(void)
{
    
}

bool LayerNormalPet::init()
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    
    // init
    initVar();
    initUi();

    //
    RefreshManager::getInstance()->addUI(E_REFRESH::PET_EQUIP, this);
    
    return true;
}

void LayerNormalPet::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiInfo();
        uiList();
    }
    
    //
    drawInfo();
}

#pragma mark - refresh
void LayerNormalPet::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::PET_EQUIP )
    {
        drawInfo();
    }
}
  

#pragma mark - init
void LayerNormalPet::initVar()
{
    
}

void LayerNormalPet::initUi()
{

}

#pragma mark - ui : defalut
void LayerNormalPet::uiContainer()
{
    Size size = this->getContentSize();
    
    _uiContainerInfo = ui::Layout::create();
    _uiContainerInfo->setContentSize(Size(size.width, 110));
    _uiContainerInfo->setIgnoreAnchorPointForPosition(false);
    _uiContainerInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContainerInfo->setPosition(Vec2(size.width / 2, size.height));
    addChild(_uiContainerInfo);
    
    _uiContainerList = ui::Layout::create();
    _uiContainerList->setContentSize(Size(size.width, size.height - _uiContainerInfo->getContentSize().height));
    _uiContainerList->setIgnoreAnchorPointForPosition(false);
    _uiContainerList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContainerList->setPosition(Vec2(size.width / 2, 0));
    addChild(_uiContainerList);
}

void LayerNormalPet::uiInfo()
{
    auto layout = _uiContainerInfo;
    Size size = layout->getContentSize();
    
    auto uiGuideBG = ui::ImageView::create("Assets/ui/pet/pet_guide_2.png");
    uiGuideBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiGuideBG->setPosition(Vec2(0, size.height));
    layout->addChild(uiGuideBG);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_pet_equip_desc_title_1"), GAME_FONT, 20);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiText->setPosition(Vec2(10, uiGuideBG->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(160, uiText->getContentSize().height));
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiGuideBG->addChild(uiText);
    }
    
    auto uiBonus = ui::Layout::create();
    uiBonus->setContentSize(Size(480, 70));
    uiBonus->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiBonus->setPosition(Vec2(15, 10));
    uiBonus->setName("UI_BONUS");
    layout->addChild(uiBonus);
    {
        auto uiLine = ui::ImageView::create("Assets/ui_common/vertical_line.png");
        uiLine->setPosition(Vec2(uiBonus->getContentSize().width / 2, uiBonus->getContentSize().height / 2));
        uiBonus->addChild(uiLine);
        
        auto uiBonusPet = ui::CText::create(GAME_TEXTFORMAT("t_pet_equip_desc_content_3", 0), GAME_FONT, 18);
        uiBonusPet->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiBonusPet->setPosition(Vec2(0, uiBonus->getContentSize().height / 2));
        uiBonusPet->setTextAreaSize(Size(210, uiBonusPet->getContentSize().height));
        uiBonusPet->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiBonusPet->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiBonusPet->setTextOverflow(Label::Overflow::SHRINK);
        uiBonusPet->setTextColor(Color4B::GRAY);
        uiBonusPet->setName("UI_BONUS_PET");
        uiBonus->addChild(uiBonusPet);
        
        auto uiBonusPetEffect = ui::CText::create(GAME_TEXT("t_pet_equip_box_dmgbonus"), GAME_FONT, 20);
        uiBonusPetEffect->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiBonusPetEffect->setPosition(Vec2(0, uiBonus->getContentSize().height / 2));
        uiBonusPetEffect->setTextAreaSize(Size(210, uiBonusPetEffect->getContentSize().height));
        uiBonusPetEffect->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiBonusPetEffect->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiBonusPetEffect->setTextOverflow(Label::Overflow::SHRINK);
        uiBonusPetEffect->setName("UI_BONUS_PET_EFFECT");
        uiBonus->addChild(uiBonusPetEffect);
        
        auto uiBonusCostume = ui::CText::create(GAME_TEXT("t_pet_equip_desc_content_2"), GAME_FONT, 18);
        uiBonusCostume->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiBonusCostume->setPosition(Vec2(uiBonus->getContentSize().width / 2 + 30, uiBonus->getContentSize().height / 2));
        uiBonusCostume->setTextAreaSize(Size(210, uiBonusCostume->getContentSize().height));
        uiBonusCostume->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiBonusCostume->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiBonusCostume->setTextOverflow(Label::Overflow::SHRINK);
        uiBonusCostume->setTextColor(Color4B::GRAY);
        uiBonusCostume->setName("UI_BONUS_COSTUME");
        uiBonus->addChild(uiBonusCostume);
        
        auto uiBonusCostumeEffect = ui::CText::create(GAME_TEXTFORMAT("t_pet_equip_effect_per_bong_1", "0"), GAME_FONT, 20);
        uiBonusCostumeEffect->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiBonusCostumeEffect->setPosition(Vec2(uiBonus->getContentSize().width / 2 + 30, uiBonus->getContentSize().height / 2));
        uiBonusCostumeEffect->setTextAreaSize(Size(210, uiBonusCostumeEffect->getContentSize().height));
        uiBonusCostumeEffect->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiBonusCostumeEffect->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiBonusCostumeEffect->setTextOverflow(Label::Overflow::SHRINK);
        uiBonusCostumeEffect->setName("UI_BONUS_COSTUME_EFFECT");
        uiBonus->addChild(uiBonusCostumeEffect);
    }
    
    auto uiPetEquip = ui::Button::create("Assets/ui/pet/btn_summon.png");
    uiPetEquip->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiPetEquip->setPosition(Vec2(size.width - 10, size.height / 2));
    uiPetEquip->addClickEventListener(CC_CALLBACK_1(LayerNormalPet::onClickPetEquip, this));
    uiPetEquip->setName("UI_PETEQUIP");
    layout->addChild(uiPetEquip);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_pet_equip_box_title_1"), GAME_FONT, 20);
        uiText->setPosition(Vec2(uiPetEquip->getContentSize().width - 64, 29.5));
        uiText->setTextAreaSize(Size(120, uiText->getContentSize().height));
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiPetEquip->addChild(uiText);
        
        auto uiReddotEvent = UIReddot::create(E_REDDOT_TYPE::EVENT, Vec2::ANCHOR_MIDDLE);
        uiReddotEvent->setPosition(uiPetEquip->getContentSize().width - 64, 59);
        uiReddotEvent->setName("UI_REDDOT_EVENT");
        uiPetEquip->addChild(uiReddotEvent);
        
        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
        uiReddot->setPosition(3, uiPetEquip->getContentSize().height - 10);
        uiReddot->setVisible(false);
        uiPetEquip->addChild(uiReddot);
        RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_BOX, uiReddot);
    }
}

void LayerNormalPet::uiList()
{
    auto layout = _uiContainerList;
    Size size = layout->getContentSize();
    
    auto uiCell = UILayoutPetCell::create();
    uiCell->setContentSize(size);
    uiCell->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(uiCell);
}

#pragma mark - ui : draw
void LayerNormalPet::drawInfo()
{
    auto layout = _uiContainerInfo;
    Size size = layout->getContentSize();
    
    auto uiBonus = layout->getChildByName<ui::Layout*>("UI_BONUS");
    if ( uiBonus != nullptr )
    {
        int collectLevelTotal = PetNewManager::getInstance()->getPetEquipCollectLevelTotal();
        
        auto uiBonusPet = uiBonus->getChildByName<ui::CText*>("UI_BONUS_PET");
        uiBonusPet->setString(GAME_TEXTFORMAT("t_pet_equip_desc_content_3", collectLevelTotal));
        
        auto uiBonusPetEffect = uiBonus->getChildByName<ui::CText*>("UI_BONUS_PET_EFFECT");
        if ( collectLevelTotal <= 310 )
        {
            std::string effectText = MafUtils::format("%.2f", std::floor(PetNewManager::getInstance()->getPetEquipCollectEffect() * 100) / 100);
            uiBonusPetEffect->setString(GAME_TEXT("t_pet_equip_box_dmgbonus") + " x" + effectText);
        }
        else
        {
            std::string effectText = MafUtils::doubleToString(PetNewManager::getInstance()->getPetEquipCollectEffect());
            effectText = MafUtils::convertNumberToShort(effectText);
            effectText = GAME_TEXT("t_pet_equip_box_dmgbonus") + " x" + effectText;
            uiBonusPetEffect->setString(effectText);
        }
        
        //
        std::string effectText = MafUtils::doubleToString(PetNewManager::getInstance()->getPetEquipCollectEffectBongsik());
        effectText = MafUtils::convertNumberToShort(effectText);
        
        auto uiBonusCostumeEffect = uiBonus->getChildByName<ui::CText*>("UI_BONUS_COSTUME_EFFECT");
        uiBonusCostumeEffect->setString(GAME_TEXTFORMAT("t_pet_equip_effect_per_bong_1", effectText.c_str()));
    }
    
    auto uiPetEquip = layout->getChildByName<ui::Button*>("UI_PETEQUIP");
    if ( uiPetEquip != nullptr )
    {
        bool bEvent = EventManager::getInstance()->isActivatedEvent(16);
        if ( bEvent == true )
        {
            uiPetEquip->getChildByName("UI_REDDOT_EVENT")->setVisible(true);
        }
        else
        {
            uiPetEquip->getChildByName("UI_REDDOT_EVENT")->setVisible(false);
        }
    }
    
     
}

#pragma mark - ui : set, get

#pragma mark - ui : click
void LayerNormalPet::onClickPetEquip(cocos2d::Ref* sender)
{
    auto popup = PopupPetEquipBox::create();
    popup->show();
}

#pragma mark - callback

#pragma mark - schedule

#pragma mark - game
