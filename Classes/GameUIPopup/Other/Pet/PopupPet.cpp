//
//  lkjkl.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#include "PopupPet.h"

#include "Common/ConfigGameNormal.h"

#include "GameUnit/Pet.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUIPopup/Other/Pet/Layer/UILayoutPetCapability.h"
#include "GameUIPopup/Other/Pet/Layer/UILayoutPetEvolution.h"
#include "GameUIPopup/Other/Pet/Layer/UILayoutPetReinforce.h"
#include "GameUIPopup/Other/Pet/Layer/UILayoutPetEquip.h"
#include "GameUIPopup/Other/Pet/Layer/UILayoutPetSkin.h"
#include "GameUIPopup/Other/PetEquip/PopupPetEquipBox.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/UserInfoStackManager.h"
#include "ManagerGame/PetNewManager.h"

USING_NS_CC;

PopupPet* PopupPet::create(E_PET ePet)
{
    PopupPet* pRet = new(std::nothrow) PopupPet();
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

PopupPet::PopupPet(void) :
_bInitUI(false),

_ePet(E_PET::CHICK_1),
_eTab(E_TAB::CAPABILITY),

_bEvent(false)
{
    
}

PopupPet::~PopupPet(void)
{
    
}

bool PopupPet::init(E_PET ePet)
{
    if( !PopupBase::init() )
    {
        return false;
    }
    
    //
    setName("PopupPet");
    
    //
    _ePet = ePet;
    
    //
    initVar();
    initUi();
    
    //
    RefreshManager::getInstance()->addUI(E_REFRESH::PET, this);
    
    return true;
}

void PopupPet::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiInfo();
        uiTab();
        uiEquip();
    }
    
    //
    drawInfo();
    drawTab();
    drawEquip();
}

#pragma mark - refresh
void PopupPet::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::PET )
    {
        drawInfo();
    }
}
  

#pragma mark - init
void PopupPet::initVar()
{
    _listPet.push_back(E_PET::CHICK_1);
    _listPet.push_back(E_PET::HAMSTER_2);
    _listPet.push_back(E_PET::DRAGON_3);
    _listPet.push_back(E_PET::PRINCESS_4);
    _listPet.push_back(E_PET::PURIRING_5);
    _listPet.push_back(E_PET::HONG_6);
    _listPet.push_back(E_PET::MOLE_7);
    _listPet.push_back(E_PET::HANDSOME_8);
    
    _bEvent = EventManager::getInstance()->isActivatedEvent(16);
}

void PopupPet::initUi()
{
    Size size = Size(750, 1300);
    
    // size
    _spriteContainer->setContentSize( size );
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPosition(getContentSize().width / 2, 25);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_pet"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupPet::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
}

#pragma mark - ui
void PopupPet::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = 730;
    size.height = size.height - 75 - 25;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(layout);
    
    _layerContainerInfo = ui::Layout::create();
    _layerContainerInfo->setContentSize( Size(size.width, 341) );
    _layerContainerInfo->setIgnoreAnchorPointForPosition(false);
    _layerContainerInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerInfo->setPosition( Vec2(size.width / 2, size.height) );
    layout->addChild(_layerContainerInfo);
    
    _layerContainerEquip = ui::Layout::create();
    _layerContainerEquip->setContentSize( Size(size.width, 134) );
    _layerContainerEquip->setIgnoreAnchorPointForPosition(false);
    _layerContainerEquip->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerEquip->setPosition( Vec2(size.width / 2, 0) );
    layout->addChild(_layerContainerEquip);
    
    _layerContainerTab = ui::Layout::create();
    _layerContainerTab->setContentSize( Size(size.width, size.height - _layerContainerInfo->getContentSize().height - _layerContainerEquip->getContentSize().height) );
    _layerContainerTab->setIgnoreAnchorPointForPosition(false);
    _layerContainerTab->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerTab->setPosition( Vec2(size.width / 2, size.height - _layerContainerInfo->getContentSize().height) );
    layout->addChild(_layerContainerTab);
}

