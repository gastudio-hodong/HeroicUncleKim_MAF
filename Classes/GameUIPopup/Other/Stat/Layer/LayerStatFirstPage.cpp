//
//  LayerStatFirstPage.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 07/01/2020.
//

#include "LayerStatFirstPage.h"

#include "GameUIPopup/Other/Stat/PopupBadge.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/BadgeManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/AdventureRelicManager.h"
#include "ManagerGame/MineralManager.h"

USING_NS_CC;

StatFirstPage* StatFirstPage::create(E_PLACE ePlace)
{
    StatFirstPage* pRet = new(std::nothrow) StatFirstPage();
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

StatFirstPage::StatFirstPage(void):
_layerContainerStat(nullptr),
_layerContainerBadge(nullptr)
{
    
}

StatFirstPage::~StatFirstPage(void)
{
    
}
bool StatFirstPage::init(E_PLACE ePlace)
{
    if ( !LayerColor::init() )
    {
        return false;
    }
    
    //
    _ePlace = ePlace;
    
    // init
    initVar();
    initUI();
    
    return true;
}

void StatFirstPage::onEnter()
{
    LayerColor::onEnter();
    
    //
    RefreshManager::getInstance()->addUI(E_REFRESH::BADGE, this);
}

void StatFirstPage::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::BADGE )
    {
        initBadge();
    }
}

#pragma mark - init
void StatFirstPage::initUI()
{
    Size size = Size(750, Director::getInstance()->getVisibleSize().height - UserInfoManager::getInstance()->_fHeightIphoneX);
    
    _layerContainerStat = Layer::create();
    _layerContainerStat->setContentSize( size );
    _layerContainerStat->setIgnoreAnchorPointForPosition(false);
    _layerContainerStat->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    addChild(_layerContainerStat);
    
    _layerContainerBadge = Layer::create();
    _layerContainerBadge->setContentSize(Size(632, 151));
    _layerContainerBadge->setIgnoreAnchorPointForPosition(false);
    _layerContainerBadge->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _layerContainerBadge->setPosition(_layerContainerStat->getContentSize().width / 2, 400);
    _layerContainerStat->addChild(_layerContainerBadge);
      
    initHistory();
    initBadge();
    /* 배지 추가 하면서 출력 안함.
    if ( _currentPlace == DG_NORMAL )
    {
        initMine();
    }
     */
}
void StatFirstPage::initVar()
{
    _lbCurrentValue.clear();
}

