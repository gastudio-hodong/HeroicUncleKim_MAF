//
// UILayerAdventureRelicCrafting.cpp
// FantasyClicker-mobile
//
// Created by MAF_TG on 2022/05/09.
//

#include "UILayerAdventureRelicCrafting.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoSetting.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Common/PopupButtonForceTutorial.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"
#include "GameUIPopup/Other/Ads/PopupAdsWatch.h"
#include "GameUIPopup/Other/Adventure/Relic/PopupAdventureRelicCraft.h"
#include "GameUIPopup/Other/Adventure/Relic/PopupAdventureRelicCraftRateInfo.h"
#include "GameUIPopup/Other/Adventure/PopupAdventureTutorial.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/AdventureRelicManager.h"

USING_NS_CC;

bool UILayerAdventureRelicCrafting::sortCore(InfoAdventureCore* a, InfoAdventureCore* b)
{
    bool result = a->getIdx() < b->getIdx();
   
    if(a->getIdx() == 5)
        result = true;
   
    return result;
}
UILayerAdventureRelicCrafting* UILayerAdventureRelicCrafting::create(cocos2d::Size size)
{
    UILayerAdventureRelicCrafting* pRet = new(std::nothrow) UILayerAdventureRelicCrafting();
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

UILayerAdventureRelicCrafting::UILayerAdventureRelicCrafting(void):
_isCloseCondition(false)
,_lyTopUI(nullptr)
,_lyBottomUI(nullptr)
,_particleDust(nullptr)
,_particleFire(nullptr)
,_callbackHide(nullptr)
,_callbackMoveShop(nullptr)
,_table(nullptr)
,_btnTutorial(nullptr)
,_currentInfoCore(nullptr)
{
    _listCore.clear();
}

UILayerAdventureRelicCrafting::~UILayerAdventureRelicCrafting(void)
{
    _listCore.clear();
    
    if(_particleDust != nullptr)
    {
        _particleDust->release();
        _particleDust = nullptr;
    }
    if(_particleFire != nullptr)
    {
        _particleFire->release();
        _particleFire = nullptr;
    }
}

bool UILayerAdventureRelicCrafting::init(cocos2d::Size size)
{
    if (!UILayerAdventureParent::init())
    {
        return false;
    }
    setContentSize(size);
    setCoreList();
    
    if(AdventureManager::getInstance()->getTutorialStep() == 3)
        _currentInfoCore = getCore(1);
    else
        _currentInfoCore = getCore(5);
    
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE_TUTORIAL_RUNNING, this);
    else
        RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE, this);
    
    initVar();
    initUi();
    return true;
}
void UILayerAdventureRelicCrafting::onEnter()
{
    UILayerAdventureParent::onEnter();
    PopupLoading::show();
    AdventureRelicManager::getInstance()->requestInfoCore(CC_CALLBACK_2(UILayerAdventureRelicCrafting::callbackInit, this));
}
#pragma mark - table
void UILayerAdventureRelicCrafting::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
}

Size UILayerAdventureRelicCrafting::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 80);
}

