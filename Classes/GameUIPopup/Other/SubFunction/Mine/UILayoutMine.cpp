//
//  UILayoutMine.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "UILayoutMine.h"

#include "GameObject/InfoArtifact.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseBuy.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/AchievementManager.h"
#include "ManagerGame/MineralManager.h"
#include "ManagerGame/PassManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/WeaponManager.h"

USING_NS_CC;

UILayoutMine* UILayoutMine::create()
{
    UILayoutMine* pRet = new(std::nothrow) UILayoutMine();
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

UILayoutMine::UILayoutMine():
_ePlace(E_PLACE::EMPTY),
_eArtifact(E_ARTIFACT::ARTI_34_PICKAX),
_eMineralSpecial(E_MINERAL::MINERAL_7_SAPPHIRE),

//
_uiContainerMineInfo(nullptr),
_uiContainerList(nullptr),

_uiList(nullptr),

_uiImgPet(nullptr),
_uiBtnArtifactUpgrade(nullptr),
_uiTextArtifactName(nullptr),
_uiTextArtifactDesc(nullptr),
_uiTextArtifactInfo(nullptr)
{
    
}

bool UILayoutMine::init()
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
        
    //
    initVar();
    initUI();
    
    return true;
}


void UILayoutMine::onEnter()
{
    ui::Layout::onEnter();
    
    //
    uiContainer();
    uiMineInfo();
    uiList();
    
    // story
    if ( WeaponManager::getInstance()->getEquipHigh() >= E_WEAPON::WEAPON_76_MINE && PetNewManager::getInstance()->getPet(E_PET::MOLE_7) == false )
    {
        PetNewManager::getInstance()->onPetFree(E_PET::MOLE_7);
    }
    
    //
    MafAnalyticsManager::LogEvent(kAnalEventMineEnter, kRepeatTrue);
}

#pragma mark - init
void UILayoutMine::initVar()
{
    _ePlace = E_PLACE::DG_NORMAL;
    _eMineralSpecial = MineralManager::getInstance()->getSpecialMineral();
    
    _listMine.push_back(E_MINERAL::MINERAL_7_SAPPHIRE);
    _listMine.push_back(E_MINERAL::MINERAL_1);
    _listMine.push_back(E_MINERAL::MINERAL_2);
    _listMine.push_back(E_MINERAL::MINERAL_3);
    _listMine.push_back(E_MINERAL::MINERAL_4);
    _listMine.push_back(E_MINERAL::MINERAL_5);
    _listMine.push_back(E_MINERAL::MINERAL_6);
}

void UILayoutMine::initUI()
{
    
}

#pragma mark - override DelegateListView
void UILayoutMine::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size UILayoutMine::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 135;

    return Size(width, height);
}

