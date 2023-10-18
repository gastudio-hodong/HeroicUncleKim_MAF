//
//  ModelCell.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 8. 7..
//
//

#include "ModelCell.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoPetEquip.h"
#include "GameObject/InfoItems.h"
#include "GameObject/InfoArtifact.h"

#include "GameScene/HelloWorldScene.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/BattlePrison/PopupPrison.h"
#include "GameUIPopup/Other/BattleFrost/PopupFrost.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"
#include "GameUIPopup/Other/Pet/PopupPet.h"
#include "GameUIPopup/Other/ArtifactCalculator/PopupCalArtifact.h"
#include "GameUIPopup/Other/ArtifactCalculator/PopupCalArtifactReinforce.h"
#include "GameUIPopup/Other/Equip/PopupWeaponCraft.h"
#include "GameUIPopup/Other/Ads/PopupAdsWatch.h"
#include "GameUIPopup/Other/Equip/PopupWeaponPermanent.h"
#include "GameUIPopup/Other/BonsikDex/PopupBonsikDexDefault.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidDefault.h"
#include "GameUIPopup/Other/WeaponSpirit/PopupListWeaponSpirit.h"
#include "GameUIPopup/Other/Adventure/Guild/PopupAdventureGuild.h"
#include "GameUIPopup/Other/ArtifactInfinite/PopupUpgradeInfiniteArtifact.h"
#include "GameUIPopup/Other/ArtifactCalculator/PopupCalInfiniteArtifact.h"
#include "GameUIPopup/Other/Flashman/PopupFlashman.h"
#include "GameUIPopup/Other/Defense/PopupDefenseDefault.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/StoryManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/PrisonManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/AchievementManager.h"
#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/PrisonFrostManager.h"
#include "ManagerGame/WeaponManager.h"
#include "ManagerGame/MineralManager.h"
#include "ManagerGame/FlashmanManager.h"
#include "ManagerGame/InfiniteArtifactManager.h"
#include "ManagerGame/DefenseManager.h"
#include "ManagerGame/DefenseManaAMPManager.h"

USING_NS_CC;
using namespace ModelCell;

UpgradeBtn::UpgradeBtn(){
    
    _fSecondTime = 0;
    _bShowSecondBtn = false;
    _bMakeSecondBtn = false;
}
UpgradeBtn::~UpgradeBtn(){}

void UpgradeBtn::plusTime(){
    _fSecondTime +=1;
    if(_fSecondTime>=5)
        _fSecondTime = 5;
    
}

bool UpgradeBtn::checkTime(float dt){
    
    if(!_bMakeSecondBtn)
        return false;
    
    if(_fSecondTime>0)
        _fSecondTime -= dt;
    else
        _fSecondTime = 0;
    
    if(_bShowSecondBtn){
        if(_fSecondTime<=0){
            _bShowSecondBtn = false;
            showSecondUpgradeBtn(false);
        }
    }
    else{
        if(_fSecondTime>=2.5){
            _bShowSecondBtn = true;
            showSecondUpgradeBtn(true);
            return true;
        }
    }
    
    return false;
}

void UpgradeBtn::showSecondUpgradeBtn(bool val){
    _itemBtn[1]->setVisible(val);
    _itemBtn[2]->setVisible(val);
    _bShowSecondBtn = val;
    
    if ( val == false )
        _fSecondTime = 0;
}

void UpgradeBtn::createQuestStyle(Size listSize,Node *parent){
    //최대한 소스수정없이 하기 위해서 참조하게 변경
    MafNode::MafMenuItemSprite *_itemUpgradeBtn;
    Label *_lbUpgrade;
    Label *_lbUpgrade_top;
    Sprite *_sprUpgradeIcon;
    
    
//    _itemUpgradeBtn = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_gift_bg.png"),
//                                                Sprite::create("Assets/ui_common/btn_gift_bg.png"),
//                                                Sprite::create("Assets/ui_common/btn_gift_bg_max.png"),nullptr);
    _itemUpgradeBtn = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_upgrade_bg_2.png"),
                                                Sprite::create("Assets/ui_common/btn_upgrade_bg_2.png"),
                                                Sprite::create("Assets/ui_common/btn_upgrade_bg_2_max.png"),nullptr);
    _itemUpgradeBtn->setPosition(Vec2(listSize.width,listSize.height/2));
    _itemUpgradeBtn->setAnchorPoint(Vec2(1,0.5));
    parent->addChild(_itemUpgradeBtn);
    
    if(UserInfoManager::getInstance()->getCurrentPlace()==E_PLACE::DG_DEVIL_DOM){
        _sprUpgradeIcon = Sprite::create("Assets/icon/icon_gold_1.png");
    }
    else{
        _sprUpgradeIcon = Sprite::create("Assets/icon/menu_coin.png");
    }
    
    _sprUpgradeIcon->setPosition(Vec2(_itemUpgradeBtn->getContentSize().width*0.175,
                                      _itemUpgradeBtn->getContentSize().height*0.25));
    _itemUpgradeBtn->addChild(_sprUpgradeIcon);
    
    _lbUpgrade = MafNode::createBmFont("", GAME_BM_FONT_AL, 30);
    _lbUpgrade->setColor(COLOR_COIN);
    _lbUpgrade->setPosition(Vec2(_itemUpgradeBtn->getContentSize().width*0.350,
                                 _itemUpgradeBtn->getContentSize().height*0.25));
    _lbUpgrade->setAnchorPoint(Vec2(0,0.50));
    _lbUpgrade->setAlignment(TextHAlignment::LEFT);
    _lbUpgrade->setOverflow(Label::Overflow::SHRINK);
    _lbUpgrade->setDimensions(85, 25);
    _itemUpgradeBtn->addChild(_lbUpgrade);
    
    _lbUpgrade_top = MafNode::createBmFont("", GAME_BM_FONT_AL, 24);
    _lbUpgrade_top->setColor(COLOR_UPGRADE_TOP);
    _lbUpgrade_top->setPosition(Vec2(_itemUpgradeBtn->getContentSize().width*0.5,
                                     _itemUpgradeBtn->getContentSize().height*0.8));
    _lbUpgrade_top->setAnchorPoint(Vec2(0.5,1));
    _lbUpgrade_top->setAlignment(TextHAlignment::CENTER);
    _itemUpgradeBtn->addChild(_lbUpgrade_top);
    
    _itemBtn[0] = _itemUpgradeBtn;
    _lbMoney[0] = _lbUpgrade;
    _lbTop[0] = _lbUpgrade_top;
    _sprIcon[0] = _sprUpgradeIcon;
    _lbUp[0] = _lbUpgrade;
}

void UpgradeBtn::createQuestAddBtn(Size listSize,Node *parent){
    
    _bMakeSecondBtn = true;
    MafNode::MafMenuItemSprite *_itemUpgradeBtn;
    Label *_lbUpgrade;
    Label *_lbUpgrade_top;
    Sprite *_sprUpgradeIcon;
    
    int order = 2;
    _itemUpgradeBtn = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_upgrade_bg_2.png"),
                                                Sprite::create("Assets/ui_common/btn_upgrade_bg_2.png"),
                                                Sprite::create("Assets/ui_common/btn_upgrade_bg_2_max.png"),nullptr);
    _itemUpgradeBtn->setPosition(Vec2(_itemBtn[0]->getPosition().x - _itemBtn[0]->getContentSize().width - 10,
                                      listSize.height/2));
    _itemUpgradeBtn->setAnchorPoint(Vec2(1,0.5));
    parent->addChild(_itemUpgradeBtn,order);
    
    if(UserInfoManager::getInstance()->getCurrentPlace()==E_PLACE::DG_DEVIL_DOM){
        _sprUpgradeIcon = Sprite::create("Assets/icon/icon_gold_1.png");
    }
    else{
        _sprUpgradeIcon = Sprite::create("Assets/icon/menu_coin.png");
    }
    _sprUpgradeIcon->setPosition(Vec2(_itemUpgradeBtn->getContentSize().width*0.175, _itemUpgradeBtn->getContentSize().height*0.25));
    _itemUpgradeBtn->addChild(_sprUpgradeIcon);
    
    _lbUpgrade = MafNode::createBmFont("", GAME_BM_FONT_AL, 30);
    _lbUpgrade->setColor(COLOR_COIN);
    _lbUpgrade->setPosition(Vec2(_itemUpgradeBtn->getContentSize().width*0.350,
                                 _itemUpgradeBtn->getContentSize().height*0.25));
    _lbUpgrade->setAnchorPoint(Vec2(0,0.50));
    _lbUpgrade->setAlignment(TextHAlignment::LEFT);
    _lbUpgrade->setOverflow(Label::Overflow::SHRINK);
    _lbUpgrade->setDimensions(85, 25);
    _itemUpgradeBtn->addChild(_lbUpgrade);
    
    _lbUpgrade_top = MafNode::createBmFont("LV+10", GAME_BM_FONT_AL, 24);
    _lbUpgrade_top->setColor(COLOR_UPGRADE_TOP);
    _lbUpgrade_top->setPosition(Vec2(_itemUpgradeBtn->getContentSize().width*0.5,
                                     _itemUpgradeBtn->getContentSize().height*0.8));
    _lbUpgrade_top->setAnchorPoint(Vec2(0.5,1));
    _lbUpgrade_top->setAlignment(TextHAlignment::CENTER);
    _itemUpgradeBtn->addChild(_lbUpgrade_top);
    
    _itemBtn[1] = _itemUpgradeBtn;
    _lbMoney[1] = _lbUpgrade;
    _lbTop[1] = _lbUpgrade_top;
    _sprIcon[1] = _sprUpgradeIcon;
    _lbUp[1] = _lbUpgrade;
    
    _itemUpgradeBtn = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_upgrade_bg_2.png"),
                                                Sprite::create("Assets/ui_common/btn_upgrade_bg_2.png"),
                                                Sprite::create("Assets/ui_common/btn_upgrade_bg_2_max.png"),nullptr);
    _itemUpgradeBtn->setPosition(Vec2(_itemBtn[1]->getPosition().x - _itemBtn[1]->getContentSize().width - 10,
                                      listSize.height/2));
    _itemUpgradeBtn->setAnchorPoint(Vec2(1,0.5));
    parent->addChild(_itemUpgradeBtn,order);
    
    if(UserInfoManager::getInstance()->getCurrentPlace()==E_PLACE::DG_DEVIL_DOM){
        _sprUpgradeIcon = Sprite::create("Assets/icon/icon_gold_1.png");
    }
    else{
        _sprUpgradeIcon = Sprite::create("Assets/icon/menu_coin.png");
    }
    _sprUpgradeIcon->setPosition(Vec2(_itemUpgradeBtn->getContentSize().width*0.175, _itemUpgradeBtn->getContentSize().height*0.25));
    _itemUpgradeBtn->addChild(_sprUpgradeIcon);
    
    _lbUpgrade = MafNode::createBmFont("", GAME_BM_FONT_AL, 30);
    _lbUpgrade->setColor(COLOR_COIN);
    _lbUpgrade->setPosition(Vec2(_itemUpgradeBtn->getContentSize().width*0.350,
                                 _itemUpgradeBtn->getContentSize().height*0.25));
    _lbUpgrade->setAnchorPoint(Vec2(0,0.50));
    _lbUpgrade->setAlignment(TextHAlignment::LEFT);
    _lbUpgrade->setOverflow(Label::Overflow::SHRINK);
    _lbUpgrade->setDimensions(85, 25);
    _itemUpgradeBtn->addChild(_lbUpgrade);
    
    _lbUpgrade_top = MafNode::createBmFont("LV+100", GAME_BM_FONT_AL, 24);
    _lbUpgrade_top->setColor(COLOR_UPGRADE_TOP);
    _lbUpgrade_top->setPosition(Vec2(_itemUpgradeBtn->getContentSize().width*0.5,
                                     _itemUpgradeBtn->getContentSize().height*0.8));
    _lbUpgrade_top->setAnchorPoint(Vec2(0.5,1));
    _lbUpgrade_top->setAlignment(TextHAlignment::CENTER);
    _itemUpgradeBtn->addChild(_lbUpgrade_top);

    
    _itemBtn[2] = _itemUpgradeBtn;
    _lbMoney[2] = _lbUpgrade;
    _lbTop[2] = _lbUpgrade_top;
    _sprIcon[2] = _sprUpgradeIcon;
    _lbUp[2] = _lbUpgrade;
//    _bShowSecondBtn = true;
    
}

void UpgradeBtn::setUpgradeLabelPositionCenter(){
//    if(_bMakeSecondBtn){
//    }
    _lbUp[0]->setPositionY(_itemBtn[0]->getContentSize().height*0.5);
    _sprIcon[0]->setPositionY(_itemBtn[0]->getContentSize().height*0.5);
}

#pragma mark-

Cell::Cell(){
    
    _eState = E_CHANGE_STATE::ALL;
    
    _bUpgrade = false;
    _fUpgradeTimeMax = 0.2f;
    _fUpgradeTime = 0;
    _fRedrawTimeMax = 0.2f;
    _fRedrawTime = 0;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    setContentSize(Size(visibleSize.width,C_BAR_SIZE));

    
    _listBg = ui::Scale9Sprite::create("Assets/ui_common/list_bg_1.png", Rect::ZERO, Rect(0,51,150,10));
    _listBg->setContentSize(Size(visibleSize.width*0.98, C_BAR_SIZE*0.95));
    _listBg->setPosition(Vec2(getContentSize().width/2,getContentSize().height/2));
    addChild(_listBg,2);
    
    listSize = _listBg->getContentSize();
    
    menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _listBg->addChild(menu,1);
    
    
    _itemStartBtn = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/box_quest.png"),Sprite::create("Assets/ui_common/box_quest.png"),nullptr);
    _itemStartBtn->setPosition(Vec2(3,listSize.height/2));
    _itemStartBtn->setAnchorPoint(Vec2(0,0.5));
    menu->addChild(_itemStartBtn);
    
    _sprIcon = Sprite::create();
    _sprIcon->setPosition(Vec2(_itemStartBtn->getContentSize().width/2,
                              _itemStartBtn->getContentSize().height/2));
    _itemStartBtn->addChild(_sprIcon,1);
    
    float leftStartPos = _itemStartBtn->getContentSize().width +10;
    
    auto fontsize = 27;
    if(TextManager::getInstance()->getLang().compare("ru") == 0 || TextManager::getInstance()->getLang().compare("pt") == 0 ||
        TextManager::getInstance()->getLang().compare("es") == 0)
    {
        fontsize = 20;
    }

    _lbTitle = Label::createWithTTF("", GAME_FONT, fontsize);
    _lbTitle->setColor(COLOR_COIN);
    _lbTitle->setPosition(Vec2(leftStartPos, _listBg->getContentSize().height*0.98));
    _lbTitle->setAnchorPoint(Vec2(0,1));
    _lbTitle->setAlignment(TextHAlignment::LEFT);
    _listBg->addChild(_lbTitle);
    
    _lbLv = MafNode::createBmFont("", GAME_BM_FONT_AL, 22);
    _lbLv->setColor(Color3B::WHITE);
    _lbLv->setAlignment(TextHAlignment::LEFT);
    _lbLv->setAnchorPoint(Vec2(0,1));
    _lbLv->setPosition(Vec2(leftStartPos+3, listSize.height*0.70));
    _listBg->addChild(_lbLv);
    _lbLv->setVisible(false);
    
    _lbMoney = MafNode::createBmFont("", GAME_BM_FONT_AL, 28);
    _lbMoney->setColor(Color3B::WHITE);
    _lbMoney->setPosition(Vec2(leftStartPos,_listBg->getContentSize().height*0.1));
    _lbMoney->setAnchorPoint(Vec2(0,0));
    _lbMoney->setAlignment(TextHAlignment::LEFT);
    _listBg->addChild(_lbMoney);
    
    _btnSet = new UpgradeBtn();
    _btnSet->createQuestStyle(listSize, menu);
    _itemUpgradeBtn = _btnSet->_itemBtn[0];
    _lbUpgrade = _btnSet->_lbMoney[0];
    _lbUpgrade_top = _btnSet->_lbTop[0];
    _sprUpgradeIcon = _btnSet->_sprIcon[0];

}

Cell::~Cell(){
    if ( _btnSet != nullptr )
        _btnSet->release();
}

void Cell::setState(E_CHANGE_STATE state){
    
    _eState = state;
    checkState();
}

void Cell::setChangeStateListener(std::function<void()> listener){
    _onChangeStateListener = listener;
    checkState();
//    auto _pScheduler = Director::getInstance()->getScheduler();
//    _pScheduler->schedule(schedule_selector(Cell::checkState), this, 0.5 , kRepeatForever, 0, false);
}

void Cell::checkState(){
    
    if(_eState == E_CHANGE_STATE::NONE)
        return;
    if(_onChangeStateListener!=nullptr)
        _onChangeStateListener();
    
    _eState = E_CHANGE_STATE::NONE;
}



#pragma mark-
CellArtifactTop::CellArtifactTop(E_PLACE ePlace) :
_ePlace(ePlace),