extension::TableViewCell* UILayerAdventureRelicCrafting::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if ( cell == nullptr )
    {
        cell = new extension::TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();
    // size
    const Size size = tableCellSizeForIndex(table, idx);
    cell->setContentSize(size);
   

    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    cell->addChild(menu);

    auto const objCore = _listCore.at(idx);
    if(objCore == nullptr)
    return cell;

    std::string strBgPath = "Assets/ui/adventure/list_relic_1_1.png";
    std::string strRadioBtnPath = "Assets/ui_common/radio_button1_1.png";
    bool isCurrent = objCore->getIdx() == _currentInfoCore->getIdx();

    if(objCore->getIdx() == 5)
        strBgPath = "Assets/ui/adventure/list_relic_1_2.png";
    if(isCurrent)
        strRadioBtnPath = "Assets/ui_common/radio_button1_2.png";


    auto btnCore = MafNode::MafMenuItemSprite::create(Sprite::create(strBgPath), nullptr,  CC_CALLBACK_1(UILayerAdventureRelicCrafting::onClickCore, this));
    btnCore->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    btnCore->setPosition(Vec2(20, cell->getContentSize().height - 20));
    btnCore->setTag(objCore->getIdx());
    menu->addChild(btnCore);

    auto const sprRadioBtn = Sprite::create(strRadioBtnPath);
    sprRadioBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprRadioBtn->setPosition(Vec2(15, btnCore->getContentSize().height/2));
    btnCore->addChild(sprRadioBtn);

    auto const sprIconCore = Sprite::create(MafUtils::format("Assets/icon/icon_core_%02d.png",objCore->getIdx()));
    sprIconCore->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprIconCore->setPosition(sprRadioBtn->getPosition() + Vec2::RIGHT * (sprRadioBtn->getContentSize().width + 10 ));
    btnCore->addChild(sprIconCore);
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_currency_core_%d", objCore->getIdx())), GAME_FONT, 24);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbTitle->setPosition(sprIconCore->getPosition() + Vec2::RIGHT * sprIconCore->getContentSize().width);
    lbTitle->setDimensions(200, btnCore->getContentSize().height-10);
    lbTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lbTitle->setOverflow(Label::Overflow::SHRINK);
    btnCore->addChild(lbTitle);
    
    auto lbCount = Label::createWithTTF(MafUtils::format("x%d", objCore->getCount()), GAME_FONT, 24);
    lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbCount->setPosition(btnCore->getContentSize()/2);
    lbCount->setDimensions(200, btnCore->getContentSize().height-10);
    lbCount->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbCount->setOverflow(Label::Overflow::SHRINK);
    btnCore->addChild(lbCount);
    
    if(objCore->getIdx() != 5 && objCore->getIdx() != 6)
    {
        int nLv = objCore->getCurrentLv();
        auto infoLv = AdventureRelicManager::getInstance()->getInfoCoreLevelOrigin(nLv + 1);
        int nTotalEXP = 0;
        int nCurrentEXP = objCore->getExp();
        int nPercent = 0;
        bool isMaxLv = false;
        
        if(infoLv != nullptr)
        {
            nTotalEXP = infoLv->getTotalEXP();
        }
        nPercent = (float)nCurrentEXP / (float)nTotalEXP * 100;
        if ( nPercent > 100 )
            nPercent = 100;
        
        if(nLv >= AdventureRelicManager::getInstance()->getCoreMaxLv())
        {
            nPercent = 0;
            isMaxLv = true;
        }
        
        auto sprExpBg = Sprite::create("Assets/ui/adventure/adventure_progressbar3_1.png");
        sprExpBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        sprExpBg->setPosition(btnCore->getContentSize().width - 20 , btnCore->getContentSize().height/2);
        btnCore->addChild(sprExpBg);
        
        std::string strProgressPath = "Assets/ui/adventure/adventure_progressbar3_2.png";
        std::string strTimer = MafUtils::format("LV.%d", nLv);
        
        if(isMaxLv)
        {
            strProgressPath = "Assets/ui/adventure/adventure_progressbar3_3.png";
            strTimer = "LV.MAX";
        }
        
        auto progreesExp = ProgressTimer::create(Sprite::create(strProgressPath));
        progreesExp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        progreesExp->setPosition( sprExpBg->getPosition() );
        progreesExp->setMidpoint(Vec2(0, 0.5));
        progreesExp->setBarChangeRate(Vec2(1,0));
        progreesExp->setType(ProgressTimer::Type::BAR);
        progreesExp->setPercentage(nPercent);
        btnCore->addChild(progreesExp);
        {
            
            auto labelTimer = MafNode::createBmFont(strTimer, GAME_BM_FONT_AL, 16);
            labelTimer->setPosition( Vec2(progreesExp->getContentSize().width / 2, progreesExp->getContentSize().height / 2) );
            progreesExp->addChild(labelTimer);
        }
        
        if(!objCore->isActive())
        {
            btnCore->setColor(Color3B(128,128,128));
            lbTitle->setColor(Color3B(128,128,128));
            lbCount->setColor(Color3B(128,128,128));
            sprRadioBtn->setColor(Color3B(128,128,128));
            sprIconCore->setTexture(MafUtils::format("Assets/icon/icon_core_off_%02d.png",objCore->getIdx()));
            sprIconCore->setColor(Color3B(128,128,128));
        }
    }
    
    if(isCurrent)
    {
        auto const sprSelect = Sprite::create("Assets/ui/adventure/list_relic_1_on.png");
        sprSelect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprSelect->setPosition(btnCore->getContentSize()/2);
        btnCore->addChild(sprSelect);
        
        auto fadeOut = FadeOut::create(1.f);
        auto fadeIn = FadeIn::create(1.f);
        auto sequience = Sequence::create(fadeOut, fadeIn, nullptr);
        sprSelect->runAction(RepeatForever::create(sequience));
    }
    
    return cell;
}

