//
//  PopupPrison.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "PopupPrison.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIBuff.h"
#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/Other/BattlePrison/Layer/LayerPrisonBattle.h"
#include "GameUIPopup/Other/BattlePrison/Layer/LayerPrisonList.h"
#include "GameUIPopup/Other/BattlePrison/PopupBafometz.h"
#include "GameUIPopup/Other/BattlePrison/PopupJelly.h"
#include "GameUIPopup/Other/BattlePrison/PopupTicket.h"
#include "GameUIPopup/Other/BattlePrison/PopupBeelzebub.h"
#include "GameUIPopup/Other/BattlePrison/PopupPrisonAchievement.h"
#include "GameUIPopup/Other/BattlePrison/PopupEnterCount.h"
#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseGrowth.h"
#include "GameUIPopup/Other/Buff/PopupBuff.h"
#include "GameUIPopup/Other/Ads/PopupAdsInterstitial.h"
#include "GameUIPopup/Other/TimingPackage/PopupTimingPackage.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/StoryManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/AchievementManager.h"
#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/TimingPackageManager.h"
#include "ManagerGame/ServerMissionManager.h"

#include <regex>

USING_NS_CC;

PopupPrison* PopupPrison::create()
{
    PopupPrison* pRet = new(std::nothrow) PopupPrison();
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

PopupPrison::PopupPrison() :
_bInitEnter(false),
_bInitInfo(false),

_eType(PrisonManager::PrisonType::DEFAULT),

// widget
_uiContentsBattleParent(nullptr),
_uiContentsBattle(nullptr),

_uiContentsUITop(nullptr),
_uiContentsUISubMenu(nullptr),
_uiContentsUIListParent(nullptr),
_uiContentsUIList(nullptr),


_uiCurrency(nullptr),
_uiCurrencyFire(nullptr),
_itemClose(nullptr),
_itemMinimize(nullptr)
{
    
}

PopupPrison::~PopupPrison(void)
{
    MafGoogleRewardAds::HideGoogleBanner();
    
    PrisonManager::getInstance()->unSubEventInfo(this);
    PrisonManager::getInstance()->unSubEventInfoTime(this);
}

bool PopupPrison::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    setName("PopupPrison");
    
    //
    initVar();
    initUI();
    
    // event
    PrisonManager::getInstance()->subEventInfo(new MafFunction<void(bool, bool, int)>(CC_CALLBACK_3(PopupPrison::callbackDungeonInfo, this), this));
    PrisonManager::getInstance()->subEventInfoTime(new MafFunction<void()>(CC_CALLBACK_0(PopupPrison::callbackDungeonInfoTime, this), this));
    
    // 로그 이벤트
    MafAnalyticsManager::LogEvent(kAnalEventPrisonEnter, kRepeatTrue);

    return true;
}

void PopupPrison::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitEnter == false )
    {
        _bInitEnter = true;
        
        // event
        EventManager::getInstance()->requestInfo(nullptr);
        
        // achievement
        {
            // 코스튬 수(3)
            int nCostumeCount = 0;
            nCostumeCount += CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::HEAD);
            nCostumeCount += CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::BODY);
            nCostumeCount += CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::CAPE);
            nCostumeCount += CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::HAND);
            nCostumeCount += CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::FOOT);
            AchievementManager::getInstance()->requestUpdate(0, 3, nCostumeCount);
            
            // 불꽃 수(4)
            int nCostumeAuraCount = CostumeManager::getInstance()->getExistAuraCount();
            AchievementManager::getInstance()->requestUpdate(0, 4, nCostumeAuraCount );
            
            // 바포메트(5)
            if ( UserInfoManager::getInstance()->getPetBafometzHave() == true )
            {
                AchievementManager::getInstance()->requestUpdate(0, 5, 1);
            }
            
            // 베엘제붑(10)
            if ( CostumeManager::getInstance()->isExist(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_18) == true )
            {
                AchievementManager::getInstance()->requestUpdate(0, 10, 1);
            }
        }
        
        // story
        StoryManager::getInstance()->showStory((int)E_COMIC::DUNGEON_JOIN);
        
        //
        bool bPlay = PrisonManager::getInstance()->isDungeonPlay();
        if ( bPlay == false )
        {
            PrisonManager::getInstance()->requestInfo(true, (int)_eType, nullptr);
        }
    }
}

void PopupPrison::backkey()
{
    bool bPlay = PrisonManager::getInstance()->isDungeonPlay();
    int nEnterAutoCount = PrisonManager::getInstance()->getEnterAutoCount();
    if ( bPlay == true || nEnterAutoCount > 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_close_check"));
        popup->addButton(GAME_TEXT("t_ui_cancel"), true, nullptr);
        popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
            
            hide();
            
        });
        popup->show();
    }
    else
    {
        hide();
    }
}

#pragma mark - init
void PopupPrison::initVar()
{
    _eType = PrisonManager::getInstance()->getPrisonType();
}

