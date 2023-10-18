//
//  UILayoutMyHome.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "UILayoutMyHome.h"

#include "GameUIPopup/Other/Ads/PopupAdsWatch.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/SubFunction/MyHome/UILayoutShopAds.h"
#include "GameUIPopup/Other/SubFunction/MyHome/UILayoutBoost.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/ServerMissionManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/StoryManager.h"
#include "ManagerGame/AchievementManager.h"
#include "ManagerGame/MineralManager.h"

USING_NS_CC;

UILayoutMyHome* UILayoutMyHome::create()
{
    UILayoutMyHome* pRet = new(std::nothrow) UILayoutMyHome();
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

UILayoutMyHome::UILayoutMyHome():
_ePlace(E_PLACE::EMPTY),

//
_uiContainerShopAds(nullptr),
_uiContainerBoost(nullptr),
_uiContainerList(nullptr),

_uiList(nullptr)
{
    
}

bool UILayoutMyHome::init()
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


void UILayoutMyHome::onEnter()
{
    ui::Layout::onEnter();
    
    //
    uiContainer();
    uiShopAds();
    uiBoost();
    uiList();
}

#pragma mark - init
void UILayoutMyHome::initVar()
{
    _ePlace = UserInfoManager::getInstance()->getCurrentPlace();
    
    _listMyHome.push_back(E_MYHOME::MYHOME_HONG);       // floor : 5
    _listMyHome.push_back(E_MYHOME::MYHOME_HAMSTER);    // floor : 4
    _listMyHome.push_back(E_MYHOME::MYHOME_CHICK);      // floor : 3
    _listMyHome.push_back(E_MYHOME::MYHOME_PRINCESS);   // floor : 2
    _listMyHome.push_back(E_MYHOME::MYHOME_BONGSIK);    // floor : 1
}

void UILayoutMyHome::initUI()
{
    // size
    double height = 5 * 170 + 135 + 80;
    
    Size size = Size(750, height);
    setContentSize(size);
    setAnchorPoint(Vec2::ZERO);
}

#pragma mark - override DelegateListView
void UILayoutMyHome::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size UILayoutMyHome::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 170;

    return Size(width, height);
}