ssize_t UILayerAdventureRelicCrafting::numberOfCellsInTableView(extension::TableView *table)
{
    return _listCore.size();
}

#pragma mark - override
void UILayerAdventureRelicCrafting::refreshUI()
{
    uiTop();
    uiMiddle();
    uiBottom();
}

void UILayerAdventureRelicCrafting::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if(listData.size() <= 0)
    {
        _table->reloadData();
    }
    else
    {
        const auto iterValue = listData.find("inspection_code");
        if(iterValue == listData.end())
        {
            _table->reloadData();
            return;
        }
        const int nInspectionCode = listData.at("inspection_code").asInt();
        std::string strText = "";
        if( nInspectionCode == 7 )
        {
            strText = "t_ui_new_raid_error_1";
        }
        else if( nInspectionCode == 8 )
        {
            strText = "t_ui_new_raid_error_2";
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strText));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        
        if(_callbackHide != nullptr)
            _callbackHide();
        _callbackHide = nullptr;
    }
}

#pragma mark - get set
void UILayerAdventureRelicCrafting::setCloseCondition(bool value)
{
    _isCloseCondition = value;
}
void UILayerAdventureRelicCrafting::setCallbackHide(const std::function<void(void)>& callback)
{
    _callbackHide = callback;
}
void UILayerAdventureRelicCrafting::setCallbackMoveShop(const std::function<void(PopupAdventureRelicDefault::E_TYPE)>& callback)
{
    _callbackMoveShop = callback;
}
#pragma mark - Tutorial
void UILayerAdventureRelicCrafting::showButtonForceTutorial()
{
    if(_btnTutorial == nullptr)
        return;
    
    auto popup = PopupButtonForceTutorial::create();
    popup->setButton(_btnTutorial);
    if(AdventureManager::getInstance()->getTutorialStep() == 3)
        popup->setCallbackListener(CC_CALLBACK_1(UILayerAdventureRelicCrafting::onClickCraft, this));
    popup->show();
    
    _btnTutorial = nullptr;
}

void UILayerAdventureRelicCrafting::showTutorial()
{
    if(AdventureManager::getInstance()->getTutorialStep() != 3)
        return;
    
    auto popup = PopupAdventureTutorial::create(AdventureManager::getInstance()->getTutorialStep());
    popup->show();
}

void UILayerAdventureRelicCrafting::requestInfo()
{
    PopupLoading::show();
    AdventureRelicManager::getInstance()->requestInfoCore(CC_CALLBACK_2(UILayerAdventureRelicCrafting::callbackInit, this));
}

#pragma mark - init
void UILayerAdventureRelicCrafting::initVar()
{
   
}