void PopupPrison::initUI()
{
    // size
    Size size = Size(750, 1300);

    _spriteContainer->setContentSize(size);
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(0);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_prison"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    _itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupPrison::onClickClose, this));
    _itemClose->setPosition(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 37.5);
    menu->addChild(_itemClose);
    
    // item close
    _itemMinimize = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_minimize.png"), nullptr, CC_CALLBACK_1(PopupPrison::onClickMinimize, this));
    _itemMinimize->setPosition(_spriteContainer->getContentSize().width - 130, _spriteContainer->getContentSize().height - 37.5);
    _itemMinimize->setVisible(false);
    menu->addChild(_itemMinimize);
    
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::KEY);
    list.push_back(E_ITEMS::MATERIAL);
    list.push_back(E_ITEMS::POINT);
    list.push_back(E_ITEMS::COSTUME_COIN);
    
    _uiCurrency = UICurrency::create(list, Size(_spriteContainer->getContentSize().width, 50));
    _uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiCurrency->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _uiCurrency->setVisible(false);
    _spriteContainer->addChild(_uiCurrency);
    
    std::vector<E_ITEMS> listFire;
    listFire.push_back(E_ITEMS::HELL);
    
    _uiCurrencyFire = UICurrency::create(listFire, Size(_spriteContainer->getContentSize().width, 50));
    _uiCurrencyFire->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiCurrencyFire->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _uiCurrencyFire->setVisible(false);
    _spriteContainer->addChild(_uiCurrencyFire);
    
    //
    {
        double width = size.width - 15;
        double heightBattle = 600;
        double heightUITop = 600;
        double heightUIBottom = size.height - 75 - 50 - heightBattle;
        
        _uiContentsBattleParent = ui::Layout::create();
        _uiContentsBattleParent->setContentSize(Size(width, heightBattle));
        _uiContentsBattleParent->setIgnoreAnchorPointForPosition(false);
        _uiContentsBattleParent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _uiContentsBattleParent->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 50));
        _spriteContainer->addChild(_uiContentsBattleParent);
         
        _uiContentsUITop = ui::Layout::create();
        _uiContentsUITop->setContentSize(Size(width, heightUITop));
        _uiContentsUITop->setIgnoreAnchorPointForPosition(false);
        _uiContentsUITop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _uiContentsUITop->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 50));
        _spriteContainer->addChild(_uiContentsUITop);
        
        _uiContentsUIListParent = ui::Layout::create();
        _uiContentsUIListParent->setContentSize(Size(width, heightUIBottom));
        _uiContentsUIListParent->setIgnoreAnchorPointForPosition(false);
        _uiContentsUIListParent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _uiContentsUIListParent->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 0));
        _spriteContainer->addChild(_uiContentsUIListParent);
        
        _uiContentsUISubMenu = ui::Layout::create();
        _uiContentsUISubMenu->setIgnoreAnchorPointForPosition(false);
        _uiContentsUISubMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _uiContentsUISubMenu->setContentSize(Size(width, 110));
        _uiContentsUISubMenu->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _uiContentsUIListParent->getContentSize().height));
        _uiContentsUISubMenu->setLayoutType(ui::Layout::Type::HORIZONTAL);
        _spriteContainer->addChild(_uiContentsUISubMenu);
    }
    
    //
    uiContentsUI();
    uiContentsUISubMenu();
    uiContentsUIBattle();
    uiContentsUIList();
    
    //
    drawUI();
}

