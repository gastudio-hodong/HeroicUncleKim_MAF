//
//  UIRandomBox.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "UIRandomBox.h"

#include "GameObject/InfoItems.h"
#include "ManagerGame/UserInfoManager.h"

#include "GameUIPopup/Other/Ads/PopupAdsWatch.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "ManagerGame/DataManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

UIRandomBox* UIRandomBox::create(Size size, E_PLACE ePlace)
{
    UIRandomBox* pRet = new(std::nothrow) UIRandomBox();
    if ( pRet && pRet->init(size, ePlace) )
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

UIRandomBox::UIRandomBox(void) :
_ePlace(E_PLACE::EMPTY),

_timeShow(0),
_timeNow(0),

_nFloorPrev(0),
_bResetTime(true),

_bAdsReward(false),

//
_layerContainerContent(nullptr)
{
    
}

UIRandomBox::~UIRandomBox(void)
{
    
}

bool UIRandomBox::init(Size size, E_PLACE ePlace)
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    
    setIgnoreAnchorPointForPosition(false);
    setContentSize(size);
    setAnchorPoint( Vec2(0.5f, 0.5f) );
    
    //
    _ePlace = ePlace;
    
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void UIRandomBox::initVar()
{
    //
    setTime();
}

void UIRandomBox::initUi()
{
    //
    _layerContainerContent = Layer::create();
    _layerContainerContent->setContentSize(Director::getInstance()->getVisibleSize());
    _layerContainerContent->setIgnoreAnchorPointForPosition(false);
    _layerContainerContent->setAnchorPoint( Vec2::ZERO );
    addChild(_layerContainerContent);
}

void UIRandomBox::uiBox()
{
    //
    _layerContainerContent->removeAllChildren();
    
    SoundManager::SoundEffectStart(SOUND_BOX_CREATE);
    // run
    auto aniframeRun = Animation::create();
    if ( _bAdsReward == false )
    {
        aniframeRun->addSpriteFrameWithFile("Assets/ui/ads/btn_ad_mail1_1.png");
        aniframeRun->addSpriteFrameWithFile("Assets/ui/ads/btn_ad_mail1_2.png");
        aniframeRun->addSpriteFrameWithFile("Assets/ui/ads/btn_ad_mail1_3.png");
        aniframeRun->addSpriteFrameWithFile("Assets/ui/ads/btn_ad_mail1_4.png");
        aniframeRun->addSpriteFrameWithFile("Assets/ui/ads/btn_ad_mail1_5.png");
        aniframeRun->addSpriteFrameWithFile("Assets/ui/ads/btn_ad_mail1_6.png");
    }
    else
    {
        aniframeRun->addSpriteFrameWithFile("Assets/ui/ads/btn_ad_mail2_1.png");
        aniframeRun->addSpriteFrameWithFile("Assets/ui/ads/btn_ad_mail2_2.png");
        aniframeRun->addSpriteFrameWithFile("Assets/ui/ads/btn_ad_mail2_3.png");
        aniframeRun->addSpriteFrameWithFile("Assets/ui/ads/btn_ad_mail2_4.png");
        aniframeRun->addSpriteFrameWithFile("Assets/ui/ads/btn_ad_mail2_5.png");
        aniframeRun->addSpriteFrameWithFile("Assets/ui/ads/btn_ad_mail2_6.png");
    }
    
    aniframeRun->setDelayPerUnit(0.1f);
    
    auto aniRun = Animate::create(aniframeRun);
    auto sprintRun = Sprite::create("Assets/ui/ads/btn_ad_mail1_1.png");
    sprintRun->runAction(RepeatForever::create(aniRun));
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    _layerContainerContent->addChild(menu);
        
    auto itemBuy = MafNode::MafMenuItemSprite::create(sprintRun, nullptr, CC_CALLBACK_1(UIRandomBox::onClickBox, this));
    menu->addChild(itemBuy);
        
    auto visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::Vector<FiniteTimeAction*> _vecAction;
    
    itemBuy->setPosition(this->convertToNodeSpace(Vec2(0, visibleSize.height*0.5)));
    auto jump = JumpTo::create(15, Vec2(this->convertToNodeSpace(Vec2(visibleSize.width*1,visibleSize.height*0.6))), 200, 3);
    _vecAction.pushBack(jump);
    
        
    auto finishAction = CallFuncN::create( [=]( Ref* sender ) {
        auto item = (MafNode::MafMenuItemSprite*)sender;
        auto menu = (MafNode::MafMenu*)item->getParent();
        if ( menu != nullptr )
        {
            menu->removeFromParent();
            _bResetTime = false;
            setTime();
        }
    });
    
    _vecAction.pushBack(finishAction);
    auto seq = Sequence::create(_vecAction);
    itemBuy->runAction(seq);
}

#pragma mark - set, get, add
void UIRandomBox::setTime()
{
    _timeNow = 0;
    int nFloor = UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL);
    if ( _timeShow == 0 )
    {// 처음 시작시 4분 뒤 박스 출력
        _timeShow = 120;
    }
    else if ( _bResetTime == true )
    {// 두번째 부터
        if ( nFloor == _nFloorPrev )
        {// 같은 층수 일때 ((4분 ~ 6분) * 박스 출력수) 뒤 박스 출력
            _timeShow = random(180, 240);
        }
        else
        {// 다른 층수 일때 2분 ~ 4분 뒤 박스 출력
            _timeShow = random(120, 180);
        }
    }
    
    _nFloorPrev = nFloor;
    this->schedule(schedule_selector(UIRandomBox::scheduleTime), 1.0f);
}

