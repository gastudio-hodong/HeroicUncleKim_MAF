//
//  UILayoutMineRefine.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "UILayoutMineRefine.h"

#include "GameObject/InfoSetting.h"
#include "GameObject/Mineral/InfoMineralRefineUpgrade.h"

#include "GameUIPopup/Other/SubFunction/Mine/UILayoutMineRefineOpen.h"
#include "GameUIPopup/Other/SubFunction/Mine/PopupMineRefineProduction.h"
#include "GameUIPopup/Other/SubFunction/Mine/PopupMineRefineUpgrade.h"
#include "GameUIPopup/Other/SubFunction/Mine/PopupMineRefineShop.h"

#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/MineralManager.h"
#include "ManagerGame/PetNewManager.h"

USING_NS_CC;

UILayoutMineRefine* UILayoutMineRefine::create()
{
    UILayoutMineRefine* pRet = new(std::nothrow) UILayoutMineRefine();
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

UILayoutMineRefine::UILayoutMineRefine():
_bInitUI(false),

_ePlace(E_PLACE::EMPTY),

//
_uiContainerInfo(nullptr),
_uiContainerList(nullptr),

_uiList(nullptr),
_uiPet(nullptr)
{
    
}

bool UILayoutMineRefine::init()
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
        
    //
    initVar();
    initUI();
    
    //
    RefreshManager::getInstance()->addUI(E_REFRESH::MINE_REFINE_UPGRADE, this);
    RefreshManager::getInstance()->addUI(E_REFRESH::MINE_REFINE_PRODUCTION, this);
    
    return true;
}


void UILayoutMineRefine::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        //
        uiContainer();
        uiInfo();
        uiList();
        
        if ( MineralManager::getInstance()->getRefineOpen() <= 0 )
        {
            auto uiOpen = UILayoutMineRefineOpen::create();
            uiOpen->setContentSize(getContentSize());
            addChild(uiOpen);
        }
        
        //
        while (true) {
            int result = MineralManager::getInstance()->onRefineProductionUpdate();
            if ( result != 0 )
            {
                break;
            }
        }
        
        _bInitUI = true;
    }
    
    // draw
    drawInfo();
    drawInfoPet();
    drawList();
}

#pragma mark - refresh
void UILayoutMineRefine::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::MINE_REFINE_UPGRADE || eType == E_REFRESH::MINE_REFINE_PRODUCTION )
    {
        drawInfo();
        drawList();
    }
}

#pragma mark - override DelegateListView
void UILayoutMineRefine::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size UILayoutMineRefine::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 155;

    return Size(width, height);
}

