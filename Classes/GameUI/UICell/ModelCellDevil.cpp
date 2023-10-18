//
//  ModelCell.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 8. 7..
//
//

#include "ModelCellDevil.h"

#include "Common/ConfigGameDevil.h"

#include "GameObject/InfoArtifact.h"

#include "GameScene/DevilDomScene.h"

#include "GameUIPopup/Other/ArtifactCalculator/PopupCalArtifact.h"
#include "GameUIPopup/Other/Pet/PopupPetDevil.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/PetNewDevilManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/AchievementManager.h"

USING_NS_CC;
using namespace ModelCellDevil;

#pragma mark-
CellDevilQuestTop::CellDevilQuestTop(UtilsScene::eType eSceneType) :
_nType(0),
_fRedrawTimeMax(3),
_fRedrawTime(0),

_lbTotalMoney(nullptr),
_onCallbackTab(nullptr)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    if(visibleSize.width >= 750)
        visibleSize.width = 750;
    
    bool isHelloWorld = false;
    
    if(eSceneType == UtilsScene::eType::scene_dungeon_normal)
        isHelloWorld = true;
    
    if(isHelloWorld == true)
        setContentSize(Size(visibleSize.width, 120));
    else
        setContentSize(Size(visibleSize.width, 64));
    
    
    
    //
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    addChild(menu);
    
    _itemTab0 = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/quest_btn_off.png"), Sprite::create("Assets/ui_common/quest_btn_off.png"), Sprite::create("Assets/ui_common/quest_btn_on.png"), CC_CALLBACK_1(CellDevilQuestTop::onClickTab, this));
    if(isHelloWorld == true)
    {
    _itemTab0->setPosition(getContentSize().width - 5 - _itemTab0->getContentSize().width / 2, getContentSize().height);
    _itemTab0->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    }
    else
    {
    _itemTab0->setPosition(getContentSize().width - 5 - _itemTab0->getContentSize().width / 2, getContentSize().height / 2);
    }
    _itemTab0->setTag(1);
    menu->addChild(_itemTab0);
    {
        auto label = Label::createWithTTF("+Lv.1", GAME_FONT, 24);
        label->setPosition(_itemTab0->getContentSize().width / 2, _itemTab0->getContentSize().height / 2);
        label->setDimensions(_itemTab0->getContentSize().width * 0.9, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        label->setTag(10000);
        _itemTab0->addChild(label);
    }
    
    _itemTab1 = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/quest_btn_off.png"), Sprite::create("Assets/ui_common/quest_btn_off.png"), Sprite::create("Assets/ui_common/quest_btn_on.png"), CC_CALLBACK_1(CellDevilQuestTop::onClickTab, this));
    if(isHelloWorld == true)
    {
        _itemTab1->setPosition(_itemTab0->getPositionX() - _itemTab0->getContentSize().width / 2 - _itemTab1->getContentSize().width / 2, getContentSize().height);
        _itemTab1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    }
    else
    {
        _itemTab1->setPosition(_itemTab0->getPositionX() - _itemTab0->getContentSize().width / 2 - _itemTab1->getContentSize().width / 2, getContentSize().height / 2);
    }
    
    _itemTab1->setTag(2);
    menu->addChild(_itemTab1);
    {
        auto label = Label::createWithTTF("+Lv.10", GAME_FONT, 24);
        label->setPosition(_itemTab1->getContentSize().width / 2, _itemTab1->getContentSize().height / 2);
        label->setDimensions(_itemTab1->getContentSize().width * 0.9, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        label->setTag(10000);
        _itemTab1->addChild(label);
    }
    
    _itemTab2 = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/quest_btn_off.png"), Sprite::create("Assets/ui_common/quest_btn_off.png"), Sprite::create("Assets/ui_common/quest_btn_on.png"), CC_CALLBACK_1(CellDevilQuestTop::onClickTab, this));
    if(isHelloWorld == true)
    {
        _itemTab2->setPosition(_itemTab1->getPositionX() - _itemTab1->getContentSize().width / 2 - _itemTab2->getContentSize().width / 2, getContentSize().height);
        _itemTab2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    }
    else
    {
        _itemTab2->setPosition(_itemTab1->getPositionX() - _itemTab1->getContentSize().width / 2 - _itemTab2->getContentSize().width / 2, getContentSize().height / 2);
    }
    _itemTab2->setTag(3);
    menu->addChild(_itemTab2);
    {
        auto label = Label::createWithTTF("+Lv.100", GAME_FONT, 24);
        label->setPosition(_itemTab2->getContentSize().width / 2, _itemTab2->getContentSize().height / 2);
        label->setDimensions(_itemTab2->getContentSize().width * 0.9, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        label->setTag(10000);
        _itemTab2->addChild(label);
    }
    
    _itemTab3 = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/quest_btn_off.png"), Sprite::create("Assets/ui_common/quest_btn_off.png"), Sprite::create("Assets/ui_common/quest_btn_on.png"), CC_CALLBACK_1(CellDevilQuestTop::onClickTab, this));
    if(isHelloWorld == true)
    {
        _itemTab3->setPosition(_itemTab2->getPositionX() - _itemTab2->getContentSize().width / 2 - _itemTab3->getContentSize().width / 2, getContentSize().height);
        _itemTab3->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    }
    else
    {
        _itemTab3->setPosition(_itemTab2->getPositionX() - _itemTab2->getContentSize().width / 2 - _itemTab3->getContentSize().width / 2, getContentSize().height / 2);
    }
    
    _itemTab3->setTag(4);
    menu->addChild(_itemTab3);
    {
        auto label = Label::createWithTTF("+Lv.Max", GAME_FONT, 24);
        label->setPosition(_itemTab3->getContentSize().width / 2, _itemTab3->getContentSize().height / 2);
        label->setDimensions(_itemTab3->getContentSize().width * 0.9, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        label->setTag(10000);
        _itemTab3->addChild(label);
    }
    if(isHelloWorld == true)
    {
        auto sprIcon = Sprite::create("Assets/icon/menu_coin.png");
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprIcon->setPosition(Vec2(10, sprIcon->getContentSize().height/2));
        addChild(sprIcon);
        {
            _lbTotalMoney = Label::createWithTTF("", GAME_FONT, 24);
            _lbTotalMoney->enableBold();
            _lbTotalMoney->setAnchorPoint(sprIcon->getAnchorPoint());
            _lbTotalMoney->setPosition(sprIcon->getPosition() + Vec2::RIGHT * sprIcon->getContentSize().width);
            addChild(_lbTotalMoney);
        }
    }
    //
    onClickTab(_itemTab0);
    if(isHelloWorld == true)
    {
        changeTotalGold();
        scheduleUpdate();
    }
}

CellDevilQuestTop::~CellDevilQuestTop()
{
    
}

void CellDevilQuestTop::update(float dt)
{
    _fRedrawTime += dt;
    if (_fRedrawTime >= _fRedrawTimeMax)
    {
        _fRedrawTime = 0;
        changeTotalGold();
    }
}


void CellDevilQuestTop::setCallbackTab(const std::function<void(int)>& callback)
{
    _onCallbackTab = callback;
}

void CellDevilQuestTop::changeTotalGold()
{
    std::string money = UserInfoManager::getInstance()->getSecondEarnGoldNormal();
    MafUtils::convertNumberToShort(money);
    if(_lbTotalMoney != nullptr)
        _lbTotalMoney->setString(GAME_TEXTFORMAT("t_ui_quest_gold", money.c_str()));
}

void CellDevilQuestTop::onClickTab(cocos2d::Ref* sender)
{
    auto item = (MenuItem*)sender;
    
    if ( _nType == item->getTag() )
    {
        return;
    }
 
    
    _nType = item->getTag();
    
    auto listChilds = item->getParent()->getChildren();
    for ( auto child : listChilds )
    {
        auto temp = (MafNode::MafMenuItem*)child;
        
        if ( temp == nullptr )
        {
            continue;
        }
        
        temp->setEnabled(true);
        if ( _nType == child->getTag() )
        {
            temp->setEnabled(false);
        }
        
        auto label = (Label*)temp->getChildByTag(10000);
        if ( label != nullptr )
        {
            label->setTextColor(Color4B(115, 116, 118, 255));
            if ( _nType == child->getTag() )
            {
                label->setTextColor(Color4B::WHITE);
            }
        }
    }
    
    if ( _onCallbackTab != nullptr )
    {
        _onCallbackTab(_nType);
    }
}

#pragma mark-
CellDevilQuest::CellDevilQuest(E_PLACE ePlace, E_QUEST eQuest)
{
    _ePlace = ePlace;
    _eQuest = eQuest;
    
    _uNextSpend[0] = _uNextSpend[1] = _uNextSpend[2] = "0";
    _uQuestEarnMoney = "0";
    _uQuestNextEarnMoney = "0";

    _bQuestStart = false;
    _bDoubleCoinResume = true;
    
    _strLevelAdd = MafAes256::XorEncrypt(1);
    
    int level =  UserInfoManager::getInstance()->getQuestLevel(E_PLACE::DG_DEVIL_DOM,_eQuest);
    _levelEncrypt = MafAes256::XorEncrypt(level);
    if ( level == 0 )
    {
        _itemStartBtn_disable = Sprite::create("Assets/ui_common/box_quest_disable.png");
        _itemStartBtn_disable->setAnchorPoint(Vec2(0,0));
        _itemStartBtn_disable->setPosition(0,0);
        _itemStartBtn->addChild(_itemStartBtn_disable,1);
    }
    else
    {
        _itemStartBtn_disable = nullptr;
    }
    
    Sprite *sprBarBg = Sprite::create("Assets/battle/gauge_bg.png");
    sprBarBg->setPosition(Vec2(listSize.width*0.02+_itemStartBtn->getContentSize().width,
                               _listBg->getContentSize().height*0.1));
    sprBarBg->setAnchorPoint(Vec2(0,0));
    _listBg->addChild(sprBarBg);
    
    Sprite *sprBar = Sprite::create("Assets/battle/gauge_gauge.png");
    sprBar->setAnchorPoint(Vec2(0,0.5));
    ProgressTimer *progress = ProgressTimer::create(sprBar);
    progress->setType(ProgressTimer::Type::BAR);
    progress->setPercentage(0);
    progress->setMidpoint(Vec2(0,0.5));
    progress->setBarChangeRate(Vec2(1,0));
    progress->setAnchorPoint(Vec2(0,0));
    sprBarBg->addChild(progress);
    _bar = progress;
    
    auto sprCoin = Sprite::create("Assets/icon/icon_gold_1.png");
    sprCoin->setAnchorPoint(Vec2(0,0.5));
    sprCoin->setPosition(Vec2(-10, sprBarBg->getContentSize().height/2));
    sprBarBg->addChild(sprCoin);
    
    _lbTitle->setTTFConfig(TTFConfig(GAME_FONT, 26, GlyphCollection::DYNAMIC));
    _lbTitle->setString(DataManager::GetQuestName(E_PLACE::DG_DEVIL_DOM,_eQuest));
    
//    _lbLv = Label::createWithTTF("Lv", GAME_FONT, 23);
    _lbLv = MafNode::createBmFont("", GAME_BM_FONT_AL, 22);
    _lbLv->setColor(Color3B::WHITE);
    _lbLv->setAlignment(TextHAlignment::LEFT);
    _lbLv->setAnchorPoint(Vec2(0,0.5f));
    _lbLv->setPosition(Vec2(listSize.width*0.025+_itemStartBtn->getContentSize().width, listSize.height*0.55f));
    _listBg->addChild(_lbLv);
    
    _sprLvUpEffect = Sprite::create("Assets/ui_common/ef_level_1.png");
    _sprLvUpEffect->setPosition(Vec2(_lbLv->getPosition().x+40,_lbLv->getPosition().y));
    _sprLvUpEffect->setVisible(false);
    _listBg->addChild(_sprLvUpEffect);
    
    _lbMoney->retain();
    _lbMoney->removeFromParent();
    sprBarBg->addChild(_lbMoney);
    _lbMoney->setPosition(Vec2(50,sprBarBg->getContentSize().height/2));
    _lbMoney->setAnchorPoint(Vec2(0,0.5));
    _lbMoney->setAlignment(TextHAlignment::LEFT);
    
    _lbTime = MafNode::createBmFont("0", GAME_BM_FONT_AL, 28);
    _lbTime->setColor(Color3B::WHITE);
    _lbTime->setPosition(Vec2(sprBarBg->getContentSize().width-10,sprBarBg->getContentSize().height/2));
    _lbTime->setAnchorPoint(Vec2(1,0.5));
    _lbTime->setAlignment(TextHAlignment::RIGHT);
    sprBarBg->addChild(_lbTime);
    
    _sprIcon->setTexture(MafUtils::format("Assets/icon_quest/devil/quest_%.2d.png", _eQuest).c_str());
    _sprIcon->getTexture()->setTexParameters(texParams);
    _sprIcon->setScale(_itemStartBtn->getContentSize().width / _sprIcon->getContentSize().width);
    
    _itemStartBtn->setEnabled(false);
    
    _sprMoney = Sprite::create("Assets/icon/icon_gold_1.png");
    _sprMoney->setPosition(Vec2(_itemStartBtn->getContentSize().width/2,_itemStartBtn->getContentSize().height/2));
    _itemStartBtn->addChild(_sprMoney);
    _sprMoney->setOpacity(0);
    

    _itemUpgradeBtn->setCallback(CC_CALLBACK_1(CellDevilQuest::onUpgrade, this));
    _itemUpgradeBtn->setTag(1);
    
    
    scheduleUpdate();
    
    setChangeStateListener(CC_CALLBACK_0(CellDevilQuest::drawChangeState,this));
    drawChangeState();
}

CellDevilQuest::~CellDevilQuest()
{
    
}

void CellDevilQuest::update(float dt)
{
    
    _fRedrawTime += dt;
    if (_fRedrawTime >= _fRedrawTimeMax)
    {
        _fRedrawTime = 0;
        
        int level = MafAes256::XorDecryptInt(_levelEncrypt);
        if ( level >= C_MAX_QUEST )
        {
            _itemUpgradeBtn->setVisible(false);
            unscheduleUpdate();
            return;
        }
        
        auto money= ItemsMoneyManager::getInstance()->getDevilGold();
        if(MafUtils::compareStr(money, _uNextSpend[0]))
        {
            ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2.png");
        }
        else
        {
            ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
        }
    }
}

void CellDevilQuest::drawChangeState()
{
    setSpend(_eState);
    drawUpgrade(_eState);
}

void CellDevilQuest::drawUpgrade(ModelCell::E_CHANGE_STATE change)
{
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    _lbLv->setString(MafUtils::format("LV. %d", level));
    _lbMoney->setString(_uQuestEarnMoneyShort);
    
    if ( level >= C_MAX_QUEST )
    {
        _itemUpgradeBtn->setVisible(false);
        unscheduleUpdate();
    }
    else
    {
        _lbUpgrade->setString(_uNextSpendShort);
        _lbUpgrade_top->setString(_uQuestNextEarnMoneyShort);
    }
}

void CellDevilQuest::setSpend(ModelCell::E_CHANGE_STATE change)
{
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    int levelPlus = MafAes256::XorDecryptInt(_strLevelAdd);
    if ( level + levelPlus >= C_MAX_QUEST )
    {
        levelPlus = C_MAX_QUEST - level;
    }
    
    //
    double total = 0;
    for ( int i = level; i < level+levelPlus; i++ )
    {
        auto temp = DataManager::GetQuestUpgradeMoney(E_PLACE::DG_DEVIL_DOM,_eQuest, i);
        total = total + temp;
    }
    
    _uNextSpend[0] = MafUtils::doubleToString(total);
    _uNextSpendShort = _uNextSpend[0];
    _uNextSpendShort = MafUtils::convertNumberToShort(_uNextSpendShort);
    
    //
    _uQuestEarnMoney = UserInfoManager::getInstance()->getQuestEarnGold(_ePlace, _eQuest, level);
    _uQuestEarnMoneyShort = _uQuestEarnMoney;
    _uQuestEarnMoneyShort = MafUtils::convertNumberToShort(_uQuestEarnMoneyShort,true);
    
    //
    _uQuestNextEarnMoney = UserInfoManager::getInstance()->getQuestEarnGold(_ePlace, _eQuest, level + levelPlus);
    _uQuestNextEarnMoney = MafUtils::bigSubNum(_uQuestNextEarnMoney, _uQuestEarnMoney);
    
    _uQuestNextEarnMoneyShort = _uQuestNextEarnMoney;
    _uQuestNextEarnMoneyShort = MafUtils::convertNumberToShort(_uQuestNextEarnMoneyShort);
    _uQuestNextEarnMoneyShort = "+" + _uQuestNextEarnMoneyShort;
}

void CellDevilQuest::onUpgrade(cocos2d::Ref* pSender)
{
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    int levelOld = level;
    if ( level >= C_MAX_QUEST )
    {
        return;
    }
    
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    int levelMax = level + MafAes256::XorDecryptInt(_strLevelAdd);
    if ( levelMax >= C_MAX_QUEST )
    {
        levelMax = C_MAX_QUEST;
    }
    
    auto money = ItemsMoneyManager::getInstance()->getDevilGold();
    if ( MafUtils::compareStr(money, _uNextSpend[0]) == true )
    {
        level = levelMax;
        
        std::string tempSpend = std::string("-").append(_uNextSpend[0]);
        ItemsMoneyManager::getInstance()->setDevilGold(tempSpend, false);
    }
    else
    {
        for ( int i = level; i < levelMax; i++ )
        {
            double temp = DataManager::GetQuestUpgradeMoney(E_PLACE::DG_DEVIL_DOM,_eQuest, i);
            std::string strTemp = MafUtils::doubleToString(temp);
            
            auto money = ItemsMoneyManager::getInstance()->getDevilGold();
            if ( MafUtils::compareStr(money, strTemp) == false )
            {
                break;
            }
            
            std::string tempSpend = std::string("-").append(strTemp);
            ItemsMoneyManager::getInstance()->setDevilGold(tempSpend, false, false);
            
            level = i+1;
        }
        
        //
        RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
        
        if ( levelOld == level )
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::D_GOLD);
            popup->show();
        }
    }
    
    if ( levelOld != level )
    {
        _sprLvUpEffect->setVisible(true);
        _listBg->reorderChild(_sprLvUpEffect, 50);
        
        
        auto animationEffect = Animation::create();
        animationEffect->setDelayPerUnit(0.03f);
        animationEffect->addSpriteFrameWithFile("Assets/ui_common/ef_level_1.png");
        animationEffect->addSpriteFrameWithFile("Assets/ui_common/ef_level_2.png");
        animationEffect->addSpriteFrameWithFile("Assets/ui_common/ef_level_3.png");
        animationEffect->addSpriteFrameWithFile("Assets/ui_common/ef_level_4.png");
        auto actionEffect1 = Animate::create(animationEffect);
        
        auto actionEffect2 = CallFunc::create([=](){
            
            _sprLvUpEffect->setVisible(false);
        });
        
        auto sequenceEffect = Sequence::create(actionEffect1,actionEffect2, NULL);
        _sprLvUpEffect->stopAllActions();
        _sprLvUpEffect->runAction(sequenceEffect);
    }
    
    if ( level >= C_MAX_QUEST )
    {
        level = C_MAX_QUEST;
    }
    UserInfoManager::getInstance()->setQuestLevel(E_PLACE::DG_DEVIL_DOM,_eQuest, level);
    _levelEncrypt = MafAes256::XorEncrypt(level);
    
    setState(ModelCell::E_CHANGE_STATE::LEVEL);
    start();
}

void CellDevilQuest::start()
{
    if(_bQuestStart)
        return;
    
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    if ( level == 0 )
        return;
    
    _bQuestStart = true;
    
    auto _timer = DataManager::GetQuestSpendTime(E_PLACE::DG_DEVIL_DOM,_eQuest);
    auto perTime = UserInfoManager::getInstance()->getQuestTimeRate(E_PLACE::DG_DEVIL_DOM);
    if(perTime<100){
        _timer = _timer*perTime*0.01;
    }
    
    UserInfoManager::getInstance()->setQuestTime(E_PLACE::DG_DEVIL_DOM,_eQuest, _timer,false);
    
    ProgressFromTo *timer = ProgressFromTo::create(_timer, _bar->getPercentage(), 100);
    CallFunc *call = CallFunc::create(CC_CALLBACK_0(CellDevilQuest::onCompleteCallBack,this,this));
    Sequence *seq= Sequence::create(timer,call, nullptr);
    _bar->runAction(seq);
    //    CallFunc *call = CallFunc::create(CC_CALLBACK_0(CellDevilQuest::onCompleteCallBack,this,this));
    //    Sequence *seq= Sequence::create(DelayTime::create(_timer),call, nullptr);
    //    _bar->runAction(seq);
    
    CallFunc *callTimer = CallFunc::create(CC_CALLBACK_0(CellDevilQuest::drawTimer,this));
    seq = Sequence::create(callTimer,DelayTime::create(1.0f),nullptr);
    Repeat *rep = Repeat::create(seq, _timer);
    _lbTime->runAction(rep);
    
    if ( level > 0 )
    {
        if(_itemStartBtn_disable !=nullptr){
            _itemStartBtn_disable->removeFromParentAndCleanup(true);
            _itemStartBtn_disable = nullptr;
        }
    }
}

void CellDevilQuest::startResume()
{
    _bar->stopAllActions();
    _lbTime->stopAllActions();
    
    auto _timer = DataManager::GetQuestSpendTime(E_PLACE::DG_DEVIL_DOM,_eQuest);
    auto maxTimer = _timer;
    auto perTime = UserInfoManager::getInstance()->getQuestTimeRate(E_PLACE::DG_DEVIL_DOM);
    if(perTime<100){
        _timer = _timer*perTime*0.01;
        maxTimer = _timer;
    }
    
    auto savedTime = UserInfoManager::getInstance()->getQuestTime(E_PLACE::DG_DEVIL_DOM,_eQuest);
    if(savedTime <_timer){
        _timer = savedTime;
    }
    
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    if ( level == 0 )
    {
        _bar->setPercentage(0);
        drawTimer();
    }
    else if(_timer==-1){
        _bQuestStart = false;
        start();
    }
    else if(_timer==0){
        _bDoubleCoinResume = false;
        onCompleteCallBack(this);
    }
    else if(_timer<maxTimer){
        
        _bQuestStart = true;
        
        //_lbTap->setVisible(false);
        int curPer = 100 - _timer*100/maxTimer;
        _bar->setPercentage(curPer);
        
        ProgressFromTo *timer = ProgressFromTo::create(_timer, _bar->getPercentage(), 100);
        CallFunc *call = CallFunc::create(CC_CALLBACK_0(CellDevilQuest::onCompleteCallBack,this,this));
        Sequence *seq= Sequence::create(timer,call, nullptr);
        _bar->runAction(seq);
        
        CallFunc *callTimer = CallFunc::create(CC_CALLBACK_0(CellDevilQuest::drawTimer,this));
        seq = Sequence::create(callTimer,DelayTime::create(1.0f),nullptr);
        Repeat *rep = Repeat::create(seq, _timer);
        _lbTime->runAction(rep);
    }
    else{
        _bQuestStart = false;
        start();
    }
}

void CellDevilQuest::drawTimer(){
    
    auto _timer = UserInfoManager::getInstance()->getQuestTime(E_PLACE::DG_DEVIL_DOM,_eQuest);
    if(_timer<0){
        _lbTime->setString("00:00:00");
    }
    else{
        _lbTime->setString(MafUtils::convertNumberToTime(_timer, true));
        _timer -= 1.0;
        if(_timer<1.0)
            _timer = 0;
    }

    UserInfoManager::getInstance()->setQuestTime(E_PLACE::DG_DEVIL_DOM,_eQuest, _timer,false);
}

void CellDevilQuest::onCompleteCallBack(CellDevilQuest *cell)
{
    _sprMoney->setOpacity(255);
    if(_eQuest != E_QUEST::QUEST_1){
        Sequence *seq = Sequence::create(FadeOut::create(0.3), nullptr);
        _sprMoney->runAction(seq);
    }
    
    ItemsMoneyManager::getInstance()->setDevilGold(_uQuestEarnMoney);
    
    _bQuestStart = false;
    _bDoubleCoinResume = true;
    _bar->setPercentage(0);
    
    UserInfoManager::getInstance()->setQuestTime(E_PLACE::DG_DEVIL_DOM,_eQuest, -1,false);
    drawTimer();
    start();
}

void CellDevilQuest::setLevelAdd(int level)
{
    int levelAddNow = MafAes256::XorDecryptInt(_strLevelAdd);
    if ( levelAddNow == level )
    {
        return;
    }
    
    _strLevelAdd = MafAes256::XorEncrypt(level);
    
    setSpend(_eState);
    drawUpgrade(_eState);
}


#pragma mark-

CellEquipTop::CellEquipTop() :
_nType(0),
_onCallbackTab(nullptr)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    setContentSize(Size(visibleSize.width, 64));
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    addChild(menu);
    
    _itemTab0 = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/quest_btn_off.png"), nullptr, Sprite::create("Assets/ui_common/quest_btn_on.png"), CC_CALLBACK_1(CellEquipTop::onClickTab, this));
    _itemTab0->setPosition(getContentSize().width - 5 - _itemTab0->getContentSize().width / 2, getContentSize().height / 2);
    _itemTab0->setTag(1);
    menu->addChild(_itemTab0);
    {
        auto label = Label::createWithTTF("+Lv.1", GAME_FONT, 24);
        label->setPosition(_itemTab0->getContentSize().width / 2, _itemTab0->getContentSize().height / 2);
        label->setDimensions(_itemTab0->getContentSize().width * 0.9, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        label->setTag(10000);
        _itemTab0->addChild(label);
    }
    
    _itemTab1 = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/quest_btn_off.png"), nullptr, Sprite::create("Assets/ui_common/quest_btn_on.png"), CC_CALLBACK_1(CellEquipTop::onClickTab, this));
    _itemTab1->setPosition(_itemTab0->getPositionX() - _itemTab0->getContentSize().width / 2 - _itemTab1->getContentSize().width / 2, getContentSize().height / 2);
    _itemTab1->setTag(2);
    menu->addChild(_itemTab1);
    {
        auto label = Label::createWithTTF("+Lv.10", GAME_FONT, 24);
        label->setPosition(_itemTab1->getContentSize().width / 2, _itemTab1->getContentSize().height / 2);
        label->setDimensions(_itemTab1->getContentSize().width * 0.9, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        label->setTag(10000);
        _itemTab1->addChild(label);
    }
    
    _itemTab2 = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/quest_btn_off.png"), nullptr, Sprite::create("Assets/ui_common/quest_btn_on.png"), CC_CALLBACK_1(CellEquipTop::onClickTab, this));
    _itemTab2->setPosition(_itemTab1->getPositionX() - _itemTab1->getContentSize().width / 2 - _itemTab2->getContentSize().width / 2, getContentSize().height / 2);
    _itemTab2->setTag(3);
    menu->addChild(_itemTab2);
    {
        auto label = Label::createWithTTF("+Lv.100", GAME_FONT, 24);
        label->setPosition(_itemTab2->getContentSize().width / 2, _itemTab2->getContentSize().height / 2);
        label->setDimensions(_itemTab2->getContentSize().width * 0.9, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        label->setTag(10000);
        _itemTab2->addChild(label);
    }
    
    _itemTab3 = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/quest_btn_off.png"), nullptr, Sprite::create("Assets/ui_common/quest_btn_on.png"), CC_CALLBACK_1(CellEquipTop::onClickTab, this));
    _itemTab3->setPosition(_itemTab2->getPositionX() - _itemTab2->getContentSize().width / 2 - _itemTab3->getContentSize().width / 2, getContentSize().height / 2);
    _itemTab3->setTag(4);
    menu->addChild(_itemTab3);
    {
        auto label = Label::createWithTTF("+Lv.Max", GAME_FONT, 24);
        label->setPosition(_itemTab3->getContentSize().width / 2, _itemTab3->getContentSize().height / 2);
        label->setDimensions(_itemTab3->getContentSize().width * 0.9, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        label->setTag(10000);
        _itemTab3->addChild(label);
    }
    
    //
    onClickTab(_itemTab0);
}

CellEquipTop::~CellEquipTop()
{
    
}

void CellEquipTop::setCallbackTab(const std::function<void(int)>& callback)
{
    _onCallbackTab = callback;
}

void CellEquipTop::onClickTab(cocos2d::Ref* sender)
{
    auto item = (MenuItem*)sender;
    
    if ( _nType == item->getTag() )
    {
        return;
    }
    
    
    _nType = item->getTag();
    
    auto listChilds = item->getParent()->getChildren();
    for ( auto child : listChilds )
    {
        auto temp = (MafNode::MafMenuItem*)child;
        
        if ( temp == nullptr )
        {
            continue;
        }
        
        temp->setEnabled(true);
        if ( _nType == child->getTag() )
        {
            temp->setEnabled(false);
        }
        
        auto label = (Label*)temp->getChildByTag(10000);
        if ( label != nullptr )
        {
            label->setTextColor(Color4B(115, 116, 118, 255));
            if ( _nType == child->getTag() )
            {
                label->setTextColor(Color4B::WHITE);
            }
        }
    }
    
    if ( _onCallbackTab != nullptr )
    {
        _onCallbackTab(_nType);
    }
}
#pragma mark-

CellDevilEquip::CellDevilEquip(E_PLACE ePlace, E_EQUIP_TYPE eEquipType)
{
    
    _onCompleteListener = nullptr;
    _ePlace = ePlace;
    _eEquipType= eEquipType;
    _eEquipIdx = UserInfoManager::getInstance()->getEquipDevil(_eEquipType);
    
    _strLevelAdd = MafAes256::XorEncrypt(1);
    int level = UserInfoManager::getInstance()->getEquipLevelDevil(_eEquipType);
    _levelEncrypt = MafAes256::XorEncrypt(level);
    
    _sprIcon = Sprite::create();
    _sprIcon->setPosition(Vec2(_itemStartBtn->getContentSize().width/2,_itemStartBtn->getContentSize().height/2));
    _itemStartBtn->addChild(_sprIcon);
    _itemStartBtn->setCallback(CC_CALLBACK_1(CellDevilEquip::onShowDetail, this));
    _itemStartBtn->setTag((int)_eEquipType);
    
    auto spriteMagnifier = Sprite::create("Assets/ui/review/icon_magnifier.png");
    spriteMagnifier->setPosition( Vec2(_itemStartBtn->getContentSize().width - 15, 15) );
    _itemStartBtn->addChild(spriteMagnifier);
    
    _lbTitle->setTTFConfig(TTFConfig(GAME_FONT, 26, GlyphCollection::DYNAMIC));
    
    _lbLv = MafNode::createBmFont("", GAME_BM_FONT_AL, 22);
    _lbLv->setColor(Color3B::WHITE);
    _lbLv->setAlignment(TextHAlignment::LEFT);
    _lbLv->setAnchorPoint(Vec2(0,0.5f));
    _lbLv->setPosition(Vec2(listSize.width*0.025+_itemStartBtn->getContentSize().width, listSize.height*0.55f));
    _listBg->addChild(_lbLv);
    
    
    std::string str;
    if(_eEquipType == E_EQUIP_TYPE::E_D_WEAPON)
        str = GAME_TEXT("t_equip_d2_contents_weapon");
    else if(_eEquipType == E_EQUIP_TYPE::E_D_HEAD)
        str = GAME_TEXT("t_equip_d2_contents_head");
    else if(_eEquipType == E_EQUIP_TYPE::E_D_BODY)
        str = GAME_TEXT("t_equip_d2_contents_body");
    else if(_eEquipType == E_EQUIP_TYPE::E_D_CAPE)
        str = GAME_TEXT("t_equip_d2_contents_cape");
    
    auto lb_c_atk = Label::createWithTTF(str, GAME_FONT, 30);
    lb_c_atk->setPosition(Vec2(listSize.width*0.025+_itemStartBtn->getContentSize().width, _listBg->getContentSize().height*0.22));
    lb_c_atk->setAnchorPoint(Vec2(0,0.5));
    lb_c_atk->setDimensions(_listBg->getContentSize().width - 116 - 150 - 150 - 30, lb_c_atk->getContentSize().height);
    lb_c_atk->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lb_c_atk->setOverflow(Label::Overflow::SHRINK);
    _listBg->addChild(lb_c_atk);
    
    _lbMoney->setColor(COLOR_COIN);
    _lbMoney->setAlignment(TextHAlignment::RIGHT);
    _lbMoney->setAnchorPoint(Vec2(1,0.5));
    _lbMoney->setPosition(listSize.width - _itemUpgradeBtn->getContentSize().width - 20, _listBg->getContentSize().height*0.22);
    
    
//    _sprUpgradeIcon->setTexture("Assets/icon/icon_gold_1.png");
//    _btnSet->createQuestAddBtn(listSize, menu);
    _itemUpgradeBtn->setCallback(CC_CALLBACK_1(CellDevilEquip::onLevelUp, this));
    _itemUpgradeBtn->setTag(1);
    
    _btnSet->setUpgradeLabelPositionCenter();


    setChangeStateListener(CC_CALLBACK_0(CellDevilEquip::drawChangeState,this));
//    setSpend(true);
//    drawUpgrade(true);
}

CellDevilEquip::~CellDevilEquip(){}

void CellDevilEquip::update(float dt)
{
    
    _fRedrawTime += dt;
    if (_fRedrawTime >= _fRedrawTimeMax)
    {
        _fRedrawTime = 0;
        
        int level = MafAes256::XorDecryptInt(_levelEncrypt);
        if ( level >= C_MAX_EQUIP_LEVEL_DEVIL )
        {
            _itemUpgradeBtn->setVisible(false);
            unscheduleUpdate();
            return;
        }
        
        auto money= ItemsMoneyManager::getInstance()->getDevilGold();
        if(MafUtils::compareStr(money, _uNextSpend[0]))
        {
            ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2.png");
        }
        else
        {
            ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
        }
    }
}

void CellDevilEquip::drawChangeState(){
    setSpend(_eState);
    drawUpgrade(_eState);
}

void CellDevilEquip::setSpend(ModelCell::E_CHANGE_STATE change)
{
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    int levelPlus = MafAes256::XorDecryptInt(_strLevelAdd);
    if ( level + levelPlus >= C_MAX_EQUIP_LEVEL_DEVIL )
    {
        levelPlus = C_MAX_EQUIP_LEVEL_DEVIL - level;
    }
    
    double total = 0;
    for ( int i = level; i < level+levelPlus; i++ )
    {
        auto temp = DataManager::GetEquipUpgradeMoneyDevil(_eEquipIdx, i);
        total = total + temp;
    }
    _uNextSpend[0] = MafUtils::doubleToString(total);
    
    
    if(_eEquipType==E_EQUIP_TYPE::E_D_WEAPON)
        _uAtk = UserInfoManager::getInstance()->getAttackDamageDevil();
    else{
        auto per = UserInfoManager::getInstance()->getEquipTotalEffectPercentDevil(_eEquipType);
        if(_eEquipType==E_EQUIP_TYPE::E_D_BODY){
            if(per>=1000){
                _uAtk = MafUtils::doubleToString(per);
            }
            else{
                _uAtk = MafUtils::format("%.2lf",per);
            }
            
        }
        else{
            _uAtk = MafUtils::doubleToString(per);
        }
        
    }
}

void CellDevilEquip::drawUpgrade(ModelCell::E_CHANGE_STATE change)
{
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    auto data = ModelInfoDevil::DevilEquipInfo::getInstance()->getData(_eEquipType, _eEquipIdx);
    
    auto title = MafUtils::format("%s",data->_title.c_str());
    _lbTitle->setString(title);
    
    auto strLevel = MafUtils::format("LV.%d", level);
    _lbLv->setString(strLevel);
    
    std::string strAtk;
    if(_eEquipType==E_EQUIP_TYPE::E_D_BODY){
        auto offset = _uAtk.find(".");
        if(offset == std::string::npos){
            strAtk = MafUtils::convertNumberToShort(_uAtk);
        }else{
            strAtk = _uAtk;
        }
        
        
    }
    else{
        strAtk = MafUtils::convertNumberToShort(_uAtk);
    }
    
    if(_eEquipType!=E_EQUIP_TYPE::E_D_WEAPON)
        strAtk += "%";
    auto strSpend = _uNextSpend[0];
    MafUtils::convertNumberToShort(strSpend);
    _lbMoney->setString(strAtk);
    _lbUpgrade->setString(strSpend);
    
    //icon
    std::string str;
    if(_eEquipType == E_EQUIP_TYPE::E_D_WEAPON)
        str = MafUtils::format("Assets/icon_equip/icon_weapon_%.2d.png",UserInfoManager::getInstance()->getEquipDevil(_eEquipType));
    else if(_eEquipType == E_EQUIP_TYPE::E_D_HEAD)
        str = MafUtils::format("Assets/icon_equip/icon_head_%.2d.png",UserInfoManager::getInstance()->getEquipDevil(_eEquipType));
    else if(_eEquipType == E_EQUIP_TYPE::E_D_BODY)
        str = MafUtils::format("Assets/icon_equip/icon_body_%.2d.png",UserInfoManager::getInstance()->getEquipDevil(_eEquipType));
    else if(_eEquipType == E_EQUIP_TYPE::E_D_CAPE)
        str = MafUtils::format("Assets/icon_equip/icon_cape_%.2d.png",UserInfoManager::getInstance()->getEquipDevil(_eEquipType));
    _sprIcon->setTexture(str);
}

void CellDevilEquip::onLevelUp(cocos2d::Ref* pSender)
{
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    int levelOld = level;
    if ( level >= C_MAX_EQUIP_LEVEL_DEVIL )
    {
        return;
    }
    
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
 
    int levelUp = level + MafAes256::XorDecryptInt(_strLevelAdd);
    
    auto money = ItemsMoneyManager::getInstance()->getDevilGold();
    if(MafUtils::compareStr(money, _uNextSpend[0]))
    {
        
        level = levelUp;
        if ( level>=C_MAX_EQUIP_LEVEL_DEVIL )
            level = C_MAX_EQUIP_LEVEL_DEVIL;
        
        std::string tempSpend = "-";
        tempSpend.append(_uNextSpend[0]);
        ItemsMoneyManager::getInstance()->setDevilGold(tempSpend,false);
    }
    else
    {
        for ( int i = level; i < levelUp; i++ )
        {
            double temp = DataManager::GetEquipUpgradeMoneyDevil(_eEquipIdx, i);
            std::string strTemp = MafUtils::doubleToString(temp);
            
            auto money = ItemsMoneyManager::getInstance()->getDevilGold();
            if ( MafUtils::compareStr(money, strTemp) == false )
            {
                break;
            }
            
            std::string tempSpend = std::string("-").append(strTemp);
            ItemsMoneyManager::getInstance()->setDevilGold(tempSpend, false, false);

            level = i+1;
        }
        
        //
        RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
        
        if ( levelOld == level )
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::D_GOLD);
            popup->show();
        }
    }
    
    UserInfoManager::getInstance()->setEquipDevil(_eEquipType,_eEquipIdx);
    UserInfoManager::getInstance()->setEquipLevelDevil(_eEquipType, level);
    _levelEncrypt = MafAes256::XorEncrypt(level);
    
    setState(ModelCell::E_CHANGE_STATE::LEVEL);
    
    if(_onCompleteListener!=nullptr)
        _onCompleteListener();
}

void CellDevilEquip::onShowDetail(cocos2d::Ref* pSender){
    _onShowDetailListener(this);
}

void CellDevilEquip::startSchedule(){
    if(!Director::getInstance()->getScheduler()->isScheduled(schedule_selector(CellDevilEquip::update), this))
        Director::getInstance()->getScheduler()->schedule(schedule_selector(CellDevilEquip::update),
                                                          this, 0.1f , kRepeatForever, 0, false);
}

void CellDevilEquip::stopSchedule(){
    if(Director::getInstance()->getScheduler()->isScheduled(schedule_selector(CellDevilEquip::update), this))
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(CellDevilEquip::update), this);
}
void CellDevilEquip::setLevelAdd(int level)
{
    int levelAddNow = MafAes256::XorDecryptInt(_strLevelAdd);
    if ( levelAddNow == level )
    {
        return;
    }
    
    _strLevelAdd = MafAes256::XorEncrypt(level);
    
    setSpend(_eState);
    drawUpgrade(_eState);
}

#pragma mark-




#pragma mark-

//static E_SHOP_ADS _eAds;
CellDevilArtifactTop::CellDevilArtifactTop(){
    
    _currentType = E_D_ARTIFACT_TYPE::E_D_TYPE_1_ATK;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    setContentSize(Size(visibleSize.width,80));
    
    extension::ControlButton *item = extension::ControlButton::create();
    item->addTargetWithActionForControlEvents(this, nullptr, extension::Control::EventType::TOUCH_UP_INSIDE);
    item->setPosition(getContentSize()*0.5);
    item->setPreferredSize(getContentSize());
    item->setScaleRatio(1);
    item->setOpacity(0);
    addChild(item);
    
    
    auto _listBg = ui::Scale9Sprite::create(PIXEL,Rect(0,0,visibleSize.width*0.98,80*0.95));
    _listBg->setPosition(Vec2(getContentSize().width/2,getContentSize().height/2));
    _listBg->setColor(Color3B(29,41,51));
    addChild(_listBg);
    
    auto listSize = _listBg->getContentSize();
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2(0,0));
    menu->setPosition(Vec2(0,0));
    _listBg->addChild(menu);
    
    std::string strOn[5] = {
        "Assets/ui_devil/artifact/artifact_btn_atk_on.png",
        "Assets/ui_devil/artifact/artifact_btn_cri_on.png",
        "Assets/ui_devil/artifact/artifact_btn_gold_on.png",
        "Assets/ui_devil/artifact/artifact_btn_spd_on.png",
        "Assets/ui_devil/artifact/artifact_btn_treasure_on.png"
    };
    std::string strOff[5] = {
        "Assets/ui_devil/artifact/artifact_btn_atk_off.png",
        "Assets/ui_devil/artifact/artifact_btn_cri_off.png",
        "Assets/ui_devil/artifact/artifact_btn_gold_off.png",
        "Assets/ui_devil/artifact/artifact_btn_spd_off.png",
        "Assets/ui_devil/artifact/artifact_btn_treasure_off.png"
    };
    
    for(int i=(int)E_D_ARTIFACT_TYPE::E_D_TYPE_1_ATK; i<=(int)E_D_ARTIFACT_TYPE::E_D_TYPE_4_ATK_SPD; i++){
        auto item = MafNode::MafMenuItemSprite::create(Sprite::create(strOff[i]),
                                                 Sprite::create(strOn[i]),
                                                 CC_CALLBACK_1(CellDevilArtifactTop::onClickItem, this));
        
        item->setPosition(listSize.width*0.02 + (item->getContentSize().width*(0.5+i)) + (20*i),listSize.height*0.5);
        item->setTag(i);
        menu->addChild(item);
        _item[i] = item;
    }
    _item[0]->selected();
    
    
    _itemArtifact = MafNode::MafMenuItemSprite::create(Sprite::create(strOff[4]),
                                          Sprite::create(strOn[4]),
                                          CC_CALLBACK_1(CellDevilArtifactTop::onClickItem, this));
    _itemArtifact->setPosition(listSize.width*0.02 + (_itemArtifact->getContentSize().width*(0.5+4)) + (20*4),listSize.height*0.5);
    _itemArtifact->setTag(E_D_ARTIFACT_TYPE::E_D_TYPE_6_ARTIFACT);
    menu->addChild(_itemArtifact);
    
    
}

