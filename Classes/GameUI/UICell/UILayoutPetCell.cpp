//
//  UILayoutPetCell.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "UILayoutPetCell.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/Equip/PopupWeaponPermanent.h"
#include "GameUIPopup/Other/Pet/PopupPet.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/WeaponManager.h"

USING_NS_CC;

UILayoutPetCell* UILayoutPetCell::create(E_PET ePet/* = E_PET::PET_NONE*/)
{
    UILayoutPetCell* pRet = new(std::nothrow) UILayoutPetCell();
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

UILayoutPetCell::UILayoutPetCell(void) :
_bInitUI(false),

_ePlace(E_PLACE::EMPTY),
_ePet(E_PET::PET_NONE),

_bDetailPopup(true),

//
_uiContentsUIList(nullptr),
_uiList(nullptr)
{
    
}

UILayoutPetCell::~UILayoutPetCell(void)
{
    
}

bool UILayoutPetCell::init(E_PET ePet)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    _ePlace = E_PLACE::DG_NORMAL;
    _ePet = ePet;
    
    // init
    initVar();
    initUi();
    
    //
    RefreshManager::getInstance()->addUI(E_REFRESH::PET_FREE, this);
    RefreshManager::getInstance()->addUI(E_REFRESH::PET, this);
    
    return true;
}

void UILayoutPetCell::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiList();
    }
    
    // draw
    drawList();
}

#pragma mark - refresh
void UILayoutPetCell::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::PET || eType == E_REFRESH::PET_FREE )
    {
        drawList();
    }
}

#pragma mark - override DelegateListView
void UILayoutPetCell::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size UILayoutPetCell::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 135;

    return Size(width, height);
}