void UILayoutMyHome::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();

    // data
    auto eType = (E_MYHOME)_listMyHome.at(idx);
    
    int floor = (int)eType;
    int level = UserInfoManager::getInstance()->getMyhomeLevel(floor);
    int levelMax = UserInfoManager::getInstance()->getMyhomeLevelMax();
    
    int time = UserInfoManager::getInstance()->getMyhomeAdsTime(floor);
    
    int adsWatch = UserInfoManager::getInstance()->getMyhomeWatchcount(floor);
    int adsWatchMax = getAdsWatchMax(floor);
    double adsWatchPercent = (double)adsWatch / adsWatchMax * 100;
    if ( adsWatchPercent > 100 )
        adsWatchPercent = 100;

    std::string pathDressRoom = "Assets/ui/myhome/myroom_0.png";
    if ( level > 0 )
    {
        int room = level / 2;
        if ( level % 2 == 1 )
        {
            room = room + 1;
        }

        pathDressRoom = StringUtils::format("Assets/ui/myhome/myroom%d_%d.png", floor, room);
    }
    
    std::string pathTVScreen = "Assets/ui/myhome/btn_myroom_ad.png";
    if ( time > 0 )
    {
        pathTVScreen = "Assets/ui/myhome/btn_myroom_ad2.png";
    }
    else if ( adsWatch >= adsWatchMax )
    {
        pathTVScreen = "Assets/ui/myhome/btn_myroom_ad3.png";
    }
    
    std::string pathItem = "";
    switch (eType) {
        case E_MYHOME::MYHOME_BONGSIK:      pathItem = "Assets/icon/icon_buff1_1.png";      break;
        case E_MYHOME::MYHOME_PRINCESS:     pathItem = "Assets/icon/icon_jewel.png";        break;
        case E_MYHOME::MYHOME_CHICK:        pathItem = "Assets/icon/menu_coin.png";         break;
        case E_MYHOME::MYHOME_HAMSTER:      pathItem = "Assets/icon/icon_mineral_1_2.png";  break;
        case E_MYHOME::MYHOME_HONG:         pathItem = "Assets/icon/menu_cube.png";         break;
            
        default:
            return;
    }
    
    // ui
    childview->removeAllChildren();
    
    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(size.width, size.height - 5));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    uiLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    uiLayout->setBackGroundColor(Color3B(29, 41, 51));
    childview->addChild(uiLayout);
    
    // dress room
    auto uiImgDressRoomBG = ui::ImageView::create(pathDressRoom);
    uiImgDressRoomBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiImgDressRoomBG->setPosition(Vec2(0, uiLayout->getContentSize().height / 2));
    uiLayout->addChild(uiImgDressRoomBG);
    {
        auto uiImgBlock = ui::ImageView::create("Assets/ui/myhome/myroom_bg_tablet.png");
        uiImgBlock->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiImgBlock->setPosition(Vec2(uiImgDressRoomBG->getContentSize().width, uiImgDressRoomBG->getContentSize().height / 2));
        uiImgDressRoomBG->addChild(uiImgBlock);
        
        if ( level > 0 )
        {
            if ( floor == 1 )
            {
                auto hero = UnitBehaviorHero::create(E_PLACE::DG_NORMAL);
                hero->setTalkShow(false);
                hero->setMotion();
                hero->actionRun();
                hero->getLayerCharacter()->setScale(0.75);
                hero->setPosition(Vec2(uiImgDressRoomBG->getContentSize().width/2, 0));
                uiImgDressRoomBG->addChild(hero);
            }
            else
            {
                Pet* pet;
                if ( floor == 2)
                {
                    pet = Princess::create();
                    pet->setScale(0.8);
                }
                else if ( floor == 3 )
                {
                    pet = Chick::create();
                }
                else if ( floor == 4 )
                {
                    pet = Hamster::create();
                }
                else if ( floor == 5 )
                {
                    pet = Hong::create();
                }
                pet->setAnchorPoint(Vec2(0,0));
                pet->setPosition(Vec2(uiImgDressRoomBG->getContentSize().width/4, 0));
                pet->setTag(50);
                pet->setSkinUse(false);
                pet->setEvolution();
                uiImgDressRoomBG->addChild(pet);
                
                if ( floor == 4 )
                {
                    pet->setPositionX(pet->getPositionX()-65);
                }
            }
                
            //
            auto uiBtnAds = ui::Button::create(pathTVScreen);
            uiBtnAds->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            uiBtnAds->setPosition(Vec2(512, 131));
            uiBtnAds->addClickEventListener(CC_CALLBACK_1(UILayoutMyHome::onClickAdsAsk, this));
            uiBtnAds->setZoomScale(0.05f);
            uiBtnAds->setTag((int)idx);
            uiImgDressRoomBG->addChild(uiBtnAds);
            
            if ( time <= 0 && adsWatch < adsWatchMax )
            {
                auto uiImgTVLight = ui::ImageView::create("Assets/ui/myhome/btn_myroom_ad_1.png");
                uiImgTVLight->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                uiImgTVLight->setPosition(Vec2(uiBtnAds->getContentSize().width, uiBtnAds->getContentSize().height));
                uiBtnAds->addChild(uiImgTVLight, -1);
            }
            
            if ( floor > 1 && UserInfoManager::getInstance()->getMyhomeLevel(floor - 1) == 0 )
            {
                uiBtnAds->setVisible(false);
            }
        }
        else
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_myhome_zero_text"), GAME_FONT, 24);
            uiText->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            uiText->setPosition(Vec2(5, uiImgDressRoomBG->getContentSize().height));
            uiText->setTextAreaSize(Size(310, 108));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::TOP);
            uiText->setTextHorizontalAlignment(TextHAlignment::LEFT);
            uiImgDressRoomBG->addChild(uiText);
            
            auto uiBtnAds = ui::Button::create(pathTVScreen);
            uiBtnAds->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            uiBtnAds->setPosition(Vec2(512, 131));
            uiBtnAds->addClickEventListener(CC_CALLBACK_1(UILayoutMyHome::onClickAdsAsk, this));
            uiBtnAds->setZoomScale(0.05f);
            uiBtnAds->setTag((int)idx);
            uiImgDressRoomBG->addChild(uiBtnAds);
            
            if ( time <= 0 && adsWatch < adsWatchMax )
            {
                auto uiImgTVLight = ui::ImageView::create("Assets/ui/myhome/btn_myroom_ad_1.png");
                uiImgTVLight->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                uiImgTVLight->setPosition(Vec2(uiBtnAds->getContentSize().width, uiBtnAds->getContentSize().height));
                uiBtnAds->addChild(uiImgTVLight, -1);
            }
            
            if ( floor > 1 && UserInfoManager::getInstance()->getMyhomeLevel(floor - 1) == 0 )
            {
                uiText->setVisible(false);
                uiBtnAds->setVisible(false);
            }
        }
        
        auto uiTextAdsTime = ui::CText::create("", GAME_FONT_AL, 23);
        uiTextAdsTime->setPosition(Vec2(uiImgDressRoomBG->getContentSize().width - 222, uiImgDressRoomBG->getContentSize().height - 55));
        uiTextAdsTime->setVisible(false);
        uiImgDressRoomBG->addChild(uiTextAdsTime);
        {
            auto action1 = CallFuncN::create([=](Node* sender){
                
                auto uiTextAdsTime = (ui::CText*)sender;
                
                int time = UserInfoManager::getInstance()->getMyhomeAdsTime(floor);
                if ( time > 0 )
                {
                    uiTextAdsTime->setVisible(true);
                    uiTextAdsTime->setString(MafUtils::convertNumberToTime(time, false));
                }
                else
                {
                    if ( uiTextAdsTime->isVisible() == true )
                    {
                        drawList();
                    }
                }
            });
            auto action2 = DelayTime::create(1.0);
            auto seq = Sequence::create(action1, action2, NULL);
            uiTextAdsTime->runAction(RepeatForever::create(seq));
        }
    }
    
    // info
    auto uiImgInfoBG = ui::ImageView::create("Assets/ui/myhome/myhome_box_2_1.png");
    uiImgInfoBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiImgInfoBG->setPosition(Vec2(uiLayout->getContentSize().width, uiLayout->getContentSize().height / 2));
    uiLayout->addChild(uiImgInfoBG);
    {
        auto uiBtnItem = ui::Button::create("Assets/ui_common/box_item.png");
        uiBtnItem->setContentSize(Size(82, 82));
        uiBtnItem->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiBtnItem->setPosition(Vec2(0, uiImgInfoBG->getContentSize().height));
        uiBtnItem->setZoomScale(0.05f);
        uiBtnItem->setScale9Enabled(true);
        uiBtnItem->addClickEventListener(CC_CALLBACK_1(UILayoutMyHome::onClickItem, this));
        uiBtnItem->setTag((int)idx);
        uiImgInfoBG->addChild(uiBtnItem);
        {
            auto uiImgItemIcon = ui::ImageView::create(pathItem);
            uiImgItemIcon->setPosition(Vec2(uiBtnItem->getContentSize().width / 2, uiBtnItem->getContentSize().height / 2));
            uiBtnItem->addChild(uiImgItemIcon);
        }
         
        // progress
        auto uiImgWatchBarBG = ui::ImageView::create("Assets/ui/myhome/myhome_box_title1_1.png");
        uiImgWatchBarBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiImgWatchBarBG->setPosition(Vec2(82, uiImgInfoBG->getContentSize().height));
        uiImgInfoBG->addChild(uiImgWatchBarBG);
        {
            auto uiBarWatch = ui::LoadingBar::create("Assets/ui/myhome/myhome_box_title1_2.png");
            uiBarWatch->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            uiBarWatch->setPercent(adsWatchPercent);
            uiImgWatchBarBG->addChild(uiBarWatch);
            
            auto uiTextLevel = ui::CText::create(std::string("LV.").append(MafUtils::toString(level)), GAME_FONT, 20);
            uiTextLevel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiTextLevel->setPosition(Vec2(10, uiImgWatchBarBG->getContentSize().height / 2));
            uiTextLevel->setTextColor(Color4B(242, 255, 0, 255));
            uiTextLevel->enableBold();
            uiImgWatchBarBG->addChild(uiTextLevel);
        }
        
        // home icon
        auto uiImgHome = ui::ImageView::create("Assets/ui/myhome/myhome_box_icon1_off.png");
        uiImgHome->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiImgHome->setPosition(Vec2(82 + 20, uiImgInfoBG->getContentSize().height - 82));
        uiImgInfoBG->addChild(uiImgHome);
        {
            auto uiTextFloor = ui::CText::create(GAME_TEXTFORMAT("t_ui_town_floor", floor), GAME_FONT, 18);
            uiTextFloor->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            uiTextFloor->setPosition(Vec2(uiImgHome->getContentSize().width, 0));
            uiTextFloor->setTextColor(Color4B(118, 104, 69, 255));
            uiImgHome->addChild(uiTextFloor);
        }
        
        if ( level < levelMax && adsWatch >= adsWatchMax )
        {
            std::string path = "Assets/ui/myhome/btn_myhome_lv_on.png";
            if ( level == 0 )
                path = "Assets/ui/myhome/btn_myhome_lv_on_1.png";
                
            auto uiBtnUpgrade = ui::Button::create(path);
            uiBtnUpgrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiBtnUpgrade->setPosition(Vec2(uiImgInfoBG->getContentSize().width / 2, 10));
            uiBtnUpgrade->setZoomScale(0.05f);
            uiBtnUpgrade->addClickEventListener(CC_CALLBACK_1(UILayoutMyHome::onClickUpgrade, this));
            uiBtnUpgrade->setTag((int)idx);
            uiImgInfoBG->addChild(uiBtnUpgrade);
        }
        else
        {
            auto uiImgUpgradeBG = ui::ImageView::create("Assets/ui/myhome/btn_myhome_lv_off.png");
            uiImgUpgradeBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiImgUpgradeBG->setPosition(Vec2(uiImgInfoBG->getContentSize().width / 2, 10));
            uiImgInfoBG->addChild(uiImgUpgradeBG);
            {
                auto uiImgIcon = ui::ImageView::create("Assets/ui/myhome/myhome_ad_icon2.png");
                uiImgIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                uiImgIcon->setPosition(Vec2(uiImgUpgradeBG->getContentSize().width / 2 - 15, uiImgUpgradeBG->getContentSize().height / 2));
                uiImgUpgradeBG->addChild(uiImgIcon);
                
                auto uiTextWatch = ui::CText::create(MafUtils::format("%d/%d", adsWatch, adsWatchMax), GAME_FONT, 22);
                uiTextWatch->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                uiTextWatch->setPosition(Vec2(uiImgUpgradeBG->getContentSize().width / 2 + 5, uiImgUpgradeBG->getContentSize().height / 2));
                uiImgUpgradeBG->addChild(uiTextWatch);
            }
        }
    }
}