#pragma mark - ui
void PopupPrison::uiContentsUI()
{
    auto layout = _uiContentsUITop;
    auto size = _uiContentsUITop->getContentSize();
    
    double heightBattle = _uiContentsBattleParent->getContentSize().height;
    
    // left Top
    {
        // buff
        {
            std::vector<E_BUFF_TYPE> listBuff = {E_BUFF_TYPE::BUFF_ATTACK_BONUS, E_BUFF_TYPE::BUFF_COIN_BONUS, E_BUFF_TYPE::BUFF_MOVE_BONUS};

            auto uiLayout = UIBuff::create(listBuff);
            uiLayout->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            uiLayout->setPosition(Vec2(0, size.height));
            layout->addChild(uiLayout);
        }
        
        // help
        {
            auto uiBtnHelp = ui::Button::create("Assets/ui_common/icon_help.png");
            uiBtnHelp->setPosition(Vec2(46, size.height - 110));
            uiBtnHelp->addClickEventListener(CC_CALLBACK_1(PopupPrison::onClickHelp, this));
            layout->addChild(uiBtnHelp);
        }
    }
    
    // right Top
    {
        double posX = size.width - 80;
        double posY = 0;
        
        // achievement
        {
            posY = size.height - 50;
            
            auto uiBtnAchievement = ui::Button::create("Assets/ui/prison/icon_achievements.png");
            uiBtnAchievement->setPosition(Vec2(posX, posY));
            uiBtnAchievement->addClickEventListener(CC_CALLBACK_1(PopupPrison::onClickAchievement, this));
            layout->addChild(uiBtnAchievement);
            {
                auto uiTextTime = ui::CText::create(GAME_TEXT("t_ui_menu_achievements"), GAME_FONT, 18);
                uiTextTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                uiTextTime->setPosition(Vec2(uiBtnAchievement->getContentSize().width / 2, 0));
                uiBtnAchievement->addChild(uiTextTime);
            }
        }
        
        // time
        {
            posY = size.height - 150;
            
            auto uiBtnTime = ui::Button::create("Assets/ui/prison/icon_prison_time.png");
            uiBtnTime->setPosition(Vec2(posX, posY));
            uiBtnTime->addClickEventListener([=](Ref* sender){
                PopupToast::show(GAME_TEXT("t_ui_prison_msg_41"));
            });
            layout->addChild(uiBtnTime);
            {
                auto uiTextTime = ui::CText::create(" ", GAME_FONT, 18);
                uiTextTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                uiTextTime->setPosition(Vec2(uiBtnTime->getContentSize().width / 2, 10));
                uiBtnTime->addChild(uiTextTime);
                
                auto action1 = DelayTime::create(1.0);
                auto action2 = CallFuncN::create([=](Ref* sender){
                    
                    std::string strTime = UtilsDate::getInstance()->getTimeString(PrisonManager::getInstance()->getResetTime(), UtilsDate::eType::date_user_type_1);
                    ((ui::CText*)sender)->setString(strTime);
                });
                auto seq = Sequence::create(action1, action2, NULL);
                uiTextTime->runAction(RepeatForever::create(seq));
            }
        }
    }
    
    // right middle(battle)
    {
        double posX = size.width - 50;
        double posY = size.height - heightBattle;
        
        // growth package
        {
            posX = size.width - 70;
            posY = posY + 140;
            
            auto uiBtnGrowth = ui::Button::create("Assets/ui/bg_icon/btn_prison.png");
            uiBtnGrowth->setPosition(Vec2(posX, posY));
            uiBtnGrowth->addClickEventListener(CC_CALLBACK_1(PopupPrison::onClickGrowthPackage, this));
            layout->addChild(uiBtnGrowth);
            {
                auto animation = Animation::create();
                animation->setDelayPerUnit(0.13f);
                for ( int i = 1; i <= 3; i++ )
                {
                    animation->addSpriteFrameWithFile(MafUtils::format("Assets/ui/bg_icon/ef_btn_package_%d.png", i));
                }
                
                auto uiImgEffect = ui::ImageView::create("Assets/ui/bg_icon/ef_btn_package_1.png");
                uiImgEffect->setPosition(Vec2(uiBtnGrowth->getContentSize().width / 2, uiBtnGrowth->getContentSize().height / 2));
                uiBtnGrowth->addChild(uiImgEffect);

                auto action1 = Show::create();
                auto action2 = Animate::create(animation);
                auto action3 = Hide::create();
                auto action4 = DelayTime::create(1.0);
                auto seq = Sequence::create(action1, action2, action3, action4, NULL);
                ((ui::Scale9Sprite*)uiImgEffect->getVirtualRenderer())->runAction(RepeatForever::create(seq));
                
                //
                auto uiTextName = ui::CText::create(GAME_TEXT("t_cash_title_simple_403"), GAME_FONT, 16);
                uiTextName->setPosition(Vec2(uiBtnGrowth->getContentSize().width / 2, 0));
                uiTextName->enableOutline(Color4B::BLACK, 1);
                uiTextName->setScale(MIN(1, uiBtnGrowth->getContentSize().width / uiTextName->getContentSize().width));
                uiBtnGrowth->addChild(uiTextName);
            }
            {
                auto action1 = CallFuncN::create([=](Ref* sender){
                    
                    auto uiReddot = (UIReddot*)sender;
                    
                    bool bFinish = CashManager::getInstance()->isGrowthFinish("PRISON_FLOOR");
                    if ( bFinish == true )
                    {
                        uiReddot->stopAllActions();
                        uiReddot->setVisible(false);
                        return;
                    }

                    bool bReward = CashManager::getInstance()->isGrowthReward("PRISON_FLOOR");
                    if ( bReward == true )
                        uiReddot->setVisible(true);
                    else
                        uiReddot->setVisible(false);
                });
                auto action2 = DelayTime::create(2);
                auto seq = Sequence::create(action1, action2, NULL);
                
                
                auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_RIGHT);
                uiReddot->setPosition(uiBtnGrowth->getContentSize().width, uiBtnGrowth->getContentSize().height + 10);
                uiReddot->runAction(RepeatForever::create(seq));
                uiBtnGrowth->addChild(uiReddot);
            }
        }
    }
    
    // enter
    {
        double posX = size.width / 2;
        double posY = size.height - heightBattle + 120;
        
        auto uiBtnEnter = ui::Button::create("Assets/ui/prison/btn_entrance.png");
        uiBtnEnter->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiBtnEnter->setPosition(Vec2(posX, posY + 2.5));
        uiBtnEnter->addClickEventListener(CC_CALLBACK_1(PopupPrison::onClickEnter, this));
        layout->addChild(uiBtnEnter);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_prison_enter"), GAME_FONT, 28);
            uiText->setTextColor(Color4B(COLOR_TITLE));
            uiText->setPosition(Vec2(uiBtnEnter->getContentSize().width / 2, uiBtnEnter->getContentSize().height / 2));
            uiBtnEnter->addChild(uiText);
        }
        
        _uiEnterInfoBG = ui::ImageView::create("Assets/ui_common/base_box_1_13.png");
        _uiEnterInfoBG->setContentSize(Size(224, 30));
        _uiEnterInfoBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _uiEnterInfoBG->setPosition(Vec2(posX, posY - 2.5));
        _uiEnterInfoBG->setScale9Enabled(true);
        layout->addChild(_uiEnterInfoBG);
        {
            auto uiImgBarTimeBG = ui::ImageView::create("Assets/ui/prison/progressbar_freepass_1.png");
            uiImgBarTimeBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiImgBarTimeBG->setPosition(Vec2(10, _uiEnterInfoBG->getContentSize().height / 2));
            _uiEnterInfoBG->addChild(uiImgBarTimeBG);
            
            auto uiBarTime = ui::LoadingBar::create("Assets/ui/prison/progressbar_freepass_2.png");
            uiBarTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiBarTime->setPosition(Vec2(10, _uiEnterInfoBG->getContentSize().height / 2));
            uiBarTime->setPercent(50);
            uiBarTime->setName("UI_BAR_ENTER");
            _uiEnterInfoBG->addChild(uiBarTime);
            
            auto uiTextTime = ui::CText::create("", GAME_FONT, 18);
            uiTextTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiTextTime->setPosition(Vec2(20, _uiEnterInfoBG->getContentSize().height / 2));
            uiTextTime->enableOutline(Color4B::BLACK, 2);
            uiTextTime->setName("UI_TEXT_TIME");
            _uiEnterInfoBG->addChild(uiTextTime);
            
            auto uiImgIcon = ui::ImageView::create("Assets/icon/icon_freepass_2.png");
            uiImgIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiImgIcon->setPosition(Vec2(85 - 11, _uiEnterInfoBG->getContentSize().height / 2));
            _uiEnterInfoBG->addChild(uiImgIcon);
            
            auto uiTextEnter = ui::CText::create("0/0", GAME_FONT, 20);
            uiTextEnter->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiTextEnter->setPosition(Vec2(_uiEnterInfoBG->getContentSize().width, _uiEnterInfoBG->getContentSize().height / 2));
            uiTextEnter->setTextAreaSize(Size(100, uiTextEnter->getContentSize().height));
            uiTextEnter->setTextOverflow(Label::Overflow::SHRINK);
            uiTextEnter->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiTextEnter->setName("UI_TEXT_ENTER");
            _uiEnterInfoBG->addChild(uiTextEnter);
        }
    }
    
    // raid warning
    {
        _uiRaidWarningBG = ui::ImageView::create("Assets/ui_common/base_text_1_2.png");
        _uiRaidWarningBG->setScale9Enabled(true);
        _uiRaidWarningBG->setContentSize(Size(size.width - 200, 100));
        _uiRaidWarningBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _uiRaidWarningBG->setPosition(Vec2(size.width / 2, size.height - 80));
        _uiRaidWarningBG->setVisible(false);
        layout->addChild(_uiRaidWarningBG);
        {
            auto uiWarning = ui::CText::create("", GAME_FONT, 24);
            uiWarning->setPosition(_uiRaidWarningBG->getContentSize()/2);
            uiWarning->enableOutline(Color4B::BLACK, 1);
            uiWarning->setColor(COLOR_TITLE);
            uiWarning->setTextAreaSize(Size(_uiRaidWarningBG->getContentSize().width - 10, _uiRaidWarningBG->getContentSize().height - 10));
            uiWarning->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiWarning->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiWarning->setName("UI_TEXT");
            _uiRaidWarningBG->addChild(uiWarning);
        }
    }
}