CellDevilArtifactTop::~CellDevilArtifactTop(){}

void CellDevilArtifactTop::onClickItem(cocos2d::Ref* pSender){
    
    int tag = ((Node*)pSender)->getTag();
    _currentType = (E_D_ARTIFACT_TYPE)tag;
    
    for(int i=0; i <C_COUNT_ARTIFACT_TYPE_SORT;i++){
        if(i==tag){
            _item[i]->selected();
        }
        else{
            _item[i]->unselected();
        }
    }
    
    if(_currentType == E_D_ARTIFACT_TYPE::E_D_TYPE_6_ARTIFACT){
        _itemArtifact->selected();
    }
    else{
        _itemArtifact->unselected();
    }
    
    //showtable
    _onSelectListener(_currentType);
}

#pragma mark-

CellDevilArtifact::CellDevilArtifact(E_PLACE ePlace, ModelInfoDevil::DevilArtifactInfo &info)
{
    _ePlace = ePlace;
    _info = &info;
    _onCompleteListener = nullptr;
    _lbUpgrade_top->removeFromParentAndCleanup(true);
    
    auto spr1 = Sprite::create("Assets/icon_slate/artifact_box.png");
    auto spr2 = Sprite::create("Assets/icon_slate/artifact_box.png");
    _itemStartBtn->setNormalImage(spr1);
    _itemStartBtn->setSelectedImage(spr2);
    _itemStartBtn->setEnabled(false);
    
    std::string str;
    str = MafUtils::format("Assets/icon_slate/artifact_%.2d.png",info._idx);
    _sprIcon->setTexture(str);
    _sprIcon->setScale(DevilScale);
    _sprIcon->getTexture()->setTexParameters(texParams);
    _sprIcon->setPosition(Vec2(_itemStartBtn->getContentSize().width/2,
                               _itemStartBtn->getContentSize().height/2));

    _lbTitle->setTTFConfig(TTFConfig(GAME_FONT, 26, GlyphCollection::DYNAMIC));
    _lbTitle->setString(_info->_title);
    _lbTitle->setPositionX(_itemStartBtn->getPosition().x + _itemStartBtn->getContentSize().width + 12);
    
    auto lbexplain = Label::createWithTTF(_info->_text, GAME_FONT, 22);
    lbexplain->setAnchorPoint(Vec2(0,1));
    lbexplain->setPosition(Vec2(_itemStartBtn->getPosition().x + _itemStartBtn->getContentSize().width + 12, listSize.height - 15 - 34));
    lbexplain->setDimensions(_listBg->getContentSize().width - 116 - 150 - 15, lbexplain->getContentSize().height);
    lbexplain->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbexplain->setOverflow(Label::Overflow::SHRINK);
    _listBg->addChild(lbexplain);
    
    str.clear();
    if(_info->_type==E_D_ARTIFACT_TYPE::E_D_TYPE_1_ATK){
        if(_info->_idx==0){
            double bonus = 0;
            auto high = UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL);
            high -= 10000;
            if(high>0){
                bonus = high/100*10;
            }
            str = GAME_TEXTFORMAT("t_arti_d2_option_1", bonus);
        }
        else
            str = GAME_TEXTFORMAT("t_arti_d2_option_1", _info->_effect);
    }
    else if(_info->_type==E_D_ARTIFACT_TYPE::E_D_TYPE_2_CRI)
        str = GAME_TEXTFORMAT("t_arti_d2_option_2", _info->_effect);
    else if(_info->_type==E_D_ARTIFACT_TYPE::E_D_TYPE_3_GOLD)
        str = GAME_TEXTFORMAT("t_arti_d2_option_3", _info->_effect);
    else if(_info->_type==E_D_ARTIFACT_TYPE::E_D_TYPE_4_ATK_SPD)
        str = GAME_TEXTFORMAT("t_arti_d2_option_4", _info->_effect);
    else if(_info->_type==E_D_ARTIFACT_TYPE::E_D_TYPE_5_MOVE_SPD)
        str = GAME_TEXTFORMAT("t_arti_d2_option_5", _info->_effect);
    
    _lbMoney = Label::createWithTTF(str, GAME_FONT, 22);
    _lbMoney->setAnchorPoint(Vec2(0,0));
    _lbMoney->setPosition(Vec2(_itemStartBtn->getPosition().x + _itemStartBtn->getContentSize().width + 12, 15));
    _lbMoney->setDimensions(_listBg->getContentSize().width - 116 - 150 - 15, _lbMoney->getContentSize().height);
    _lbMoney->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    _lbMoney->setOverflow(Label::Overflow::SHRINK);
    _listBg->addChild(_lbMoney);

    auto price = MafUtils::doubleToString(_info->_price);
    price = MafUtils::convertNumberToShort(price);
    _lbUpgrade->setString(price);
    _lbUpgrade->setColor(COLOR_D_KEY);    
    _sprUpgradeIcon->setTexture("Assets/icon/icon_gold_2.png");
    _itemUpgradeBtn->setCallback(CC_CALLBACK_1(CellDevilArtifact::onBuyArtifact, this));
    
    _btnSet->setUpgradeLabelPositionCenter();
    
    drawArtifact();
}
CellDevilArtifact::~CellDevilArtifact(){

}
void CellDevilArtifact::update(float dt){

}

