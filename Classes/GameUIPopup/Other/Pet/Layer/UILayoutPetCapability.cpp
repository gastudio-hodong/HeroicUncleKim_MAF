//
//  UILayoutPetCapability.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "UILayoutPetCapability.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UICell/UILayoutPetCell.h"

#include "GameUIPopup/Other/Equip/PopupWeaponPermanent.h"

#include "ManagerGame/PetNewManager.h"

USING_NS_CC;

UILayoutPetCapability* UILayoutPetCapability::create(E_PET ePet)
{
    UILayoutPetCapability* pRet = new(std::nothrow) UILayoutPetCapability();
    if ( pRet && pRet->init(ePet) )
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

UILayoutPetCapability::UILayoutPetCapability(void) :
_bInitUI(false),

_ePlace(E_PLACE::DG_NORMAL),
_ePet(E_PET::CHICK_1)
{
    
}

UILayoutPetCapability::~UILayoutPetCapability(void)
{
    
}

bool UILayoutPetCapability::init(E_PET ePet)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    _ePet = ePet;
    
    // init
    initVar();
    initUi();
    
    return true;
}

void UILayoutPetCapability::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiLevelUP();
        uiInfo();
        uiInfoOther();
    }
    
    // draw
    drawLevelUP();
    drawInfo();
    drawInfoOther();
}

#pragma mark - init
void UILayoutPetCapability::initVar()
{
    
}

void UILayoutPetCapability::initUi()
{
    
}



#pragma mark - ui
void UILayoutPetCapability::uiContainer()
{
    Size size = this->getContentSize();
    
    _uiContentsUILevelUP = ui::Layout::create();
    _uiContentsUILevelUP->setContentSize(Size(size.width, 135));
    _uiContentsUILevelUP->setIgnoreAnchorPointForPosition(false);
    _uiContentsUILevelUP->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContentsUILevelUP->setPosition(Vec2(size.width / 2, size.height));
    addChild(_uiContentsUILevelUP);
    
    _uiContentsUIInfo = ui::Layout::create();
    _uiContentsUIInfo->setContentSize(Size(size.width, 216));
    _uiContentsUIInfo->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContentsUIInfo->setPosition(Vec2(size.width / 2, size.height - _uiContentsUILevelUP->getContentSize().height));
    addChild(_uiContentsUIInfo);
    
    _uiContentsUIInfoOther = ui::Layout::create();
    _uiContentsUIInfoOther->setContentSize(Size(size.width, size.height - _uiContentsUILevelUP->getContentSize().height - _uiContentsUIInfo->getContentSize().height));
    _uiContentsUIInfoOther->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIInfoOther->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsUIInfoOther->setPosition(Vec2(size.width / 2, 0));
    addChild(_uiContentsUIInfoOther);
}

void UILayoutPetCapability::uiLevelUP()
{
    auto layout = _uiContentsUILevelUP;
    Size size = layout->getContentSize();
    
    auto uiCell = UILayoutPetCell::create(_ePet);
    uiCell->setContentSize(size);
    uiCell->setPosition(Vec2(size.width / 2, size.height / 2));
    uiCell->setDetailPopup(false);
    layout->addChild(uiCell);
}

void UILayoutPetCapability::uiInfo()
{
    auto layout = _uiContentsUIInfo;
    Size size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/box.png");
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(size);
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(uiBG);
    
    auto uiInfo = ui::CText::create("", GAME_FONT, 22);
    uiInfo->setPosition(Vec2(size.width / 2, size.height / 2));
    uiInfo->setTextAreaSize(Size(size.width - 50, size.height - 50));
    uiInfo->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiInfo->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiInfo->setTextOverflow(Label::Overflow::SHRINK);
    uiInfo->setName("UI_INFO");
    layout->addChild(uiInfo);
}

void UILayoutPetCapability::uiInfoOther()
{
    auto layout = _uiContentsUIInfoOther;
    Size size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/base_box_1_1.png");
    uiBG->setScale9Enabled(true);
    uiBG->setCapInsets(Rect(0,0,20,20));
    uiBG->setContentSize(size);
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(uiBG);
    
    auto uiEvolutionBG = ui::ImageView::create("Assets/ui/pet/pet_guide_2.png");
    uiEvolutionBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiEvolutionBG->setPosition(Vec2(0, size.height - 10));
    uiEvolutionBG->setName("UI_EVOLUTION_BG");
    layout->addChild(uiEvolutionBG);
  
    auto uiReinforceBG = ui::ImageView::create("Assets/ui/pet/pet_guide_2.png");
    uiReinforceBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiReinforceBG->setPosition(Vec2(0, size.height - 110));
    uiReinforceBG->setName("UI_REINFORCE_BG");
    layout->addChild(uiReinforceBG);
    
}

#pragma mark - draw
void UILayoutPetCapability::drawLevelUP()
{
    
}