void PopupPrison::uiContentsUISubMenu()
{
    auto layout = _uiContentsUISubMenu;
     
    std::vector<std::string> listMenu = {"TYPE_DEFAULT", "TYPE_FIRE", "BAFOMETZ", "BEELZEBUB", "JELLY", "TICKET"};
    std::vector<std::string> listMenuIcon = {"Assets/ui/prison/icon_prison1_1.png", "Assets/ui/prison/icon_prison1_2.png", "Assets/ui/prison/icon_baformetz_1.png", "Assets/ui/prison/icon_beelzebub_1.png", "Assets/ui/prison/icon_jelly.png", "Assets/ui/prison/prison_ticket.png"};
    std::vector<std::string> listMenuName = {"t_ui_prison", "t_ui_prison_title_fire", "t_ui_pet_ui_bafometz", "t_ui_menu_hellprison", "t_ui_prison_jelly", "t_ui_prison_msg_1"};
    
    for ( int i = 0; i < listMenu.size(); i++ )
    {
        std::string menuName = listMenu.at(i);
        std::string icon = listMenuIcon.at(i);
        std::string name = listMenuName.at(i);
        
        auto path = "Assets/ui/prison/btn_prison_2_off.png";
        if ( menuName.compare("BEELZEBUB") == 0 )
        {
            path = "Assets/ui/prison/btn_prison_3_off.png";
        }
        
        auto uiButton = ui::Button::create(path);
        uiButton->addClickEventListener(CC_CALLBACK_1(PopupPrison::onClickMenu, this));
        uiButton->setName(menuName);
        layout->addChild(uiButton);
        {
            auto uiImgIcon = ui::ImageView::create(icon);
            uiImgIcon->setPosition(Vec2(uiButton->getContentSize().width / 2, uiButton->getContentSize().height / 2 + 15));
            uiImgIcon->setName("UI_IMG_ICON");
            uiButton->addChild(uiImgIcon);
            
            auto uiTextName = ui::CText::create(GAME_TEXT(name), GAME_FONT, 24);
            uiTextName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiTextName->setPosition(Vec2(uiButton->getContentSize().width / 2, 10));
            uiTextName->setName("UI_TEXT_NAME");
            uiButton->addChild(uiTextName);
        }
    }
}

void PopupPrison::uiContentsUIBattle()
{
    auto layout = _uiContentsBattleParent;
    auto size = _uiContentsBattleParent->getContentSize();
    
    auto sceen = UtilsScene::getInstance()->getRunningScene();
    auto uiLayoutBattle = sceen->getChildByTag<LayerPrisonBattle*>(TAG_SCENE_PRISON);
    if ( uiLayoutBattle != nullptr )
    {
        // retain
        uiLayoutBattle->retain();
        uiLayoutBattle->removeFromParentAndCleanup(false);
        layout->addChild(uiLayoutBattle);
        
        // release
        uiLayoutBattle->release();
    }
    else
    {
        uiLayoutBattle = LayerPrisonBattle::create(size);
        layout->addChild(uiLayoutBattle);
    }
    uiLayoutBattle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayoutBattle->setPosition(Vec2(size.width / 2, size.height / 2));
    uiLayoutBattle->setScale(1.0f);
    uiLayoutBattle->setMinimize(false);
    
    //
    _uiContentsBattle = uiLayoutBattle;
}

void PopupPrison::uiContentsUIList()
{
    auto layout = _uiContentsUIListParent;
    auto size = _uiContentsUIListParent->getContentSize();
    
    _uiListTitleBG = ui::ImageView::create("Assets/ui_common/box.png");
    _uiListTitleBG->setScale9Enabled(true);
    _uiListTitleBG->setContentSize(Size(size.width, 40));
    _uiListTitleBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiListTitleBG->setPositionType(ui::Layout::PositionType::PERCENT);
    _uiListTitleBG->setPositionPercent(Vec2::ANCHOR_MIDDLE_TOP);
    layout->addChild(_uiListTitleBG);
    {
        auto uiTextTitle = ui::CText::create(GAME_TEXT("t_ui_prison_title_quest"), GAME_FONT, 24);
        uiTextTitle->setPosition( Vec2(238, _uiListTitleBG->getContentSize().height / 2) );
        _uiListTitleBG->addChild(uiTextTitle);
        
        auto uiTextReward = ui::CText::create(GAME_TEXT("t_ui_reward"), GAME_FONT, 24);
        uiTextReward->setPosition( Vec2(_uiListTitleBG->getContentSize().width - 130, _uiListTitleBG->getContentSize().height / 2) );
        _uiListTitleBG->addChild(uiTextReward);
        
        auto uiImgRatio = ui::ImageView::create("Assets/ui/prison/p_ticket_buff_x2.png");
        uiImgRatio->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiImgRatio->setPosition(Vec2(uiTextReward->getPositionX() + uiTextReward->getContentSize().width / 2, uiTextReward->getPositionY()));
        uiImgRatio->setName("UI_IMG_RATIO");
        uiImgRatio->setVisible(false);
        _uiListTitleBG->addChild(uiImgRatio);
    }
    
    _uiContentsUIList = LayerPrisonList::create(Size(size.width, size.height - _uiListTitleBG->getContentSize().height));
    _uiContentsUIList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsUIList->setPosition(Vec2(size.width / 2, 0));
    layout->addChild(_uiContentsUIList);
}
          
          
#pragma mark - draw


void PopupPrison::drawUI()
{
    Size size = _spriteContainer->getContentSize();
    
    double width = size.width - 15;
    double heightUITop = 600;
    double heightUIBottom = size.height - 75 - 50 - heightUITop - 10;
    
    //
    if ( _eType == PrisonManager::PrisonType::DEFAULT )
    {
        _uiCurrency->setVisible(true);
        _uiCurrencyFire->setVisible(false);
    }
    else
    {
        _uiCurrency->setVisible(false);
        _uiCurrencyFire->setVisible(true);
    }
    
    //
    if ( PrisonManager::getInstance()->isDungeonPlay() == false )
    {
        //
        _itemMinimize->setVisible(false);
        
        //
        _uiContentsUITop->setVisible(true);
        _uiContentsUISubMenu->setVisible(true);
        
        heightUIBottom -= 30;
    }
    else
    {
        //
        _itemMinimize->setVisible(true);
        
        //
        _uiContentsUITop->setVisible(false);
        _uiContentsUISubMenu->setVisible(false);
    }
    _uiContentsUIListParent->setContentSize(Size(width, heightUIBottom));
    
    // draw
    drawUIEnter();
    drawUISubMenu();
    drawUIBattle();
    drawUIList();
}