void UILayoutMine::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();

    // data
    auto eType = (E_MINERAL)_listMine.at(idx);
    if ( eType >= E_MINERAL::MINERAL_7_SAPPHIRE )
    {
        eType = _eMineralSpecial;
    }
        
    std::string pathIcon = DataManager::GetMineMineralPath(eType);
    std::string name = DataManager::GetMineMineralName(eType);
    
    int countAdd = MineralManager::getInstance()->getCountAdd(eType);
    
    int buyAdd = MineralManager::getInstance()->getBuyAdd(eType);
    int buyPrice = MineralManager::getInstance()->getBuyPrice(eType);
    
    // ui
    childview->removeAllChildren();
    
    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(size.width, size.height - 5));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    childview->addChild(uiLayout);
    
    // bg
    auto uiImgBG = ui::ImageView::create("Assets/ui_common/list_bg_1.png");
    uiImgBG->setContentSize(uiLayout->getContentSize());
    uiImgBG->setPosition(Vec2(uiLayout->getContentSize().width / 2, uiLayout->getContentSize().height / 2));
    uiImgBG->setScale9Enabled(true);
    uiImgBG->setCapInsets(Rect(0,51,150,10));
    uiLayout->addChild(uiImgBG);
    
    //
    auto uiBtnIcon = ui::Button::create("Assets/ui_common/box_quest.png");
    uiBtnIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiBtnIcon->setPosition(Vec2(3, uiLayout->getContentSize().height / 2));
    uiBtnIcon->addClickEventListener(CC_CALLBACK_1(UILayoutMine::onClickMineSpecialChange, this));
    uiBtnIcon->setZoomScale(0.05f);
    uiBtnIcon->setTag((int)idx);
    uiLayout->addChild(uiBtnIcon);
    {
        auto uiImgIcon = ui::ImageView::create(pathIcon);
        uiImgIcon->setPosition(Vec2(uiBtnIcon->getContentSize().width / 2, uiBtnIcon->getContentSize().height / 2));
        uiImgIcon->setScale(1.5);
        uiBtnIcon->addChild(uiImgIcon);
        
        if ( eType >= E_MINERAL::MINERAL_7_SAPPHIRE )
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_mine_change"), GAME_FONT, 22);
            uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiText->setPosition(Vec2(uiBtnIcon->getContentSize().width / 2, 5));
            uiText->setTextColor(Color4B(COLOR_COIN));
            uiBtnIcon->addChild(uiText);
        }
    }
    
    // name
    auto uiTextName = ui::CText::create(name, GAME_FONT, 26);
    uiTextName->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiTextName->setPosition(Vec2(125, uiLayout->getContentSize().height - 7));
    uiTextName->setTextVerticalAlignment(TextVAlignment::TOP);
    uiTextName->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiTextName->setTextColor(Color4B(COLOR_COIN));
    uiLayout->addChild(uiTextName);
    
    // progress
    auto uiImgProgressBG = ui::ImageView::create("Assets/battle/gauge_bg.png");
    uiImgProgressBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiImgProgressBG->setPosition(Vec2(125, 7));
    uiLayout->addChild(uiImgProgressBG);
    {
        auto uiBarMineralGetTime = ui::LoadingBar::create("Assets/battle/gauge_gauge.png");
        uiBarMineralGetTime->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiBarMineralGetTime->setPosition(Vec2(0, 0));
        uiBarMineralGetTime->setPercent(90);
        uiImgProgressBG->addChild(uiBarMineralGetTime);
        {
            auto action1 = CallFuncN::create([=](Node* sender){
                
                double timeMax = MineralManager::getInstance()->getTimeMax(eType);
                double time = MineralManager::getInstance()->getTime(eType);
                int nPercent = 100 - (double)time / timeMax * 100;
                if ( nPercent > 100 )
                    nPercent = 100;
                
                auto uiBarMineralGetTime = (ui::LoadingBar*)sender;
                uiBarMineralGetTime->setPercent(nPercent);
            });
            auto seq = Sequence::create(action1, NULL);
            uiBarMineralGetTime->runAction(RepeatForever::create(seq));
        }
        
        // icon
        auto uiImgMineralIcon = ui::ImageView::create(pathIcon);
        uiImgMineralIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiImgMineralIcon->setPosition(Vec2(2.5, uiImgProgressBG->getContentSize().height / 2));
        uiImgProgressBG->addChild(uiImgMineralIcon);
        
        // get count
        auto uiTextMineralGetCount = ui::CText::create(MafUtils::toString(countAdd), GAME_FONT_AL, 28);
        uiTextMineralGetCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiTextMineralGetCount->setPosition(Vec2(55, uiImgProgressBG->getContentSize().height / 2));
        uiImgProgressBG->addChild(uiTextMineralGetCount);
        
        // time
        auto uiTextMineralGetTime = ui::CText::create("00:00:00", GAME_FONT_AL, 28);
        uiTextMineralGetTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiTextMineralGetTime->setPosition(Vec2(uiImgProgressBG->getContentSize().width - 5, uiImgProgressBG->getContentSize().height / 2));
        uiImgProgressBG->addChild(uiTextMineralGetTime);
        {
            auto action1 = CallFuncN::create([=](Node* sender){
            
                auto time = MineralManager::getInstance()->getTime(eType);
                
                auto uiTextMineralGetTime = (ui::CText*)sender;
                if ( time < 0 )
                {
                    uiTextMineralGetTime->setString("00:00:00");
                }
                else
                {
                    uiTextMineralGetTime->setString(MafUtils::convertNumberToTime(time, true));
                }
            });
            auto action2 = DelayTime::create(1.0);
            auto seq = Sequence::create(action1, action2, NULL);
            uiTextMineralGetTime->runAction(RepeatForever::create(seq));
        }
    }
    
    // count
    auto uiTextMineralCount = ui::CText::create(GAME_TEXTFORMAT("t_ui_mine_msg_1", 0), GAME_FONT, 22);
    uiTextMineralCount->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiTextMineralCount->setPosition(Vec2(uiImgProgressBG->getPositionX(), uiImgProgressBG->getPositionY() + uiImgProgressBG->getContentSize().height));
    uiTextMineralCount->setName("UI_COUNT");
    uiLayout->addChild(uiTextMineralCount);
    {
        auto action1 = CallFuncN::create([=](Node* sender){
        
            auto count = MineralManager::getInstance()->getCount(eType);
            
            auto uiTextMineralCount = (ui::CText*)sender;
            uiTextMineralCount->setString(GAME_TEXTFORMAT("t_ui_mine_msg_1", count));
        });
        auto action2 = DelayTime::create(1.0);
        auto seq = Sequence::create(action1, action2, NULL);
        uiTextMineralCount->runAction(RepeatForever::create(seq));
    }
    
    // buy
    auto uiBtnBuy = ui::Button::create("Assets/ui_common/btn_upgrade_bg_2.png");
    uiBtnBuy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiBtnBuy->setPosition(Vec2(uiLayout->getContentSize().width - 5, uiLayout->getContentSize().height / 2));
    uiBtnBuy->addClickEventListener(CC_CALLBACK_1(UILayoutMine::onClickMineBuy, this));
    uiBtnBuy->setZoomScale(0.05f);
    uiBtnBuy->setTag((int)idx);
    uiLayout->addChild(uiBtnBuy);
    {
        auto uiImgIcon = ui::ImageView::create("Assets/icon/icon_jewel.png");
        uiImgIcon->setPosition(Vec2(uiBtnBuy->getContentSize().width * 0.175, uiBtnBuy->getContentSize().height * 0.25));
        uiBtnBuy->addChild(uiImgIcon);
        
        auto uiTextPrice = ui::CText::createWithBMFont(MafUtils::toString(buyPrice), GAME_BM_FONT_AL, 30);
        uiTextPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiTextPrice->setPosition(Vec2(uiBtnBuy->getContentSize().width * 0.350, uiBtnBuy->getContentSize().height * 0.25));
        uiTextPrice->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiTextPrice->setColor(COLOR_COIN);
        uiTextPrice->setTextAreaSize(Size(85, 25));
        uiTextPrice->setTextOverflow(Label::Overflow::SHRINK);
        uiBtnBuy->addChild(uiTextPrice);
        
        auto uiText = ui::CText::createWithBMFont(MafUtils::format("+%d", buyAdd), GAME_BM_FONT_AL, 24);
        uiText->setAnchorPoint(Vec2(0.5,1));
        uiText->setPosition(Vec2(uiBtnBuy->getContentSize().width*0.5, uiBtnBuy->getContentSize().height*0.8));
        uiBtnBuy->addChild(uiText);
    }
    {
        auto action1 = CallFuncN::create([=](Node* sender){
        
            int buyCount = MineralManager::getInstance()->getBuyCount(eType);
            int buyCountMax = MineralManager::getInstance()->getBuyCountMax();
            int buyPrice = MineralManager::getInstance()->getBuyPrice(eType);
            int currency = ItemsMoneyManager::getInstance()->getGem();
            
            auto uiBtnBuy = (ui::Button*)sender;
            if ( buyCount >= buyCountMax || currency < buyPrice )
            {
                uiBtnBuy->loadTextureNormal("Assets/ui_common/btn_upgrade_bg_2_max.png");
            }
            else
            {
                uiBtnBuy->loadTextureNormal("Assets/ui_common/btn_upgrade_bg_2.png");
            }
        });
        auto action2 = DelayTime::create(1.0);
        auto seq = Sequence::create(action1, action2, NULL);
        uiBtnBuy->runAction(RepeatForever::create(seq));
    }
    
    // buy purchase
    auto uiBtnBuyPurchase = ui::Button::create(MafUtils::format("Assets/ui/mine/ore_shop_%d.png", (int)eType));
    uiBtnBuyPurchase->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiBtnBuyPurchase->setPosition(Vec2(uiBtnBuy->getPositionX() - uiBtnBuy->getContentSize().width - 5, uiLayout->getContentSize().height / 2));
    uiBtnBuyPurchase->addClickEventListener(CC_CALLBACK_1(UILayoutMine::onClickMineBuyPurchase, this));
    uiBtnBuyPurchase->setZoomScale(0.05f);
    uiBtnBuyPurchase->setTag((int)idx);
    uiLayout->addChild(uiBtnBuyPurchase);
    {
        auto objProduct = CashManager::getInstance()->getProduct(MineralManager::getInstance()->getBuyPurchaseType(eType));
        if ( objProduct != nullptr )
        {
            std::string priceTemp = objProduct->getPrice();
            if ( priceTemp.empty() == true )
            {
                priceTemp = GAME_TEXT("t_ui_shop_price_loading");
            }
            auto uiTextPrice = ui::CText::create(priceTemp, GAME_FONT, 24);
            uiTextPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiTextPrice->setPosition(Vec2(uiBtnBuyPurchase->getContentSize().width / 2, 5));
            uiTextPrice->setTextAreaSize(Size(uiBtnBuyPurchase->getContentSize().width * 0.95, uiTextPrice->getContentSize().height));
            uiTextPrice->setTextOverflow(Label::Overflow::SHRINK);
            uiTextPrice->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiTextPrice->enableOutline(Color4B::BLACK, 2);
            uiBtnBuyPurchase->addChild(uiTextPrice);
        }
        
    }
}

