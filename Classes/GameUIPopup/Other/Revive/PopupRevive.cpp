//
//  PopupRevive.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 07/06/2019.
//

#include "PopupRevive.h"

#include "GameScene/HelloWorldScene.h"

#include "GameUIPopup/Other/Revive/Popup/PopupReviveBooster.h"
#include "GameUIPopup/Other/Revive/Popup/PopupReviveDetail.h"
#include "GameUIPopup/Other/Revive/Popup/PopupReviveDetailDevil.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/UserInfoStackManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/AdventureRelicManager.h"
#include "ManagerGame/PrisonManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

PopupRevive* PopupRevive::create()
{
    PopupRevive* pRet = new(std::nothrow) PopupRevive();
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
PopupRevive::PopupRevive(void) :
_nFloor(0),
_nFloorSub(0),

_onCallbackResult(nullptr),

_layerContainerParent(nullptr),
_sprBoosterIcon(nullptr),
_labelBoosterEffect(nullptr),
_labelMsg(nullptr),
_labelBooster(nullptr)
{
    
}

PopupRevive::~PopupRevive(void)
{
    
}

bool PopupRevive::init()
{
    if ( !PopupBase::init(PopupBaseType::TRANSPARENT) )
    {
        return false;
    }

    // init
    initVar();
    initUi();

    return true;
}

#pragma mark - override
void PopupRevive::show()
{
    auto scene = UtilsScene::getInstance()->getRunningScene();
    if ( scene == nullptr )
    {
        return;
    }
    
    if ( scene != nullptr )
    {
        scene->addChild(this, ZORDER_SCENE_POPUP, TAG_SCENE_POPUP);
    }
    
    runAction( FadeTo::create(0.4, 200) );
}

#pragma mark - init
void PopupRevive::initVar()
{
    _nFloor = UserInfoManager::getInstance()->getFloor(_ePlace);
}

void PopupRevive::initUi()
{
    _layerContainerParent = Layer::create();
    _layerContainerParent->setContentSize( Size(744, getContentSize().height));
    _layerContainerParent->setIgnoreAnchorPointForPosition(false);
    _layerContainerParent->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    _layerContainerParent->setPosition( getContentSize().width / 2, getContentSize().height / 2 );
    addChild(_layerContainerParent);
    
    auto spriteCloseBg = Sprite::create("Assets/ui/revive/rebirth_x_bg.png");
    spriteCloseBg->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    spriteCloseBg->setPosition(_layerContainerParent->getContentSize().width, _layerContainerParent->getContentSize().height - 10 - UserInfoManager::getInstance()->_fHeightIphoneX);
    _layerContainerParent->addChild(spriteCloseBg);
    {
        // menu
        auto menu = MafNode::MafMenu::create();
        menu->setAnchorPoint(Vec2::ZERO);
        menu->setPosition(Vec2::ZERO);
        spriteCloseBg->addChild(menu);
        
        // item close
        auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupRevive::onClickClose, this));
        itemClose->setPosition(spriteCloseBg->getContentSize().width - 10 - itemClose->getContentSize().width / 2, spriteCloseBg->getContentSize().height / 2);
        itemClose->setTouchRect( itemClose->getContentSize() * 2 );
        menu->addChild(itemClose);
    }
    
    {
        auto sprOuterCircle = Sprite::create("Assets/ui/revive/rebirth_circle3.png");
        sprOuterCircle->setPosition(Vec2(_layerContainerParent->getContentSize().width / 2, _layerContainerParent->getContentSize().height*2/3));
        sprOuterCircle->runAction(RepeatForever::create(RotateBy::create(2.0f, 40.0f)));
        _layerContainerParent->addChild(sprOuterCircle);
        
        auto sprInnerCircle = Sprite::create("Assets/ui/revive/rebirth_circle2.png");
        sprInnerCircle->setPosition(Vec2(_layerContainerParent->getContentSize().width / 2, _layerContainerParent->getContentSize().height*2/3));
        sprInnerCircle->runAction(RepeatForever::create(RotateBy::create(1.0f, -40)));
        _layerContainerParent->addChild(sprInnerCircle);
        if ( _ePlace == DG_DEVIL_DOM )
        {
            sprOuterCircle->setColor(Color3B(201,32,247));
            sprInnerCircle->setColor(Color3B(201,32,247));
        }
        
        auto animationBonsik = Animation::create();
        animationBonsik->setDelayPerUnit(0.15f);
        for (int i = 1; i <= 4; i++)
        {
            animationBonsik->addSpriteFrameWithFile(MafUtils::format("Assets/ui/revive/rebirth_circle1_%d.png",i));
        }
        
        auto spriteBonsik = Sprite::create("Assets/ui/revive/rebirth_circle1_1.png");
        spriteBonsik->setPosition( Vec2(_layerContainerParent->getContentSize().width / 2, _layerContainerParent->getContentSize().height*2/3) );
        spriteBonsik->runAction(RepeatForever::create(Animate::create(animationBonsik)));
        _layerContainerParent->addChild(spriteBonsik);
        
        if ( _ePlace == DG_DEVIL_DOM )
        {
            auto animationBonsikeyes = Animation::create();
            animationBonsikeyes->setDelayPerUnit(0.15f);
            for (int i = 1; i <= 4; i++)
            {
                animationBonsikeyes->addSpriteFrameWithFile(MafUtils::format("Assets/ui/revive/rebirth_circle_darkeyes%d.png",i));
            }
            
            auto spriteBonsikeyes = Sprite::create("Assets/ui/revive/rebirth_circle_darkeyes1.png");
            spriteBonsikeyes->setPosition( Vec2(_layerContainerParent->getContentSize().width / 2, _layerContainerParent->getContentSize().height*2/3) );
            spriteBonsikeyes->runAction(RepeatForever::create(Animate::create(animationBonsikeyes)));
            _layerContainerParent->addChild(spriteBonsikeyes);
        }
    }
    
    
    if ( _ePlace == DG_NORMAL )
    {
        uiContentNormal();
        uiButtonNormal();
        
        //강화 큐브
        if ( UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_1_EVOL1) >= 99999 ||
             UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_3_EVOL1) >= 99999 ||
             UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_4_EVOL1) >= 99999 ||
             UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_8_EVOL1) >= 99999 )
        {
            uiCube();
        }
    }
    else if ( _ePlace == DG_DEVIL_DOM )
    {
        uiContentDevil();
        uiButtonDevil();
    }
}