void PopupPrison::drawUIEnter()
{
    _uiEnterInfoBG->stopAllActions();
    
    //
    int nHeart = PrisonManager::getInstance()->getHeart();
    int nHeartMax = PrisonManager::getInstance()->getHeartMax();
    if ( nHeart >= nHeartMax )
    {
        auto uiBarTime = _uiEnterInfoBG->getChildByName<ui::LoadingBar*>("UI_BAR_ENTER");
        auto uiTextTime = _uiEnterInfoBG->getChildByName<ui::CText*>("UI_TEXT_TIME");
        auto uiTextEnter = _uiEnterInfoBG->getChildByName<ui::CText*>("UI_TEXT_ENTER");
        
        uiBarTime->setPercent(100);
        uiTextTime->setString("MAX");
        uiTextEnter->setString(MafUtils::format("%d/%d", nHeart, nHeartMax));
    }
    else
    {
        auto action1 = CallFuncN::create([=](Node* sender){
            
            auto uiBarTime = _uiEnterInfoBG->getChildByName<ui::LoadingBar*>("UI_BAR_ENTER");
            auto uiTextTime = _uiEnterInfoBG->getChildByName<ui::CText*>("UI_TEXT_TIME");
            auto uiTextEnter = _uiEnterInfoBG->getChildByName<ui::CText*>("UI_TEXT_ENTER");
            
            int nHeart = PrisonManager::getInstance()->getHeart();
            int nHeartMax = PrisonManager::getInstance()->getHeartMax();
            int nHeartTime = (int)(PrisonManager::getInstance()->getHeartTime() - UtilsDate::getInstance()->getTime());
            int nHeartTimeMax = 900;
            if ( nHeartTime <= 0)
            {
                _uiEnterInfoBG->stopAllActions();
                PrisonManager::getInstance()->requestInfoTime((int)_eType);
            }
            
            double percent = (1.0 - (double)nHeartTime / nHeartTimeMax) * 100;
            if ( percent > 100 )
                percent = 100;

            //
            uiBarTime->setPercent(percent);
            uiTextTime->setString(MafUtils::format("%02d:%02d", nHeartTime/60, nHeartTime%60));
            uiTextEnter->setString(MafUtils::format("%d/%d", nHeart, nHeartMax));
            
        });
        auto action2 = DelayTime::create(1);
        auto seq = Sequence::create(action1, action2, NULL);
        _uiEnterInfoBG->runAction(RepeatForever::create(seq));
    }
}

void PopupPrison::drawUISubMenu()
{
    auto layout = _uiContentsUISubMenu;
    
    std::string listMenuDefault[] = {"TYPE_FIRE", "BAFOMETZ", "JELLY", "TICKET"};
    std::string listMenuFire[] = {"TYPE_DEFAULT", "BEELZEBUB", "JELLY"};

    std::vector<std::string> listMenu;
    if ( _eType == PrisonManager::PrisonType::DEFAULT )
    {
        listMenu.assign(std::begin(listMenuDefault), std::end(listMenuDefault));
    }
    else
    {
        listMenu.assign(std::begin(listMenuFire), std::end(listMenuFire));
    }
    
    // hide
    for ( auto& child : layout->getChildren() )
    {
        child->setVisible(false);
    }
    
    // show & draw
    for ( int i = 0; i < listMenu.size(); i++ )
    {
        std::string menuName = listMenu.at(i);
        auto uiButton = layout->getChildByName<ui::Button*>(menuName);
        uiButton->setVisible(true);

        auto uiTextName = uiButton->getChildByName<ui::CText*>("UI_TEXT_NAME");
        
        if ( menuName.compare("TICKET") == 0 )
        {
            if ( PrisonManager::getInstance()->getTicketUseRatio() > 1 )
            {
                uiButton->loadTextureNormal("Assets/ui/prison/btn_prison_4_off.png");
                
                uiTextName->setColor(COLOR_COIN);
                uiTextName->setString(GAME_TEXT("t_ui_used"));
                
                // effect
                auto animation = Animation::create();
                animation->setDelayPerUnit(0.1f);
                for ( int i = 1; i <= 3; i++ )
                {
                    animation->addSpriteFrameWithFile(MafUtils::format("Assets/ui/prison/prison_ticket_fx1_%d.png", i));
                }
                
                auto uiImgEffect = ui::ImageView::create("Assets/ui/prison/prison_ticket_fx1_1.png");
                uiImgEffect->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                uiImgEffect->setPosition(Vec2(uiButton->getContentSize().width - 30, uiButton->getContentSize().height));
                uiImgEffect->setName("UI_IMG_EFFECT");
                uiButton->removeChildByName("UI_IMG_EFFECT");
                uiButton->addChild(uiImgEffect);
                 
                auto action1 = Animate::create(animation);
                auto action2 = DelayTime::create(1.0);
                auto seq = Sequence::create(action1, action2, NULL);
                ((ui::Scale9Sprite*)uiImgEffect->getVirtualRenderer())->runAction(RepeatForever::create(seq));
            }
            else
            {
                uiButton->loadTextureNormal("Assets/ui/prison/btn_prison_2_off.png");
                
                uiTextName->setString(GAME_TEXTFORMAT("t_ui_prison_msg_1", PrisonManager::getInstance()->getTicketCount()));
                uiTextName->setTextColor(Color4B::WHITE);
                
                // effect
                uiButton->removeChildByName("UI_IMG_EFFECT");
            }
        }
        else if ( menuName.compare("TYPE_FIRE") == 0 )
        {
            int nKill = SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_PRISON_METER);
            if ( nKill < 8000 )
            {
                uiButton->loadTextureNormal("Assets/ui/prison/btn_prison_2_lock_1_1.png");
                
                // lock
                auto uiImgLock = ui::ImageView::create("Assets/ui/prison/btn_prison_2_lock_1_2.png");
                uiImgLock->setPosition(Vec2(uiButton->getContentSize().width / 2, uiButton->getContentSize().height / 2));
                uiImgLock->setName("UI_IMG_LOCK");
                uiButton->removeChildByName("UI_IMG_LOCK");
                uiButton->addChild(uiImgLock);
            }
            else
            {
                uiButton->loadTextureNormal("Assets/ui/prison/btn_prison_2_off.png");
                
                // lock
                uiButton->removeChildByName("UI_IMG_LOCK");
            }
        }
        else if ( menuName.compare("JELLY") == 0 )
        {
            if ( PrisonManager::getInstance()->isJellyRewardMax() == true )
            {
                auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
                uiReddot->setPosition(3, uiButton->getContentSize().height - 3);
                uiReddot->setName("UI_REDDOT");
                uiButton->removeChildByName("UI_REDDOT");
                uiButton->addChild(uiReddot);
            }
            else
            {
                uiButton->removeChildByName("UI_REDDOT");
            }
        }
            
    }
    
    // size
    auto size = UtilsWidget::getInstance()->getLayoutAccumulatedSize(layout);
    layout->setContentSize(size);
    layout->setPositionY(_uiContentsUIListParent->getContentSize().height + 10);
    layout->requestDoLayout();
}