int64_t UILayoutMine::listviewNumberOfCells(ui::CListView *listview)
{
    int64_t count = _listMine.size();
   
    return count;
}
 

#pragma mark - ui
void UILayoutMine::uiContainer()
{
    Size size = getContentSize();
    
    //
    _uiContainerMineInfo = ui::Layout::create();
    _uiContainerMineInfo->setContentSize( Size(size.width - 12, 100) );
    _uiContainerMineInfo->setIgnoreAnchorPointForPosition(false);
    _uiContainerMineInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerMineInfo->setPosition( Vec2(size.width / 2, size.height) );
    _uiContainerMineInfo->setClippingEnabled(true);
    addChild(_uiContainerMineInfo);
    
    _uiContainerList = ui::Layout::create();
    _uiContainerList->setContentSize( Size(size.width - 12, size.height - _uiContainerMineInfo->getContentSize().height) );
    _uiContainerList->setIgnoreAnchorPointForPosition(false);
    _uiContainerList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerList->setPosition( Vec2(size.width / 2, _uiContainerMineInfo->getPositionY() - _uiContainerMineInfo->getContentSize().height) );
    addChild(_uiContainerList);
}

void UILayoutMine::uiMineInfo()
{
    _uiContainerMineInfo->removeAllChildren();
    
    auto layout = _uiContainerMineInfo;
    
    //
    auto uiImgBG = ui::ImageView::create("Assets/ui/mine/mine_bg.png");
    uiImgBG->setAnchorPoint(Vec2::ZERO);
    layout->addChild(uiImgBG);
   
    
    // pet
    auto animationIcon = Animation::create();
    animationIcon->setDelayPerUnit(0.15);
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = MafUtils::format("Assets/character_pet/normal/mole/pet_mole_attack_%d.png",i);
        animationIcon->addSpriteFrameWithFile(strTemp);
    }
    
    _uiImgPet = ui::ImageView::create("Assets/character_pet/normal/mole/pet_mole_attack_1.png");
    _uiImgPet->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiImgPet->setPosition(Vec2(getContentSize().width * 0.45 + 10, -30));
    _uiImgPet->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animationIcon)));
    layout->addChild(_uiImgPet);
    
    // upgrade
    auto uiLayout = ui::Layout::create();
    uiLayout->setLayoutType(ui::Layout::Type::HORIZONTAL_RIGHT);
    uiLayout->setContentSize(Size(layout->getContentSize()));
    layout->addChild(uiLayout);
 
    // info btn
    _uiBtnArtifactUpgrade = ui::Button::create("Assets/ui_common/btn_1_1_on.png", "Assets/ui_common/btn_1_1_on.png", "Assets/ui_common/btn_1_off.png");
    _uiBtnArtifactUpgrade->setContentSize(Size(150, 95));
    _uiBtnArtifactUpgrade->setScale9Enabled(true);
    _uiBtnArtifactUpgrade->setCapInsets(Rect(33,33,2,2));
    _uiBtnArtifactUpgrade->setZoomScale(0.05f);
    _uiBtnArtifactUpgrade->addClickEventListener(CC_CALLBACK_1(UILayoutMine::onClickUpgradeArtifactAsk, this));
    uiLayout->addChild(_uiBtnArtifactUpgrade);
    {
        auto params = ui::LinearLayoutParameter::create();
        params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
        params->setMargin(ui::Margin(10, 0, 5, 0));
        _uiBtnArtifactUpgrade->setLayoutParameter(params);
    }

    // info text
    auto uiLayoutInfo = ui::Layout::create();
    uiLayoutInfo->setContentSize(Size(600, uiLayout->getContentSize().height));
    uiLayout->addChild(uiLayoutInfo);
    {
        _uiTextArtifactName = ui::CText::create(GAME_TEXTFORMAT("t_ui_pickaxe", 0), GAME_FONT, 24);
        _uiTextArtifactName->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        _uiTextArtifactName->setPosition(Vec2(uiLayoutInfo->getContentSize().width, uiLayoutInfo->getContentSize().height));
        _uiTextArtifactName->setTextAreaSize(Size(uiLayoutInfo->getContentSize().width, _uiTextArtifactName->getContentSize().height));
        _uiTextArtifactName->setTextColor(Color4B(255, 210, 28, 255));
        _uiTextArtifactName->setTextVerticalAlignment(TextVAlignment::CENTER);
        _uiTextArtifactName->setTextHorizontalAlignment(TextHAlignment::RIGHT);
        uiLayoutInfo->addChild(_uiTextArtifactName);

        _uiTextArtifactDesc = ui::CText::create(GAME_TEXT("t_pet_d1_contents_mole"), GAME_FONT, 22);
        _uiTextArtifactDesc->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _uiTextArtifactDesc->setPosition(Vec2(uiLayoutInfo->getContentSize().width, uiLayoutInfo->getContentSize().height / 2));
        _uiTextArtifactDesc->setTextAreaSize(Size(uiLayoutInfo->getContentSize().width, _uiTextArtifactDesc->getContentSize().height));
        _uiTextArtifactDesc->setTextVerticalAlignment(TextVAlignment::CENTER);
        _uiTextArtifactDesc->setTextHorizontalAlignment(TextHAlignment::RIGHT);
        uiLayoutInfo->addChild(_uiTextArtifactDesc);

        _uiTextArtifactInfo = ui::CText::create(GAME_TEXTFORMAT("t_arti_d1_option_3", 0), GAME_FONT, 22);
        _uiTextArtifactInfo->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        _uiTextArtifactInfo->setPosition(Vec2(uiLayoutInfo->getContentSize().width, 0));
        _uiTextArtifactInfo->setTextAreaSize(Size(uiLayoutInfo->getContentSize().width, _uiTextArtifactInfo->getContentSize().height));
        _uiTextArtifactInfo->setTextVerticalAlignment(TextVAlignment::CENTER);
        _uiTextArtifactInfo->setTextHorizontalAlignment(TextHAlignment::RIGHT);
        uiLayoutInfo->addChild(_uiTextArtifactInfo);
    }
     
    // draw
    drawMinePet();
    drawMineInfo();
}

