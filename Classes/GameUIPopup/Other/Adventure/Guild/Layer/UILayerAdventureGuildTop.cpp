//
//  UILayerAdventureGuildTop.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/15.
//

#include "UILayerAdventureGuildTop.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoSetting.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/Common/PopupButtonForceTutorial.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/Adventure/Guild/PopupAdventureParty.h"
#include "GameUIPopup/Other/Adventure/Guild/PopupAdventureOffice.h"
#include "GameUIPopup/Other/Adventure/Relic/PopupAdventureRelicDefault.h"
#include "GameUIPopup/Other/Adventure/PopupAdventureMission.h"
#include "GameUIPopup/Other/Adventure/PopupAdventureTutorial.h"
#include "GameUIPopup/Other/Adventure/Boss/PopupAdventureBossSelect.h"
#include "GameUIPopup/Other/Adventure/Boss/PopupAdventureBossBattle.h"

#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/AdventureRelicManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/PrisonManager.h"

USING_NS_CC;

UILayerAdventureGuildTop* UILayerAdventureGuildTop::create(const cocos2d::Size size)
{
    UILayerAdventureGuildTop* pRet = new(std::nothrow) UILayerAdventureGuildTop();
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

UILayerAdventureGuildTop::UILayerAdventureGuildTop(void):
_lyUI(nullptr)
,_lyDecorative(nullptr)
,_lyButton(nullptr)
,_sprManagerCat(nullptr)
,_callbackHide(nullptr)
,_btnTutorial(nullptr)
{
}

UILayerAdventureGuildTop::~UILayerAdventureGuildTop(void)
{
    _callbackHide = nullptr;
}

bool UILayerAdventureGuildTop::init(const cocos2d::Size size)
{
    if (!UILayerAdventureParent::init())
    {
        return false;
    }
    setContentSize(size);
    
    RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE, this);
    // init
    initVar();
    initUi();
    
    if(AdventureManager::getInstance()->getTutorialStep() == 3)
    {
        showTutorial();
    }
    
    return true;
}

#pragma mark - init
void UILayerAdventureGuildTop::initVar()
{
}

void UILayerAdventureGuildTop::initUi()
{
    // UI
    _lyDecorative = LayerColor::create();
    _lyDecorative->setContentSize(Size(getContentSize().width, getContentSize().height - 90));
    _lyDecorative->setIgnoreAnchorPointForPosition(false);
    _lyDecorative->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyDecorative->setPosition(getContentSize().width/2, getContentSize().height);
    addChild(_lyDecorative);
    
    _lyUI = Layer::create();
    _lyUI->setContentSize(Size(getContentSize().width, getContentSize().height - 90));
    _lyUI->setIgnoreAnchorPointForPosition(false);
    _lyUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyUI->setPosition(getContentSize().width/2, getContentSize().height);
    addChild(_lyUI);
    
    _lyButton = LayerColor::create(Color4B::BLACK);
    _lyButton->setContentSize(Size(getContentSize().width, 90));
    _lyButton->setIgnoreAnchorPointForPosition(false);
    _lyButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyButton->setPosition(_lyUI->getPosition() + Vec2::DOWN * _lyUI->getContentSize().height);
    addChild(_lyButton);
    
    //
    uiDecorative();
    uiTopUI();
    uiCurrency();
    uiButton();
}

#pragma mark - override
void UILayerAdventureGuildTop::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    refreshUI();
    if(AdventureManager::getInstance()->getTutorialStep() == 3 ||
       AdventureManager::getInstance()->getTutorialStep() == 5 ||
       AdventureManager::getInstance()->getTutorialStep() == 6)
        showButtonForceTutorial();
    showTutorial();
}

void UILayerAdventureGuildTop::refreshUI()
{
    uiTopUI();
    uiButton();
    uiDecorative();
    
    auto func = CallFunc::create(CC_CALLBACK_0(UILayerAdventureGuildTop::uiCurrency, this));
    runAction(Sequence::create(DelayTime::create(0.2f), func, nullptr));
}

void UILayerAdventureGuildTop::setCallbackHide(const std::function<void(void)>& callback)
{
    _callbackHide = callback;
}