void UIRandomBox::setReward(bool bAdsView)
{
    //
    _listItems.clear();
    
    E_ITEMS eType = E_ITEMS::NONE;
    std::string strCount = "0";
    
    //
    if ( bAdsView == true )
    {// 광고시청 보상
        int rand = random(1, 100);
        
        if ( rand <= 5 )            eType = E_ITEMS::CUBE;
        else if ( rand <= 15 )       eType = E_ITEMS::GEM;
        else if ( rand <= 35 )       eType = E_ITEMS::GOLD;
        else if ( rand <= 45 )       eType = E_ITEMS::POINT;
        else if ( rand <= 60 )       eType = E_ITEMS::KEY;
        else if ( rand <= 65 )       eType = E_ITEMS::BUFF_ATK_10;
        else if ( rand <= 75 )       eType = E_ITEMS::BUFF_ALL;
        else if ( rand <= 85 )       eType = E_ITEMS::MINE_BOX_PIECE;
        else if ( rand <= 95 )       eType = E_ITEMS::MINE_BOX_STONE;
        else                         eType = E_ITEMS::MINE_SPECIAL_BOX;
        
        if ( eType == E_ITEMS::CUBE )                   strCount = "50";
        else if ( eType == E_ITEMS::GEM )               strCount = "300";
        else if ( eType == E_ITEMS::POINT )             strCount = "75";
        else if ( eType == E_ITEMS::BUFF_ATK_10 )        strCount = "300";
        else if ( eType == E_ITEMS::BUFF_ALL )          strCount = "300";
        else if ( eType == E_ITEMS::MINE_BOX_PIECE )    strCount = "400";
        else if ( eType == E_ITEMS::MINE_BOX_STONE )    strCount = "1200";
        else if ( eType == E_ITEMS::MINE_SPECIAL_BOX )  strCount = "15";
        else if ( eType == E_ITEMS::GOLD )
        {
            strCount = UserInfoManager::getInstance()->getEarnGoldFinal(_ePlace, E_EARNGOLD::MON_KILL);
            strCount = MafUtils::bigMulNum(strCount, "600");
        }
        
        else if ( eType == E_ITEMS::KEY )
        {
            strCount = DataManager::getInstance()->GetPrestigeKey(_ePlace, UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL));
            strCount = MafUtils::bigMulNum(strCount, "1500");
            if ( strCount.length() > 3 )
            {
                strCount.pop_back();
                strCount.pop_back();
                strCount.pop_back();
            }
            else
            {
                strCount = "1";
            }
        }
    }
    else
    {// 일반 보상
        int rand = random(1, 100);
        
        if ( rand <= 50 )            eType = E_ITEMS::GOLD;
        else if ( rand <= 60 )       eType = E_ITEMS::KEY;
        else if ( rand <= 70 )       eType = E_ITEMS::BUFF_ATK;
        else if ( rand <= 80 )       eType = E_ITEMS::BUFF_COIN;
        else if ( rand <= 90 )       eType = E_ITEMS::BUFF_SPD;
        else if ( rand <= 95 )       eType = E_ITEMS::MINE_BOX_PIECE;
        else                         eType = E_ITEMS::MINE_BOX_STONE;
        
        if ( eType == E_ITEMS::BUFF_ATK )           strCount = "60";
        else if ( eType == E_ITEMS::BUFF_COIN )     strCount = "60";
        else if ( eType == E_ITEMS::BUFF_SPD )      strCount = "60";
        else if ( eType == E_ITEMS::MINE_BOX_PIECE )      strCount = "50";
        else if ( eType == E_ITEMS::MINE_BOX_STONE )      strCount = "200";
        else if ( eType == E_ITEMS::GOLD )
        {
            strCount = UserInfoManager::getInstance()->getEarnGoldFinal(_ePlace, E_EARNGOLD::MON_KILL);
            strCount = MafUtils::bigMulNum(strCount, "25");
        }
        else if ( eType == E_ITEMS::KEY )
        {
            strCount = DataManager::getInstance()->GetPrestigeKey(_ePlace, UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL));
            strCount = MafUtils::bigMulNum(strCount, "100");
            if ( strCount.length() > 3 )
            {
                strCount.pop_back();
                strCount.pop_back();
                strCount.pop_back();
            }
            else
            {
                strCount = "1";
            }
        }
    }
  
    //
    if ( eType == E_ITEMS::MINE_BOX || eType == E_ITEMS::MINE_SPECIAL_BOX || eType == E_ITEMS::MINE_BOX_PIECE || eType == E_ITEMS::MINE_BOX_STONE )
    {
        auto obj = ItemsManager::getInstance()->getInfoItems(eType);
        
        auto listMine = ItemsManager::getInstance()->getConvertArray(obj->getReward());
        for ( auto& objMine : listMine )
        {
            objMine->setCount(strCount);
            
            _listItems.pushBack(objMine);
        }
    }
    else
    {
        auto obj = ItemsManager::getInstance()->getInfoItems(eType);
        obj->setCount(strCount);
        
        _listItems.pushBack(obj);
    }
}