void UILayerAdventureRelicCrafting::initUi()
{
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize(Size(getContentSize().width - 10, 300));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTopUI->setPosition(getContentSize().width / 2, getContentSize().height);
    addChild(_lyTopUI);
    
    _lyMiddleUI = LayerColor::create();
    _lyMiddleUI->setContentSize( Size(getContentSize().width - 20, getContentSize().height - _lyTopUI->getContentSize().height - 300));
    _lyMiddleUI->setIgnoreAnchorPointForPosition(false);
    _lyMiddleUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyMiddleUI->setPosition( _lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    addChild(_lyMiddleUI);
    
    _lyBottomUI = LayerColor::create();
    _lyBottomUI->setContentSize( Size(getContentSize().width - 10, getContentSize().height - _lyTopUI->getContentSize().height - _lyMiddleUI->getContentSize().height -10));
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyBottomUI->setPosition( _lyMiddleUI->getPosition() + Vec2::DOWN * _lyMiddleUI->getContentSize().height);
    addChild(_lyBottomUI);
    
    //
    uiTop();
    uiMiddle();
    uiBottom();
}

#pragma mark - ui
void UILayerAdventureRelicCrafting::uiTopBG()
{
    auto sprBG = Sprite::create("Assets/ui/adventure/adventure_production_img_bg.png");
    sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBG->setPosition(_lyTopUI->getContentSize().width/2, _lyTopUI->getContentSize().height - 6);
    sprBG->getTexture()->setTexParameters(texParams);
    sprBG->setScale(3);
    _lyTopUI->addChild(sprBG);
    
    auto sprWizard = Sprite::create("Assets/ui/adventure/adventure_production_ch_00.png");
    sprWizard->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprWizard->setPosition(Vec2(_lyTopUI->getContentSize()/2) + Vec2::UP * 30);
    sprWizard->getTexture()->setTexParameters(texParams);
    sprWizard->setScale(3);
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.15f);
        for ( int i = 0; i <= 6; ++i )
        {
            auto sprFrame = Sprite::create(MafUtils::format("Assets/ui/adventure/adventure_production_ch_%02d.png", i));
            sprFrame->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(sprFrame->getSpriteFrame());
        }
        sprWizard->runAction(RepeatForever::create(Animate::create(animation)));
    }
    _lyTopUI->addChild(sprWizard);
    //211
    auto sprCore = Sprite::create(MafUtils::format("Assets/ui/adventure/relic_box_icon/production_box_%02d_00.png", _currentInfoCore->getIdx()));
    sprCore->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprCore->setPosition(_lyTopUI->getContentSize()/2);
    sprCore->getTexture()->setTexParameters(texParams);
    sprCore->setScale(3);
    _lyTopUI->addChild(sprCore);
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.15f);
        for ( int i = 0; i <= 6; ++i )
        {
            auto sprFrame = Sprite::create(MafUtils::format("Assets/ui/adventure/relic_box_icon/production_box_%02d_%02d.png",_currentInfoCore->getIdx(), i));
            sprFrame->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(sprFrame->getSpriteFrame());
        }
        sprCore->runAction(RepeatForever::create(Animate::create(animation)));
    }
    
    if(_particleFire == nullptr)
    {
        _particleFire = ParticleSnow::createWithTotalParticles(random(40, 80));
        _particleFire->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _particleFire->setPosition(_lyTopUI->getContentSize().width/2, 20);
        _particleFire->setGravity(Vec2(0, 10));
        _particleFire->setPosVar(Vec2(40, 10));
        _particleFire->setAngle(70);
        _particleFire->setSpeed(20);
        _particleFire->setSpeedVar(40);
        _particleFire->setStartSize(random(1, 8));
        _particleFire->setStartSizeVar(5);
        _particleFire->setEndSize(-1);
        _particleFire->setEndSizeVar(0);
        _particleFire->setLife(0.2f);
        _particleFire->setLifeVar(0.7f);
        _particleFire->setEmissionRate(_particleFire->getTotalParticles()/_particleFire->getLife());
        _particleFire->setTexture(Director::getInstance()->getTextureCache()->addImage("Assets/ui/team_raid/flame_s_1.png"));
        _particleFire->retain();
    }
    _lyTopUI->addChild(_particleFire);
    
    if(_particleDust == nullptr)
    {
        _particleDust = ParticleSystemQuad::create("Assets/Particle/particle_dust_1.plist");
        _particleDust->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _particleDust->setPosVar(Vec2(280, _lyTopUI->getContentSize().height/2));
        _particleDust->setPosition(_lyTopUI->getContentSize()/2);
        _particleDust->retain();
    }
    _lyTopUI->addChild(_particleDust);
}
void UILayerAdventureRelicCrafting::uiTop()
{
    _lyTopUI->removeAllChildren();
//
    uiTopBG();
    auto sprTextBox = ui::Scale9Sprite::create("Assets/ui_common/base_text_1_5.png");
    sprTextBox->setContentSize(Size(280, 40));
    sprTextBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprTextBox->setPosition(_lyTopUI->getContentSize().width/2, _lyTopUI->getContentSize().height - 10);
    _lyTopUI->addChild(sprTextBox);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_advt_relic_crafting_box_%d", _currentInfoCore->getIdx())), GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(sprTextBox->getContentSize()/2);
        lbText->setDimensions(sprTextBox->getContentSize().width - 10, sprTextBox->getContentSize().height-10);
        lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        sprTextBox->addChild(lbText);
    }
    
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    _lyTopUI->addChild(menu);
    
    auto btnInfo = ui::Button::create("Assets/ui_common/base_text_2_4.png");
    btnInfo->setScale9Enabled(true);
    btnInfo->setContentSize(Size(btnInfo->getContentSize().width - 80, btnInfo->getContentSize().height));
    btnInfo->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    btnInfo->setPosition(Vec2(5, 10));
    btnInfo->setZoomScale(0.05f);
    btnInfo->addClickEventListener(CC_CALLBACK_1(UILayerAdventureRelicCrafting::onClickHelp, this));
    _lyTopUI->addChild(btnInfo);
    {
        auto sprIcon = Sprite::create("Assets/ui/pet/btn_pet_info.png");
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprIcon->setPosition(Vec2(5, btnInfo->getContentSize().height/2));
        btnInfo->addChild(sprIcon);
        
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_raidshop_spiritbox_drawrate_pop_title"), GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbText->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        lbText->setDimensions(btnInfo->getContentSize().width - 5 - sprIcon->getContentSize().width, btnInfo->getContentSize().height - 5);
        lbText->setPosition(sprIcon->getPosition() + Vec2::RIGHT * sprIcon->getContentSize().width);
        btnInfo->addChild(lbText);
    }
    
    if(_currentInfoCore->getIdx() != 6 && _currentInfoCore->getIdx() != 5)
    {
        auto sprBoxDropAreaInfo = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_5.png");
        sprBoxDropAreaInfo->setContentSize(Size(186, 100));
        sprBoxDropAreaInfo->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        sprBoxDropAreaInfo->setPosition(btnInfo->getPosition() + Vec2::UP * (btnInfo->getContentSize().height + 10));
        _lyTopUI->addChild(sprBoxDropAreaInfo);
        
        auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_advt_core_drop_info_1"), GAME_FONT, 24);
        lbTitle->setPosition(sprBoxDropAreaInfo->getContentSize().width / 2, sprBoxDropAreaInfo->getContentSize().height - 15.5);
        sprBoxDropAreaInfo->addChild(lbTitle);
        
        auto lbDropAreaInfo = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_advt_tutorial_core_unlock_%d", _currentInfoCore->getIdx())), GAME_FONT, 24);
        lbDropAreaInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbDropAreaInfo->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbDropAreaInfo->setDimensions(sprBoxDropAreaInfo->getContentSize().width - 10, sprBoxDropAreaInfo->getContentSize().height - 40);
        lbDropAreaInfo->setPosition(sprBoxDropAreaInfo->getContentSize().width/2 , (sprBoxDropAreaInfo->getContentSize().height - 33) /2);
        sprBoxDropAreaInfo->addChild(lbDropAreaInfo);
    }
}