#pragma mark - ui
void PopupRevive::uiContentNormal()
{
    //
    float fRatio = 0.27;
    
    //증폭기 주석처리 (추가 예정)
//    if(InfiniteArtifactManager::getInstance()->isActive())
//        fRatio = 0.21;
    
    for ( int p=0, e=(int)ARTI_9_KEY; e <= (int)ARTI_11_KEY; e++,p++ )
    {
        int level = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, (E_ARTIFACT)e);
        if ( e == E_ARTIFACT::ARTI_9_KEY )
        {
            if ( CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_6) )
            {
                level += 10;
            }
        }
        
        int reinforce = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, (E_ARTIFACT)e);
        auto effect = DataManager::GetArtifactEffect((E_ARTIFACT)e, level, reinforce);
        
        auto btnBG = ui::Button::create("Assets/ui_common/box_item.png");
        btnBG->setScale9Enabled(true);
        btnBG->setContentSize(Size(100,100));
        btnBG->setPosition(Vec2(_layerContainerParent->getContentSize().width*(fRatio+p*0.15),_layerContainerParent->getContentSize().height*0.45));
        btnBG->addClickEventListener([=](Ref*){
            
            PopupToast::show(GAME_TEXT("t_ui_revive_default_toast_" + std::to_string(p + 1)), getContentSize().width/2, getContentSize().height/2);
        });
        _layerContainerParent->addChild(btnBG);
        
        auto spriteIcon = Sprite::create(MafUtils::format("Assets/icon_artifact/artifact_%d.png", e));
        spriteIcon->setPosition(btnBG->getContentSize().width / 2, btnBG->getContentSize().height / 2);
        spriteIcon->setScale((btnBG->getContentSize().width - 3) / spriteIcon->getContentSize().width);
        btnBG->addChild(spriteIcon);
        if ( level == 0 )
        {
            spriteIcon->setOpacity(60);
        }
        
        std::string strText = GAME_TEXT("t_ui_revive_msg_2");
        if ( level > 0 )
        {
            if ( e == (int)ARTI_11_KEY )
                strText = GAME_TEXTFORMAT("t_ui_revive_msg_1", effect);
            else
                strText = MafUtils::format("%.0lf%%",effect);
        }
        
        auto levelEffect = Label::createWithTTF(strText, GAME_FONT, 18);
        levelEffect->setPosition(btnBG->getContentSize().width / 2, -10);
        levelEffect->setDimensions(btnBG->getContentSize().width * 0.9, levelEffect->getContentSize().height);
        levelEffect->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        levelEffect->setOverflow(Label::Overflow::SHRINK);
        btnBG->addChild(levelEffect);
    }
    
    //
    {
        int nBooster = UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_KEY_BONUS);
        
        auto bntKeyBooster = ui::Button::create("Assets/ui_common/box_item2_1.png");
        bntKeyBooster->setScale9Enabled(true);
        bntKeyBooster->addClickEventListener(CC_CALLBACK_1(PopupRevive::onClickBoosterPopup, this));
        bntKeyBooster->setContentSize(Size(100,100));
        bntKeyBooster->setPosition(Vec2(_layerContainerParent->getContentSize().width*(fRatio+3*0.15),_layerContainerParent->getContentSize().height*0.45));
        _layerContainerParent->addChild(bntKeyBooster);
        
        _sprBoosterIcon = Sprite::create("Assets/icon/icon_keybuff.png");
        _sprBoosterIcon->setPosition(bntKeyBooster->getContentSize().width / 2, bntKeyBooster->getContentSize().height / 2);
        _sprBoosterIcon->setScale((bntKeyBooster->getContentSize().width - 3) / _sprBoosterIcon->getContentSize().width);
        _sprBoosterIcon->getTexture()->setTexParameters(texParams);
        bntKeyBooster->addChild(_sprBoosterIcon);
        if ( nBooster == 0 )
        {
            _sprBoosterIcon->setOpacity(60);
        }
        
        std::string strText = GAME_TEXT("t_ui_revive_msg_2");
        if ( nBooster > 0 )
        {
            strText = "30%";
        }
        _labelBoosterEffect = Label::createWithTTF(strText, GAME_FONT, 18);
        _labelBoosterEffect->setPosition(bntKeyBooster->getContentSize().width / 2, -10);
        _labelBoosterEffect->setDimensions(bntKeyBooster->getContentSize().width * 0.9, _labelBoosterEffect->getContentSize().height);
        _labelBoosterEffect->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        _labelBoosterEffect->setOverflow(Label::Overflow::SHRINK);
        bntKeyBooster->addChild(_labelBoosterEffect);
        
        auto spritePlus = Sprite::create("Assets/ui_common/plus_2.png");
        spritePlus->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        spritePlus->setPosition(bntKeyBooster->getContentSize() - Size(5, 5));
        bntKeyBooster->addChild(spritePlus);
        
        auto spriteBoosterBG = ui::Scale9Sprite::create("Assets/ui_common/base_text_3_1.png");
        spriteBoosterBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        spriteBoosterBG->setContentSize(Size(82, 28));
        spriteBoosterBG->setPosition(bntKeyBooster->getContentSize().width/2, bntKeyBooster->getContentSize().height);
        bntKeyBooster->addChild(spriteBoosterBG);
        
        _labelBooster = Label::createWithTTF(std::to_string(nBooster), GAME_FONT, 24);
        _labelBooster->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
        _labelBooster->setDimensions(spriteBoosterBG->getContentSize().width, spriteBoosterBG->getContentSize().height);
        _labelBooster->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        _labelBooster->setOverflow(cocos2d::Label::Overflow::SHRINK);
        _labelBooster->setPosition(spriteBoosterBG->getContentSize()/2);
        spriteBoosterBG->addChild(_labelBooster);
    }
    
    //증폭기 주석처리 (추가 예정)