_nType(true),
_sprLock(nullptr),
_onCallbackTab(nullptr),
_onCallbackShowRelic(nullptr)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    setContentSize(Size(visibleSize.width, 68));
    
    //
    _nType = 0;
    
    
    //
    _menu = MafNode::MafMenu::create();
    _menu->setPosition(Vec2::ZERO);
    addChild(_menu);
    _menu->setTag(1000);
    
    
    int nTag[] = {0, 1, 100, 1000};
    std::string strTextKey[] = {"t_ui_artifact_tab_2", "t_ui_artifact_tab_3", "t_ui_artifact_tab_1", "t_ui_relic"};
    
    int posX = getContentSize().width - 5;
    for ( int i = 0; i < sizeof(nTag)/sizeof(int); i++ )
    {
        auto spriteOn = ui::Scale9Sprite::create("Assets/ui_common/btn_tap_on.png", Rect::ZERO, Rect(26.f,25,2,10));
        spriteOn->setContentSize(Size(141, 64));
        
        auto spriteTouch = ui::Scale9Sprite::create("Assets/ui_common/btn_tap_off.png", Rect::ZERO, Rect(26.f,25,2,10));
        spriteTouch->setContentSize(Size(141, 64));
        
        auto spriteOff = ui::Scale9Sprite::create("Assets/ui_common/btn_tap_off.png", Rect::ZERO, Rect(26.f,25,2,10));
        spriteOff->setContentSize(Size(141, 64));
        
        auto item = MafNode::MafMenuItemSprite::create(spriteOff, spriteTouch, spriteOn, CC_CALLBACK_1(CellArtifactTop::onClickTab, this));
        item->setPosition(posX - item->getContentSize().width / 2, item->getContentSize().height / 2);
        item->setTag(nTag[i]);
        _menu->addChild(item);
        if ( i == 0 )
        {
            item->setEnabled(false);
        }
        
        
        auto label = Label::createWithTTF(GAME_TEXT(strTextKey[i]), GAME_FONT, 24);
        label->setPosition(item->getContentSize().width / 2, item->getContentSize().height / 2);
        label->setDimensions(item->getContentSize().width * 0.9, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        label->setTag(10000);
        item->addChild(label);
        
        posX -= item->getContentSize().width;
        
        if(nTag[i] == 100)
        {
            if ( UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_1_EVOL1) < 99999 &&
                 UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_3_EVOL1) < 99999 &&
                 UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_4_EVOL1) < 99999 &&
                 UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_8_EVOL1) < 99999 )
            {
                _sprLock = Sprite::create("Assets/ui_common/tap_lock.png");
                _sprLock->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                _sprLock->setPosition(label->getPositionX()-52,label->getPositionY());
                item->addChild(_sprLock);
            }
            
        }
    }
    
    //초월 보물 탭
    auto infiniteMgr = InfiniteArtifactManager::getInstance();
    if(infiniteMgr->isActive())
    {
        auto tabNomal = (Node*)_menu->getChildByTag(1);
        
        auto spriteOn = ui::Scale9Sprite::create("Assets/ui_common/btn_tap_on.png", Rect::ZERO, Rect(26.f,25,2,10));
        spriteOn->setContentSize(Size(141, 64));
        
        auto spriteSelect = ui::Scale9Sprite::create("Assets/ui_common/btn_tap_off.png", Rect::ZERO, Rect(26.f,25,2,10));
        spriteSelect->setContentSize(Size(141, 64));
        
        auto spriteOff = ui::Scale9Sprite::create("Assets/ui_common/btn_tap_off.png", Rect::ZERO, Rect(26.f,25,2,10));
        spriteOff->setContentSize(Size(141, 64));
        
        auto item = MafNode::MafMenuItemSprite::create(spriteOff, spriteSelect, spriteOn, CC_CALLBACK_1(CellArtifactTop::onClickTab, this));
        item->setTag(9000);
        _menu->addChild(item);
        
        item->setPosition(tabNomal->getPosition());
        tabNomal->setPosition(Vec2(item->getContentSize().width / 2, item->getContentSize().height / 2));
        
        
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_menu_arti_ascend_btn"), GAME_FONT, 24);
        label->setPosition(item->getContentSize().width / 2, item->getContentSize().height / 2);
        label->setDimensions(item->getContentSize().width * 0.9, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        label->setTag(10000);
        item->addChild(label);
    }
}

CellArtifactTop::~CellArtifactTop()
{
    
}

void CellArtifactTop::drawInfo()
{
}

void CellArtifactTop::reDrawCell()
{
    auto infiniteMgr = InfiniteArtifactManager::getInstance();
    if(infiniteMgr->isActive())
    {
        auto tabNomal = (Node*)_menu->getChildByTag(1);
        
        auto spriteOn = ui::Scale9Sprite::create("Assets/ui_common/btn_tap_on.png", Rect::ZERO, Rect(26.f,25,2,10));
        spriteOn->setContentSize(Size(141, 64));
        
        auto spriteOff = ui::Scale9Sprite::create("Assets/ui_common/btn_tap_off.png", Rect::ZERO, Rect(26.f,25,2,10));
        spriteOff->setContentSize(Size(141, 64));
        
        auto item = MafNode::MafMenuItemSprite::create(spriteOff, nullptr, spriteOn, CC_CALLBACK_1(CellArtifactTop::onClickTab, this));
        item->setTag(9000);
        _menu->addChild(item);
        
        item->setPosition(tabNomal->getPosition());
        tabNomal->setPosition(Vec2(item->getContentSize().width / 2, item->getContentSize().height / 2));
        
        
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_menu_arti_ascend_btn"), GAME_FONT, 24);
        label->setPosition(item->getContentSize().width / 2, item->getContentSize().height / 2);
        label->setDimensions(item->getContentSize().width * 0.9, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        label->setTag(10000);
        item->addChild(label);
        onClickTab(item);
    }
}

void CellArtifactTop::setCallbackShowRelic(const std::function<void(int)>& callback)
{
    _onCallbackShowRelic = callback;
}

void CellArtifactTop::setCallbackTab(const std::function<void(int)>& callback)
{
    _onCallbackTab = callback;
}

int CellArtifactTop::getTabType()
{
    return _nType;
}

void CellArtifactTop::onClickTab(cocos2d::Ref* sender)
{
    auto item = (MenuItem*)sender;
    
    if ( _nType == item->getTag() )
    {
        return;
    }
    
    if ( item->getTag() == 100 )
    {
        if ( UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_1_EVOL1) < 99999 &&
             UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_3_EVOL1) < 99999 &&
             UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_4_EVOL1) < 99999 &&
             UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_8_EVOL1) < 99999 )
        {
            
            Vec2 posToast = item->getParent()->convertToWorldSpace(item->getPosition());
            posToast.y += item->getContentSize().height / 2;
            posToast.y += 10;
            posToast.x = getContentSize().width/2;
                
            PopupToast::showImmediately(GAME_TEXT("t_ui_arti_notice"), posToast.x, posToast.y);
            
            return;
        }
        else
        {
            if(_sprLock != nullptr)
                _sprLock->setVisible(false);
        }
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
    if ( item->getTag() == 1000)
    {
        _onCallbackShowRelic(HelloWorldScene::E_GAME_STATE::RELIC);
        return;
    }
    else if (item->getTag() == 9000)
    {
        _onCallbackShowRelic(HelloWorldScene::E_GAME_STATE::ARTIFACT_INFINITE);
        return;
    }
    else
    {
        _onCallbackShowRelic(HelloWorldScene::E_GAME_STATE::ARTIFACT);
    }

    if ( _onCallbackTab != nullptr )
    {
        _onCallbackTab(_nType);
    }
}

#pragma mark-
CellArtifact::CellArtifact(E_PLACE ePlace, E_ARTIFACT eArtifact)
{
    _labelUpgradeCashPrice = nullptr;
    _labelReinforce = nullptr;
    
    _itemUpgradeCashBtn = nullptr;
    _itemUpgradePointBtn = nullptr;
    
    _itemUpgradeExpandBtn = nullptr;
    
    _onCompleteListener = nullptr;
    _onCompleteReinforceListener = nullptr;
    
    _cellArtifactTop = nullptr;
    
    _ePlace = ePlace;
    _eArtifact= eArtifact;
    _nTab = 0;
    
    _artInfo = DataManager::GetArtifactInfo(_eArtifact);
    _levelEncrypt = MafAes256::XorEncrypt(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _eArtifact));
    
    if(eArtifact == E_ARTIFACT::ARTI_103_BERSERKER_SPRIT ||
       eArtifact == E_ARTIFACT::ARTI_104_BERSERKER_AMULET ||
       eArtifact == E_ARTIFACT::ARTI_105_INFINITE_DISCOUNT)
    {
        _listBg->initWithFile("Assets/ui_common/list_bg_4.png", Rect::ZERO, Rect(0,51,150,10));
        _listBg->setContentSize(Size(getContentSize().width*0.98, C_BAR_SIZE*0.95));
        
        _itemStartBtn->setNormalImage(Sprite::create("Assets/ui_common/box_transcendence1_1.png"));
        _itemStartBtn->setSelectedImage(Sprite::create("Assets/ui_common/box_transcendence1_1.png"));
        
        auto sprIconEffect = Sprite::create("Assets/ui_common/box_transcendence1_2.png");
        sprIconEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprIconEffect->setPosition(_itemStartBtn->getContentSize()/2);
        sprIconEffect->setLocalZOrder(0);
        _sprIcon->setLocalZOrder(1);
        
        auto rotate = RotateBy::create(10, 360);
        sprIconEffect->runAction(RepeatForever::create(rotate));
        
        _itemStartBtn->addChild(sprIconEffect);
    }
    // 제목
    _lbTitle->setString(_artInfo->getTitle());
    
    // 설명
    _lbExp = Label::createWithTTF(_artInfo->getDesc(), GAME_FONT, 22);
    _lbExp->setColor(Color3B::WHITE);
    _lbExp->setAlignment(TextHAlignment::LEFT);
    _lbExp->setAnchorPoint(Vec2(0,0));
    _lbExp->setPosition(Vec2(_lbTitle->getPosition().x, listSize.height*0.2 +2));
    _lbExp->setDimensions(_listBg->getContentSize().width - 116 - 15, _lbExp->getContentSize().height);
    _lbExp->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    _lbExp->setOverflow(Label::Overflow::SHRINK);
    _listBg->addChild(_lbExp);
    
    // 레벨
    _lbLv->setVisible(true);
    
    // 이펙트 수치
    _lbMoney->setTTFConfig(TTFConfig(GAME_FONT, 22, GlyphCollection::DYNAMIC));
    _lbMoney->setColor(Color3B::WHITE);
    _lbMoney->setPosition(Vec2(_lbTitle->getPosition().x, _listBg->getContentSize().height*0.025));
    _lbMoney->setAnchorPoint(Vec2(0,0));
    _lbMoney->setAlignment(TextHAlignment::LEFT);
    
    // 왼쪽 버튼 보물 아이콘 이미지
    _sprIcon->setTexture(_artInfo->getIconPath());
    _sprIcon->getTexture()->setTexParameters(texParams);
    _sprIcon->setScale(_itemStartBtn->getContentSize().width / _sprIcon->getContentSize().width);
    
    // 왼쪽 버튼 계산기 이벤트 설정
    if ( _artInfo->isCalculator() != 0 )
    {
        _itemStartBtn->setCallback(CC_CALLBACK_1(CellArtifact::onCalculator, this));
        
        auto spriteMagnifier = Sprite::create("Assets/ui/calculator/icon_calculator.png");
        spriteMagnifier->setPosition( _itemStartBtn->getContentSize().width - 15, 15 );
        _itemStartBtn->addChild(spriteMagnifier);
    }
    
    _labelCondition = Label::createWithTTF("", GAME_FONT, 22);
    _labelCondition->setPosition(_itemUpgradeBtn->getContentSize().width / 2, _itemUpgradeBtn->getContentSize().height / 2);
    _labelCondition->setDimensions(_itemUpgradeBtn->getContentSize().width * 0.95, _itemUpgradeBtn->getContentSize().height * 0.95);
    _labelCondition->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    _labelCondition->setOverflow(Label::Overflow::SHRINK);
    _labelCondition->setVisible(false);
    _itemUpgradeBtn->addChild(_labelCondition);
    
    // 업그레이드 버튼 이벤트 설정
    if ( _artInfo->getPriceType() == E_ITEMS::GEM )
    {
        _itemUpgradeBtn->setCallback(CC_CALLBACK_1(CellArtifact::onAskPopup, this));
        _sprUpgradeIcon->setTexture("Assets/icon/icon_jewel.png");
    }
    else if ( _artInfo->getPriceType() == E_ITEMS::POINT )
    {
        _itemUpgradeBtn->setCallback(CC_CALLBACK_1(CellArtifact::onAskPopup, this));
        _sprUpgradeIcon->setTexture("Assets/icon/icon_point1.png");
    }
    else
    {
        _itemUpgradeBtn->setCallback(CC_CALLBACK_1(CellArtifact::onUpgrade, this));
        _sprUpgradeIcon->setTexture("Assets/icon/menu_key.png");
    }
    _itemUpgradeBtn->setVisible(false);
    
    // 할인 보물
    if ( _artInfo->getType() == E_ARTIFACT_TYPE::ARTI_TYPE_DISCOUNT )
    {
        _listBg->initWithFile("Assets/ui_common/list_bg_3.png", Rect::ZERO, Rect(0,51,150,10));
        _listBg->setContentSize(Size(getContentSize().width*0.98, C_BAR_SIZE*0.95));
        
        ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_3.png");
        ((Sprite*)_itemUpgradeBtn->getSelectedImage())->setTexture("Assets/ui_common/btn_upgrade_bg_3.png");
        _itemUpgradeBtn->setCallback(CC_CALLBACK_1(CellArtifact::onAskDevilPopup, this));
        _sprUpgradeIcon->setTexture("Assets/icon/icon_coin_2.png");
        _lbUpgrade_top->setVisible(false);
    }
    
    
    if ( _artInfo->getPricePoint() != 0 )
    {
        _itemUpgradePointBtn = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_point.png"), Sprite::create("Assets/ui_common/btn_point.png"), nullptr,  CC_CALLBACK_1(CellArtifact::onUpgradePoint, this));
        _itemUpgradePointBtn->setVisible(false);
        menu->addChild(_itemUpgradePointBtn);
        
        auto spriteIcon = Sprite::create("Assets/icon/icon_point1.png");
        spriteIcon->setPosition(Vec2(_itemUpgradePointBtn->getContentSize().width*0.25, _itemUpgradePointBtn->getContentSize().height*0.25));
        _itemUpgradePointBtn->addChild(spriteIcon);
        
        auto labelPoint = MafNode::createBmFont(MafUtils::toString(_artInfo->getPricePoint()), GAME_BM_FONT_AL, 28);
        labelPoint->setAnchorPoint(Vec2(0.0,0.5));
        labelPoint->setPosition( Vec2(_itemUpgradePointBtn->getContentSize().width*0.47, _itemUpgradePointBtn->getContentSize().height*0.25) );
        labelPoint->setColor(COLOR_COIN);
        _itemUpgradePointBtn->addChild(labelPoint);
        
        auto labelEffect = MafNode::createBmFont("LV+1", GAME_BM_FONT_AL, 24);
        labelEffect->setAnchorPoint(Vec2(0.5,1));
        labelEffect->setPosition(_itemUpgradePointBtn->getContentSize().width*0.5, _itemUpgradePointBtn->getContentSize().height*0.82);
        labelEffect->setColor(COLOR_UPGRADE_TOP);
        _itemUpgradePointBtn->addChild(labelEffect);
    }
    
    if( _artInfo->getType() > E_ARTIFACT_TYPE::ARTI_TYPE_NONE && _artInfo->getType() < E_ARTIFACT_TYPE::ARTI_TYPE_CROSS)
    {
        _itemUpgradeCashBtn = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/artifacts/pack_1.png"), Sprite::create("Assets/ui/artifacts/pack_1.png"), nullptr, CC_CALLBACK_1(CellArtifact::onPurhcase, this));
        _itemUpgradeCashBtn->setVisible(false);
        menu->addChild(_itemUpgradeCashBtn);
        
        _labelUpgradeCashPrice = Label::createWithSystemFont("0", GAME_FONT, 24);
        _labelUpgradeCashPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _labelUpgradeCashPrice->setPosition(_itemUpgradeCashBtn->getContentSize().width / 2, 5);
        _labelUpgradeCashPrice->setColor(Color3B::WHITE);
        _labelUpgradeCashPrice->setDimensions(_itemUpgradeCashBtn->getContentSize().width * 0.95, _labelUpgradeCashPrice->getContentSize().height);
        _labelUpgradeCashPrice->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        _labelUpgradeCashPrice->setOverflow(Label::Overflow::SHRINK);
        _itemUpgradeCashBtn->addChild(_labelUpgradeCashPrice);
    }
    
    if ( _artInfo->isReinforce() != 0 )
    {
        _itemUpgradeExpandBtn = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_above_level.png"), Sprite::create("Assets/ui_common/btn_above_level.png"), nullptr, CC_CALLBACK_1(CellArtifact::onUpgradeExpand, this));
        _itemUpgradeExpandBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _itemUpgradeExpandBtn->setPosition(_itemUpgradeBtn->getPosition());
        _itemUpgradeExpandBtn->setVisible(false);
        menu->addChild(_itemUpgradeExpandBtn);
        
        _labelReinforce = Label::createWithTTF("0", GAME_FONT, 20);
        _labelReinforce->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        _labelReinforce->setPosition(_itemStartBtn->getContentSize().width - 5, _itemStartBtn->getContentSize().height);
        _labelReinforce->setColor(Color3B::MAGENTA);
        _labelReinforce->enableOutline(Color4B::BLACK, 2);
        _labelReinforce->setVisible(false);
        _itemStartBtn->addChild(_labelReinforce, 10);
    }
    
    
    
    //
    drawUpgrade();
    
    //
    if ( UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _eArtifact) < _artInfo->getLevelMax() )
    {
        _fRedrawTimeMax = 0.5f;
        scheduleUpdate();
    }
    else
    {
        showArtifactComic();
    }
}

CellArtifact::~CellArtifact()
{
    
}

void CellArtifact::setTab(int tab)
{
    if ( _nTab != tab )
    {
        _nTab = tab;
        
        //
        drawUpgrade();
    }
}

void CellArtifact::setCellArtifactTop(CellArtifactTop* cellArtifactTop)
{
    _cellArtifactTop = cellArtifactTop;
}

