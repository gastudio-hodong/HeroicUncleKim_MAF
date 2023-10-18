//
//  PopupPetEquipStatus.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 31/07/2019.
//


#include "PopupPetEquipStatus.h"

#include "GameObject/InfoPetEquip.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/PetNewManager.h"

USING_NS_CC;

PopupPetEquipStatus* PopupPetEquipStatus::create(int idx)
{
    PopupPetEquipStatus* pRet = new(std::nothrow) PopupPetEquipStatus();
    if ( pRet && pRet->init(idx) )
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

PopupPetEquipStatus::PopupPetEquipStatus(void) :
_bInitUI(false),

_idx(0),

_layerContainerInfo(nullptr),
_layerContainerInfoLevel(nullptr)
{
    
}

PopupPetEquipStatus::~PopupPetEquipStatus(void)
{
    
}

bool PopupPetEquipStatus::init(int idx)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    // init
    _idx = idx;
    
    //
    initVar();
    initUi();
    
    return true;
}

void PopupPetEquipStatus::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiInfo();
        uiInfoLevel();
    }
    
    //
    drawInfo();
    drawInfoLevel();
}

#pragma mark - init
void PopupPetEquipStatus::initVar()
{
    
}
void PopupPetEquipStatus::initUi()
{
    // size
    _spriteContainer->setContentSize( Size(750, 640) );
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _spriteContainer->setPositionY(getContentSize().height - UserInfoManager::getInstance()->_fHeightIphoneX);
    
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
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupPetEquipStatus::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
}

#pragma mark - ui
void PopupPetEquipStatus::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width -= 50;
    size.height = size.height - 75 - 50;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 25));
    _spriteContainer->addChild(layout);
    
    _layerContainerInfoLevel = ui::Layout::create();
    _layerContainerInfoLevel->setContentSize( Size(size.width, 370) );
    _layerContainerInfoLevel->setIgnoreAnchorPointForPosition(false);
    _layerContainerInfoLevel->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerInfoLevel->setPosition( Vec2(size.width / 2, 0) );
    layout->addChild(_layerContainerInfoLevel);
    
    _layerContainerInfo = ui::Layout::create();
    _layerContainerInfo->setContentSize(Size(size.width, size.height - _layerContainerInfoLevel->getContentSize().height));
    _layerContainerInfo->setIgnoreAnchorPointForPosition(false);
    _layerContainerInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerInfo->setPosition( Vec2(size.width / 2, size.height) );
    layout->addChild(_layerContainerInfo);
}

void PopupPetEquipStatus::uiInfo()
{
    auto layout = _layerContainerInfo;
    Size size = layout->getContentSize();
    
    auto objPetEquip = PetNewManager::getInstance()->getInfoPetEquip(_idx);
    
    E_PET ePet = objPetEquip->getType();
    
    int equipLevel = PetNewManager::getInstance()->getPetEquipLevel(_idx);
    int evolution = 1;
    if ( PetNewManager::getInstance()->getPetEvolution(ePet) > 0 )
    {
        evolution = 2;
    }
    
    std::string name = PetNewManager::getInstance()->getPetName(ePet);
    std::string desc = objPetEquip->getStory();
    
    std::string pathIconPet = MafUtils::format("Assets/ui/pet/peticon_0%d_s1_%d.png", ePet, evolution);
    std::string pathIconEquip = MafUtils::format("Assets/ui/pet/icon_equip/icon_pet_equip_%d.png", _idx);
    
    auto uiIconBG = ui::ImageView::create("Assets/ui/pet/btn_pet_icon_slot3_1.png");
    uiIconBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiIconBG->setPosition(Vec2(0, size.height));
    layout->addChild(uiIconBG);
    {
        auto uiIcon = ui::ImageView::create(pathIconEquip);
        uiIcon->setPosition(Vec2(uiIconBG->getContentSize().width / 2, uiIconBG->getContentSize().height / 2));
        uiIcon->setScale(2);
        uiIconBG->addChild(uiIcon);
    }
    
    double posTextX = uiIconBG->getPositionX() + uiIconBG->getContentSize().width + 10;
    
    auto uiIconPet = ui::ImageView::create(pathIconPet);
    uiIconPet->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiIconPet->setPosition(Vec2(posTextX, size.height - 20));
    uiIconPet->setScale(3);
    layout->addChild(uiIconPet);
    
    auto uiName = ui::CText::create(name, GAME_FONT, 26);
    uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiName->setPosition(Vec2(uiIconPet->getPositionX() + uiIconPet->getBoundingBox().size.width + 10, uiIconPet->getPositionY() - uiIconPet->getBoundingBox().size.height / 2));
    layout->addChild(uiName);

    auto uiDesc = ui::CText::create(desc, GAME_FONT, 22);
    uiDesc->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiDesc->setPosition(Vec2(posTextX, uiIconPet->getPositionY() - uiIconPet->getBoundingBox().size.height));
    uiDesc->setTextAreaSize(Size(550, 60));
    uiDesc->setTextVerticalAlignment(TextVAlignment::TOP);
    uiDesc->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiDesc->setTextOverflow(Label::Overflow::SHRINK);
    uiDesc->setLineSpacing(-5);
    layout->addChild(uiDesc);
}