#pragma mark - Tutorial
void UILayerAdventureGuildTop::showButtonForceTutorial()
{
    if(_btnTutorial == nullptr)
        return;
    
    auto popup = PopupButtonForceTutorial::create();
    popup->setButton(_btnTutorial);
    if(AdventureManager::getInstance()->getTutorialStep() == 3||
       AdventureManager::getInstance()->getTutorialStep() == 4)
        popup->setCallbackListener(CC_CALLBACK_1(UILayerAdventureGuildTop::onClickRelic, this));
    else if(AdventureManager::getInstance()->getTutorialStep() == 5)
             popup->setCallbackListener(CC_CALLBACK_1(UILayerAdventureGuildTop::onClickOfficeUpgrade, this));
    else if(AdventureManager::getInstance()->getTutorialStep() == 6)
             popup->setCallbackListener(CC_CALLBACK_1(UILayerAdventureGuildTop::onClickPartyUpgrade, this));
    popup->show();
    
    _btnTutorial = nullptr;
}

void UILayerAdventureGuildTop::showTutorial()
{
    if(AdventureManager::getInstance()->getTutorialStep() != 3)
        return;
    
    auto popup = PopupAdventureTutorial::create(AdventureManager::getInstance()->getTutorialStep());
    popup->show();
}
#pragma mark - ui
void UILayerAdventureGuildTop::uiTopUI()
{
    _lyUI->removeAllChildren();
    
    int nLv = AdventureManager::getInstance()->getLevel();
    auto infoLv = AdventureManager::getInstance()->getInfoBongLevel(nLv);
    int nTotalEXP = 0;
    int nPercent = 0;
    bool isMaxLv = false;
    
    //
    auto sprTitleBox = ui::Scale9Sprite::create("Assets/ui_common/base_text_1_2.png");
    sprTitleBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprTitleBox->setPosition( _lyUI->getContentSize().width/2, _lyUI->getContentSize().height - 10);
    _lyUI->addChild(sprTitleBox);
    {
        auto lbText = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_advt_main_1", AccountManager::getInstance()->getUserNick().c_str(), nLv), GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(sprTitleBox->getContentSize()/2);
        lbText->setDimensions(sprTitleBox->getContentSize().width - 30, sprTitleBox->getContentSize().height - 5);
        lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        sprTitleBox->addChild(lbText);
    }
    
    if(infoLv != nullptr)
    {
        nTotalEXP = infoLv->getTotalEXP();
    }
    nPercent = (double)AdventureManager::getInstance()->getExp() / (double)nTotalEXP * 100;
    if ( nPercent > 100 )
        nPercent = 100;
    
    if(nLv >= AdventureManager::getInstance()->getMaxLevel())
    {
        nPercent = 100;
        isMaxLv = true;
    }
    
    auto sprExpBg = Sprite::create("Assets/ui/adventure/adventure_progressbar1_1.png");
    sprExpBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprExpBg->setPosition(sprTitleBox->getPosition() + Vec2::DOWN * (sprTitleBox->getContentSize().height + 15));
    _lyUI->addChild(sprExpBg);
    
    auto sprExpIcon = Sprite::create("Assets/icon/icon_exp_2.png");
    sprExpIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    sprExpIcon->setPosition(sprExpBg->getPosition() + Vec2::LEFT * sprExpBg->getContentSize().width/2 + Vec2::DOWN * 5);
    _lyUI->addChild(sprExpIcon);
    
    std::string strProgressPath = "Assets/ui/adventure/adventure_progressbar1_2.png";
    
    auto progreesExp = ProgressTimer::create(Sprite::create(strProgressPath));
    progreesExp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    progreesExp->setPosition( sprExpBg->getPosition() );
    progreesExp->setMidpoint(Vec2(0, 0.5));
    progreesExp->setBarChangeRate(Vec2(1,0));
    progreesExp->setType(ProgressTimer::Type::BAR);
    progreesExp->setPercentage(nPercent);
    _lyUI->addChild(progreesExp);
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    menu->setAnchorPoint(Vec2::ZERO);
    _lyUI->addChild(menu);
    
    auto lyPoint = LayerColor::create();
    lyPoint->setIgnoreAnchorPointForPosition(false);
    lyPoint->setContentSize(Size(300,50));
    
    auto itemArea = MafNode::MafMenuItemSprite::create(lyPoint, nullptr, [=](Ref* sender){
        auto node = (Node*)sender;
        Vec2 posToast = node->getParent()->convertToWorldSpace(node->getPosition() + Vec2::DOWN * node->getContentSize().height);
        
        std::string strText = GAME_TEXT("t_ui_advt_office_info_1");
        PopupToast::showImmediately(strText, posToast.x, posToast.y);
        
        auto infoLv = AdventureManager::getInstance()->getInfoBongLevel(AdventureManager::getInstance()->getLevel());
        int nTotalEXP = 0;
        
        if(infoLv == nullptr)
        {
            strText = "MAX";
        }
        else
        {
            nTotalEXP = infoLv->getTotalEXP();
            strText = MafUtils::format("%d/%d", AdventureManager::getInstance()->getExp(), nTotalEXP);
        }
        
        PopupToast::showImmediately(strText, posToast.x, posToast.y - 50);
    });
    itemArea->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP);
    itemArea->setPosition(sprExpBg->getPosition());
    menu->addChild(itemArea);
    
    auto btnHelp = ui::Button::create("Assets/ui_common/icon_help.png");
    btnHelp->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    btnHelp->setPosition(Vec2(_lyUI->getContentSize().width - 30, _lyUI->getContentSize().height - 20));
    btnHelp->setZoomScale(0.05f);
    btnHelp->addClickEventListener(CC_CALLBACK_1(UILayerAdventureGuildTop::onClickHelp, this));
    _lyUI->addChild(btnHelp);
    
    auto btnBoss = ui::Button::create("Assets/ui/adventure/boss/icon_adventure_boss.png");
    btnBoss->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    btnBoss->setPosition(Vec2(_lyUI->getContentSize().width - 100, _lyUI->getContentSize().height - 20));
    btnBoss->setZoomScale(0.05f);
    btnBoss->addClickEventListener(CC_CALLBACK_1(UILayerAdventureGuildTop::onClickBoss, this));
    _lyUI->addChild(btnBoss);
    
    auto btnTimer = ui::Button::create("Assets/ui/prison/icon_prison_time.png");
    btnTimer->addClickEventListener([=](Ref*){
        
        Vec2 posToast = _lyUI->getParent()->convertToWorldSpace(_lyUI->getPosition());
        posToast.y -= _lyUI->getContentSize().height / 1.5;

        PopupToast::showImmediately(GAME_TEXT("t_ui_advt_time_toast_1"), posToast.x, posToast.y);
    });
    btnTimer->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    btnTimer->setZoomScale(0.05f);
    btnTimer->setPosition(Vec2(_lyUI->getContentSize().width - 10, btnHelp->getPosition().y - btnHelp->getContentSize().height - 10));
    _lyUI->addChild(btnTimer);
    {
        auto lbTime = Label::createWithTTF("", GAME_FONT, 18);
        lbTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbTime->setPosition(btnTimer->getContentSize().width / 2, 10);
        lbTime->enableOutline(Color4B::BLACK, 1);
        btnTimer->addChild(lbTime);
        
        auto action1 = DelayTime::create(1.0);
        auto action2 = CallFuncN::create([&, lbTime](Ref* sender){
            long time = AdventureManager::getInstance()->getResetTime() - UtilsDate::getInstance()->getTime();//서버에서 주는 시간으로 변경 필요
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
    
    //test
    auto btnMission = ui::Button::create("Assets/ui/bg_icon/icon_mission.png");
    btnMission->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    btnMission->setPosition(Vec2(20, _lyUI->getContentSize().height - 20));
    btnMission->setZoomScale(0.05f);
    btnMission->addClickEventListener(CC_CALLBACK_1(UILayerAdventureGuildTop::onClickMission, this));
    _lyUI->addChild(btnMission);
    {
        auto btnBox = ui::Button::create("Assets/ui/adventure/adventure_mission_textbox.png");
        btnBox->setScale9Enabled(true);
        btnBox->setContentSize(Size(150, btnBox->getContentSize().height));
        btnBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        btnBox->setPosition(btnMission->getPosition() + Vec2::RIGHT * btnMission->getContentSize().width + Vec2::DOWN * btnMission->getContentSize().height/2);
        btnBox->setZoomScale(0.02f);
        btnBox->addClickEventListener(CC_CALLBACK_1(UILayerAdventureGuildTop::onClickMission, this));
        _lyUI->addChild(btnBox);
            
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_mission_title_1"), GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbText->setPosition(10, btnBox->getContentSize().height/2);
        lbText->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        lbText->setDimensions(btnBox->getContentSize().width, btnBox->getContentSize().height - 5 );
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->setColor(COLOR_TITLE);
        btnBox->addChild(lbText);
        
        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
        uiReddot->setPosition(btnMission->getContentSize().width - 10, btnMission->getContentSize().height - 10);
        uiReddot->setVisible(false);
        btnMission->addChild(uiReddot);
        
        auto action1 = CallFuncN::create([=](Ref* sender){
            auto uiReddot = (UIReddot*)sender;
            bool isActivate = false;
            
            if(AdventureManager::getInstance()->isMissionComplete())
                isActivate = true;
            
            uiReddot->setVisible(isActivate);
        });
        auto action2 = DelayTime::create(2);
        auto seq = Sequence::create(action1, action2, NULL);
        uiReddot->runAction(RepeatForever::create(seq));
    }
}

void UILayerAdventureGuildTop::uiDecorative()
{
    _lyDecorative->removeAllChildren();
    _lyDecorative->stopAllActions();
    
    auto sprBG = Sprite::create("Assets/ui/adventure/adventure_main_bg1_1.png");
    sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBG->setPosition(_lyDecorative->getContentSize().width/2, _lyDecorative->getContentSize().height);
    sprBG->getTexture()->setTexParameters(texParams);
    sprBG->setScale(4.f);
    _lyDecorative->addChild(sprBG);
    
    uiMember();
    uiFurniture();
    uiManagerCat();
    uiGummie();
}

void UILayerAdventureGuildTop::uiMember()
{
    std::vector<tagDecorateInfo> listDecoInfo = {
        {Vec2(658, _lyDecorative->getContentSize().height - 311), 1},
        {Vec2(27, _lyDecorative->getContentSize().height - 301), 1},
        {Vec2(289, _lyDecorative->getContentSize().height - 444), 5},
        {Vec2(308, _lyDecorative->getContentSize().height - 308), 1},
        {Vec2(495, _lyDecorative->getContentSize().height - 440), 5},
        {Vec2(395, _lyDecorative->getContentSize().height - 357), 2},
        {Vec2(17, _lyDecorative->getContentSize().height - 450), 5},
        {Vec2(180, _lyDecorative->getContentSize().height - 374), 3},
        {Vec2(649, _lyDecorative->getContentSize().height - 452), 5},
    };
    
    std::vector<bool> listIsFlipped = {
        true, false, true,
        true, false, false,
        false, true, true,
    };
    
    auto listParty = AdventureManager::getInstance()->getListInfoParty();
    for(int i = 0; i < listParty.size(); ++i)
    {
        const auto objParty = listParty.at(i);
        if(!objParty->isOpen())
            continue;
        
        auto listMember = objParty->getListMemeber();
        
        for(int j = 0; j < listMember.size(); ++j)
        {
            auto objMember = listMember.at(j);
            if(objMember->getIdx() == 1 || !objMember->isOpen())
                continue;
            auto decoInfo = listDecoInfo[objMember->getImageIdx()-2];
            bool isFlipeed = listIsFlipped[objMember->getImageIdx()-2];
            
            std::string strPath = "Assets/ui/adventure/adventure_crew_%d_%d_1.png";
            auto sprMember = Sprite::create(MafUtils::format(strPath.c_str(), objMember->getImageIdx(), objMember->getImageGrade()));
            sprMember->setPosition(decoInfo.pos);
            sprMember->setScale(3.f);
            sprMember->setFlippedX(isFlipeed);
            sprMember->getTexture()->setTexParameters(texParams);
            sprMember->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            sprMember->setLocalZOrder(decoInfo.nZorder);
            _lyDecorative->addChild(sprMember);
            {
                auto animation = Animation::create();
                animation->setDelayPerUnit(0.25f);
                for ( int k = 1; k <= 2; k++ )
                {
                    auto sprFrame = Sprite::create(MafUtils::format("Assets/ui/adventure/adventure_crew_%d_%d_%d.png", objMember->getImageIdx(), objMember->getImageGrade(), k));
                    sprFrame->getTexture()->setTexParameters(texParams);
                    animation->addSpriteFrame(sprFrame->getSpriteFrame());
                }
                sprMember->runAction(RepeatForever::create(Animate::create(animation)));
            }
        }
    }
}
void UILayerAdventureGuildTop::uiManagerCat()
{
    std::vector<tagDecorateInfo> listDecoInfo = {
        {Vec2(40, _lyDecorative->getContentSize().height - 200), 0},
        {Vec2(142, _lyDecorative->getContentSize().height - 236), 0},
        {Vec2(456, _lyDecorative->getContentSize().height - 208), 0},
        {Vec2(564, _lyDecorative->getContentSize().height - 300), 0},
        {Vec2(96, _lyDecorative->getContentSize().height - 424), 3}
    };
    
    std::string strCatPath = "Assets/ui/adventure/adventure_manager_%d_1.png";
    int nRandNum = random(1, 5);
    auto decoInfo = listDecoInfo[nRandNum-1];
    
    _sprManagerCat = Sprite::create(MafUtils::format(strCatPath.c_str(), nRandNum));
    _sprManagerCat->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _sprManagerCat->setPosition(decoInfo.pos);
    _sprManagerCat->setScale(4.f);
    _sprManagerCat->getTexture()->setTexParameters(texParams);
    _sprManagerCat->setLocalZOrder(decoInfo.nZorder);
    _lyDecorative->addChild(_sprManagerCat);
    
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.25f);
    for ( int i = 1; i <= 2; i++ )
    {
        auto sprFrame = Sprite::create(MafUtils::format("Assets/ui/adventure/adventure_manager_%d_%d.png", nRandNum, i));
        sprFrame->getTexture()->setTexParameters(texParams);
        animation->addSpriteFrame(sprFrame->getSpriteFrame());
    }
    _sprManagerCat->runAction(RepeatForever::create(Animate::create(animation)));
    
    auto func = CallFunc::create([=](){
        std::vector<tagDecorateInfo> listDecoInfo = {
            {Vec2(40, _lyDecorative->getContentSize().height - 200), 0},
            {Vec2(142, _lyDecorative->getContentSize().height - 236), 0},
            {Vec2(456, _lyDecorative->getContentSize().height - 208), 0},
            {Vec2(564, _lyDecorative->getContentSize().height - 300), 0},
            {Vec2(96, _lyDecorative->getContentSize().height - 424), 3}
        };
        
        int nRandNum = random(1, 5);
        auto decoInfo = listDecoInfo[nRandNum-1];
        _sprManagerCat->stopAllActions();
        _sprManagerCat->setTexture(MafUtils::format("Assets/ui/adventure/adventure_manager_%d_1.png", nRandNum));
        _sprManagerCat->setPosition(decoInfo.pos);
        _sprManagerCat->setLocalZOrder(decoInfo.nZorder);
        _sprManagerCat->getTexture()->setTexParameters(texParams);
        
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.25f);
        for ( int i = 1; i <= 2; i++ )
        {
            auto sprFrame = Sprite::create(MafUtils::format("Assets/ui/adventure/adventure_manager_%d_%d.png", nRandNum, i));
            sprFrame->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(sprFrame->getSpriteFrame());
        }
        _sprManagerCat->runAction(RepeatForever::create(Animate::create(animation)));
    });
    auto delayTimer = DelayTime::create(10);
    auto seq = Sequence::create(delayTimer, func, nullptr);
    _lyDecorative->runAction(RepeatForever::create(seq));
}
void UILayerAdventureGuildTop::uiFurniture()
{
    std::vector<tagDecorateInfo> listDecoInfo = {
        {Vec2(228, _lyDecorative->getContentSize().height - 348), 2},
        {Vec2(68, _lyDecorative->getContentSize().height - 352), 2},
        {Vec2(348, _lyDecorative->getContentSize().height - 392), 2},
        {Vec2(488, _lyDecorative->getContentSize().height - 428), 4},
        {Vec2(156, _lyDecorative->getContentSize().height - 424), 3}
    };
    
    for(int i = 0; i < 5; ++i)
    {
        auto decoInfo = listDecoInfo[i];
        auto sprFurniture = Sprite::create(MafUtils::format("Assets/ui/adventure/adventure_main_obj_%02d.png", i+1));
        sprFurniture->getTexture()->setTexParameters(texParams);
        sprFurniture->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        sprFurniture->setScale(4.f);
        sprFurniture->setPosition(decoInfo.pos);
        sprFurniture->setLocalZOrder(decoInfo.nZorder);
        _lyDecorative->addChild(sprFurniture);
    }
}
void UILayerAdventureGuildTop::uiGummie()
{
    if(PrisonManager::getInstance()->isGummy())
        return;
    
    //x:152.y:236
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    menu->setAnchorPoint(Vec2::ZERO);
    _lyDecorative->addChild(menu);
    
    auto lyGummieTouchArea = LayerColor::create();
    lyGummieTouchArea->setIgnoreAnchorPointForPosition(false);
    lyGummieTouchArea->setContentSize(Size(80, 160));
    
    auto itemGummieTouchArea = MafNode::MafMenuItemSprite::create(lyGummieTouchArea, nullptr, CC_CALLBACK_1(UILayerAdventureGuildTop::onClickGummie, this));
    itemGummieTouchArea->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT);
    itemGummieTouchArea->setPosition(Vec2(224, 266));
    menu->addChild(itemGummieTouchArea);
    
    auto sprGummie = Sprite::create("Assets/ui/prison/jelly/pet_gummy_1.png");
    sprGummie->setPosition(itemGummieTouchArea->getContentSize().width/2, 0);
    sprGummie->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprGummie->getTexture()->setTexParameters(texParams);
    sprGummie->setScale(4);
    itemGummieTouchArea->addChild(sprGummie);
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.25f);
        for ( int i = 1; i <= 4; ++i )
        {
            auto sprFrame = Sprite::create(MafUtils::format("Assets/ui/prison/jelly/pet_gummy_%d.png", i));
            sprFrame->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(sprFrame->getSpriteFrame());
        }
        sprGummie->runAction(RepeatForever::create(Animate::create(animation)));
    }
    
    auto sprArrow = Sprite::create("Assets/ui/prison/arrow_1.png");
    sprArrow->setPosition(sprGummie->getPosition() + Vec2::UP * sprGummie->getContentSize().height * sprGummie->getScale());
    sprArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    itemGummieTouchArea->addChild(sprArrow);
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.25f);
        for ( int i = 1; i <= 6; ++i )
        {
            auto sprFrame = Sprite::create(MafUtils::format("Assets/ui/prison/arrow_%d.png", i));
            sprFrame->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(sprFrame->getSpriteFrame());
        }
        sprArrow->runAction(RepeatForever::create(Animate::create(animation)));
    }
}
void UILayerAdventureGuildTop::uiCurrency()
{
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::GEM);

    auto uiCurrency = UICurrency::create(list, Size(170, 58));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiCurrency->setPosition(Vec2(10, 20));
    _lyUI->addChild(uiCurrency, E_ZORER::UI+1);
    
    list.clear();
    list.push_back(E_ITEMS::ADVENTURE_TICKET);

    auto uiCurrency2 = UICurrency::create(list, Size(100, 58));
    uiCurrency2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiCurrency2->setPosition(Vec2(_lyUI->getContentSize().width - 10, 20));
    _lyUI->addChild(uiCurrency2, E_ZORER::UI+1);
}

