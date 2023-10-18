//
//  UILayerRaidMain.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/03.
//

#include "UILayerRaidMain.h"

#include "GameUnit/UnitBehaviorHeroAnother.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidBuyAttackCount.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidTotalLevelEffect.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidMission.h"
#include "GameUIPopup/Other/Ranking/PopupTierRanking.h"
#include "GameUIPopup/Other/WeaponSpirit/PopupListWeaponSpirit.h"
#include "GameUIPopup/Other/Chat/PopupChat.h"

#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/CostumeSkinManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/ChatManager.h"
#include "ManagerGame/WeaponManager.h"

USING_NS_CC;

UILayerRaidMain* UILayerRaidMain::create(cocos2d::Size size)
{
    UILayerRaidMain* pRet = new(std::nothrow) UILayerRaidMain();
    if ( pRet && pRet->init(size) )
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

UILayerRaidMain::UILayerRaidMain(void):
_lyUI(nullptr)
,_lyBattle(nullptr)
,_sprDragonHead(nullptr)
,_sprAttackFx(nullptr)

,_lbSeasonTimer(nullptr)

,_progressHP(nullptr)
,_hero(nullptr)

,_callbackHide(nullptr)
{
    _listReward.clear();
}

UILayerRaidMain::~UILayerRaidMain(void)
{
    _listReward.clear();
}

bool UILayerRaidMain::init(cocos2d::Size size)
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    setContentSize(size);
    
    //
    
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void UILayerRaidMain::initVar()
{
}

void UILayerRaidMain::initUi()
{
    // Battle
    _lyBattle = LayerColor::create(Color4B::BLACK);
    _lyBattle->setContentSize(Size(getContentSize()));
    _lyBattle->setIgnoreAnchorPointForPosition(false);
    _lyBattle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyBattle->setPosition(getContentSize().width/2, getContentSize().height);
    addChild(_lyBattle);
    
    // UI
    _lyUI = Layer::create();
    _lyUI->setContentSize(Size(getContentSize()));
    _lyUI->setIgnoreAnchorPointForPosition(false);
    _lyUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyUI->setPosition(getContentSize().width/2, getContentSize().height);
    addChild(_lyUI);
    
    
   
    
    //
    uiBattle();
    uiIcon();
}

void UILayerRaidMain::refreshUI()
{
    uiBattle();
    uiIcon();
}

#pragma mark - ui
void UILayerRaidMain::uiBattle()
{
    _lyBattle->removeAllChildren();
    
    std::string strBgPath = "Assets/ui/raid/raid_bg_1_1.png";
    std::string strHeadPath = "Assets/ui/raid/raid_bg_1_2.png";
    
    auto sprBG = Sprite::create(strBgPath);
    sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBG->setPosition(_lyBattle->getContentSize()/2);
    sprBG->setScale(MAX(_lyBattle->getContentSize().width / sprBG->getContentSize().width, _lyBattle->getContentSize().height / sprBG->getContentSize().height));
    sprBG->getTexture()->setTexParameters(texParams);
    _lyBattle->addChild(sprBG);
    
    _sprDragonHead = Sprite::create(strHeadPath);
    _sprDragonHead->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _sprDragonHead->setPosition(_lyBattle->getContentSize()/2);
    _lyBattle->addChild(_sprDragonHead);
    {
        auto action1 = MoveBy::create(1.0, Vec2(0,10));
        auto seq = Sequence::create(action1, action1->reverse(), nullptr);
        _sprDragonHead->runAction(RepeatForever::create(seq));
    }
    
    auto particle = ParticleSnow::createWithTotalParticles(random(80, 100));
    particle->setGravity(Vec2(0, 10));
    particle->setAngle(70);
    particle->setSpeed(130);
    particle->setSpeedVar(30);
    particle->setStartSize(random(8, 12));
    particle->setStartSizeVar(5);
    particle->setEndSize(-1);
    particle->setEndSizeVar(0);
    particle->setPosition( Vec2( _lyBattle->getContentSize().width*0.5, 0) );
    particle->setLife(4.5f);
    particle->setLifeVar(1);
    particle->setEmissionRate(particle->getTotalParticles()/particle->getLife());
    particle->setTexture( Director::getInstance()->getTextureCache()->addImage("Assets/ui/team_raid/flame_s_1.png") );

    _lyBattle->addChild(particle, E_ZORER::UI-1);
    
    //hp
    auto spriteHPBg = Sprite::create("Assets/ui/raid/boss_hp_1.png");
    spriteHPBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    spriteHPBg->setPosition( _lyBattle->getContentSize().width/2, _lyBattle->getContentSize().height - 40);
    _lyBattle->addChild(spriteHPBg, E_ZORER::UI);
        
    std::string strProgressPath = "Assets/ui/raid/boss_hp_2.png";
        
    _progressHP = ProgressTimer::create(Sprite::create(strProgressPath));
    _progressHP->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _progressHP->setPosition( spriteHPBg->getPosition() );
    _progressHP->setMidpoint(Vec2(0, 0.5));
    _progressHP->setBarChangeRate(Vec2(1,0));
    _progressHP->setType(ProgressTimer::Type::BAR);
    _progressHP->setPercentage(50.f);
    _lyBattle->addChild(_progressHP, E_ZORER::UI);
    {
        auto infoNowDragon = NewRaidManager::getInstance()->getNowDragon();
        
        double nNowHP = (infoNowDragon->getHP() - NewRaidManager::getInstance()->getActualDamage());
        double nPercent = (double)nNowHP / infoNowDragon->getHP() * 100;
        if ( nPercent > 100 )
            nPercent = 100;
        _progressHP->setPercentage(nPercent);
        
        std::string strLv = MafUtils::format("LV.%d", (int)infoNowDragon->getLevel());
        auto labelLv = Label::createWithTTF(strLv, GAME_FONT, 20);
        labelLv->setPosition( Vec2(45, _progressHP->getContentSize().height / 2) );
        labelLv->enableBold();
        _progressHP->addChild(labelLv);
        
        std::string sHP = std::to_string(nNowHP);
        std::string sMaxHP = std::to_string(infoNowDragon->getHP());
        sHP = sHP.substr(0, sHP.rfind('.'));
        sMaxHP = sMaxHP.substr(0, sMaxHP.rfind('.'));
        auto labelHP = Label::createWithTTF(MafUtils::format("%s / %s", MafUtils::addCommaToString(sHP, 3, ',').c_str(), MafUtils::addCommaToString(sMaxHP, 3, ',').c_str()), GAME_FONT, 20);
        labelHP->setPosition( Vec2(_progressHP->getContentSize().width / 2, _progressHP->getContentSize().height / 2) );
        labelHP->setName("TEXT");
        _progressHP->addChild(labelHP);
    }
    
    //co_op
    auto btnCo_OpBg = ui::Button::create("Assets/ui/raid/honor_gauge_1_1.png");
    btnCo_OpBg->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    btnCo_OpBg->setPosition(Vec2(20, spriteHPBg->getPosition().y - spriteHPBg->getContentSize().height - 10));
    btnCo_OpBg->setZoomScale(0.05f);
    btnCo_OpBg->addClickEventListener([=](Ref*){
        
        Vec2 posToast = _lyUI->getParent()->convertToWorldSpace(_lyUI->getPosition());
        posToast.y -= _lyUI->getContentSize().height / 1.5;

        PopupToast::showImmediately(GAME_TEXT("t_ui_new_raid_popup_coop"), posToast.x, posToast.y);
    });
    _lyBattle->addChild(btnCo_OpBg, E_ZORER::UI);
        
    strProgressPath = "Assets/ui/raid/honor_gauge_1_2.png";
        
    _progressCO_OP = ProgressTimer::create(Sprite::create(strProgressPath));
    _progressCO_OP->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _progressCO_OP->setPosition( btnCo_OpBg->getPosition() );
    _progressCO_OP->setMidpoint(Vec2(0, 0.5));
    _progressCO_OP->setBarChangeRate(Vec2(1,0));
    _progressCO_OP->setType(ProgressTimer::Type::BAR);
    _progressCO_OP->setPercentage(0);
    _lyBattle->addChild(_progressCO_OP, E_ZORER::UI);
    {
        auto objCo_OpSetting = NewRaidManager::getInstance()->getRaidSetting("max_cooperation_point");
        
        double nPercent = (double)NewRaidManager::getInstance()->getTotalPoint() / objCo_OpSetting->getValueToInt() * 100;
        if ( nPercent > 100 )
            nPercent = 100;
        _progressCO_OP->setPercentage(nPercent);
        
        std::string strCo_Op = MafUtils::format("%d/%d", NewRaidManager::getInstance()->getTotalPoint(), objCo_OpSetting->getValueToInt());
        auto labelCo_Op = Label::createWithTTF(strCo_Op, GAME_FONT, 16);
        labelCo_Op->setPosition( Vec2(_progressCO_OP->getContentSize().width / 2, _progressCO_OP->getContentSize().height / 2) );
        labelCo_Op->setName("TEXT");
        _progressCO_OP->addChild(labelCo_Op);
        
        auto sprHonor = Sprite::create("Assets/icon/menu_honor.png");
        sprHonor->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprHonor->setPosition(btnCo_OpBg->getPosition() + Vec2::DOWN * btnCo_OpBg->getContentSize().height/2 + Vec2::RIGHT * (btnCo_OpBg->getContentSize().width - 10));
        _lyBattle->addChild(sprHonor, E_ZORER::UI);
        
    }
    
    int skinIdx = CostumeSkinManager::getInstance()->getCostumeSkinEquip();
    _hero = UnitBehaviorHeroAnother::create(E_PLACE::DG_NORMAL);
    _hero->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _hero->setPosition(200, 120);
    if(skinIdx == 0 || CostumeSkinManager::getInstance()->getInfoSkin(skinIdx) == nullptr)
    {
        _hero->setImage(WeaponManager::getInstance()->getEquipVisual(),
                        CostumeManager::getInstance()->getEquip(E_COSTUME::HEAD),
                        CostumeManager::getInstance()->getEquip(E_COSTUME::BODY),
                        CostumeManager::getInstance()->getEquip(E_COSTUME::CAPE),
                        CostumeManager::getInstance()->getEquip(E_COSTUME::HAND),
                        CostumeManager::getInstance()->getEquip(E_COSTUME::FOOT));
        
        _hero->setMotion(WeaponManager::getInstance()->getEquipVisual(),
                         CostumeManager::getInstance()->getEquip(E_COSTUME::HEAD),
                         CostumeManager::getInstance()->getEquip(E_COSTUME::BODY),
                         CostumeManager::getInstance()->getEquip(E_COSTUME::CAPE),
                         CostumeManager::getInstance()->getEquip(E_COSTUME::HAND),
                         CostumeManager::getInstance()->getEquip(E_COSTUME::FOOT));
    }
    else
    {
        _hero->setSkin(skinIdx);
    }
    
    _hero->actionMove();
    _lyBattle->addChild(_hero);
    
    
    _sprAttackFx = Sprite::create("Assets/ui/raid/raid_bg_1_4.png");
    _sprAttackFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _sprAttackFx->setScale(sprBG->getScale());
    _sprAttackFx->getTexture()->setTexParameters(texParams);
    _sprAttackFx->setPosition(_lyBattle->getContentSize()/2);
    _sprAttackFx->setVisible(false);
    _lyBattle->addChild(_sprAttackFx);
}

void UILayerRaidMain::uiIcon()
{
    _lyUI->removeAllChildren();
    
    _lbSeasonTimer = Label::createWithTTF("", GAME_FONT, 24);
    _lbSeasonTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lbSeasonTimer->setPosition(_lyUI->getContentSize().width/2, _lyUI->getContentSize().height - 5);
    _lbSeasonTimer->enableOutline(Color4B::BLACK, 1);
    _lbSeasonTimer->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
    _lbSeasonTimer->setDimensions(_lyUI->getContentSize().width - 100, _lbSeasonTimer->getContentSize().height);
    _lyUI->addChild(_lbSeasonTimer);
    {
        auto callFunc = CallFunc::create([=](){
            long time = NewRaidManager::getInstance()->getEndSeasonTime() - UtilsDate::getInstance()->getTime();//서버에서 주는 시간으로 변경 필요
            int day = (int)(time / (24 * 3600));
            if(day >0)
                time = time % (24 * 3600);
            int hour = (int)time / 3600;
            int min = (time % 3600) / 60;
            
            if(day < 0)
                day = 0;
            if(hour < 0)
                hour = 0;
            if(min < 0)
                min = 0;
            
            _lbSeasonTimer->setString(GAME_TEXTFORMAT("t_ui_raid_end_time", day, hour,min));
        });
        auto delayTime = DelayTime::create(60.f);
        auto seq = Sequence::create(callFunc, delayTime, nullptr);
        _lbSeasonTimer->runAction(RepeatForever::create(seq));
    }
    
    
    std::string strIconPath = "Assets/ui/bg_icon/icon_mission.png";
    std::string strBoxPath = "Assets/ui_common/base_text_2_4.png";
    std::string strText = GAME_TEXT("t_ui_raid_tickt_noti_off");
    Color3B textColor = Color3B(204,204,204);
    
    if(NewRaidManager::getInstance()->isTicket())
    {
        strIconPath = "Assets/ui/bg_icon/icon_mission2_1.png";
        strBoxPath = "Assets/ui_common/base_text_2_5.png";
        strText = GAME_TEXT("t_ui_raid_tickt_noti_on");
        textColor = Color3B(255,228,92);
    }
    
    auto btnMission = ui::Button::create(strIconPath);
    btnMission->addClickEventListener(CC_CALLBACK_1(UILayerRaidMain::onClickMission, this));
    btnMission->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    btnMission->setZoomScale(0.05f);
    btnMission->setPosition(Vec2(20, _lyUI->getContentSize().height - 190));
    _lyUI->addChild(btnMission);
    {
        auto btnBox = ui::Button::create(strBoxPath);
        btnBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        btnBox->setPosition(btnMission->getPosition() + Vec2::RIGHT * btnMission->getContentSize().width + Vec2::DOWN * btnMission->getContentSize().height/2);
        btnBox->setZoomScale(0.05f);
        btnBox->addClickEventListener(CC_CALLBACK_1(UILayerRaidMain::onClickMission, this));
        
        _lyUI->addChild(btnBox);
        
        auto lbText = Label::createWithTTF(strText, GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbText->setPosition(10, btnBox->getContentSize().height/2);
        lbText->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        lbText->setDimensions(btnBox->getContentSize().width, btnBox->getContentSize().height - 5 );
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->setColor(textColor);
        btnBox->addChild(lbText);
        
        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
        uiReddot->setPosition(0, btnMission->getContentSize().height - 15);
        btnMission->addChild(uiReddot);
        uiReddot->setVisible(false);
        
        if(NewRaidManager::getInstance()->isMissionComplete())
        {
            auto action1 = CallFuncN::create([=](Ref* sender){
                
                auto uiReddot = (UIReddot*)sender;
                
                if(NewRaidManager::getInstance()->isMissionComplete())
                {
                    uiReddot->setVisible(true);
                }
                else
                {
                    uiReddot->setVisible(false);
                }
            });
            auto action2 = DelayTime::create(2);
            auto seq = Sequence::create(action1, action2, NULL);
            uiReddot->runAction(RepeatForever::create(seq));
        }
    }
    
    auto btnHelp = ui::Button::create("Assets/ui_common/icon_help.png");
    btnHelp->addClickEventListener(CC_CALLBACK_1(UILayerRaidMain::onClickHelp, this));
    btnHelp->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    btnHelp->setZoomScale(0.05f);
    btnHelp->setPosition(btnMission->getPosition() + Vec2::DOWN * (btnMission->getContentSize().height + 25));
    _lyUI->addChild(btnHelp);
    
    auto btnChat = ui::Button::create("Assets/ui/chatting/btn_chatting.png");
    btnChat->addClickEventListener(CC_CALLBACK_1(UILayerRaidMain::onClickChat, this));
    btnChat->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    btnChat->setZoomScale(0.05f);
    btnChat->setPosition(btnMission->getPosition() + Vec2::DOWN * (btnMission->getContentSize().height + 120));
    btnChat->setScale(0.6);
    _lyUI->addChild(btnChat);
    
    auto btnTimer = ui::Button::create("Assets/ui/prison/icon_prison_time.png");
    btnTimer->addClickEventListener([=](Ref*){
        
        Vec2 posToast = _lyUI->getParent()->convertToWorldSpace(_lyUI->getPosition());
        posToast.y -= _lyUI->getContentSize().height / 1.5;

        PopupToast::showImmediately(GAME_TEXT("t_ui_new_raid_toast_daytime"), posToast.x, posToast.y);
    });
    btnTimer->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    btnTimer->setZoomScale(0.05f);
    btnTimer->setPosition(Vec2(_lyUI->getContentSize().width - 20, _lyUI->getContentSize().height - 140));
    _lyUI->addChild(btnTimer);
    {
        auto lbTime = Label::createWithTTF("", GAME_FONT, 18);
        lbTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbTime->setPosition(btnTimer->getContentSize().width / 2, 10);
        lbTime->enableOutline(Color4B::BLACK, 1);
        btnTimer->addChild(lbTime);
        
        auto action1 = DelayTime::create(1.0);
        auto action2 = CallFuncN::create([&, lbTime](Ref* sender){
            long time = NewRaidManager::getInstance()->getNextAttackTime() - UtilsDate::getInstance()->getTime();//서버에서 주는 시간으로 변경 필요
            int hour = (int)time / 3600;
            int min = (time % 3600) / 60;
            int second = time % 3600 % 60;
            
            if(hour < 0)
                hour = 0;
            if(min < 0)
                min = 0;
            if(second < 0)
                second = 0;
            
            lbTime->setString(MafUtils::format("%02d:%02d:%02d", hour, min, second));
        });
        auto seq = Sequence::create(action2, action1, NULL);
        lbTime->runAction(RepeatForever::create(seq));
    }
    
    auto btnRank = ui::Button::create("Assets/ui/ranking/icon_ranking_2.png");
    btnRank->addClickEventListener(CC_CALLBACK_1(UILayerRaidMain::onClickRank, this));
    btnRank->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    btnRank->setZoomScale(0.05f);
    btnRank->setPosition(btnTimer->getPosition() + Vec2::DOWN * (btnTimer->getContentSize().height + 35) + Vec2::LEFT * btnTimer->getContentSize().width/2);
    _lyUI->addChild(btnRank);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_rank_icon"), GAME_FONT, 18);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbText->setPosition(btnRank->getContentSize().width / 2, 20);
        lbText->enableOutline(Color4B::BLACK, 1);
        btnRank->addChild(lbText);
    }
    
    auto btnWeekReward = ui::Button::create("Assets/ui/raid/icon_cumulative_lv.png");
    btnWeekReward->addClickEventListener(CC_CALLBACK_1(UILayerRaidMain::onClickWeekReward, this));
    btnWeekReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    btnWeekReward->setZoomScale(0.05f);
    btnWeekReward->setPosition(btnRank->getPosition() + Vec2::DOWN * (btnRank->getContentSize().height + 35));
    _lyUI->addChild(btnWeekReward);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_stat_reward_icon"), GAME_FONT, 18);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbText->setPosition(btnWeekReward->getContentSize().width / 2, 10);
        lbText->enableOutline(Color4B::BLACK, 1);
        btnWeekReward->addChild(lbText);
    }
    
    
    auto btnAttack = ui::Button::create("Assets/ui/raid/btn_attack_1.png");
    btnAttack->addClickEventListener(CC_CALLBACK_1(UILayerRaidMain::onClickAttack, this));
    btnAttack->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    btnAttack->setZoomScale(0.05f);
    btnAttack->setPosition(Vec2(_lyUI->getContentSize().width/2, 5));
    _lyUI->addChild(btnAttack);
    {
        auto objSetting = NewRaidManager::getInstance()->getRaidSetting("attack_count");
        int currentCnt = NewRaidManager::getInstance()->getAttackCount();
        int maxCnt = objSetting->getValueToInt();
        
        auto lbText = Label::createWithTTF(MafUtils::format("%d/%d", currentCnt, maxCnt), GAME_FONT, 22);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        lbText->setPosition(btnAttack->getContentSize().width/2, 5);
        btnAttack->addChild(lbText);
    }
    
    std::string strTicketPath = "Assets/ui/raid/btn_dra_shop1_2.png";
    if(DataManager::isEventFoolsDay())
        strTicketPath = "Assets/ui/raid/btn_dra_shop1_1.png";
    auto btnTicketShop = ui::Button::create(strTicketPath);
    btnTicketShop->addClickEventListener(CC_CALLBACK_1(UILayerRaidMain::onClickTicketShop, this));
    btnTicketShop->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    btnTicketShop->setZoomScale(0.05f);
    btnTicketShop->setPosition(Vec2(_lyUI->getContentSize().width - 5, 5));
    _lyUI->addChild(btnTicketShop);
}