void UILayoutPetCell::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    Size sizeLayout = Size(size.width, 130);

    // data
    E_PET ePet = _listPet.at(idx);
    
    int level = PetNewManager::getInstance()->getPetLevel(ePet);
    int levelMax = PetNewManager::getInstance()->getPetLevelMax(ePet);
    bool bLevelMax = level >= levelMax ? true : false;
    bool bFree = false;
    bool bAcquire = false;
    if ( level == 0 )
    {
        int floor = UserInfoManager::getInstance()->getHighFloor(_ePlace);
        int floorAcquire = PetNewManager::getInstance()->getPetAcquireFloor(ePet);
        
        if ( ePet == E_PET::MOLE_7 && WeaponManager::getInstance()->getEquipHigh() >= E_WEAPON::WEAPON_76_MINE )
        {
            bFree = true;
        }
        else if ( floorAcquire != 0 && floor > floorAcquire )
        {
            bFree = true;
        }
        else
        {
            bAcquire = true;
        }
    }
    
    double coolTime = PetNewManager::getInstance()->getPetCoolTime(ePet);
    double effect = PetNewManager::getInstance()->getPetEffect(ePet, level);
    if ( level <= 1 )
    {
        effect = PetNewManager::getInstance()->getPetEffect(ePet, 1);
    }
    
    int evolutionLevel = PetNewManager::getInstance()->getPetEvolution(ePet);
    double evolutionEffect1 =PetNewManager::getInstance()->getPetEvolutionEffect1(ePet, evolutionLevel);
    double evolutionEffect2 =PetNewManager::getInstance()->getPetEvolutionEffect2(ePet, evolutionLevel);
    
    int reinforceLevel = PetNewManager::getInstance()->getPetReinforce(ePet);
    double reinforceEffect = PetNewManager::getInstance()->getPetReinforceEffect(ePet, reinforceLevel);
    
    std::string pathIcon = PetNewManager::getInstance()->getPetImagePath(ePet);
    
    std::string nameText = PetNewManager::getInstance()->getPetName(ePet);
    std::string levelText = MafUtils::format("LV.%d", level);
    std::string infoText = "";
    
    if ( ePet == E_PET::CHICK_1 )
    {
        std::string text = MafUtils::format(PetNewManager::getInstance()->getPetExplain(ePet).c_str(), coolTime);
        infoText.append(text);
    }
    else if ( ePet == E_PET::HONG_6 )
    {
        int effect = 10 + evolutionEffect1;
        
        std::string text = MafUtils::format(PetNewManager::getInstance()->getPetExplain(ePet).c_str(), coolTime, effect);
        infoText.append(text);
    }
    else
    {
        std::string text = PetNewManager::getInstance()->getPetExplain(ePet);
        infoText.append(text);
    }
    infoText.append("\n");
    
    if ( ePet == E_PET::CHICK_1 || ePet == E_PET::PURIRING_5 )
    {
        std::string text = MafUtils::format(PetNewManager::getInstance()->getPetUpgradeExplain(ePet).c_str(), effect * 100);
        infoText.append(text);
    }
    else if ( ePet == E_PET::DRAGON_3 )
    {
        double effectMin = 0;
        if ( level > 0 )
            effectMin = (level-1)*4;
        
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_38) )
        {
            effectMin += 10;
        }
        
        if (evolutionEffect1 > 0)
        {
            effectMin += evolutionEffect1;
        }

        if (reinforceEffect > 0)
        {
            effect += reinforceEffect;
        }
        
        std::string text = MafUtils::format(PetNewManager::getInstance()->getPetUpgradeExplain(ePet).c_str(), effectMin, effect);
        infoText.append(text);
    }
    else if ( ePet == E_PET::HONG_6 )
    {
        std::string text = MafUtils::format(PetNewManager::getInstance()->getPetUpgradeExplain(ePet).c_str(), effect + reinforceEffect);
        infoText.append(text);
    }
    else
    {
        std::string text = MafUtils::format(PetNewManager::getInstance()->getPetUpgradeExplain(ePet).c_str(), effect);
        infoText.append(text);
    }
    
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
     
    auto uiIconBG = ui::Button::create("Assets/ui_common/box_quest.png");
    uiIconBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiIconBG->setPosition(Vec2(3, sizeLayout.height / 2));
    uiIconBG->addClickEventListener(CC_CALLBACK_1(UILayoutPetCell::onClickDetail, this));
    uiIconBG->setTag(ePet);
    uiLayout->addChild(uiIconBG);
    {
        auto uiIcon = ui::ImageView::create(pathIcon);
        uiIcon->setPosition(Vec2(uiIconBG->getContentSize().width / 2, uiIconBG->getContentSize().height / 2));
        uiIcon->setScale(1.5);
        uiIconBG->addChild(uiIcon);
        
        if ( _bDetailPopup == true )
        {
            auto uiDetailBG = ui::ImageView::create("Assets/ui_common/base_box_1_16.png");
            uiDetailBG->setScale9Enabled(true);
            uiDetailBG->setContentSize(Size(uiIconBG->getContentSize().width, 32));
            uiDetailBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiDetailBG->setPosition(Vec2(uiIconBG->getContentSize().width / 2, 0));
            uiIconBG->addChild(uiDetailBG);
            
            auto uiDetailIcon = ui::ImageView::create("Assets/ui/pet/icon_magnifier.png");
            uiDetailIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiDetailIcon->setPosition(Vec2(10, uiDetailBG->getContentSize().height / 2));
            uiDetailBG->addChild(uiDetailIcon);
            
            auto uiDetailText = ui::CText::create(GAME_TEXT("t_ui_shop_bonus_msg_5"), GAME_FONT, 16);
            uiDetailText->setPosition(Vec2(uiDetailBG->getContentSize().width / 2 + 10, uiDetailBG->getContentSize().height / 2));
            uiDetailText->setTextAreaSize(Size(75, uiDetailText->getContentSize().height));
            uiDetailText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiDetailText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiDetailText->setTextOverflow(Label::Overflow::SHRINK);
            uiDetailBG->addChild(uiDetailText);
            
            auto uiReddot1 = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
            uiReddot1->setPosition(3, uiIconBG->getContentSize().height - 3);
            uiReddot1->setVisible(false);
            uiIconBG->addChild(uiReddot1);
            if ( ePet == E_PET::CHICK_1 )           RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_CHICK, uiReddot1);
            else if ( ePet == E_PET::HAMSTER_2 )    RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_HAMSTER, uiReddot1);
            else if ( ePet == E_PET::DRAGON_3 )     RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_DRAGON, uiReddot1);
            else if ( ePet == E_PET::PRINCESS_4 )   RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_PRINCESS, uiReddot1);
            else if ( ePet == E_PET::PURIRING_5 )   RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_PURIRING, uiReddot1);
            else if ( ePet == E_PET::HONG_6 )       RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_HONG, uiReddot1);
            else if ( ePet == E_PET::MOLE_7 )       RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_MOLE, uiReddot1);
            else if ( ePet == E_PET::HANDSOME_8 )   RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_HANDSOME, uiReddot1);
            
            auto uiReddot2 = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
            uiReddot2->setPosition(3, uiIconBG->getContentSize().height - 3);
            uiReddot2->setVisible(false);
            uiIconBG->addChild(uiReddot2);
            if ( ePet == E_PET::CHICK_1 )           RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_CHICK, uiReddot2);
            else if ( ePet == E_PET::HAMSTER_2 )    RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_HAMSTER, uiReddot2);
            else if ( ePet == E_PET::DRAGON_3 )     RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_DRAGON, uiReddot2);
            else if ( ePet == E_PET::PRINCESS_4 )   RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_PRINCESS, uiReddot2);
            else if ( ePet == E_PET::PURIRING_5 )   RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_PURIRING, uiReddot2);
            else if ( ePet == E_PET::HONG_6 )       RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_HONG, uiReddot2);
            else if ( ePet == E_PET::MOLE_7 )       RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_MOLE, uiReddot2);
            else if ( ePet == E_PET::HANDSOME_8 )   RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_HANDSOME, uiReddot2);
            
            auto uiReddot3 = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
            uiReddot3->setPosition(3, uiIconBG->getContentSize().height - 3);
            uiReddot3->setVisible(false);
            uiIconBG->addChild(uiReddot3);
            if ( ePet == E_PET::CHICK_1 )           RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_CHICK, uiReddot3);
            else if ( ePet == E_PET::HAMSTER_2 )    RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_HAMSTER, uiReddot3);
            else if ( ePet == E_PET::DRAGON_3 )     RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_DRAGON, uiReddot3);
            else if ( ePet == E_PET::PRINCESS_4 )   RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_PRINCESS, uiReddot3);
            else if ( ePet == E_PET::PURIRING_5 )   RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_PURIRING, uiReddot3);
            else if ( ePet == E_PET::HONG_6 )       RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_HONG, uiReddot3);
            else if ( ePet == E_PET::MOLE_7 )       RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_MOLE, uiReddot3);
            else if ( ePet == E_PET::HANDSOME_8 )   RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_HANDSOME, uiReddot3);
        }
    }
    
    double posTextX = uiIconBG->getPositionX() + uiIconBG->getContentSize().width + 10;
    
    auto uiName = ui::CText::create(nameText, GAME_FONT, 26);
    uiName->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiName->setPosition(Vec2(posTextX, sizeLayout.height - 35));
    uiName->setTextColor(Color4B(COLOR_COIN));
    uiLayout->addChild(uiName);
    
    auto uiLevel = ui::CText::create(levelText, GAME_FONT_AL, 22);
    uiLevel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiLevel->setPosition(Vec2(posTextX + 3, sizeLayout.height - 30));
    uiLayout->addChild(uiLevel);
    
    auto uiInfo = ui::CText::create(infoText, GAME_FONT, 22);
    uiInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiInfo->setPosition(Vec2(posTextX, 40));
    uiInfo->setTextAreaSize(Size(sizeLayout.width - 200 - posTextX, uiInfo->getContentSize().height));
    uiInfo->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiInfo->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiInfo->setTextOverflow(Label::Overflow::SHRINK);
    uiInfo->setLineSpacing(-10);
    uiLayout->addChild(uiInfo);
    if ( bLevelMax == true )
    {
        uiInfo->setTextAreaSize(Size(sizeLayout.width - 50 - posTextX, uiInfo->getContentSize().height));
    }
    
    if ( bAcquire == true )
    {
        int floor = UserInfoManager::getInstance()->getFloor(_ePlace);
        int floorAcquire = PetNewManager::getInstance()->getPetAcquireFloor(ePet);
        int floorDiff = (floorAcquire + 1) - floor;
        
        std::string acquireText = GAME_TEXTFORMAT("t_ui_pet_msg_16", floorAcquire);
        if ( ePet == E_PET::MOLE_7 )
        {
            acquireText = GAME_TEXT("t_ui_pet_msg_16_1");
        }
        
        auto uiAcquire = ui::CText::create(acquireText, GAME_FONT, 20);
        uiAcquire->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiAcquire->setPosition(Vec2(sizeLayout.width - 10, sizeLayout.height - 25));
        uiAcquire->setTextAreaSize(Size(300, uiAcquire->getContentSize().height));
        uiAcquire->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiAcquire->setTextHorizontalAlignment(TextHAlignment::RIGHT);
        uiAcquire->setTextOverflow(Label::Overflow::SHRINK);
        uiLayout->addChild(uiAcquire);
        
        if ( floorAcquire > 0 )
        {
            auto uiAcquireLEFT = ui::ImageView::create("Assets/ui/pet/pet_remaining_f_1.png");
            uiAcquireLEFT->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiAcquireLEFT->setPosition(Vec2(sizeLayout.width - 10, 40));
            uiLayout->addChild(uiAcquireLEFT);
            {
                auto uiText = ui::CText::create(GAME_TEXTFORMAT("t_ui_pet_msg_17", floorDiff), GAME_FONT, 18);
                uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                uiText->setPosition(Vec2(uiAcquireLEFT->getContentSize().width - 10, uiAcquireLEFT->getContentSize().height / 2));
                uiText->setTextAreaSize(Size(90, uiText->getContentSize().height));
                uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
                uiText->setTextHorizontalAlignment(TextHAlignment::RIGHT);
                uiText->setTextOverflow(Label::Overflow::SHRINK);
                uiAcquireLEFT->addChild(uiText);
                
                auto action1 = DelayTime::create(1.0);
                auto action2 = CallFuncN::create([=](Ref* sender){
                    
                    int floor = UserInfoManager::getInstance()->getFloor(_ePlace);
                    int floorAcquire = PetNewManager::getInstance()->getPetAcquireFloor(ePet);
                    int floorDiff = (floorAcquire + 1) - floor;
                    if ( floorDiff <= 0 )
                    {
                        ((ui::CText*)sender)->stopAllActions();
                        drawList();
                    }
                    else
                    {
                        ((ui::CText*)sender)->setString(GAME_TEXTFORMAT("t_ui_pet_msg_17", floorDiff));
                        ((ui::CText*)sender)->setTextAreaSize(Size(90, uiText->getContentSize().height));
                        ((ui::CText*)sender)->setTextOverflow(Label::Overflow::SHRINK);
                    }
                });
                auto seq = Sequence::create(action1, action2, NULL);
                uiText->runAction(RepeatForever::create(seq));
            }
        }
    }
    else if ( bFree == true )
    {
        auto uiFree = ui::Button::create("Assets/ui_common/btn_upgrade_bg_2.png");
        uiFree->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiFree->setPosition(Vec2(sizeLayout.width, sizeLayout.height / 2));
        uiFree->addClickEventListener(CC_CALLBACK_1(UILayoutPetCell::onClickPetFree, this));
        uiFree->setTag(ePet);
        uiLayout->addChild(uiFree);
        
        auto uiText = ui::CText::create("FREE", GAME_FONT_AL, 30);
        uiText->setPosition(Vec2(uiFree->getContentSize().width / 2, uiFree->getContentSize().height / 2));
        uiText->setTextColor(Color4B(COLOR_COIN));
        uiFree->addChild(uiText);
    }
    else if ( bLevelMax == false )
    {
        auto itemNeed = PetNewManager::getInstance()->getPetUpgradePrice(ePet, level);
        auto itemNow= ItemsMoneyManager::getInstance()->getGem();
        
        std::string pathBtn = "Assets/ui_common/btn_upgrade_bg_2.png";
        if ( itemNow < itemNeed )
        {
            pathBtn = "Assets/ui_common/btn_upgrade_bg_2_max.png";
        }
            
        auto action1 = DelayTime::create(1.0);
        auto action2 = CallFuncN::create([=](Ref* sender){
            
            auto uiLevelUp = (ui::Button*)sender;
            
            auto itemNeed = PetNewManager::getInstance()->getPetUpgradePrice(ePet, level);
            auto itemNow= ItemsMoneyManager::getInstance()->getGem();
            
            std::string pathBtn = "Assets/ui_common/btn_upgrade_bg_2.png";
            if ( itemNow < itemNeed )
            {
                pathBtn = "Assets/ui_common/btn_upgrade_bg_2_max.png";
            }
            
            uiLevelUp->loadTextureNormal(pathBtn);
        });
        auto uiLevelUp = ui::Button::create(pathBtn);
        uiLevelUp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiLevelUp->setPosition(Vec2(sizeLayout.width, sizeLayout.height / 2));
        uiLevelUp->addClickEventListener(CC_CALLBACK_1(UILayoutPetCell::onClickPetLevelUP, this));
        uiLevelUp->setTag(ePet);
        uiLayout->addChild(uiLevelUp);
        {
            uiLevelUp->runAction(RepeatForever::create(Sequence::create(action1, action2, NULL)));
        }
        
        auto uiItemIcon = ui::ImageView::create("Assets/icon/icon_jewel.png");
        uiItemIcon->setPosition(Vec2(uiLevelUp->getContentSize().width * 0.175, uiLevelUp->getContentSize().height * 0.5));
        uiLevelUp->addChild(uiItemIcon);
        
        auto uiItemCount = ui::CText::create(MafUtils::toString(itemNeed), GAME_FONT_AL, 30);
        uiItemCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiItemCount->setPosition(Vec2(uiLevelUp->getContentSize().width * 0.35, uiLevelUp->getContentSize().height * 0.5));
        uiItemCount->setTextColor(Color4B(COLOR_COIN));
        uiItemCount->setTextAreaSize(Size(85, 25));
        uiItemCount->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiItemCount->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiItemCount->setTextOverflow(Label::Overflow::SHRINK);
        uiLevelUp->addChild(uiItemCount);
    }
}

