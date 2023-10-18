//
//  LayerNormalWeapon.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "LayerNormalWeapon.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoWeapon.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUI/UICell/UILayoutWeaponCell.h"
#include "GameUIPopup/Other/BonsikDex/PopupBonsikDexDefault.h"
#include "GameUIPopup/Other/WeaponSpirit/PopupListWeaponSpirit.h"
#include "GameUIPopup/Other/Equip/PopupWeaponCraft.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/TutorialManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/WeaponManager.h"
#include "ManagerGame/WeaponSpiritManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/AdventureRelicManager.h"
#include "ManagerGame/SpecialMissionManager.h"

USING_NS_CC;

LayerNormalWeapon* LayerNormalWeapon::create()
{
    LayerNormalWeapon* pRet = new(std::nothrow) LayerNormalWeapon();
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

LayerNormalWeapon::LayerNormalWeapon(void) :
_bInitUI(false),

_ePlace(E_PLACE::EMPTY),

_dps("0"),

// widget
_uiContainerInfo(nullptr),
_uiContainerList(nullptr),

_uiDictionary(nullptr),
_uiDPS(nullptr),
_uiDexBonus(nullptr)

{
    
}

LayerNormalWeapon::~LayerNormalWeapon(void)
{
    
}

bool LayerNormalWeapon::init()
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    _ePlace = E_PLACE::DG_NORMAL;
    
    
    
    return true;
}

void LayerNormalWeapon::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        // init
        initVar();
        initUi();

        //
        RefreshManager::getInstance()->addUI(E_REFRESH::TUTORIAL, this);
        
        RefreshManager::getInstance()->addUI(E_REFRESH::WEAPON_INFO, this);
        RefreshManager::getInstance()->addUI(E_REFRESH::WEAPON_BUY, this);
        RefreshManager::getInstance()->addUI(E_REFRESH::WEAPON_BUY_REINFORCE, this);
        RefreshManager::getInstance()->addUI(E_REFRESH::WEAPON_BUY_PERMANENT, this);
        
        //
        this->schedule(schedule_selector(LayerNormalWeapon::scheduleDPS), 2.0, kRepeatForever, 0);
        
        //
        _bInitUI = true;
    }
    
    //
    drawInfo();
}

#pragma mark - refresh
void LayerNormalWeapon::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::TUTORIAL )
    {
        auto progress = TutorialManager::getInstance()->getTutorialProgress();
        if ( progress.first == PROGRESS_WEAPON )
        {
            if ( SpecialMissionManager::getInstance()->getSpecialMissionClear() == 91 )
            {
                if ( progress.second == SUB_OBJECT_1 && _uiDictionary != nullptr )
                {
                    TutorialManager::getInstance()->attachEffect(EFFECT::EFFECT_FINGER, progress, _uiDictionary);
                }
            }
        }
    }
    else if ( eType == E_REFRESH::WEAPON_BUY_PERMANENT || eType == E_REFRESH::WEAPON_BUY_REINFORCE )
    {
        drawInfo();
    }
}
  

#pragma mark - init
void LayerNormalWeapon::initVar()
{
    
}

void LayerNormalWeapon::initUi()
{
    //
    uiContainer();
    uiInfo();
}

#pragma mark - ui : defalut
void LayerNormalWeapon::uiContainer()
{
    Size size = this->getContentSize();
    
    _uiContainerInfo = ui::Layout::create();
    _uiContainerInfo->setContentSize(Size(size.width, 120));
    _uiContainerInfo->setIgnoreAnchorPointForPosition(false);
    _uiContainerInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContainerInfo->setPosition(Vec2(size.width / 2, size.height));
    addChild(_uiContainerInfo);
    
    _uiContainerList = UILayoutWeaponCell::create();
    _uiContainerList->setContentSize(Size(size.width, size.height - _uiContainerInfo->getContentSize().height));
    _uiContainerList->setIgnoreAnchorPointForPosition(false);
    _uiContainerList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContainerList->setPosition(Vec2(size.width / 2, 0));
    addChild(_uiContainerList);
}