#pragma mark - onClick
void UILayerRaidMain::onClickMission(Ref* pSender)
{
    if(NewRaidManager::getInstance()->isAttack())
        return;
    
    auto popup = PopupRaidMission::create();
    popup->setCallbackHide(CC_CALLBACK_0(UILayerRaidMain::callbackHide, this));
    popup->show();
}

void UILayerRaidMain::onClickHelp(Ref* pSender)
{
    if(NewRaidManager::getInstance()->isAttack())
        return;
    
    auto popup = PopupHelp::create(9, "t_ui_raid_main_info_title_%d", "t_ui_raid_main_info_msg_%d");
    popup->show();
}

void UILayerRaidMain::onClickChat(Ref* pSender)
{
    if ( ChatManager::getInstance()->isBan() == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_7"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto popup = PopupChat::create(E_CHAT_TYPE::RAID);
    popup->show();
}

void UILayerRaidMain::onClickRank(Ref* pSender)
{
    if(NewRaidManager::getInstance()->isAttack())
        return;
    
    auto popup = PopupTierRanking::create(E_RANK_LOCAL_TYPE::NEW_RAID);
    popup->show();
}

void UILayerRaidMain::onClickWeekReward(Ref* pSender)
{
    if(NewRaidManager::getInstance()->isAttack())
        return;
    auto popup = PopupRaidTotalLevelEffect::create();
    
    popup->show();
}

void UILayerRaidMain::onClickAttack(Ref* pSender)
{
    if(NewRaidManager::getInstance()->isAttack())
        return;
    
    if(NewRaidManager::getInstance()->getAttackCount() <= 0)
    {
        auto popup = PopupRaidBuyAttackCount::create();
        popup->setCallbackResult(CC_CALLBACK_0(UILayerRaidMain::refreshUI, this));
        popup->show();
        
        return;
    }
    
    PopupLoading::show();
    NewRaidManager::getInstance()->requestAttack(CC_CALLBACK_4(UILayerRaidMain::callbackAttack, this), 1);
}

void UILayerRaidMain::onClickTicketShop(Ref* pSender)
{
    if(NewRaidManager::getInstance()->isAttack() || NewRaidManager::getInstance()->isTicket())
    {
        std::string strMsg = "t_ui_new_raid_ticket_pop_2";
        
        if(NewRaidManager::getInstance()->isAttack())
            strMsg = "t_ui_new_raid_ticket_pop_3";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMsg));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    auto objProduct = CashManager::getInstance()->getProduct(70000000);
    if ( objProduct == nullptr )
    {
        return;
    }
    /*
    0 : 오류 없음
    1 : 다른상품 결제 오류 있음.
    2 : 같은상품 결제 오류 있음.
    */
    CashManager::getInstance()->purchaseHas(70000000, [=](int nHas){
        if ( nHas == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(70000000, CC_CALLBACK_4(UILayerRaidMain::callbackPurchase, this));
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
            long time = NewRaidManager::getInstance()->getEndSeasonTime() - UtilsDate::getInstance()->getTime();//서버에서 주는 시간으로 변경 필요
            int day = (int)(time / (24 * 3600));
            if(day >0)
                time = time % (24 * 3600);
            int hour = (int)time / 3600;
            int min = (time % 3600) / 60;
            
            if(day < 0)
                day = 0;
            if(hour < 0)
                hour = 0;
            if(min < 0)
                min = 0;
            
            
            std::string strMsg = GAME_TEXT("t_ui_new_raid_ticket_pop_1") + "\n" + GAME_TEXTFORMAT("t_ui_raid_end_time", day, hour,min);
            
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMsg);
            popup->addButton(GAME_TEXT("t_ui_costume_skin_msg_2"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(70000000, CC_CALLBACK_4(UILayerRaidMain::callbackPurchase, this));
            });
            popup->addButton(GAME_TEXT("t_ui_close"), true, nullptr);
            popup->show();
        }
    });
}
void UILayerRaidMain::setCallbackHide(const std::function<void(void)>& callback)
{
    _callbackHide = callback;
}