void UILayoutMine::uiList()
{
    _uiContainerList->removeAllChildren();
    
    auto layout = _uiContainerList;
    
    //
    _uiList = ui::CListView::create(this, Size(layout->getContentSize().width, layout->getContentSize().height), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(Vec2(layout->getContentSize().width / 2, 0));
    _uiList->setBounceEnabled(true);
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    layout->addChild(_uiList);
     
}

#pragma mark - draw
void UILayoutMine::drawMinePet()
{
    bool bSkin = false;
    std::string strPath = "Assets/character_pet/normal/";
    if ( PetNewManager::getInstance()->getPetSkin(E_PET::MOLE_7) > 0 )
    {
        std::string strTemp = strPath;
        strTemp.append( MafUtils::format("mole_skin_%d", PetNewManager::getInstance()->getPetSkin(E_PET::MOLE_7)) );
        
        if ( FileUtils::getInstance()->isDirectoryExist(strTemp) == true )
        {
            bSkin = true;
            strPath = strTemp;
            strPath.append("/");
        }
    }
    
    if ( bSkin == false )
    {
        if ( PetNewManager::getInstance()->getPetEvolution(E_PET::MOLE_7) > 0 )
        {
            bSkin = true;
            strPath.append("mole_evolution/");
        }
    }
    
    if ( bSkin == false )
    {
        return;
    }
    
    //
    auto animationIcon = Animation::create();
    animationIcon->setDelayPerUnit(0.15);
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_mole_attack_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        animationIcon->addSpriteFrameWithFile(strTemp);
    }
    _uiImgPet->getVirtualRenderer()->stopAllActions();
    _uiImgPet->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animationIcon)));
}

