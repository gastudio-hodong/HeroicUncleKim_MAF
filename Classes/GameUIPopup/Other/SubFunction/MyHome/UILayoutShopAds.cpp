//
//  UILayoutShopAds.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "UILayoutShopAds.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/ItemsMoneyManager.h"

#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"
#include "GameUIPopup/Other/Ads/PopupAdsWatch.h"

#include "ManagerGame/SaveManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/ServerMissionManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/MineralManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

UILayoutShopAds* UILayoutShopAds::create(E_PLACE ePlace)
{
    UILayoutShopAds* pRet = new(std::nothrow) UILayoutShopAds();
    if ( pRet && pRet->init(ePlace) )
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

UILayoutShopAds::UILayoutShopAds(void) :
_layerContainer(nullptr)
{
    
}

UILayoutShopAds::~UILayoutShopAds(void)
{
    
}

bool UILayoutShopAds::init(E_PLACE ePlace)
{
    if ( !Layout::init() )
    {
        return false;
    }
    
    //
    _ePlace = ePlace;
    
    //
    initVar();
    initUI();
    
    return true;
}

#pragma mark - init
void UILayoutShopAds::initVar()
{
    if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        _listShopAds.push_back(E_SHOP_ADS::SHOP_D_GEM_ADS);
        _listShopAds.push_back(E_SHOP_ADS::SHOP_D_GOLD_ADS);
        _listShopAds.push_back(E_SHOP_ADS::SHOP_D_BUFF_ADS);
        
        _listShopAdsIcon.push_back("Assets/ui/myhome/btn_ad_item6.png");
        _listShopAdsIcon.push_back("Assets/ui/myhome/btn_ad_item7.png");
        _listShopAdsIcon.push_back("Assets/ui/myhome/btn_ad_item8.png");
    }
    else
    {
        _listShopAds.push_back(E_SHOP_ADS::SHOP_GEM_ADS);
        _listShopAds.push_back(E_SHOP_ADS::SHOP_GOLD_ADS);
        _listShopAds.push_back(E_SHOP_ADS::SHOP_BUFF_ADS);
        _listShopAds.push_back(E_SHOP_ADS::SHOP_MINE_ADS);
        
        _listShopAdsIcon.push_back("Assets/ui/myhome/btn_ad_item1.png");
        _listShopAdsIcon.push_back("Assets/ui/myhome/btn_ad_item2.png");
        _listShopAdsIcon.push_back("Assets/ui/myhome/btn_ad_item3.png");
        _listShopAdsIcon.push_back("Assets/ui/myhome/btn_ad_item4.png");
    }
    
}

void UILayoutShopAds::initUI()
{
    double width = 178;
    double height = 97;
    width *= _listShopAds.size();
    
    //
    setContentSize(Size(width, height));
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    // ui
    uiShopAds();
    
    // draw
    drawShopAds();
    schedule(schedule_selector(UILayoutShopAds::drawShopAds), 1);
}

#pragma mark - ui
void UILayoutShopAds::uiShopAds()
{
    Size size = getContentSize();
    
    _layerContainer = ui::Layout::create();
    _layerContainer->setContentSize(size);
    _layerContainer->setLayoutType(ui::Layout::Type::HORIZONTAL);
    this->addChild(_layerContainer);
    
    for ( int i = 0; i < _listShopAds.size(); i++ )
    {
        E_SHOP_ADS eType = _listShopAds.at(i);
        std::string pathIcon = _listShopAdsIcon.at(i);
        std::string name = DataManager::GetShopAdsName(eType);
        
        auto uiBtn = ui::Button::create("Assets/ui/myhome/btn_myhome_ad1_1.png", "", "Assets/ui/myhome/btn_myhome_ad1_1.png");
        uiBtn->addClickEventListener(CC_CALLBACK_1(UILayoutShopAds::onClickShopAds, this));
        uiBtn->setZoomScale(0.05f);
        uiBtn->setTag(i);
        _layerContainer->addChild(uiBtn);
        
        auto uiImgMask = Sprite::create("Assets/ui/myhome/btn_myhome_ad1_2.png");
        uiImgMask->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiImgMask->setPosition(Vec2(0, uiBtn->getContentSize().height));
        uiImgMask->setVisible(false);
        uiImgMask->setName("UI_IMG_MASK");
        uiBtn->addChild(uiImgMask);
        
        auto uiImgIcon = ui::ImageView::create(pathIcon);
        uiImgIcon->setPosition(Vec2(uiBtn->getContentSize().width / 2, uiBtn->getContentSize().height / 2 + 10));
        uiBtn->addChild(uiImgIcon);
            
        auto uiImgAdsIcon = ui::ImageView::create("Assets/ui/myhome/myhome_ad_icon1.png");
        uiImgAdsIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiImgAdsIcon->setPosition(Vec2(uiBtn->getContentSize().width * 0.35, 5));
        uiImgAdsIcon->setScale(1.2);
        uiBtn->addChild(uiImgAdsIcon);
        
        auto uiTextAdsName = ui::CText::create(name, GAME_FONT, 20);
        uiTextAdsName->setAnchorPoint(Vec2::ZERO);
        uiTextAdsName->setPosition(Vec2(uiBtn->getContentSize().width * 0.44, 0));
        uiTextAdsName->setName("UI_TEXT_NAME");
        uiBtn->addChild(uiTextAdsName);
    }
}

