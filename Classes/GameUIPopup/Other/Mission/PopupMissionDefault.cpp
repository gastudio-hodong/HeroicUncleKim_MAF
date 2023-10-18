//
//  PopupMissionDefault.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/26.
//

#include "PopupMissionDefault.h"

#include "Common/ConfigKey.h"

#include "GameScene/HelloWorldScene.h"

#include "GameObject/InfoBadge.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/Pass/DungeonPass/PopupDungeonPass.h"
#include "GameUIPopup/Other/Pass/DungeonPass/PopupDungeonPassBadgeInfo.h"
#include "GameUIPopup/Other/Mission/PopupNormalAchievement.h"
#include "GameUIPopup/Other/Mission/Layer/LayerDailyMission.h"
#include "GameUIPopup/Other/Mission/Layer/LayerSpecialMission.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/BadgeManager.h"
#include "ManagerGame/ServerMissionManager.h"
#include "ManagerGame/SpecialMissionManager.h"
#include "ManagerGame/PassManager.h"
#include "ManagerGame/AchievementManager.h"

USING_NS_CC;

PopupMissionDefault* PopupMissionDefault::create(E_MISSION eType)
{
    PopupMissionDefault* pRet = new(std::nothrow) PopupMissionDefault();
    if ( pRet && pRet->init(eType) )
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
PopupMissionDefault::PopupMissionDefault():
_layerContainerTopBanner(nullptr)
,_layerContainerTop(nullptr)
,_layerContainerTab(nullptr)
,_layerContentsArea(nullptr)
,_layerContainerDaily(nullptr)
,_layerContainerWeekly(nullptr)
,_layerContainerSpecial(nullptr)
,_lyNowContents(nullptr)
,_eType(E_MISSION::SPECIAL)
{
    
}
PopupMissionDefault::~PopupMissionDefault()
{
}
bool PopupMissionDefault::init(E_MISSION eType)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    _eType = eType;
    if ( AccountManager::getInstance()->getUserIdx() == 0 )
    {
        _eType = E_MISSION::SPECIAL;
    }
    
    SpecialMissionManager::getInstance()->checkSpeicalMission(E_PLACE::DG_NORMAL);
    //
    initVar();
    initUI();
    
    return true;
}

void PopupMissionDefault::onEnter()
{
    PopupBase::onEnter();
    
    //this->scheduleUpdate();
}
void PopupMissionDefault::update(float dt)
{
    
}
#pragma mark -ui
void PopupMissionDefault::initVar()
{
    
}
void PopupMissionDefault::initUI()
{
    double nHeight = getContentSize().height;
    if ( nHeight > 1150 )
        nHeight = 1150;
    
    _spriteContainer->setContentSize(Size(729, nHeight));
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(0);
    
    _layerContainerTopBanner = LayerColor::create();
    _layerContainerTopBanner->setContentSize( Size(_spriteContainer->getContentSize().width, getContentSize().height - _spriteContainer->getContentSize().height - 10));
    _layerContainerTopBanner->setIgnoreAnchorPointForPosition(false);
    _layerContainerTopBanner->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerTopBanner->setPosition( _spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height);
    _spriteContainer->addChild(_layerContainerTopBanner);
    
    _layerContainerTab = Layer::create();
    _layerContainerTab->setContentSize( Size(_spriteContainer->getContentSize().width, 80));
    _layerContainerTab->setIgnoreAnchorPointForPosition(false);
    _layerContainerTab->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerTab->setPosition( _spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 );
    _spriteContainer->addChild(_layerContainerTab);
    
    _layerContainerTop = LayerColor::create();
    _layerContainerTop->setContentSize( Size(_spriteContainer->getContentSize().width, 170));
    _layerContainerTop->setIgnoreAnchorPointForPosition(false);
    _layerContainerTop->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerTop->setPosition(_layerContainerTab->getPosition() + Vec2::DOWN * _layerContainerTab->getContentSize().height);
    _spriteContainer->addChild(_layerContainerTop);
    
    _layerContentsArea = Layer::create();
    _layerContentsArea->setContentSize( _spriteContainer->getContentSize() - Size(58, _layerContainerTop->getContentSize().height + 85 + _layerContainerTab->getContentSize().height) );
    _layerContentsArea->setIgnoreAnchorPointForPosition(false);
    _layerContentsArea->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContentsArea->setPosition( _spriteContainer->getContentSize().width / 2, 15 );
    _spriteContainer->addChild(_layerContentsArea);
    
    _layerContainerSpecial = Layer::create();
    _layerContainerSpecial->setContentSize( _spriteContainer->getContentSize() - Size(58, _layerContainerTab->getContentSize().height + 100) );
    _layerContainerSpecial->setIgnoreAnchorPointForPosition(false);
    _layerContainerSpecial->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerSpecial->setPosition( _spriteContainer->getContentSize().width / 2, 5 );
    _spriteContainer->addChild(_layerContainerSpecial);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_mission_title"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupMissionDefault::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    uiReload();
}