void CellArtifact::update(float dt)
{
    _fRedrawTime += dt;
    if (_fRedrawTime >= _fRedrawTimeMax)
    {
        _fRedrawTime = 0;
        
        //
        drawUpgrade();
        
        //
        int level = MafAes256::XorDecryptInt(_levelEncrypt);
        if ( level >= _artInfo->getLevelMax() )
        {
            unscheduleUpdate();
            
            if ( _bUpgrade == true )
            {
                _bUpgrade = false;
                if ( _onCompleteListener != nullptr )
                {
                    _onCompleteListener();
                }
            }
            auto info = DataManager::GetArtifactInfo(_eArtifact);
            if(info->isCalculator()!=0) //일반보물,십자가등에 사용 가능.
            {
                showArtifactComic();
                HelloWorldScene::getInstance()->createArtifactTable(_nTab);
                HelloWorldScene::getInstance()->showTable();
            }
            if(info->getType() != ARTI_TYPE_NONE && InfiniteArtifactManager::getInstance()->isOpenCondition())
            {
                HelloWorldScene::getInstance()->createArtifactTable(1);
                HelloWorldScene::getInstance()->showTable();
            }
            return;
        }
    }
    
    
    if ( _artInfo->getPriceType() == E_ITEMS::KEY )
    {
        if (_itemUpgradeBtn->isSelected() )
        {
            _fUpgradeTime += dt;
            if (_fUpgradeTimeMax <= _fUpgradeTime)
            {
                _bUpgrade = true;
                _fUpgradeTime = 0;
                _fUpgradeTimeMax -= 0.02f;
                
                
                onUpgrade(nullptr);
            }
        }
        else if (_itemUpgradePointBtn != nullptr && _itemUpgradePointBtn->isSelected() == true )
        {
            _fUpgradeTime += dt;
            if ( _fUpgradeTimeMax <= _fUpgradeTime )
            {
                _bUpgrade = true;
                _fUpgradeTime = 0;
                _fUpgradeTimeMax -= 0.02f;
               
                onUpgradePoint(nullptr);
            }
        }
        else
        {
            _fUpgradeTime = 0;
            _fUpgradeTimeMax = 0.2f;
            if ( _bUpgrade == true )
            {
                _bUpgrade = false;
            
                if(_onCompleteListener!=nullptr){
                    _onCompleteListener();
                }
            }
        }
    }
}

void CellArtifact::onAskPopup(cocos2d::Ref* pSender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    if ( _eArtifact == ARTI_34_PICKAX && WeaponManager::getInstance()->getEquipHigh() < E_WEAPON::WEAPON_76_MINE )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_5"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    
    std::string strTitle = _artInfo->getTitle();
    std::string strMessage = GAME_TEXT("t_ui_msg_buy");
    
    auto popup = PopupDefault::create(strTitle, strMessage);
    popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_yes"), false, CC_CALLBACK_1(CellArtifact::onUpgrade, this));
    popup->show();
}

void CellArtifact::onAskDevilPopup(cocos2d::Ref* pSender)
{
    int level = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _eArtifact);
    if ( level >= _artInfo->getLevelMax() )
    {
        return;
    }
    
    int nowCurrency = atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str());
    auto needCurrency = DataManager::GetArtifactUpgradeKey(E_PLACE::DG_NORMAL, _eArtifact, level);
    if ( nowCurrency < needCurrency )
    {
        drawUpgrade();
        
        auto popup = PopupCurrencyDetail::create(E_ITEMS::COSTUME_COIN);
        popup->show();
        return;
    }
    
    std::string strMessage = GAME_TEXTFORMAT("t_ui_artifact_contents_discount_2", _artInfo->getGeneration(), atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str()));
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
    popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_yes"), false, [=](Ref* sender){
        
        PopupLoading::show();
        requestArtifactDiscount(_artInfo->getGeneration());
    });
    popup->show();
}

void CellArtifact::onUpgrade(cocos2d::Ref* pSender)
{
    int level = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _eArtifact);
    if ( level >= _artInfo->getLevelMax() )
    {
        return;
    }
    
    if(_artInfo->getIdx() == E_ARTIFACT::ARTI_105_INFINITE_DISCOUNT)
    {
        if(!InfiniteArtifactManager::getInstance()->isUpgradeConditionToRiviveKeyArti())
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"),  GAME_TEXT("t_ui_error_48"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            
            return;
        }
    }
    
    if( _artInfo->getPriceType() == E_ITEMS::GEM || _artInfo->getPriceType() == E_ITEMS::POINT )
    {
        auto nowCurrency = 0;
        if ( _artInfo->getPriceType() == E_ITEMS::GEM )
            nowCurrency = ItemsMoneyManager::getInstance()->getGem();
        else if ( _artInfo->getPriceType() == E_ITEMS::POINT )
            nowCurrency = ItemsMoneyManager::getInstance()->getPoint();
        
        auto needCurrency = DataManager::GetArtifactUpgradeKey(E_PLACE::DG_NORMAL,_eArtifact, level);
        if ( nowCurrency < needCurrency )
        {
            _fUpgradeTime = 0;
            _fUpgradeTimeMax = 0.2f;
            
            //
            drawUpgrade();
            
            //
            if ( _itemUpgradeBtn->isSelected() == false )
            {
                if ( _artInfo->getPriceType() == E_ITEMS::GEM )
                {
                    auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
                    popup->show();
                }
                else if ( _artInfo->getPriceType() == E_ITEMS::POINT )
                {
                    auto popup = PopupCurrencyDetail::create(E_ITEMS::POINT);
                    popup->show();
                }
            }
            _itemUpgradeBtn->unselected();
            return;
        }
        
        level ++;
        if ( level >= _artInfo->getLevelMax() )
        {
            level = _artInfo->getLevelMax();
        }
        
        // 재화 감소
        if ( _artInfo->getPriceType() == E_ITEMS::GEM )
            ItemsMoneyManager::getInstance()->setGem(-needCurrency);
        else if ( _artInfo->getPriceType() == E_ITEMS::POINT )
            ItemsMoneyManager::getInstance()->setPoint(-needCurrency);
        
        // 레벨 적용
        UserInfoManager::getInstance()->setArtifactLevel(_ePlace, _eArtifact, level);
        _levelEncrypt = MafAes256::XorEncrypt(level);
    }
    else
    {
        auto nowCurrency = ItemsMoneyManager::getInstance()->getKey();
        auto needCurrency = DataManager::GetArtifactUpgradeKey(E_PLACE::DG_NORMAL,_eArtifact, level);
        
        if ( MafUtils::compareStr(nowCurrency, MafUtils::doubleToString(needCurrency)) == false )
        {
            _fUpgradeTime = 0;
            _fUpgradeTimeMax = 0.2;
            
            //
            drawUpgrade();
            
            //
            if ( _itemUpgradeBtn->isSelected() == false )
            {
                auto popup = PopupCurrencyDetail::create(E_ITEMS::KEY);
                popup->show();
            }
            _itemUpgradeBtn->unselected();
            return;
        }
        
        level ++;
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_10) && (DataManager::IsArtifactTypeNormal(_eArtifact) == true || DataManager::IsArtifactTypeSpecial(_eArtifact) == true) )
        {
            if ( random(1, 100) <= 10 )
            {
                level += 1;
            }
        }
        
        if ( level>=_artInfo->getLevelMax() )
        {
            level = _artInfo->getLevelMax();
        }
            
        // 재화 감소
        ItemsMoneyManager::getInstance()->setKey(-needCurrency);
        
        // 레벨 적용
        UserInfoManager::getInstance()->setArtifactLevel(_ePlace, _eArtifact, level);
        _levelEncrypt = MafAes256::XorEncrypt(level);
    }
    
    if ( _eArtifact == E_ARTIFACT::ARTI_1_EVOL1 )
    {
        if ( level >= 25 )
        {
            // story
            StoryManager::getInstance()->showStory((int)E_COMIC::ARTIFACT_RING);
        }
        
        if ( level >= 30000 )
        {
            MafGooglePlay::UnlockAchievement(ACHIEVE_30000_ARTIFACT1, 1, 1);
        }
        //tutorial->drawTutorial();
    }
    else if(_eArtifact == E_ARTIFACT::ARTI_3_EVOL1)
    {
        if ( level >= 1 )
        {
            // story
            StoryManager::getInstance()->showStory((int)E_COMIC::ARTIFACT_STORMY);
        }
        //tutorial->drawTutorial();
    }
    else if(_eArtifact == E_ARTIFACT::ARTI_4_EVOL1)
    {
        if ( level >= 50 )
        {
            // story
            StoryManager::getInstance()->showStory((int)E_COMIC::ARTIFACT_EAGLE);
        }
        
        if ( level >= 20000 )
        {
            MafGooglePlay::UnlockAchievement(ACHIEVE_20000_ARTIFACT4, 1, 1);
        }
        //tutorial->drawTutorial();
    }
    else if(_eArtifact == E_ARTIFACT::ARTI_8_EVOL1)
    {
        if ( level >= 50 )
        {
            // story
            StoryManager::getInstance()->showStory((int)E_COMIC::ARTIFACT_EAGLE);
        }
        
        if ( level >= 20000 )
        {
            MafGooglePlay::UnlockAchievement(ACHIEVE_20000_ARTIFACT4, 1, 1);
        }
        //tutorial->drawTutorial();
    }
    
    //
    drawUpgrade();
    
    if ( level >= _artInfo->getLevelMax() )
    {
        if ( _artInfo->getGeneration() == 1 )
        {
            showArtifactComic();
            
            HelloWorldScene::getInstance()->createArtifactTable(_nTab);
            HelloWorldScene::getInstance()->showTable();
        }
    }
    
    //
    if ( pSender != nullptr )
    {
        if ( _onCompleteListener != nullptr )
        {
            _onCompleteListener();
        }
    }
    
    //업적
    auto achieveMgr = AchievementManager::getInstance();
    auto objAchieve = achieveMgr->getObjAchieveNormal(_eArtifact);
    
    if(objAchieve != nullptr)
        achieveMgr->setAchieveNormalMission(objAchieve->getNormalMission(), UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _eArtifact));
    
    if(_eArtifact == E_ARTIFACT::ARTI_5)
    {
        UserInfoManager::getInstance()->getSecondEarnGoldNormal(true);
    }
}

void CellArtifact::onUpgradePoint(cocos2d::Ref* pSender)
{
    int level = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _eArtifact);
    if ( level >= _artInfo->getLevelMax() )
    {
        return;
    }
    
    int nowCurrency = ItemsMoneyManager::getInstance()->getPoint();
    int needCurrency = _artInfo->getPricePoint();
    if ( nowCurrency < needCurrency )
    {
        _fUpgradeTime = 0;
        _fUpgradeTimeMax = 0.2;
        
        //
        drawUpgrade();
        
        //
        if ( _itemUpgradePointBtn->isSelected() == false )
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::POINT);
            popup->show();
        }
        _itemUpgradePointBtn->unselected();
        return;
    }
    
    level ++;
    if (  level >= _artInfo->getLevelMax() )
    {
        level = _artInfo->getLevelMax();
    }
    
    // 재화 감소
    ItemsMoneyManager::getInstance()->setPoint(-needCurrency);

    // 레벨 적용
    UserInfoManager::getInstance()->setArtifactLevel(_ePlace, _eArtifact, level);
    _levelEncrypt = MafAes256::XorEncrypt(level);
    
    
    
    if ( _eArtifact == E_ARTIFACT::ARTI_1_EVOL1 )
    {
        if ( level >= 25 )
        {
            // story
            StoryManager::getInstance()->showStory((int)E_COMIC::ARTIFACT_RING);
        }
        
        if ( level >= 30000 )
        {
            MafGooglePlay::UnlockAchievement(ACHIEVE_30000_ARTIFACT1, 1, 1);
        }
    }
    else if(_eArtifact == E_ARTIFACT::ARTI_3_EVOL1)
    {
        if ( level >= 50 )
        {
            // story
            StoryManager::getInstance()->showStory((int)E_COMIC::ARTIFACT_STORMY);
        }
    }
    else if(_eArtifact == E_ARTIFACT::ARTI_4_EVOL1)
    {
        if ( level >= 50 )
        {
            // story
            StoryManager::getInstance()->showStory((int)E_COMIC::ARTIFACT_EAGLE);
        }
        
        if ( level >= 20000 )
        {
            MafGooglePlay::UnlockAchievement(ACHIEVE_20000_ARTIFACT4, 1, 1);
        }
    }
    
    //
    drawUpgrade();
    
    //
    if ( pSender != nullptr )
    {
        if ( _onCompleteListener != nullptr )
        {
            _onCompleteListener();
        }
    }
    //업적
    auto achieveMgr = AchievementManager::getInstance();
    auto objAchieve = achieveMgr->getObjAchieveNormal(_eArtifact);
    
    if(objAchieve != nullptr)
        achieveMgr->setAchieveNormalMission(objAchieve->getNormalMission(), UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _eArtifact));
    
    if(_eArtifact == E_ARTIFACT::ARTI_5)
    {
        UserInfoManager::getInstance()->getSecondEarnGoldNormal(true);
    }
}

void CellArtifact::onPurhcase(cocos2d::Ref* pSender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    //
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
                ItemsManager::getInstance()->setParameterArtifact(_eArtifact);
                CashManager::getInstance()->purchaseInitiate(nItemIdx, CC_CALLBACK_4(CellArtifact::ResultInAppPurchase, this));
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
            ItemsManager::getInstance()->setParameterArtifact(_eArtifact);
            CashManager::getInstance()->purchaseInitiate(nItemIdx, CC_CALLBACK_4(CellArtifact::ResultInAppPurchase, this));
        }
    });
}

void CellArtifact::onCalculator(cocos2d::Ref* pSender)
{
    if ( _artInfo == nullptr || _artInfo->isCalculator() == 0 )
    {
        return;
    }
    
    if ( _nTab == 100 )
    {
        if ( UserInfoManager::getInstance()->isArtifactReinforce(_ePlace, _artInfo->getIdx()) == true )
        {
            auto popup = PopupCalArtifactReinforce::create(_artInfo);
            popup->setCallback(CC_CALLBACK_0(CellArtifact::callbackCalculator, this));
            popup->show();
        }
    }
    else
    {
        auto popup = PopupCalArtifact::create(_artInfo);
        popup->setCallback(CC_CALLBACK_0(CellArtifact::callbackCalculator, this));
        popup->show();
    }
}

void CellArtifact::onUpgradeExpand(cocos2d::Ref* pSender)
{
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    if ( level < _artInfo->getLevelMax() )
    {
        return;
    }
    
    int nReinforce = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, _artInfo->getIdx());
    if ( nReinforce >= _artInfo->getLevelReinforceMax() )
    {
        return;
    }
    
    //맥스 레벨 이상
    if ( UserInfoManager::getInstance()->isArtifactReinforce(_ePlace, _artInfo->getIdx()) == true )
    {//만랩 해제 : O
        auto nowCurrency = ItemsMoneyManager::getInstance()->getCube();
        auto needCurrency = DataManager::GetArtifactReinforcePrice(_artInfo->getIdx());
        if ( nowCurrency < needCurrency )
        {
            drawUpgrade();
            
            //
            auto popup = PopupCurrencyDetail::create(E_ITEMS::CUBE);
            popup->show();
            return;
        }
        
        int nReinforce = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, _artInfo->getIdx());
        if ( nReinforce >= _artInfo->getLevelReinforceMax() )
        {
            drawUpgrade();
            return;
        }
        
        // 재화 감소
        ItemsMoneyManager::getInstance()->setCube(-needCurrency);
        
        // 레벨 적용
        UserInfoManager::getInstance()->setArtifactReinforce(_ePlace, _artInfo->getIdx(), nReinforce+1);
        
        //
        drawUpgrade();
        
        if ( _onCompleteReinforceListener != nullptr )
        {
            _onCompleteReinforceListener();
        }
    }
    else
    {//만랩 해제 : X
        auto nowCurrency= ItemsMoneyManager::getInstance()->getGem();
        auto needCurrency = DataManager::GetArtifactReinforceExpandPrice(_artInfo->getIdx());
        
        if ( nowCurrency < needCurrency )
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
            popup->show();
        }
        else
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_artifact_rein_msg_2"));
            popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
            popup->addButton(GAME_TEXT("t_ui_yes"), false, [=](Ref* sender){
                
                onUpgradeExpandOk(pSender);
                
            });
            popup->show();
        }
        
    }
    
    
}

void CellArtifact::onUpgradeExpandOk(cocos2d::Ref* pSender)
{
    if ( UserInfoManager::getInstance()->isArtifactReinforce(_ePlace, _artInfo->getIdx()) == false )
    {//만랩 해제 : X
        auto nowCurrency = ItemsMoneyManager::getInstance()->getGem();
        auto needCurrency = DataManager::GetArtifactReinforceExpandPrice(_artInfo->getIdx());
        
        if ( nowCurrency < needCurrency )
        {
            drawUpgrade();
            return;
        }
        
        // 재화 감소
        ItemsMoneyManager::getInstance()->setGem(-needCurrency);
        
        // 만램 해제
        UserInfoManager::getInstance()->setArtifactReinforce(_ePlace, _artInfo->getIdx(), 0);
        
    }
    
    
    //
    drawUpgrade();
    
    //
    if ( _onCompleteReinforceListener != nullptr )
    {
        _onCompleteReinforceListener();
    }
}