#pragma mark - action
void UILayerRaidMain::actionAttackStart(bool isCritical, bool isDoubleAttack)
{
    NewRaidManager::getInstance()->setIsAttack(true);
    
    std::string path = "Assets/ui/raid/raid_ability_1_1.png";
    
    if(isCritical)
    {
        path = "Assets/ui/raid/raid_ability_1_3.png";
    }
    if(isDoubleAttack)
    {
        path = "Assets/ui/raid/raid_ability_1_2.png";
    }
    if(isDoubleAttack && isCritical)
    {
        path = "Assets/ui/raid/raid_ability_1_4.png";
    }
    
    auto spriteEffect = Sprite::create(path);
    spriteEffect->setPosition(0, 0);
    spriteEffect->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _lyBattle->addChild(spriteEffect);
    {
        //
        auto action1_1 = MoveTo::create(0.5, Vec2(spriteEffect->getContentSize().width, spriteEffect->getContentSize().height));
        auto action1_2 = EaseOut::create(action1_1, 20.0);
        auto action1_3 = FadeOut::create(0.4);
        auto seq = Sequence::create(action1_2, action1_3, nullptr);
        spriteEffect->runAction(seq);
    }
    auto seqParent = Sequence::create(DelayTime::create(0.9), RemoveSelf::create(), CallFunc::create([=](void){
        actionAttackMiddle(isCritical,isDoubleAttack);
    }),nullptr);
    spriteEffect->runAction(seqParent);
    
    _sprAttackFx->setVisible(true);
}