void PopupPrison::drawUIBattle()
{
    auto layout = _uiContentsBattleParent;
    auto size = _uiContentsBattleParent->getContentSize();
    
    if ( _uiContentsBattle == nullptr )
    {
        auto uiLayoutBattle = LayerPrisonBattle::create(size);
        uiLayoutBattle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiLayoutBattle->setPosition(Vec2(size.width / 2, size.height / 2));
        uiLayoutBattle->setScale(1.0f);
        uiLayoutBattle->setMinimize(false);
        layout->addChild(uiLayoutBattle);
        
        //
        _uiContentsBattle = uiLayoutBattle;
    }
    
    auto uiLayoutBattle = dynamic_cast<LayerPrisonBattle*>(_uiContentsBattle);
    if ( uiLayoutBattle != nullptr )
    {
        uiLayoutBattle->onDraw();
    }
}

void PopupPrison::drawUIList()
{
    auto layout = _uiContentsUIListParent;
    auto size = _uiContentsUIListParent->getContentSize();
    
    auto uiList = dynamic_cast<LayerPrisonList*>(_uiContentsUIList);
    if ( uiList != nullptr )
    {
        uiList->setContentSize(Size(size.width, size.height - 40));
        uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiList->setPosition(Vec2(size.width / 2, 0));
        uiList->onDrawList();
    }
    
    //
    auto uiImgRatio = _uiListTitleBG->getChildByName<ui::ImageView*>("UI_IMG_RATIO");
    if ( uiImgRatio != nullptr )
    {
        int idxRatio = PrisonManager::getInstance()->getTicketUseIdx();
        if ( idxRatio == -1 || _eType == PrisonManager::PrisonType::FIRE )
        {
            uiImgRatio->setVisible(false);
        }
        else
        {
            std::string path = "Assets/ui/prison/p_ticket_buff_x2.png";
            switch (idxRatio) {
                case 0:     path = "Assets/ui/prison/p_ticket_buff_x2.png"; break;
                case 1:     path = "Assets/ui/prison/p_ticket_buff_x3.png"; break;
                case 2:     path = "Assets/ui/prison/p_ticket_buff_x4.png"; break;
                case 3:     path = "Assets/ui/prison/p_ticket_buff_x5.png"; break;
                case 4:     path = "Assets/ui/prison/p_ticket_buff_x7.png"; break;
                default:
                    break;
            }
            
            uiImgRatio->loadTexture(path);
            uiImgRatio->setVisible(true);
        }
    }
    
    
     
}
 
#pragma mark - ui : set, get