void PopupPet::uiInfo()
{
    auto layout = _layerContainerInfo;
    Size size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui/pet/pet_bg_1_1.png");
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiBG->setPosition(Vec2(size.width / 2, 0));
    uiBG->setScale(4.4);
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiBG->setName("UI_BG");
    layout->addChild(uiBG);
    
    //
    auto uiGuide = ui::ImageView::create("Assets/ui/pet/pet_guide_title.png");
    uiGuide->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiGuide->setPosition(Vec2(5, 197));
    ((ui::Scale9Sprite*)uiGuide->getVirtualRenderer())->setFlippedX(true);
    uiGuide->setName("UI_GUIDE");
    layout->addChild(uiGuide);
  
    //
    auto uiBtnStroy = ui::Button::create("Assets/ui/pet/btn_pet_story.png");
    uiBtnStroy->setPosition(Vec2(65, 160));
    uiBtnStroy->addClickEventListener(CC_CALLBACK_1(PopupPet::onClickStory, this));
    layout->addChild(uiBtnStroy);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_menu_store"), GAME_FONT, 20);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiText->setPosition(Vec2(uiBtnStroy->getContentSize().width / 2, 0));
        uiText->enableOutline(Color4B::BLACK, 2);
        uiBtnStroy->addChild(uiText);
    }
    
    //
    auto uiBtnPrincess = ui::Button::create("Assets/ui/mission/mission_on.png");
    uiBtnPrincess->setPosition(Vec2(65,40));
    uiBtnPrincess->addClickEventListener(CC_CALLBACK_1(PopupPet::onClickPetPrincessSwitch, this));
    uiBtnPrincess->setName("UI_PRINCESS");
    uiBtnPrincess->setScale(0.8);
    layout->addChild(uiBtnPrincess);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_evolution"), GAME_FONT, 30);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiText->setPosition(Vec2(uiBtnPrincess->getContentSize().width / 2, uiBtnPrincess->getContentSize().height + 2));
        uiText->enableOutline(Color4B::BLACK, 2);
        uiBtnPrincess->addChild(uiText);
    }
    
    // pet list layout
    auto uiList = ui::Layout::create();
    uiList->setContentSize(Size(size.width, 113));
    uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiList->setPosition(Vec2(size.width / 2, 242));
    uiList->setName("UI_LIST");
    layout->addChild(uiList);
}

void PopupPet::uiTab()
{
    auto layout = _layerContainerTab;
    Size size = layout->getContentSize();
    
    // button
    auto uiTab = ui::Layout::create();
    uiTab->setContentSize(Size(size.width, 64));
    uiTab->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiTab->setPosition(Vec2(size.width / 2, size.height));
    uiTab->setName("UI_TAB");
    layout->addChild(uiTab);
    
    // content
    auto uiContent = ui::Layout::create();
    uiContent->setContentSize(Size(size.width, size.height - uiTab->getContentSize().height));
    uiContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiContent->setPosition(Vec2(size.width / 2, 0));
    uiContent->setName("UI_CONTENT");
    layout->addChild(uiContent);
}

void PopupPet::uiEquip()
{
    auto layout = _layerContainerEquip;
    Size size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui/pet/pet_equipment_bar_1.png");
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(uiBG);
    {
        auto uiTitle = ui::CText::create(GAME_TEXT("t_pet_equip_box_title_1"), GAME_FONT, 24);
        uiTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiTitle->setPosition(Vec2(140, 101 - 17.5));
        uiTitle->setTextAreaSize(Size(300, uiTitle->getContentSize().height));
        uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiTitle->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiTitle->setTextOverflow(Label::Overflow::SHRINK);
        uiBG->addChild(uiTitle);
        
        auto uiDesc = ui::CText::create(GAME_TEXT("t_pet_equip_box_desc_1"), GAME_FONT, 22);
        uiDesc->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiDesc->setPosition(Vec2(140, 66 - 25.5));
        uiDesc->setTextAreaSize(Size(300, 51));
        uiDesc->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiDesc->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiDesc->setTextOverflow(Label::Overflow::SHRINK);
        uiDesc->setTextColor(Color4B::GRAY);
        uiBG->addChild(uiDesc);
        
        //
        auto uiPetEquip = ui::Button::create("Assets/ui/pet/btn_pet_equipment_1.png");
        uiPetEquip->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiPetEquip->setPosition(Vec2(uiBG->getContentSize().width - 25, 60.5));
        uiPetEquip->addClickEventListener(CC_CALLBACK_1(PopupPet::onClickPetEquip, this));
        uiPetEquip->setName("UI_PETEQUIP");
        uiBG->addChild(uiPetEquip);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_pet_equip_box_title_1"), GAME_FONT, 28);
            uiText->setPosition(Vec2(uiPetEquip->getContentSize().width / 2, uiPetEquip->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiPetEquip->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiPetEquip->addChild(uiText, 10);
            
            auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE_RIGHT);
            uiReddot->setPosition(uiPetEquip->getPosition() + Vec2::UP * (uiPetEquip->getContentSize().height/2 - 15));
            uiReddot->setVisible(false);
            uiPetEquip->addChild(uiReddot);
            
            RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_BOX, uiReddot);
        }
    }
}