void UILayoutMine::drawMineInfo()
{
    auto objInfo = DataManager::GetArtifactInfo(_eArtifact);
    
    int level = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _eArtifact);
    int levelMax = objInfo->getLevelMax();
    
    int reinforce = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, _eArtifact);
    
    double effect = DataManager::GetArtifactEffect(_eArtifact, level, reinforce);
    double effectNext = DataManager::GetArtifactEffect(_eArtifact, level+1, reinforce);
    effectNext -= effect;
    
    double effectReal = MineralManager::getInstance()->getPickaxEffect();
    
    //
    _uiTextArtifactName->setString(GAME_TEXTFORMAT("t_ui_pickaxe", level));
    _uiTextArtifactInfo->setString(GAME_TEXTFORMAT("t_arti_d1_option_3", effectReal));
    
    //
    _uiBtnArtifactUpgrade->removeAllChildren();
    if ( level >= levelMax )
    {
        _uiBtnArtifactUpgrade->setEnabled(false);
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_max"), GAME_FONT, 24);
        uiText->setPosition(Vec2(_uiBtnArtifactUpgrade->getContentSize().width / 2, _uiBtnArtifactUpgrade->getContentSize().height / 2));
        _uiBtnArtifactUpgrade->addChild(uiText);
    }
    else
    {
        _uiBtnArtifactUpgrade->setEnabled(true);
        
        std::string price = MafUtils::doubleToString(DataManager::GetArtifactUpgradeKey(_ePlace, _eArtifact, level));
        
        auto uiImgIcon = ui::ImageView::create("Assets/icon/icon_jewel.png");
        uiImgIcon->setPosition(Vec2(_uiBtnArtifactUpgrade->getContentSize().width * 0.175, _uiBtnArtifactUpgrade->getContentSize().height * 0.25));
        _uiBtnArtifactUpgrade->addChild(uiImgIcon);
        
        auto uiTextPrice = ui::CText::createWithBMFont(price, GAME_BM_FONT_AL, 30);
        uiTextPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiTextPrice->setPosition(Vec2(_uiBtnArtifactUpgrade->getContentSize().width * 0.350, _uiBtnArtifactUpgrade->getContentSize().height * 0.25));
        uiTextPrice->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiTextPrice->setColor(COLOR_COIN);
        uiTextPrice->setTextAreaSize(Size(85, 25));
        uiTextPrice->setTextOverflow(Label::Overflow::SHRINK);
        _uiBtnArtifactUpgrade->addChild(uiTextPrice);
        
        auto uiTextEffect = ui::CText::createWithBMFont(MafUtils::format("+%.0lf", effectNext), GAME_BM_FONT_AL, 24);
        uiTextEffect->setAnchorPoint(Vec2(0.5,1));
        uiTextEffect->setPosition(Vec2(_uiBtnArtifactUpgrade->getContentSize().width*0.5, _uiBtnArtifactUpgrade->getContentSize().height*0.8));
        _uiBtnArtifactUpgrade->addChild(uiTextEffect);
    }
}