void UILayerAdventureRelicCrafting::uiMiddle()
{
    _lyMiddleUI->removeAllChildren();
    
    _table = extension::TableView::create(this, Size(_lyMiddleUI->getContentSize().width, _lyMiddleUI->getContentSize().height));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _table->setPosition(_lyMiddleUI->getContentSize().width / 2, _lyMiddleUI->getContentSize().height);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lyMiddleUI->addChild(_table);

    //
    _table->reloadData();
}
void UILayerAdventureRelicCrafting::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    const int nPrice = AdventureRelicManager::getCraftPrice(_currentInfoCore->getIdx(), 1);
    const int nAllCount = _currentInfoCore->getCount() / nPrice >= 30 ? 30 : _currentInfoCore->getCount() / nPrice;
    
    std::vector<int> listCoreCount = {1, 10, nAllCount};
    
    int size = (int)listCoreCount.size();
    
    float width = 190;
    float posX = _lyBottomUI->getContentSize().width/2 - width * (size/2) - 10 * (size/2);
    
    for(int i = 0; i < listCoreCount.size(); ++i)
    {
        const int cnt = listCoreCount[i];
        auto btnCraft = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
        btnCraft->setScale9Enabled(true);
        btnCraft->setContentSize(Size(190,107));
        btnCraft->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        btnCraft->setPosition(Vec2(posX, _lyBottomUI->getContentSize().height - 30));
        btnCraft->addClickEventListener(CC_CALLBACK_1(UILayerAdventureRelicCrafting::onClickCraft, this));
        btnCraft->setZoomScale(0.05f);
        btnCraft->setTag(cnt);
        _lyBottomUI->addChild(btnCraft);
        {
            auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_relic_crafting_bt") + MafUtils::format("x%d", cnt), GAME_FONT, 26);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbText->setPosition(btnCraft->getContentSize().width/2, btnCraft->getContentSize().height - btnCraft->getContentSize().height/4);
            lbText->setDimensions(btnCraft->getContentSize().width - 10, btnCraft->getContentSize().height/2);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbText->setOverflow(Label::Overflow::SHRINK);
            btnCraft->addChild(lbText);
            
            auto sprCurrencyIcon = Sprite::create(MafUtils::format("Assets/icon/icon_core_%02d.png",_currentInfoCore->getIdx()));
            sprCurrencyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprCurrencyIcon->setPosition(btnCraft->getContentSize().width/2.6f, btnCraft->getContentSize().height/4);
            btnCraft->addChild(sprCurrencyIcon);
            
            auto lbCount = Label::createWithTTF(std::to_string(AdventureRelicManager::getCraftPrice(_currentInfoCore->getIdx(), cnt)), GAME_FONT, 26);
            lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            lbCount->setPosition(sprCurrencyIcon->getPosition() + Vec2::RIGHT * sprCurrencyIcon->getContentSize().width/2);
            lbCount->setDimensions(100, btnCraft->getContentSize().height/2);
            lbCount->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
            lbCount->setOverflow(Label::Overflow::SHRINK);
            btnCraft->addChild(lbCount);
        }
        
        if(AdventureManager::getInstance()->getTutorialStep() == 3 && cnt == 10)
            _btnTutorial = btnCraft;
        
        if(size % 2 == 0)
            btnCraft->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        
        posX += width + 10;
    }
    
    
    auto sprFriendPointBox = ui::Scale9Sprite::create("Assets/ui_common/list_bg_s_2_1.png");
    sprFriendPointBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprFriendPointBox->setContentSize(Size(712, 102));
    sprFriendPointBox->setPosition(_lyBottomUI->getContentSize().width/2, 10);
    _lyBottomUI->addChild(sprFriendPointBox);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_relic_crafting_box_7"), GAME_FONT, 26);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbText->setPosition(15, sprFriendPointBox->getContentSize().height - sprFriendPointBox->getContentSize().height/4);
        lbText->setDimensions(sprFriendPointBox->getContentSize().width - 220, sprFriendPointBox->getContentSize().height/2);
        lbText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        sprFriendPointBox->addChild(lbText);
        
        int nPercent = 0;
        int nCurrentCount = 0;
        auto objCore = AdventureRelicManager::getInstance()->getInfoCore(7);
        if(objCore != nullptr)
            nCurrentCount = objCore->getCount();
        
        nPercent = (float)nCurrentCount / (float)AdventureRelicManager::getInstance()->getCraftPrice(7, 1) * 100;
        if ( nPercent > 100 )
            nPercent = 100;
        
        auto sprExpBg = Sprite::create("Assets/ui/adventure/adventure_progressbar2_1.png");
        sprExpBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprExpBg->setPosition(15 , sprFriendPointBox->getContentSize().height/4);
        sprFriendPointBox->addChild(sprExpBg);
        
        std::string strProgressPath = "Assets/ui/adventure/adventure_progressbar2_2.png";
        
        auto progreesPoint = ProgressTimer::create(Sprite::create(strProgressPath));
        progreesPoint->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        progreesPoint->setPosition( sprExpBg->getPosition() );
        progreesPoint->setMidpoint(Vec2(0, 0.5));
        progreesPoint->setBarChangeRate(Vec2(1,0));
        progreesPoint->setType(ProgressTimer::Type::BAR);
        progreesPoint->setPercentage(nPercent);
        sprFriendPointBox->addChild(progreesPoint);
        {
            std::string strPoint = "%d/%d";
            
            auto labelTimer = MafNode::createBmFont(MafUtils::format(strPoint.c_str(), objCore->getCount(), AdventureRelicManager::getInstance()->getCraftPrice(7, 1)), GAME_BM_FONT_AL, 25);
            labelTimer->setPosition( Vec2(progreesPoint->getContentSize().width / 2, progreesPoint->getContentSize().height / 2) );
            labelTimer->setDimensions(progreesPoint->getContentSize().width, progreesPoint->getContentSize().height);
            labelTimer->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelTimer->setOverflow(Label::Overflow::SHRINK);
            progreesPoint->addChild(labelTimer);
        }
        
        
        auto btnFriendCraft = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
        btnFriendCraft->setScale9Enabled(true);
        btnFriendCraft->setContentSize(Size(190,78));
        btnFriendCraft->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        btnFriendCraft->setPosition(Vec2(_lyBottomUI->getContentSize().width - 15, sprFriendPointBox->getContentSize().height/2));
        btnFriendCraft->addClickEventListener(CC_CALLBACK_1(UILayerAdventureRelicCrafting::onClickFriendCraft, this));
        btnFriendCraft->setZoomScale(0.05f);
        btnFriendCraft->setTitleText(GAME_TEXT("t_ui_advt_relic_crafting_bt"));
        btnFriendCraft->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        btnFriendCraft->getTitleLabel()->setDimensions(btnFriendCraft->getContentSize().width - 10, btnFriendCraft->getContentSize().height - 5);
        btnFriendCraft->setTitleFontSize(26);
        btnFriendCraft->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
        sprFriendPointBox->addChild(btnFriendCraft);
    }
    
}