int64_t UILayoutPetCell::listviewNumberOfCells(ui::CListView *listview)
{
    return _listPet.size();
}

#pragma mark - init
void UILayoutPetCell::initVar()
{
    if ( _ePet != E_PET::PET_NONE )
    {
        _listPet.push_back(_ePet);
    }
    else
    {
        _listPet.push_back(E_PET::CHICK_1);
        _listPet.push_back(E_PET::HAMSTER_2);
        _listPet.push_back(E_PET::DRAGON_3);
        _listPet.push_back(E_PET::PRINCESS_4);
        _listPet.push_back(E_PET::PURIRING_5);
        _listPet.push_back(E_PET::HONG_6);
        _listPet.push_back(E_PET::MOLE_7);
        _listPet.push_back(E_PET::HANDSOME_8);
    }
}

void UILayoutPetCell::initUi()
{
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
}

#pragma mark - ui
void UILayoutPetCell::uiContainer()
{
    Size size = this->getContentSize();
    
    _uiContentsUIList = ui::Layout::create();
    _uiContentsUIList->setContentSize(Size(size.width, size.height));
    _uiContentsUIList->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsUIList->setPosition(Vec2(size.width / 2, 0));
    addChild(_uiContentsUIList);
}

void UILayoutPetCell::uiList()
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
void UILayoutPetCell::drawList()
{
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}
 
