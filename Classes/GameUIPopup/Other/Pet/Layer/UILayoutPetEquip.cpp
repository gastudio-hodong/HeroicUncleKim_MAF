//
//  UILayoutPetEquip.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "UILayoutPetEquip.h"

#include "GameObject/InfoPetEquip.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/Pet/PopupPetEquipStatus.h"

#include "ManagerGame/PetNewManager.h"

USING_NS_CC;

UILayoutPetEquip* UILayoutPetEquip::create(E_PET ePet)
{
    UILayoutPetEquip* pRet = new(std::nothrow) UILayoutPetEquip();
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

UILayoutPetEquip::UILayoutPetEquip(void) :
_bInitUI(false),

_ePlace(E_PLACE::EMPTY),
_ePet(E_PET::CHICK_1),

_uiContentsUIInfo(nullptr),
_uiContentsUIList(nullptr),
_uiList(nullptr)
{
    
}

UILayoutPetEquip::~UILayoutPetEquip(void)
{
    
}

bool UILayoutPetEquip::init(E_PET ePet)
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
    
    //
    RefreshManager::getInstance()->addUI(E_REFRESH::PET_EQUIP, this);
    
    return true;
}

void UILayoutPetEquip::onEnter()
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
    
    // draw
    drawInfo();
    drawList();
}

#pragma mark - refresh
void UILayoutPetEquip::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::PET_EQUIP )
    {
        drawInfo();
        drawList();
    }
}

#pragma mark - init
void UILayoutPetEquip::initVar()
{
    _listPetEquip = PetNewManager::getInstance()->getListPetEquip(_ePet);
}

void UILayoutPetEquip::initUi()
{
    
}

#pragma mark - override DelegateListView
void UILayoutPetEquip::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
{
    switch (type) {
        case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
            break;
        case ui::ScrollView::EventType::SCROLL_TO_TOP:
            break;
        default:
            break;
    }
}

Size UILayoutPetEquip::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 135;

    return Size(width, height);
}