void UILayoutMineRefine::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    Size sizeLayout = Size(size.width, 150);

    // data
    auto obj = _listInfoRefineUpgrade.at(idx);
    
    int level = MineralManager::getInstance()->getRefineUpgradeLevel(obj->getIdx());
    int levelMax = obj->getLevelMax();
    bool bLevelMax = level >= levelMax ? true : false;
    
    double effect = MineralManager::getInstance()->getRefineUpgradeEffect(obj->getIdx());
    
    std::string pathIcon = obj->getIconPath();
    
    std::string nameText = obj->getName();
    nameText.append(MafUtils::format(" [Lv.%d]", level));
    
    std::string infoText = obj->getDesc();
    infoText = MafUtils::format(infoText.c_str(), effect);
    
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(sizeLayout.width, sizeLayout.height));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    childview->addChild(uiLayout);
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/list_bg_1_s.png");
    uiBG->setScale9Enabled(true);
    uiBG->setCapInsets(Rect(0,51,150,10));
    uiBG->setContentSize(uiLayout->getContentSize());
    uiBG->setPosition(Vec2(uiLayout->getContentSize().width / 2, uiLayout->getContentSize().height / 2));
    uiLayout->addChild(uiBG);
     
    auto uiIconBG = ui::Button::create("Assets/ui/mine/smelt_item_box.png");
    uiIconBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiIconBG->setPosition(Vec2(3, sizeLayout.height / 2));
    uiLayout->addChild(uiIconBG);
    {
        auto uiIcon = ui::ImageView::create(pathIcon);
        uiIcon->setPosition(Vec2(uiIconBG->getContentSize().width / 2, uiIconBG->getContentSize().height / 2));
        uiIconBG->addChild(uiIcon);
    }
    
    double posTextX = uiIconBG->getPositionX() + uiIconBG->getContentSize().width + 10;
    
    auto uiName = ui::CText::create(nameText, GAME_FONT, 26);
    uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiName->setPosition(Vec2(posTextX, sizeLayout.height - 25));
    uiName->setTextColor(Color4B(COLOR_COIN));
    uiLayout->addChild(uiName);
    
    auto uiInfo = ui::CText::create(infoText, GAME_FONT, 22);
    uiInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiInfo->setPosition(Vec2(posTextX, 50));
    uiInfo->setTextAreaSize(Size(sizeLayout.width - 200 - posTextX, uiInfo->getContentSize().height));
    uiInfo->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiInfo->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiInfo->setTextOverflow(Label::Overflow::SHRINK);
    uiInfo->setLineSpacing(-10);
    uiLayout->addChild(uiInfo);
    
    {
        std::string pathBtn = "Assets/ui_common/btn_upgrade_bg_2.png";
        if ( bLevelMax == true )
        {
            pathBtn = "Assets/ui_common/btn_upgrade_bg_2_max.png";
        }
        
        auto uiLevelUp = ui::Button::create(pathBtn);
        uiLevelUp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiLevelUp->setPosition(Vec2(sizeLayout.width, sizeLayout.height / 2));
        uiLevelUp->addClickEventListener(CC_CALLBACK_1(UILayoutMineRefine::onClickUpgradeLevelUp, this));
        uiLevelUp->setTag((int)idx);
        uiLayout->addChild(uiLevelUp);
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_raid_p_lvup_bt"), GAME_FONT, 22);
        uiText->setPosition(Vec2(uiLevelUp->getContentSize().width / 2, uiLevelUp->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(uiLevelUp->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiLevelUp->addChild(uiText);
    }
    
}

int64_t UILayoutMineRefine::listviewNumberOfCells(ui::CListView *listview)
{
    return _listInfoRefineUpgrade.size();
}

#pragma mark - init
void UILayoutMineRefine::initVar()
{
    _ePlace = E_PLACE::DG_NORMAL;
    
    _listInfoRefineUpgrade = MineralManager::getInstance()->getInfoRefineUpgradeAll();
}

void UILayoutMineRefine::initUI()
{
    
}

#pragma mark - ui
void UILayoutMineRefine::uiContainer()
{
    Size size = getContentSize();
    
    //
    _uiContainerInfo = ui::Layout::create();
    _uiContainerInfo->setContentSize( Size(size.width, 495) );
    _uiContainerInfo->setIgnoreAnchorPointForPosition(false);
    _uiContainerInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerInfo->setPosition( Vec2(size.width / 2, size.height) );
    _uiContainerInfo->setClippingEnabled(true);
    addChild(_uiContainerInfo);
    
    _uiContainerList = ui::Layout::create();
    _uiContainerList->setContentSize( Size(size.width, size.height - _uiContainerInfo->getContentSize().height) );
    _uiContainerList->setIgnoreAnchorPointForPosition(false);
    _uiContainerList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContainerList->setPosition( Vec2(size.width / 2, 0) );
    addChild(_uiContainerList);
}
 
void UILayoutMineRefine::uiInfo()
{
    auto layout = _uiContainerInfo;
    auto size = layout->getContentSize();
    
    auto uiTopBG = ui::ImageView::create("Assets/ui/mine/mine_bg2.png");
    uiTopBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiTopBG->setPosition(Vec2(size.width / 2, size.height));
    layout->addChild(uiTopBG);
    {
        // pet
        auto animationIcon = Animation::create();
        animationIcon->setDelayPerUnit(0.15);
        for ( int i = 1; i <= 4; i++ )
        {
            std::string strTemp = MafUtils::format("Assets/character_pet/normal/mole/pet_mole_attack_%d.png",i);
            animationIcon->addSpriteFrameWithFile(strTemp);
        }
        
        _uiPet = ui::ImageView::create("Assets/character_pet/normal/mole/pet_mole_attack_1.png");
        _uiPet->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _uiPet->setPosition(Vec2(uiTopBG->getContentSize().width * 0.45 + 10, -28));
        _uiPet->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animationIcon)));
        uiTopBG->addChild(_uiPet);
        
        {
            auto animationIcon = Animation::create();
            animationIcon->setDelayPerUnit(0.15);
            for ( int i = 1; i <= 4; i++ )
            {
                std::string strTemp = MafUtils::format("Assets/ui/mine/mole_smith_idle_%d.png",i);
                animationIcon->addSpriteFrameWithFile(strTemp);
            }
            
            auto uiPetSmith = ui::ImageView::create("Assets/ui/mine/mole_smith_idle_1.png");
            uiPetSmith->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiPetSmith->setPosition(Vec2(uiTopBG->getContentSize().width * 0.6, 6));
            uiPetSmith->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animationIcon)));
            uiTopBG->addChild(uiPetSmith);
        }

        auto uiShop = ui::Button::create("Assets/ui_common/btn_1_8_on.png");
        uiShop->setScale9Enabled(true);
        uiShop->setContentSize(Size(150, 95));
        uiShop->setCapInsets(Rect(33,33,2,2));
        uiShop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiShop->setPosition(Vec2(uiTopBG->getContentSize().width - 10, uiTopBG->getContentSize().height / 2));
        uiShop->addClickEventListener(CC_CALLBACK_1(UILayoutMineRefine::onClickShop, this));
        uiTopBG->addChild(uiShop);
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_mine_mk2_buy_bt"), GAME_FONT, 24);
        uiText->setPosition(Vec2(uiShop->getContentSize().width / 2, uiShop->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(uiShop->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiShop->addChild(uiText);
    }
    
    auto uiInfoBG = ui::ImageView::create("Assets/ui_common/popup_mini_scale2_8.png");
    uiInfoBG->setScale9Enabled(true);
    uiInfoBG->setContentSize(Size(668, 334));
    uiInfoBG->setCapInsets(Rect(40, 70, 10, 10));
    uiInfoBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiInfoBG->setPosition(Vec2(size.width / 2, size.height - 120));
    layout->addChild(uiInfoBG);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_mine_mk2_msg_1"), GAME_FONT, 24);
        uiText->setPosition(Vec2(uiInfoBG->getContentSize().width / 2, uiInfoBG->getContentSize().height - 27.5));
        uiText->setTextAreaSize(Size(uiInfoBG->getContentSize().width - 100, uiText->getContentSize().height));
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiInfoBG->addChild(uiText);
        
        auto uiInfoLine = ui::ImageView::create("Assets/ui_common/popup_mini_line1_1.png");
        uiInfoLine->setScale9Enabled(true);
        uiInfoLine->setContentSize(Size(3, 264));
        uiInfoLine->setPosition(Vec2(uiInfoBG->getContentSize().width / 2, (uiInfoBG->getContentSize().height - 50) / 2));
        uiInfoBG->addChild(uiInfoLine);
    }
    
    auto uiInfoInL = ui::Layout::create();
    uiInfoInL->setContentSize(Size((uiInfoBG->getContentSize().width - 42) / 2, uiInfoBG->getContentSize().height - 52 - 21));
    uiInfoInL->setIgnoreAnchorPointForPosition(false);
    uiInfoInL->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    uiInfoInL->setPosition(Vec2(uiInfoBG->getContentSize().width / 2, 21));
    uiInfoInL->setName("UI_INFO_L");
    uiInfoBG->addChild(uiInfoInL);
    {
        double posX = uiInfoInL->getContentSize().width * 0.5;
        double posY = uiInfoInL->getContentSize().height;
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_mine_mk2_msg_2"), GAME_FONT, 28);
        uiText->setPosition(Vec2(posX, posY - 30));
        uiText->setTextColor(Color4B(246, 203, 28, 255));
        uiText->setName("UI_PROGRESS_NAME");
        uiInfoInL->addChild(uiText);
        
        auto uiAniEffect = ui::ImageView::create("Assets/ui/mine/smelt_2_base.png");
        uiAniEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiAniEffect->setPosition(Vec2(posX, posY - 150));
        uiAniEffect->setVisible(false);
        uiAniEffect->setName("UI_ANI_EFFECT");
        uiInfoInL->addChild(uiAniEffect);
        
        auto uiAni = ui::ImageView::create("Assets/ui/mine/smelt_1_1.png");
        uiAni->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiAni->setPosition(Vec2(posX, posY - 150));
        ((ui::Scale9Sprite*)uiAni->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiAni->setScale(3);
        uiAni->setName("UI_ANI");
        uiInfoInL->addChild(uiAni);
        
        auto uiBarBG = ui::ImageView::create("Assets/ui/mine/smelt_progressbar_1_1.png");
        uiBarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiBarBG->setPosition(Vec2(posX, posY - 150));
        uiBarBG->setName("UI_PROGRESS_BG");
        uiInfoInL->addChild(uiBarBG);
        {
            auto uiBar = ui::LoadingBar::create("Assets/ui/mine/smelt_progressbar_1_2.png");
            uiBar->setPosition(Vec2(uiBarBG->getContentSize().width / 2, uiBarBG->getContentSize().height / 2));
            uiBar->setPercent(0);
            uiBar->setName("UI_PROGRESS");
            uiBarBG->addChild(uiBar);
            
            auto uiTime = ui::CText::create("00:00:00", GAME_FONT_AL, 28);
            uiTime->setPosition(Vec2(uiBarBG->getContentSize().width / 2, uiBarBG->getContentSize().height / 2));
            uiTime->setName("UI_PROGRESS_TIME");
            uiBarBG->addChild(uiTime);
        }
        
        auto uiBtnProduction = ui::Button::create("Assets/ui_common/btn_2_4_on.png");
        uiBtnProduction->setScale9Enabled(true);
        uiBtnProduction->setContentSize(Size(226, 70));
        uiBtnProduction->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiBtnProduction->setPosition(Vec2(posX, 0));
        uiBtnProduction->addClickEventListener(CC_CALLBACK_1(UILayoutMineRefine::onClickItemProduction, this));
        uiBtnProduction->setName("UI_PRODUCTION");
        uiInfoInL->addChild(uiBtnProduction);
        {
            auto text = MafUtils::format("%d/%d", 0, 0);
            text.append("\n");
            text.append(GAME_TEXT("t_ui_mine_mk2_production_bt_1"));
            
            auto uiText = ui::CText::create(text, GAME_FONT, 26);
            uiText->setPosition(Vec2(uiBtnProduction->getContentSize().width / 2, uiBtnProduction->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiBtnProduction->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setLineSpacing(-10);
            uiText->setName("UI_TEXT");
            uiBtnProduction->addChild(uiText);
        }
        
    }
    
    auto uiInfoInR = ui::Layout::create();
    uiInfoInR->setContentSize(Size((uiInfoBG->getContentSize().width - 42) / 2, uiInfoBG->getContentSize().height - 52 - 21));
    uiInfoInR->setIgnoreAnchorPointForPosition(false);
    uiInfoInR->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiInfoInR->setPosition(Vec2(uiInfoBG->getContentSize().width / 2, 21));
    uiInfoInR->setName("UI_INFO_R");
    uiInfoBG->addChild(uiInfoInR);
    {
        double posX = uiInfoInR->getContentSize().width * 0.5;
        double posY = uiInfoInR->getContentSize().height;
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_mine_mk2_msg_4"), GAME_FONT, 28);
        uiText->setPosition(Vec2(posX, posY - 30));
        uiText->setTextColor(Color4B(246, 203, 28, 255));
        uiInfoInR->addChild(uiText);
        
        auto uiItemBG = ui::ImageView::create("Assets/ui_common/box_04.png");
        uiItemBG->setScale9Enabled(true);
        uiItemBG->setContentSize(Size(116, 116));
        uiItemBG->setPosition(Vec2(posX, posY - 120));
        uiInfoInR->addChild(uiItemBG);
        {
            auto objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::MYTHRIL);
            
            auto uiItem = ui::ImageView::create(objItem->getPath());
            uiItem->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2 + 5));
            uiItem->setScale(1.5);
            ((ui::Scale9Sprite*)uiItem->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiItemBG->addChild(uiItem);
            
            auto uiItemCount = ui::CText::create("0", GAME_FONT, 28);
            uiItemCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiItemCount->setPosition(Vec2(uiItemBG->getContentSize().width / 2, 5));
            uiItemCount->enableOutline(Color4B::BLACK, 2);
            uiItemCount->setName("UI_ITEM_COUNT");
            uiItemBG->addChild(uiItemCount);
        }
        
        auto uiBtnReward = ui::Button::create("Assets/ui_common/btn_2_4_on.png");
        uiBtnReward->setScale9Enabled(true);
        uiBtnReward->setContentSize(Size(226, 70));
        uiBtnReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiBtnReward->setPosition(Vec2(posX, 0));
        uiBtnReward->addClickEventListener(CC_CALLBACK_1(UILayoutMineRefine::onClickItemReward, this));
        uiBtnReward->setName("UI_REWARD");
        uiInfoInR->addChild(uiBtnReward);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_mine_mk2_production_bt_3"), GAME_FONT, 26);
            uiText->setPosition(Vec2(uiBtnReward->getContentSize().width / 2, uiBtnReward->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiBtnReward->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiBtnReward->addChild(uiText);
        }
    }
    
}

void UILayoutMineRefine::uiList()
{
    auto layout = _uiContainerList;
    auto size = layout->getContentSize();
    
    _uiList = ui::CListView::create(this, Size(size.width, size.height), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(Vec2(size.width / 2, 0));
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    layout->addChild(_uiList);
}

#pragma mark - draw
void UILayoutMineRefine::drawInfo()
{
    auto layout = _uiContainerInfo;
    
    int itemNow = MineralManager::getInstance()->getRefineProductionItem();
    int itemMax = MineralManager::getInstance()->getRefineProductionItemMax();
    
    if ( auto uiInfoIn = (ui::Layout*)utils::findChild(layout, "UI_INFO_L"); uiInfoIn != nullptr )
    {
        int count = MineralManager::getInstance()->getRefineProductionCount();
        int countMax = MineralManager::getInstance()->getRefineProductionCountMax();
        
        //
        auto uiProgressName = uiInfoIn->getChildByName<ui::CText*>("UI_PROGRESS_NAME");
        if ( count > 0 )
        {
            uiProgressName->setString(GAME_TEXT("t_ui_mine_mk2_msg_3"));
        }
        else
        {
            uiProgressName->setString(GAME_TEXT("t_ui_mine_mk2_msg_2"));
        }
        
        //
        auto uiAniEffect = uiInfoIn->getChildByName<ui::ImageView*>("UI_ANI_EFFECT");
        auto uiAni = uiInfoIn->getChildByName<ui::ImageView*>("UI_ANI");
        if ( count > 0 )
        {
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.15);
            for ( int i = 1; i <= 4; i++ )
            {
                auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/mine/smelt_2_%d.png", i).c_str());
                spriteAni->getTexture()->setTexParameters(texParams);
                animation->addSpriteFrame(spriteAni->getSpriteFrame());
            }
            
            uiAni->getVirtualRenderer()->stopAllActions();
            uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
            
            if ( uiAniEffect != nullptr )
            {
                uiAniEffect->setVisible(true);
            }
        }
        else
        {
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.15);
            for ( int i = 1; i <= 4; i++ )
            {
                auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/mine/smelt_1_%d.png", i).c_str());
                spriteAni->getTexture()->setTexParameters(texParams);
                animation->addSpriteFrame(spriteAni->getSpriteFrame());
            }
            
            uiAni->getVirtualRenderer()->stopAllActions();
            uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
            
            if ( uiAniEffect != nullptr )
            {
                uiAniEffect->setVisible(false);
            }
        }
        
        //
        auto action1 = CallFuncN::create([=](Ref* sender){
            
            auto uiProgressBG = (ui::ImageView*)sender;
            
            int64_t timeNow = UtilsDate::getInstance()->getTime();
            int64_t timeStart = MineralManager::getInstance()->getRefineProductionTimeStart();
            int64_t timeNeed = MineralManager::getInstance()->getRefineProductionTimeNeed();
            int64_t timeDiff = 0;
            
            double percent = 0;
            
            int count = MineralManager::getInstance()->getRefineProductionCount();
            if ( count > 0 )
            {
                timeDiff = (timeStart + timeNeed)  - timeNow;
                if ( timeDiff < 0 )
                {
                    timeDiff = 0;
                    
                    //
                    MineralManager::getInstance()->onRefineProductionUpdate();
                }
                
                percent = (double)timeDiff / timeNeed * 100;
                if ( percent > 100 )
                    percent = 100;
            }
            
            auto uiProgress = uiProgressBG->getChildByName<ui::LoadingBar*>("UI_PROGRESS");
            uiProgress->setPercent(percent);
            
            auto uiProgressTime = uiProgressBG->getChildByName<ui::CText*>("UI_PROGRESS_TIME");
            uiProgressTime->setString(UtilsDate::getInstance()->getTimeString(timeDiff, UtilsDate::date_user_type_1));
            
        });
        auto action2 = DelayTime::create(1.0);
        auto uiProgressBG = uiInfoIn->getChildByName<ui::ImageView*>("UI_PROGRESS_BG");
        uiProgressBG->stopAllActions();
        uiProgressBG->runAction(RepeatForever::create(Sequence::create(action1, action2, NULL)));
        
        //
        auto uiBtnProduction = uiInfoIn->getChildByName<ui::Button*>("UI_PRODUCTION");
        {
            std::string text = MafUtils::format("%d/%d", count, countMax);
            text.append("\n");
            text.append(GAME_TEXT("t_ui_mine_mk2_production_bt_1"));
            
            auto uiText = uiBtnProduction->getChildByName<ui::CText*>("UI_TEXT");
            uiText->setString(text);
            
            if ( itemNow < itemMax )
            {
                uiBtnProduction->loadTextureNormal("Assets/ui_common/btn_2_4_on.png");
            }
            else
            {
                uiBtnProduction->loadTextureNormal("Assets/ui_common/btn_2_4_gray.png");
            }
        }
    }
    
    if ( auto uiInfoIn = (ui::Layout*)utils::findChild(layout, "UI_INFO_R"); uiInfoIn != nullptr )
    {
        auto uiItemCount = (ui::CText*)utils::findChild(uiInfoIn, "UI_ITEM_COUNT");
        uiItemCount->setString(MafUtils::toString(itemNow));
        
        //
        auto uiBtnReward = uiInfoIn->getChildByName<ui::Button*>("UI_REWARD");
        {
            if ( itemNow > 0 )
            {
                uiBtnReward->loadTextureNormal("Assets/ui_common/btn_2_4_on.png");
            }
            else
            {
                uiBtnReward->loadTextureNormal("Assets/ui_common/btn_2_4_gray.png");
            }
        }
    }
}

void UILayoutMineRefine::drawInfoPet()
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
        std::string strTemp = strPath + "pet_mole_walk_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        animationIcon->addSpriteFrameWithFile(strTemp);
    }
    _uiPet->getVirtualRenderer()->stopAllActions();
    _uiPet->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animationIcon)));
}