#pragma mark - ui
void PopupPet::drawInfo()
{
    auto layout = _layerContainerInfo;
    Size size = layout->getContentSize();
    
    //
    auto uiBG = layout->getChildByName<ui::ImageView*>("UI_BG");
    if ( uiBG != nullptr )
    {
        std::string pathBG = "Assets/ui/pet/pet_bg_1_1.png";
        switch (_ePet) {
            case E_PET::CHICK_1:            pathBG = "Assets/ui/pet/pet_bg_1_1.png";     break;
            case E_PET::HAMSTER_2:          pathBG = "Assets/ui/pet/pet_bg_1_1.png";     break;
            case E_PET::DRAGON_3:           pathBG = "Assets/ui/pet/pet_bg_1_1.png";     break;
            case E_PET::PRINCESS_4:         pathBG = "Assets/ui/pet/pet_bg_1_1.png";     break;
            case E_PET::PURIRING_5:         pathBG = "Assets/ui/pet/pet_bg_1_2.png";     break;
            case E_PET::HONG_6:             pathBG = "Assets/ui/pet/pet_bg_1_3.png";     break;
            case E_PET::MOLE_7:             pathBG = "Assets/ui/pet/pet_bg_1_4.png";     break;
            case E_PET::HANDSOME_8:         pathBG = "Assets/ui/pet/pet_bg_1_5.png";     break;
            case E_PET::D_1_PENGUIN:        pathBG = "Assets/ui/pet/pet_bg_1_1.png";     break;
            case E_PET::D_2_HEDGEHOG:       pathBG = "Assets/ui/pet/pet_bg_1_1.png";     break;
            case E_PET::D_3_GATEKEEPER:     pathBG = "Assets/ui/pet/pet_bg_1_1.png";     break;
            case E_PET::D_4_FLASH:          pathBG = "Assets/ui/pet/pet_bg_1_1.png";     break;
            default:
                break;
        }
        
        uiBG->loadTexture(pathBG);
        ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    }
    
    //
    auto uiGuide = layout->getChildByName<ui::ImageView*>("UI_GUIDE");
    if ( uiGuide != nullptr )
    {
        uiGuide->removeAllChildren();
        
        std::string strText = "";
        switch (_ePet) {
            case E_PET::CHICK_1:            strText = GAME_TEXT("t_pet_d1_detail_chick_1");             break;
            case E_PET::HAMSTER_2:          strText = GAME_TEXT("t_pet_d1_detail_hamster_1");           break;
            case E_PET::DRAGON_3:           strText = GAME_TEXT("t_pet_d1_detail_dragon_1");            break;
            case E_PET::PRINCESS_4:         strText = GAME_TEXT("t_pet_d1_detail_princess_1");          break;
            case E_PET::PURIRING_5:         strText = GAME_TEXT("t_pet_d1_detail_puriring_1");          break;
            case E_PET::HONG_6:             strText = GAME_TEXT("t_pet_d1_detail_hong_1");              break;
            case E_PET::MOLE_7:             strText = GAME_TEXT("t_pet_d1_detail_mole_1");              break;
            case E_PET::HANDSOME_8:         strText = GAME_TEXT("t_pet_d1_detail_mole_handsome_1");     break;
            case E_PET::D_1_PENGUIN:        strText = GAME_TEXT("t_pet_d1_detail_penguin_1");           break;
            case E_PET::D_2_HEDGEHOG:       strText = GAME_TEXT("t_pet_d1_detail_hedgehog_1");          break;
            case E_PET::D_3_GATEKEEPER:     strText = GAME_TEXT("t_pet_d1_detail_gatekeeper_1");        break;
            case E_PET::D_4_FLASH:          strText = GAME_TEXT("t_pet_d1_detail_flash_1");             break;
            default:
                break;
        }
        
        auto uiText = ui::CText::create(strText, GAME_FONT, 20);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiText->setPosition(Vec2(29, uiGuide->getContentSize().height / 2));
        uiText->setTextColor(Color4B(250, 206, 0, 255));
        uiText->setTextAreaSize(Size(300, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiGuide->addChild(uiText);
    }
    
    auto uiBtnPrincess = layout->getChildByName<ui::Button*>("UI_PRINCESS");
    if ( uiBtnPrincess != nullptr )
    {
        uiBtnPrincess->setVisible(false);
        if ( _ePet == E_PET::PRINCESS_4 && PetNewManager::getInstance()->getPetEvolution(_ePet) >= 1 )
        {
            uiBtnPrincess->setVisible(true);
            if ( UserInfoStackManager::getInstance()->getPrincessEvolutionActive(_ePlace) == false )
            {
                uiBtnPrincess->loadTextureNormal("Assets/ui/mission/mission_off.png");
            }
            else
            {
                uiBtnPrincess->loadTextureNormal("Assets/ui/mission/mission_on.png");
            }
                
        }
    }
    
    
    //
    auto uiList = layout->getChildByName<ui::Layout*>("UI_LIST");
    if ( uiList != nullptr )
    {
        uiList->removeAllChildren();
        
        size_t cellCount = _listPet.size();
        double cellPadding = 5;
        double cellWidth = (uiList->getContentSize().width - cellPadding * (cellCount - 1)) / cellCount;
        
        for ( int i = 0; i < cellCount; i++ )
        {
            E_PET ePet = _listPet.at(i);
            
            double posX = cellWidth * i + cellPadding * i;
            double posY = 0;
            
            std::string pathCell = "Assets/ui/mission/achievements_icon_btn_off.png";
            if ( ePet == _ePet )
            {
                pathCell = "Assets/ui/mission/achievements_icon_btn_on.png";
            }
            auto uiBtnCell = ui::Button::create(pathCell);
            uiBtnCell->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            uiBtnCell->setPosition(Vec2(posX, posY));
            uiBtnCell->setScale(cellWidth / uiBtnCell->getContentSize().width);
            uiBtnCell->setTag(ePet);
            uiBtnCell->addClickEventListener(CC_CALLBACK_1(PopupPet::onClickPet, this));
            uiList->addChild(uiBtnCell);
            
            auto uiIcon = ui::ImageView::create(MafUtils::format("Assets/icon_pet/skin_%d_%d.png", ePet, 0));
            uiIcon->setPosition(Vec2(uiBtnCell->getContentSize().width / 2, uiBtnCell->getContentSize().height / 2));
            uiIcon->setScale(1.7);
            uiBtnCell->addChild(uiIcon);
            
            //
            auto uiReddot1 = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
            uiReddot1->setPosition(uiBtnCell->getContentSize().width - 12, uiBtnCell->getContentSize().height - 12);
            uiReddot1->setVisible(false);
            uiBtnCell->addChild(uiReddot1);
            {
                if ( ePet == E_PET::CHICK_1 )               RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_CHICK, uiReddot1);
                else if ( ePet == E_PET::HAMSTER_2 )        RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_HAMSTER, uiReddot1);
                else if ( ePet == E_PET::DRAGON_3 )         RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_DRAGON, uiReddot1);
                else if ( ePet == E_PET::PRINCESS_4 )       RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_PRINCESS, uiReddot1);
                else if ( ePet == E_PET::PURIRING_5 )       RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_PURIRING, uiReddot1);
                else if ( ePet == E_PET::HONG_6 )           RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_HONG, uiReddot1);
                else if ( ePet == E_PET::MOLE_7 )           RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_MOLE, uiReddot1);
                else if ( ePet == E_PET::HANDSOME_8 )       RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_HANDSOME, uiReddot1);
            }
            
            auto uiReddot2 = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
            uiReddot2->setPosition(uiBtnCell->getContentSize().width - 12, uiBtnCell->getContentSize().height - 12);
            uiReddot2->setVisible(false);
            uiBtnCell->addChild(uiReddot2);
            {
                if ( ePet == E_PET::CHICK_1 )               RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_CHICK, uiReddot2);
                else if ( ePet == E_PET::HAMSTER_2 )        RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_HAMSTER, uiReddot2);
                else if ( ePet == E_PET::DRAGON_3 )         RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_DRAGON, uiReddot2);
                else if ( ePet == E_PET::PRINCESS_4 )       RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_PRINCESS, uiReddot2);
                else if ( ePet == E_PET::PURIRING_5 )       RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_PURIRING, uiReddot2);
                else if ( ePet == E_PET::HONG_6 )           RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_HONG, uiReddot2);
                else if ( ePet == E_PET::MOLE_7 )           RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_MOLE, uiReddot2);
                else if ( ePet == E_PET::HANDSOME_8 )       RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_HANDSOME, uiReddot2);
            }
            
            auto uiReddot3 = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
            uiReddot3->setPosition(uiBtnCell->getContentSize().width - 12, uiBtnCell->getContentSize().height - 12);
            uiReddot3->setVisible(false);
            uiBtnCell->addChild(uiReddot3);
            {
                if ( ePet == E_PET::CHICK_1 )               RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_CHICK, uiReddot3);
                else if ( ePet == E_PET::HAMSTER_2 )        RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_HAMSTER, uiReddot3);
                else if ( ePet == E_PET::DRAGON_3 )         RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_DRAGON, uiReddot3);
                else if ( ePet == E_PET::PRINCESS_4 )       RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_PRINCESS, uiReddot3);
                else if ( ePet == E_PET::PURIRING_5 )       RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_PURIRING, uiReddot3);
                else if ( ePet == E_PET::HONG_6 )           RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_HONG, uiReddot3);
                else if ( ePet == E_PET::MOLE_7 )           RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_MOLE, uiReddot3);
                else if ( ePet == E_PET::HANDSOME_8 )       RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_HANDSOME, uiReddot3);
            }
        }
    }
    
    //
    layout->removeChildByName("UI_PET");
    {
        Pet* unitPet = nullptr;
        
        double posPetX = 0;
        double posPetY = 0;
        switch (_ePet) {
            case E_PET::CHICK_1:
                unitPet = Chick::create();
                unitPet->setScale(1.2);
                posPetY = -220;
                if ( PetNewManager::getInstance()->getPetEvolution(_ePet) || PetNewManager::getInstance()->getPetSkin(_ePet) > 0 )
                {
                    posPetY = -190;
                }
                break;
            case E_PET::HAMSTER_2:
                unitPet = Hamster::create();
                unitPet->setScale(1.2);
                posPetX = 0;
                posPetY = -190;
                if ( PetNewManager::getInstance()->getPetEvolution(_ePet) || PetNewManager::getInstance()->getPetSkin(_ePet) > 0 )
                {
                    posPetX = 0;
                    posPetY = -115;
                }
                break;
            case E_PET::DRAGON_3:
                unitPet = Dragon::create();
                unitPet->setScale(1.2);
                posPetX = 0;
                posPetY = -155;
                if ( PetNewManager::getInstance()->getPetEvolution(_ePet) || PetNewManager::getInstance()->getPetSkin(_ePet) > 0 )
                {
                    posPetX = 0;
                    posPetY = -150;
                }
                break;
            case E_PET::PRINCESS_4:
                unitPet = Princess::create();
                unitPet->setScale(1.2);
                posPetX = -55;
                posPetY = -115;
                if ( PetNewManager::getInstance()->getPetEvolution(_ePet) || PetNewManager::getInstance()->getPetSkin(_ePet) > 0 )
                {
                    posPetX = 0;
                    posPetY = -140;
                }
                break;
            case E_PET::PURIRING_5:
                unitPet = Puriring::create();
                unitPet->setScale(1.2);
                posPetX = 0;
                posPetY = -130;
                if ( PetNewManager::getInstance()->getPetEvolution(_ePet) || PetNewManager::getInstance()->getPetSkin(_ePet) > 0 )
                {
                    posPetX = 0;
                    posPetY = -110;
                }
                break;
            case E_PET::HONG_6:
                unitPet = Hong::create();
                unitPet->setScale(1.2);
                posPetX = 5;
                posPetY = -135;
                if ( PetNewManager::getInstance()->getPetEvolution(_ePet) || PetNewManager::getInstance()->getPetSkin(_ePet) > 0 )
                {
                    posPetX = 0;
                    posPetY = -140;
                }
                break;
            case E_PET::MOLE_7:
                unitPet = Mole::create();
                unitPet->setScale(1.2);
                posPetX = 0;
                posPetY = -185;
                if ( PetNewManager::getInstance()->getPetEvolution(_ePet) || PetNewManager::getInstance()->getPetSkin(_ePet) > 0 )
                {
                    posPetX = 10;
                    posPetY = -180;
                }
                break;
            case E_PET::HANDSOME_8:
                unitPet = Handsome::create();
                unitPet->setScale(1.2);
                posPetX = 0;
                posPetY = -135;
                if ( PetNewManager::getInstance()->getPetEvolution(_ePet) || PetNewManager::getInstance()->getPetSkin(_ePet) > 0 )
                {
                    posPetX = 10;
                    posPetY = -120;
                }
                break;
                
            default:
                break;
        }
        unitPet->setFlippedX(true);
        unitPet->setPlace(E_PLACE::DG_NORMAL);
        unitPet->setPosition(size.width - 120 + posPetX, 240 + posPetY);
        unitPet->setEvolutionDummy(PetNewManager::getInstance()->getPetSkin(_ePet));
        unitPet->setVisible(false);
        unitPet->setName("UI_PET");
        layout->addChild(unitPet);
        
        //
        auto action1 = DelayTime::create(0.1);
        auto action2 = Show::create();
        auto seq = Sequence::create(action1, action2, NULL);
        unitPet->runAction(seq);
    }
    
}