#pragma mark - set, get, add
void UILayoutPetCell::setDetailPopup(bool bPopup)
{
    _bDetailPopup = bPopup;
    
    if ( _bInitUI == true )
    {
        drawList();
    }
}

#pragma mark - callback
 

#pragma mark - click
void UILayoutPetCell::onClickDetail(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    if ( _bDetailPopup == false )
    {
        return;
    }
    
    E_PET ePet = (E_PET)uiBtn->getTag();
    
    auto popup = PopupPet::create(ePet);
    popup->show();
}

void UILayoutPetCell::onClickPetFree(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    E_PET ePet = (E_PET)uiBtn->getTag();
    
    int level = PetNewManager::getInstance()->getPetLevel(ePet);
    if ( level > 0 )
    {
        return;
    }
    
    int floor = UserInfoManager::getInstance()->getHighFloor(_ePlace);
    int floorAcquire = PetNewManager::getInstance()->getPetAcquireFloor(ePet);
    
    bool bFree = false;
    if ( ePet == E_PET::MOLE_7 && WeaponManager::getInstance()->getEquipHigh() >= E_WEAPON::WEAPON_76_MINE )
    {
        bFree = true;
    }
    else if ( floorAcquire != 0 && floor > floorAcquire )
    {
        bFree = true;
    }
    
    if ( bFree == false )
    {
        return;
    }
     
    int result = PetNewManager::getInstance()->onPetFree(ePet);
    if ( result != 0 )
    {
        return;
    }
    
    // auto refresh
}