int64_t UILayoutMyHome::listviewNumberOfCells(ui::CListView *listview)
{
    int64_t count = _listMyHome.size();
   
    return count;
}
 

#pragma mark - ui
void UILayoutMyHome::uiContainer()
{
    Size size = getContentSize();
    
    //
    _uiContainerShopAds = ui::Layout::create();
    _uiContainerShopAds->setContentSize( Size(size.width - 12, 135) );
    _uiContainerShopAds->setIgnoreAnchorPointForPosition(false);
    _uiContainerShopAds->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerShopAds->setPosition( Vec2(size.width / 2, size.height) );
    _uiContainerShopAds->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _uiContainerShopAds->setBackGroundColor(Color3B(29, 41, 51));
    addChild(_uiContainerShopAds);
    
    _uiContainerBoost = ui::Layout::create();
    _uiContainerBoost->setContentSize( Size(size.width - 12, 80) );
    _uiContainerBoost->setIgnoreAnchorPointForPosition(false);
    _uiContainerBoost->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerBoost->setPosition( Vec2(size.width / 2, _uiContainerShopAds->getPositionY() - _uiContainerShopAds->getContentSize().height) );
    addChild(_uiContainerBoost);
    
    _uiContainerList = ui::Layout::create();
    _uiContainerList->setContentSize( Size(size.width - 12, size.height - _uiContainerShopAds->getContentSize().height - _uiContainerBoost->getContentSize().height) );
    _uiContainerList->setIgnoreAnchorPointForPosition(false);
    _uiContainerList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerList->setPosition( Vec2(size.width / 2, _uiContainerBoost->getPositionY() - _uiContainerBoost->getContentSize().height) );
    addChild(_uiContainerList);
}