void PopupPet::drawTab()
{
    auto layout = _layerContainerTab;
    Size size = layout->getContentSize();
    
    // button
    auto uiTab = layout->getChildByName<ui::Layout*>("UI_TAB");
    if ( uiTab != nullptr )
    {
        uiTab->removeAllChildren();
        
        //
        std::vector<E_TAB> listTab = {E_TAB::CAPABILITY, E_TAB::EVOLUTION, E_TAB::REINFORCE, E_TAB::EQUIP, E_TAB::SKIN};
        std::vector<std::string> listTabTextKey = {"t_ui_capability", "t_ui_evolution", "t_ui_upgrade", "t_ui_equip", "t_ui_skin"};
 
        size_t cellCount = listTab.size();
        double cellPadding = 0;
        double cellWidth = (uiTab->getContentSize().width - cellPadding * (cellCount - 1)) / cellCount;
        double cellHeight = 64;
        
        for ( int i = 0; i < cellCount; i++ )
        {
            E_TAB eTab = listTab.at(i);
            
            double posX = cellWidth * i + cellPadding * i;
            double posY = 0;
            
            std::string pathCell = "Assets/ui_common/btn_tap_off.png";
            if ( eTab == _eTab )
            {
                pathCell = "Assets/ui_common/btn_tap_on.png";
            }
            auto uiBtnCell = ui::Button::create(pathCell);
            uiBtnCell->setScale9Enabled(true);
            uiBtnCell->setContentSize(Size(cellWidth, cellHeight));
            uiBtnCell->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            uiBtnCell->setPosition(Vec2(posX, posY));
            uiBtnCell->setScale(cellWidth / uiBtnCell->getContentSize().width);
            uiBtnCell->setTag(enumToInt(eTab));
            uiBtnCell->addClickEventListener(CC_CALLBACK_1(PopupPet::onClickTab, this));
            uiTab->addChild(uiBtnCell);
            {
                auto uiText = ui::CText::create(GAME_TEXT(listTabTextKey.at(i)), GAME_FONT, 24);
                uiText->setPosition(Vec2(uiBtnCell->getContentSize().width / 2, uiBtnCell->getContentSize().height / 2));
                uiText->setTextColor(Color4B(86, 86, 86, 255));
                uiText->setTextAreaSize(Size(uiBtnCell->getContentSize().width - 10, uiText->getContentSize().height));
                uiText->setTextOverflow(Label::Overflow::SHRINK);
                uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
                uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
                uiBtnCell->addChild(uiText);
                if ( eTab == _eTab )
                {
                    uiText->setTextColor(Color4B(255, 255, 255, 255));
                }
            }
            
            if ( E_TAB::EVOLUTION == eTab )
            {
                auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
                uiReddot->setPosition(0, uiBtnCell->getContentSize().height);
                uiReddot->setVisible(false);
                uiBtnCell->addChild(uiReddot);
                {
                    if ( _ePet == E_PET::CHICK_1 )          RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_CHICK, uiReddot);
                    else if ( _ePet == E_PET::HAMSTER_2 )   RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_HAMSTER, uiReddot);
                    else if ( _ePet == E_PET::DRAGON_3 )    RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_DRAGON, uiReddot);
                    else if ( _ePet == E_PET::PRINCESS_4 )  RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_PRINCESS, uiReddot);
                    else if ( _ePet == E_PET::PURIRING_5 )  RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_PURIRING, uiReddot);
                    else if ( _ePet == E_PET::HONG_6 )      RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_HONG, uiReddot);
                    else if ( _ePet == E_PET::MOLE_7 )      RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_MOLE, uiReddot);
                    else if ( _ePet == E_PET::HANDSOME_8 )  RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_HANDSOME, uiReddot);
                }
            }
            else if ( E_TAB::REINFORCE == eTab )
            {
                auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
                uiReddot->setPosition(0, uiBtnCell->getContentSize().height);
                uiReddot->setVisible(false);
                uiBtnCell->addChild(uiReddot);
                {
                    if ( _ePet == E_PET::CHICK_1 )          RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_CHICK, uiReddot);
                    else if ( _ePet == E_PET::HAMSTER_2 )   RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_HAMSTER, uiReddot);
                    else if ( _ePet == E_PET::DRAGON_3 )    RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_DRAGON, uiReddot);
                    else if ( _ePet == E_PET::PRINCESS_4 )  RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_PRINCESS, uiReddot);
                    else if ( _ePet == E_PET::PURIRING_5 )  RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_PURIRING, uiReddot);
                    else if ( _ePet == E_PET::HONG_6 )      RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_HONG, uiReddot);
                    else if ( _ePet == E_PET::MOLE_7 )      RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_MOLE, uiReddot);
                    else if ( _ePet == E_PET::HANDSOME_8 )  RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_HANDSOME, uiReddot);
                }
            }
            else if ( E_TAB::EQUIP == eTab )
            {
                auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
                uiReddot->setPosition(0, uiBtnCell->getContentSize().height);
                uiReddot->setVisible(false);
                uiBtnCell->addChild(uiReddot);
                {
                    if ( _ePet == E_PET::CHICK_1 )          RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_CHICK, uiReddot);
                    else if ( _ePet == E_PET::HAMSTER_2 )   RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_HAMSTER, uiReddot);
                    else if ( _ePet == E_PET::DRAGON_3 )    RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_DRAGON, uiReddot);
                    else if ( _ePet == E_PET::PRINCESS_4 )  RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_PRINCESS, uiReddot);
                    else if ( _ePet == E_PET::PURIRING_5 )  RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_PURIRING, uiReddot);
                    else if ( _ePet == E_PET::HONG_6 )      RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_HONG, uiReddot);
                    else if ( _ePet == E_PET::MOLE_7 )      RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_MOLE, uiReddot);
                    else if ( _ePet == E_PET::HANDSOME_8 )  RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_HANDSOME, uiReddot);
                }
            }
        }
    }
    
    
    auto uiContent = layout->getChildByName<ui::Layout*>("UI_CONTENT");
    if ( uiContent != nullptr )
    {
        uiContent->removeAllChildren();
        
        ui::Layout* uiContentChild = nullptr;
        switch (_eTab) {
            case E_TAB::CAPABILITY:     uiContentChild = UILayoutPetCapability::create(_ePet); break;
            case E_TAB::EVOLUTION:      uiContentChild = UILayoutPetEvolution::create(_ePet); break;
            case E_TAB::REINFORCE:      uiContentChild = UILayoutPetReinforce::create(_ePet); break;
            case E_TAB::EQUIP:          uiContentChild = UILayoutPetEquip::create(_ePet); break;
            case E_TAB::SKIN:           uiContentChild = UILayoutPetSkin::create(_ePet); break;
                
            default:
                break;
        }
        
        if ( uiContentChild != nullptr )
        {
            uiContentChild->setContentSize(Size(uiContent->getContentSize()));
            uiContent->addChild(uiContentChild);
        }
    }
}