//    if(InfiniteArtifactManager::getInstance()->isActive())
//    {
//        auto level = UserInfoManager::getInstance()->getArtifactLevel(E_ARTIFACT::ARTI_105_INFINITE_DISCOUNT);
//         int reinforce = UserInfoManager::getInstance()->getArtifactReinforce(ARTI_105_INFINITE_DISCOUNT);
//        auto effect = DataManager::GetArtifactEffect(ARTI_105_INFINITE_DISCOUNT, level, reinforce);
//
//
//        auto spriteBg = ui::Scale9Sprite::create("Assets/ui_common/box_item.png", Rect::ZERO, Rect(5,30,10,10));
//        spriteBg->setContentSize(Size(100,100));
//        spriteBg->setPosition(Vec2(_layerContainerParent->getContentSize().width*(fRatio+4*0.15),_layerContainerParent->getContentSize().height*0.45));
//        _layerContainerParent->addChild(spriteBg);
//
//        auto spriteIcon = Sprite::create("Assets/icon_artifact/artifact_111.png");
//        spriteIcon->setPosition(spriteBg->getContentSize().width / 2, spriteBg->getContentSize().height / 2);
//        spriteIcon->setScale((spriteBg->getContentSize().width - 3) / spriteIcon->getContentSize().width);
//        spriteIcon->getTexture()->setTexParameters(texParams);
//        spriteBg->addChild(spriteIcon);
//        if ( level == 0 )
//        {
//            spriteIcon->setOpacity(60);
//        }
//
//        std::string strText = GAME_TEXT("t_ui_revive_msg_2");
//        if ( level > 0 )
//        {
//            std::string strEffect = MafUtils::doubleToString(effect);
//            MafUtils::convertNumberToShort(strEffect);
//            strText = MafUtils::format("%s%%", strEffect.c_str());
//        }
//        auto levelEffect = Label::createWithTTF(strText, GAME_FONT, 18);
//        levelEffect->setPosition(spriteBg->getContentSize().width / 2, -10);
//        levelEffect->setDimensions(spriteBg->getContentSize().width * 0.9, levelEffect->getContentSize().height);
//        levelEffect->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
//        levelEffect->setOverflow(Label::Overflow::SHRINK);
//        spriteBg->addChild(levelEffect);
//    }
    
    //
    auto spriteKeyBg = Sprite::create("Assets/ui_common/base_text_1_1.png");
    spriteKeyBg->setPosition(_layerContainerParent->getContentSize().width / 2, _layerContainerParent->getContentSize().height*0.36);
    _layerContainerParent->addChild(spriteKeyBg);
    {
        std::string strPrestigeKey = DataManager::getInstance()->GetPrestigeKey(_ePlace, _nFloor);
        
        strPrestigeKey = MafUtils::bigMulNum(strPrestigeKey, MafUtils::doubleToString(PetNewManager::getInstance()->getPetEquipEffect(502, 1) * 100));
        strPrestigeKey.pop_back();
        strPrestigeKey.pop_back();
        
        MafUtils::convertNumberToShort(strPrestigeKey);
        
        int level11 = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_11_KEY);
        int reinforce11 = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, E_ARTIFACT::ARTI_11_KEY);
        int effect11 = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_11_KEY, level11, reinforce11);
        
        std::string strMsg = GAME_TEXTFORMAT("t_ui_revive_msg_3", strPrestigeKey.c_str(), _nFloor);
        if ( effect11 > 0 )
        {
            strMsg =  GAME_TEXTFORMAT("t_ui_revive_msg_4", strPrestigeKey.c_str(), _nFloor, effect11);
        }
        
        _labelMsg = Label::createWithTTF(strMsg, GAME_FONT, 27);
        _labelMsg->setPosition(spriteKeyBg->getContentSize().width / 2, spriteKeyBg->getContentSize().height / 2);
        _labelMsg->setColor(Color3B(255, 127, 0));
        spriteKeyBg->addChild(_labelMsg);
    }
    
    // buff
    time_t nTimeBuff = UserInfoManager::getInstance()->getBuffRevival() - UtilsDate::getInstance()->getTime();
    if ( nTimeBuff > 0 )
    {
        auto labelBuffTitle = Label::createWithTTF(GAME_TEXT("t_ui_revive_msg_36"), GAME_FONT, 24);
        labelBuffTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        labelBuffTitle->setPosition(_layerContainerParent->getContentSize().width / 2, _layerContainerParent->getContentSize().height - 150);
        labelBuffTitle->setColor(Color3B(255, 127, 0));
        _layerContainerParent->addChild(labelBuffTitle);
        
        int32_t h = (int)nTimeBuff / 3600;
        int32_t m = (int)nTimeBuff % 3600 / 60;
        int32_t s = (int)nTimeBuff % 3600 % 60;
        
        auto labelBuffTime = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_revive_msg_38", h, m, s), GAME_FONT, 20);
        labelBuffTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        labelBuffTime->setPosition(labelBuffTitle->getPosition() + Vec2::DOWN * 35);
        _layerContainerParent->addChild(labelBuffTime);
    }
    
    //각성 공주
    if ( PetNewManager::getInstance()->getPetEvolution(E_PET::PRINCESS_4) >= 1 )
    {
        auto btnPet = ui::Button::create("Assets/ui/revive/princess_awakening_bg1.png");
        btnPet->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        btnPet->setPosition(Vec2(_layerContainerParent->getContentSize().width / 2 - 10, 230));
        btnPet->setZoomScale(0.05f);
        btnPet->setName("UI_PET_ACTIVE");
        btnPet->addClickEventListener([=](Ref* sender){
            
            bool bActive = !UserInfoStackManager::getInstance()->getPrincessEvolutionActive(DG_NORMAL);
            UserInfoStackManager::getInstance()->setPrincessEvolutionActive(DG_NORMAL, bActive);
            
            _nFloorSub = 0;
            
            //
            drawContentNormal();
        });
        _layerContainerParent->addChild(btnPet);
        {
            auto uiDesc = ui::CText::create(GAME_TEXT("t_ui_revive_princess_1"), GAME_FONT, 20);
            uiDesc->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            uiDesc->setPosition(btnPet->getContentSize() - Size(10, 5));
            btnPet->addChild(uiDesc);
            
            std::string pathToggle = "Assets/ui/mission/mission_off.png";
            if ( UserInfoStackManager::getInstance()->getPrincessEvolutionActive(DG_NORMAL) )
            {
                pathToggle = "Assets/ui/mission/mission_on.png";
            }
            auto uiToggle = ui::ImageView::create(pathToggle);
            uiToggle->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            uiToggle->setPosition(Vec2(btnPet->getContentSize().width - 10, 10));
            uiToggle->setScale(0.7f);
            uiToggle->setName("UI_TOGGLE");
            btnPet->addChild(uiToggle);
        }
        
        auto btnFloor = ui::Button::create("Assets/ui/revive/princess_awakening_bg2.png");
        btnFloor->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        btnFloor->setPosition(Vec2(_layerContainerParent->getContentSize().width / 2 + 10, 230));
        btnFloor->setZoomScale(0.05f);
        btnFloor->setName("UI_PET_FLOOR");
        btnFloor->addClickEventListener([=](Ref* sender){
            
            if ( _nFloorSub >= 10 )
            {
                _nFloorSub = 0;
            }
            else
            {
                _nFloorSub++;
            }
            
            //
            drawContentNormal();
        });
        _layerContainerParent->addChild(btnFloor);
        {
            auto uiDesc = ui::CText::create(GAME_TEXT("t_ui_revive_princess_2"), GAME_FONT, 20);
            uiDesc->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            uiDesc->setPosition(btnFloor->getContentSize() - Size(10, 5));
            btnFloor->addChild(uiDesc);

            auto uiFloorTo = ui::CText::create(GAME_TEXTFORMAT("t_ui_revive_princess_3", 1), GAME_FONT, 34);
            uiFloorTo->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            uiFloorTo->setPosition(Vec2(btnFloor->getContentSize().width - 10, 5));
            uiFloorTo->setName("UI_PET_FLOOR_TO");
            uiFloorTo->setTextColor(Color4B(255, 220, 0, 255));
            btnFloor->addChild(uiFloorTo);
        }
    }
    
    //
    if ( _nFloor >= C_MIN_PRESTIGE_FLOOR )
    {
        auto labelMsg = Label::createWithTTF(GAME_TEXT("t_ui_revive_msg_7"), GAME_FONT, 24);
        labelMsg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        labelMsg->setPosition(_layerContainerParent->getContentSize().width / 2, _layerContainerParent->getContentSize().height*0.18);
        labelMsg->setDimensions(_layerContainerParent->getContentSize().width * 0.9, 300);
        labelMsg->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelMsg->setOverflow(Label::Overflow::SHRINK);
        _layerContainerParent->addChild(labelMsg);
    }
    else
    {
        auto labelMsg = Label::createWithTTF(GAME_TEXT("t_ui_revive_msg_10"), GAME_FONT, 32);
        labelMsg->setPosition(Vec2(_layerContainerParent->getContentSize().width/2, 200));
        labelMsg->setColor(Color3B::RED);
        _layerContainerParent->addChild(labelMsg);
    }
    
    //
    drawContentNormal();
}