void CellArtifact::drawUpgrade()
{
    _itemUpgradeBtn->setVisible(false);
    
    if ( _itemUpgradePointBtn != nullptr )              _itemUpgradePointBtn->setVisible(false);
    if ( _itemUpgradeCashBtn != nullptr )               _itemUpgradeCashBtn->setVisible(false);
    
    if ( _itemUpgradeExpandBtn != nullptr )             _itemUpgradeExpandBtn->setVisible(false);
    
    if ( _nTab == 100 )
    {
        drawUpgradeExpand();
    }
    else
    {
        drawUpgradeDefault();
    }
    
    
    // 레벨
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    int reinforce = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, _eArtifact);
    std::string strLevel = StringUtils::format("LV.%d(Max%d)", level, _artInfo->getLevelMax());
    if ( _artInfo->getLevelMax() == 0 )
    {
        strLevel = StringUtils::format("LV.%d", level);
    }
    else if ( level >= _artInfo->getLevelMax() )
    {
        strLevel = StringUtils::format("LV.%d(Max)", level);
    }
    
    if (_eArtifact == E_ARTIFACT::ARTI_9_KEY)
    {
        int honorEffect = DataManager::GetHonorEffect(E_HONOR::HONOR_3, UserInfoManager::getInstance()->getHonorLevel(E_HONOR::HONOR_3));
        if ( CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_6) )
        {
            honorEffect += 10;
            level += 10;
        }
        
        if ( honorEffect > 0)
        {
            strLevel.append(MafUtils::format(" [+%dLV]", honorEffect));
        }
    }
    _lbLv->setString(strLevel);
    
    
    // 효과(이펙트)
    std::string strEffect = "";
    double effect = effect = DataManager::GetArtifactEffect(_eArtifact, level, reinforce);
    if(_eArtifact == E_ARTIFACT::ARTI_34_PICKAX)
    {
        effect = MineralManager::getInstance()->getPickaxEffect();
    }
    
    if( _eArtifact == E_ARTIFACT::ARTI_7 || _eArtifact == E_ARTIFACT::ARTI_32_PRISON_WEAPON )
    {
        strEffect = GAME_TEXTFORMAT("t_arti_d1_option_1", level);
    }
    else if ( _eArtifact == E_ARTIFACT::ARTI_11_KEY )
    {
        strEffect = GAME_TEXTFORMAT("t_arti_d1_option_2", effect);
    }
    else if ( _eArtifact == E_ARTIFACT::ARTI_34_PICKAX )
    {
        strEffect = GAME_TEXTFORMAT("t_arti_d1_option_3", effect);
    }
    else if ( _eArtifact == E_ARTIFACT::ARTI_50 )
    {
        if ( level > 0 )
        {
            strEffect = GAME_TEXTFORMAT("t_arti_d1_option_4", effect);
        }
    }
    else if(_eArtifact == E_ARTIFACT::ARTI_51)
    {
        strEffect = GAME_TEXTFORMAT("t_arti_d1_option_5", effect);
    }
    else if(_eArtifact == E_ARTIFACT::ARTI_52)
    {
        strEffect = GAME_TEXTFORMAT("t_arti_d1_option_6", effect);
    }
    else
    {
        if(_eArtifact == E_ARTIFACT::ARTI_17_CROSS_EVOL2)
        {
            if (effect < 1000)  strEffect = MafUtils::format("%.1lf",effect);
            else                strEffect = MafUtils::format("%.0lf", effect);
        }
        else if(_eArtifact == E_ARTIFACT::ARTI_2 ||
                _eArtifact == E_ARTIFACT::ARTI_6 ||
                _eArtifact == E_ARTIFACT::ARTI_9_KEY ||
                _eArtifact == E_ARTIFACT::ARTI_10_KEY ||
                _eArtifact == E_ARTIFACT::ARTI_1_EVOL1 ||
                _eArtifact == E_ARTIFACT::ARTI_3_EVOL1 ||
                _eArtifact == E_ARTIFACT::ARTI_4_EVOL1 ||
                _eArtifact == E_ARTIFACT::ARTI_8_EVOL1 ||
                _eArtifact == E_ARTIFACT::ARTI_12_CROSS_EVOL1)
        {
            strEffect = MafUtils::format("%.0lf", effect);
        }
        else if(_eArtifact == E_ARTIFACT::ARTI_5)
        {
            strEffect = MafUtils::format("%.0lf", effect * 100);
        }
        else if(_eArtifact == E_ARTIFACT::ARTI_106)
        {
            strEffect = MafUtils::format("%.1lf",effect);
        }
        else
        {
            if (effect < 1000)  strEffect = MafUtils::format("%.2lf",effect);
            else                strEffect = MafUtils::format("%.0lf", effect);
        }
        
        strEffect = MafUtils::addCommaToString(strEffect, 3, ',');
        strEffect.append("%");
    }
    _lbMoney->setString(strEffect);
    
    
    // 설명 : 사이즈 재설정
    double widthExp = _listBg->getContentSize().width - 116 - 150 - 15;
    if ( _itemUpgradePointBtn != nullptr && _itemUpgradePointBtn->isVisible() == true )
    {
        widthExp -= 105;
    }

    if ( _itemUpgradeCashBtn != nullptr && _itemUpgradeCashBtn->isVisible() == true )
    {
        widthExp -= 105;
    }

    auto config = _lbExp->getTTFConfig();
    config.fontSize = 22;
    _lbExp->setTTFConfig(config);
    _lbExp->setDimensions(widthExp, _lbExp->getContentSize().height);
    

    // 강화
    if ( _labelReinforce != nullptr )
    {
        int nReinforce = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, _artInfo->getIdx());
        if ( nReinforce > 0 )
        {
            std::string strReinforce = "+";
            strReinforce.append(MafUtils::toString(nReinforce));
            
            _labelReinforce->setString(strReinforce);
            _labelReinforce->setVisible(true);
        }
        else
        {
            _labelReinforce->setString("");
            _labelReinforce->setVisible(false);
        }
    }
}