void PopupPet::drawEquip()
{
    auto layout = _layerContainerEquip;
    Size size = layout->getContentSize();
    
    auto uiPetEquip = (ui::Button*)utils::findChild(layout, "UI_PETEQUIP");
    if ( uiPetEquip != nullptr )
    {
        uiPetEquip->removeChildByName("UI_EVENT");
        
        //
        if ( _bEvent == true )
        {
            uiPetEquip->loadTextureNormal("Assets/ui/pet/event_btn_pet1_1.png");
            
            //
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.1);
            for ( int i = 1; i <= 5; i++ )
            {
                auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/pet/event1_%d.png",i).c_str());
                spriteAni->getTexture()->setTexParameters(texParams);
                animation->addSpriteFrame(spriteAni->getSpriteFrame());
            }
            
            auto uiAni = ui::ImageView::create("Assets/ui/pet/event1_1.png");
            uiAni->setPosition(Vec2(uiPetEquip->getContentSize().width / 2, uiPetEquip->getContentSize().height));
            uiAni->getVirtualRenderer()->stopAllActions();
            uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
            uiAni->setName("UI_EVENT");
            uiPetEquip->addChild(uiAni, 1000);
        }
        else
        {
            uiPetEquip->loadTextureNormal("Assets/ui/pet/btn_pet_equipment_1.png");
        }
    }
}