void UILayerRaidMain::actionAttackMiddle(bool isCritical, bool isDoubleAttack)
{
    _hero->actionAttack();
    
    _sprDragonHead->stopAllActions();
    _sprDragonHead->setPosition(_lyBattle->getContentSize()/2);
    
    auto action1 = CallFunc::create([=](){
        
        _sprDragonHead->setTexture("Assets/ui/raid/raid_bg_1_3.png");
        
        std::string sprFrontPath = "Assets/character_hero/normal/effect_hit/ef_hit_3_1_1.png";
        std::string sprBackPath = "Assets/character_hero/normal/effect_hit/ef_hit_3_2_1.png";
        
        auto spriteHitFront = Sprite::create(sprFrontPath);
        spriteHitFront->setPosition(_sprDragonHead->getContentSize().width/2 + 10, _sprDragonHead->getContentSize().height / 2.5f);
        _sprDragonHead->addChild(spriteHitFront);
        
        auto spriteHitBack = Sprite::create(sprBackPath);
        spriteHitBack->setPosition(_sprDragonHead->getContentSize().width/2 + 10, _sprDragonHead->getContentSize().height / 2.5f);
        spriteHitBack->setColor(Color3B(random(1, 255), random(1, 255), random(1, 255)));
        _sprDragonHead->addChild(spriteHitBack);
        
        auto animFront = Animation::create();
        animFront->setDelayPerUnit(0.1);
        animFront->addSpriteFrameWithFile("Assets/character_hero/normal/effect_hit/ef_hit_3_1_1.png");
        animFront->addSpriteFrameWithFile("Assets/character_hero/normal/effect_hit/ef_hit_3_1_2.png");
        animFront->addSpriteFrameWithFile("Assets/character_hero/normal/effect_hit/ef_hit_3_1_3.png");
        animFront->addSpriteFrameWithFile("Assets/character_hero/normal/effect_hit/ef_hit_3_1_4.png");
        
        auto animBack = Animation::create();
        animBack->setDelayPerUnit(0.1);
        animBack->addSpriteFrameWithFile("Assets/character_hero/normal/effect_hit/ef_hit_3_2_1.png");
        animBack->addSpriteFrameWithFile("Assets/character_hero/normal/effect_hit/ef_hit_3_2_2.png");
        animBack->addSpriteFrameWithFile("Assets/character_hero/normal/effect_hit/ef_hit_3_2_3.png");
        animBack->addSpriteFrameWithFile("Assets/character_hero/normal/effect_hit/ef_hit_3_2_4.png");
        
        spriteHitFront->getTexture()->setTexParameters(texParams);
        spriteHitFront->setScale(2.f);
        spriteHitBack->getTexture()->setTexParameters(texParams);
        spriteHitBack->setScale(2.f);
        
        spriteHitFront->runAction(Sequence::create(Animate::create(animFront), RemoveSelf::create() ,nullptr));
        spriteHitBack->runAction(Sequence::create(Animate::create(animBack), RemoveSelf::create() ,nullptr));
    });
    auto action2 = DelayTime::create(0.7);
    
    auto seq = Sequence::create(action1,action2, CallFunc::create([=](){
        actionAttackEnd(isCritical,isDoubleAttack);
    }), nullptr);
    
    
    actionDamage(isCritical,isDoubleAttack);
    
    _lyBattle->runAction(seq);
}