void PopupPetEquipStatus::uiInfoLevel()
{
    auto layout = _layerContainerInfoLevel;
    Size size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/box.png");
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(Size(size.width, size.height));
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiBG->setPosition(Vec2(size.width / 2, 0));
    uiBG->setName("UI_BG");
    layout->addChild(uiBG);
}

#pragma mark - draw
void PopupPetEquipStatus::drawInfo()
{
    
}

void PopupPetEquipStatus::drawInfoLevel()
{
    auto layout = _layerContainerInfoLevel;
    Size size = layout->getContentSize();
    
    auto uiBG = layout->getChildByName<ui::ImageView*>("UI_BG");
    uiBG->removeAllChildren();
    
    int level = PetNewManager::getInstance()->getPetEquipLevel(_idx);
    int levelMax = PetNewManager::getInstance()->getPetEquipLevelMax();
    bool bLevelMax = level >= levelMax ? true : false;
    
    int itemNeed = PetNewManager::getInstance()->getPetEquipAmountNeed(level);
    int itemNow = PetNewManager::getInstance()->getPetEquipAmount(_idx);
    
    double percent = 0;
    if ( level > 0 )
    {
        percent = (double)itemNow / itemNeed * 100;
        if ( percent > 100 )
            percent = 100;
        
        if ( bLevelMax == true )
        {
            percent = 100;
        }
    }
    
    std::string levelText = MafUtils::format("LV.%d", level);
    std::string countText = MafUtils::format("%d/%d", itemNow, itemNeed);
    if ( bLevelMax == true )
    {
        levelText = "LV.MAX";
        countText = "MAX";
    }
    
    {
        auto uiCountBG = ui::ImageView::create("Assets/ui/pet/pet_exp_gauge_bg.png");
        uiCountBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiCountBG->setPosition(Vec2(uiBG->getContentSize().width / 2 - 650 / 2, uiBG->getContentSize().height - 100));
        uiBG->addChild(uiCountBG);
        {
            auto uiCountBar = ui::LoadingBar::create("Assets/ui/pet/pet_exp_gauge_blue.png");
            uiCountBar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            uiCountBar->setPosition(Vec2(0, 0));
            uiCountBar->setPercent(percent);
            uiCountBG->addChild(uiCountBar);
            
            auto uiCount = ui::CText::create(countText, GAME_FONT_AL, 24);
            uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiCount->setPosition(Vec2(uiCountBG->getContentSize().width / 2, uiCountBG->getContentSize().height / 2));
            uiCountBG->addChild(uiCount);
        }
        
        auto uiLevel = ui::CText::create(levelText, GAME_FONT_AL, 32);
        uiLevel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiLevel->setPosition(Vec2(2, uiCountBG->getContentSize().height));
        uiCountBG->addChild(uiLevel);
        
        if ( bLevelMax == false )
        {
            auto uiLevelUP = ui::Button::create("Assets/ui_common/btn_1_5_on.png");
            uiLevelUP->setScale9Enabled(true);
            uiLevelUP->setCapInsets(Rect(32,35,10,2));
            uiLevelUP->setContentSize(Size(117, 73));
            uiLevelUP->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            uiLevelUP->setPosition(Vec2(uiBG->getContentSize().width / 2 + 650 / 2, uiBG->getContentSize().height - 100));
            uiLevelUP->addClickEventListener(CC_CALLBACK_1(PopupPetEquipStatus::onClickLevelUP, this));
            uiBG->addChild(uiLevelUP);
            
            auto uiText = Label::createWithTTF("LV UP", GAME_FONT, 24);
            uiText->setPosition(Vec2(uiLevelUP->getContentSize().width / 2, uiLevelUP->getContentSize().height / 2));
            uiLevelUP->addChild(uiText);
        }
    }
    
    {
        std::string explain = PetNewManager::getInstance()->getPetEquipExplain(_idx);
        
        auto uiExplainBG = ui::ImageView::create("Assets/ui_common/base_box_1_1.png");
        uiExplainBG->setScale9Enabled(true);
        uiExplainBG->setContentSize(Size(uiBG->getContentSize().width - 50, 115));
        uiExplainBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiExplainBG->setPosition(Vec2(uiBG->getContentSize().width / 2, uiBG->getContentSize().height - 110));
        uiBG->addChild(uiExplainBG);
        
        double posY = uiExplainBG->getContentSize().height;
        
        auto uiExplain = ui::CText::create(GAME_TEXTFORMAT("t_pet_equip_explain_current", level), GAME_FONT, 24);
        uiExplain->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiExplain->setPosition(Vec2(10, posY));
        uiExplainBG->addChild(uiExplain);
        
        posY -= uiExplain->getContentSize().height;
        auto uiExplainOther = ui::CText::create(explain, GAME_FONT, 18);
        uiExplainOther->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiExplainOther->setPosition(Vec2(10, posY));
        uiExplainBG->addChild(uiExplainOther);
    }
    
    
    if ( level < levelMax )
    {
        std::string explain = PetNewManager::getInstance()->getPetEquipExplain(_idx, 1);
        
        auto uiExplainBG = ui::ImageView::create("Assets/ui_common/base_box_1_1.png");
        uiExplainBG->setScale9Enabled(true);
        uiExplainBG->setContentSize(Size(uiBG->getContentSize().width - 50, 115));
        uiExplainBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiExplainBG->setPosition(Vec2(uiBG->getContentSize().width / 2, uiBG->getContentSize().height - 235));
        uiBG->addChild(uiExplainBG);
        
        double posY = uiExplainBG->getContentSize().height;
        
        auto uiExplain = ui::CText::create(GAME_TEXTFORMAT("t_pet_equip_explain_current", level + 1), GAME_FONT, 24);
        uiExplain->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiExplain->setPosition(Vec2(10, posY));
        uiExplainBG->addChild(uiExplain);
        
        posY -= uiExplain->getContentSize().height;
        auto uiExplainOther = ui::CText::create(explain, GAME_FONT, 18);
        uiExplainOther->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiExplainOther->setPosition(Vec2(10, posY));
        uiExplainBG->addChild(uiExplainOther);
        
        posY -= uiExplainOther->getContentSize().height;
        auto uiExplainItem = ui::CText::create(GAME_TEXTFORMAT("t_pet_equip_levelup_amount", itemNeed), GAME_FONT, 18);
        uiExplainItem->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiExplainItem->setPosition(Vec2(10, posY));
        uiExplainItem->setTextColor(Color4B(COLOR_COIN));
        uiExplainBG->addChild(uiExplainItem);
    }
    
}

#pragma mark - callback
void PopupPetEquipStatus::callbackEquipLevelUP(bool bResult)
{
    //
    drawInfo();
    drawInfoLevel();
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::PET_EQUIP);
}

#pragma mark - click
void PopupPetEquipStatus::onClickClose(Ref* sender)
{
    hide();
}

void PopupPetEquipStatus::onClickLevelUP(cocos2d::Ref *sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    int equipIdx = _idx;
    
    int result = PetNewManager::getInstance()->onPetEquipLevelUP(equipIdx, true, CC_CALLBACK_1(PopupPetEquipStatus::callbackEquipLevelUP, this));
    if ( result != 0 )
    {
        /*
         0 : 성공
         1 : 조건 안됨
         2 : 재화 부족
         3 : 최대 레벨
         */
        if ( result == 2 )
        {
            auto popup = PopupDefault::create("LEVEL UP", GAME_TEXT("t_pet_equip_levelup_error"));
            popup->addButton(GAME_TEXT("t_ui_yes"), true, nullptr);
            popup->show();
        }
        return;
    }
    
}