void CellDevilArtifact::drawArtifact()
{
    if ( UserInfoManager::getInstance()->isArtifactHaveDevil(_info->_idx) )
    {
        _itemUpgradeBtn->setVisible(false);
    }
    
    auto myKey = ItemsMoneyManager::getInstance()->getDevilKey();
    auto needKey = MafUtils::doubleToString(_info->_price);
    if ( MafUtils::compareStr(myKey, needKey) )
    {
        ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2.png");
    }
    else
    {
        ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
    }
}

void CellDevilArtifact::onBuyArtifact(cocos2d::Ref* pSender){
    
    auto _userMoney = ItemsMoneyManager::getInstance();
    auto myKey = _userMoney->getDevilKey();
    auto needKey = MafUtils::doubleToString(_info->_price);
    
    if ( MafUtils::compareStr(myKey, needKey) )
    {
        UserInfoManager::getInstance()->setArtifactDevil(_info->_idx, true,true);
        needKey.insert(0, "-");
        _userMoney->setDevilKey(needKey);
        drawArtifact();
        
        if(_onCompleteListener!=nullptr)
            _onCompleteListener(_info->_type);
    }
    else
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::D_KEY);
        popup->show();
    }
}

#pragma mark-

CellDevilArtifactForOriginal::CellDevilArtifactForOriginal(E_PLACE ePlace, E_ARTIFACT eArtifact)
{
    _itemBuyBtn = nullptr;
    _itemPointBtn = nullptr;
    _labelBuyPrice = nullptr;
    _onCompleteListener = nullptr;
    _ePlace = ePlace;
    _eArtifact= eArtifact;
    
    _artInfo = DataManager::GetArtifactInfo(_eArtifact);
    int level = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _eArtifact);
    _levelEncrypt = MafAes256::XorEncrypt(level);
    
    std::stringstream str;
    str << "Assets/icon_artifact/artifact_" << _eArtifact << ".png";
    _sprIcon->setTexture(str.str());
    _sprIcon->getTexture()->setTexParameters(texParams);
    _sprIcon->setScale(_itemStartBtn->getContentSize().width / _sprIcon->getContentSize().width);
    
    _itemUpgradeBtn->setCallback(CC_CALLBACK_1(CellDevilArtifactForOriginal::onUpgrade, this));
    _itemStartBtn->setCallback(CC_CALLBACK_1(CellDevilArtifactForOriginal::onCalculator, this));
    
    _sprUpgradeIcon->setTexture("Assets/icon/icon_gold_2.png");
    
    _lbMoney->setTTFConfig(TTFConfig(GAME_FONT, 22, GlyphCollection::DYNAMIC));
    _lbMoney->setColor(Color3B::WHITE);
    _lbMoney->setPosition(Vec2(listSize.width*0.02+_itemStartBtn->getContentSize().width, _listBg->getContentSize().height*0.025));
    _lbMoney->setAnchorPoint(Vec2(0,0));
    _lbMoney->setAlignment(TextHAlignment::LEFT);

    //    _lbLv = Label::createWithTTF("Lv", GAME_FONT, 23);
    _lbLv = MafNode::createBmFont("", GAME_BM_FONT_AL, 22);
    _lbLv->setColor(Color3B::WHITE);
    _lbLv->setAlignment(TextHAlignment::LEFT);
    _lbLv->setAnchorPoint(Vec2(0,0.5f));
    _lbLv->setPosition(Vec2(listSize.width*0.025+_itemStartBtn->getContentSize().width, listSize.height*0.55f));
    _listBg->addChild(_lbLv);
    
    _lbExp = Label::createWithTTF("Exp", GAME_FONT, 22);
    _lbExp->setColor(Color3B::WHITE);
    _lbExp->setAlignment(TextHAlignment::LEFT);
    _lbExp->setAnchorPoint(Vec2(0,0));
    _lbExp->setPosition(Vec2(listSize.width*0.02+_itemStartBtn->getContentSize().width, listSize.height*0.2f));
    _lbExp->setDimensions(_listBg->getContentSize().width - 116 - 15, _lbExp->getContentSize().height);
    _lbExp->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    _lbExp->setOverflow(Label::Overflow::SHRINK);
    _listBg->addChild(_lbExp);
    //Add End
    
    // calculator icon
    if ( _artInfo != nullptr && _artInfo->isCalculator() != 0 )
    {
        auto spriteMagnifier = Sprite::create("Assets/ui/calculator/icon_calculator.png");
        spriteMagnifier->setPosition( Vec2(_itemStartBtn->getContentSize().width - 15, 15) );
        _itemStartBtn->addChild(spriteMagnifier);
    }
    
    if(_artInfo->getPricePoint()!=0)
    {
        _itemPointBtn = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_point.png"), nullptr, CC_CALLBACK_1(CellDevilArtifactForOriginal::onUpgradePoint, this));
        _itemPointBtn->setPosition(Vec2(_itemUpgradeBtn->getPositionX() - _itemUpgradeBtn->getContentSize().width - 5,listSize.height/2));
        _itemPointBtn->setAnchorPoint(Vec2(1,0.5));
        menu->addChild(_itemPointBtn);
        
        auto iconPoint = Sprite::create("Assets/icon/icon_point1.png");
        iconPoint->setPosition(Vec2(_itemPointBtn->getContentSize().width*0.25,
                                    _itemPointBtn->getContentSize().height*0.25));
        iconPoint->setAnchorPoint(Vec2(0.5,0.5));
        _itemPointBtn->addChild(iconPoint);
        
        auto lbUpgradePoint = MafNode::createBmFont(MafUtils::toString(_artInfo->getPricePoint()), GAME_BM_FONT_AL, 28);
        lbUpgradePoint->setColor(COLOR_COIN);
        lbUpgradePoint->setPosition(Vec2(_itemPointBtn->getContentSize().width*0.58,
                                         _itemPointBtn->getContentSize().height*0.25));
        lbUpgradePoint->setAnchorPoint(Vec2(0.5,0.5));
        lbUpgradePoint->setAlignment(TextHAlignment::CENTER);
        _itemPointBtn->addChild(lbUpgradePoint);
        
        auto lbUpgradeTop = MafNode::createBmFont("LV+1", GAME_BM_FONT_AL, 24);
        lbUpgradeTop->setColor(COLOR_UPGRADE_TOP);
        lbUpgradeTop->setPosition(Vec2(_itemPointBtn->getContentSize().width*0.5,
                                       _itemPointBtn->getContentSize().height*0.82));
        lbUpgradeTop->setAnchorPoint(Vec2(0.5,1));
        lbUpgradeTop->setAlignment(TextHAlignment::CENTER);
        _itemPointBtn->addChild(lbUpgradeTop);
        
    }
    
    if(_artInfo->getPricePoint()!=0){
        /*천원.3천원 패키지*/
        _itemBuyBtn = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/artifacts/pack_1.png"), nullptr, nullptr);
        _itemBuyBtn->setCallback(CC_CALLBACK_1(CellDevilArtifactForOriginal::onPurhcase, this));
        _itemBuyBtn->setPosition(Vec2(_itemPointBtn->getPositionX() - _itemPointBtn->getContentSize().width - 5,listSize.height/2));
        _itemBuyBtn->setAnchorPoint(Vec2(1,0.5));
        _itemBuyBtn->setTag(0);
        menu->addChild(_itemBuyBtn);
        {
            _labelBuyPrice = Label::createWithSystemFont("0", SYSTEM_FONT, 24);
            _labelBuyPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            _labelBuyPrice->setPosition(_itemBuyBtn->getContentSize().width / 2, 5);
            _labelBuyPrice->setColor(Color3B::WHITE);
            _labelBuyPrice->setDimensions(_itemBuyBtn->getContentSize().width * 0.95, _labelBuyPrice->getContentSize().height);
            _labelBuyPrice->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            _labelBuyPrice->setOverflow(Label::Overflow::SHRINK);
            _itemBuyBtn->addChild(_labelBuyPrice);
        }
    }
    
    drawUpgrade();
    
    
    if ( level < _artInfo->getLevelMax() )
    {
        scheduleUpdate();
    }
    else
    {
        _itemUpgradeBtn->setVisible(false);
        
        if(_artInfo->getPricePoint()!=0){
            _itemPointBtn->setVisible(false);
        }
        
    }
    //Director::getInstance()->getScheduler()->schedule(schedule_selector(CellDevilArtifactForOriginal::update), this, 1 , kRepeatForever, 1, false);
    //this->scheduleUpdate();
}