#pragma mark - callback
void PopupPrison::callbackDungeonInfo(bool bResult, bool bAvailable, int nType)
{
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    if ( bAvailable == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    // draw
    if ( _eType != PrisonManager::getInstance()->getPrisonType() )
    {
        _uiContentsBattle->removeFromParent();
        _uiContentsBattle = nullptr;
        
        _eType = PrisonManager::getInstance()->getPrisonType();
    }
    drawUI();
//    runAction(CallFunc::create([=](){
//        drawUI();
//    }));
    
    // TimingPackage
    onTimingPackage();
    
    //
    if ( _eType == PrisonManager::PrisonType::FIRE )
    {
        StoryManager::getInstance()->showStory((int)E_COMIC::PRISON_FIRE_JOIN);
    }
     
    //
    if ( _bInitInfo == false )
    {
        _bInitInfo = true;
        
        std::string strTodayOld = UserDefault::getInstance()->getStringForKey(KEY_TODAY_PRISON, "");
        std::string strToday = UtilsDate::getInstance()->getTimeString(UtilsDate::getInstance()->getTime(), UtilsDate::eType::date_real_Ymd);
        if ( PrisonManager::getInstance()->getDungeonCount() == 0 && strTodayOld.compare(strToday) != 0 )
        {
            UserDefault::getInstance()->setStringForKey(KEY_TODAY_PRISON, strToday);
            
            //
            onPopupTicket();
        }
        
        if ( PrisonManager::getInstance()->getTicketUseRatio() > 1 && PrisonManager::getInstance()->getDungeonCount() == 0 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_prison_msg_43", PrisonManager::getInstance()->getTicketUseRatio()));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
    }
    
    //
    if ( PrisonManager::getInstance()->getDungeonCount() >= 3 )
    {
        int level = CashManager::getInstance()->getGrowthNowLevel("PRISON_FLOOR");
        int idxCash = CashManager::getInstance()->getGrowthToIdxCash("PRISON_FLOOR", level);
        
        bool bHistory = CashManager::getInstance()->getHistoryAds(idxCash);
        if ( bHistory == false )
        {
            bool bFinish = CashManager::getInstance()->isGrowthFinish("PRISON_FLOOR");
            if ( bFinish == false )
            {
                onPopupGrowthPackage();
            }
        }
    }
    
    //
    if ( PrisonManager::getInstance()->getBafometzCount() >= 1500 && UserInfoManager::getInstance()->getPetBafometzHave() == false )
    {
        UserInfoManager::getInstance()->setPetBafometzHave(true);
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_raid_chuseok_msg_14"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    
    //
    AchievementManager::getInstance()->requestInfo(0, nullptr);
    
    //
    if ( AdsUtilsManager::getInstance()->isErrorAdsInterstitial(E_ADS_INTERSTITIAL_TYPE::PRISON_PLUS) == true )
    {
        if ( AdsUtilsManager::getInstance()->isErrorAdsInterstitial(E_ADS_INTERSTITIAL_TYPE::PRISON_PLUS) )
        {
            PopupLoading::show();
            PrisonManager::getInstance()->requestAdsInterstitial(CC_CALLBACK_2(PopupPrison::callbackAdsInterstitialErrorReward, this));
        }
    }
    
    if ( NewRaidManager::getInstance()->getEndSeasonTime() - UtilsDate::getInstance()->getTime() < 0 )
    {
        PopupLoading::show();
        NewRaidManager::getInstance()->requestInfo(CC_CALLBACK_2(PopupPrison::callbackNewRaidBreakTime, this));
    }
}

void PopupPrison::callbackDungeonInfoTime()
{
    drawUIEnter();
}

void PopupPrison::callbackDungeonResultSkip(bool bResult)
{
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    // draw
    drawUI();
    
    // ads
    onPopupAdsInterstitial();
    
    //
    PrisonManager::getInstance()->requestInfo(true, (int)_eType, nullptr);
}

void PopupPrison::callbackTicketUse()
{
    drawUI();
}

void PopupPrison::callbackTicketBuy()
{
    drawUI();
}

void PopupPrison::callbackEnter(int nCount)
{
    if ( PrisonManager::getInstance()->getResetTime() <= 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_error_reset"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){

            PrisonManager::getInstance()->requestInfo(true, (int)_eType, nullptr);
            
        });
        popup->setHideBackKey(false);
        popup->show();
        return;
    }
    
    
    PrisonManager::getInstance()->setEnterAutoCount(nCount);
    PrisonManager::getInstance()->setEnterAutoCountMax(nCount);
    PrisonManager::getInstance()->setEnterAuto(true);
    
    // battle
    onBattle();
}

void PopupPrison::callbackBeelzebub()
{
    
}

void PopupPrison::callbackAdsInterstitial(int nType)
{
    PopupLoading::show();
    PrisonManager::getInstance()->requestAdsInterstitial(CC_CALLBACK_2(PopupPrison::callbackAdsInterstitialResult, this));
}

void PopupPrison::callbackAdsInterstitialResult(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 3 )
        {
            strMessage = GAME_TEXT("t_ui_error_49");
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
}

void PopupPrison::callbackAdsInterstitialErrorReward(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if ( bResult == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_50"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
}

void PopupPrison::callbackBafometzSkip(int nCount)
{
    if ( nCount <= 0 )
    {
        return;
    }
    
    //
    ServerMissionManager::getInstance()->setServerMission(E_DAILY_QUEST::PRISON_TICKET_USE, nCount);
    
    // network
    PrisonManager::getInstance()->requestDefaultResultSkip(true, nCount, CC_CALLBACK_1(PopupPrison::callbackDungeonResultSkip, this));
}

void PopupPrison::callbackBafometzTicket()
{
    PrisonManager::getInstance()->requestInfo(true, (int)_eType, nullptr);
}

void PopupPrison::callbackJellyHeart()
{
    PrisonManager::getInstance()->requestInfo(true, (int)_eType, nullptr);
}

void PopupPrison::callbackNewRaidBreakTime(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    auto callFunc = CallFuncN::create([=](Node* sender){
        
        int64_t time = NewRaidManager::getInstance()->getSeasonBreakTime() - UtilsDate::getInstance()->getTime();//서버에서 주는 시간으로 변경 필요
        if ( time <= 0 )
        {
            sender->setVisible(false);
            sender->stopAllActions();
            return;
        }
        
        int min = (int)(time / 60);
        int sec = (time % 3600) % 60;
        
        if(min < 0)
            min = 0;
        if(sec < 0)
            sec = 0;
        
        sender->setVisible(true);
        
        auto uiWarning = sender->getChildByName<ui::CText*>("UI_TEXT");
        uiWarning->setString(GAME_TEXTFORMAT("t_ui_new_raid_error_prison", MafUtils::format("%02d:%02d", min, sec).c_str()));
    });
    auto delayTime = DelayTime::create(1.f);
    auto seq = Sequence::create(callFunc, delayTime, nullptr);
    _uiRaidWarningBG->stopAllActions();
    _uiRaidWarningBG->runAction(RepeatForever::create(seq));
}

#pragma mark - click
void PopupPrison::onClickClose(Ref* sender)
{
    bool bPlay = PrisonManager::getInstance()->isDungeonPlay();
    int nEnterAutoCount = PrisonManager::getInstance()->getEnterAutoCount();
    if ( bPlay == true || nEnterAutoCount > 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_close_check"));
        popup->addButton(GAME_TEXT("t_ui_cancel"), true, nullptr);
        popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
            
            hide();
            
        });
        popup->show();
    }
    else
    {
        hide();
    }
}

void PopupPrison::onClickMinimize(cocos2d::Ref* sender)
{
    onMinimize();
}

void PopupPrison::onClickAchievement(cocos2d::Ref* sender)
{
    onPopupAchievement();
}

void PopupPrison::onClickGrowthPackage(cocos2d::Ref* sender)
{
    onPopupGrowthPackage();
}

void PopupPrison::onClickHelp(cocos2d::Ref* sender)
{
    auto popup = PopupHelp::create(5, "t_ui_prison_info_title_%d", "t_ui_prison_info_msg_%d");
    popup->show();
}

void PopupPrison::onClickBuff(cocos2d::Ref* sender)
{
    ValueVector list;
    list.push_back(Value(E_SHOP::SHOP_ATTACK_BONUS));
    list.push_back(Value(E_SHOP::SHOP_COIN_BONUS));
    list.push_back(Value(E_SHOP::SHOP_MOVE_BONUS));
    
    auto popup = PopupBuff::create(list);
    popup->show();
}

void PopupPrison::onClickMenu(Ref* sender)
{
    auto uiButton = (ui::Button*)sender;
    
    std::string menuName = uiButton->getName();
    if ( menuName.compare("TYPE_DEFAULT") == 0 )
    {
        PrisonManager::getInstance()->requestInfo(true, (int)PrisonManager::PrisonType::DEFAULT, nullptr);
    }
    else if ( menuName.compare("TYPE_FIRE") == 0 )
    {
        int nKill = SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_PRISON_METER);
        if ( nKill >= 8000 )
        {
            PrisonManager::getInstance()->requestInfo(true, (int)PrisonManager::PrisonType::FIRE, nullptr);
        }
        else
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_msg_37"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
    }
    else if ( menuName.compare("BAFOMETZ") == 0 )
    {
        onPopupBafometz();
    }
    else if ( menuName.compare("BEELZEBUB") == 0 )
    {
        onPopupBeelzebub();
    }
    else if ( menuName.compare("JELLY") == 0 )
    {
        onPopupJelly();
    }
    else if ( menuName.compare("TICKET") == 0 )
    {
        onPopupTicket();
    }
    
}