void PopupRevive::uiContentDevil()
{
    //
    auto spriteKeyBg = Sprite::create("Assets/ui_common/base_text_1_1.png");
    spriteKeyBg->setPosition(_layerContainerParent->getContentSize().width / 2, _layerContainerParent->getContentSize().height*0.36);
    _layerContainerParent->addChild(spriteKeyBg);
    {
        std::string strPrestigeKey = DataManager::getInstance()->GetPrestigeKey(_ePlace, _nFloor);
        if ( _nFloor >= C_MIN_PRESTIGE_FLOOR )
        {
            auto per = UserInfoManager::getInstance()->getEquipTotalEffectPercentDevil(E_EQUIP_TYPE::E_D_BODY);
            if(per>0)
            {
                per*=100;
                if(per>100){
                    auto tt = MafUtils::bigMulNum(strPrestigeKey, MafUtils::doubleToString(per));
                    tt.pop_back();
                    tt.pop_back();
                    tt.pop_back();
                    tt.pop_back();
                    strPrestigeKey = MafUtils::bigAddNum(strPrestigeKey, tt);
                }
            }
        }
        MafUtils::convertNumberToShort(strPrestigeKey);
        
        
        std::string strMsg = GAME_TEXTFORMAT("t_ui_revive_msg_5", strPrestigeKey.c_str(), _nFloor);
        
        auto labelMsg = Label::createWithTTF(strMsg, GAME_FONT, 27);
        labelMsg->setPosition(spriteKeyBg->getContentSize().width / 2, spriteKeyBg->getContentSize().height / 2);
        labelMsg->setColor(COLOR_D_KEY);
        spriteKeyBg->addChild(labelMsg);
    }
    
    //
    if ( _nFloor >= C_MIN_PRESTIGE_FLOOR )
    {
        auto labelMsg = Label::createWithTTF(GAME_TEXT("t_ui_revive_msg_22"), GAME_FONT, 24);
        labelMsg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        labelMsg->setPosition(_layerContainerParent->getContentSize().width / 2, _layerContainerParent->getContentSize().height*0.25);
        labelMsg->setDimensions(_layerContainerParent->getContentSize().width * 0.9, 110);
        labelMsg->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelMsg->setOverflow(Label::Overflow::SHRINK);
        _layerContainerParent->addChild(labelMsg);
        
        std::string strFloor = GAME_TEXT("t_ui_revive_msg_8");
        auto labelFloor = Label::createWithTTF(strFloor, GAME_FONT, 24);
        labelFloor->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        labelFloor->setPosition(_layerContainerParent->getContentSize().width / 2, _layerContainerParent->getContentSize().height*0.25);
        labelFloor->setColor(COLOR_TITLE);
        _layerContainerParent->addChild(labelFloor);
    }
    else
    {
        auto labelMsg = Label::createWithTTF(GAME_TEXT("t_ui_revive_msg_10"), GAME_FONT, 32);
        labelMsg->setPosition(Vec2(_layerContainerParent->getContentSize().width/2,_layerContainerParent->getContentSize().height*0.25));
        labelMsg->setColor(Color3B::RED);
        _layerContainerParent->addChild(labelMsg);
    }
 
}