void UILayoutMineRefine::drawList()
{
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}

#pragma mark - set, get

#pragma mark - callback
#pragma mark - click
void UILayoutMineRefine::onClickItemProduction(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    int itemNow = MineralManager::getInstance()->getRefineProductionItem();
    int itemMax = MineralManager::getInstance()->getRefineProductionItemMax();
    if ( itemNow >= itemMax )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_mine_mk2_mine_max_msg"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto popup = PopupMineRefineProduction::create();
    popup->show();
}

void UILayoutMineRefine::onClickItemReward(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    int itemNow = MineralManager::getInstance()->getRefineProductionItem();
    if ( itemNow <= 0 )
    {
        return;
    }
    
    int result = MineralManager::getInstance()->onRefineProductionReward();
    if ( result != 0 )
    {
        return;
    }
}

void UILayoutMineRefine::onClickUpgradeLevelUp(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto obj = _listInfoRefineUpgrade.at(uiBtn->getTag());
    if ( obj == nullptr )
    {
        return;
    }
    
    int level = 0;
    int levelMax = obj->getLevelMax();
    if ( level >= levelMax )
    {
        return;
    }
    
    auto popup = PopupMineRefineUpgrade::create();
    popup->setUpgradeIdx(obj->getIdx());
    popup->show();
}

void UILayoutMineRefine::onClickShop(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto popup = PopupMineRefineShop::create();
    popup->show();
}