void UILayoutPetEquip::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    Size sizeLayout = Size(size.width, 130);

    // data
    auto objPetSkin = _listPetEquip.at(idx);
    
    int equipIdx = objPetSkin->getIdx();
    
    int level = PetNewManager::getInstance()->getPetEquipLevel(equipIdx);
    int levelMax = PetNewManager::getInstance()->getPetEquipLevelMax();
    bool bLevelMax = level >= levelMax ? true : false;
    
    int itemNeed = PetNewManager::getInstance()->getPetEquipAmountNeed(level);
    int itemNow = PetNewManager::getInstance()->getPetEquipAmount(equipIdx);
    
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
    
    std::string pathIcon = MafUtils::format("Assets/ui/pet/icon_equip/icon_pet_equip_%d.png", equipIdx);
    
    std::string nameText = objPetSkin->getName();
    std::string levelText = MafUtils::format("LV.%d", level);
    std::string countText = MafUtils::format("%d/%d", itemNow, itemNeed);
    if ( bLevelMax == true )
    {
        countText = "MAX";
    }
    std::string infoText = PetNewManager::getInstance()->getPetEquipExplain(equipIdx);
    
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(sizeLayout.width, sizeLayout.height));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    childview->addChild(uiLayout);
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/list_bg_1_s.png");
    uiBG->setScale9Enabled(true);
    uiBG->setCapInsets(Rect(10,60,10,10));
    uiBG->setContentSize(uiLayout->getContentSize());
    uiBG->setPosition(Vec2(uiLayout->getContentSize().width / 2, uiLayout->getContentSize().height / 2));
    uiLayout->addChild(uiBG);
    
    
    auto uiIconBG = ui::Button::create("Assets/ui/pet/btn_pet_icon_slot3_1.png");
    uiIconBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiIconBG->setPosition(Vec2(3, sizeLayout.height / 2));
    uiIconBG->addClickEventListener(CC_CALLBACK_1(UILayoutPetEquip::onClickDetail, this));
    uiIconBG->setTag(equipIdx);
    uiLayout->addChild(uiIconBG);
    {
        auto uiIcon = ui::ImageView::create(pathIcon);
        uiIcon->setPosition(Vec2(uiIconBG->getContentSize().width / 2, uiIconBG->getContentSize().height / 2));
        uiIcon->setScale(2);
        uiIconBG->addChild(uiIcon);
        
        auto uiLevel = ui::CText::create(levelText, GAME_FONT_AL, 22);
        uiLevel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        uiLevel->setPosition(Vec2(uiIconBG->getContentSize().width - 20, 20));
        uiIconBG->addChild(uiLevel);
    }
    
    double posTextX = uiIconBG->getPositionX() + uiIconBG->getContentSize().width + 10;
    
    auto uiName = ui::CText::create(nameText, GAME_FONT, 26);
    uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiName->setPosition(Vec2(posTextX, sizeLayout.height - 25));
    uiName->setTextColor(Color4B(COLOR_COIN));
    uiLayout->addChild(uiName);
    
    auto uiCountBG = ui::ImageView::create("Assets/ui/pet/pet_equipment_progressbar_1.png");
    uiCountBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiCountBG->setPosition(Vec2(posTextX, sizeLayout.height - 80));
    uiLayout->addChild(uiCountBG);
    {
        std::string pathCountBar = "Assets/ui/pet/pet_equipment_progressbar_2.png";
        if ( percent >= 100 )
            pathCountBar = "Assets/ui/pet/pet_equipment_progressbar_3.png";
        
        auto uiCountBar = ui::LoadingBar::create(pathCountBar);
        uiCountBar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiCountBar->setPosition(Vec2(0, 0));
        uiCountBar->setPercent(percent);
        uiCountBG->addChild(uiCountBar);
        
        auto uiCount = ui::CText::create(countText, GAME_FONT_AL, 24);
        uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiCount->setPosition(Vec2(10, uiCountBG->getContentSize().height / 2));
        uiCountBG->addChild(uiCount);
    }
    
    auto uiInfo = ui::CText::create(infoText, GAME_FONT, 18);
    uiInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiInfo->setPosition(Vec2(posTextX, 25));
    uiInfo->setTextAreaSize(Size(sizeLayout.width - 150 - posTextX, uiInfo->getContentSize().height));
    uiInfo->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiInfo->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiInfo->setTextOverflow(Label::Overflow::SHRINK);
    uiInfo->setLineSpacing(-10);
    uiLayout->addChild(uiInfo);
    if ( bLevelMax == true )
    {
        uiInfo->setTextAreaSize(Size(sizeLayout.width - 50 - posTextX, uiInfo->getContentSize().height));
    }
    
    if ( bLevelMax == false )
    {
        std::string pathLevelUP = "Assets/ui_common/btn_1_1_on.png";
        if ( itemNow < itemNeed )
        {
            pathLevelUP = "Assets/ui_common/btn_1_off.png";
        }
        
        auto uiLevelUp = ui::Button::create(pathLevelUP);
        uiLevelUp->setScale9Enabled(true);
        uiLevelUp->setCapInsets(Rect(33,33,2,2));
        uiLevelUp->setContentSize(Size(130, 110));
        uiLevelUp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiLevelUp->setPosition(Vec2(sizeLayout.width, sizeLayout.height / 2));
        uiLevelUp->addClickEventListener(CC_CALLBACK_1(UILayoutPetEquip::onClickLevelUP, this));
        uiLevelUp->setTag(equipIdx);
        uiLayout->addChild(uiLevelUp);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_wdungeon_m_skill_lvup"), GAME_FONT, 24);
            uiText->setPosition(Vec2(uiLevelUp->getContentSize().width / 2, uiLevelUp->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiLevelUp->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiLevelUp->addChild(uiText);
        }
    }
    
    if ( level <= 0 )
    {
        auto uiDisableBG = ui::ImageView::create(PIXEL);
        uiDisableBG->setScale9Enabled(true);
        uiDisableBG->setContentSize(sizeLayout);
        uiDisableBG->setPosition(Vec2(sizeLayout.width / 2, sizeLayout.height / 2));
        uiDisableBG->setColor(Color3B::BLACK);
        uiDisableBG->setOpacity(150);
        uiLayout->addChild(uiDisableBG);
    }
    
}