void PopupMissionDefault::uiReload()
{
    uiTopBanner();
    uiTab();
    uiTop();
    uiLoadLayer();
}

void PopupMissionDefault::uiLoadLayer()
{
    if(_lyNowContents != nullptr)
        _lyNowContents->setVisible(false);
    
    switch(_eType)
    {
        case E_MISSION::DAILY:
        case E_MISSION::WEEK:
        {
            uiServerMission();
        }break;
        case E_MISSION::SPECIAL:
        {
            uiSpecialMission();
        }break;
    }
    
    _lyNowContents->setVisible(true);
}
void PopupMissionDefault::uiTopBanner()
{
    switch(_eType)
    {
        case E_MISSION::DAILY:
        case E_MISSION::WEEK:
        {
            uiTopBannerDailyMission();
        }break;
        case E_MISSION::SPECIAL:
        {
            uiTopBannerSpecialMission();
        }break;
    }
}
void PopupMissionDefault::uiTopBannerSpecialMission()
{
    _layerContainerTopBanner->removeAllChildren();
    
    bool isViewSpecialQuest = UserDefault::getInstance()->getBoolForKey(KEY_VIEW_SP_QUEST, true);
    
    std::string strPath = "Assets/ui/mission/mission_on.png";
    if(!isViewSpecialQuest)
        strPath = "Assets/ui/mission/mission_off.png";
    
    auto btnToggle = ui::Button::create(strPath);
    btnToggle->addClickEventListener([&, btnToggle](Ref* sender){
        bool isViewSpecialQuest = UserDefault::getInstance()->getBoolForKey(KEY_VIEW_SP_QUEST, true);
        isViewSpecialQuest = !isViewSpecialQuest;
        
        UserDefault::getInstance()->setBoolForKey(KEY_VIEW_SP_QUEST, isViewSpecialQuest);
        HelloWorldScene::getInstance()->reDrawSpecialQuest();
        
        if(isViewSpecialQuest)
            btnToggle->getRendererNormal()->setTexture("Assets/ui/mission/mission_on.png");
        else
            btnToggle->getRendererNormal()->setTexture("Assets/ui/mission/mission_off.png");
        
    });
    btnToggle->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    btnToggle->setPosition(Vec2(_layerContainerTopBanner->getContentSize().width, 5));
    _layerContainerTopBanner->addChild(btnToggle);
    
    auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_mission_btn_1"), GAME_FONT, 24);
    lbText->setColor(Color3B::WHITE);
    lbText->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    lbText->setPosition(btnToggle->getContentSize() + Size(0,10));
    btnToggle->addChild(lbText);
    
}
void PopupMissionDefault::uiTopBannerDailyMission()
{
    _layerContainerTopBanner->removeAllChildren();
    
    auto objMission = SpecialMissionManager::getInstance()->getInfoSpecialMission(SpecialMissionManager::getInstance()->getSpecialMissionClear()+1);
    
    if(objMission == nullptr)
        return;
    
    auto sprBox = Sprite::create("Assets/ui/mission/mission_special_3.png");
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprBox->setPosition(_layerContainerTopBanner->getContentSize().width/2, 0);
    _layerContainerTopBanner->addChild(sprBox);
    
    auto lbTitle = Label::createWithTTF(GAME_TEXTFORMAT("t_new_mission_title", objMission->getIdx()), GAME_FONT, 23);
    lbTitle->setColor(Color3B::WHITE);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbTitle->setPosition( Vec2(sprBox->getContentSize().width * 0.2, sprBox->getContentSize().height *0.7) );
    sprBox->addChild(lbTitle);
    
    auto lbContent = Label::createWithTTF(objMission->getContent(), GAME_FONT, 28);
    lbContent->setColor(Color3B(92, 203, 255));
    lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbContent->setDimensions(370, sprBox->getContentSize().height/2);
    lbContent->setOverflow(Label::Overflow::SHRINK);
    lbContent->setPosition( Vec2(sprBox->getContentSize().width * 0.2, sprBox->getContentSize().height *0.34) );
    sprBox->addChild(lbContent);
    
    
    std::string strPath = "";
    auto callFunc = CC_CALLBACK_1(PopupMissionDefault::onClickSpecialMissionShortCut, this);
    if(SpecialMissionManager::getInstance()->getSpecialMissionComplete() == 1)
    {
        strPath = "Assets/ui/mission/btn_acheive_1.png";
        callFunc = CC_CALLBACK_1(PopupMissionDefault::onClickSpecialMissionComplete, this);
    }
    else
    {
        strPath = "Assets/ui/mission/btn_shortcuts_1.png";
    }
    
    auto itemReceive = ui::Button::create(strPath);
    itemReceive->addClickEventListener(callFunc);
    itemReceive->setPosition(Vec2(sprBox->getContentSize().width*0.86, sprBox->getContentSize().height*0.5));
    itemReceive->setTag(objMission->getIdx());
    sprBox->addChild(itemReceive);
}
void PopupMissionDefault::uiTab()
{
    _layerContainerTab->removeAllChildren();
    
    std::vector<std::string> listMissionOnPath =
    {
        "Assets/ui/mission/btn_daily_on.png",
        "Assets/ui/mission/btn_weekly_on.png",
        "Assets/ui/mission/btn_special_on.png"
    };
    std::vector<std::string> listMissionOffPath =
    {
        "Assets/ui/mission/btn_daily_off.png",
        "Assets/ui/mission/btn_weekly_off.png",
        "Assets/ui/mission/btn_special_off.png"
    };
    
    Node* tmpNode = nullptr;
    float posX = 35;
    for( int i = 0; i < listMissionOnPath.size(); i++ )
    {
        auto strOnPath = listMissionOnPath[i];
        auto strOffPath = listMissionOffPath[i];
        std::string strPath = strOffPath;
        
        if(_eType == (E_MISSION)i)
            strPath = strOnPath;
        
        auto btnButton = ui::Button::create(strPath);
        btnButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        btnButton->setPosition(Vec2(posX, _layerContainerTab->getContentSize().height / 2 ));
        btnButton->setTag(i);
        btnButton->addClickEventListener(CC_CALLBACK_1(PopupMissionDefault::onClickTab, this));
        _layerContainerTab->addChild(btnButton);
        tmpNode = btnButton;
        
        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
        uiReddot->setPosition(10, btnButton->getContentSize().height-10);
        uiReddot->setVisible(false);
        btnButton->addChild(uiReddot);
        
        auto action1 = CallFunc::create([&, uiReddot, i](){
            
            switch((E_MISSION)i)
            {
                case E_MISSION::DAILY:
                {
                    uiReddot->setVisible(ServerMissionManager::getInstance()->isServerMissionCompleteDaily());
                }break;
                case E_MISSION::WEEK:
                {
                    uiReddot->setVisible(ServerMissionManager::getInstance()->isServerMissionCompleteWeekly());
                }break;
                case E_MISSION::SPECIAL:
                {
                    uiReddot->setVisible(SpecialMissionManager::getInstance()->isCompleteSpeicalMission(E_PLACE::DG_NORMAL));
                }break;
            }
        });
        auto action2 = DelayTime::create(1);
        auto seq = Sequence::create(action1, action2, nullptr);
        uiReddot->runAction(RepeatForever::create(seq));
        
        posX += btnButton->getContentSize().width + 5;
    };
    
    auto btnAchive = ui::Button::create("Assets/ui/mission/btn_achievements_on.png", "Assets/ui/mission/btn_achievements_on.png");
    btnAchive->addClickEventListener(CC_CALLBACK_1(PopupMissionDefault::onClickAchievement, this));
    btnAchive->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnAchive->setPosition(Vec2(posX, _layerContainerTab->getContentSize().height / 2));
    _layerContainerTab->addChild(btnAchive);
        
    if(AchievementManager::getInstance()->isRewardCondition())
    {
        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_RIGHT);
        uiReddot->setPosition(btnAchive->getContentSize().width + 10, btnAchive->getContentSize().height + 10);
        btnAchive->addChild(uiReddot);
    }
}
void PopupMissionDefault::uiTop()
{
    _layerContainerTop->removeAllChildren();
    if(_eType == E_MISSION::SPECIAL)
    {
        _layerContainerTop->setVisible(false);
        return;
    }
    else
        _layerContainerTop->setVisible(true);
    
    
    auto sprTimer = Sprite::create("Assets/ui/prison/clock.png");
    sprTimer->setPosition(Vec2(65, _layerContainerTop->getContentSize().height - 10));
    _layerContainerTop->addChild(sprTimer);
    
    auto lbTimer = Label::createWithTTF("", GAME_FONT, 20);
    lbTimer->setPosition(Vec2(sprTimer->getContentSize().width, sprTimer->getContentSize().height/2) + Vec2::RIGHT * 10);
    lbTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbTimer->setColor(Color3B::WHITE);
    sprTimer->addChild(lbTimer);
    {
        auto callFunc = CallFunc::create([&, lbTimer](){
            time_t nTimeEnd = 0;
            if(_eType == E_MISSION::DAILY)
                nTimeEnd = ServerMissionManager::getInstance()->getResetTime(E_MISSION_RESET_TYPE::DAILY);
            else if(_eType == E_MISSION::WEEK)
                nTimeEnd = ServerMissionManager::getInstance()->getResetTime(E_MISSION_RESET_TYPE::WEEK);
            int64_t nTimeNow = UtilsDate::getInstance()->getTime();
                        
            int nTimeDiff = (int)(nTimeEnd - nTimeNow);
            int d = nTimeDiff / 24 / 3600;
            int h = nTimeDiff / 3600;
            int m = nTimeDiff % 3600 / 60;
            
            if(d >= 1 )
                h %= 24;
            
            if(d < 0)
                d=0;
            if(h < 0)
                h=0;
            if(m < 0)
                m=0;
            
            if(_eType == E_MISSION::DAILY)
                lbTimer->setString(GAME_TEXTFORMAT("t_server_dailymission_dayleft", h, m));
            else if(_eType == E_MISSION::WEEK)
                lbTimer->setString(GAME_TEXTFORMAT("t_server_dailymission_weekleft", d, h, m));
        });
        auto seq = Sequence::create(callFunc, DelayTime::create(60), nullptr);
        lbTimer->runAction(RepeatForever::create(seq));
    }
    
    uiTopPass();
    
    Sprite* sprInfo = Sprite::create("Assets/ui/mission/mission_list.png");
    sprInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprInfo->setPosition(_layerContainerTop->getContentSize().width/2, 5);
    _layerContainerTop->addChild(sprInfo);
    {
        auto labelTitle1 = Label::createWithTTF(GAME_TEXT("t_ui_mission_title"), GAME_FONT, 24);
        labelTitle1->setPosition( Vec2(210, sprInfo->getContentSize().height / 2) );
        sprInfo->addChild(labelTitle1);
        
        auto labelTitle2 = Label::createWithTTF(GAME_TEXT("t_ui_reward"), GAME_FONT, 24);
        labelTitle2->setPosition( Vec2(565, sprInfo->getContentSize().height / 2) );
        sprInfo->addChild(labelTitle2);
    }
}
void PopupMissionDefault::uiTopPass()
{
    auto objPass = PassManager::getInstance()->getPassInfo(E_PASS_TYPE::PASS_DUNGEON);
    
    //하단 백그라운드 배경 이미지
    auto btnPassBG = ui::Button::create("Assets/ui/purchase/pass_progress_lv_bar.png");
    btnPassBG->addClickEventListener(CC_CALLBACK_1(PopupMissionDefault::onClickPass, this));
    btnPassBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    btnPassBG->setPosition(_layerContainerTop->getContentSize()/2);
    _layerContainerTop->addChild(btnPassBG);
    {
        auto lbProgressContent = Label::createWithTTF("", GAME_FONT, 25);
        if ( objPass != nullptr )
            lbProgressContent->setString(GAME_TEXTFORMAT("t_ui_dungeonpass_progress", objPass->getLevel()));
        else
            lbProgressContent->setString(GAME_TEXTFORMAT("t_ui_dungeonpass_progress", 0));
            
        lbProgressContent->setPosition(Vec2(130, btnPassBG->getContentSize().height - 8));
        lbProgressContent->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        lbProgressContent->setColor(Color3B(255,252,178));
        lbProgressContent->setName("lbContent");
        btnPassBG->addChild(lbProgressContent);
    }
    
    //프로그래스 회색 바
    auto sprProgressBack = Sprite::create("Assets/ui/purchase/pass_progress_1_1.png");
    sprProgressBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprProgressBack->setPosition(Vec2(btnPassBG->getContentSize().width/2 - 50, 8));
    btnPassBG->addChild(sprProgressBack);
    {
        //프로그래스 초록 바
        auto sprProgressFront = Sprite::create("Assets/ui/purchase/pass_progress_1_2.png");
        sprProgressFront->setAnchorPoint(Vec2(0,0.5));
        
        auto passProgressBar = ProgressTimer::create(sprProgressFront);
        passProgressBar->setType(ProgressTimer::Type::BAR);
        passProgressBar->setMidpoint(Vec2(0, 0.5));
        passProgressBar->setBarChangeRate(Vec2(1, 0));
        passProgressBar->setAnchorPoint(Vec2(0, 0));
        sprProgressBack->addChild(passProgressBar);
        
        auto lbPassProgressRate = Label::createWithTTF("", GAME_FONT, 20);
        lbPassProgressRate->setPosition(sprProgressBack->getContentSize()/2);
        sprProgressBack->addChild(lbPassProgressRate);
        
        if( objPass != nullptr)
        {
            if ( objPass->isLevelFinish() == true )
            {
                passProgressBar->setPercentage(100);
                lbPassProgressRate->setString("MAX");
            }
            else
            {
                double nPercent = (double)objPass->getExp() / objPass->getExpNext() * 100;
                if ( nPercent > 100 )
                    nPercent = 100;
                
                passProgressBar->setPercentage(nPercent);
                lbPassProgressRate->setString(GAME_TEXTFORMAT("t_ui_friend_info_1", objPass->getExp(), objPass->getExpNext()));
            }
        }
        else
        {
            passProgressBar->setPercentage(0);
            lbPassProgressRate->setString("MAX");
        }
    }
    
    //다음 레벨 버튼 같은 이미지
    auto lbPassProgressLv = Label::createWithTTF("LV.0", GAME_FONT, 25);
    lbPassProgressLv->setPosition(sprProgressBack->getPosition() + Vec2::RIGHT * (sprProgressBack->getContentSize().width/2 + 10));
    lbPassProgressLv->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    lbPassProgressLv->setAlignment(TextHAlignment::LEFT, TextVAlignment::BOTTOM);
    lbPassProgressLv->setColor(Color3B(128, 128, 128));
    
    if(objPass != nullptr)
    {
        auto menuBadge = MafNode::MafMenu::create();
        menuBadge->setAnchorPoint(Vec2::ZERO);
        menuBadge->setPosition(Vec2::ZERO);
        btnPassBG->addChild(menuBadge);
        
        auto lyBadge = Layer::create();
        lyBadge->setContentSize(Size(115, 70));
        lyBadge->setIgnoreAnchorPointForPosition(false);
        {
            auto objBadgeFree = BadgeManager::getInstance()->getBadge(objPass->getBadgeFree());
            auto sprBadgeFree = Sprite::create(objBadgeFree->getPath());
            sprBadgeFree->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprBadgeFree->setPosition(Vec2(lyBadge->getContentSize()/2) + Vec2::RIGHT * 20);
            lyBadge->addChild(sprBadgeFree);
            
            auto objBadgePur = BadgeManager::getInstance()->getBadge(objPass->getBadgePur());
            auto sprBadgePur = Sprite::create(objBadgePur->getPath());
            sprBadgePur->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprBadgePur->setPosition(Vec2(lyBadge->getContentSize()/2) + Vec2::LEFT * 20);
            lyBadge->addChild(sprBadgePur);
        }
        
        auto itemBadge = MafNode::MafMenuItemSprite::create(lyBadge, nullptr, CC_CALLBACK_1(PopupMissionDefault::onClickPassBadge, this));
        itemBadge->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        itemBadge->setPosition(15, btnPassBG->getContentSize().height/2);
        menuBadge->addChild(itemBadge, E_ZORER::UI);
    }
    
    if ( objPass != nullptr )
        lbPassProgressLv->setString( "LV." + MafUtils::toString(objPass->getLevel() + 1));
        
    if ( objPass == nullptr || objPass->getListStep().size() <= objPass->getLevel() + 1 )
            lbPassProgressLv->setString("MAX");
            
    btnPassBG->addChild(lbPassProgressLv);
    
    // reddot
    {
        auto redDot = UIReddot::create(E_REDDOT_TYPE::NOMAL);
        redDot->setPosition(btnPassBG->getContentSize().width - 93, btnPassBG->getContentSize().height - 30);
        redDot->setVisible(false);
        btnPassBG->addChild(redDot);
        RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::DUNGEONPASS_REWARD, redDot);
    }
    
}
void PopupMissionDefault::uiServerMission()
{
    switch(_eType)
    {
        case E_MISSION::DAILY:
        {
            if(_layerContainerDaily == nullptr)
            {
                auto layer = LayerDailyMission::create(E_MISSION_RESET_TYPE::DAILY, _layerContentsArea->getContentSize());
                layer->setCallbackHide(CC_CALLBACK_0(PopupMissionDefault::callbackHide, this));
                layer->setCallbackRefresh(CC_CALLBACK_0(PopupMissionDefault::uiReload, this));
                layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                layer->setPosition(_layerContentsArea->getContentSize()/2);
                _layerContentsArea->addChild(layer);

                _layerContainerDaily = layer;
            }
            _lyNowContents =  _layerContainerDaily;

            ((LayerDailyMission*) _lyNowContents)->requestUpdate();
        }break;
        case E_MISSION::WEEK:
        {
            if(_layerContainerWeekly == nullptr)
            {
                auto layer = LayerDailyMission::create(E_MISSION_RESET_TYPE::WEEK, _layerContentsArea->getContentSize());
                layer->setCallbackHide(CC_CALLBACK_0(PopupMissionDefault::callbackHide, this));
                layer->setCallbackRefresh(CC_CALLBACK_0(PopupMissionDefault::uiReload, this));
                layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                layer->setPosition(_layerContentsArea->getContentSize()/2);
                _layerContentsArea->addChild(layer);

                _layerContainerWeekly = layer;
            }
            _lyNowContents =  _layerContainerWeekly;

            ((LayerDailyMission*) _lyNowContents)->requestUpdate();
        }break;

    }
}
void PopupMissionDefault::uiSpecialMission()
{
    auto layer = (Layer*)_layerContainerSpecial->getChildByName("SPECIAL");
    if(layer == nullptr)
    {
        auto uiLayer = LayerSpecialMission::create( _layerContainerSpecial->getContentSize());
        uiLayer->setCallbackHide(CC_CALLBACK_0(PopupMissionDefault::callbackHide, this));
        uiLayer->setCallbackRefresh(CC_CALLBACK_0(PopupMissionDefault::uiReload, this));
        uiLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiLayer->setPosition(_layerContainerSpecial->getContentSize()/2);
        uiLayer->setName("SPECIAL");
        _layerContainerSpecial->addChild(uiLayer);
        layer = uiLayer;
    }
    _lyNowContents =  layer;
    ((LayerSpecialMission*) _lyNowContents)->uiReload();
}