void UILayerAdventureGuildTop::uiButton()
{
    _lyButton->removeAllChildren();
    
    enum class E_BUTTON_TYPE
    {
        RELIC,
        OFFICE,
        PARTY
    };
    
    std::vector<E_BUTTON_TYPE> listTab = {
        E_BUTTON_TYPE::RELIC,
        E_BUTTON_TYPE::OFFICE,
        E_BUTTON_TYPE::PARTY,
    };
    
    float width = (_lyButton->getContentSize().width) / listTab.size() - 10;
    float posX = _lyButton->getContentSize().width/2 - width * (listTab.size()/2) - 8 * (listTab.size()/2);
    
    for(int i = 0; i < listTab.size(); ++i)
    {
        auto obj = listTab[i];
        
        std::string strPath = "Assets/ui/adventure/btn_adventure_menu_1_1.png";
        std::string strIconPath = "";
        std::string strTitle = "";
        
        switch(obj)
        {
            case E_BUTTON_TYPE::RELIC:
            {
                strIconPath = "Assets/ui/adventure/menu_btn_relics.png";
            }break;
            case E_BUTTON_TYPE::OFFICE:
            {
                strIconPath = "Assets/ui/adventure/menu_btn_office.png";
            }break;
            case E_BUTTON_TYPE::PARTY:
            {
                strIconPath = "Assets/ui/adventure/menu_btn_member.png";
            }break;
        }
        
        auto btnTab = ui::Button::create(strPath);
        btnTab->setScale9Enabled(true);
        btnTab->setContentSize(Size(width, _lyButton->getContentSize().height - 10));
        btnTab->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        btnTab->setPosition(Vec2(posX, _lyButton->getContentSize().height/2));
        btnTab->setZoomScale(0.05f);
        btnTab->setTag((int)obj);
        _lyButton->addChild(btnTab);
        {
            switch(obj)
            {
                case E_BUTTON_TYPE::RELIC:
                {
                    btnTab->addClickEventListener(CC_CALLBACK_1(UILayerAdventureGuildTop::onClickRelic, this));
                    strTitle = GAME_TEXT("t_ui_advt_info_title_7");
                    
                    auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
                    uiReddot->setPosition(20, btnTab->getContentSize().height - 20);
                    uiReddot->setVisible(false);
                    btnTab->addChild(uiReddot);
                    
                    auto action1 = CallFuncN::create([=](Ref* sender){
                        auto uiReddot = (UIReddot*)sender;
                        bool isActivate = false;
                        
                        if(AdventureRelicManager::getInstance()->isRelicReddotCondition() || AdventureRelicManager::getInstance()->isRelicCollectReddotCondition())
                            isActivate = true;
                        
                        uiReddot->setVisible(isActivate);
                    });
                    auto action2 = DelayTime::create(2);
                    auto seq = Sequence::create(action1, action2, NULL);
                    uiReddot->runAction(RepeatForever::create(seq));
                    
                    if(AdventureManager::getInstance()->getTutorialStep() == 3||
                       AdventureManager::getInstance()->getTutorialStep() == 4)
                    {
                        _btnTutorial = btnTab;
                    }
                    if(AdventureManager::getInstance()->getTutorialStep() < 3)
                        btnTab->setEnabled(false);
                }break;
                case E_BUTTON_TYPE::OFFICE:
                {
                    btnTab->addClickEventListener(CC_CALLBACK_1(UILayerAdventureGuildTop::onClickOfficeUpgrade, this));
                    strTitle = GAME_TEXT("t_ui_advt_main_bt_1");
                    
                    if(AdventureManager::getInstance()->getTutorialStep() == 5)
                    {
                        _btnTutorial = btnTab;
                    }
                    
                    if(AdventureManager::getInstance()->getTutorialStep() < 5)
                        btnTab->setEnabled(false);
                }break;
                case E_BUTTON_TYPE::PARTY:
                {
                    btnTab->addClickEventListener(CC_CALLBACK_1(UILayerAdventureGuildTop::onClickPartyUpgrade, this));
                    strTitle = GAME_TEXT("t_ui_advt_main_bt_2");
                    
                    if(AdventureManager::getInstance()->getTutorialStep() == 6)
                    {
                        _btnTutorial = btnTab;
                    }
                    
                    if(AdventureManager::getInstance()->getTutorialStep() < 6)
                        btnTab->setEnabled(false);
                }break;
            }
            auto sprIcon = Sprite::create(strIconPath.c_str());
            sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            sprIcon->setPosition(Vec2(20, btnTab->getContentSize().height/2));
            btnTab->addChild(sprIcon);
            
            auto lbText = Label::createWithTTF(strTitle, GAME_FONT, 25);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            lbText->setPosition(sprIcon->getPosition() + Vec2::RIGHT * (sprIcon->getContentSize().width * sprIcon->getScale() + 10));
            lbText->setDimensions(btnTab->getContentSize().width - sprIcon->getContentSize().width * sprIcon->getScale() - 20, btnTab->getContentSize().height - 5);
            lbText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
            lbText->setOverflow(Label::Overflow::SHRINK);
            btnTab->addChild(lbText);
        }
        
        if(listTab.size() % 2 == 0)
            btnTab->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        
        posX += width + 10;
    }
}
#pragma mark -Click
void UILayerAdventureGuildTop::onClickHide(Ref* const pSender)
{
    if(_callbackHide != nullptr)
        _callbackHide();
}