void StatFirstPage::initHistory()
{
    float gap = 40;
    int fontSize = 22;
    int fontSizeValue = 26;
    
    float rightPos = 0;
    float downPos = 0;
    float padding = 0;
    
    
    std::vector<E_HISTORY_DATA> vecHistory[3];
    
    if(_ePlace == DG_NORMAL)
    {
        padding = 0.30;
        vecHistory[0].push_back(E_HISTORY_DATA::K_ATK_SEC);
        vecHistory[0].push_back(E_HISTORY_DATA::K_CRI_SEC);
        vecHistory[0].push_back(E_HISTORY_DATA::K_QUEST_SEC);
        vecHistory[0].push_back(E_HISTORY_DATA::K_REWARD_SEC);
        vecHistory[0].push_back(E_HISTORY_DATA::K_MOVE);
        
        vecHistory[1].push_back(E_HISTORY_DATA::K_START_DAY);
        vecHistory[1].push_back(E_HISTORY_DATA::K_LAST_DEVICE_TIME);
        vecHistory[1].push_back(E_HISTORY_DATA::K_PLAY_TIME);
        vecHistory[1].push_back(E_HISTORY_DATA::K_WEAPON_HIGH);
        vecHistory[1].push_back(E_HISTORY_DATA::K_PRISON_METER);
        vecHistory[1].push_back(E_HISTORY_DATA::K_PRISON_COUNT);
        vecHistory[1].push_back(E_HISTORY_DATA::K_VIDEO_COUNT);
        
        vecHistory[2].push_back(E_HISTORY_DATA::K_REVIVAL);
        vecHistory[2].push_back(E_HISTORY_DATA::K_REVIVAL_2);
        vecHistory[2].push_back(E_HISTORY_DATA::K_REVIVAL_3);
        vecHistory[2].push_back(E_HISTORY_DATA::K_REBORN_KEY);
        vecHistory[2].push_back(E_HISTORY_DATA::K_ONE_SHOT);
        vecHistory[2].push_back(E_HISTORY_DATA::K_HIGH_FLOOR);
        vecHistory[2].push_back(E_HISTORY_DATA::K_TOTAL_FLOOR);
        vecHistory[2].push_back(E_HISTORY_DATA::K_TOTAL_GEM);
        vecHistory[2].push_back(E_HISTORY_DATA::K_TOTAL_KEY);
        vecHistory[2].push_back(E_HISTORY_DATA::K_VIP_DAY);
        vecHistory[2].push_back(E_HISTORY_DATA::K_GIFT_COUNT);
        vecHistory[2].push_back(E_HISTORY_DATA::K_BUFF_COUNT);
    }
    else if(_ePlace == DG_DEVIL_DOM)
    {
        padding = 0.24;
        vecHistory[0].push_back(E_HISTORY_DATA::K_ATK_SEC);
        vecHistory[0].push_back(E_HISTORY_DATA::K_CRI_SEC);
        vecHistory[0].push_back(E_HISTORY_DATA::K_QUEST_SEC);
        vecHistory[0].push_back(E_HISTORY_DATA::K_MOVE);
        
        vecHistory[1].push_back(E_HISTORY_DATA::D_PET_LEVEL_1);
        vecHistory[1].push_back(E_HISTORY_DATA::D_PET_LEVEL_2);
        vecHistory[1].push_back(E_HISTORY_DATA::D_PET_LEVEL_3);
        vecHistory[1].push_back(E_HISTORY_DATA::D_PET_LEVEL_4);
        vecHistory[1].push_back(E_HISTORY_DATA::D_WEAPON_HIGH);
        vecHistory[1].push_back(E_HISTORY_DATA::D_HEAD_HIGH);
        vecHistory[1].push_back(E_HISTORY_DATA::D_BODY_HIGH);
        vecHistory[1].push_back(E_HISTORY_DATA::D_CAPE_HIGH);
        
        vecHistory[2].push_back(E_HISTORY_DATA::D_REVIVAL);
        vecHistory[2].push_back(E_HISTORY_DATA::D_HIGH_FLOOR);
        vecHistory[2].push_back(E_HISTORY_DATA::D_ARTIFACT_HIGH_ATK);
        vecHistory[2].push_back(E_HISTORY_DATA::D_ARTIFACT_HIGH_CRI);
        vecHistory[2].push_back(E_HISTORY_DATA::D_ARTIFACT_HIGH_GOLD);
        vecHistory[2].push_back(E_HISTORY_DATA::D_ARTIFACT_HIGH_ATKSPD);
        vecHistory[2].push_back(E_HISTORY_DATA::D_ARTIFACT_HIGH_MOVESPD);
    }


    
    //좌측
    for(int i = 0; i < vecHistory[0].size(); i++ )
    {
        Label *lbTitle = Label::createWithTTF(SaveManager::getInstance()->getHistoryKey(vecHistory[0].at(i)) , GAME_FONT, fontSize);
        lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbTitle->setPosition(_layerContainerStat->getContentSize().width*(0.03+rightPos), _layerContainerStat->getContentSize().height*(0.73f+downPos) -(gap*i));
        lbTitle->setDimensions(180, 30);
        lbTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbTitle->setOverflow(Label::Overflow::SHRINK);
        _layerContainerStat->addChild(lbTitle);
        
        Label *lbValue = MafNode::createBmFont("0", GAME_BM_FONT_AL, fontSizeValue);
        lbValue->setColor(COLOR_COIN);
        lbValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbValue->setPosition(_layerContainerStat->getContentSize().width*(0.30+rightPos), lbTitle->getPosition().y);
        lbValue->setDimensions(150, 30);
        lbValue->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbValue->setOverflow(Label::Overflow::SHRINK);
        _layerContainerStat->addChild(lbValue);
        
        _lbCurrentValue.pushBack(lbValue);
    }
    
    for(int i = 0;i < vecHistory[1].size(); i++ )
    {
        Label *lbTitle = Label::createWithTTF(SaveManager::getInstance()->getHistoryKey(vecHistory[1].at(i)) , GAME_FONT, fontSize);
        lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbTitle->setPosition(_layerContainerStat->getContentSize().width*(0.03+rightPos), _layerContainerStat->getContentSize().height*(0.73f+downPos) -(gap*(vecHistory[0].size()+i)));
        lbTitle->setDimensions(180, 30);
        lbTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbTitle->setOverflow(Label::Overflow::SHRINK);
        _layerContainerStat->addChild(lbTitle);
        
        Label *lbValue;
        if(_ePlace == DG_NORMAL)
        {
            if(vecHistory[1].at(i) == E_HISTORY_DATA::K_WEAPON_HIGH)
            {
                lbValue = Label::createWithTTF(SaveManager::getInstance()->getHistoryData(vecHistory[1].at(i)), GAME_FONT, fontSizeValue);
            }
            else if (vecHistory[1].at(i) == E_HISTORY_DATA::K_PLAY_TIME)
            {
                auto value = SaveManager::getInstance()->getHistoryData(vecHistory[1].at(i));
                auto playtime = atol(value.c_str());
                int tm_day, tm_hour, tm_min, tm_sec;
                
                tm_day   = (int)(playtime / ( 60 *60 * 24));
                
                playtime   = playtime - ( tm_day *60 *60 *24);
                tm_hour  = (int)(playtime / ( 60 *60));
                
                playtime   = playtime - ( tm_hour *60 *60);
                tm_min   = (int)(playtime / 60);
                
                playtime   = playtime - ( tm_min *60);
                tm_sec   = (int)playtime;
                
                value = GAME_TEXTFORMAT("t_ui_time_1", tm_day, tm_hour, tm_min);
                lbValue = Label::createWithTTF(value, GAME_FONT, fontSizeValue);
            }
            else if(vecHistory[1].at(i) == E_HISTORY_DATA::K_LAST_DEVICE_TIME)
            {
                std::string savedTime = SaveManager::getHistoryData(E_HISTORY_DATA::K_LAST_DEVICE_TIME);
                if(savedTime.compare("0")==0){
                    time_t timer_s = UtilsDate::getInstance()->getTime();
                    std::string nowTime = MafUtils::toString(timer_s);
                    SaveManager::setHistoryData(E_HISTORY_DATA::K_LAST_DEVICE_TIME, nowTime);
                    savedTime = nowTime;
                }
                auto time = std::atoi(savedTime.c_str());
                time_t t_time = (time_t)time;
                struct tm *tm;
                tm = localtime(&t_time); // time_t  형식으로 변환합니다.
                int year = tm->tm_year + 1900;
                int month = tm->tm_mon + 1;
                int yday = tm->tm_mday;
                lbValue = MafNode::createBmFont(MafUtils::format("%d-%.2d-%.2d",year,month,yday), GAME_BM_FONT_AL, fontSizeValue);
                
            }
            else
            {
                lbValue = MafNode::createBmFont(SaveManager::getHistoryData(vecHistory[1].at(i)), GAME_BM_FONT_AL, fontSizeValue);
            }
            lbValue->setDimensions(150, 30);
        }
        else if(_ePlace == DG_DEVIL_DOM)
        {
            if(vecHistory[1].at(i) == E_HISTORY_DATA::D_WEAPON_HIGH ||
               vecHistory[1].at(i) == E_HISTORY_DATA::D_HEAD_HIGH   ||
               vecHistory[1].at(i) == E_HISTORY_DATA::D_BODY_HIGH   ||
               vecHistory[1].at(i) == E_HISTORY_DATA::D_CAPE_HIGH){
                lbValue = Label::createWithTTF(SaveManager::getHistoryData(vecHistory[1].at(i)), GAME_FONT, fontSize);
            }


            else{
                lbValue = MafNode::createBmFont(SaveManager::getHistoryData(vecHistory[1].at(i)), GAME_BM_FONT_AL, fontSize);
            }
            lbValue->setDimensions(250, 30);
           
        }
            
        lbValue->setColor(COLOR_KEY);
        lbValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbValue->setPosition(_layerContainerStat->getContentSize().width*(0.30+rightPos), lbTitle->getPosition().y);
        lbValue->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbValue->setOverflow(Label::Overflow::SHRINK);
        
        _layerContainerStat->addChild(lbValue);
        
    }
    
    //우측
    rightPos = 0.5;
    
    for( int i = 0; i < vecHistory[2].size(); i++ )
    {
        int width = 0;
        if(_ePlace == DG_NORMAL)
        {
            width = 180;
        }
        else if(_ePlace == DG_DEVIL_DOM)
        {
            width = 150;
        }
        Label *lbTitle = Label::createWithTTF(SaveManager::getHistoryKey(vecHistory[2].at(i)) , GAME_FONT, fontSize);
        lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbTitle->setPosition(_layerContainerStat->getContentSize().width*(0.03+rightPos), _layerContainerStat->getContentSize().height*(0.73f+downPos) -(gap*i));
        lbTitle->setDimensions(width, 30);
        lbTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbTitle->setOverflow(Label::Overflow::SHRINK);
        _layerContainerStat->addChild(lbTitle);
        
        Label *lbValue;
        if(vecHistory[2].at(i) == E_HISTORY_DATA::K_REBORN_KEY || vecHistory[2].at(i) == E_HISTORY_DATA::K_TOTAL_KEY)
        {
            auto value = SaveManager::getHistoryData(vecHistory[2].at(i));
            value = MafUtils::convertNumberToShort(value);
            lbValue = MafNode::createBmFont(value, GAME_BM_FONT_AL, fontSizeValue);
        }
        else if(vecHistory[2].at(i) == E_HISTORY_DATA::D_ARTIFACT_HIGH_ATK    ||
                vecHistory[2].at(i) == E_HISTORY_DATA::D_ARTIFACT_HIGH_CRI    ||
                vecHistory[2].at(i) == E_HISTORY_DATA::D_ARTIFACT_HIGH_GOLD   ||
                vecHistory[2].at(i) == E_HISTORY_DATA::D_ARTIFACT_HIGH_ATKSPD ||
                vecHistory[2].at(i) == E_HISTORY_DATA::D_ARTIFACT_HIGH_MOVESPD)
        {
            lbValue = Label::createWithTTF(SaveManager:: getHistoryData(vecHistory[2].at(i)),GAME_FONT,fontSize);
        }
        else
        {
            lbValue = MafNode::createBmFont(SaveManager::getHistoryData(vecHistory[2].at(i)), GAME_BM_FONT_AL, fontSizeValue);
        }
        lbValue->setColor(COLOR_KEY);
        lbValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbValue->setPosition(_layerContainerStat->getContentSize().width*(padding+rightPos), lbTitle->getPosition().y);
        lbValue->setDimensions(150, 30);
        lbValue->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbValue->setOverflow(Label::Overflow::SHRINK);
        _layerContainerStat->addChild(lbValue);
    }
    
    
}