void UILayoutPetCapability::drawInfo()
{
    auto layout = _uiContentsUIInfo;
    Size size = layout->getContentSize();
    
    std::string infoText = "";
    switch (_ePet) {
        case E_PET::CHICK_1:            infoText = GAME_TEXT("t_pet_d1_detail_chick_3");             break;
        case E_PET::HAMSTER_2:          infoText = GAME_TEXT("t_pet_d1_detail_hamster_3");           break;
        case E_PET::DRAGON_3:           infoText = GAME_TEXT("t_pet_d1_detail_dragon_3");            break;
        case E_PET::PRINCESS_4:         infoText = GAME_TEXT("t_pet_d1_detail_princess_3");          break;
        case E_PET::PURIRING_5:         infoText = GAME_TEXT("t_pet_d1_detail_puriring_3");          break;
        case E_PET::HONG_6:             infoText = GAME_TEXT("t_pet_d1_detail_hong_3");              break;
        case E_PET::MOLE_7:             infoText = GAME_TEXT("t_pet_d1_detail_mole_3");              break;
        case E_PET::HANDSOME_8:         infoText = GAME_TEXT("t_pet_d1_detail_mole_handsome_3");     break;
        case E_PET::D_1_PENGUIN:        infoText = GAME_TEXT("t_pet_d1_detail_penguin_3");           break;
        case E_PET::D_2_HEDGEHOG:       infoText = GAME_TEXT("t_pet_d1_detail_hedgehog_3");          break;
        case E_PET::D_3_GATEKEEPER:     infoText = GAME_TEXT("t_pet_d1_detail_gatekeeper_3");        break;
        case E_PET::D_4_FLASH:          infoText = GAME_TEXT("t_pet_d1_detail_flash_3");             break;
        default:
            break;
    }
    
    auto uiInfo = layout->getChildByName<ui::CText*>("UI_INFO");
    if ( uiInfo != nullptr )
    {
        uiInfo->setString(infoText);
    }
}

void UILayoutPetCapability::drawInfoOther()
{
    auto layout = _uiContentsUIInfoOther;
    Size size = layout->getContentSize();
    
    auto uiEvolutionBG = layout->getChildByName<ui::ImageView*>("UI_EVOLUTION_BG");
    if ( uiEvolutionBG != nullptr )
    {
        uiEvolutionBG->removeAllChildren();
        
        int level = PetNewManager::getInstance()->getPetEvolution(_ePet);
        
        std::string titleText = GAME_TEXTFORMAT("t_ui_pet_msg_5", level);
        if ( level == 1 )          titleText = GAME_TEXTFORMAT("t_ui_pet_msg_5_1", level);
        else if ( level == 2 )     titleText = GAME_TEXTFORMAT("t_ui_pet_msg_5_2", level);
        else if ( level == 3 )     titleText = GAME_TEXTFORMAT("t_ui_pet_msg_5_3", level);
        
        auto uiTitle = ui::CText::create(titleText, GAME_FONT, 20);
        uiTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiTitle->setPosition(Vec2(5, uiEvolutionBG->getContentSize().height / 2));
        uiEvolutionBG->addChild(uiTitle);
        
        std::string infoText = GAME_TEXTFORMAT("t_ui_pet_msg_6", PetNewManager::getInstance()->getPetEvolutionFloor(_ePet, level-1) - 1, PetNewManager::getInstance()->getPetEvolutionExplain(_ePet, level).c_str());
        if (  level == 0 )         infoText = GAME_TEXT("t_pet_d1_none");
        
        auto uiInfo = ui::CText::create(infoText, GAME_FONT, 20);
        uiInfo->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiInfo->setPosition(Vec2(10, 0));
        uiInfo->setTextAreaSize(Size(600, uiInfo->getContentSize().height));
        uiInfo->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiInfo->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiInfo->setTextOverflow(Label::Overflow::SHRINK);
        uiEvolutionBG->addChild(uiInfo);
    }
    
    auto uiReinforceBG = layout->getChildByName<ui::ImageView*>("UI_REINFORCE_BG");
    if ( uiReinforceBG != nullptr )
    {
        uiReinforceBG->removeAllChildren();
        
        int level = PetNewManager::getInstance()->getPetReinforce(_ePet);
        
        std::string titleText = GAME_TEXTFORMAT("t_ui_pet_msg_7", level);
        if ( level == 1 )          titleText = GAME_TEXTFORMAT("t_ui_pet_msg_7_1", level);
        else if ( level == 2 )     titleText = GAME_TEXTFORMAT("t_ui_pet_msg_7_2", level);
        else if ( level == 3 )     titleText = GAME_TEXTFORMAT("t_ui_pet_msg_7_3", level);
        
        auto uiTitle = ui::CText::create(titleText, GAME_FONT, 20);
        uiTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiTitle->setPosition(Vec2(5, uiReinforceBG->getContentSize().height / 2));
        uiReinforceBG->addChild(uiTitle);
        
        std::string infoText = MafUtils::format("%s", PetNewManager::getInstance()->getPetReinforceExplain(_ePet, level).c_str());
        
        auto uiInfo = ui::CText::create(infoText, GAME_FONT, 20);
        uiInfo->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiInfo->setPosition(Vec2(10, 0));
        uiInfo->setTextAreaSize(Size(600, uiInfo->getContentSize().height));
        uiInfo->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiInfo->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiInfo->setTextOverflow(Label::Overflow::SHRINK);
        uiReinforceBG->addChild(uiInfo);
    }
}
 
#pragma mark - set, get, add


#pragma mark - callback
 

#pragma mark - click