void CellArtifact::drawUpgradeDefault()
{
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    int reinforce = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, _eArtifact);
    int floorHigh = UserInfoManager::getInstance()->getHighFloor(_ePlace);
    
    _sprUpgradeIcon->setVisible(false);
    _lbUpgrade->setVisible(false);
    _lbUpgrade_top->setVisible(false);
    _labelCondition->setVisible(false);
    
    if ( level >= _artInfo->getLevelMax() )
    {
        _itemUpgradeBtn->setVisible(false);
        if ( _itemUpgradePointBtn != nullptr )              _itemUpgradePointBtn->setVisible(false);
        if ( _itemUpgradeCashBtn != nullptr )               _itemUpgradeCashBtn->setVisible(false);
        
        return;
    }
    
    if ( _artInfo->getCondition() >= floorHigh )
    {
        int nConditionDiff = (_artInfo->getCondition() + 1) - floorHigh;
        
        _itemUpgradeBtn->setVisible(true);
        _itemUpgradeBtn->setEnabled(false);
        ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
        ((Sprite*)_itemUpgradeBtn->getSelectedImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
        
        std::string strCondition = GAME_TEXTFORMAT("t_ui_pet_msg_6_1", _artInfo->getCondition());
        strCondition.append("\n");
        strCondition.append(GAME_TEXTFORMAT("t_ui_pet_msg_17", nConditionDiff));
        _labelCondition->setVisible(true);
        _labelCondition->setString(strCondition);
        return;
    }
    
    // 업그레이드 버튼
    _itemUpgradeBtn->setVisible(true);
    _itemUpgradeBtn->setEnabled(true);
    ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2.png");
    ((Sprite*)_itemUpgradeBtn->getSelectedImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2.png");
    _sprUpgradeIcon->setVisible(true);
    _lbUpgrade->setVisible(true);
    _lbUpgrade_top->setVisible(true);
    _labelCondition->setVisible(false);
    
    //
    if( _artInfo->getPriceType() == E_ITEMS::GEM || _artInfo->getPriceType() == E_ITEMS::POINT || _artInfo->getPriceType() == E_ITEMS::COSTUME_COIN )
    {
        auto nowCurrency = 0;
        if ( _artInfo->getPriceType() == E_ITEMS::GEM )
            nowCurrency = ItemsMoneyManager::getInstance()->getGem();
        else if ( _artInfo->getPriceType() == E_ITEMS::POINT )
            nowCurrency = ItemsMoneyManager::getInstance()->getPoint();
        else if ( _artInfo->getPriceType() == E_ITEMS::COSTUME_COIN )
            nowCurrency = atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str());
        
        auto needCurrency = DataManager::GetArtifactUpgradeKey(E_PLACE::DG_NORMAL,_eArtifact, level);
        if ( nowCurrency < needCurrency )
        {
            ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
            ((Sprite*)_itemUpgradeBtn->getSelectedImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
        }
        else
        {
            if(_artInfo->getIdx() == E_ARTIFACT::ARTI_105_INFINITE_DISCOUNT)
            {
                if(!InfiniteArtifactManager::getInstance()->isUpgradeConditionToRiviveKeyArti())
                {
                    ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
                    ((Sprite*)_itemUpgradeBtn->getSelectedImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
                }
            }
        }
    }
    else
    {
        auto nowCurrency= ItemsMoneyManager::getInstance()->getKey();
        auto needCurrency = DataManager::GetArtifactUpgradeKey(E_PLACE::DG_NORMAL,_eArtifact, level);
        
        if ( MafUtils::compareStr(nowCurrency, MafUtils::doubleToString(needCurrency)) == false )
        {
            ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
            ((Sprite*)_itemUpgradeBtn->getSelectedImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
        }
    }
    
    //
    Vec2 posLast = Vec2(_itemUpgradeBtn->getPositionX() - _itemUpgradeBtn->getContentSize().width - 5, _itemUpgradeBtn->getPositionY());
    
    // 업그레이드 버튼 : 필요 금액
    auto needKey = DataManager::GetArtifactUpgradeKey(E_PLACE::DG_NORMAL,_eArtifact, level);
    std::string strNeedKey = MafUtils::doubleToString(needKey);
    if ( _artInfo->getPriceType() == E_ITEMS::KEY )
    {
        MafUtils::convertNumberToShort(strNeedKey);
    }
    _lbUpgrade->setString(strNeedKey.c_str());

    // 업그레이드 버튼 : 추가 이펙트
    double effect = DataManager::GetArtifactEffect(_eArtifact, level, reinforce);
    double effectNext = DataManager::GetArtifactEffect(_eArtifact, level+1, reinforce);
    if ( _eArtifact == E_ARTIFACT::ARTI_50 )
    {
        effectNext = DataManager::GetArtifactEffect(_eArtifact, level+1, reinforce);
    }
    else
    {
        effectNext -= effect;
    }

    auto artiInfo = DataManager::GetArtifactInfo(_eArtifact);
    std::string strEffect = "";
    if(_eArtifact==E_ARTIFACT::ARTI_7)
    {
        strEffect = "1";
    }
    else if(_eArtifact == E_ARTIFACT::ARTI_2 ||
            _eArtifact == E_ARTIFACT::ARTI_6 ||
            _eArtifact == E_ARTIFACT::ARTI_9_KEY ||
            _eArtifact == E_ARTIFACT::ARTI_10_KEY ||
            _eArtifact == E_ARTIFACT::ARTI_11_KEY ||
            _eArtifact == E_ARTIFACT::ARTI_32_PRISON_WEAPON ||
            _eArtifact == E_ARTIFACT::ARTI_34_PICKAX ||
            _eArtifact == E_ARTIFACT::ARTI_50 ||
            _eArtifact == E_ARTIFACT::ARTI_51 ||
            _eArtifact == E_ARTIFACT::ARTI_52 )
    {
        strEffect = MafUtils::format("%.0lf", effectNext);
    }
    else if(_eArtifact == E_ARTIFACT::ARTI_5)
    {
        strEffect = MafUtils::format("%.0lf", effectNext * 100);
    }
    else if(artiInfo->getGeneration()==1)
    {
        strEffect = MafUtils::doubleToString(effectNext);
        strEffect = MafUtils::convertNumberToShort(strEffect);
    }
    else
    {
        if (effectNext >= 1000)
        {
            strEffect = MafUtils::doubleToString(effectNext);
            strEffect = MafUtils::convertNumberToShort(strEffect);
        }
        else
        {
            strEffect = MafUtils::format("%.2f", effectNext);
        }
    }

    if ( _eArtifact != E_ARTIFACT::ARTI_50 ) // 만능 포인트 보관함 + 기호 추가 안함.
    {
        strEffect.insert(0, "+");
    }
    _lbUpgrade_top->setString(strEffect);

    
    // 업그레이드 만능 포인트 버튼
    if ( _artInfo->getPricePoint() > 0 && _itemUpgradePointBtn != nullptr )
    {
        _itemUpgradePointBtn->setVisible(true);
        ((Sprite*)_itemUpgradePointBtn->getNormalImage())->setTexture("Assets/ui_common/btn_point.png");
        ((Sprite*)_itemUpgradePointBtn->getSelectedImage())->setTexture("Assets/ui_common/btn_point.png");
        _itemUpgradePointBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _itemUpgradePointBtn->setPosition(posLast);
        
        auto nowCurrency= ItemsMoneyManager::getInstance()->getPoint();
        auto needCurrency = _artInfo->getPricePoint();
        if ( nowCurrency < needCurrency )
        {
            ((Sprite*)_itemUpgradePointBtn->getNormalImage())->setTexture("Assets/ui_common/btn_point_off.png");
            ((Sprite*)_itemUpgradePointBtn->getSelectedImage())->setTexture("Assets/ui_common/btn_point_off.png");
        }
        
        posLast.x = posLast.x - _itemUpgradePointBtn->getContentSize().width - 5;
    }
    
    
    // 업그레이드 결제 버튼
    if( _itemUpgradeCashBtn != nullptr && ( _artInfo->getType() > E_ARTIFACT_TYPE::ARTI_TYPE_NONE && _artInfo->getType() < E_ARTIFACT_TYPE::ARTI_TYPE_CROSS))
    {
        int nBuyCount = UserDefault::getInstance()->getIntegerForKey(MafUtils::format("pack_%d",_eArtifact).c_str(), 0);
        if ( nBuyCount == 2 )
        {
            _itemUpgradeCashBtn->setVisible(false);
        }
        else
        {
            _itemUpgradeCashBtn->setVisible(true);
            _itemUpgradeCashBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            _itemUpgradeCashBtn->setPosition(posLast);
            
            posLast.x = posLast.x - _itemUpgradeCashBtn->getContentSize().width - 5;
            
            int nIdx = 0;            
            if(_artInfo->getType() == E_ARTIFACT_TYPE::ARTI_TYPE_ATK)
            {
                if(nBuyCount >= 1)
                    _itemUpgradeCashBtn->setVisible(false);
                nIdx = E_CASH::CASH_ARTIFACT_3000_1;
            }
            else if(_artInfo->getType() == E_ARTIFACT_TYPE::ARTI_TYPE_CRI)
            {
                if(nBuyCount >= 1)
                    _itemUpgradeCashBtn->setVisible(false);
                nIdx = E_CASH::CASH_ARTIFACT_3000_2;
            }
            else if(_artInfo->getType() == E_ARTIFACT_TYPE::ARTI_TYPE_QUEST)
            {
                if(nBuyCount==0)    nIdx = E_CASH::CASH_ARTIFACT_300_3;
                else                nIdx = E_CASH::CASH_ARTIFACT_3000_3;
            }
            else if(_artInfo->getType() == E_ARTIFACT_TYPE::ARTI_TYPE_KILL)
            {
                if(nBuyCount==0)    nIdx = E_CASH::CASH_ARTIFACT_300_4;
                else                nIdx = E_CASH::CASH_ARTIFACT_3000_4;
            }
            
            if(_artInfo->getType() ==  E_ARTIFACT_TYPE::ARTI_TYPE_ATK || _artInfo->getType() == E_ARTIFACT_TYPE::ARTI_TYPE_CRI)
            {
                ((Sprite*)_itemUpgradeCashBtn->getNormalImage())->setTexture("Assets/ui/artifacts/pack_2.png");
                ((Sprite*)_itemUpgradeCashBtn->getSelectedImage())->setTexture("Assets/ui/artifacts/pack_2.png");
            }
            else
            {
                ((Sprite*)_itemUpgradeCashBtn->getNormalImage())->setTexture(nBuyCount == 0 ? "Assets/ui/artifacts/pack_1.png" : "Assets/ui/artifacts/pack_2.png");
                ((Sprite*)_itemUpgradeCashBtn->getSelectedImage())->setTexture(nBuyCount == 0 ? "Assets/ui/artifacts/pack_1.png" : "Assets/ui/artifacts/pack_2.png");
            }
            _itemUpgradeCashBtn->setTag(nIdx);
            
            auto objProduct = CashManager::getInstance()->getProduct(nIdx);
            if ( objProduct != nullptr )
            {
                _labelUpgradeCashPrice->setString(objProduct->getPrice());
                
                std::string strPrice = objProduct->getPrice();
                if ( strPrice.length() == 0 )
                {
                    _labelUpgradeCashPrice->setString(GAME_TEXT("t_ui_shop_price_loading"));
                }
            }
            else
            {
                _itemUpgradeCashBtn->setVisible(false);
            }
        }
    }
   
    
    //
}

void CellArtifact::drawUpgradeExpand()
{
    if ( _artInfo->isReinforce() != 1 )
    {
        if ( _itemUpgradeExpandBtn != nullptr )                 _itemUpgradeExpandBtn->setVisible(false);
        
        return;
    }
    
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    if ( level < _artInfo->getLevelMax() )
    {
        return;
    }
    
    int nReinforce = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, _artInfo->getIdx());
    if ( nReinforce >= _artInfo->getLevelReinforceMax() )
    {
        return;
    }
    
    if(_cellArtifactTop != nullptr)
    {
        _cellArtifactTop->drawInfo();
    }
    
    
    Vec2 posLast = Vec2(_itemUpgradeBtn->getPositionX(), _itemUpgradeBtn->getPositionY());
    
    //4차 보물 이상, 맥스 레벨 이상
    _itemUpgradeExpandBtn->setVisible(true);
    ((Sprite*)_itemUpgradeExpandBtn->getNormalImage())->setTexture("Assets/ui_common/btn_above_level.png");
    ((Sprite*)_itemUpgradeExpandBtn->getSelectedImage())->setTexture("Assets/ui_common/btn_above_level.png");
    _itemUpgradeExpandBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _itemUpgradeExpandBtn->setPosition(posLast);
    
    posLast.x = posLast.x - _itemUpgradeExpandBtn->getContentSize().width - 5;
    
    auto layerTemp = _itemUpgradeExpandBtn->getChildByTag(100);
    if ( layerTemp == nullptr )
    {
        layerTemp = Layer::create();
        layerTemp->setContentSize(_itemUpgradeExpandBtn->getContentSize());
        layerTemp->setPosition(0, 0);
        layerTemp->setTag(100);
        _itemUpgradeExpandBtn->addChild(layerTemp);
    }
    layerTemp->removeAllChildren();
    
    if ( UserInfoManager::getInstance()->isArtifactReinforce(_ePlace, _artInfo->getIdx()) == true )
    {//만랩 해제 : O
        
        auto nowCurrency= ItemsMoneyManager::getInstance()->getCube();
        auto needCurrency = DataManager::GetArtifactReinforcePrice(_artInfo->getIdx());
        
        if ( nowCurrency < needCurrency )
        {
//            _itemUpgradeExpandBtn->setEnabled(false);
            ((Sprite*)_itemUpgradeExpandBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
            ((Sprite*)_itemUpgradeExpandBtn->getSelectedImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
        }
        
        double effect = DataManager::GetArtifactEffect(_eArtifact, level, nReinforce);
        double effectNext = DataManager::GetArtifactEffect(_eArtifact, level, nReinforce+1);
        effectNext -= effect;
        
        std::string strEffect = MafUtils::doubleToString(effectNext);
        strEffect = MafUtils::convertNumberToShort(strEffect);
        strEffect.insert(0, "+");
        
        auto label = MafNode::createBmFont(strEffect, GAME_BM_FONT_AL, 24);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        label->setPosition( layerTemp->getContentSize().width / 2, layerTemp->getContentSize().height * 0.8 );
        label->setDimensions(layerTemp->getContentSize().width * 0.95, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        layerTemp->addChild(label);
        
        auto sprite = Sprite::create("Assets/icon/menu_cube.png");
        sprite->setPosition( layerTemp->getContentSize().width * 0.4, layerTemp->getContentSize().height * 0.4 );
        layerTemp->addChild(sprite);
        
        //
        auto labelPrice = MafNode::createBmFont(MafUtils::toString(DataManager::GetArtifactReinforcePrice(_artInfo->getIdx())), GAME_BM_FONT_AL, 30);
        labelPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelPrice->setPosition(layerTemp->getContentSize().width*0.6, layerTemp->getContentSize().height*0.4);
        labelPrice->setColor(COLOR_COIN);
        labelPrice->setAlignment(TextHAlignment::LEFT);
        labelPrice->setOverflow(Label::Overflow::SHRINK);
        labelPrice->setDimensions(85, 25);
        layerTemp->addChild(labelPrice);
    }
    else
    {//만랩 해제 : X
        auto nowCurrency= ItemsMoneyManager::getInstance()->getGem();
        auto needCurrency = DataManager::GetArtifactReinforceExpandPrice(_artInfo->getIdx());
        
        if ( nowCurrency < needCurrency )
        {
//            _itemUpgradeExpandBtn->setEnabled(false);
            ((Sprite*)_itemUpgradeExpandBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
            ((Sprite*)_itemUpgradeExpandBtn->getSelectedImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
        }
        
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_artifact_rein_msg_1"), GAME_FONT, 24);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        label->setPosition( layerTemp->getContentSize().width / 2, layerTemp->getContentSize().height * 0.8 );
        label->setDimensions(layerTemp->getContentSize().width * 0.95, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        layerTemp->addChild(label);
        
        auto sprite = Sprite::create("Assets/icon/icon_jewel.png");
        sprite->setPosition( layerTemp->getContentSize().width * 0.175, layerTemp->getContentSize().height * 0.25 );
        layerTemp->addChild(sprite);
        
        auto labelPrice = MafNode::createBmFont(MafUtils::toString(DataManager::GetArtifactReinforceExpandPrice(_artInfo->getIdx())), GAME_BM_FONT_AL, 30);
        labelPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelPrice->setPosition(layerTemp->getContentSize().width * 0.350, _itemUpgradeBtn->getContentSize().height * 0.25);
        labelPrice->setColor(COLOR_COIN);
        labelPrice->setAlignment(TextHAlignment::LEFT);
        labelPrice->setOverflow(Label::Overflow::SHRINK);
        labelPrice->setDimensions(85, 25);
        layerTemp->addChild(labelPrice);
    }
    
}

void CellArtifact::showArtifactComic(){
    
    E_COMIC comic;
    if (_eArtifact == E_ARTIFACT::ARTI_1_EVOL1) {
        comic = E_COMIC::MAX_ARTIFACT_1;
    }else if (_eArtifact == E_ARTIFACT::ARTI_3_EVOL1) {
        comic = E_COMIC::MAX_ARTIFACT_3;
    }else if (_eArtifact == E_ARTIFACT::ARTI_4_EVOL1) {
        comic = E_COMIC::MAX_ARTIFACT_4;
    }else if (_eArtifact == E_ARTIFACT::ARTI_8_EVOL1) {
        comic = E_COMIC::MAX_ARTIFACT_8;
    }else if (_eArtifact == E_ARTIFACT::ARTI_12_CROSS_EVOL1) {
        comic = E_COMIC::MAX_ARTIFACT_12;
    }
    else{
        return;
    }
    
    // story
    StoryManager::getInstance()->showStory((int)comic);
    
}

void CellArtifact::callbackCalculator()
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
    if ( _nTab == 100 )
    {
        if ( _onCompleteReinforceListener != nullptr )
        {
            _onCompleteReinforceListener();
        }
    }
    
    //
    if( _onCompleteListener != nullptr )
    {
        _onCompleteListener();
    }
}

void CellArtifact::ResultInAppPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
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
        //업적
    auto achieveMgr = AchievementManager::getInstance();
    auto objAchieve = achieveMgr->getObjAchieveNormal(_eArtifact);
    
    if(objAchieve != nullptr)
        achieveMgr->setAchieveNormalMission(objAchieve->getNormalMission(), UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _eArtifact));
}

void CellArtifact::ResultInAppPurchaseExpand(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    PopupLoading::hide();
    
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        //
        int nCount = UserDefault::getInstance()->getIntegerForKey(MafUtils::format("pack_e_%d",_eArtifact).c_str(), 0);
        UserDefault::getInstance()->setIntegerForKey(MafUtils::format("pack_e_%d", (int)_eArtifact).c_str(), nCount + 1);
        
        // redraw
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
    
        //업적
    auto achieveMgr = AchievementManager::getInstance();
    auto objAchieve = achieveMgr->getObjAchieveNormal(_eArtifact);
    
    if(objAchieve != nullptr)
        achieveMgr->setAchieveNormalMission(objAchieve->getNormalMission(), UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _eArtifact));
}

void CellArtifact::requestArtifactDiscount(int nGeneration)
{
    std::string url = "/artifact/buy";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_gen", nGeneration);
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data)
    {
        std::string dd = data;
        responseArtifactDiscount(response,dd);
    };
    req->send(cb);
}
void CellArtifact::responseArtifactDiscount(cocos2d::network::HttpResponse* response,std::string &data)
{
    PopupLoading::hide();
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
       
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
       
    if ( bError == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 || nResult == 3 )
    {
        int level = MafAes256::XorDecryptInt(_levelEncrypt);
        level ++;
        if ( level >= _artInfo->getLevelMax() )
        {
            level = _artInfo->getLevelMax();
        }
        UserInfoManager::getInstance()->setArtifactLevel(_ePlace, _eArtifact, level);
        _levelEncrypt = MafAes256::XorEncrypt(level);
        
        // 재화 감소
        int nPrice = jsonParser["_price"].GetInt();
        int nCoin = jsonParser["_total_count"].GetInt();
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, nCoin);
    
        // 레벨 적용
        DataManager::initArtifactSaveData(E_PLACE::DG_NORMAL);
        drawUpgrade();
        
        if ( nPrice == 0 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_artifact_contents_discount_3"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
    }
    else
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
}

#pragma mark- Infinite_artifact_mover

CellUpgradeInfinite::CellUpgradeInfinite():
_callbackUpgrade(nullptr),
_callbackMoveInfinite(nullptr)
{
    _nTab = 0;
    
    // 제목
    _lbTitle->setString(GAME_TEXT("t_ui_menu_arti_ascend_title"));
    
    // 설명
    _lbExp = Label::createWithTTF("", GAME_FONT, 22);
    _lbExp->setColor(Color3B::WHITE);
    _lbExp->setAlignment(TextHAlignment::LEFT);
    _lbExp->setAnchorPoint(Vec2(0,0));
    _lbExp->setPosition(Vec2(_lbTitle->getPosition().x, listSize.height*0.2 +2));
    _lbExp->setDimensions(_listBg->getContentSize().width - 116 - 15, _lbExp->getContentSize().height);
    _lbExp->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    _lbExp->setOverflow(Label::Overflow::SHRINK);
    _listBg->addChild(_lbExp);
    
    _lbExp->setString(GAME_TEXT("t_ui_menu_arti_ascend_desc"));
    
    // 레벨
    _lbLv->setVisible(false);
    
    // 이펙트 수치
    _lbMoney->setVisible(false);
    
    // 왼쪽 버튼 보물 아이콘 이미지
    if(!SpriteFrameCache::getInstance()->isSpriteFramesWithFileLoaded("Assets/icon_artifact/transcendence/infinite_icon.plist"))
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Assets/icon_artifact/transcendence/infinite_icon.plist");
    
    

    _sprIcon->getTexture()->setTexParameters(texParams);
    //animation  객체 생성
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.1f);
    for (int i = 1; i < 12; i++)
    {
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("icon_transcendence_%02d.png", i+1));

        animation->addSpriteFrame(frame);
    }

    //animate 객체 생성, animation의 액션형
    auto animate = Animate::create(animation);

    //런액션 실행
    _sprIcon->runAction(RepeatForever::create(animate));
    
    //셀 배경
    _listBg->initWithFile("Assets/ui_common/list_bg_3.png", Rect::ZERO, Rect(0,51,150,10));
    _listBg->setContentSize(Size(getContentSize().width*0.98, C_BAR_SIZE*0.95));
    
    // 업그레이드 버튼 이벤트 설정
    _itemUpgradeBtn->setCallback(CC_CALLBACK_1(CellUpgradeInfinite::onClick, this));
    _sprUpgradeIcon->setTexture("Assets/icon/icon_coin_2.png");
    _itemUpgradeBtn->setVisible(false);

    _lbButtonContent = Label::createWithTTF("", GAME_FONT, 22);
    _lbButtonContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lbButtonContent->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    _lbButtonContent->setDimensions(_itemUpgradeBtn->getContentSize().width, _itemUpgradeBtn->getContentSize().height);
    _itemUpgradeBtn->addChild(_lbButtonContent);
    drawUpgrade();
}

CellUpgradeInfinite::~CellUpgradeInfinite()
{
    
}

void CellUpgradeInfinite::update(float dt)
{
    
}

void CellUpgradeInfinite::setTab(int tab)
{
    if ( _nTab != tab )
    {
        _nTab = tab;
        
        //
        drawUpgrade();
    }
}

void CellUpgradeInfinite::onClick(cocos2d::Ref *sender)
{
    Node* item = (Node*) sender;
    
    if(item->getName().compare("MOVE_INFINITE") == 0)
    {
        if(_callbackMoveInfinite != nullptr)
        {
            _callbackMoveInfinite(HelloWorldScene::E_GAME_STATE::ARTIFACT_INFINITE);
        }
    }
    else if(item->getName().compare("BUY_INFINITE") == 0)
    {
        auto popup = PopupUpgradeInfiniteArtifact::create();
        popup->setCallbackMoveInfinite(CC_CALLBACK_1(CellUpgradeInfinite::callbackUpgrade, this));
        popup->show();
    }
}

void CellUpgradeInfinite::drawUpgrade()
{
    auto infiniteArtiMgr = InfiniteArtifactManager::getInstance();
    bool bInfiniteActive = infiniteArtiMgr->isActive();
    
    if(bInfiniteActive)
    {
        //초월 보물 탭일 때
        _lbButtonContent->setString(GAME_TEXT("t_ui_menu_arti_ascend_btn"));
        _lbButtonContent->setPosition(Vec2(_itemUpgradeBtn->getContentSize()/2) + Vec2::UP * 20);
        _itemUpgradeBtn->setName("BUY_INFINITE");
        
        auto basicInfo = infiniteArtiMgr->getInfiniteBasicInfoFromGrade(infiniteArtiMgr->getCurGrade() + 1);
        
        if(basicInfo != nullptr)
        {
            _lbUpgrade->setString(MafUtils::toString(basicInfo->getDemandCoin()));
            _itemUpgradeBtn->setVisible(true);
        }
        else
        {//없다는 것은 다음 초월 단계가 풀리지 않은 상태
            _itemUpgradeBtn->setVisible(false);
        }
    }
    else
    {
        _lbButtonContent->setString(GAME_TEXT("t_ui_menu_arti_ascend_btn"));
        _lbButtonContent->setPosition(Vec2(_itemUpgradeBtn->getContentSize()/2) + Vec2::UP * 20);
        _itemUpgradeBtn->setName("BUY_INFINITE");
            
        auto basicInfo = infiniteArtiMgr->getInfiniteBasicInfoFromGrade(1);
                
        if(basicInfo != nullptr)
        {
            _itemUpgradeBtn->setVisible(true);
            _lbUpgrade->setString(MafUtils::toString(basicInfo->getDemandCoin()));
        }
    }
    
}

void CellUpgradeInfinite::callbackUpgrade(bool bResult)
{
    if(!bResult)
        return;
    
    if(_callbackUpgrade != nullptr)
    {
        if(InfiniteArtifactManager::getInstance()->isFirst())
        {
            _callbackUpgrade(true);
            
            if(_callbackMoveInfinite != nullptr)
                _callbackMoveInfinite(HelloWorldScene::E_GAME_STATE::ARTIFACT_INFINITE);
            InfiniteArtifactManager::getInstance()->setFirstUpgrade(false);
        }
        else
            _callbackUpgrade(false);
    }
    
    auto uiBattle = dynamic_cast<IBattleInfo*>(UserInfoManager::getInstance()->getBattleUI());
    if ( uiBattle != nullptr )
    {
        auto hero = uiBattle->getUnitHero();
        if ( hero != nullptr )
        {
            hero->actionDeathBlowActive();
        }
    }
    
    removeFromParent();
    
}

void CellUpgradeInfinite::setCallbackMove(std::function<void(int)> callback)
{
    _callbackMoveInfinite = callback;
}

void CellUpgradeInfinite::setCallbackUpgrade(std::function<void(bool)> callback)
{
    _callbackUpgrade = callback;
}

#pragma mark- Infinite_artifact
CellInfiniteArtifact::CellInfiniteArtifact(E_PLACE ePlace, INFINITE_ARTIFACT_TYPE type):
_eArtiType(type),
_fRedrawTime(0),
_fRedrawTimeMax(0.5),
_checkUpgradeListener(nullptr)
{
    _labelUpgradeCashPrice = nullptr;
    
    _itemUpgradeCashBtn = nullptr;
    _itemUpgradePointBtn = nullptr;

    _onCompleteListener = nullptr;
    _ePlace = ePlace;
    
    _artiInfo = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(_eArtiType);
    _levelEncrypt = MafAes256::XorEncrypt(_artiInfo->getCurLv());
    
    _listBg->initWithFile("Assets/ui_common/list_bg_4.png", Rect::ZERO, Rect(0,51,150,10));
    _listBg->setContentSize(Size(getContentSize().width*0.98, C_BAR_SIZE*0.95));
    
    _itemStartBtn->setNormalImage(Sprite::create("Assets/ui_common/box_transcendence1_1.png"));
    _itemStartBtn->setSelectedImage(Sprite::create("Assets/ui_common/box_transcendence1_1.png"));
    
    auto sprIconEffect = Sprite::create("Assets/ui_common/box_transcendence1_2.png");
    sprIconEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprIconEffect->setPosition(_itemStartBtn->getContentSize()/2);
    sprIconEffect->setLocalZOrder(1);
    sprIconEffect->runAction(RepeatForever::create(RotateBy::create(10, 360)));
    sprIconEffect->setName("UI_EFFECT");
    _itemStartBtn->addChild(sprIconEffect);
    
    auto sprGrade = Sprite::create("Assets/ui_common/icon_star1_1.png");
    sprGrade->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    sprGrade->setPosition(0, _itemStartBtn->getContentSize().height);
    sprGrade->setLocalZOrder(3);
    _itemStartBtn->addChild(sprGrade);
    
    _lbGrade = MafNode::createBmFont(MafUtils::toString(_artiInfo->getGrade()), GAME_BM_FONT_AL, 22);
    _lbGrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _lbGrade->setPosition(sprGrade->getContentSize().width, sprGrade->getContentSize().height/2);
    _lbGrade->setColor(Color3B(255,229,0));
    sprGrade->addChild(_lbGrade);
    
    // 제목, 설명, 경로
    std::string strTitle = _artiInfo->getName();
    std::string strContent = _artiInfo->getDesc();
    std::string strPath = _artiInfo->getIconPath();
    
    //
    _lbTitle->setString(strTitle);
    
    // 설명
    _lbExp = Label::createWithTTF(strContent, GAME_FONT, 22);
    _lbExp->setColor(Color3B::WHITE);
    _lbExp->setAlignment(TextHAlignment::LEFT);
    _lbExp->setAnchorPoint(Vec2(0,0));
    _lbExp->setPosition(Vec2(_lbTitle->getPosition().x, listSize.height*0.2 +2));
    _lbExp->setDimensions(_listBg->getContentSize().width - 116 - 15, _lbExp->getContentSize().height);
    _lbExp->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    _lbExp->setOverflow(Label::Overflow::SHRINK);
    _listBg->addChild(_lbExp);
    
    // 레벨
    _lbLv->setVisible(true);
    
    // 이펙트 수치
    _lbMoney->setTTFConfig(TTFConfig(GAME_FONT, 22, GlyphCollection::DYNAMIC));
    _lbMoney->setColor(Color3B::WHITE);
    _lbMoney->setPosition(Vec2(_lbTitle->getPosition().x, _listBg->getContentSize().height*0.025));
    _lbMoney->setAnchorPoint(Vec2(0,0));
    _lbMoney->setAlignment(TextHAlignment::LEFT);
    
    // 왼쪽 버튼 보물 아이콘 이미지
    _sprIcon->setTexture(strPath);
    _sprIcon->getTexture()->setTexParameters(texParams);
    _sprIcon->setScale(_itemStartBtn->getContentSize().width / _sprIcon->getContentSize().width);
    _sprIcon->setLocalZOrder(2);
    
    // 왼쪽 버튼 계산기 이벤트 설정
    _itemStartBtn->setCallback(CC_CALLBACK_1(CellInfiniteArtifact::onClick, this));
    _itemStartBtn->setName("CALCULATOR");
        
    auto spriteMagnifier = Sprite::create("Assets/ui/calculator/icon_calculator.png");
    spriteMagnifier->setPosition( _itemStartBtn->getContentSize().width - 15, 15 );
    spriteMagnifier->setLocalZOrder(3);
    _itemStartBtn->addChild(spriteMagnifier);
    
    
    // 업그레이드 버튼 이벤트 설정
    _itemUpgradeBtn->setCallback(CC_CALLBACK_1(CellInfiniteArtifact::onClick, this));
    _itemUpgradeBtn->setName("UPGRADE_KEY");
    _itemUpgradeBtn->setVisible(false);
    
    _sprUpgradeIcon->setTexture("Assets/icon/menu_key.png");
    
    
    if (_artiInfo->getDemandPoint() != 0 )
    {
        _itemUpgradePointBtn = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_point.png"), Sprite::create("Assets/ui_common/btn_point.png"), nullptr,  CC_CALLBACK_1(CellInfiniteArtifact::onClick, this));
        _itemUpgradePointBtn->setName("UPGRADE_POINT");
        _itemUpgradePointBtn->setVisible(false);
        menu->addChild(_itemUpgradePointBtn);
        
        auto spriteIcon = Sprite::create("Assets/icon/icon_point1.png");
        spriteIcon->setPosition(Vec2(_itemUpgradePointBtn->getContentSize().width*0.25, _itemUpgradePointBtn->getContentSize().height*0.25));
        _itemUpgradePointBtn->addChild(spriteIcon);
        
        auto labelPoint = MafNode::createBmFont(MafUtils::toString(_artiInfo->getDemandPoint()), GAME_BM_FONT_AL, 28);
        labelPoint->setAnchorPoint(Vec2(0.0,0.5));
        labelPoint->setPosition( Vec2(_itemUpgradePointBtn->getContentSize().width*0.47, _itemUpgradePointBtn->getContentSize().height*0.25) );
        labelPoint->setColor(COLOR_COIN);
        labelPoint->setName("LABEL_POINT");
        _itemUpgradePointBtn->addChild(labelPoint);
        
        auto labelEffect = MafNode::createBmFont("LV+1", GAME_BM_FONT_AL, 24);
        labelEffect->setAnchorPoint(Vec2(0.5,1));
        labelEffect->setPosition(_itemUpgradePointBtn->getContentSize().width*0.5, _itemUpgradePointBtn->getContentSize().height*0.82);
        labelEffect->setColor(COLOR_UPGRADE_TOP);
        _itemUpgradePointBtn->addChild(labelEffect);
    }
    
    if( _eArtiType != INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS)
    {
        _itemUpgradeCashBtn = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/artifacts/pack_1.png"), Sprite::create("Assets/ui/artifacts/pack_1.png"), nullptr,  CC_CALLBACK_1(CellInfiniteArtifact::onClick, this));
        _itemUpgradeCashBtn->setName("PURCHASE");
        _itemUpgradeCashBtn->setVisible(false);
        menu->addChild(_itemUpgradeCashBtn);
        
        _labelUpgradeCashPrice = Label::createWithSystemFont("0", GAME_FONT, 24);
        _labelUpgradeCashPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _labelUpgradeCashPrice->setPosition(_itemUpgradeCashBtn->getContentSize().width / 2, 5);
        _labelUpgradeCashPrice->setColor(Color3B::WHITE);
        _labelUpgradeCashPrice->setDimensions(_itemUpgradeCashBtn->getContentSize().width * 0.95, _labelUpgradeCashPrice->getContentSize().height);
        _labelUpgradeCashPrice->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        _labelUpgradeCashPrice->setOverflow(Label::Overflow::SHRINK);
        _itemUpgradeCashBtn->addChild(_labelUpgradeCashPrice);
    }
    
    //
    drawUpgrade();
    
    //
    if (_artiInfo->getCurLv() < _artiInfo->getMaxLv())
    {
        _fRedrawTimeMax = 0.5f;
        scheduleUpdate();
    }
}

CellInfiniteArtifact::~CellInfiniteArtifact()
{
    
}

void CellInfiniteArtifact::update(float dt)
{
    _fRedrawTime += dt;
    if (_fRedrawTime >= _fRedrawTimeMax)
    {
        _fRedrawTime = 0;
        
        //
        drawUpgrade();
        
        //
        int level = MafAes256::XorDecryptInt(_levelEncrypt);
        if ( level >= _artiInfo->getMaxLv() )
        {
            unscheduleUpdate();
            
            if ( _bUpgrade == true )
            {
                _bUpgrade = false;
                if ( _onCompleteListener != nullptr )
                {
                    _onCompleteListener();
                }
            }
            return;
        }
    }
    
    
    if (_itemUpgradeBtn->isSelected() )
    {
        _fUpgradeTime += dt;
        if (_fUpgradeTimeMax <= _fUpgradeTime)
        {
            _bUpgrade = true;
            _fUpgradeTime = 0;
            _fUpgradeTimeMax -= 0.02f;
            
            auto item = Node::create();
            item->setName("UPGRADE_KEY");
            onClick(item);
        }
    }
    else if (_itemUpgradePointBtn != nullptr && _itemUpgradePointBtn->isSelected() == true )
    {
        _fUpgradeTime += dt;
        if ( _fUpgradeTimeMax <= _fUpgradeTime )
        {
            _bUpgrade = true;
            _fUpgradeTime = 0;
            _fUpgradeTimeMax -= 0.02f;
           
            auto item = Node::create();
            item->setName("UPGRADE_POINT");
            onClick(item);
        }
    }
    else
    {
        _fUpgradeTime = 0;
        _fUpgradeTimeMax = 0.2f;
        if ( _bUpgrade == true )
        {
            _bUpgrade = false;
        
            if(_onCompleteListener!=nullptr){
                _onCompleteListener();
            }
        }
    }


}

void CellInfiniteArtifact::drawUpgrade()
{
    
    _itemUpgradeBtn->setVisible(false);
    
    if ( _itemUpgradePointBtn != nullptr )              _itemUpgradePointBtn->setVisible(false);
    if ( _itemUpgradeCashBtn != nullptr )               _itemUpgradeCashBtn->setVisible(false);
    
    drawUpgradeDefault();
    
    
    //
    _lbTitle->setString(_artiInfo->getName());
    
    //
    _sprIcon->setTexture(_artiInfo->getIconPath());
    _sprIcon->getTexture()->setTexParameters(texParams);
    _sprIcon->setScale(_itemStartBtn->getContentSize().width / _sprIcon->getContentSize().width);
    
    if ( _artiInfo->getGrade() > 7 )
    {
        _itemStartBtn->setNormalImage(Sprite::create("Assets/ui_common/box_transcendence2_1.png"));
        _itemStartBtn->setSelectedImage(Sprite::create("Assets/ui_common/box_transcendence2_1.png"));
        
        if ( auto sprIconEffect = _itemStartBtn->getChildByName<Sprite*>("UI_EFFECT"); sprIconEffect != nullptr )
        {
            sprIconEffect->setTexture("Assets/ui_common/box_transcendence2_2.png");
        }
    }
    else
    {
        _itemStartBtn->setNormalImage(Sprite::create("Assets/ui_common/box_transcendence1_1.png"));
        _itemStartBtn->setSelectedImage(Sprite::create("Assets/ui_common/box_transcendence1_1.png"));
        
        if ( auto sprIconEffect = _itemStartBtn->getChildByName<Sprite*>("UI_EFFECT"); sprIconEffect != nullptr )
        {
            sprIconEffect->setTexture("Assets/ui_common/box_transcendence1_2.png");
        }
    }
    
    // 레벨
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    auto maxLv =  _artiInfo->getMaxLv();
    std::string strLevel = StringUtils::format("LV.%d(Max%d)", level, maxLv);
    if ( maxLv == 0 )
    {
        strLevel = StringUtils::format("LV.%d", level);
    }
    else if ( level >= maxLv )
    {
        strLevel = StringUtils::format("LV.%d(Max)", level);
    }
    
    _lbLv->setString(strLevel);
    
    
    // 효과(이펙트)
    std::string strEffect = "";
    double effect = _artiInfo->getEfficiency();
    
    if (effect < 1000)  strEffect = MafUtils::format("%.2lf",effect);
    else                strEffect = MafUtils::format("%.0lf", effect);
    
    strEffect = MafUtils::addCommaToString(strEffect, 3, ',');
    strEffect.append("%");
    
    _lbMoney->setString(strEffect);
    
    
    // 설명 : 사이즈 재설정
    double widthExp = _listBg->getContentSize().width - 116 - 150 - 15;
    if ( _itemUpgradePointBtn != nullptr && _itemUpgradePointBtn->isVisible() == true )
    {
        widthExp -= 105;
    }

    if ( _itemUpgradeCashBtn != nullptr && _itemUpgradeCashBtn->isVisible() == true )
    {
        widthExp -= 105;
    }

    auto config = _lbExp->getTTFConfig();
    config.fontSize = 22;
    _lbExp->setTTFConfig(config);
    _lbExp->setDimensions(widthExp, _lbExp->getContentSize().height);
    

}

void CellInfiniteArtifact::drawUpgradeDefault()
{
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    auto maxLv = _artiInfo->getMaxLv();
    
    if ( level >= maxLv )
    {
        _itemUpgradeBtn->setVisible(false);
        if ( _itemUpgradePointBtn != nullptr )              _itemUpgradePointBtn->setVisible(false);
        if ( _itemUpgradeCashBtn != nullptr )               _itemUpgradeCashBtn->setVisible(false);
        
        return;
    }
        
    // 업그레이드 버튼
    _itemUpgradeBtn->setVisible(true);
    _itemUpgradeBtn->setEnabled(true);
    ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2.png");
    ((Sprite*)_itemUpgradeBtn->getSelectedImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2.png");
    
    auto nowCurrency = ItemsMoneyManager::getInstance()->getKey();
    //string
    //auto needKey = InfiniteArtifactManager::getInfiniteArtiUpgradeKey(_artiInfo->getGrade(), level+1, _eArtiType);
    //double
    auto needKey = InfiniteArtifactManager::getInfiniteArtiUpgradeKey(_artiInfo->getGrade(), _artiInfo->getIncreaseA(), _artiInfo->getIncreaseB(), level+1, _eArtiType, true);
    
    std::string strKey = MafUtils::doubleToString(needKey);
        
    if ( MafUtils::compareStr(nowCurrency, strKey) == false )
    {
        ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
        ((Sprite*)_itemUpgradeBtn->getSelectedImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
    }
        
    //
    Vec2 posLast = Vec2(_itemUpgradeBtn->getPositionX() - _itemUpgradeBtn->getContentSize().width - 5, _itemUpgradeBtn->getPositionY());
        
    // 업그레이드 버튼 : 필요 금액
    MafUtils::convertNumberToShort(strKey);
    
    _lbUpgrade->setString(strKey.c_str());

    // 업그레이드 버튼 : 추가 이펙트
    double effect = _artiInfo->getEfficiency();
    double effectNext = _artiInfo->getEfficiency(1);
    effectNext -= effect;

    std::string strEffect = "";
    if (effectNext >= 1000)
    {
        strEffect = MafUtils::doubleToString(effectNext);
        strEffect = MafUtils::convertNumberToShort(strEffect);
    }
    else
    {
        strEffect = MafUtils::format("%.2f", effectNext);
    }
    
    _lbUpgrade_top->setString(strEffect);

        
    // 업그레이드 만능 포인트 버튼
    if ( _artiInfo->getDemandPoint() > 0 && _itemUpgradePointBtn != nullptr )
    {
        _itemUpgradePointBtn->setVisible(true);
        ((Sprite*)_itemUpgradePointBtn->getNormalImage())->setTexture("Assets/ui_common/btn_point.png");
        ((Sprite*)_itemUpgradePointBtn->getSelectedImage())->setTexture("Assets/ui_common/btn_point.png");
        _itemUpgradePointBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _itemUpgradePointBtn->setPosition(posLast);
        
        auto nowCurrency= ItemsMoneyManager::getInstance()->getPoint();
        auto needCurrency = _artiInfo->getDemandPoint();
        if ( nowCurrency < needCurrency )
        {
            ((Sprite*)_itemUpgradePointBtn->getNormalImage())->setTexture("Assets/ui_common/btn_point_off.png");
            ((Sprite*)_itemUpgradePointBtn->getSelectedImage())->setTexture("Assets/ui_common/btn_point_off.png");
        }
        
        //
        auto labelPoint = (Label*)_itemUpgradePointBtn->getChildByName("LABEL_POINT");
        if ( labelPoint != nullptr )
        {
            labelPoint->setString(MafUtils::toString(_artiInfo->getDemandPoint()));
        }
            
        posLast.x = posLast.x - _itemUpgradePointBtn->getContentSize().width - 5;
    }
        
        
    // 업그레이드 결제 버튼
    if( _itemUpgradeCashBtn != nullptr && ( _eArtiType != INFINITE_ARTIFACT_TYPE::NONE && _eArtiType != INFINITE_ARTIFACT_TYPE::END && _eArtiType != INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS))
    {
        int nBuyCount = UserDefault::getInstance()->getIntegerForKey(MafUtils::format("Infinite_pack_%d_%d",_artiInfo->getGrade(), (int)_artiInfo->getType()).c_str(),0);
        if ( nBuyCount == 1 || _artiInfo->getCurLv() + 3000 > _artiInfo->getMaxLv())
        {
            _itemUpgradeCashBtn->setVisible(false);
        }
        else
        {
            _itemUpgradeCashBtn->setVisible(true);
            _itemUpgradeCashBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            _itemUpgradeCashBtn->setPosition(posLast);
            
            posLast.x = posLast.x - _itemUpgradeCashBtn->getContentSize().width - 5;
            
            int nIdx = 0;
            if(_eArtiType == INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_ATK)
            {
                nIdx = E_CASH::CASH_ARTIFACT_INFINITE_3000_1;
            }
            else if(_eArtiType == INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CRI)
            {
                nIdx = E_CASH::CASH_ARTIFACT_INFINITE_3000_2;
            }
            else if(_eArtiType == INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST)
            {
                nIdx = E_CASH::CASH_ARTIFACT_INFINITE_3000_3;
            }
            else if(_eArtiType == INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_KILL)
            {
                nIdx = E_CASH::CASH_ARTIFACT_INFINITE_3000_4;
            }
            
            ((Sprite*)_itemUpgradeCashBtn->getNormalImage())->setTexture("Assets/ui/artifacts/pack_2.png");
            ((Sprite*)_itemUpgradeCashBtn->getSelectedImage())->setTexture("Assets/ui/artifacts/pack_2.png");
            _itemUpgradeCashBtn->setTag(nIdx);
            
            auto objProduct = CashManager::getInstance()->getProduct(nIdx);
            if ( objProduct != nullptr )
            {
                _labelUpgradeCashPrice->setString(objProduct->getPrice());
                
                std::string strPrice = objProduct->getPrice();
                if ( strPrice.length() == 0 )
                {
                    _labelUpgradeCashPrice->setString(GAME_TEXT("t_ui_shop_price_loading"));
                }
            }
            else
            {
                _itemUpgradeCashBtn->setVisible(false);
            }
        }
    }
    
}

void CellInfiniteArtifact::callbackCalculator()
{
    int level = _artiInfo->getCurLv();
    _levelEncrypt = MafAes256::XorEncrypt(level);
    
    drawUpgrade();
    
    if(_checkUpgradeListener != nullptr)
        _checkUpgradeListener();
    
    if( _onCompleteListener != nullptr )
    {
        _onCompleteListener();
    }
}

void CellInfiniteArtifact::onClick(cocos2d::Ref *sender)
{
    Node* item = (Node*)sender;
    
    
    if(item->getName().compare("UPGRADE_KEY") == 0)
    {
        onUpgrade(true);
    }
    else if(item->getName().compare("UPGRADE_POINT") == 0)
    {
        onUpgradeFromPoint(true);
    }
    else if(item->getName().compare("PURCHASE") == 0)
    {
        onUpgradeFromPurchase(item->getTag());
    }
    else if(item->getName().compare("CALCULATOR") == 0)
    {
        auto popup = PopupCalInfiniteArtifact::create(_artiInfo);
        popup->setCallback(CC_CALLBACK_0(CellInfiniteArtifact::callbackCalculator, this));
        popup->show();
    }
}

void CellInfiniteArtifact::onUpgrade(bool onCallback)
{
    int level = _artiInfo->getCurLv();
    if ( level >= _artiInfo->getMaxLv() )
    {
        return;
    }
    
    auto nowCurrency = ItemsMoneyManager::getInstance()->getKey();
    //string
    //auto needCurrency = InfiniteArtifactManager::getInfiniteArtiUpgradeKey(_artiInfo->getGrade(), level+1, _eArtiType);
    //double
    auto needCurrency = InfiniteArtifactManager::getInfiniteArtiUpgradeKey(_artiInfo->getGrade(), _artiInfo->getIncreaseA(), _artiInfo->getIncreaseB(), level+1, _eArtiType, true);
    
    std::string strKey = MafUtils::doubleToString(needCurrency);
    if ( MafUtils::compareStr(nowCurrency, strKey) == false )
    {
        _fUpgradeTime = 0;
        _fUpgradeTimeMax = 0.2;
        
        //
        drawUpgrade();
        
        //
        if ( _itemUpgradeBtn->isSelected() == false )
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::KEY);
            popup->show();
        }
        _itemUpgradeBtn->unselected();
        return;
    }
    
    level ++;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_10) &&_artiInfo->getType() !=  INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS)
    {
        if ( random(1, 100) <= 10 )
        {
            level += 1;
        }
    }
    
    if ( level >= _artiInfo->getMaxLv() )
    {
        level = _artiInfo->getMaxLv();
    }
        
    // 재화 감소
    ItemsMoneyManager::getInstance()->setKey("-" + strKey);
    
    // 레벨 적용
    InfiniteArtifactManager::getInstance()->setInfiniteArtifactLevel(_artiInfo, level);
    _levelEncrypt = MafAes256::XorEncrypt(level);
    
    
    //
    drawUpgrade();
    
    if( getInfo()->getType() == INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST )
    {
        RefreshManager::getInstance()->refreshUI(E_REFRESH::QUEST_INFO);
    }
    
    if(_checkUpgradeListener != nullptr)
        _checkUpgradeListener();
    
    //
    if (onCallback)
    {
        if ( _onCompleteListener != nullptr )
        {
            _onCompleteListener();
        }
    }
}