void UILayoutMyHome::uiShopAds()
{
    _uiContainerShopAds->removeAllChildren();
    
    auto layout = _uiContainerShopAds;
    
    auto size = layout->getContentSize();
    
    //
    auto uiLayoutShopAds = UILayoutShopAds::create(_ePlace);
    uiLayoutShopAds->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(uiLayoutShopAds);
}

void UILayoutMyHome::uiBoost()
{
    _uiContainerBoost->removeAllChildren();

    auto layout = _uiContainerBoost;

    auto size = layout->getContentSize();
    
    _uiLayoutBoost = UILayoutBoost::create(Size(712, size.height));
    _uiLayoutBoost->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(_uiLayoutBoost);
}

void UILayoutMyHome::uiList()
{
    _uiContainerList->removeAllChildren();
    
    auto layout = _uiContainerList;
    
    auto size = layout->getContentSize();
    
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
void UILayoutMyHome::drawBoost()
{
    if ( _uiLayoutBoost != nullptr )
    {
        _uiLayoutBoost->refresh();
    }
}

void UILayoutMyHome::drawList()
{
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}


#pragma mark - set, get
int UILayoutMyHome::getAdsWatchMax(int floor)
{
    int level = UserInfoManager::getInstance()->getMyhomeLevel(floor);//레벨 받아와서 다시 그리기
    int levelMax = 0;

    if ( level == 0 )
    {
        levelMax = 1;
    }
    else
    {
        switch (level) {
            case 1:     levelMax = 3;    break;
            case 2:     levelMax = 3;    break;
            case 3:     levelMax = 3;    break;
            case 4:     levelMax = 3;    break;
            case 5:     levelMax = 4;    break;
            case 6:     levelMax = 4;    break;
            case 7:     levelMax = 4;    break;
            case 8:     levelMax = 4;    break;
            case 9:     levelMax = 5;    break;
            case 10:    levelMax = 5;    break;
        }
    }
    
    return levelMax;
}

#pragma mark - callback
void UILayoutMyHome::callbackAds(ADS_RESULT result)
{
    int floor = (int)_eMyhome;
    
    if ( result == ADS_RESULT::SUCCESS )
    {
        // analytics
        MafRewardAdsManager::getInstance()->setHistoryData(MafUtils::format(kAdsSlotHome, floor).c_str(), CashManager::getInstance()->isAdsfreeTime());
        
        //
        reward();
    }
    else
    {
        if ( result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(UILayoutMyHome::callbackAds, this));
            popup->setLogParams(kAnalEventAdsLoadParameterSlot);
            popup->setLogValue(MafUtils::format(kAdsSlotHome, floor).c_str());
            popup->show();
        }
        else
        {
            std::string strMessage = GAME_TEXT("t_ui_error_14");
            
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
    }
}

#pragma mark - click
void UILayoutMyHome::onClickItem(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);

    //
    E_MYHOME eType = _listMyHome.at(uiBtn->getTag());
    
    int floor = (int)eType;
    
    std::string strToast = "";
    switch (floor) {
        case 1:     strToast = GAME_TEXT("t_ui_mission_reward_4");//3종버프
            break;
        case 2:     strToast = GAME_TEXT("t_ui_currency_gem"); //보석
            break;
        case 3:     strToast = GAME_TEXT("t_ui_currency_gold"); // 골드
            break;
        case 4:     strToast = GAME_TEXT("t_ui_mineral_r"); // 광물
            break;
        case 5:     strToast = GAME_TEXT("t_ui_currency_cube");  // 큐브
            break;
        default:
            break;
    }
    
    Vec2 posToast = uiBtn->getParent()->convertToWorldSpace(uiBtn->getPosition());
    posToast.x += uiBtn->getContentSize().width / 2;
    posToast.y += uiBtn->getContentSize().height / 4;
    posToast.y += 10;
    
    PopupToast::showImmediately(strToast, posToast.x, posToast.y);
}