#pragma mark - schedule
void UIRandomBox::scheduleTime(float dt)
{
    // time
    _timeNow += dt;
    
    if ( _timeNow > _timeShow )
    {
        //
        _bAdsReward = false;
        if ( random(1, 100) < 40 )
        {
            _bAdsReward = true;
        }
        
        if ( _bAdsReward == true )
        {
            std::string strPlayTime = SaveManager::getHistoryData(E_HISTORY_DATA::K_PLAY_TIME);
            long nTime = atol(strPlayTime.c_str());
            int nTimeMin = (int)nTime / 60;
            if ( nTimeMin < 20 )
                _bAdsReward = false;
        }
        
        // 보상 설정
        setReward(_bAdsReward);
        
        //
        uiBox();
        this->unschedule(schedule_selector(UIRandomBox::scheduleTime));
    }
}

#pragma mark - callback
void UIRandomBox::callbackAds(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotMailBox, CashManager::getInstance()->isAdsfreeTime());
        
        //
        onReward();
    }
    else
    {
        //
        if ( result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(UIRandomBox::callbackAds, this));
            popup->setLogParams(kAnalEventAdsLoadParameterSlot);
            popup->setLogValue(kAdsSlotMailBox);
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
void UIRandomBox::onClickBox(cocos2d::Ref* sender)
{
    if(UserInfoManager::getInstance()->getRevival(E_PLACE::DG_NORMAL) == 1 &&
       UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) <= 100)
    {
        MafAnalyticsManager::LogEvent(kAnalEventNewbieBoxClick, kRepeatTrue);
    }
    //
    auto item = (MafNode::MafMenuItemSprite*)sender;
    auto menu = (MafNode::MafMenu*)item->getParent();
    if ( menu != nullptr )
    {
        menu->removeFromParent();
        
        // time
        _bResetTime = true;
        setTime();
    }
        
    // 광고시청 및 일반보상 지급
    if ( _bAdsReward == true )
    {// 광고 시청
        if ( CashManager::getInstance()->isAdsfreeTime() == true )
        {
            callbackAds(ADS_RESULT::SUCCESS);
        }
        else
        {
            auto obj = _listItems.at(0);
            
            E_ADS_WATCH eType = E_ADS_WATCH::NONE;
            switch ((E_ITEMS)obj->getIdx()) {
                case E_ITEMS::BUFF_COIN :       	eType = E_ADS_WATCH::BUFF_COIN; break;
                case E_ITEMS::BUFF_ATK :            eType = E_ADS_WATCH::BUFF_ATK; break;
                case E_ITEMS::BUFF_SPD :            eType = E_ADS_WATCH::BUFF_SPD; break;
                case E_ITEMS::BUFF_ALL : 	        eType = E_ADS_WATCH::BUFF_ALL; break;
                case E_ITEMS::BUFF_ATK_9 :          eType = E_ADS_WATCH::BUFF_ATK_9; break;
                case E_ITEMS::BUFF_ATK_10 :         eType = E_ADS_WATCH::BUFF_ATK_10; break;
                case E_ITEMS::GOLD :            	eType = E_ADS_WATCH::ITEM_GOLD; break;
                case E_ITEMS::GEM : 	            eType = E_ADS_WATCH::ITEM_GEM; break;
                case E_ITEMS::CUBE :                eType = E_ADS_WATCH::ITEM_CUBE; break;
                case E_ITEMS::KEY :                 eType = E_ADS_WATCH::ITEM_KEY; break;
                case E_ITEMS::POINT :               eType = E_ADS_WATCH::ITEM_POINT; break;
                    
                    
                case E_ITEMS::MINE_STONE_IRON :
                case E_ITEMS::MINE_STONE_COPPER :
                case E_ITEMS::MINE_STONE_SILVER :
                    eType = E_ADS_WATCH::MINE_RAND_5; break;
                case E_ITEMS::MINE_PIECE_IRON :
                case E_ITEMS::MINE_PIECE_COPPER :
                case E_ITEMS::MINE_PIECE_SILVER :
                    eType = E_ADS_WATCH::MINE_RAND_6; break;
                    
                case E_ITEMS::MINE_SAPPHIRE :
                case E_ITEMS::MINE_OPAL :
                case E_ITEMS::MINE_AQUAMARINE :
                case E_ITEMS::MINE_EMERALD :
                case E_ITEMS::MINE_AMETHYST :
                    eType = E_ADS_WATCH::MINE_RAND_2; break;
                    
                default:
                    break;
            }
            
            if ( eType != E_ADS_WATCH::NONE )
            {
                auto popup = PopupAdsWatch::create(eType, 0);
                popup->setCallbackResult([=](){
                    MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(UIRandomBox::callbackAds, this));
                });
                popup->show();
            }
        }
    }
    else
    {// 일반보상 지급
        callbackAds(ADS_RESULT::SUCCESS);
    }
}


void UIRandomBox::onReward()
{
    ItemsManager::getInstance()->addItems(_listItems);
    
    //
    if(UserInfoManager::getInstance()->getRevival(E_PLACE::DG_NORMAL) == 1 && UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) <= 100)
    {
        MafAnalyticsManager::LogEvent(kAnalEventNewbieBoxPop, kRepeatTrue);
    }
}