#pragma mark -utils
void UILayerAdventureRelicCrafting::onClickHelp(Ref* sender)
{
    auto popup = PopupAdventureRelicCraftRateInfo::create();
    popup->show();
}

void UILayerAdventureRelicCrafting::onClickCore(Ref* sender)
{
    auto node = (Node*)sender;
    
    auto tmpCore = getCore(node->getTag());
    if((tmpCore->getIdx() != 5 && tmpCore->getIdx() != 6) && !tmpCore->isActive())
    {
        Vec2 posToast = node->getParent()->convertToWorldSpace(node->getPosition() + Vec2::RIGHT * node->getContentSize().width/2 + Vec2::DOWN * node->getContentSize().height/2);
        
        std::string strText = "";
        
        PopupToast::showImmediately(GAME_TEXT(MafUtils::format("t_ui_advt_tutorial_core_unlock_%d", tmpCore->getIdx())), posToast.x, posToast.y);
        return;
    }
    _currentInfoCore = tmpCore;
    
    uiTop();
    uiBottom();
    _table->reloadData();
}

void UILayerAdventureRelicCrafting::onClickCraft(Ref* sender)
{
    auto node = (Node*)sender;
    
    if(node == nullptr)
        return;
    
    const int nPrice = AdventureRelicManager::getCraftPrice(_currentInfoCore->getIdx(), node->getTag());
    
    if(_currentInfoCore->getCount() < nPrice || nPrice <= 0)
    {
        if(_currentInfoCore->getIdx() == 6)
        {
            Vec2 posToast = _lyMiddleUI->getParent()->convertToWorldSpace(_lyMiddleUI->getPosition());
            
            PopupToast::showImmediately(GAME_TEXT("t_ui_advt_arror_7"), posToast.x, posToast.y);
            return;
        }
        else
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::ADVENTURE_CORE, PopupCurrencyDetail::E_TYPE::NONE, _currentInfoCore->getIdx());
            popup->setCallbackTransferShop(CC_CALLBACK_0(UILayerAdventureRelicCrafting::callbackTransferShop, this));
            popup->show();
            return;
        }
    }
    
    PopupLoading::show();
    AdventureRelicManager::getInstance()->requestCraftRelic(CC_CALLBACK_3(UILayerAdventureRelicCrafting::callbackCraft, this), _currentInfoCore->getIdx(), node->getTag());
}