void UILayerAdventureGuildTop::onClickHelp(Ref* const pSender)
{
    int nPage = 9;
    auto objSetting = AdventureManager::getInstance()->getSetting("advt_info_last number");
    if(objSetting != nullptr)
        nPage = objSetting->getValueToInt();
    auto popup = PopupHelp::create(nPage, "t_ui_advt_info_title_%d", "t_ui_advt_info_msg_%d");
    popup->show();
}

void UILayerAdventureGuildTop::onClickBoss(Ref* const pSender)
{
    int level = AdventureManager::getInstance()->getLevel();
    int levelCondition = AdventureManager::getInstance()->getSetting("boss_open_map_lv")->getValueToInt();
    if ( level < levelCondition )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_advt_boss_unlock", levelCondition));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto bossIdx = AdventureManager::getInstance()->getBossIdx();
    if ( bossIdx == 0 )
    {
        auto popup = PopupAdventureBossSelect::create();
        popup->show();
    }
    else
    {
        auto popup = PopupAdventureBossBattle::create();
        popup->show();
    }
}

void UILayerAdventureGuildTop::onClickRelic(Ref* const pSender)
{
    auto popup = PopupAdventureRelicDefault::create();
    popup->showButtonForceTutorial();
    popup->show();
}

void UILayerAdventureGuildTop::onClickOfficeUpgrade(Ref* const pSender)
{
    auto popup = PopupAdventureOffice::create();
    popup->showButtonForceTutorial();
    popup->show();
}