void PopupRevive::uiButtonNormal()
{
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(_layerContainerParent->getContentSize().width / 2, 90);
    _layerContainerParent->addChild(menu);

    if ( _nFloor >= C_MIN_PRESTIGE_FLOOR )
    {
        auto itemTriple = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_prestige_booster.png"), Sprite::create("Assets/ui_common/btn_prestige_booster.png"), nullptr, CC_CALLBACK_1(PopupRevive::onClickPrestigePopup, this));
        itemTriple->setTag(2);
        menu->addChild(itemTriple);
        {
            auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_revival_3"), GAME_FONT, 24);
            labelTitle->setPosition( Vec2(itemTriple->getContentSize().width / 2, 90) );
            labelTitle->setColor(COLOR_COIN);
            labelTitle->enableBold();
            labelTitle->setDimensions(itemTriple->getContentSize().width * 0.9, labelTitle->getContentSize().height);
            labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelTitle->setOverflow(Label::Overflow::SHRINK);
            itemTriple->addChild(labelTitle);
            
            auto labelKey = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_revive_key", 5), GAME_FONT, 22);
            labelKey->setPosition( Vec2(itemTriple->getContentSize().width / 2, itemTriple->getContentSize().height / 2) );
            itemTriple->addChild(labelKey);
            
            auto spriteIcon = Sprite::create("Assets/icon/icon_jewel.png");
            spriteIcon->setPosition( Vec2(itemTriple->getContentSize().width / 2 - 20, 25) );
            itemTriple->addChild(spriteIcon);

            auto labelCount = Label::createWithTTF(MafUtils::toString(C_PRESTIGE_GEM2), GAME_FONT, 24);
            labelCount->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
            labelCount->setPosition( Vec2(itemTriple->getContentSize().width / 2, 25) );
            itemTriple->addChild(labelCount);
        }

        auto itemDouble = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_prestige_booster.png"), Sprite::create("Assets/ui_common/btn_prestige_booster.png"), nullptr, CC_CALLBACK_1(PopupRevive::onClickPrestigePopup, this));
        itemDouble->setTag(1);
        menu->addChild(itemDouble);
        {
            auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_stat_revival_2"), GAME_FONT, 24);
            labelTitle->setPosition( Vec2(itemDouble->getContentSize().width / 2, 90) );
            labelTitle->setColor(COLOR_COIN);
            labelTitle->enableBold();
            labelTitle->setDimensions(itemDouble->getContentSize().width * 0.9, labelTitle->getContentSize().height);
            labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelTitle->setOverflow(Label::Overflow::SHRINK);
            itemDouble->addChild(labelTitle);
            
            auto labelKey = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_revive_key", 3), GAME_FONT, 22);
            labelKey->setPosition( Vec2(itemDouble->getContentSize().width / 2, itemDouble->getContentSize().height / 2) );
            itemDouble->addChild(labelKey);
            
            auto spriteIcon = Sprite::create("Assets/icon/icon_jewel.png");
            spriteIcon->setPosition( Vec2(itemDouble->getContentSize().width / 2 - 20, 25) );
            itemDouble->addChild(spriteIcon);

            auto labelCount = Label::createWithTTF(MafUtils::toString(C_PRESTIGE_GEM), GAME_FONT, 24);
            labelCount->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
            labelCount->setPosition( Vec2(itemDouble->getContentSize().width / 2, 25) );
            itemDouble->addChild(labelCount);
        }
        
        auto itemDefault = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_prestige_booster.png"), Sprite::create("Assets/ui_common/btn_prestige_booster.png"), nullptr, CC_CALLBACK_1(PopupRevive::onClickPrestigePopup, this));
        itemDefault->setTag(0);
        menu->addChild(itemDefault);
        {
            auto label = Label::createWithTTF(GAME_TEXT("t_ui_revive_title"), GAME_FONT, 26);
            label->setPosition( Vec2(itemDefault->getContentSize().width / 2, itemDefault->getContentSize().height / 2) );
            label->setDimensions(itemDefault->getContentSize().width * 0.9, label->getContentSize().height);
            label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            label->setOverflow(Label::Overflow::SHRINK);
            itemDefault->addChild(label);
        }

//        auto itemBooster = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_prestige_booster.png"), nullptr, CC_CALLBACK_1(PopupRevive::onClickBoosterPopup, this));
//        menu->addChild(itemBooster);
//        {
//            auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_keybooster"), GAME_FONT, 24);
//            labelTitle->setPosition( Vec2(itemBooster->getContentSize().width / 2, 90) );
//            labelTitle->setColor(COLOR_COIN);
//            labelTitle->enableBold();
//            labelTitle->setDimensions(itemBooster->getContentSize().width * 0.9, labelTitle->getContentSize().height);
//            labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
//            labelTitle->setOverflow(Label::Overflow::SHRINK);
//            itemBooster->addChild(labelTitle);
//
//            auto spriteIcon = Sprite::create("Assets/icon/icon_keybuff.png");
//            spriteIcon->setPosition( Vec2(itemBooster->getContentSize().width / 2 - 20, 40) );
//            itemBooster->addChild(spriteIcon);
//
//            int nBooster = UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_KEY_BONUS);
//            _labelBooster = Label::createWithTTF(MafUtils::format("x %d", nBooster), GAME_FONT, 24);
//            _labelBooster->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
//            _labelBooster->setPosition( Vec2(itemBooster->getContentSize().width / 2, 40) );
//            itemBooster->addChild(_labelBooster);
//        }
    }
    else
    {
        auto itemCheck = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr, CC_CALLBACK_1(PopupRevive::onClickClose, this));
        menu->addChild(itemCheck);
        {
            auto label = Label::createWithTTF(GAME_TEXT("t_ui_confirm"), GAME_FONT, 26);
            label->setPosition(itemCheck->getContentSize().width / 2, itemCheck->getContentSize().height / 2);
            itemCheck->addChild(label);
        }
    }
    
    //
    menu->alignItemsHorizontally();
}