#pragma mark -onClick
void PopupMissionDefault::onClickTab(Ref* sender)
{
    auto node = (Node*)sender;
    
    _eType = (E_MISSION)node->getTag();
    uiReload();
}

void PopupMissionDefault::onClickClose(Ref* sender)
{
    hide();
}

void PopupMissionDefault::onClickPass(Ref* sender)
{
    if ( AccountManager::getInstance()->getUserIdx() == 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto popup = PopupDungeonPass::create();
    popup->show();
    
    //
    hide();
}

void PopupMissionDefault::onClickAchievement(Ref* sender)
{
    auto popup = PopupNormalAchievement::create();
    popup->show();
    hide();
}

void PopupMissionDefault::onClickPassBadge(Ref* sender)
{
    auto popup = PopupDungeonPassBadgeInfo::create();
    popup->show();
}

void PopupMissionDefault::onClickSpecialMissionShortCut(Ref* sender)
{
    auto node = (Node*)sender;

    //씬이동이 아니면 닫는다
    if(SpecialMissionManager::getInstance()->onShortCut(node->getTag()) != E_SHORTCUTS::SHORTCUT_PETEQUIP ||
       SpecialMissionManager::getInstance()->onShortCut(node->getTag()) != E_SHORTCUTS::SHORTCUT_DEVILDOM)
        hide();
}

void PopupMissionDefault::onClickSpecialMissionComplete(Ref* sender)
{
    auto node = (Node*)sender;
    auto objMission = SpecialMissionManager::getInstance()->getInfoSpecialMission(node->getTag());
    if(objMission == nullptr)
        return;
    
    PopupLoading::show();
    SpecialMissionManager::getInstance()->requestMissionReward(CC_CALLBACK_2(PopupMissionDefault::callbackSpecialMissionComplete, this), objMission->getIdx());
}

#pragma mark -utils
void PopupMissionDefault::showRedDotAction(E_MISSION type)
{
    
}

#pragma mark -callback
void PopupMissionDefault::callbackSetTab(E_MISSION type)
{
    uiReload();
}
void PopupMissionDefault::callbackHide()
{
    hide();
}
void PopupMissionDefault::callbackSpecialMissionComplete(bool bResult, int nResult)
{
    PopupLoading::hide();

    if(bResult == false || nResult != 1)
    {
        std::string strTextKey = "t_ui_error_2";
        if(nResult == 0)
            strTextKey = "t_ui_mail_msg_3";
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strTextKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();

    }
    SpecialMissionManager::getInstance()->checkSpeicalMission(E_PLACE::DG_NORMAL);
    
    uiTopBanner();
}