void UILayerRaidMain::actionDamage(bool isCritical, bool isDoubleAttack)
{
    int fontSize = 60;
    int nNowDamage = NewRaidManager::getInstance()->getNowDamage();
    Color3B color = Color3B::WHITE;
    auto lbDamage = MafNode::createBmFont("", GAME_BM_FONT_AL, 24);
    lbDamage->setPosition(_sprDragonHead->getContentSize().width/2 + 100, _sprDragonHead->getContentSize().height - _sprDragonHead->getContentSize().height / 2.4);
    Label* lbDamageDouble = nullptr;
    
    std::string dmg = "";
    dmg = MafUtils::doubleToString((double)nNowDamage);
    MafUtils::convertNumberToShort(dmg);
    if(dmg == "0")
    {
        dmg = "";
    }
    lbDamage->setString(dmg.c_str());
    lbDamage->setOpacity(255);
    _lyBattle->addChild(lbDamage);
    
    if(isDoubleAttack)
    {
        fontSize = 72;
        lbDamageDouble = MafNode::createBmFont("", GAME_BM_FONT_AL, 24);
        lbDamageDouble->setPosition(lbDamage->getPosition() + Vec2::UP * 60);
        
        std::string dmg = "";
        dmg = MafUtils::doubleToString((double)nNowDamage/2);
        MafUtils::convertNumberToShort(dmg);
        if(dmg == "0")
        {
            dmg = "";
        }
        lbDamage->setString(dmg.c_str());
        lbDamageDouble->setString(dmg.c_str());
        lbDamageDouble->setOpacity(255);
        _lyBattle->addChild(lbDamageDouble);
    }
    if(isCritical)
    {
        fontSize = 90;
        color = Color3B(255, 255, 0);
        
        if(lbDamageDouble != nullptr)
        {
            
            lbDamageDouble->setPosition(lbDamage->getPosition() + Vec2::UP * 60);
        }
    }
    
    lbDamage->setColor(color);
    MafNode::setNewScale(*lbDamage, fontSize);
    
    MoveBy* moveLb = MoveBy::create(0.5f, Vec2(0,50));
    FadeOut* fadeLb = FadeOut::create(0.2f);
    Sequence* seqLb = Sequence::create(moveLb,fadeLb,RemoveSelf::create(), nullptr);
    EaseIn* easeLb = EaseIn::create(seqLb, 0.5f);
    lbDamage->runAction(easeLb);
    
    if(lbDamageDouble != nullptr)
    {
        lbDamageDouble->setColor(color);
        MafNode::setNewScale(*lbDamageDouble, fontSize);
        
        MoveBy* moveLb = MoveBy::create(0.5f, Vec2(0,50));
        FadeOut* fadeLb = FadeOut::create(0.2f);
        Sequence* seqLb = Sequence::create(moveLb,fadeLb,RemoveSelf::create(), nullptr);
        EaseIn* easeLb = EaseIn::create(seqLb, 0.5f);
        lbDamageDouble->runAction(easeLb);
    }
}