void PopupRevive::uiButtonDevil()
{
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(_layerContainerParent->getContentSize().width / 2, 90);
    _layerContainerParent->addChild(menu);
    
    if ( _nFloor >= C_MIN_PRESTIGE_FLOOR )
    {
        auto itemTriple = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_prestige_booster.png"), Sprite::create("Assets/ui_common/btn_prestige_booster.png"), nullptr, CC_CALLBACK_1(PopupRevive::onClickPrestigePopup, this));
        itemTriple->setTag(2);
        menu->addChild(itemTriple);
        {
            auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_revival_3"), GAME_FONT, 24);
            labelTitle->setPosition( Vec2(itemTriple->getContentSize().width / 2, 90) );
            labelTitle->setColor(COLOR_COIN);
            labelTitle->enableBold();
            labelTitle->setDimensions(itemTriple->getContentSize().width * 0.9, labelTitle->getContentSize().height);
            labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelTitle->setOverflow(Label::Overflow::SHRINK);
            itemTriple->addChild(labelTitle);
            
            {
                auto spriteKeyIcon = Sprite::create("Assets/icon/icon_gold_2.png");
                spriteKeyIcon->setPosition( Vec2(itemTriple->getContentSize().width / 2 - 20, itemTriple->getContentSize().height / 2) );
                itemTriple->addChild(spriteKeyIcon);
                
                auto labelKey = Label::createWithTTF(MafUtils::format("x%d", 10), GAME_FONT, 22);
                labelKey->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
                labelKey->setPosition( Vec2(itemTriple->getContentSize().width / 2 + 5, itemTriple->getContentSize().height / 2) );
                itemTriple->addChild(labelKey);
            }
            
            {
                auto spriteIcon = Sprite::create("Assets/icon/icon_jewel.png");
                spriteIcon->setPosition( Vec2(itemTriple->getContentSize().width / 2 - 20, 25) );
                itemTriple->addChild(spriteIcon);
                
                auto labelCount = Label::createWithTTF(MafUtils::toString(C_PRESTIGE_GEM2), GAME_FONT, 24);
                labelCount->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
                labelCount->setPosition( Vec2(itemTriple->getContentSize().width / 2, 25) );
                itemTriple->addChild(labelCount);
            }
        }
        
        auto itemDouble = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_prestige_booster.png"), Sprite::create("Assets/ui_common/btn_prestige_booster.png"), nullptr, CC_CALLBACK_1(PopupRevive::onClickPrestigePopup, this));
        itemDouble->setTag(1);
        menu->addChild(itemDouble);
        {
            auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_stat_revival_2"), GAME_FONT, 24);
            labelTitle->setPosition( Vec2(itemDouble->getContentSize().width / 2, 90) );
            labelTitle->setColor(COLOR_COIN);
            labelTitle->enableBold();
            labelTitle->setDimensions(itemDouble->getContentSize().width * 0.9, labelTitle->getContentSize().height);
            labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelTitle->setOverflow(Label::Overflow::SHRINK);
            itemDouble->addChild(labelTitle);
            
            {
                auto spriteKeyIcon = Sprite::create("Assets/icon/icon_gold_2.png");
                spriteKeyIcon->setPosition( Vec2(itemDouble->getContentSize().width / 2 - 20, itemDouble->getContentSize().height / 2) );
                itemDouble->addChild(spriteKeyIcon);
                
                auto labelKey = Label::createWithTTF(MafUtils::format("x%d", 5), GAME_FONT, 22);
                labelKey->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
                labelKey->setPosition( Vec2(itemDouble->getContentSize().width / 2, itemDouble->getContentSize().height / 2) );
                itemDouble->addChild(labelKey);
            }
            
            {
                auto spriteIcon = Sprite::create("Assets/icon/icon_jewel.png");
                spriteIcon->setPosition( Vec2(itemDouble->getContentSize().width / 2 - 20, 25) );
                itemDouble->addChild(spriteIcon);
                
                auto labelCount = Label::createWithTTF(MafUtils::toString(C_PRESTIGE_GEM), GAME_FONT, 24);
                labelCount->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
                labelCount->setPosition( Vec2(itemDouble->getContentSize().width / 2, 25) );
                itemDouble->addChild(labelCount);
            }
        }
        
        auto itemDefault = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_prestige_booster.png"), Sprite::create("Assets/ui_common/btn_prestige_booster.png"), nullptr, CC_CALLBACK_1(PopupRevive::onClickPrestigePopup, this));
        itemDefault->setTag(0);
        menu->addChild(itemDefault);
        {
            auto label = Label::createWithTTF(GAME_TEXT("t_ui_revive_title"), GAME_FONT, 26);
            label->setPosition( Vec2(itemDefault->getContentSize().width / 2, itemDefault->getContentSize().height / 2) );
            label->setDimensions(itemDefault->getContentSize().width * 0.9, label->getContentSize().height);
            label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            label->setOverflow(Label::Overflow::SHRINK);
            itemDefault->addChild(label);
        }
    }
    else
    {
        auto itemCheck = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr, CC_CALLBACK_1(PopupRevive::onClickClose, this));
        menu->addChild(itemCheck);
        {
            auto label = Label::createWithTTF(GAME_TEXT("t_ui_confirm"), GAME_FONT, 26);
            label->setPosition(itemCheck->getContentSize().width / 2, itemCheck->getContentSize().height / 2);
            itemCheck->addChild(label);
        }
    }
    
    //
    menu->alignItemsHorizontally();
}