#pragma mark - set, get, other

#pragma mark - click
void PopupPet::onClickClose(Ref* sender)
{
    hide();
}

void PopupPet::onClickStory(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    //
    Vec2 pos = uiBtn->getParent()->convertToWorldSpace(uiBtn->getPosition());
    pos.x -= uiBtn->getContentSize().width / 2;
    pos.y += uiBtn->getContentSize().height / 2;
    pos = this->convertToNodeSpace(pos);
    
    auto uiPanel = ui::ImageView::create("Assets/ui/pet/pet_guide_3.png");
    uiPanel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiPanel->setPosition(pos);
    this->addChild(uiPanel, 99);
    {
        std::string strText = "";
        switch (_ePet) {
            case E_PET::CHICK_1:        strText = GAME_TEXT("t_pet_d1_detail_chick_2");         break;
            case E_PET::HAMSTER_2:      strText = GAME_TEXT("t_pet_d1_detail_hamster_2");       break;
            case E_PET::DRAGON_3:       strText = GAME_TEXT("t_pet_d1_detail_dragon_2");        break;
            case E_PET::PRINCESS_4:     strText = GAME_TEXT("t_pet_d1_detail_princess_2");      break;
            case E_PET::PURIRING_5:     strText = GAME_TEXT("t_pet_d1_detail_puriring_2");      break;
            case E_PET::HONG_6:         strText = GAME_TEXT("t_pet_d1_detail_hong_2");          break;
            case E_PET::MOLE_7:         strText = GAME_TEXT("t_pet_d1_detail_mole_2");          break;
            case E_PET::HANDSOME_8:     strText = GAME_TEXT("t_pet_d1_detail_mole_handsome_2"); break;
            default:
                break;
        }
        
        auto uiText = ui::CText::create(strText, GAME_FONT, 20);
        uiText->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiText->setPosition(Vec2(20, uiPanel->getContentSize().height - 10));
        uiText->setTextAreaSize(Size(340, 130));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::TOP);
        uiText->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiText->setColor(Color3B(73, 44, 20));
        uiPanel->addChild(uiText);
    }
    
    //
    pos.y -= uiPanel->getContentSize().height;
    pos *= -1;
    
    auto uiTouchHide = ui::Layout::create();
    uiTouchHide->setContentSize(getContentSize());
    uiTouchHide->setAnchorPoint(Vec2::ZERO);
    uiTouchHide->setPosition(pos);
    uiTouchHide->setTouchEnabled(true);
    uiTouchHide->addTouchEventListener([=](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        auto uiTouchHide = (ui::Layout*)sender;
        
        if ( type == ui::Widget::TouchEventType::BEGAN )
        {
            auto uiPanel = uiTouchHide->getParent();
            uiPanel->removeFromParent();
        }
    });
    uiPanel->addChild(uiTouchHide, -1);
}