void UILayerRaidMain::actionAttackEnd(bool isCritical, bool isDoubleAttack)
{
    NewRaidManager::getInstance()->setIsAttack(false);
    _sprAttackFx->setVisible(false);
    PopupToastItems::show(_listReward);
    _listReward.clear();
    refreshUI();
}

void UILayerRaidMain::actionLevelup()
{
    auto sprLevelup = Sprite::create("Assets/ui/raid/level_1_1.png");
    sprLevelup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprLevelup->setPosition(_hero->getPosition() + Vec2::UP * 60 + Vec2::LEFT * 50);
    _lyUI->addChild(sprLevelup);
    
    auto action1 = MoveBy::create(0.4f, Vec2(0, 30));
    auto action2 = Sequence::create(DelayTime::create(0.1f), FadeOut::create(0.5f), nullptr);
    auto spawn = Spawn::create(action1, action2, nullptr);
    
    sprLevelup->runAction(Sequence::create(spawn, DelayTime::create(1.5f),RemoveSelf::create(), nullptr));
}

#pragma mark - callback
void UILayerRaidMain::callbackAttack(int nResult, bool isCritical, bool isDoubleAttack, Vector<InfoItems*> listReward)
{
    PopupLoading::hide();
    
    if(nResult != 1)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if(nResult == 3)
            strMessageKey = "t_ui_error_31";
        else if( nResult == 8 )
        {
            strMessageKey = "t_ui_new_raid_error_1";
        }
        else if( nResult == 9 )
        {
            strMessageKey = "t_ui_new_raid_error_2";
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        if(nResult == 8 || nResult == 9)
        {
            if(_callbackHide != nullptr)
                _callbackHide();
        }
        return;
    }
    
    actionAttackStart(isCritical, isDoubleAttack);
    _listReward = listReward;
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::RAID_STAT);
}