CellDevilArtifactForOriginal::~CellDevilArtifactForOriginal(){}

void CellDevilArtifactForOriginal::update(float dt)
{
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    
    _fRedrawTime += dt;
    if (_fRedrawTime >= _fRedrawTimeMax)
    {
        _fRedrawTime = 0;
        
        if ( level >=_artInfo->getLevelMax() )
        {
            _itemUpgradeBtn->setVisible(false);
            if(_artInfo->getPricePoint()!=0)
            {
                _itemPointBtn->setVisible(false);
            }
            unscheduleUpdate();
            
            if(_onCompleteListener!=nullptr){
                _onCompleteListener();
            }
            return;
        }
        
        auto need = DataManager::GetArtifactUpgradeKey(E_PLACE::DG_DEVIL_DOM,_eArtifact, level);
        auto key= ItemsMoneyManager::getInstance()->getDevilKey();
        
        if ( MafUtils::compareStr(key, MafUtils::doubleToString(need)) )
        {
            ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2.png");
        }
        else
        {
            ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
        }
        
        if ( _artInfo->getPricePoint() != 0 )
        {
            int need = _artInfo->getPricePoint();
            int myPoint = ItemsMoneyManager::getInstance()->getPoint();
            if(myPoint >= need)
            {
                ((Sprite*)_itemPointBtn->getNormalImage())->setTexture("Assets/ui_common/btn_point.png");
            }
            else
            {
                ((Sprite*)_itemPointBtn->getNormalImage())->setTexture("Assets/ui_common/btn_point_off.png");
            }
        }
        
    }
    
    if(_artInfo->getPriceType() != E_ITEMS::GEM)
    {
        if ( level != UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _eArtifact))
        {
            level = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _eArtifact);
            _levelEncrypt = MafAes256::XorEncrypt(level);
            
            drawUpgrade();
        }
        
        if (_itemUpgradeBtn->isSelected()){
            _fUpgradeTime += dt;
            if (_fUpgradeTimeMax <= _fUpgradeTime) {
                _bUpgrade = true;
                _fUpgradeTime = 0;
                _fUpgradeTimeMax -= 0.05f;
                
                int count = 0;
                if      (_fUpgradeTimeMax < -2.00f)   count = 8;
                else if (_fUpgradeTimeMax < -1.75f)   count = 7;
                else if (_fUpgradeTimeMax < -1.50f)   count = 6;
                else if (_fUpgradeTimeMax < -1.25f)   count = 5;
                else if (_fUpgradeTimeMax < -1.00f)   count = 4;
                else if (_fUpgradeTimeMax < -0.75f)   count = 3;
                else if (_fUpgradeTimeMax < -0.50f)   count = 2;
                else if (_fUpgradeTimeMax < -0.25f)   count = 1;
                
                std::string consumeKey = "0";
                std::string userKey = ItemsMoneyManager::getInstance()->getDevilKey();
                for ( int i = 0; i < count; i++ )
                {
                    auto needKey = DataManager::GetArtifactUpgradeKey(E_PLACE::DG_DEVIL_DOM,_eArtifact, level);
                    if (MafUtils::compareStr(userKey, MafUtils::doubleToString(needKey)))
                    {
                        level ++;
                        if ( level>=_artInfo->getLevelMax())
                        {
                            level = _artInfo->getLevelMax();
                            _levelEncrypt = MafAes256::XorEncrypt(level);
                            
                            drawUpgrade();
                            break;
                        }
                        
                        userKey = MafUtils::bigSubNum(userKey, MafUtils::doubleToString(needKey));
                        consumeKey = MafUtils::bigAddNum(consumeKey, MafUtils::doubleToString(needKey));
                    }
                }
                UserInfoManager::getInstance()->setArtifactLevel(_ePlace, _eArtifact, level);
                _levelEncrypt = MafAes256::XorEncrypt(level);
                
                consumeKey.insert(0, "-");
                ItemsMoneyManager::getInstance()->setDevilKey(consumeKey);
                
                onUpgrade(nullptr);
            }
        }else{
            _fUpgradeTime = 0;
            _fUpgradeTimeMax = 0.5f;
            if (_bUpgrade){
                _bUpgrade = false;
                
                if(_onCompleteListener!=nullptr){
                    _onCompleteListener();
                }
            }
        }
    }
}