void CellInfiniteArtifact::onUpgradeFromPoint(bool onCallback)
{
    int level = _artiInfo->getCurLv();
    if ( level >= _artiInfo->getMaxLv() )
    {
        return;
    }
    
    int nowCurrency = ItemsMoneyManager::getInstance()->getPoint();
    int needCurrency = _artiInfo->getDemandPoint();
    if ( nowCurrency < needCurrency )
    {
        _fUpgradeTime = 0;
        _fUpgradeTimeMax = 0.2;
        
        //
        drawUpgrade();
        
        //
        if ( _itemUpgradePointBtn->isSelected() == false )
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::POINT);
            popup->show();
        }
        _itemUpgradePointBtn->unselected();
        return;
    }
    
    level ++;
    if (  level >= _artiInfo->getMaxLv() )
    {
        level = _artiInfo->getMaxLv();
    }
    
    // 재화 감소
    ItemsMoneyManager::getInstance()->setPoint(-needCurrency);

    // 레벨 적용
    InfiniteArtifactManager::getInstance()->setInfiniteArtifactLevel(_artiInfo, level);
    _levelEncrypt = MafAes256::XorEncrypt(level);
    
    drawUpgrade();
    
    if( getInfo()->getType() == INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST )
    {
        RefreshManager::getInstance()->refreshUI(E_REFRESH::QUEST_INFO);
    }
    
    if(_checkUpgradeListener != nullptr)
        _checkUpgradeListener();
    
    //
    if (onCallback)
    {
        if ( _onCompleteListener != nullptr )
        {
            _onCompleteListener();
        }
    }
}