void PopupRevive::uiCube()
{
    auto spriteCubeBg = Sprite::create("Assets/ui/revive/revive_box_1.png");
    spriteCubeBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    spriteCubeBg->setPosition(Vec2(_layerContainerParent->getContentSize().width*(0.29+4*0.15),_layerContainerParent->getContentSize().height*0.55));
    _layerContainerParent->addChild(spriteCubeBg);
    {
        auto labelCubeTitle = Label::createWithTTF(GAME_TEXT("t_ui_revive_msg_29"), GAME_FONT, 20);
        labelCubeTitle->setPosition(spriteCubeBg->getContentSize().width / 2, spriteCubeBg->getContentSize().height - 25);
        labelCubeTitle->setDimensions(spriteCubeBg->getContentSize().width * 0.95, labelCubeTitle->getContentSize().height);
        labelCubeTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelCubeTitle->setOverflow(Label::Overflow::SHRINK);
        spriteCubeBg->addChild(labelCubeTitle);
        
        int nCubeGain = UserInfoManager::getInstance()->getCubeTotalGain();
        if ( nCubeGain > 0 )
        {
            auto spriteCubeIcon = Sprite::create("Assets/icon/menu_cube.png");
            spriteCubeIcon->setPosition(spriteCubeBg->getContentSize().width / 2 - 20, spriteCubeBg->getContentSize().height / 2 - 5);
            spriteCubeBg->addChild(spriteCubeIcon);
            
            auto labelCubeGain = Label::createWithTTF(MafUtils::toString(UserInfoManager::getInstance()->getCubeTotalGain()), GAME_FONT, 24);
            labelCubeGain->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labelCubeGain->setPosition(spriteCubeBg->getContentSize().width / 2 + 10, spriteCubeBg->getContentSize().height / 2 - 5);
            labelCubeGain->setColor(COLOR_COIN);
            spriteCubeBg->addChild(labelCubeGain);
        }
        else
        {
            int nFloorCondition = UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) * 0.7;
            if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM,E_COSTUME_IDX::IDX_8))
            {
                nFloorCondition = UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) * 0.65f;
            }
            auto labelCondition = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_revive_msg_35", nFloorCondition), GAME_FONT, 16);
            labelCondition->setPosition(spriteCubeBg->getContentSize().width / 2, spriteCubeBg->getContentSize().height / 2 - 5);
            labelCondition->setDimensions(spriteCubeBg->getContentSize().width * 0.95, labelCondition->getContentSize().height);
            labelCondition->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelCondition->setOverflow(Label::Overflow::SHRINK);
            spriteCubeBg->addChild(labelCondition);
        }
        
        auto labelCubeDay = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_revive_msg_30", UserInfoManager::getInstance()->getCubeDay(), C_MAX_CUBE_DAY), GAME_FONT, 16);
        labelCubeDay->setPosition(spriteCubeBg->getContentSize().width / 2, 20);
        labelCubeDay->setDimensions(spriteCubeBg->getContentSize().width * 0.95, labelCubeTitle->getContentSize().height);
        labelCubeDay->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelCubeDay->setOverflow(Label::Overflow::SHRINK);
        spriteCubeBg->addChild(labelCubeDay);
    }
}

#pragma mark - ui draw
void PopupRevive::drawContentNormal()
{
    bool bActive = UserInfoStackManager::getInstance()->getPrincessEvolutionActive(DG_NORMAL);
    
    if ( auto btnPet = utils::findChild<ui::Button*>(_layerContainerParent, "UI_PET_ACTIVE"); btnPet != nullptr )
    {
        if ( auto uiToggle = utils::findChild<ui::ImageView*>(btnPet, "UI_TOGGLE"); uiToggle != nullptr )
        {
            if ( bActive )
            {
                uiToggle->loadTexture("Assets/ui/mission/mission_on.png");
            }
            else
            {
                uiToggle->loadTexture("Assets/ui/mission/mission_off.png");
            }
        }
    }
    
    if ( auto btnPetFloor = utils::findChild<ui::Button*>(_layerContainerParent, "UI_PET_FLOOR"); btnPetFloor != nullptr )
    {
        int floor = 1;
        if ( bActive == true && _nFloor >= 2001 )
        {
            int count = (_nFloor-1) / 2000;
            
            floor = count * 2000 - (_nFloorSub * 2000 + 999);
            if ( floor < 1 )
                floor = 1;
            
            //모험 유물 적용
            floor += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::PRINCESSES_SKILL_FLOOR_INCREASE);
        }
        
        if ( auto uiFloorTo = utils::findChild<ui::CText*>(btnPetFloor, "UI_PET_FLOOR_TO"); uiFloorTo != nullptr )
        {
            uiFloorTo->setString(GAME_TEXTFORMAT("t_ui_revive_princess_3", floor));
        }
    }
}