void CellDevilArtifactForOriginal::onUpgradePoint(cocos2d::Ref* pSender)
{
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    if ( level >= _artInfo->getLevelMax() )
        return;
    
    int need = _artInfo->getPricePoint();
    int myPoint = ItemsMoneyManager::getInstance()->getPoint();
    if ( myPoint >= need )
    {
        SoundManager::SoundEffectStart(SOUND_UPGRADE);
        
        level ++;
        if ( level>=_artInfo->getLevelMax())
            level = _artInfo->getLevelMax();
        
        ItemsMoneyManager::getInstance()->setPoint(-need);
        UserInfoManager::getInstance()->setArtifactLevel(_ePlace, _eArtifact, level);
        _levelEncrypt = MafAes256::XorEncrypt(level);
        
        drawUpgrade();
        if (pSender != nullptr)
        {
            if(_onCompleteListener!=nullptr)
                _onCompleteListener();
        }
        
        //업적
        auto achieveMgr = AchievementManager::getInstance();
        auto objAchieve = achieveMgr->getObjAchieveNormal(_eArtifact);
        
        if(objAchieve != nullptr)
            achieveMgr->setAchieveNormalMission(objAchieve->getNormalMission(), UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _eArtifact));
    }
    else
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::POINT);
        popup->show();
    }
}