int64_t UILayoutPetEquip::listviewNumberOfCells(ui::CListView *listview)
{
    return _listPetEquip.size();
}

#pragma mark - ui
void UILayoutPetEquip::uiContainer()
{
    Size size = this->getContentSize();
    
    _uiContentsUIInfo = ui::Layout::create();
    _uiContentsUIInfo->setContentSize(Size(size.width, 110));
    _uiContentsUIInfo->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContentsUIInfo->setPosition(Vec2(size.width / 2, size.height));
    addChild(_uiContentsUIInfo);
    
    _uiContentsUIList = ui::Layout::create();
    _uiContentsUIList->setContentSize(Size(size.width, size.height - _uiContentsUIInfo->getContentSize().height));
    _uiContentsUIList->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsUIList->setPosition(Vec2(size.width / 2, 0));
    addChild(_uiContentsUIList);
}

void UILayoutPetEquip::uiInfo()
{
    auto layout = _uiContentsUIInfo;
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
    uiBonus->setContentSize(Size(440, 70));
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
        uiBonusPet->setTextAreaSize(Size(190, uiBonusPet->getContentSize().height));
        uiBonusPet->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiBonusPet->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiBonusPet->setTextOverflow(Label::Overflow::SHRINK);
        uiBonusPet->setTextColor(Color4B::GRAY);
        uiBonusPet->setName("UI_BONUS_PET");
        uiBonus->addChild(uiBonusPet);
        
        auto uiBonusPetEffect = ui::CText::create(GAME_TEXT("t_pet_equip_box_dmgbonus"), GAME_FONT, 20);
        uiBonusPetEffect->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiBonusPetEffect->setPosition(Vec2(0, uiBonus->getContentSize().height / 2));
        uiBonusPetEffect->setTextAreaSize(Size(190, uiBonusPet->getContentSize().height));
        uiBonusPetEffect->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiBonusPetEffect->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiBonusPetEffect->setTextOverflow(Label::Overflow::SHRINK);
        uiBonusPetEffect->setName("UI_BONUS_PET_EFFECT");
        uiBonus->addChild(uiBonusPetEffect);
        
        auto uiBonusCostume = ui::CText::create(GAME_TEXT("t_pet_equip_desc_content_2"), GAME_FONT, 18);
        uiBonusCostume->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiBonusCostume->setPosition(Vec2(uiBonus->getContentSize().width / 2 + 30, uiBonus->getContentSize().height / 2));
        uiBonusCostume->setTextAreaSize(Size(190, uiBonusPet->getContentSize().height));
        uiBonusCostume->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiBonusCostume->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiBonusCostume->setTextOverflow(Label::Overflow::SHRINK);
        uiBonusCostume->setTextColor(Color4B::GRAY);
        uiBonusCostume->setName("UI_BONUS_COSTUME");
        uiBonus->addChild(uiBonusCostume);
        
        auto uiBonusCostumeEffect = ui::CText::create(GAME_TEXTFORMAT("t_pet_equip_effect_per_bong_1", "0"), GAME_FONT, 20);
        uiBonusCostumeEffect->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiBonusCostumeEffect->setPosition(Vec2(uiBonus->getContentSize().width / 2 + 30, uiBonus->getContentSize().height / 2));
        uiBonusCostumeEffect->setTextAreaSize(Size(190, uiBonusPet->getContentSize().height));
        uiBonusCostumeEffect->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiBonusCostumeEffect->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiBonusCostumeEffect->setTextOverflow(Label::Overflow::SHRINK);
        uiBonusCostumeEffect->setName("UI_BONUS_COSTUME_EFFECT");
        uiBonus->addChild(uiBonusCostumeEffect);
    }
    
    auto uiLevelUPAll = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
    uiLevelUPAll->setScale9Enabled(true);
    uiLevelUPAll->setCapInsets(Rect(33,33,2,2));
    uiLevelUPAll->setContentSize(Size(200, 74));
    uiLevelUPAll->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiLevelUPAll->setPosition(Vec2(size.width - 10, size.height / 2));
    uiLevelUPAll->addClickEventListener(CC_CALLBACK_1(UILayoutPetEquip::onClickLevelUPAll, this));
    layout->addChild(uiLevelUPAll);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_advt_relic_levelup_all_bt"), GAME_FONT, 24);
        uiText->setPosition(Vec2(uiLevelUPAll->getContentSize().width / 2, uiLevelUPAll->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(uiLevelUPAll->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiLevelUPAll->addChild(uiText);
    }
}

void UILayoutPetEquip::uiList()
{
    auto layout = _uiContentsUIList;
    Size size = layout->getContentSize();
    
    _uiList = ui::CListView::create(this, Size(size.width, size.height), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(Vec2(size.width / 2, 0));
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    layout->addChild(_uiList);
}

#pragma mark - draw
void UILayoutPetEquip::drawInfo()
{
    auto layout = _uiContentsUIInfo;
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
}

void UILayoutPetEquip::drawList()
{
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}
 
#pragma mark - set, get, add


#pragma mark - callback
void UILayoutPetEquip::callbackEquipLevelUP(bool bResult)
{
    //
    if ( bResult == false )
    {
        drawInfo();
        drawList();
    }
    
}

void UILayoutPetEquip::callbackEquipLevelUPAll(bool bResult)
{
    //
    if ( bResult == false )
    {
        drawInfo();
        drawList();
    }
    
    
    //
    onPetEquipLevelUPAll();
}

#pragma mark - click
void UILayoutPetEquip::onClickDetail(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    int equipIdx = uiBtn->getTag();
    
    int level = PetNewManager::getInstance()->getPetEquipLevel(equipIdx);
    if ( level <= 0 )
    {
        return;
    }
    
    auto popup = PopupPetEquipStatus::create(equipIdx);
    popup->show();
}

void UILayoutPetEquip::onClickLevelUP(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    int equipIdx = uiBtn->getTag();
    
    int result = PetNewManager::getInstance()->onPetEquipLevelUP(equipIdx, true, CC_CALLBACK_1(UILayoutPetEquip::callbackEquipLevelUP, this));
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
    
    //
    drawInfo();
    drawList();
}

void UILayoutPetEquip::onClickLevelUPAll(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    
    //
    onPetEquipLevelUPAll();
}

#pragma mark -
void UILayoutPetEquip::onPetEquipLevelUPAll()
{
    for ( int i = 0; i < _listPetEquip.size(); i++ )
    {
        auto objPetSkin = _listPetEquip.at(i);
        
        int equipIdx = objPetSkin->getIdx();
        
        int level = PetNewManager::getInstance()->getPetEquipLevel(equipIdx);
        int levelMax = PetNewManager::getInstance()->getPetEquipLevelMax();
        if ( level <= 0 )
        {
            continue;
        }
        
        if ( level >= levelMax )
        {
            continue;
        }
        
        int itemNeed = PetNewManager::getInstance()->getPetEquipAmountNeed(level);
        int itemNow = PetNewManager::getInstance()->getPetEquipAmount(equipIdx);
        if ( itemNow < itemNeed )
        {
            continue;
        }
            
        PetNewManager::getInstance()->onPetEquipLevelUP(equipIdx, true, CC_CALLBACK_1(UILayoutPetEquip::callbackEquipLevelUPAll, this));
        return;
    }
}