void UILayoutMine::drawList()
{
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}

#pragma mark - set, get

#pragma mark - callback
void UILayoutMine::callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    //
    PopupLoading::hide();
    
    //
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        PopupLoading::show();
        SaveManager::getInstance()->requestSave([=](bool bResult){
            PopupLoading::hide();
        });
        
        //
        drawList();
        
        // alert
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_cash_msg_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    else if ( eResult == MafGooglePlay::FAIL )
    {
        // alert
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_29"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    else if ( eResult == MafGooglePlay::CANCEL && nResult != -1 )
    {
        /*
         0 # 더이상 구매 불가능
         1 # 성공
         2 # 이벤트 중이 아닙니다. ( 복귀자 )
         3 # 판매 시작한 상품이 아닙니다.
         4 # 기간별 판매이면서 종료시간이 없음 # 주간판매 or 일간판매이면서 간격정보없음
         5 # 샵데이터가 없습니다.
         6 # 판매 종료된 상품입니다.
         7 # 조건이 맞지 않습니다. (뿌리링 선행 상품구매 x)
         8 # 서버 점검중..
         */
        std::string strMessageKey = "t_ui_error_30";
        switch (nResult) {
            case 0: strMessageKey = "t_ui_cash_msg_9_0";    break;
            case 2: strMessageKey = "t_ui_cash_msg_9_2";    break;
            case 3: strMessageKey = "t_ui_cash_msg_9_3";    break;
            case 6: strMessageKey = "t_ui_cash_msg_9_3";    break;
            case 8: strMessageKey = "t_ui_cash_msg_9_8";    break;
                
            default:
                break;
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
}

#pragma mark - click
void UILayoutMine::onClickUpgradeArtifactAsk(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    if ( WeaponManager::getInstance()->getEquipHigh() < E_WEAPON::WEAPON_76_MINE )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_5"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto objInfo = DataManager::GetArtifactInfo(_eArtifact);
    
    std::string strTitle = objInfo->getTitle();
    std::string strMessage = GAME_TEXT("t_ui_msg_buy");
    
    auto popup = PopupDefault::create(strTitle, strMessage);
    popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_yes"), false, CC_CALLBACK_1(UILayoutMine::onClickUpgradeArtifact, this));
    popup->show();
}

void UILayoutMine::onClickUpgradeArtifact(Ref* sender)
{
    auto objInfo = DataManager::GetArtifactInfo(_eArtifact);
    
    int level = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _eArtifact);
    if ( level >= objInfo->getLevelMax() )
    {
        return;
    }

    if( objInfo->getPriceType() == E_ITEMS::GEM || objInfo->getPriceType() == E_ITEMS::POINT )
    {
        auto nowCurrency = 0;
        if ( objInfo->getPriceType() == E_ITEMS::GEM )
            nowCurrency = ItemsMoneyManager::getInstance()->getGem();
        else if ( objInfo->getPriceType() == E_ITEMS::POINT )
            nowCurrency = ItemsMoneyManager::getInstance()->getPoint();
        
        auto needCurrency = DataManager::GetArtifactUpgradeKey(_ePlace, _eArtifact, level);
        if ( nowCurrency < needCurrency )
        {
            if ( objInfo->getPriceType() == E_ITEMS::GEM )
            {
                auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
                popup->show();
            }
            else if ( objInfo->getPriceType() == E_ITEMS::POINT )
            {
                auto popup = PopupCurrencyDetail::create(E_ITEMS::POINT);
                popup->show();
            }
            return;
        }
        
        level ++;
        if ( level >= objInfo->getLevelMax() )
        {
            level = objInfo->getLevelMax();
        }
        
        // 재화 감소
        if ( objInfo->getPriceType() == E_ITEMS::GEM )
            ItemsMoneyManager::getInstance()->setGem(-needCurrency);
        else if ( objInfo->getPriceType() == E_ITEMS::POINT )
            ItemsMoneyManager::getInstance()->setPoint(-needCurrency);
        
        // 레벨 적용
        UserInfoManager::getInstance()->setArtifactLevel(_ePlace, _eArtifact, level);
    }
      
    //
    drawMinePet();
    drawMineInfo();
    
    // achievement
    auto objAchieve = AchievementManager::getInstance()->getObjAchieveNormal(_eArtifact);
    if ( objAchieve != nullptr )
        AchievementManager::getInstance()->setAchieveNormalMission(objAchieve->getNormalMission(), UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _eArtifact));
}