void UILayoutMyHome::onClickAdsAsk(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);

    //
    E_MYHOME eType = _listMyHome.at(uiBtn->getTag());
    _eMyhome = eType;
    
    int floor = (int)eType;
    
    int level = UserInfoManager::getInstance()->getMyhomeLevel(floor);
    int levelMax = UserInfoManager::getInstance()->getMyhomeLevelMax();
    
    int adsWatch = UserInfoManager::getInstance()->getMyhomeWatchcount(floor);
    int adsWatchMax = getAdsWatchMax(floor);
    
    double adsWatchPercent = (double)adsWatch / adsWatchMax * 100;
    if ( adsWatchPercent > 100 )
        adsWatchPercent = 100;
    
    int time = UserInfoManager::getInstance()->getMyhomeAdsTime(floor);
    if ( time > 0 )
    {
        return;
    }
    
    if ( time == 0 && adsWatch >= adsWatchMax )
    {
        if ( level >= levelMax )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_myhome_notice"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
    }
    else if( CashManager::getInstance()->isAdsfreeTime() == true )
    {
        callbackAds(ADS_RESULT::SUCCESS);
    }
    else
    {
        if ( level > 0 )
        {
            E_ADS_WATCH eType = E_ADS_WATCH::NONE;
            std::string strCount = "0";
            switch (floor) {
                case 1:
                    eType = E_ADS_WATCH::BUFF_ALL;
                    
                    strCount = "1800";
                    break;
                    
                case 2:
                    eType = E_ADS_WATCH::ITEM_GEM;
                    
                    strCount = "300";
                    break;
                    
                case 3:
                    eType = E_ADS_WATCH::ITEM_GOLD;
                    break;
                
                case 4:
                    eType = E_ADS_WATCH::MINE_RAND_4;
                    
                    strCount = "10";
                    break;
                    
                case 5:
                    eType = E_ADS_WATCH::ITEM_CUBE;
                    
                    strCount = "20";
                    break;
                    
                
                default:
                    return;
            }
            
            auto popup = PopupAdsWatch::create(eType, strCount);
            popup->setCallbackResult([=](){
                MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(UILayoutMyHome::callbackAds, this));
            });
            popup->show();
        }
        else
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_myhome_buff_8"));
            popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
            popup->addButton(GAME_TEXT("t_ui_yes"), false, [=](Ref* sender){
                MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(UILayoutMyHome::callbackAds, this));
            });
            popup->show();
        }
    }
}