void CellInfiniteArtifact::onUpgradeFromPurchase(int purIdx)
{
    if(_artiInfo == nullptr)
        return;

    SoundManager::SoundEffectStart(SOUND_UPGRADE);

    /*
    0 : 오류 없음
    1 : 다른상품 결제 오류 있음.
    2 : 같은상품 결제 오류 있음.
    */

    int nItemIdx = purIdx;
    CashManager::getInstance()->purchaseHas(nItemIdx, [=](int nHas){
        if ( nHas == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                PopupLoading::show();
                //초월보물 천원의 행복
                CashManager::getInstance()->purchaseInitiate(nItemIdx, CC_CALLBACK_4(CellInfiniteArtifact::ResultInAppPurchase, this));
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
            //초월보물 천원의 행복
            CashManager::getInstance()->purchaseInitiate(nItemIdx, CC_CALLBACK_4(CellInfiniteArtifact::ResultInAppPurchase, this));
        }
    });
}

void CellInfiniteArtifact::ResultInAppPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    PopupLoading::hide();
    
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        int nCount = UserDefault::getInstance()->getIntegerForKey(MafUtils::format("Infinite_pack_%d_%d",_artiInfo->getGrade(), (int)_artiInfo->getType()).c_str(),0);

        UserDefault::getInstance()->setIntegerForKey(MafUtils::format("Infinite_pack_%d_%d",_artiInfo->getGrade(), (int)_artiInfo->getType()).c_str(), nCount + 1);

        // redraw
        _artiInfo = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(_eArtiType);
        _levelEncrypt = MafAes256::XorEncrypt( _artiInfo->getCurLv() );
        
        //
        drawUpgrade();
        
        //
        if( getInfo()->getType() == INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST )
        {
            RefreshManager::getInstance()->refreshUI(E_REFRESH::QUEST_INFO);
        }

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


void CellInfiniteArtifact::upgradeInfiniteArtifact()
{
    _artiInfo = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(_eArtiType);
    _levelEncrypt = MafAes256::XorEncrypt(_artiInfo->getCurLv());
    scheduleUpdate();
    drawUpgrade();
    _lbGrade->setString(MafUtils::toString(_artiInfo->getGrade()));
}

void CellInfiniteArtifact::setCompleteListener(std::function<void()> listener)
{
    _onCompleteListener = listener;
}

void CellInfiniteArtifact::setCheckUpgradeListener(std::function<void()> listener)
{
    _checkUpgradeListener = listener;
}

INFINITE_ARTIFACT_TYPE CellInfiniteArtifact::getType()
{
    return _eArtiType;
}

InfiniteArtifactInfo* CellInfiniteArtifact::getInfo()
{
    return _artiInfo;
}

#pragma mark-
CellRelic::CellRelic(E_PLACE ePlace, E_RELIC eRelic)
{
    _onCompleteListener = nullptr;
    _ePlace = ePlace;
    _eRelic = eRelic;
    
    int level = 0;
    if ( UserInfoManager::getInstance()->getRelic(_ePlace, _eRelic) )
    {
        level = UserInfoManager::getInstance()->getRelicLevel(_ePlace, _eRelic);
    }
    _levelEncrypt = MafAes256::XorEncrypt(level);
    
    std::stringstream str;
    str << "Assets/icon_artifact/relic_" << _eRelic << ".png";
    
    _sprIcon->setTexture(str.str());
    _sprIcon->getTexture()->setTexParameters(texParams);
    _sprIcon->setScale(_itemStartBtn->getContentSize().width / _sprIcon->getContentSize().width);

    _itemUpgradeBtn->setCallback(CC_CALLBACK_1(CellRelic::onAskPopup, this));
    _itemStartBtn->setCallback(CC_CALLBACK_1(CellRelic::onDetailView, this));
    
    auto spriteMagnifier = Sprite::create("Assets/ui/review/icon_magnifier.png");
    spriteMagnifier->setPosition( Vec2(_itemStartBtn->getContentSize().width - 15, 15) );
    _itemStartBtn->addChild(spriteMagnifier);
    
    _sprUpgradeIcon->setTexture("Assets/icon/icon_jewel.png");
    _btnSet->setUpgradeLabelPositionCenter();

    //Add Start
    _lbTitle->setTTFConfig(TTFConfig(GAME_FONT, 26, GlyphCollection::DYNAMIC));
    _lbTitle->setColor(Color3B(222, 106, 255));
    _lbTitle->setString(DataManager::GetRelicName(_eRelic));
    _lbLv->setVisible(true);
    
    _lbMoney->setTTFConfig(TTFConfig(GAME_FONT, 22, GlyphCollection::DYNAMIC));
    _lbMoney->setColor(Color3B::WHITE);
    _lbMoney->setPosition(Vec2(_lbTitle->getPosition().x, _listBg->getContentSize().height*0.025));
    _lbMoney->setAnchorPoint(Vec2(0,0));
    _lbMoney->setAlignment(TextHAlignment::LEFT);
    
    _lbExp = Label::createWithTTF("Exp", GAME_FONT, 22);
    _lbExp->setColor(Color3B::WHITE);
    _lbExp->setAlignment(TextHAlignment::LEFT);
    _lbExp->setAnchorPoint(Vec2(0,0));
    _lbExp->setPosition(Vec2(_lbTitle->getPosition().x, listSize.height*0.2 +2));
    _lbExp->setDimensions(_listBg->getContentSize().width - 116 - 15, _lbExp->getContentSize().height);
    _lbExp->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    _lbExp->setOverflow(Label::Overflow::SHRINK);
    _listBg->addChild(_lbExp);
    //Add End
    
    drawUpgrade();
    
    Director::getInstance()->getScheduler()->schedule(schedule_selector(CellRelic::update),
                                                      this, 1.0 , kRepeatForever, 0, false);

}

CellRelic::~CellRelic(){}