#pragma mark - draw
void UILayoutShopAds::drawShopAds(float dt/* = 0*/)
{
    for ( auto& child : _layerContainer->getChildren() )
    {
        E_SHOP_ADS eType = _listShopAds.at(child->getTag());
        
        auto uiBtn = (ui::Button*)child;
        auto uiTextAdsName = uiBtn->getChildByName<ui::CText*>("UI_TEXT_NAME");
        auto uiImgMask = uiBtn->getChildByName<Sprite*>("UI_IMG_MASK");
        
        int time = UserInfoManager::getInstance()->getAdsTime(eType);
        int timeMax = TIME_ADS_GEM_BONUS;
        switch(eType)
        {
            case E_SHOP_ADS::SHOP_GEM_ADS:     timeMax = TIME_ADS_GEM_BONUS;       break;
            case E_SHOP_ADS::SHOP_GOLD_ADS:     timeMax = TIME_ADS_GOLD_BONUS;      break;
            case E_SHOP_ADS::SHOP_BUFF_ADS:     timeMax = TIME_ADS_BUFF_BONUS;      break;
            case E_SHOP_ADS::SHOP_MINE_ADS:     timeMax = TIME_ADS_MINE_BONUS;      break;
            default:
                break;
        }
       
        //
        std::string name = DataManager::GetShopAdsName(eType);
        std::string timeText = MafUtils::convertNumberToTime(time, false);

        if ( time > 0 )
        {
            uiBtn->setEnabled(false);
            uiTextAdsName->setString(timeText);
            
            if ( time > timeMax )
            {
                time = timeMax;
            }
            uiImgMask->setVisible(true);
            uiImgMask->setTextureRect(Rect(0, 0, uiImgMask->getContentSize().width, uiBtn->getContentSize().height * time / timeMax));
        }
        else
        {
            uiBtn->setEnabled(true);
            uiTextAdsName->setString(name);
            uiImgMask->setVisible(false);
        }
    }
}

#pragma mark - set, get, add