void UILayerAdventureRelicCrafting::onClickFriendCraft(Ref* sender)
{
    auto node = (Node*)sender;
    
    const int nPrice = AdventureRelicManager::getCraftPrice(7, 1);
    auto objCore = AdventureRelicManager::getInstance()->getInfoCore(7);
    
    if(objCore->getCount() < nPrice)
    {
        Vec2 posToast = node->getParent()->convertToWorldSpace(node->getPosition());
        
        PopupToast::showImmediately(GAME_TEXT("t_ui_advt_arror_8"), _lyBottomUI->getContentSize().width/2, posToast.y);
        return;
    }
    const int nCount = objCore->getCount() / nPrice;
    
    PopupLoading::show();
    AdventureRelicManager::getInstance()->requestCraftRelic(CC_CALLBACK_3(UILayerAdventureRelicCrafting::callbackFriendCraft, this), objCore->getIdx(), nCount);
}

InfoAdventureCore* UILayerAdventureRelicCrafting::getCore( int nIdx)
{
    InfoAdventureCore* result = nullptr;
       
    for(auto obj : _listCore)
    {
        if(obj->getIdx() == nIdx)
        {
            result = obj;
            break;
        }
    }
       
    return result;
}

void UILayerAdventureRelicCrafting::setCoreList()
{
    _listCore.clear();
    
    bool isGrindOpen = false;
    
    auto objSettingGrindOpen = AdventureManager::getInstance()->getSetting("reric_grind_switch");
    if(objSettingGrindOpen != nullptr)
        isGrindOpen = objSettingGrindOpen->getValueToBool();
    
    for(int i = 0; i < 6; ++i)
    {
        auto objCore = AdventureRelicManager::getInstance()->getInfoCore(i+1);
        
        if(objCore->getIdx() == 6 && !isGrindOpen)
            continue;
        
        if(objCore != nullptr)
            _listCore.pushBack(objCore);
    }
    
    std::sort(_listCore.begin(), _listCore.end(), UILayerAdventureRelicCrafting::sortCore);
    
}