void UILayerAdventureGuildTop::onClickPartyUpgrade(Ref* const pSender)
{
    auto popup = PopupAdventureParty::create();
    popup->show();
}
void UILayerAdventureGuildTop::onClickGummie(Ref* const pSender)
{
    int nGummieObtainCondition = 6;
    auto objSetting = AdventureManager::getInstance()->getSetting("advt_open_lv_gumi");
    
    if(objSetting != nullptr)
        nGummieObtainCondition = objSetting->getValueToInt();
    
    if(AdventureManager::getInstance()->getLevel() < nGummieObtainCondition || AdventureManager::getInstance()->getTutorialStep() < 7)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_advt_gummie_1", nGummieObtainCondition));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    PopupLoading::show();
    AdventureManager::getInstance()->requestObtainGummie(CC_CALLBACK_2(UILayerAdventureGuildTop::callbackGummie, this));
}

void UILayerAdventureGuildTop::onClickMission(Ref* const pSender)
{
    auto popup = PopupAdventureMission::create();
    popup->show();
}

#pragma mark -callback
void UILayerAdventureGuildTop::callbackGummie(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if(nResult == 1)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_advt_gummie_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    else if(nResult != 1 && nResult != 8)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if( nResult == 7 )
        {
            strMessageKey = "t_ui_advt_error_2";
        }
        else if( nResult == 4 )
        {
            strMessageKey = "t_ui_advt_error_5";
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        
        if(_callbackHide != nullptr)
            _callbackHide();
        _callbackHide = nullptr;
        return;
    }
    
    refreshUI();
}