void UILayoutMyHome::onClickUpgrade(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);

    auto uiBtn = dynamic_cast<ui::Button*>(sender);

    //
    E_MYHOME eType = _listMyHome.at(uiBtn->getTag());
    _eMyhome = eType;

    int floor = (int)eType;
    int level = UserInfoManager::getInstance()->getMyhomeLevel(floor);
    int levelMax = UserInfoManager::getInstance()->getMyhomeLevelMax();
    
    int time = UserInfoManager::getInstance()->getMyhomeAdsTime(floor);

    int adsWatch = UserInfoManager::getInstance()->getMyhomeWatchcount(floor);
    int adsWatchMax = getAdsWatchMax(floor);

    if ( adsWatch < adsWatchMax )
    {
        return;
    }
    
    if ( level == 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_myhome_name"), GAME_TEXT("t_ui_town_msg_7"));
        popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
        popup->addButton(GAME_TEXT("t_ui_yes"), false, [=](Ref* sender){
            
            upgrade();
            
        });
        popup->show();
    }
    else if ( level < levelMax )
    {
        int price = 50;
        int currency = ItemsMoneyManager::getInstance()->getGem();
        if ( currency < price )
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
            popup->show();
            return;
        }
        
        //
        upgrade();
        ItemsMoneyManager::getInstance()->setGem(-price);
    }
}