void UILayoutPetCell::onClickPetLevelUP(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    E_PET ePet = (E_PET)uiBtn->getTag();
    
    int level = PetNewManager::getInstance()->getPetLevel(ePet);
    int levelMax = PetNewManager::getInstance()->getPetLevelMax(ePet);
    bool bLevelMax = level >= levelMax ? true : false;
    
    if ( bLevelMax == true )
    {
        return;
    }
    
    auto itemNow = ItemsMoneyManager::getInstance()->getGem();
    auto itemNeed = PetNewManager::getInstance()->getPetUpgradePrice(ePet, level);
    if ( itemNow >= itemNeed )
    {
        std::string strTitle = PetNewManager::getInstance()->getPetName(ePet);
        std::string strMessage = GAME_TEXT("t_ui_msg_buy");
        strMessage.append(MafUtils::format("\n(Lv.%d -> Lv.%d)", level, level + 1));
        
        auto popup = PopupDefault::create(strTitle, strMessage);
        popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
        popup->addButton(GAME_TEXT("t_ui_yes"), false, [=](Ref* sender){
            
            int result = PetNewManager::getInstance()->onPetLevelUP(ePet);
            if ( result != 0 )
            {
                return;
            }
            
            // auto refresh
        });
        popup->show();
    }
    else
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
        popup->show();
    }
}