void UILayerRaidMain::callbackLevelup()
{
    actionLevelup();
}

#pragma mark - Purchase
void UILayerRaidMain::callbackProductOk(int nIdx)
{
    //
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    
    PopupLoading::show();
    CashManager::getInstance()->purchaseInitiate(nIdx, CC_CALLBACK_4(UILayerRaidMain::callbackPurchase, this));
}

void UILayerRaidMain::callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    //
    PopupLoading::hide();
    
    //
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        // 재화 다시 가져오기.
        PopupLoading::show();
        CashManager::getInstance()->requestInfo([=](bool){
            PopupLoading::hide();
            RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
        });
        
        PopupLoading::show();
        SaveManager::getInstance()->requestSave([=](bool bResult){
            PopupLoading::hide();
        });
        
        PopupLoading::show();
        NewRaidManager::getInstance()->requestInfo([=](bool, int){
            PopupLoading::hide();
            
            RefreshManager::getInstance()->refreshUI(E_REFRESH::RAID_ALL);
        });
        
        // alert
        std::string strText = GAME_TEXT("t_ui_cash_msg_1");
        if(nIdx == 70000000)
            strText = GAME_TEXT("t_ui_new_raid_ticket_pop_2");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strText);
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
         2 #이벤트 중이 아닙니다. ( 복귀자 )
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
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}

void UILayerRaidMain::callbackHide()
{
    if(_callbackHide != nullptr)
        _callbackHide();
}