void StatFirstPage::initMine()
{
    auto gridView = extension::MafGridView::create(this, Size(_layerContainerStat->getContentSize().width, 150), 11);
    gridView->setDirection(extension::TableView::Direction::HORIZONTAL);
    gridView->setPosition(Point(_layerContainerStat->getContentSize().width*0.5-(65*11*0.5)+5, 10));
    gridView->setDelegate(this);
    gridView->setVerticalFillOrder(extension::MafGridView::VerticalFillOrder::BOTTOM_UP);
    gridView->setBounceable(false);
    _layerContainerStat->addChild(gridView);
}

void StatFirstPage::initBadge()
{
    _layerContainerBadge->removeAllChildren();
    
    //
    auto listBadge = BadgeManager::getInstance()->getListSelect();
    
    //
    auto spriteTitleBG = Sprite::create("Assets/ui_common/base_text_1_3.png");
    spriteTitleBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    spriteTitleBG->setPosition(_layerContainerBadge->getContentSize().width / 2, _layerContainerBadge->getContentSize().height);
    _layerContainerBadge->addChild(spriteTitleBG);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_badge_title"), GAME_FONT, 28);
        labelText->setPosition(spriteTitleBG->getContentSize().width / 2, spriteTitleBG->getContentSize().height / 2);
        labelText->setDimensions(spriteTitleBG->getContentSize().width * 0.95, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        spriteTitleBG->addChild(labelText);
    }
    
    auto menuBadge = MafNode::MafMenu::create();
    menuBadge->setPosition(_layerContainerBadge->getContentSize().width / 2, (_layerContainerBadge->getContentSize().height - 35) / 2);
    _layerContainerBadge->addChild(menuBadge);
    for ( int i = 0; i < 5; i++ )
    {
        auto labelBG = Layer::create();
        labelBG->setContentSize(Size(115, 115));
        
        auto item = MafNode::MafMenuItemSprite::create(labelBG, nullptr, CC_CALLBACK_1(StatFirstPage::onClickBadge, this));
        menuBadge->addChild(item);
        
        auto spriteEmpty = Sprite::create("Assets/ui/stat/badge_plus1_2.png");
        spriteEmpty->setPosition(item->getContentSize().width / 2, item->getContentSize().height / 2);
        spriteEmpty->setVisible(false);
        item->addChild(spriteEmpty);
        
        //
        if ( listBadge.size() > i )
        {
            auto objBadgeHave = listBadge.at(i);
            
            //
            item->setTag(objBadgeHave->getIdx());
            
            //
            auto objBadge = BadgeManager::getInstance()->getBadge(objBadgeHave->getIdxBadge());
            if ( objBadge == nullptr )
            {
                spriteEmpty->setVisible(true);
                continue;
            }
            
            auto spriteIcon = Sprite::create(objBadge->getPath());
            if(spriteIcon == nullptr)
            {
                spriteIcon = Sprite::create("Assets/ui/stat/badge_000.png");
            }
            spriteIcon->setPosition(item->getContentSize().width / 2, item->getContentSize().height / 2);
            item->addChild(spriteIcon);
        }
        else
        {
            spriteEmpty->setVisible(true);
        }
    }
    menuBadge->alignItemsHorizontallyWithPadding(0);
}