void LayerNormalWeapon::uiInfo()
{
    auto layout = _uiContainerInfo;
    Size size = layout->getContentSize();
    
    auto uiUpgradeMax = ui::Button::create("Assets/ui_common/tab_bar_2_1.png");
    uiUpgradeMax->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiUpgradeMax->setPosition(Vec2(size.width / 2, size.height - 5));
    uiUpgradeMax->addClickEventListener(CC_CALLBACK_1(LayerNormalWeapon::onClickUpgradeMax, this));
    layout->addChild(uiUpgradeMax);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_equip_msg_4"), GAME_FONT, 24);
        uiText->setPosition(Vec2(uiUpgradeMax->getContentSize().width / 2, uiUpgradeMax->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(uiUpgradeMax->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setColor(Color3B(255, 174, 0));
        uiUpgradeMax->addChild(uiText);
    }
    
    auto uiSpirit = ui::Button::create("Assets/ui_common/tab_bar_2_2.png");
    uiSpirit->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    uiSpirit->setPosition(Vec2(size.width / 2 - uiUpgradeMax->getContentSize().width / 2, size.height - 5));
    uiSpirit->addClickEventListener(CC_CALLBACK_1(LayerNormalWeapon::onClickSpirit, this));
    layout->addChild(uiSpirit);
    {
        auto uiIcon = ui::ImageView::create("Assets/ui/spirit/icon_spirit_0001.png");
        uiIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiIcon->setPosition(Vec2(-20, uiSpirit->getContentSize().height / 2));
        uiIcon->setScale(1.5);
        ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiSpirit->addChild(uiIcon);
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_weapon_spirit_1"), GAME_FONT, 24);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiText->setPosition(Vec2(60, uiSpirit->getContentSize().height/2));
        uiText->setTextAreaSize(Size(130, uiText->getContentSize().height));
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiSpirit->addChild(uiText);

        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
        uiReddot->setPosition(15, uiSpirit->getContentSize().height - 15);
        uiSpirit->addChild(uiReddot);
        uiReddot->setVisible(false);

        auto action1 = CallFuncN::create([=](Ref* sender){

            auto uiReddot = (UIReddot*)sender;

            if ( WeaponSpiritManager::getInstance()->isReddotCondition() )
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
    
    _uiDictionary = ui::Button::create("Assets/ui_common/tab_bar_2_2.png");
    _uiDictionary->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _uiDictionary->setPosition(Vec2(size.width / 2 + uiUpgradeMax->getContentSize().width / 2, size.height - 5));
    _uiDictionary->addClickEventListener(CC_CALLBACK_1(LayerNormalWeapon::onClickDictionary, this));
    layout->addChild(_uiDictionary);
    {
        auto uiIcon = ui::ImageView::create("Assets/ui/bonsikdex/icon_book.png");
        uiIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiIcon->setPosition(Vec2(_uiDictionary->getContentSize().width - 20, _uiDictionary->getContentSize().height / 2));
        uiIcon->setScale(1.5);
        ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        _uiDictionary->addChild(uiIcon);
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_menu_bonsikdex"), GAME_FONT, 24);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiText->setPosition(Vec2(_uiDictionary->getContentSize().width - 82, _uiDictionary->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(105, uiText->getContentSize().height));
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        _uiDictionary->addChild(uiText);
    }
    
    // DPS
    {
        auto uiIcon = Sprite::create("Assets/ui_common/icon_rage1_1.png");
        uiIcon->setPosition(Vec2(40, 23));
        layout->addChild(uiIcon);

        auto action1 = DelayTime::create(1.0);
        auto action2 = CallFuncN::create([=](Ref* sender){
            ((ui::CText*)sender)->setString(_dps);
        });

        _uiDPS = ui::CText::create(_dps, GAME_FONT, 24);
        _uiDPS->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        _uiDPS->setPosition(Vec2(70, 23));
        _uiDPS->runAction(RepeatForever::create(Sequence::create(action1, action2, NULL)));
        layout->addChild(_uiDPS);
    }
    
    //
    {
        _uiDexBonus = ui::CText::create(GAME_TEXTFORMAT("t_ui_bonsikdex_weapon_enhance_bonus", "0"), GAME_FONT, 24);
        _uiDexBonus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _uiDexBonus->setPosition(Vec2(size.width - 25, 23));
        _uiDexBonus->setTextColor(Color4B::YELLOW);
        layout->addChild(_uiDexBonus);
    }
    
    
}

#pragma mark - ui : draw
void LayerNormalWeapon::drawInfo()
{
    auto layout = _uiContainerInfo;
    Size size = layout->getContentSize();
   
    
    if ( _uiDexBonus != nullptr )
    {
        double effect = 1.0;
        
        effect *= WeaponManager::getInstance()->getDexEffect() + 1;
        effect *= WeaponManager::getInstance()->getReinforceEffect() + 1;

        effect -= 1;
        effect *= 100;
        
        std::string effectString = MafUtils::doubleToString(effect);
        effectString = MafUtils::convertNumberToShort(effectString);
        
        _uiDexBonus->setString(GAME_TEXTFORMAT("t_ui_bonsikdex_weapon_enhance_bonus", effectString.c_str()));
        
    }
}

#pragma mark - ui : set, get

#pragma mark - ui : click
void LayerNormalWeapon::onClickUpgradeMax(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    int weaponEquip = WeaponManager::getInstance()->getEquip();
    int weaponEquipHigh = WeaponManager::getInstance()->getEquipHigh();
    int weaponLevel = WeaponManager::getInstance()->getLevel(weaponEquip);
    
    if ( weaponEquip < WeaponManager::getInstance()->getCount() && weaponLevel == C_MAX_EQUIP )
    {
        // 전설 무기 제작 검사
        int weaponIdx = weaponEquip + 1;
        
        auto objWeapon = WeaponManager::getInstance()->getInfoWeapon(weaponIdx);
        if ( objWeapon->getLegend() > 0 && weaponIdx > weaponEquipHigh )
        {
            auto popup = PopupWeaponCraft::create(weaponIdx);
            popup->show();
            return;
        }
    }
    
    //
    WeaponManager::getInstance()->onWeaponBuyMax();
}

void LayerNormalWeapon::onClickSpirit(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
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
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_weapon_spirit_1"), GAME_TEXT("t_ui_spirit_condition_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto popup = PopupListWeaponSpirit::create();
    popup->show();
}

void LayerNormalWeapon::onClickDictionary(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto popup = PopupBonsikDexDefault::create();
    popup->show();
}

#pragma mark - callback
void LayerNormalWeapon::callbackWeaponBuyMax()
{
    PopupLoading::hide();
    
    
}
#pragma mark - schedule
void LayerNormalWeapon::scheduleDPS(float dt)
{
    auto speed = UserInfoManager::getInstance()->getUserSpeed(_ePlace, false);
    speed*=4;
    
    //봉식이 DPS
    int criPer = UserInfoManager::getInstance()->getCriticalRatePercent(_ePlace, true, false);
    int atkSpeed = 1 / UserInfoManager::getInstance()->getAttackSpeed(_ePlace) * 100;
        
    std::string hAtk1 = UserInfoManager::getInstance()->getAttackDamage(_ePlace,E_CHARACTER::CHA_HERO, false, false);
    std::string hAtk2 = UserInfoManager::getInstance()->getAttackDamage(_ePlace,E_CHARACTER::CHA_HERO, true, false);
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
            if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_27) == true) {
                if (UserInfoManager::getInstance()->getRelicLevel(_ePlace, E_RELIC::RELIC_2) >= DataManager::GetRelicMaxLevel(E_RELIC::RELIC_2)) {
                    per += per * 6.0f;
                }
            }
        }
        
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_33) == true ) {
            per += per * 5.0f;
        }
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_23)) {
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
            if(CostumeManager::getInstance()->isEquip(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_2) == true) {
                mul *= 10;
            }
            int evolutionLevel = PetNewManager::getInstance()->getPetEvolution(E_PET::CHICK_1);
            double evolutionPer = PetNewManager::getInstance()->getPetEvolutionEffect1(E_PET::CHICK_1, evolutionLevel);
            if (evolutionPer > 0) {
                mul *= evolutionPer;
            }
            if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_26) == true) {
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
    
    _dps = MafUtils::format("%s/s", allDps.c_str());
}

#pragma mark - event
void LayerNormalWeapon::onListMove()
{
    auto uiList = dynamic_cast<UILayoutWeaponCell*>(_uiContainerList);
    if ( uiList != nullptr )
    {
        uiList->onListMove();
    }
}