void CellDevilArtifactForOriginal::onUpgrade(cocos2d::Ref* pSender)
{
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    if ( level >= _artInfo->getLevelMax() )
        return;
    
    auto key = ItemsMoneyManager::getInstance()->getDevilKey();
    auto needKey = DataManager::GetArtifactUpgradeKey(E_PLACE::DG_DEVIL_DOM,_eArtifact, level);
    
    if (MafUtils::compareStr(key, MafUtils::doubleToString(needKey)))
    {
        if (pSender != nullptr) {
            SoundManager::SoundEffectStart(SOUND_UPGRADE);
        }
        
        level ++;
        if (level>=_artInfo->getLevelMax())
            level = _artInfo->getLevelMax();
        
        ItemsMoneyManager::getInstance()->setDevilKey(-needKey);
        UserInfoManager::getInstance()->setArtifactLevel(_ePlace, _eArtifact, level);
        _levelEncrypt = MafAes256::XorEncrypt(level);
        
        drawUpgrade();
        if (pSender != nullptr) {
            if(_onCompleteListener!=nullptr)
                _onCompleteListener();
        }
        
        //업적
        auto achieveMgr = AchievementManager::getInstance();
        auto objAchieve = achieveMgr->getObjAchieveNormal(_eArtifact);
        
        if(objAchieve != nullptr)
            achieveMgr->setAchieveNormalMission(objAchieve->getNormalMission(), UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _eArtifact));
    }
    else
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::D_KEY);
        popup->show();
        
        drawUpgrade();
    }
    
}