void PopupPet::onClickPet(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    E_PET ePet = (E_PET)uiBtn->getTag();
    if ( ePet == _ePet )
    {
        return;
    }
    
    _ePet = ePet;
    drawInfo();
    drawTab();
    drawEquip();
}

void PopupPet::onClickPetEquip(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    bool bEvent = EventManager::getInstance()->isActivatedEvent(16);
    if ( _bEvent != bEvent )
    {
        _bEvent = bEvent;
        
        drawInfo();
        drawTab();
        drawEquip();
        return;
    }
    
    auto popup = PopupPetEquipBox::create();
    popup->show();
}

void PopupPet::onClickPetPrincessSwitch(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    bool bActive = !UserInfoStackManager::getInstance()->getPrincessEvolutionActive(_ePlace);
    UserInfoStackManager::getInstance()->setPrincessEvolutionActive(_ePlace, bActive);
    
    //
    if ( bActive)
    {
        uiBtn->loadTextureNormal("Assets/ui/mission/mission_on.png");
    }
    else
    {
        uiBtn->loadTextureNormal("Assets/ui/mission/mission_off.png");
    }
}

void PopupPet::onClickTab(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    E_TAB eTab = enumFromInt<E_TAB>(uiBtn->getTag());
    if ( eTab == _eTab )
    {
        return;
    }
    
    _eTab = eTab;
    drawTab();
}