#pragma mark - game
void UILayoutMyHome::reward()
{
    int floor = (int)_eMyhome;
    int level = UserInfoManager::getInstance()->getMyhomeLevel(floor);
    
    //광고 스킵정액제가 없을때만 딜레이 시간 추가
    if ( CashManager::getInstance()->isAdsfreeTime() == false )
    {
        UserInfoManager::getInstance()->setMyhomeAdsTime(floor, 900, true);
    }
    
    //
    cocos2d::Vector<InfoItems*> listReward;
    
    if ( level > 0 )
    {
         if ( floor == 1 )
         {
             int earnCount = 1800;
             
             UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_MOVE_BONUS, earnCount, false);
             UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_ATTACK_BONUS, earnCount, false);
             UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_COIN_BONUS, earnCount);
             
             //
             PopupToastBuff::show(E_TOAST_BUFF::BUFF_ALL);
         }
         else if ( floor == 2 )
         {
             int earnCount = 300;
             
             //
             ItemsMoneyManager::getInstance()->setGem(earnCount);
             
             //
             auto obj = ItemsManager::getInstance()->getInfoItems(E_ITEMS::GEM);
             obj->setCount(earnCount);
             listReward.pushBack(obj);
         }
         else if ( floor == 3 )
         {
             auto earnCount = UserInfoManager::getInstance()->getEarnGoldFinal(_ePlace, E_EARNGOLD::ADS_SHOP);
             if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_11))
             {
                  earnCount = MafUtils::bigMulNum(earnCount, MafUtils::toString(11)); //1000%증가
             }
             earnCount = MafUtils::bigMulNum(earnCount, MafUtils::toString(2)); //2배
             
             //
             ItemsMoneyManager::getInstance()->setGold(earnCount);
             
             //
             auto obj = ItemsManager::getInstance()->getInfoItems(E_ITEMS::GOLD);
             obj->setCount(earnCount);
             listReward.pushBack(obj);
         }
         else if ( floor == 4 )
         {
             int count = SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_MINE_VIDEO_COUNT);
             SaveManager::setHistoryData(E_HISTORY_DATA::K_MINE_VIDEO_COUNT, count + 1);
             
             int earnCount = 10;
             
             E_MINERAL mineral = (E_MINERAL)(random(0,4) + (int)MINERAL_7_SAPPHIRE);
             MineralManager::getInstance()->setCount(mineral, MineralManager::getInstance()->getCount(mineral)+earnCount);
             MineralManager::getInstance()->saveData();
             
             //
             E_ITEMS eItems = E_ITEMS::NONE;
             switch (mineral) {
                 case E_MINERAL::MINERAL_7_SAPPHIRE:
                     eItems = E_ITEMS::MINE_SAPPHIRE;
                     break;
                 case E_MINERAL::MINERAL_8_OPAL:
                     eItems = E_ITEMS::MINE_OPAL;
                     break;
                 case E_MINERAL::MINERAL_9_AQUA:
                     eItems = E_ITEMS::MINE_AQUAMARINE;
                     break;
                 case E_MINERAL::MINERAL_10_EMERALD:
                     eItems = E_ITEMS::MINE_EMERALD;
                     break;
                 case E_MINERAL::MINERAL_11_AMETHYST:
                     eItems = E_ITEMS::MINE_AMETHYST;
                     break;
                 default:
                     break;
             }
             
             //
             auto obj = ItemsManager::getInstance()->getInfoItems(eItems);
             obj->setCount(earnCount);
             obj->setTypeParams((int)mineral);
             listReward.pushBack(obj);
         }
         else if(floor == 5)
         {
             int earnCount = 20;
             
             ItemsMoneyManager::getInstance()->setCube(earnCount);
             
             //
             auto obj = ItemsManager::getInstance()->getInfoItems(E_ITEMS::CUBE);
             obj->setCount(earnCount);
             listReward.pushBack(obj);
         }
    }
    
    
    //
    int cnt = UserInfoManager::getInstance()->getMyhomeWatchcount(floor);
    UserInfoManager::getInstance()->setMyhomeWatchcount(floor,cnt+1,true);
    
    // 코스튬 효과 :
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_11))
    {
        int buffTime = 180;
        UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_ATTACK_BONUS,buffTime);
    }
    
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_18))
    {
        int buffTime = 180;
        UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_COIN_BONUS, buffTime);
        UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_MOVE_BONUS, buffTime);
    }
    
    //
    if ( listReward.size() != 0 )
    {
        PopupToastItems::show(listReward);
    }
    
    //
    std::string strViewData = UserDefault::getInstance()->getStringForKey(KEY_MYHOME_COUNT, "");
    int nMyhomeCount = 0;
    if ( !strViewData.empty() )
    {
        strViewData = MafAes256::Decrypt(strViewData);
        nMyhomeCount = atoi(strViewData.c_str());
    }
    nMyhomeCount = nMyhomeCount + 1;
        
    if ( nMyhomeCount >= C_MAX_MYHOME )
    {
        nMyhomeCount = 0;
        
        // reward
        int earnCount = 180;
        UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_MOVE_BONUS, earnCount, false);
        UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_ATTACK_BONUS, earnCount, false);
        UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_COIN_BONUS, earnCount);

        //
        PopupToastBuff::show(E_TOAST_BUFF::BUFF_ALL);
    }
    
    UserDefault::getInstance()->setStringForKey(KEY_MYHOME_COUNT, MafAes256::Encrypt(MafUtils::toString(nMyhomeCount)));
    
    //
    drawBoost();
    drawList();
    
    // mission
    ServerMissionManager::getInstance()->setServerMission(E_DAILY_QUEST::ADS_MYHOME, 1);
}

void UILayoutMyHome::upgrade()
{
    int floor = (int)_eMyhome;
    int level = UserInfoManager::getInstance()->getMyhomeLevel(floor);
    int levelMax = UserInfoManager::getInstance()->getMyhomeLevelMax();
    
    level += 1;
    if ( level > levelMax )
    {
        level = levelMax;
    }
    
    //
    UserInfoManager::getInstance()->setMyhomeLevel(floor, level);
    UserInfoManager::getInstance()->setMyhomeWatchcount(floor, 0, true);
    
    // story
    StoryManager::getInstance()->showStory((int)E_COMIC::MYHOME);
    
    // analytics
    switch (floor) {
        case 1: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_MYHOME_LEVEL_1, level); break;
        case 2: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_MYHOME_LEVEL_2, level); break;
        case 3: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_MYHOME_LEVEL_3, level); break;
        case 4: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_MYHOME_LEVEL_4, level); break;
        case 5: AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_MYHOME_LEVEL_5, level); break;
            
        default:
            break;
    }
    
    //
    drawBoost();
    drawList();
}