void PopupPrison::onClickEnter(Ref* sender)
{
    auto thiz = makeRef(this);
    
    auto popup = PopupEnterCount::create();
    popup->setCallbackEnter([&, thiz](int nCount){

        if (thiz->getReferenceCount() < 2) return;
        callbackEnter(nCount);
    });
    popup->setCallbackEnterBafometz([&, thiz](int nCount){

        if (thiz->getReferenceCount() < 2) return;
        callbackBafometzSkip(nCount);
    });
    popup->show();
}

#pragma mark - game
void PopupPrison::onDraw()
{
    drawUI();
}

void PopupPrison::onBattle()
{
    auto uiLayoutBattle = dynamic_cast<LayerPrisonBattle*>(_uiContentsBattle);
    if ( uiLayoutBattle == nullptr )
    {
        return;
    }

    uiLayoutBattle->onBattleStart();
}

void PopupPrison::onMinimize()
{
    auto uiLayoutBattle = dynamic_cast<LayerPrisonBattle*>(_uiContentsBattle);
    if ( uiLayoutBattle == nullptr )
    {
        return;
    }
    
    // retain
    uiLayoutBattle->retain();
    uiLayoutBattle->removeFromParentAndCleanup(false);
    
    auto scene = UtilsScene::getInstance()->getRunningScene();
    if ( scene == nullptr )
    {
        return;
    }
    
    uiLayoutBattle->setMinimize(true);
    uiLayoutBattle->setAnchorPoint(Vec2::ZERO);
    uiLayoutBattle->setPosition(Vec2(0, 140));
    uiLayoutBattle->setScale(0.3f);
    scene->addChild(uiLayoutBattle, ZORDER_SCENE_PRISON, TAG_SCENE_PRISON);
    
    // release
    uiLayoutBattle->release();
    
    //
    hide();
}

void PopupPrison::onTimingPackage()
{
    if ( AccountManager::getInstance()->getUserIdx() == 0 )
    {
        return;
    }
    
    auto listTimingPackageStartEnter = TimingPackageManager::getInstance()->getListInfoTimingPackageStart(E_TIMING_PACKAGE_CONDITION_TYPE::PRISON_ENTER);
    auto listTimingPackageStartFloor = TimingPackageManager::getInstance()->getListInfoTimingPackageStart(E_TIMING_PACKAGE_CONDITION_TYPE::PRISON_FLOOR);
    for(auto obj : listTimingPackageStartEnter)
    {
        switch (obj->getType())
        {
            case E_TIMING_PACKAGE_TYPE::FREE:
            {
                obj->setIsActivate(true);
                
                auto popup = PopupTimingPackage::create(obj);
                popup->show();
            }
                break;
            case E_TIMING_PACKAGE_TYPE::PURCHASE:
            case E_TIMING_PACKAGE_TYPE::TERMS_FREE:
            {
                TimingPackageManager::getInstance()->requestTimingPackageActivate([&, obj](bool bResult, int nResult){
                    if(!bResult)
                        return;
                    
                    if(nResult != 1)
                        return;
                    
                    auto popup = PopupTimingPackage::create(obj);
                    popup->show();
                }, obj);
            }
                break;
            default:
                break;
        }
    }
    
    for(auto obj : listTimingPackageStartFloor)
    {
        switch (obj->getType())
        {
            case E_TIMING_PACKAGE_TYPE::FREE:
            {
                obj->setIsActivate(true);
                
                auto popup = PopupTimingPackage::create(obj);
                popup->show();
            }
                break;
            case E_TIMING_PACKAGE_TYPE::PURCHASE:
            case E_TIMING_PACKAGE_TYPE::TERMS_FREE:
            {
                TimingPackageManager::getInstance()->requestTimingPackageActivate([&, obj](bool bResult, int nResult){
                    if(!bResult)
                        return;
                    
                    if(nResult != 1)
                        return;
                    
                    auto popup = PopupTimingPackage::create(obj);
                    popup->show();
                }, obj);
            }
                break;
            default:
                break;
        }
    }
    
    //
    auto listTimingPackageReward = TimingPackageManager::getInstance()->getListInfoTimingPackageRewardAll();
    for(auto obj : listTimingPackageReward)
    {
        TimingPackageManager::getInstance()->requestReward(obj, nullptr, false);
    }
}

void PopupPrison::onPopupAchievement()
{
    auto popup = PopupPrisonAchievement::create();
    popup->show();
}

void PopupPrison::onPopupGrowthPackage()
{
    auto popup = PopupPurchaseGrowth::create("PRISON_FLOOR");
    popup->show();
}

void PopupPrison::onPopupTicket()
{
    auto popup = PopupTicket::create();
    popup->setCallbackUse(CC_CALLBACK_0(PopupPrison::callbackTicketUse, this));
    popup->setCallbackBuy(CC_CALLBACK_0(PopupPrison::callbackTicketBuy, this));
    popup->setHideAll(false);
    popup->show();
}

void PopupPrison::onPopupBafometz()
{
    auto popup = PopupBafometz::create();
    popup->setCallbackSkip(CC_CALLBACK_1(PopupPrison::callbackBafometzSkip, this));
    popup->setCallbackReward(CC_CALLBACK_0(PopupPrison::callbackBafometzTicket, this));
    popup->setHideAll(false);
    popup->show();
}

void PopupPrison::onPopupBeelzebub()
{
    auto popup = PopupBeelzebub::create();
    popup->setHideAll(false);
    popup->show();
}

void PopupPrison::onPopupJelly()
{
    auto popup = PopupJelly::create();
    popup->setCallbackHeart(CC_CALLBACK_0(PopupPrison::callbackJellyHeart, this));
    popup->setHideAll(false);
    popup->show();
}

void PopupPrison::onPopupAdsInterstitial()
{
    if ( _eType == PrisonManager::PrisonType::DEFAULT && PrisonManager::getInstance()->getDungeonCount() == 0 )
    {
        auto popup = PopupAdsInterstitial::create(1);
        popup->setCallbackResult(CC_CALLBACK_1(PopupPrison::callbackAdsInterstitial, this));
        popup->show();
    }
}