void CellDevilArtifactForOriginal::onCalculator(cocos2d::Ref* pSender)
{
    if ( _artInfo == nullptr || _artInfo->isCalculator() == 0 )
    {
        return;
    }
    
    auto popup = PopupCalArtifact::create(_artInfo);
    popup->setCallback(CC_CALLBACK_0(CellDevilArtifactForOriginal::callbackCalculator, this));
    popup->show();
}

void CellDevilArtifactForOriginal::callbackCalculator()
{
    int level = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _eArtifact);
    _levelEncrypt = MafAes256::XorEncrypt(level);
    
    //업적
    auto achieveMgr = AchievementManager::getInstance();
    auto objAchieve = achieveMgr->getObjAchieveNormal(_eArtifact);
    
    if(objAchieve != nullptr)
        achieveMgr->setAchieveNormalMission(objAchieve->getNormalMission(), level);
    
    //
    drawUpgrade();
    
    //
    if( _onCompleteListener != nullptr )
    {
        _onCompleteListener();
    }
}

void CellDevilArtifactForOriginal::drawUpgrade(){
    
    drawBuyBtnVisible();
    
    std::string str;
    _lbTitle->setString(_artInfo->getTitle());
    _lbExp->setString(_artInfo->getDesc());
    
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    int reinforce = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, _eArtifact);
    if ( level >= _artInfo->getLevelMax() )
    {
        _lbLv->setString(StringUtils::format("LV.%d(Max)", level));
    }
    else
    {
        _lbLv->setString(StringUtils::format("LV.%d(Max%d)", level, _artInfo->getLevelMax()));
    }
    
    double effect = DataManager::GetArtifactEffect(_eArtifact, level, reinforce);
    std::string strValue;
    if (effect < 1000)  strValue = MafUtils::format("%.2lf",effect);
    else                strValue = MafUtils::format("%.0lf", effect);
    
    std::string commaStr = MafUtils::addCommaToString(strValue, 3, ',');
    _lbMoney->setString( MafUtils::format("%s%%",commaStr.c_str()));
    
    str.clear();
    if ( level >= _artInfo->getLevelMax() )
    {
        _itemUpgradeBtn->setVisible(false);
        
        if(_artInfo->getPricePoint()!=0){
            _itemPointBtn->setVisible(false);
        }
        
        // 보물 설명 사이즈 재설정
        _lbExp->setDimensions(_listBg->getContentSize().width - 116 - 15, _lbExp->getContentSize().height);
    }
    else
    {
        auto needKey = DataManager::GetArtifactUpgradeKey(E_PLACE::DG_DEVIL_DOM,_eArtifact, level);
        
        str = MafUtils::doubleToString(needKey);
        if(_artInfo->getPriceType() != E_ITEMS::GEM){
            MafUtils::convertNumberToShort(str);
        }
        
        double effectNext = DataManager::GetArtifactEffect(_eArtifact, level+1, reinforce);
        effectNext -= effect;
        
        std::string strDiff;
        if (effectNext >= 1000) {
            strDiff = MafUtils::doubleToString(effectNext);
            strDiff = MafUtils::convertNumberToShort(strDiff);
        }else{
            strDiff = MafUtils::format("%.2f", effectNext);
        }
        
        strDiff.insert(0, "+");
        _lbUpgrade_top->setString(strDiff);
        
        // 보물 설명 사이즈 재설정
        double temp = _listBg->getContentSize().width - 116 - 150 - 15;
        if ( _itemPointBtn != nullptr && _itemPointBtn->isVisible() == true )
        {
            temp -= 110;
        }
        
        if ( _itemBuyBtn != nullptr && _itemBuyBtn->isVisible() == true )
        {
            temp -= 110;
        }
        _lbExp->setDimensions(temp, _lbExp->getContentSize().height);
    }
    
    _lbUpgrade->setString(str.c_str());
    
    
    if ( level < _artInfo->getLevelMax() )
    {
        auto need = DataManager::GetArtifactUpgradeKey(E_PLACE::DG_DEVIL_DOM,_eArtifact, level);
        auto key= ItemsMoneyManager::getInstance()->getKey();
        if (MafUtils::compareStr(key, MafUtils::doubleToString(need)))
            ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2.png");
        else
            ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
    }
}

void CellDevilArtifactForOriginal::drawBuyBtnVisible(){
    
    if(_itemBuyBtn==nullptr)
        return;
    
    if (UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _eArtifact) >= _artInfo->getLevelMax())
    {
        _itemBuyBtn->removeFromParentAndCleanup(true);
        _itemBuyBtn = nullptr;
        return;
    }
    //1. 이미 구매했는지 체크
    //천원짜리,3천원짜리중 무엇을 보여줘야 하는지 체크. 값을 들고 있을것인지?
    std::string strValue = MafUtils::format("pack_%d",_eArtifact);
    int count = UserDefault::getInstance()->getIntegerForKey(strValue.c_str(), 0);
    if(count==2){
        _itemBuyBtn->removeFromParentAndCleanup(true);
        _itemBuyBtn = nullptr;
        return;
    }
    else{
        int nCount = UserDefault::getInstance()->getIntegerForKey(MafUtils::format("pack_%d",_eArtifact).c_str(), 0);
        int nIdx = 0;
        if(_artInfo->getType() == E_ARTIFACT_TYPE::ARTI_TYPE_ATK){
            if(nCount==0)    nIdx = E_CASH::CASH_ARTIFACT_300_1;
            else             nIdx = E_CASH::CASH_ARTIFACT_3000_1;
        }
        else if(_artInfo->getType() == E_ARTIFACT_TYPE::ARTI_TYPE_CRI){
            if(nCount==0)    nIdx = E_CASH::CASH_ARTIFACT_300_2;
            else             nIdx = E_CASH::CASH_ARTIFACT_3000_2;
        }
        else if(_artInfo->getType() == E_ARTIFACT_TYPE::ARTI_TYPE_QUEST){
            if(nCount==0)    nIdx = E_CASH::CASH_ARTIFACT_300_3;
            else             nIdx = E_CASH::CASH_ARTIFACT_3000_3;
        }
        else if(_artInfo->getType() == E_ARTIFACT_TYPE::ARTI_TYPE_KILL){
            if(nCount==0)    nIdx = E_CASH::CASH_ARTIFACT_300_4;
            else             nIdx = E_CASH::CASH_ARTIFACT_3000_4;
        }
        
        std::string name = count == 0 ? "Assets/ui/artifacts/pack_1.png" : "Assets/ui/artifacts/pack_2.png";
        
        ((Sprite*)_itemBuyBtn->getNormalImage())->setTexture(name);
        _itemBuyBtn->setTag(nIdx); //0,1,2
        
        auto objProduct = CashManager::getInstance()->getProduct(nIdx);
        if ( objProduct != nullptr )
        {
            _labelBuyPrice->setString(objProduct->getPrice());
            
            std::string strPrice = objProduct->getPrice();
            if ( strPrice.length() == 0 )
            {
                _labelBuyPrice->setString(GAME_TEXT("t_ui_shop_price_loading"));
            }
        }
    }
    
}

void CellDevilArtifactForOriginal::onPurhcase(cocos2d::Ref* pSender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto item = (MafNode::MafMenuItemSprite*)pSender;
    
    /*
    0 : 오류 없음
    1 : 다른상품 결제 오류 있음.
    2 : 같은상품 결제 오류 있음.
    */
    int nItemIdx = item->getTag();
    CashManager::getInstance()->purchaseHas(nItemIdx, [=](int nHas){
        if ( nHas == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                PopupLoading::show();
                ItemsManager::getInstance()->setParameterArtifact((int)_eArtifact);
                CashManager::getInstance()->purchaseInitiate(nItemIdx, CC_CALLBACK_4(CellDevilArtifactForOriginal::ResultInAppPurchasePackage, this));
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
            PopupLoading::show();
            ItemsManager::getInstance()->setParameterArtifact((int)_eArtifact);
            CashManager::getInstance()->purchaseInitiate(nItemIdx, CC_CALLBACK_4(CellDevilArtifactForOriginal::ResultInAppPurchasePackage, this));
        }
    });
}