void UILayoutMine::onClickMineSpecialChange(Ref* sender)
{
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    E_MINERAL eType = _listMine.at(uiBtn->getTag());
    
    if ( eType < E_MINERAL::MINERAL_7_SAPPHIRE)
    {
        return;
    }
    
    if ( _eMineralSpecial == E_MINERAL::MINERAL_11_AMETHYST )
    {
        _eMineralSpecial = E_MINERAL::MINERAL_7_SAPPHIRE;
    }
    else
    {
        _eMineralSpecial = (E_MINERAL)(_eMineralSpecial+1);
    }
    MineralManager::getInstance()->setSpecialMineral(_eMineralSpecial);
    
    //
    drawList();
}

void UILayoutMine::onClickMineBuy(Ref* sender)
{
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    auto uiLayout = dynamic_cast<ui::Layout*>(uiBtn->getParent());
    auto uiTextMineralCount = uiLayout->getChildByName<ui::CText*>("UI_COUNT");
    
    E_MINERAL eType = _listMine.at(uiBtn->getTag());
    if ( eType >= E_MINERAL::MINERAL_7_SAPPHIRE )
    {
        eType = _eMineralSpecial;
    }
    
    int buyAdd = MineralManager::getInstance()->getBuyAdd(eType);
    
    /*
     0 : 성공
     1 : 가격 부족
     2 : 구매 제한
     */
    int result = MineralManager::getInstance()->onBuy(eType);
    if ( result != 0 )
    {
        if ( result == 1 )
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
            popup->show();
        }
        else if ( result == 2 )
        {
            auto objPass = PassManager::getInstance()->getPassInfo(E_PASS_TYPE::PASS_DUNGEON);
            if ( objPass != nullptr && objPass->isBuyPass() == false )
            {
                Vec2 posToast = uiBtn->getParent()->convertToWorldSpace(uiBtn->getPosition());
                posToast.x -= uiBtn->getContentSize().width / 2;
                posToast.y += uiBtn->getContentSize().height / 4;
                posToast.y += 10;
                    
                PopupToast::showImmediately(GAME_TEXT("t_ui_dungeonpass_v2_6"), posToast.x, posToast.y);
            }
        }
        return;
    }
    
    // effect
    {
        Vec2 posToast = uiTextMineralCount->getParent()->convertToWorldSpace(uiTextMineralCount->getPosition());
        posToast.x += uiTextMineralCount->getContentSize().width + 50;
        posToast.y += uiTextMineralCount->getContentSize().height;
        
        auto uiTextAdd = ui::CText::create(MafUtils::format("+%d", buyAdd), GAME_FONT_AL, 28);
        uiTextAdd->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiTextAdd->setPosition(posToast);
        UtilsScene::getInstance()->getRunningScene()->addChild(uiTextAdd, ZORDER_SCENE_TOAST);
        
        auto seq = Sequence::create(FadeOut::create(0.5f), RemoveSelf::create(), nullptr);
        uiTextAdd->runAction(seq);
    }
    
    // draw
    drawList();
}

void UILayoutMine::onClickMineBuyPurchase(cocos2d::Ref* sender)
{
    auto uiBtn = dynamic_cast<ui::Button*>(sender);

    E_MINERAL eType = _listMine.at(uiBtn->getTag());
    if ( eType >= E_MINERAL::MINERAL_7_SAPPHIRE )
    {
        eType = _eMineralSpecial;
    }
    
    /*
    0 : 오류 없음
    1 : 다른상품 결제 오류 있음.
    2 : 같은상품 결제 오류 있음.
    */
    E_CASH eCash = MineralManager::getInstance()->getBuyPurchaseType(eType);
    CashManager::getInstance()->purchaseHas(eCash, [=](int nHas){
        if ( nHas == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate((int)eCash, CC_CALLBACK_4(UILayoutMine::callbackPurchase, this));
            });
            popup->show();
        }
        else if ( nHas == 1 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_46"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), false, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
        else
        {
            auto popup = PopupPurchaseBuy::create(eCash);
            popup->setCallbackResult([=](int idx){
                
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(idx, CC_CALLBACK_4(UILayoutMine::callbackPurchase, this));
            });
            popup->show();
        }
    });
}