void StatFirstPage::redrawLayer()
{
    //현재 층수
    auto speed = UserInfoManager::getInstance()->getUserSpeed(_ePlace, false);
    speed*=4;
    if(_ePlace == DG_NORMAL)
    {
        _lbCurrentValue.at(4)->setString(StringUtils::format("%.3f", speed));
        auto cri = UserInfoManager::getInstance()->getCriticalRatePercent(_ePlace, true, false);
        _lbCurrentValue.at(1)->setString(StringUtils::format("%d", cri));
        
        //초당 골드 획득
        std::string sQuestEarnGold = UserInfoManager::getInstance()->getEarnGoldFinal(_ePlace, E_EARNGOLD::SECOND_DEFAULT);
        if (sQuestEarnGold.compare("10") == 0) {
            _lbCurrentValue.at(2)->setString("0/s");
        }else{
            MafUtils::convertNumberToShort(sQuestEarnGold);
            _lbCurrentValue.at(2)->setString(StringUtils::format("%s/s", sQuestEarnGold.c_str()));
        }

        std::string sMonsterEarnGold = UserInfoManager::getInstance()->getEarnGoldFinal(_ePlace, E_EARNGOLD::MON_KILL);
        MafUtils::convertNumberToShort(sMonsterEarnGold);
        _lbCurrentValue.at(3)->setString(sMonsterEarnGold);
        
        //봉식이 DPS
        int criPer = UserInfoManager::getInstance()->getCriticalRatePercent(_ePlace, true, false);
        int atkSpeed = 1 / UserInfoManager::getInstance()->getAttackSpeed(E_PLACE::DG_NORMAL) * 100;
        
        std::string hAtk1 = UserInfoManager::getInstance()->getAttackDamage(E_PLACE::DG_NORMAL,E_CHARACTER::CHA_HERO, false, false);
        std::string hAtk2 = UserInfoManager::getInstance()->getAttackDamage(E_PLACE::DG_NORMAL,E_CHARACTER::CHA_HERO, true, false);
        std::string hAtk3;
        
        hAtk1 = MafUtils::bigMulNum(hAtk1, MafUtils::toString(100-criPer));
        hAtk2 = MafUtils::bigMulNum(hAtk2, MafUtils::toString(criPer));
        if (hAtk1.length() > 2) {
            hAtk1.pop_back();
            hAtk1.pop_back();
        }
        if (hAtk2.length() > 2) {
            hAtk2.pop_back();
            hAtk2.pop_back();
        }
        hAtk3 = MafUtils::bigAddNum(hAtk1, hAtk2);
        
        std::string heroDps = MafUtils::bigMulNum(hAtk3, MafUtils::toString(atkSpeed));
        if (heroDps.length() > 2) {
            heroDps.pop_back();
            heroDps.pop_back();
        }
        
        //분신 DPS
        double per = DataManager::GetRelicEffect(E_RELIC::RELIC_2, UserInfoManager::getInstance()->getRelicLevel(_ePlace, E_RELIC::RELIC_2));
        if (per > 0) {
            if ( _ePlace == E_PLACE::DG_NORMAL )
            {
                if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_27)) {
                    if (UserInfoManager::getInstance()->getRelicLevel(_ePlace, E_RELIC::RELIC_2) >= DataManager::GetRelicMaxLevel(E_RELIC::RELIC_2)) {
                        per += per * 6.0f;
                    }
                }
            }
            if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_33)){
                per += per * 5.0f;
            }
            if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_23)) {
                per *= 10.0f;
            }
            
            auto tt = MafUtils::bigMulNum(heroDps, MafUtils::doubleToString(per));
            tt.pop_back();
            tt.pop_back();
            heroDps = MafUtils::bigAddNum(heroDps, tt);
        }
        
        
        //크르르 DPS
        std::string krrDps;
        if(0 != PetNewManager::getInstance()->getPetLevel(E_PET::CHICK_1))
        {
            criPer = UserInfoManager::getInstance()->getCriticalRatePercent(_ePlace, false, false);
            atkSpeed = 1 / PetNewManager::getInstance()->getPetCoolTime(E_PET::CHICK_1) * 100;
            
            std::string kAtk1 = UserInfoManager::getInstance()->getAttackDamage(E_PLACE::DG_NORMAL,E_CHARACTER::CHA_PET_CHICK, false, false);
            std::string kAtk2 = UserInfoManager::getInstance()->getAttackDamage(E_PLACE::DG_NORMAL,E_CHARACTER::CHA_PET_CHICK, true, false);
            std::string kAtkTotal = "0";
            std::string kAtkThunder = "0";
            
            kAtk1 = MafUtils::bigMulNum(kAtk1, MafUtils::toString(100-criPer));
            kAtk2 = MafUtils::bigMulNum(kAtk2, MafUtils::toString(criPer));
            if (kAtk1.length() > 2) {
                kAtk1.pop_back();
                kAtk1.pop_back();
            }
            if (kAtk2.length() > 2) {
                kAtk2.pop_back();
                kAtk2.pop_back();
            }
            
            int normalLevel = PetNewManager::getInstance()->getPetLevel(E_PET::CHICK_1);
            int evolutionLevel = PetNewManager::getInstance()->getPetEvolution(E_PET::CHICK_1);
            if (evolutionLevel > 0) {
                int mul = 8;
                if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_2)){
                    mul *= 10;
                }
                int evolutionLevel = PetNewManager::getInstance()->getPetEvolution(E_PET::CHICK_1);
                double evolutionPer = PetNewManager::getInstance()->getPetEvolutionEffect1(E_PET::CHICK_1, evolutionLevel);
                if (evolutionPer > 0) {
                    mul *= evolutionPer;
                }
                if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY,E_COSTUME_IDX::IDX_26)) {
                    mul += mul * (normalLevel * 0.2f);
                }
                
                //일반 낙뢰 데미지
                std::string tmp1 = MafUtils::bigMulNum(kAtk1, MafUtils::toString(mul));
                //크리 낙뢰 데미지
                std::string tmp2 = MafUtils::bigMulNum(kAtk2, MafUtils::toString(mul));
                
                kAtkThunder = MafUtils::bigAddNum(kAtkThunder, tmp1);
                kAtkThunder = MafUtils::bigAddNum(kAtkThunder, tmp2);
                
                //확률 계산
                int per = PetNewManager::getInstance()->getPetEvolutionEffect2(E_PET::CHICK_1, evolutionLevel);
                kAtkThunder = MafUtils::bigMulNum(kAtkThunder, MafUtils::toString(per));
                if (kAtkThunder.size() > 2) {
                    kAtkThunder.pop_back();
                    kAtkThunder.pop_back();
                }
                
                //모험 유물 적용
                double advtRelicBuff = 100.f;
                advtRelicBuff += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::CHICK_SKILL_ATK);
                
                kAtkThunder = MafUtils::bigMulNum(kAtkThunder, MafUtils::doubleToString(advtRelicBuff));
                kAtkThunder.pop_back();
                kAtkThunder.pop_back();
            }
            
            kAtkTotal = MafUtils::bigAddNum(kAtk1, kAtk2);
            kAtkTotal = MafUtils::bigAddNum(kAtkTotal, kAtkThunder);
            
            krrDps = MafUtils::bigMulNum(kAtkTotal, MafUtils::toString(atkSpeed));
            if (krrDps.length() > 2) {
                krrDps.pop_back();
                krrDps.pop_back();
            }
        }
        
        std::string allDps = MafUtils::bigAddNum(heroDps, krrDps);
        MafUtils::convertNumberToShort(allDps);
        
        _lbCurrentValue.at(0)->setString(StringUtils::format("%s/s", allDps.c_str()));
        
    }
    else if(_ePlace == DG_DEVIL_DOM)
    {
        _lbCurrentValue.at(3)->setString(StringUtils::format("%.3f", speed));
        auto cri = UserInfoManager::getInstance()->getCriticalRatePercent(_ePlace, true, false);
        _lbCurrentValue.at(1)->setString(StringUtils::format("%d", cri));

        //초당 골드 획득
        std::string sQuestEarnGold = UserInfoManager::getInstance()->getEarnGoldFinal(_ePlace, E_EARNGOLD::SECOND_DEFAULT);
        if (sQuestEarnGold.compare("10") == 0) {
            _lbCurrentValue.at(2)->setString("0/s");
        }else{
            MafUtils::convertNumberToShort(sQuestEarnGold);
            _lbCurrentValue.at(2)->setString(StringUtils::format("%s/s", sQuestEarnGold.c_str()));
        }

        //봉식이 DPS
        int criPer = UserInfoManager::getInstance()->getCriticalRatePercent(_ePlace, true, false);
        int atkSpeed = 1 / UserInfoManager::getInstance()->getAttackSpeed(E_PLACE::DG_DEVIL_DOM) * 100;

        std::string hAtk1 = UserInfoManager::getInstance()->getAttackDamage(E_PLACE::DG_DEVIL_DOM,E_CHARACTER::CHA_HERO, false, false);
        std::string hAtk2 = UserInfoManager::getInstance()->getAttackDamage(E_PLACE::DG_DEVIL_DOM,E_CHARACTER::CHA_HERO, true, false);
        std::string hAtk3;

        hAtk1 = MafUtils::bigMulNum(hAtk1, MafUtils::toString(100-criPer));
        hAtk2 = MafUtils::bigMulNum(hAtk2, MafUtils::toString(criPer));
        if (hAtk1.length() > 2) {
            hAtk1.pop_back();
            hAtk1.pop_back();
        }
        if (hAtk2.length() > 2) {
            hAtk2.pop_back();
            hAtk2.pop_back();
        }
        hAtk3 = MafUtils::bigAddNum(hAtk1, hAtk2);

        std::string heroDps = MafUtils::bigMulNum(hAtk3, MafUtils::toString(atkSpeed));
        if (heroDps.length() > 2) {
            heroDps.pop_back();
            heroDps.pop_back();
        }






        std::string allDps = heroDps;
        MafUtils::convertNumberToShort(allDps);

        _lbCurrentValue.at(0)->setString(StringUtils::format("%s/s", allDps.c_str()));
    }
}