void CellDevilArtifactForOriginal::ResultInAppPurchasePackage(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    PopupLoading::hide();
    
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        //
        int nCount = UserDefault::getInstance()->getIntegerForKey(MafUtils::format("pack_%d",_eArtifact).c_str(), 0);
        UserDefault::getInstance()->setIntegerForKey(MafUtils::format("pack_%d", (int)_eArtifact).c_str(), nCount + 1);
        
        // redraw
        _levelEncrypt = MafAes256::XorEncrypt( UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _eArtifact) );
        drawUpgrade();
        
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
}

#pragma mark-

CellDevilPet::CellDevilPet(E_PLACE ePlace, E_PET ePet)
{
    
    _onCompleteListener = nullptr;
    _ePlace = ePlace;
    _ePet = ePet;
    
    _grade = PetNewDevilManager::getInstance()->getPetGrade(_ePet);
    
    int level = PetNewDevilManager::getInstance()->getPetLevel(_ePet);
    _levelEncrypt = MafAes256::XorEncrypt(level);
    
    if ( PetNewDevilManager::getInstance()->getPet(_ePet) )
    {
        DevilDomScene::getInstance()->createPet(_ePet);
    }
    
    
    _sprIcon->setTexture(PetNewDevilManager::getInstance()->getPetImagePath(_ePet));
    _sprIcon->getTexture()->setTexParameters(texParams);
    _sprUpgradeIcon->setTexture("Assets/icon/icon_jewel.png");
    
    _lbTitle->setTTFConfig(TTFConfig(GAME_FONT, 26, GlyphCollection::DYNAMIC));
    
    _lbMoney->setTTFConfig(TTFConfig(GAME_FONT, 22, GlyphCollection::DYNAMIC));
    _lbMoney->setColor(Color3B::WHITE);
    _lbMoney->setPosition(Vec2(listSize.width*0.02+_itemStartBtn->getContentSize().width,
                               _listBg->getContentSize().height*0.025));
    _lbMoney->setAnchorPoint(Vec2(0,0));
    _lbMoney->setAlignment(TextHAlignment::LEFT);
    
    //    _lbLv = Label::createWithTTF("Lv", GAME_FONT, 23);
    _lbLv = MafNode::createBmFont("", GAME_BM_FONT_AL, 22);
    _lbLv->setColor(Color3B::WHITE);
    _lbLv->setAlignment(TextHAlignment::LEFT);
    _lbLv->setAnchorPoint(Vec2(0,0.5f));
    _lbLv->setPosition(Vec2(listSize.width*0.025+_itemStartBtn->getContentSize().width, listSize.height*0.55f));
    _listBg->addChild(_lbLv);
    
    _lbExp = Label::createWithTTF("Exp", GAME_FONT, 22);
    _lbExp->setColor(Color3B::WHITE);
    _lbExp->setAnchorPoint(Vec2(0,0));
    _lbExp->setPosition(Vec2(listSize.width*0.02+_itemStartBtn->getContentSize().width, listSize.height*0.2f));
    _lbExp->setDimensions(_listBg->getContentSize().width - 116 - 150 - 15, _lbExp->getContentSize().height);
    _lbExp->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    _lbExp->setOverflow(Label::Overflow::SHRINK);
    _listBg->addChild(_lbExp);
    
    _lbUpgrade_top->removeFromParentAndCleanup(true);
    _lbUpgrade_top = Label::createWithTTF("", GAME_FONT, 22);
    _lbUpgrade_top->setLineSpacing(-5);
    _lbUpgrade_top->setColor(COLOR_UPGRADE_TOP);
    _itemUpgradeBtn->addChild(_lbUpgrade_top);
    
    _itemUpgradeBtn->setCallback(CC_CALLBACK_1(CellDevilPet::onAskPopup, this));
    _itemStartBtn->setCallback(CC_CALLBACK_1(CellDevilPet::onDetailView, this));
    
    auto spriteMagnifier = Sprite::create("Assets/ui/review/icon_magnifier.png");
    spriteMagnifier->setPosition( Vec2(_itemStartBtn->getContentSize().width - 15, 15) );
    _itemStartBtn->addChild(spriteMagnifier);
    
    Director::getInstance()->getScheduler()->schedule(schedule_selector(CellDevilPet::update),
                                                      this, 0.5 , kRepeatForever, 0, false);
    
    drawUpgrade();
    
}

CellDevilPet::~CellDevilPet(){}


void CellDevilPet::update(float dt){
    
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    int realLevel = PetNewDevilManager::getInstance()->getPetLevel(_ePet);
    if ( realLevel != level )
    {
        level = realLevel;
        _levelEncrypt = MafAes256::XorEncrypt(level);
        
        drawUpgrade();
    }
    
    if ( level == 0 )
    {
        _itemUpgradeBtn->setVisible(false);
        return;
    }
    else if ( level >= PetNewDevilManager::getInstance()->getPetLevelMax(_ePet, _grade) )
    {
        _itemUpgradeBtn->setVisible(false);
        //unscheduleUpdate();
        if(_grade >= C_MAX_PET_GRADE)
            Director::getInstance()->getScheduler()->unschedule(schedule_selector(CellDevilPet::update), this);
        return;
    }
    _itemUpgradeBtn->setVisible(true);
    
    auto need = PetNewDevilManager::getInstance()->getPetUpgradePrice(_ePet, level, _grade);
    auto gem= ItemsMoneyManager::getInstance()->getGem();
    
    if(gem >= need)
    {
        ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2.png");
    }
    else
    {
        ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
    }
}

void CellDevilPet::onDetailView(cocos2d::Ref* pSender)
{
    CellDevilPet *celldevil = this;
    
    auto popup = PopupPetDevil::create(*celldevil);
    DevilDomScene::getInstance()->_popupPet = popup;
    popup->show();
}

void CellDevilPet::onAskPopup(cocos2d::Ref* pSender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    E_PET ePet = _ePet;
    
    int level = PetNewDevilManager::getInstance()->getPetLevel(ePet);
    int levelMax = PetNewDevilManager::getInstance()->getPetLevelMax(ePet, _grade);
    bool bLevelMax = level >= levelMax ? true : false;
    
    if ( bLevelMax == true )
    {
        return;
    }
    
    auto itemNow = ItemsMoneyManager::getInstance()->getGem();
    auto itemNeed = PetNewDevilManager::getInstance()->getPetUpgradePrice(ePet, level, _grade);
    if ( itemNow >= itemNeed )
    {
        std::string strTitle = PetNewDevilManager::getInstance()->getPetName(ePet);
        std::string strMessage = GAME_TEXT("t_ui_msg_buy");
        strMessage.append(MafUtils::format("\n(Lv.%d -> Lv.%d)", level, level + 1));
        
        auto popup = PopupDefault::create(PetNewDevilManager::getInstance()->getPetName(_ePet), GAME_TEXT("t_ui_msg_buy"));
        popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
        popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
            
            int result = PetNewDevilManager::getInstance()->onPetLevelUP(ePet);
            if ( result != 0 )
            {
                return;
            }
            
            // auto refresh
            int level = PetNewDevilManager::getInstance()->getPetLevel(ePet);
            _levelEncrypt = MafAes256::XorEncrypt(level);
            
            drawUpgrade();
        });
        popup->show();
    }
    else
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
        popup->show();
    }
}

void CellDevilPet::drawUpgrade()
{
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    
    _lbTitle->setString(PetNewDevilManager::getInstance()->getPetName(_ePet));
    _lbLv->setString(MafUtils::format("LV.%d", level));
    
    _lbExp->setString(PetNewDevilManager::getInstance()->getPetExplain(_ePet).c_str());
    
    char charTitle[100] = {0,};
    memset(charTitle, 0, sizeof(charTitle));
    double per =0;

    if ( level < 1 )
    {
        level = 0;
        _levelEncrypt = MafAes256::XorEncrypt(level);
    }
    per = PetNewDevilManager::getInstance()->getPetEffect(_ePet, level);
    sprintf(charTitle, PetNewDevilManager::getInstance()->getPetUpgradeExplain(_ePet).c_str(),per,nullptr);
    std::string str = charTitle;
    _lbMoney->setString(str.c_str());
    
    if ( level >= PetNewDevilManager::getInstance()->getPetLevelMax(_ePet,_grade) )
    {
        _itemUpgradeBtn->setVisible(false);
        
        // 펫 설명 사이즈 재설정
        _lbExp->setDimensions(_listBg->getContentSize().width - 116 - 15, _lbExp->getContentSize().height);
        
        return;
    }
    
    // 페 설명 사이즈 재설정
    _lbExp->setDimensions(_listBg->getContentSize().width - 116 - 150 - 15, _lbExp->getContentSize().height);
    
    //_lbCondition->setString(DataManager::GetPetCondition(_ePet));
    _sprUpgradeIcon->setVisible(true);
    if(_ePet==E_PET::D_1_PENGUIN)
        _lbUpgrade_top->setString("+50%");
    else if(_ePet==E_PET::D_2_HEDGEHOG)
        _lbUpgrade_top->setString("+0.1%");
    else if(_ePet==E_PET::D_3_GATEKEEPER)
        _lbUpgrade_top->setString("+0.2초");
    else if(_ePet==E_PET::D_4_FLASH) {
        _lbUpgrade_top->setString("+1%");
        if ( level >= 50 )
            _lbUpgrade_top->setString("+0.4%");
    }
    
    
    _lbUpgrade_top->setPosition(Vec2(_itemUpgradeBtn->getContentSize().width*0.5,
                                     _itemUpgradeBtn->getContentSize().height*0.8));
    _lbUpgrade_top->setAnchorPoint(Vec2(0.5,1));
    _lbUpgrade_top->setAlignment(TextHAlignment::CENTER);
    _lbUpgrade->setVisible(true);
    _lbUpgrade->setString(MafUtils::toString(PetNewDevilManager::getInstance()->getPetUpgradePrice(_ePet, level, _grade)));
    
}