#pragma mark - click
void UILayoutShopAds::callbackShopAds(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        // analytics
        if( _eShopAds == E_SHOP_ADS::SHOP_GEM_ADS)
            MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotItemGem, CashManager::getInstance()->isAdsfreeTime());
        else if( _eShopAds == E_SHOP_ADS::SHOP_GOLD_ADS)
            MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotItemGold, CashManager::getInstance()->isAdsfreeTime());
        else if( _eShopAds == E_SHOP_ADS::SHOP_BUFF_ADS)
            MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotItemBuff, CashManager::getInstance()->isAdsfreeTime());
        else if( _eShopAds == E_SHOP_ADS::SHOP_MINE_ADS)
            MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotItemMine, CashManager::getInstance()->isAdsfreeTime());
        else
            MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotNone, CashManager::getInstance()->isAdsfreeTime());
        
        // reward 지급
        reward();
    }
    else
    {
        if ( result == ADS_RESULT::LOAD)
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(UILayoutShopAds::callbackShopAds, this));
            
            if( _eShopAds == E_SHOP_ADS::SHOP_GEM_ADS)
            {
                popup->setLogParams(kAnalEventAdsLoadParameterSlot);
                popup->setLogValue(kAdsSlotItemGem);
            }
            else if( _eShopAds == E_SHOP_ADS::SHOP_GOLD_ADS)
            {
                popup->setLogParams(kAnalEventAdsLoadParameterSlot);
                popup->setLogValue(kAdsSlotItemGold);
            }
            else if( _eShopAds == E_SHOP_ADS::SHOP_BUFF_ADS)
            {
                popup->setLogParams(kAnalEventAdsLoadParameterSlot);
                popup->setLogValue(kAdsSlotItemBuff);
            }
            else if( _eShopAds == E_SHOP_ADS::SHOP_MINE_ADS)
            {
                popup->setLogParams(kAnalEventAdsLoadParameterSlot);
                popup->setLogValue(kAdsSlotItemMine);
            }
            else
            {
                popup->setLogParams(kAnalEventAdsLoadParameterSlot);
                popup->setLogValue(kAdsSlotNone);
            }
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
void UILayoutShopAds::onClickShopAds(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    E_SHOP_ADS eType = _listShopAds.at(uiBtn->getTag());
    _eShopAds = eType;
    
    if ( UserInfoManager::getInstance()->getAdsTime(eType) > 0 )
    {
        return;
    }
    
    //
    if( CashManager::getInstance()->isAdsfreeTime() == true )
    {
        callbackShopAds(ADS_RESULT::SUCCESS);
    }
    else
    {
        E_ADS_WATCH eTypeWatch = E_ADS_WATCH::NONE;
        
        std::string strCount = "0";
        switch (eType) {
            case E_SHOP_ADS::SHOP_GEM_ADS:
            {
                eTypeWatch = E_ADS_WATCH::ITEM_GEM;
                
                int earnCount = C_BONUS_ADS_GEM;
                if ( CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_36) )
                {
                    earnCount += 10;
                }
                
                strCount = MafUtils::toString(earnCount);
                break;
            }
            case E_SHOP_ADS::SHOP_GOLD_ADS:
                eTypeWatch = E_ADS_WATCH::ITEM_GOLD;
                break;
                
            case E_SHOP_ADS::SHOP_BUFF_ADS:
                eTypeWatch = E_ADS_WATCH::BUFF_RAND;
                
                strCount = "1800";
                break;

            case E_SHOP_ADS::SHOP_MINE_ADS:
                eTypeWatch = E_ADS_WATCH::MINE_RAND_3;
                break;
                
            //
            case SHOP_D_GEM_ADS:
                eTypeWatch = E_ADS_WATCH::ITEM_GEM;
                
                strCount = MafUtils::toString(C_BONUS_ADS_GEM);
                break;
                
            case SHOP_D_GOLD_ADS:
                eTypeWatch = E_ADS_WATCH::ITEM_DEVIL_GOLD;
                break;
                
            case SHOP_D_BUFF_ADS:
                eTypeWatch = E_ADS_WATCH::BUFF_DEVIL_RAND;
                
                strCount = "1800";
                break;
                
            default:
                return;
        }
        
        //
        auto popup = PopupAdsWatch::create(eTypeWatch, strCount);
        popup->setCallbackResult([=](){
            MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(UILayoutShopAds::callbackShopAds, this));
        });
        popup->show();
        
        
    }
}