void PopupRevive::drawKeyBooster()
{
    if ( _ePlace == E_PLACE::DG_NORMAL )
    {
        int nBooster = UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_KEY_BONUS);
        _labelBooster->setString(MafUtils::format("x %d", nBooster));
        
        std::string strPrestigeKey = DataManager::getInstance()->GetPrestigeKey(_ePlace, _nFloor);
        
        strPrestigeKey = MafUtils::bigMulNum(strPrestigeKey, MafUtils::doubleToString(PetNewManager::getInstance()->getPetEquipEffect(502, 1) * 100));
        strPrestigeKey.pop_back();
        strPrestigeKey.pop_back();
        
        MafUtils::convertNumberToShort(strPrestigeKey);
        
        int level11 = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_11_KEY);
        int reinforce11 = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, E_ARTIFACT::ARTI_11_KEY);
        int effect11 = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_11_KEY, level11, reinforce11);
        
        std::string strMsg = GAME_TEXTFORMAT("t_ui_revive_msg_3", strPrestigeKey.c_str(), _nFloor);
        if ( effect11 > 0 )
        {
            strMsg =  GAME_TEXTFORMAT("t_ui_revive_msg_4", strPrestigeKey.c_str(), _nFloor, effect11);
        }
        _labelMsg->setString(strMsg);
        
        std::string strEffect = GAME_TEXT("t_ui_revive_msg_2");
        if ( nBooster > 0 )
        {
            strEffect = "30%";
        }
        _labelBoosterEffect->setString(strEffect);
    }
    else
    {
        
    }
}

#pragma mark - set, get, other
void PopupRevive::setCallbackResult(const std::function<void(int, bool, bool, int)>& callback)
{
    _onCallbackResult = callback;
}

#pragma mark - click
void PopupRevive::onClickPrestigePopup(Ref* sender)
{
    auto item = (MafNode::MafMenuItem*)sender;
    
    int nType = item->getTag();
    
    bool bPlay = PrisonManager::getInstance()->isDungeonPlay();
    int nEnterAutoCount = PrisonManager::getInstance()->getEnterAutoCount();
    if ( bPlay == true || nEnterAutoCount > 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_close_check"));
        popup->addButton(GAME_TEXT("t_ui_cancel"), true, nullptr);
        popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
            
            onPrestigePopup(nType);
            
        });
        popup->show();
    }
    else
    {
        onPrestigePopup(nType);
    }
}

void PopupRevive::onClickBoosterPopup(cocos2d::Ref* pSender)
{
    auto popup = PopupReviveBooster::create();
    popup->setCallbackResult(CC_CALLBACK_0(PopupRevive::callbackBoosterResult, this));
    popup->show();
}

void PopupRevive::onClickClose(Ref *sender)
{
    hide();
}

#pragma mark - callback
void PopupRevive::callbackBoosterResult()
{
    // Refresh
    if ( _ePlace == DG_NORMAL )
    {
        drawKeyBooster();
        
        //
        HelloWorldScene::getInstance()->reDrawBuffTimer();
        
        int nBooster = UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_KEY_BONUS);
        
        if ( nBooster == 0 )
        {
            _sprBoosterIcon->setOpacity(60);
        }
        else
        {
            _sprBoosterIcon->setOpacity(255);
        }
        
        PopupToast::show(GAME_TEXT("t_ui_key_booster_buy_1"), getContentSize().width/2, getContentSize().height/2);
    }
}

void PopupRevive::callbackReviveDetailResult(int nType, int nTypeButton)
{
    // nType 2 : 초강화환생
    // nType 1 : 강화환생
    // nType 0 : 일반환생
    if ( nTypeButton == 1 )
    {// 환생
        if ( _onCallbackResult )
        {
            _onCallbackResult(nType, false, false, _nFloorSub);
        }
        
        hide();
    }
    else if ( nTypeButton == 2 )
    {// 광고 시청 후 환생
        if ( CashManager::getInstance()->isAdsfreeTime() == true )
        {
            if ( _onCallbackResult )
            {
                _onCallbackResult(nType, true, false, _nFloorSub);
            }
            
            hide();
        }
        else
        {
            if ( _onCallbackResult )
            {
                _onCallbackResult(nType, true, false, _nFloorSub);
            }
            
            hide();
        }
    }
    else if ( nTypeButton == 3 )
    {// 광고 무료 환생
        if ( _onCallbackResult )
        {
            _onCallbackResult(nType, false, true, _nFloorSub);
        }
        
        hide();
    }
}

void PopupRevive::callbackReviveDetailDevilResult(int nType, int nTypeButton)
{
    // nType 2 : 초강화환생
    // nType 1 : 강화환생
    // nType 0 : 일반환생
    if ( nTypeButton == 1 )
    {// 환생
        if ( _onCallbackResult )
        {
            _onCallbackResult(nType, false, false, 0);
        }
        
        hide();
    }
    else if ( nTypeButton == 2 )
    {// 광고 시청 후 환생
        if ( CashManager::getInstance()->isAdsfreeTime() == true )
        {
            if ( _onCallbackResult )
            {
                _onCallbackResult(nType, true, false, 0);
            }
            
            hide();
        }
        else
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_revive_msg_19"));
            popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
            popup->addButton(GAME_TEXT("t_ui_yes"), false, [=](Ref* sender){
                
                if ( _onCallbackResult )
                {
                    _onCallbackResult(nType, true, false, 0);
                }
                
                hide();
            });
            popup->show();
        }
    }
}

#pragma mark - callback

#pragma mark - game
void PopupRevive::onPrestigePopup(int nType)
{
    if ( _ePlace == DG_NORMAL )
    {
        auto popup = PopupReviveDetail::create(nType);
        popup->setCallbackResult(CC_CALLBACK_2(PopupRevive::callbackReviveDetailResult, this));
        popup->show();
    }
    else if ( _ePlace == DG_DEVIL_DOM )
    {
        auto popup = PopupReviveDetailDevil::create(nType);
        popup->setCallbackResult(CC_CALLBACK_2(PopupRevive::callbackReviveDetailDevilResult, this));
        popup->show();
    }
}