extension::TableViewCell* StatFirstPage::tableCellAtIndex(extension::MafGridView *table, ssize_t idx)
{
    extension::TableViewCell *cell = table->dequeueCell();
    E_MINERAL mineral = (E_MINERAL)(idx+1);
    
    Sprite* bg;
    Label *lbCount;
    Label* lbName;
    Sprite *spIcon;
    
    if (!cell) {
        cell = new extension::TableViewCell();
        cell->autorelease();
        cell->setContentSize(Size(65, 90));
        cell->setScale(0.77);
        
        bg = Sprite::create("Assets/ui/mine/week_ui_minibox_1.png");
        bg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        bg->setPosition(Vec2(0, 0));
        cell->addChild(bg);
        
        spIcon = Sprite::create(DataManager::GetMineMineralPath(mineral));
        spIcon->setPosition(Vec2(bg->getContentSize().width/2,bg->getContentSize().height*0.6));
        bg->addChild(spIcon);
        
        lbCount = Label::createWithTTF("", GAME_FONT, 18);
        lbCount->setPosition(bg->getContentSize().width-6, bg->getContentSize().height*0.3);
        lbCount->setAnchorPoint(Vec2(1,0));
        lbCount->setAlignment(TextHAlignment::RIGHT);
        lbCount->enableOutline(Color4B::BLACK, 1);
        bg->addChild(lbCount);
        
        lbName = Label::createWithTTF(DataManager::GetMineMineralName(mineral), GAME_FONT, 18);
        lbName->setPosition(bg->getContentSize().width/2, bg->getContentSize().height*0.13);
        lbName->setDimensions(bg->getContentSize().width * 0.95, 30);
        lbName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbName->setOverflow(Label::Overflow::SHRINK);
        bg->addChild(lbName);
        
        
        bg->setTag(100);
        lbCount->setTag(1);
        lbName->setTag(3);
        spIcon->setTag(4);
        
        lbName->setString(DataManager::GetMineMineralName(mineral));
        lbCount->setString(MafUtils::format("%d", MineralManager::getInstance()->getCount(mineral)));
        spIcon->setTexture(DataManager::GetMineMineralPath(mineral));
    }
    
    
    
    
    return cell;
}
Size StatFirstPage::cellSizeForTable(extension::MafGridView *table)
{
    return Size(65,90);
}
ssize_t StatFirstPage::numberOfCellsInTableView(extension::MafGridView *table)
{
    return C_COUNT_MINE_MINERAL;
}

#pragma mark - click
void StatFirstPage::onClickBadge(cocos2d::Ref* sender)
{
    if ( AccountManager::getInstance()->getUserIdx() == 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto popup = PopupBadge::create();
    popup->show();
}