#pragma mark - game
void UILayoutShopAds::reward()
{
    //
    cocos2d::Vector<InfoItems*> listReward;
    if ( _eShopAds == E_SHOP_ADS::SHOP_GEM_ADS )
    {
        int earnCount = C_BONUS_ADS_GEM;
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_36) )
        {
            earnCount += 10;
        }
        
        //
        UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_GEM_ADS, TIME_ADS_GEM_BONUS);
        
        //
        ItemsMoneyManager::getInstance()->setGem(earnCount);
        
        //
        auto obj = ItemsManager::getInstance()->getInfoItems(E_ITEMS::GEM);
        obj->setCount(earnCount);
        listReward.pushBack(obj);
    }
    else if ( _eShopAds == E_SHOP_ADS::SHOP_GOLD_ADS )
    {
        auto earnCount = UserInfoManager::getInstance()->getEarnGoldFinal(_ePlace, E_EARNGOLD::ADS_SHOP);
        earnCount = MafUtils::bigMulNum(earnCount, "4"); // 광고시청 대기시간 증가로 보상 확장
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_11) )
        {
            earnCount = MafUtils::bigMulNum(earnCount, MafUtils::toString(11)); //1000%증가
        }

        //
        UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_GOLD_ADS, TIME_ADS_GOLD_BONUS);
        
        //
        ItemsMoneyManager::getInstance()->setGold(earnCount);
        
        //
        auto obj = ItemsManager::getInstance()->getInfoItems(E_ITEMS::GOLD);
        obj->setCount(earnCount);
        listReward.pushBack(obj);
    }
    else if ( _eShopAds == E_SHOP_ADS::SHOP_BUFF_ADS )
    {
        int buff = random(E_SHOP::SHOP_ATTACK_BONUS, E_SHOP::SHOP_COIN_BONUS);
        int buffTime = 1800;
        E_ITEMS eReward = E_ITEMS::BUFF_ATK;
        
        //
        UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_BUFF_ADS, TIME_ADS_BUFF_BONUS);
        
        //
        if ( buff == E_SHOP::SHOP_ATTACK_BONUS )
        {
            eReward = E_ITEMS::BUFF_ATK;
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_ATTACK_BONUS, buffTime);
            
            //
            PopupToastBuff::show(E_TOAST_BUFF::BUFF_ATK);
        }
        else if ( buff == E_SHOP::SHOP_COIN_BONUS )
        {
            eReward = E_ITEMS::BUFF_COIN;
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_COIN_BONUS, buffTime);
            
            //
            PopupToastBuff::show(E_TOAST_BUFF::BUFF_COIN);
        }
        else if(buff==E_SHOP::SHOP_MOVE_BONUS)
        {
            eReward = E_ITEMS::BUFF_SPD;
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_MOVE_BONUS, buffTime);
            
            //
            PopupToastBuff::show(E_TOAST_BUFF::BUFF_SPD);
        }
    }
    else if ( _eShopAds == E_SHOP_ADS::SHOP_MINE_ADS )
    {
        int count = SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_MINE_VIDEO_COUNT);
        SaveManager::setHistoryData(E_HISTORY_DATA::K_MINE_VIDEO_COUNT, count + 1);
        
        //
        UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_MINE_ADS, TIME_ADS_MINE_BONUS);
        
        //기본광물3개씩
        for ( int mineral = E_MINERAL::MINERAL_1; mineral <= E_MINERAL::MINERAL_6; mineral++ )
        {
            //
            E_ITEMS eItems = E_ITEMS::NONE;
            int nCount = 0;
            switch (mineral) {
                case E_MINERAL::MINERAL_1:
                    eItems = E_ITEMS::MINE_STONE_IRON;
                    nCount = 1200;
                    break;
                case E_MINERAL::MINERAL_2:
                    eItems = E_ITEMS::MINE_STONE_COPPER;
                    nCount = 1200;
                    break;
                case E_MINERAL::MINERAL_3:
                    eItems = E_ITEMS::MINE_STONE_SILVER;
                    nCount = 1200;
                    break;
                case E_MINERAL::MINERAL_4:
                    eItems = E_ITEMS::MINE_PIECE_IRON;
                    nCount = 250;
                    break;
                case E_MINERAL::MINERAL_5:
                    eItems = E_ITEMS::MINE_PIECE_COPPER;
                    nCount = 250;
                    break;
                case E_MINERAL::MINERAL_6:
                    eItems = E_ITEMS::MINE_PIECE_SILVER;
                    nCount = 250;
                    break;
                default:
                    break;
            }
            
            //
            MineralManager::getInstance()->setCount((E_MINERAL)mineral, MineralManager::getInstance()->getCount((E_MINERAL)mineral) + nCount);
            MineralManager::getInstance()->saveData();
            
            //
            auto obj = ItemsManager::getInstance()->getInfoItems(eItems);
            obj->setCount(nCount);
            obj->setTypeParams((int)mineral);
            
            listReward.pushBack(obj);
        }
    }
    else if ( _eShopAds == E_SHOP_ADS::SHOP_D_GEM_ADS )
    {
        int earnCount = C_BONUS_ADS_GEM;
        
        //
        UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_D_GEM_ADS, TIME_ADS_GEM_BONUS);
        
        //
        ItemsMoneyManager::getInstance()->setGem(earnCount);
        
        //
        auto obj = ItemsManager::getInstance()->getInfoItems(E_ITEMS::GEM);
        obj->setCount(earnCount);
        listReward.pushBack(obj);
    }
    else if ( _eShopAds == E_SHOP_ADS::SHOP_D_GOLD_ADS )
    {
        std::string earnCount = "0";
        for ( int i = 1; i <= C_COUNT_QUEST_DEVIL; i++ )
        {
            int level = UserInfoManager::getInstance()->getQuestLevel(_ePlace, (E_QUEST)i);
            std::string earnTemp = UserInfoManager::getInstance()->getQuestEarnGold(_ePlace, (E_QUEST)i, level);
            
            earnCount = MafUtils::bigAddNum(earnCount, earnTemp);
        }
        earnCount = MafUtils::bigMulNum(earnCount, "3");
        earnCount.append("0");//퀘스트완료 x30회 보상을 준다.
        earnCount = MafUtils::bigMulNum(earnCount, "4"); // 광고시청 대기시간 증가로 보상 확장
        
        //
        UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_D_GOLD_ADS, TIME_ADS_GOLD_BONUS);
        
        //
        ItemsMoneyManager::getInstance()->setDevilGold(earnCount);
        
        //
        auto obj = ItemsManager::getInstance()->getInfoItems(E_ITEMS::D_GOLD);
        obj->setCount(earnCount);
        listReward.pushBack(obj);
    }
    else if ( _eShopAds == E_SHOP_ADS::SHOP_D_BUFF_ADS )
    {
        int buff = random(E_SHOP::SHOP_D_MOVE_SPD, E_SHOP::SHOP_D_QUEST_GOLD);
        int buffTime = 1800;
        E_ITEMS eReward = E_ITEMS::BUFF_DEVIL_ATK;
        
        //
        UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_D_BUFF_ADS, TIME_ADS_BUFF_BONUS);
     
        if ( buff == E_SHOP::SHOP_D_ATK_DMG )
        {
            eReward = E_ITEMS::BUFF_DEVIL_ATK;
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_D_ATK_DMG,buffTime);
        }
        else if ( buff == E_SHOP::SHOP_D_QUEST_GOLD )
        {
            eReward = E_ITEMS::BUFF_DEVIL_COIN;
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_D_QUEST_GOLD,buffTime);
        }
        else if ( buff == E_SHOP::SHOP_D_MOVE_SPD )
        {
            eReward = E_ITEMS::BUFF_DEVIL_SPD;
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_D_MOVE_SPD,buffTime);
        }
        else if ( buff == E_SHOP::SHOP_D_ATK_SPD )
        {
            eReward = E_ITEMS::BUFF_DEVIL_ASPD;
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_D_ATK_SPD,buffTime);
        }
        
        //
        auto obj = ItemsManager::getInstance()->getInfoItems(eReward);
        obj->setCount(buffTime);
        listReward.pushBack(obj);
    }
    
    // 코스튬 효과 : 아이템 우선 지급
    if ( _eShopAds == E_SHOP_ADS::SHOP_GEM_ADS || _eShopAds == E_SHOP_ADS::SHOP_GOLD_ADS )
    {
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE,E_COSTUME_IDX::IDX_25) )
        {
            UserInfoManager::getInstance()->addHighArtifact(_ePlace, E_ARTIFACT_TYPE::ARTI_TYPE_ATK, 5);
        }
        
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_25) )
        {
            UserInfoManager::getInstance()->addHighArtifact(_ePlace, E_ARTIFACT_TYPE::ARTI_TYPE_QUEST, 5);
        }
    }
    
    // 코스튬 효과 :
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_11) )
    {
        int buffTime = 180;
        UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_ATTACK_BONUS, buffTime);
    }
    
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_18) )
    {
        int buffTime = 180;
        UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_COIN_BONUS, buffTime);
        UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_MOVE_BONUS, buffTime);
    }
     
    
    if ( listReward.size() > 0 )
    {
        PopupToastItems::show(listReward);
    }
    
    // mission
    ServerMissionManager::getInstance()->setServerMission(E_DAILY_QUEST::ADS_MYHOME, 1);
}