void CellRelic::update(float dt)
{
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    if ( level != UserInfoManager::getInstance()->getRelicLevel(_ePlace, _eRelic) )
    {
        level = UserInfoManager::getInstance()->getRelicLevel(_ePlace, _eRelic);
        _levelEncrypt = MafAes256::XorEncrypt(level);
        
        drawUpgrade();
    }
    
    if ( level >= DataManager::GetRelicMaxLevel(_eRelic) )
    {
        _itemUpgradeBtn->setVisible(false);
        
        //
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(CellRelic::update), this);
        return;
    }

    if ( level == 0 )
    { //무료제공한다.
        if ( UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= DataManager::GetRelicOpenFloor(_eRelic) )
        {
            _itemUpgradeBtn->setVisible(true);
            ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2.png");
            ((Sprite*)_itemUpgradeBtn->getSelectedImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2.png");
        }
        else
        {
            _itemUpgradeBtn->setVisible(false);
        }
    }
    else
    {
        _itemUpgradeBtn->setVisible(true);
        ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2.png");
        ((Sprite*)_itemUpgradeBtn->getSelectedImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2.png");
        
        auto need = DataManager::GetRelicUpgradeGem(_eRelic, level);
        auto gem= ItemsMoneyManager::getInstance()->getGem();
        if ( gem < need )
        {
            ((Sprite*)_itemUpgradeBtn->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
            ((Sprite*)_itemUpgradeBtn->getSelectedImage())->setTexture("Assets/ui_common/btn_upgrade_bg_2_max.png");
        }
    }
}

void CellRelic::onDetailView(cocos2d::Ref* pSender)
{
    auto popup = PopupDefault::create(DataManager::GetRelicName(_eRelic), GAME_TEXTFORMAT("t_ui_relic_msg_2", DataManager::GetRelicOpenFloor(_eRelic)-1));
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->show();
}

void CellRelic::onAskPopup(cocos2d::Ref* pSender)
{
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    if ( level == 0 )
    { //무료제공한다.
        if(UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= DataManager::GetRelicOpenFloor(_eRelic))
        {
            SoundManager::SoundEffectStart(SOUND_UPGRADE);
            
            level++;
            if ( level>=DataManager::GetRelicMaxLevel(_eRelic) )
                level = DataManager::GetRelicMaxLevel(_eRelic);
            
            //
            UserInfoManager::getInstance()->setRelicLevel(_ePlace, _eRelic, level);
            _levelEncrypt = MafAes256::XorEncrypt(level);
            
            drawUpgrade();
        }
    }
    else
    {
        auto gem = ItemsMoneyManager::getInstance()->getGem();
        auto need = DataManager::GetRelicUpgradeGem(_eRelic, level);
        if ( gem >= need )
        {
            std::string strTitle = DataManager::GetRelicName(_eRelic);
            std::string strMessage = GAME_TEXT("t_ui_msg_buy");
            strMessage.append(MafUtils::format("\n(Lv.%d -> Lv.%d)", MafAes256::XorDecryptInt(_levelEncrypt), level + 1));

            auto popup = PopupDefault::create(strTitle, strMessage);
            popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
            popup->addButton(GAME_TEXT("t_ui_yes"), false, CC_CALLBACK_1(CellRelic::onUpgrade, this));
            popup->show();
        }
        else
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
            popup->show();
        }
    }
}
void CellRelic::onUpgrade(cocos2d::Ref* pSender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    if ( level >= DataManager::GetRelicMaxLevel(_eRelic) )
        return;
    
    auto gem = ItemsMoneyManager::getInstance()->getGem();
    auto need = DataManager::GetRelicUpgradeGem(_eRelic, level);
    if ( gem >= need )
    {
        level ++;
        if ( level>=DataManager::GetRelicMaxLevel(_eRelic) )
            level = DataManager::GetRelicMaxLevel(_eRelic);
        
        //artifact 저장 필요.
        UserInfoManager::getInstance()->setRelicLevel(_ePlace, _eRelic, level);
        _levelEncrypt = MafAes256::XorEncrypt(level);
        
        ItemsMoneyManager::getInstance()->setGem(-need);
        
        drawUpgrade();
        
        if(_onCompleteListener!=nullptr)
            _onCompleteListener();
    }
}

void CellRelic::drawUpgrade()
{
    int level = MafAes256::XorDecryptInt(_levelEncrypt);
    double effect = DataManager::GetRelicEffect(_eRelic, level);
    double effectNext = DataManager::GetRelicEffect(_eRelic, level+1);
    effectNext -= effect;
    
    if(_eRelic == E_RELIC::RELIC_1){
        _lbMoney->setString(GAME_TEXTFORMAT("t_relic_option_1", level));
//        _lbUpgrade_top->setString(MafUtils::format("+ 1"));
    }else if(_eRelic == E_RELIC::RELIC_2){
        std::string strValue = MafUtils::format("%.0f",effect);
        std::string commaStr = MafUtils::addCommaToString(strValue, 3, ',');
        _lbMoney->setString( GAME_TEXTFORMAT("t_relic_option_2", commaStr.c_str()) );
//        std::string strUpgradeValue = MafUtils::doubleToString(effectNext);
//        MafUtils::convertNumberToShort(strUpgradeValue);
//        _lbUpgrade_top->setString(MafUtils::format("+ %s%%", strUpgradeValue.c_str()));
    }else if(_eRelic == E_RELIC::RELIC_6){
        std::string strValue = MafUtils::format("%.0f",effect);
        std::string commaStr = MafUtils::addCommaToString(strValue, 3, ',');
        _lbMoney->setString( GAME_TEXTFORMAT("t_relic_option_6", commaStr.c_str()) );
//        std::string strUpgradeValue = MafUtils::doubleToString(effectNext);
//        MafUtils::convertNumberToShort(strUpgradeValue);
//        _lbUpgrade_top->setString(MafUtils::format("+ %s%%", strUpgradeValue.c_str()));
    }else if(_eRelic == E_RELIC::RELIC_7){
        std::string strValue = MafUtils::format("%.0f",effect);
        std::string commaStr = MafUtils::addCommaToString(strValue, 3, ',');
        _lbMoney->setString( GAME_TEXTFORMAT("t_relic_option_7", commaStr.c_str()) );
//        std::string strUpgradeValue = MafUtils::doubleToString(effectNext);
//        MafUtils::convertNumberToShort(strUpgradeValue);
//        _lbUpgrade_top->setString(MafUtils::format("+ %s%%", strUpgradeValue.c_str()));
    }else if(_eRelic == E_RELIC::RELIC_8){
        std::string strValue = MafUtils::format("%.0f",effect);
        std::string commaStr = MafUtils::addCommaToString(strValue, 3, ',');
        _lbMoney->setString( GAME_TEXTFORMAT("t_relic_option_8", commaStr.c_str()) );
//        std::string strUpgradeValue = MafUtils::doubleToString(effectNext);
//        MafUtils::convertNumberToShort(strUpgradeValue);
//        _lbUpgrade_top->setString(MafUtils::format("+ %s%%", strUpgradeValue.c_str()));
    }else if(_eRelic == E_RELIC::RELIC_9){
        std::string strValue = MafUtils::format("%.0f",effect);
        std::string commaStr = MafUtils::addCommaToString(strValue, 3, ',');
        _lbMoney->setString( GAME_TEXTFORMAT("t_relic_option_9", commaStr.c_str()) );
//        std::string strUpgradeValue = MafUtils::doubleToString(effectNext);
//        MafUtils::convertNumberToShort(strUpgradeValue);
//        _lbUpgrade_top->setString(MafUtils::format("+ %s", strUpgradeValue.c_str()));
    }else if(_eRelic == E_RELIC::RELIC_4){
        std::string strValue = MafUtils::format("%.0f",effect);
        std::string commaStr = MafUtils::addCommaToString(strValue, 3, ',');
        _lbMoney->setString( GAME_TEXTFORMAT("t_relic_option_4", commaStr.c_str()) );
    }else{
        std::string strValue = MafUtils::format("%.0f",effect);
        std::string commaStr = MafUtils::addCommaToString(strValue, 3, ',');
        _lbMoney->setString( GAME_TEXTFORMAT("t_relic_option_2", commaStr.c_str()) );
//        std::string strUpgradeValue = MafUtils::doubleToString(effectNext);
//        MafUtils::convertNumberToShort(strUpgradeValue);
//        _lbUpgrade_top->setString(MafUtils::format("+ %s%%", strUpgradeValue.c_str()));
    }
    
    if ( level >= DataManager::GetRelicMaxLevel(_eRelic) )
    {
        _itemUpgradeBtn->setVisible(false);
        _itemUpgradeBtn->setEnabled(false);
        
        // 고대유물 설명 사이즈 재설정
        _lbExp->setDimensions(_listBg->getContentSize().width - 116 - 15, _lbExp->getContentSize().height);
    }
    else
    {
        _lbUpgrade->setString(MafUtils::format("%d", DataManager::GetRelicUpgradeGem(_eRelic, level)));
        
        // 고대유물 설명 사이즈 재설정
        _lbExp->setDimensions(_listBg->getContentSize().width - 116 - 150 - 15, _lbExp->getContentSize().height);
    }
    
    if ( level >= DataManager::GetRelicMaxLevel(_eRelic) )
    {
        _lbLv->setString(MafUtils::format("LV.%d(Max)", level));
    }
    else
    {
        _lbLv->setString(MafUtils::format("LV.%d(Max%d)", level,DataManager::GetRelicMaxLevel(_eRelic)));
    }
    _lbExp->setString(DataManager::GetRelicExplain(_eRelic));
    
    if ( level == 0 )
    {
        if(UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= DataManager::GetRelicOpenFloor(_eRelic)){
//            _lbUpgrade_top->setTTFConfig(TTFConfig(GAME_FONT, 25, GlyphCollection::DYNAMIC));
//            _lbUpgrade_top->setString(GAME_TEXT("t_ui_relic_free"));
//            _itemUpgradeBtn->setEnabled(true);
//            _sprUpgradeIcon->setVisible(false);
//            _lbUpgrade->setVisible(false);
            _lbUpgrade->setString("FREE");
        }
//        else{
//            _lbUpgrade_top->setTTFConfig(TTFConfig(GAME_FONT, 25, GlyphCollection::DYNAMIC));
//            _lbUpgrade_top->setString( GAME_TEXTFORMAT("t_ui_relic_msg_1", DataManager::GetRelicOpenFloor(_eRelic)-1) );
//
//            _itemUpgradeBtn->setEnabled(false);
//            _sprUpgradeIcon->setVisible(false);
//            _lbUpgrade->setVisible(false);
//        }
        
        _lbUpgrade_top->setPosition(Vec2(_itemUpgradeBtn->getContentSize().width*0.5,_itemUpgradeBtn->getContentSize().height*0.5));
        _lbUpgrade_top->setAnchorPoint(Vec2(0.5,0.5));
    }
    else
    {
        _itemUpgradeBtn->setEnabled(true);
//        _sprUpgradeIcon->setVisible(true);
        _lbUpgrade->setVisible(true);
        
        _lbUpgrade_top->setPosition(Vec2(_itemUpgradeBtn->getContentSize().width*0.3,
                                         _itemUpgradeBtn->getContentSize().height*0.8));
        _lbUpgrade_top->setAnchorPoint(Vec2(0,1));
    }
    update(0);
}

#pragma mark-
CellDungeon::CellDungeon(E_PLACE ePlace, E_DUNGEON eDungeon)
{
    _isSceneMove = false;
    _ePlace = ePlace;
    _eDungeon = eDungeon;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    setContentSize(Size(visibleSize.width,150));
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    addChild(menu);
    
    
    auto spriteBg = ui::Scale9Sprite::create(PIXEL, Rect(0,0,getContentSize().width * 0.98, 142));
    spriteBg->setColor(Color3B(29,41,51));
    {
        std::string strImagePath = "";
        std::string strNameKey = "";

        Vector<InfoItems*> listItems;
        
        
        switch (_eDungeon) {
            case E_DUNGEON::D_NEW_RAID:
            {
                strImagePath = "Assets/ui/dungeoninput/btn_dragon_slayer.png";
                strNameKey = "t_ui_new_raid_title";
                
                auto item = ItemsManager::getInstance()->getInfoItems(E_ITEMS::WEAPON_SPIRIT);
                item->setTypeParams(1);
                
                listItems.pushBack(item);
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::NEW_RAID_DRAGON_BALL));
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::SPIRIT_STON));
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::MATERIAL));
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::HONOR));
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::POINT));
            }
                break;
            case E_DUNGEON::D_PRISON:
            {
                strImagePath = "Assets/ui/dungeoninput/btn_move_prison.png";
                strNameKey = "t_ui_prison";
                
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::COSTUME_COIN));
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::KEY));
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::POINT));
            }
                break;
            case E_DUNGEON::D_FLASHMAN:
            {
                strImagePath = "Assets/ui/dungeoninput/btn_flashman.png";
                strNameKey = "t_ui_minigame_title_1";
                
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::LOOT_PIECE_1));
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::LOOT_PIECE_2));
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::LOOT_PIECE_3));
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::LOOT_PIECE_4));
            }
                break;
            case E_DUNGEON::D_PRISON_FROST:
            {
                strImagePath = "Assets/ui/dungeoninput/btn_frost_prison.png";
                strNameKey = "t_ui_frost_prison_title";
                
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::FROST_PRISON_POINT));
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::NEW_RAID_DRAGON_BALL));
            }
                break;
            case E_DUNGEON::D_DUNGEON_NORMAL:
            {
                strImagePath = "Assets/ui/dungeoninput/btn_move_normal.png";
                strNameKey = "t_ui_dungeon_normal";
                
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::KEY));
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::GEM));
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::POINT));
            }
                break;
            case E_DUNGEON::D_DUNGEON_DEVIL:
            {
                strImagePath = "Assets/ui/dungeoninput/btn_move_devil.png";
                strNameKey = "t_ui_dungeon_devil";
                
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::GEM));
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::POINT));
            }
                break;
            case E_DUNGEON::D_ADVENTURE:
            {
                strImagePath = "Assets/ui/dungeoninput/btn_adventure.png";
                strNameKey = "t_ui_advt_info_title_1";
                
                
                auto itemCore = ItemsManager::getInstance()->getInfoItems(E_ITEMS::ADVENTURE_CORE);
                itemCore->setTypeParams(5);
                listItems.pushBack(itemCore);
                auto itemRelic = ItemsManager::getInstance()->getInfoItems(E_ITEMS::ADVENTURE_RELIC);
                itemRelic->setTypeParams(0);
                listItems.pushBack(itemRelic);
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::MINE_BOX));
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::GEM));
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::POINT));
            }
                break;
            case E_DUNGEON::D_DEFENSE:
            {
                strImagePath = "Assets/ui/dungeoninput/btn_move_defense.png";
                strNameKey = "t_ui_defense_title_1";
                
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::DEFENSE_COIN));
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::DEFENSE_CHARACTER));
                listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::DEFENSE_WEAPON));
                
                auto manaAMP = InfoItems::create(E_ITEMS::NONE);
                manaAMP->setPath("Assets/icon/icon_defense_power_amplifier.png");
                listItems.pushBack(manaAMP);
            }
                break;
        }
        
        auto spriteBgIn = Sprite::create(strImagePath);
        spriteBgIn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        spriteBgIn->setPosition(0, spriteBg->getContentSize().height / 2);
        spriteBg->addChild(spriteBgIn);
        
        auto labelTitle = Label::createWithTTF(GAME_TEXT(strNameKey), GAME_FONT, 34);
        labelTitle->setDimensions(spriteBg->getContentSize().width - 280, spriteBg->getContentSize().height/2);
        labelTitle->setOverflow(Label::Overflow::SHRINK);
        labelTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::BOTTOM);
        labelTitle->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
        labelTitle->setPosition(150, spriteBg->getContentSize().height / 2);
        labelTitle->enableBold();
        spriteBg->addChild(labelTitle);
        
        
        
        
        for ( int i = 0; i < listItems.size(); i++ )
        {
            auto objReward = listItems.at(i);
            std::string strPath = objReward->getPath();
            if ( strPath.length() != 0 )
            {
                auto spriteRewardBox = ui::Scale9Sprite::create("Assets/ui_common/box_item.png", Rect::ZERO, Rect(5,30,10,10));
                spriteRewardBox->setContentSize(Size(62, 62));
                spriteRewardBox->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
                spriteRewardBox->setPosition(150 + i * spriteRewardBox->getContentSize().width + i * 5, spriteBg->getContentSize().height / 2);
                spriteBg->addChild(spriteRewardBox);
                
                auto spriteReward = Sprite::create(strPath);
                spriteReward->setPosition(spriteRewardBox->getBoundingBox().size.width / 2, spriteRewardBox->getBoundingBox().size.height / 2);
                spriteRewardBox->addChild(spriteReward);
            }
        }
        
        //
        auto spriteInput = Sprite::create("Assets/ui/dungeoninput/btn_move.png");
        spriteInput->setPosition(spriteBg->getContentSize().width * 0.87, spriteBg->getContentSize().height / 2);
        spriteBg->addChild(spriteInput);
        
        //
        if (_eDungeon == D_NEW_RAID)
        {
            auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
            uiReddot->setPosition(20, spriteBg->getContentSize().height - 20);
            uiReddot->setVisible(false);
            spriteBg->addChild(uiReddot);
            
            auto action1 = CallFuncN::create([=](Ref* sender){
                auto uiReddot = (UIReddot*)sender;
                
                bool isActivate = false;
                
                if((NewRaidManager::getInstance()->getAttackCount() >= 1 ||
                   NewRaidManager::getInstance()->isLevelUpPossible() ||
                   NewRaidManager::getInstance()->isMissionComplete()) && NewRaidManager::getInstance()->isRaidEnterCondition())
                {
                    isActivate = true;
                }
                
                uiReddot->setVisible(isActivate);
            });
            auto action2 = DelayTime::create(2);
            auto seq = Sequence::create(action1, action2, NULL);
            uiReddot->runAction(RepeatForever::create(seq));
            
        }
        else if( _eDungeon == D_PRISON )
        {
            double nPosTempX = 400;
            double nPosTempY = spriteBg->getContentSize().height / 2;
            
            //
            {
                auto spriteIcon = Sprite::create("Assets/icon/icon_freepass_2.png");
                spriteIcon->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
                spriteIcon->setPosition(nPosTempX, nPosTempY);
                spriteIcon->setVisible(false);
                spriteBg->addChild(spriteIcon);
                {
                    auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
                    uiReddot->setPosition(-5, spriteIcon->getContentSize().height);
                    spriteIcon->addChild(uiReddot);
                    
                    auto action1 = CallFuncN::create([=](Node* sender){
                        
                        if ( PrisonManager::getInstance()->getHeart() >= PrisonManager::getInstance()->getHeartMax() )
                        {
                            sender->setVisible(true);
                        }
                        else
                        {
                            sender->setVisible(false);
                        }
                        
                    });
                    auto action2 = DelayTime::create(1.0);
                    auto seq = Sequence::create(action1, action2, NULL);
                    spriteIcon->runAction(RepeatForever::create(seq));
                }
                nPosTempX += spriteIcon->getContentSize().width + 5;
            }
            
            //
            {
                auto spriteIcon = Sprite::create("Assets/icon/icon_jelly58.png");
                spriteIcon->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
                spriteIcon->setPosition(nPosTempX, nPosTempY);
                spriteIcon->setVisible(false);
                spriteBg->addChild(spriteIcon);
                {
                    auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
                    uiReddot->setPosition(-5, spriteIcon->getContentSize().height);
                    spriteIcon->addChild(uiReddot);
                    
                    auto action1 = CallFuncN::create([=](Node* sender){
                        
                        if ( PrisonManager::getInstance()->isJellyRewardMax() == true )
                        {
                            sender->setVisible(true);
                        }
                        else
                        {
                            sender->setVisible(false);
                        }
                        
                    });
                    auto action2 = DelayTime::create(1.0);
                    auto seq = Sequence::create(action1, action2, NULL);
                    spriteIcon->runAction(RepeatForever::create(seq));
                }
                nPosTempX += spriteIcon->getContentSize().width + 10;
            }
            if ( EventManager::getInstance()->isActivatedEvent(10) )
            {
                auto uiReddot = UIReddot::create(E_REDDOT_TYPE::EVENT, Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiReddot->setPosition(spriteBg->getContentSize().width * 0.87, spriteBg->getContentSize().height - 30);
                spriteBg->addChild(uiReddot);
            }
        }
        else if (_eDungeon == D_ADVENTURE)
        {
            auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
            uiReddot->setPosition(20, spriteBg->getContentSize().height - 20);
            uiReddot->setVisible(false);
            spriteBg->addChild(uiReddot);
            
            auto action1 = CallFuncN::create([=](Ref* sender){
                auto uiReddot = (UIReddot*)sender;
                
                bool isActivate = AdventureManager::getInstance()->isAdventureComplete();
                uiReddot->setVisible(isActivate);
            });
            auto action2 = DelayTime::create(2);
            auto seq = Sequence::create(action1, action2, NULL);
            uiReddot->runAction(RepeatForever::create(seq));
            
        }
        else if (_eDungeon == D_DEFENSE)
        {
            auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
            uiReddot->setPosition(20, spriteBg->getContentSize().height - 20);
            uiReddot->setVisible(false);
            spriteBg->addChild(uiReddot);
            
            auto action1 = CallFuncN::create([=](Ref* sender){
                auto uiReddot = (UIReddot*)sender;
                
                bool isActivate = false;
                
                if(DefenseManager::getInstance()->isReddotCondition() || DefenseManaAMPManager::getInstance()->isReddotCondition())
                {
                    isActivate = true;
                }
                
                uiReddot->setVisible(isActivate);
            });
            auto action2 = DelayTime::create(2);
            auto seq = Sequence::create(action1, action2, NULL);
            uiReddot->runAction(RepeatForever::create(seq));
            
        }
    }
    
    //
    _item = MafNode::MafMenuItemSprite::create(spriteBg,  nullptr, CC_CALLBACK_1(CellDungeon::onClickMenu, this));
    _item->setPosition(Vec2(getContentSize().width/2,getContentSize().height/2));
    _item->setTag(_eDungeon);
    menu->addChild(_item);
    
    
}

CellDungeon::~CellDungeon()
{
    
}

void CellDungeon::onClickMenu(cocos2d::Ref* sender)
{
    if ( AccountManager::getInstance()->getUserIdx() == 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto item = (MenuItem*)sender;
    if ( item->getTag() == E_DUNGEON::D_PRISON )
    {
        auto popup = PopupPrison::create();
        popup->show();
    }
    else if ( item->getTag() == E_DUNGEON::D_PRISON_FROST )
    {
        if ( PrisonFrostManager::getInstance()->getInfoSetting("frost_prison_switch")->getValueToBool() == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_error_2"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
            
        if ( WeaponManager::getInstance()->getEquipHigh() >= E_WEAPON::WEAPON_60 )
        {
            auto popup = PopupFrost::create();
            popup->show();
        }
        else
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_error_1"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
                        
        }
    }
    else if ( item->getTag() == E_DUNGEON::D_FLASHMAN )
    {
        if ( FlashmanManager::getInstance()->getInfoSetting("minigame_switch")->getValueToBool() == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_error_2"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
        bool bEnter = false;
        
        int nRevival = UserInfoManager::getInstance()->getRevival(E_PLACE::DG_NORMAL);
        if ( nRevival > 1 )
        {
            bEnter = true;
        }
        
        if ( UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= 1000 || UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL) >= 1000 )
        {
            bEnter = true;
        }
            
        if ( bEnter == true )
        {
            auto popup = PopupFlashman::create();
            popup->show();
        }
        else
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_minigame_condition_1"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
                        
        }
    }
    else if ( item->getTag() == E_DUNGEON::D_PRISON_FROST )
    {
        if ( PrisonFrostManager::getInstance()->getInfoSetting("frost_prison_switch")->getValueToBool() == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_error_2"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
            
        if ( WeaponManager::getInstance()->getEquipHigh() >= E_WEAPON::WEAPON_60 )
        {
            auto popup = PopupFrost::create();
            popup->show();
        }
        else
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_error_1"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
                        
        }
    }
    else if ( item->getTag() == E_DUNGEON::D_DUNGEON_NORMAL )
    {
        if(_isSceneMove)
            return;
        
        //
        bool bPlay = PrisonManager::getInstance()->isDungeonPlay();
        int nEnterAutoCount = PrisonManager::getInstance()->getEnterAutoCount();
        if ( bPlay == true || nEnterAutoCount > 0 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_close_check"));
            popup->addButton(GAME_TEXT("t_ui_cancel"), true, nullptr);
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                
                item->setEnabled(false);
                UtilsScene::getInstance()->sceneReplace(UtilsScene::eType::scene_dungeon_normal);
                
                //
                _isSceneMove = true;
                
            });
            popup->show();
        }
        else
        {
            item->setEnabled(false);
            UtilsScene::getInstance()->sceneReplace(UtilsScene::eType::scene_dungeon_normal);
            
            //
            _isSceneMove = true;
        }
    }
    else if ( item->getTag() == E_DUNGEON::D_DUNGEON_DEVIL )
    {
        if ( UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) < DEVIL_JOIN_FLOOR )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_dungeon_devil"), GAME_TEXT("t_ui_error_28"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
        if(_isSceneMove)
            return;
        
        //
        SaveManager::saveAllData();
        
        //
        bool bPlay = PrisonManager::getInstance()->isDungeonPlay();
        int nEnterAutoCount = PrisonManager::getInstance()->getEnterAutoCount();
        if ( bPlay == true || nEnterAutoCount > 0 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_close_check"));
            popup->addButton(GAME_TEXT("t_ui_cancel"), true, nullptr);
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                
                item->setEnabled(false);
                UtilsScene::getInstance()->sceneReplace(UtilsScene::eType::scene_dungeon_devil);
                
                //
                _isSceneMove = true;
                
            });
            popup->show();
        }
        else
        {
            item->setEnabled(false);
            UtilsScene::getInstance()->sceneReplace(UtilsScene::eType::scene_dungeon_devil);
            
            //
            _isSceneMove = true;
        }
    }
    else if ( item->getTag() == E_DUNGEON::D_NEW_RAID )
    {
        bool bEnter = false;
        
        int nRevival = UserInfoManager::getInstance()->getRevival(E_PLACE::DG_NORMAL);
        if ( nRevival > 1 )
        {
            bEnter = true;
        }
        
        if ( UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= 1000 || UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL) >= 1000 )
        {
            bEnter = true;
        }
        
        if ( bEnter == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_new_raid_title"), GAME_TEXT("t_ui_new_raid_condition_1"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
        auto popup = PopupRaidDefault::create();
        popup->show();
    }
    else if (item->getTag() == E_DUNGEON::D_ADVENTURE )
    {
        bool bEnter = true;
        if ( WeaponManager::getInstance()->getEquipHigh() < E_WEAPON::WEAPON_76_MINE)
        {
            bEnter = false;
        }
        
        if ( bEnter == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_advt_info_title_1"), GAME_TEXT("t_ui_error_12"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
        auto popup = PopupAdventureGuild::create();
        popup->show();
    }
    else if ( item->getTag() == E_DUNGEON::D_DEFENSE )
    {
        bool bEnter = false;
        
        if (UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= 10000)
        {
            bEnter = true;
        }
        
        if ( bEnter == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_new_raid_title"), GAME_TEXT("t_ui_new_raid_condition_1"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
        if(!DefenseManager::getInstance()->isOpenContentsNow())
        {
            auto popup = PopupDefenseDefault::create();
            popup->show();
        }
    }
}
void CellDungeon::redrawEventTime()
{
    auto christmasEvent = EventManager::getInstance()->getActivatedEvent(5);
    if(christmasEvent == nullptr)
    {
        christmasEvent = EventManager::getInstance()->getActivatedEvent(8);
    }
}

int CellDungeon::getDungeonTag()
{
    return _item->getTag();
}