#pragma mark -Callback
void UILayerAdventureRelicCrafting::callbackInit( bool bResult,  int nResult)
{
    PopupLoading::hide();
    
    setCoreList();
    
    if(AdventureManager::getInstance()->getTutorialStep() == 3)
        _currentInfoCore = getCore(1);
    else
        _currentInfoCore = getCore(5);
    
    // init
    refreshUI();
   
}

void UILayerAdventureRelicCrafting::callbackCraft(bool bResult,  int nResult, cocos2d::Vector<InfoItems*> list)
{
    PopupLoading::hide();
    
    if(nResult != 1)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if(nResult == 7)
            strMessageKey = "t_ui_advt_error_2";
        else if(nResult == 8)
            strMessageKey = "t_ui_advt_error_3";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        
        if(nResult == 7 || nResult == 8)
        {
            ValueMap listData;
            listData["inspection_code"] = Value(nResult);
            RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE, listData);
        }
        
        if(_callbackHide != nullptr)
            _callbackHide();
        _callbackHide = nullptr;
        return;
    }
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    
    if(AdventureManager::getInstance()->getTutorialStep() > 6)
        RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE);
    
    auto popup = PopupAdventureRelicCraft::create();
    popup->setReward(list);
    popup->setCore(_currentInfoCore);
    if(AdventureManager::getInstance()->getTutorialStep() == 3)
        popup->setCallbackTutorial(CC_CALLBACK_0(UILayerAdventureRelicCrafting::callbackTutorial, this));
    popup->setCallbackTransferShop(CC_CALLBACK_0(UILayerAdventureRelicCrafting::callbackTransferShop, this));
    popup->show();
}

void UILayerAdventureRelicCrafting::callbackFriendCraft(bool bResult,  int nResult, cocos2d::Vector<InfoItems*> list)
{
    PopupLoading::hide();
    
    if(nResult != 1)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if(nResult == 7)
            strMessageKey = "t_ui_advt_error_2";
        else if(nResult == 8)
            strMessageKey = "t_ui_advt_error_3";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        
        if(nResult == 7 || nResult == 8)
        {
            ValueMap listData;
            listData["inspection_code"] = Value(nResult);
            RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE, listData);
        }
        
        if(_callbackHide != nullptr)
            _callbackHide();
        _callbackHide = nullptr;
        return;
    }
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE);
    
    auto popup = PopupAdventureRelicCraft::create();
    popup->setReward(list);
    popup->setCore(AdventureRelicManager::getInstance()->getInfoCore(7));
    //popup->setCallbackTransferShop(CC_CALLBACK_0(UILayerAdventureRelicCrafting::callbackTransferShop, this));
    popup->show();
}

void UILayerAdventureRelicCrafting::callbackTutorial()
{
    AdventureManager::getInstance()->setTutorialStep(AdventureManager::getInstance()->getTutorialStep() + 1);
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE);
}

void UILayerAdventureRelicCrafting::callbackTransferShop()
{
    if(_callbackMoveShop != nullptr)
        _callbackMoveShop(PopupAdventureRelicDefault::E_TYPE::SHOP);
}
