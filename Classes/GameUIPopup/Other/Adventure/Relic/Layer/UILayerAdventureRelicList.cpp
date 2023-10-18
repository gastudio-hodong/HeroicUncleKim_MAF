//
//  UILayerAdventureRelicList.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/04.
//

#include "UILayerAdventureRelicList.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Common/PopupLoadingSequentialUpgrade.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"
#include "GameUIPopup/Other/Ads/PopupAdsWatch.h"
#include "GameUIPopup/Other/Adventure/Relic/PopupAdventureRelicCollect.h"
#include "GameUIPopup/Other/Adventure/Relic/PopupAdventureRelicGrind.h"
#include "GameUIPopup/Other/Adventure/PopupAdventureUpgradeComplete.h"
#include "GameUIPopup/Other/Adventure/PopupAdventureTutorial.h"
#include "GameUIPopup/Other/Adventure/Relic/PopupAdventureRelicTotalSpec.h"

#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/AdventureRelicManager.h"

USING_NS_CC;

bool UILayerAdventureRelicList::sortRelic(InfoAdventureRelic* const a, InfoAdventureRelic* const b)
{
    if(a == nullptr)
        return true;
    else if(b == nullptr)
        return false;
    
    bool result = a->getGrade() < b->getGrade();
    
    if(a->getGrade() == b->getGrade())
        result = a->getIdx() < b->getIdx();
    
    return result;
}


UILayerAdventureRelicList* UILayerAdventureRelicList::create(cocos2d::Size size)
{
    UILayerAdventureRelicList* pRet = new(std::nothrow) UILayerAdventureRelicList();
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

UILayerAdventureRelicList::UILayerAdventureRelicList(void):
_isCloseCondition(false)
,_nUpgradeRelicIdx(0)
,_nCurrentLevelUpIdx(0)
,_eType(UILayerAdventureRelicList::E_TYPE::FOREST)
,_lyTopUI(nullptr)
,_lyBottomUI(nullptr)
,_callbackHide(nullptr)
,_table(nullptr)
{
    _listRelic.clear();
}

UILayerAdventureRelicList::~UILayerAdventureRelicList(void)
{
    _listRelic.clear();
}

bool UILayerAdventureRelicList::init(cocos2d::Size size)
{
    if (!UILayerAdventureParent::init())
    {
        return false;
    }
    setContentSize(size);
    
    _listRelic = AdventureRelicManager::getInstance()->getListInfoRelicFromArea((int)_eType);
    std::sort(_listRelic.begin(), _listRelic.end(), UILayerAdventureRelicList::sortRelic);
    //
    
    RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE, this);
    
    // init
    initVar();
    initUi();
    
    return true;
}

void UILayerAdventureRelicList::onEnter()
{
    UILayerAdventureParent::onEnter();
    
    PopupLoading::show();
    AdventureRelicManager::getInstance()->requestInfoRelic(CC_CALLBACK_2(UILayerAdventureRelicList::callbackInfo, this));
}

#pragma mark - table
void UILayerAdventureRelicList::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
}

Size UILayerAdventureRelicList::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 140);
}

extension::TableViewCell* UILayerAdventureRelicList::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    
    auto objRelic = _listRelic.at(idx);
    
    if(objRelic == nullptr)
        return cell;
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/list_bg_s_1_1.png",Rect::ZERO, Rect(13, 0, 30, 78));
    sprBox->setContentSize(cell->getContentSize() - Size(10, 10));
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setPosition(cell->getContentSize()/2);
    cell->addChild(sprBox);
    
    auto sprRelicBox = ui::Scale9Sprite::create(MafUtils::format("Assets/ui/adventure/adventure_box_%02d.png", objRelic->getGrade()));
    sprRelicBox->setContentSize(Size(sprBox->getContentSize().height - 20, sprBox->getContentSize().height - 20));
    sprRelicBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprRelicBox->setPosition(10, sprBox->getContentSize().height / 2);
    sprBox->addChild(sprRelicBox);
    {
        auto sprIconAreaCore = Sprite::create(MafUtils::format("Assets/icon/icon_core_%02d.png", objRelic->getArea()));
        sprIconAreaCore->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        sprIconAreaCore->setScale(0.5f);
        sprIconAreaCore->getTexture()->setTexParameters(texParams);
        sprIconAreaCore->setPosition(0, sprRelicBox->getContentSize().height);
        sprRelicBox->addChild(sprIconAreaCore);
        
        auto sprRelic = Sprite::create(MafUtils::format("Assets/ui/adventure/adventure_relic_%d.png",objRelic->getIdx()));
        sprRelic->getTexture()->setTexParameters(texParams);
        sprRelic->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprRelic->setPosition(sprRelicBox->getContentSize()/2);
        //sprRelic->setScale(sprRelicBox->getContentSize().width/2);
        sprRelicBox->addChild(sprRelic);
        
        //
        auto const labelLv = Label::createWithTTF(MafUtils::format("Lv.%02d",objRelic->getCurrentLv()), GAME_FONT, 22);
        labelLv->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        labelLv->setPosition(sprRelicBox->getContentSize().width/2, 3);
        sprRelicBox->addChild(labelLv);
        
        auto const labelCount = Label::createWithTTF(MafUtils::format("x%02d",objRelic->getCount()), GAME_FONT, 22);
        labelCount->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        labelCount->setPosition(sprRelicBox->getContentSize().width - 5, sprRelicBox->getContentSize().height - 5);
        sprRelicBox->addChild(labelCount);

    }
    
    auto sprGrade = ui::Scale9Sprite::create("Assets/ui_common/btn_2_6_on.png");
    sprGrade->setContentSize(Size(55, 30));
    sprGrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprGrade->setPosition(sprRelicBox->getPositionX() + sprRelicBox->getContentSize().width + 10, sprBox->getContentSize().height - 25);
    sprBox->addChild(sprGrade);
    {
        auto sprIcon = Sprite::create("Assets/ui_common/icon_star1_2.png");
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprIcon->setPosition(5, sprGrade->getContentSize().height/2);
        sprGrade->addChild(sprIcon);
        
        auto lbCount = Label::createWithTTF(MafUtils::format("%d",objRelic->getGrade()), GAME_FONT, 25);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbCount->setPosition(sprIcon->getPosition() + Vec2::RIGHT * (sprIcon->getContentSize().width + 3));
        lbCount->setDimensions(sprGrade->getContentSize().width - 5 - sprIcon->getContentSize().width - 8, sprGrade->getContentSize().height);
        lbCount->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbCount->setOverflow(Label::Overflow::SHRINK);
        lbCount->enableBold();
        lbCount->setColor(COLOR_COIN);
        sprGrade->addChild(lbCount);
    }
    
    auto lbName = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_advt_relic_name_%d", objRelic->getIdx())), GAME_FONT, 20);
    lbName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbName->setPosition(sprGrade->getPosition() + Vec2::RIGHT * (sprGrade->getContentSize().width + 10));
    lbName->setDimensions(250, 45);
    lbName->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbName->setOverflow(Label::Overflow::SHRINK);
    sprBox->addChild(lbName);
    
    std::string strEffect = GAME_TEXT(MafUtils::format("t_ui_advt_property_type_%d", (int)objRelic->getType()));

    strEffect += "\n" + MafUtils::format(" %s", AdventureManager::getPropertyEffectText(objRelic->getType(), objRelic->getAbilityEffect()).c_str());

    if(objRelic->isActive() && objRelic->getCurrentLv() < AdventureRelicManager::getInstance()->getRelicMaxLv())
    {
        std::string strEffectTmp = "{#00FF1A:(+%s)}";

        if(objRelic->getType() == E_ADVENTURE_PROPERTY_TYPE::PARTY_GEM_REWARD_INCREASE ||
           objRelic->getType() == E_ADVENTURE_PROPERTY_TYPE::PRINCESSES_SKILL_FLOOR_INCREASE)
            strEffectTmp = " {#00FF1A:(%s)}";

        strEffect += MafUtils::format(strEffectTmp.c_str(), AdventureManager::getPropertyEffectText(objRelic->getType(), objRelic->getAbilityEffect(objRelic->getCurrentLv() + 1) - objRelic->getAbilityEffect()).c_str());
    }
    
    auto const menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    sprBox->addChild(menu);
    
    auto lbEffect = ui::CText::create(strEffect, GAME_FONT, 24);
    lbEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbEffect->setPosition(Vec2(sprRelicBox->getPositionX() + sprRelicBox->getContentSize().width + 10, 36));
    lbEffect->setTextVerticalAlignment(TextVAlignment::CENTER);
    lbEffect->setTextHorizontalAlignment(TextHAlignment::LEFT);
    lbEffect->setTextOverflow(Label::Overflow::SHRINK);
    lbEffect->setTextAreaSize(Size(300, 65));
    sprBox->addChild(lbEffect);
    {
        auto area = Layer::create();
        area->setContentSize(lbEffect->getContentSize());
        
        auto btn = MafNode::MafMenuItemSprite::create(area, nullptr, nullptr, [&, cell, objRelic](Ref*){
            double dCoef = AdventureRelicManager::getInstance()->getRelicTotalEffectCoefficient(objRelic->getType());
            double dEffect = objRelic->getAbilityEffect();
            
            if(!objRelic->isActive())
                dEffect = 0;
            
            std::string strEffect = AdventureManager::getPropertyEffectText(objRelic->getType(), dEffect * dCoef);
            
            std::string strText = GAME_TEXTFORMAT("t_ui_advt_relic_spec_toast_1", strEffect.c_str());
            
            Vec2 posToast = cell->getParent()->convertToWorldSpace(cell->getPosition() + cell->getContentSize()/2);
            PopupToast::showImmediately(strText, posToast.x, posToast.y);

        });
        btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        btn->setPosition(lbEffect->getContentSize()/2);
        menu->addChild(btn);
    }
    
    auto const sprDisable = ui::Scale9Sprite::create(PIXEL, Rect(0, 0, sprBox->getContentSize().width, sprBox->getContentSize().height));
    sprDisable->setPosition(sprBox->getContentSize() / 2);
    sprDisable->setColor(Color3B::BLACK);
    sprDisable->setOpacity(150);
    sprDisable->setVisible(false);
    sprBox->addChild(sprDisable);
    
    if(objRelic->isActive() && objRelic->getCurrentLv() != AdventureRelicManager::getInstance()->getRelicMaxLv())
    {
        std::string strBtnPath = "Assets/ui_common/btn_upgrade_bg_1.png";
        if(objRelic->getCount() < AdventureRelicManager::getInstance()->getRelicLevelUpPrice(objRelic->getIdx(), 1))
            strBtnPath = "Assets/ui_common/btn_upgrade_bg_1_max.png";
        
        auto sprBtnRelicLvUp = ui::Scale9Sprite::create(strBtnPath);
        sprBtnRelicLvUp->setContentSize(Size(100, 90));
        auto sprBtnRelicLvUpSelect = ui::Scale9Sprite::create(strBtnPath);
        sprBtnRelicLvUpSelect->setContentSize(Size(100, 90));
        
        auto btnRelicLvUp = MafNode::MafMenuItemSprite::create(sprBtnRelicLvUp,
                                                      sprBtnRelicLvUpSelect,
                                                      nullptr,
                                                      CC_CALLBACK_1(UILayerAdventureRelicList::onClickLevelup, this));
        btnRelicLvUp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        btnRelicLvUp->setPosition(Vec2(sprBox->getContentSize().width - 10, sprBox->getContentSize().height/2));
        btnRelicLvUp->setTag(objRelic->getIdx());
        menu->addChild(btnRelicLvUp);
        {
            auto lbCount = Label::createWithTTF(MafUtils::format("%02d / %02d", objRelic->getCount(), AdventureRelicManager::getInstance()->getRelicLevelUpPrice(objRelic->getIdx(), 1)), GAME_FONT, 20);
            lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            lbCount->setPosition(Vec2(btnRelicLvUp->getContentSize()/2) + Vec2::UP * 5);
            lbCount->setDimensions(btnRelicLvUp->getContentSize().width - 20, lbCount->getContentSize().height);
            lbCount->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
            lbCount->setOverflow(Label::Overflow::SHRINK);
            btnRelicLvUp->addChild(lbCount);
            
            auto lbText = Label::createWithTTF("LEVEL UP", GAME_FONT, 25);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            lbText->setPosition(Vec2(btnRelicLvUp->getContentSize()/2) + Vec2::DOWN * 5);
            lbText->setDimensions(btnRelicLvUp->getContentSize().width - 20, lbText->getContentSize().height);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
            lbText->setOverflow(Label::Overflow::SHRINK);
            btnRelicLvUp->addChild(lbText);
            
            if(objRelic->isReddotCondition())
            {
                auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
                uiReddot->setPosition(btnRelicLvUp->getContentSize().width - 5, btnRelicLvUp->getContentSize().height - 5);
                uiReddot->setScale(0.8f);
                btnRelicLvUp->addChild(uiReddot);
            }
        }
        
    }
    
    if(!objRelic->isActive() )
    {
        sprDisable->setVisible(true);

        lbName->setColor(Color3B(128,128,128));
        lbEffect->setColor(Color3B(128,128,128));
    }
    
    return cell;
}

ssize_t UILayerAdventureRelicList::numberOfCellsInTableView(extension::TableView *table)
{
    return _listRelic.size();
}

#pragma mark - override
void UILayerAdventureRelicList::refreshUI()
{
    uiTop();
    uiBottom();
    auto offset = _table->getContentOffset();
    _table->reloadData();
    _table->setContentOffset(offset);
}

void UILayerAdventureRelicList::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if(listData.size() <= 0)
    {
        refreshUI();
    }
    else
    {
        const auto iterValue = listData.find("inspection_code");
        if(iterValue == listData.end())
        {
            refreshUI();
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
void UILayerAdventureRelicList::setCloseCondition(bool value)
{
    _isCloseCondition = value;
}
void UILayerAdventureRelicList::setCallbackHide(const std::function<void(void)>& callback)
{
    _callbackHide = callback;
}

#pragma mark - Tutorial
void UILayerAdventureRelicList::showTutorial()
{
    if(AdventureManager::getInstance()->getTutorialStep() != 4)
        return;
    
    auto popup = PopupAdventureTutorial::create(AdventureManager::getInstance()->getTutorialStep());
    popup->show();
}
#pragma mark - init
void UILayerAdventureRelicList::initVar()
{
    
}

void UILayerAdventureRelicList::initUi()
{
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize(Size(getContentSize().width - 10, 150));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTopUI->setPosition(getContentSize().width / 2, getContentSize().height);
    addChild(_lyTopUI, E_ZORER::UI + 1);
    
    _lyTableUI = LayerColor::create();
    _lyTableUI->setContentSize( Size(getContentSize().width - 20, getContentSize().height - _lyTopUI->getContentSize().height - 120));
    _lyTableUI->setIgnoreAnchorPointForPosition(false);
    _lyTableUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTableUI->setPosition( _lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    addChild(_lyTableUI, E_ZORER::UI);
    
    _lyBottomUI = LayerColor::create();
    _lyBottomUI->setContentSize( Size(getContentSize().width - 10, getContentSize().height - _lyTopUI->getContentSize().height - _lyTableUI->getContentSize().height -10));
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyBottomUI->setPosition( _lyTableUI->getPosition() + Vec2::DOWN * _lyTableUI->getContentSize().height);
    addChild(_lyBottomUI, E_ZORER::UI +1);
    
    //
    uiTop();
    uiTable();
    uiBottom();
}

#pragma mark - init
void UILayerAdventureRelicList::uiTop()
{
    _lyTopUI->removeAllChildren();
    if(AdventureManager::getInstance()->getTutorialStep() >= 7)
    {
        auto btnRelicSpec = ui::Button::create("Assets/ui/adventure/adventure_relic_info_btn_box.png");
        btnRelicSpec->setScale9Enabled(true);
        btnRelicSpec->setContentSize(Size(90, 51));
        btnRelicSpec->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        btnRelicSpec->setPosition(Vec2(30, _lyTopUI->getContentSize().height - 10));
        btnRelicSpec->addClickEventListener(CC_CALLBACK_1(UILayerAdventureRelicList::onClickRelicTotalSpec, this));
        btnRelicSpec->setZoomScale(0.03f);
        _lyTopUI->addChild(btnRelicSpec);
        {
            auto lbText = Label::createWithTTF("TOTAL", GAME_FONT, 20);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbText->setPosition(btnRelicSpec->getContentSize()/2);
            lbText->setDimensions(btnRelicSpec->getContentSize().width - 20, btnRelicSpec->getContentSize().height - 20);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbText->setOverflow(Label::Overflow::SHRINK);
            lbText->setColor(Color3B(90,230,180));
            btnRelicSpec->addChild(lbText);
        }
    }
    
    auto btnAllLevelUP = ui::Button::create("Assets/ui/adventure/adventure_relic_info_btn_box.png");
    btnAllLevelUP->setScale9Enabled(true);
    btnAllLevelUP->setContentSize(Size(120, 51));
    btnAllLevelUP->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    btnAllLevelUP->setPosition(Vec2(_lyTopUI->getContentSize().width - 30, _lyTopUI->getContentSize().height - 10));
    btnAllLevelUP->addClickEventListener(CC_CALLBACK_1(UILayerAdventureRelicList::onClickAllLevelUp, this));
    btnAllLevelUP->setZoomScale(0.03f);
    _lyTopUI->addChild(btnAllLevelUP);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_relic_levelup_all_bt"), GAME_FONT, 20);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(btnAllLevelUP->getContentSize()/2);
        lbText->setDimensions(btnAllLevelUP->getContentSize().width - 20, btnAllLevelUP->getContentSize().height - 20);
        lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        lbText->setColor(Color3B(90,230,180));
        btnAllLevelUP->addChild(lbText);
    }
    
    std::vector<UILayerAdventureRelicList::E_TYPE> listTab = {
        UILayerAdventureRelicList::E_TYPE::FOREST,
        UILayerAdventureRelicList::E_TYPE::DESERT,
        UILayerAdventureRelicList::E_TYPE::ICE,
        UILayerAdventureRelicList::E_TYPE::VOLCANO
    };
    
    int size = (int)listTab.size();
    
    float width = (_lyTopUI->getContentSize().width - 30) / size - 10;
    float posX = _lyTopUI->getContentSize().width/2 - width * (size/2) - 8 * (size/2);
    
    for(int i = 0; i < listTab.size(); ++i)
    {
        auto obj = listTab[i];
        
        std::string strPath = "Assets/ui_common/btn_tap_2_off.png";
        std::string strIconPath = "Assets/icon/icon_core_off_%02d.png";
        if(obj == _eType)
        {
            strPath = "Assets/ui_common/btn_tap_2_on.png";
            strIconPath = "Assets/icon/icon_core_%02d.png";
        }
        
        auto btnTab = ui::Button::create(strPath);
        btnTab->setScale9Enabled(true);
        btnTab->setContentSize(Size(width, 67));
        btnTab->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        btnTab->setPosition(Vec2(posX, 10));
        btnTab->addClickEventListener(CC_CALLBACK_1(UILayerAdventureRelicList::onClickTab, this));
        btnTab->setZoomScale(0.05f);
        btnTab->setTag((int)obj);
        _lyTopUI->addChild(btnTab);
        {
            auto sprIcon = Sprite::create(MafUtils::format(strIconPath.c_str(), (int)obj));
            sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            sprIcon->setPosition(Vec2(20, btnTab->getContentSize().height/2));
            sprIcon->setScale(0.6f);
            btnTab->addChild(sprIcon);
            
            auto lbArea = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_advt_relic_list_bt_%d",i+1)), GAME_FONT, 25);
            lbArea->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            lbArea->setPosition(sprIcon->getPosition() + Vec2::RIGHT * (sprIcon->getContentSize().width * sprIcon->getScale() + 5));
            lbArea->setDimensions(btnTab->getContentSize().width - sprIcon->getContentSize().width * sprIcon->getScale() - 20, btnTab->getContentSize().height - 5);
            lbArea->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
            lbArea->setOverflow(Label::Overflow::SHRINK);
            btnTab->addChild(lbArea);
        }
        
        if(size % 2 == 0)
            btnTab->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        
        if(AdventureRelicManager::getInstance()->isRelicReddotConditionFromArea((int)obj))
        {
            auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
            uiReddot->setPosition(10, btnTab->getContentSize().height - 10);
            btnTab->addChild(uiReddot);
        }
        
        posX += width + 10;
    }
    
}
void UILayerAdventureRelicList::uiTable()
{
    _lyTableUI->removeAllChildren();
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
    sprBox->setContentSize(Size(_lyTableUI->getContentSize().width - 10, _lyTableUI->getContentSize().height));
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setPosition(_lyTableUI->getContentSize()/2);
    _lyTableUI->addChild(sprBox);
    
    _table = extension::TableView::create(this, Size(_lyTableUI->getContentSize().width - 20, _lyTableUI->getContentSize().height - 10));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _table->setPosition(_lyTableUI->getContentSize().width / 2, _lyTableUI->getContentSize().height - 5);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lyTableUI->addChild(_table);
    
    //
    _table->reloadData();
}
void UILayerAdventureRelicList::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_3.png");
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setContentSize(Size(_lyBottomUI->getContentSize().width - 10, 70));
    sprBox->setPosition(_lyBottomUI->getContentSize()/2);
    _lyBottomUI->addChild(sprBox);
    
    auto sprIcon = Sprite::create("Assets/ui/bonsikdex/icon_book.png");
    sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprIcon->setPosition(10, sprBox->getContentSize().height/2);
    sprIcon->getTexture()->setTexParameters(texParams);
    sprIcon->setScale(1.7f);
    sprBox->addChild(sprIcon);
    
    auto btnCollect = ui::Button::create("Assets/ui_common/btn_2_4_on.png");
    btnCollect->setScale9Enabled(true);
    btnCollect->setContentSize(Size(226, 70));
    btnCollect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnCollect->setPosition(Vec2(sprBox->getContentSize().width, sprBox->getContentSize().height/2));
    btnCollect->addClickEventListener(CC_CALLBACK_1(UILayerAdventureRelicList::onClickCollection, this));
    btnCollect->setZoomScale(0.05f);
    btnCollect->setTitleText(GAME_TEXT("t_ui_advt_info_title_9"));
    btnCollect->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    btnCollect->getTitleLabel()->setDimensions(btnCollect->getContentSize().width - 10, btnCollect->getContentSize().height - 5);
    btnCollect->setTitleFontSize(26);
    btnCollect->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
    sprBox->addChild(btnCollect);
    {
        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
        uiReddot->setPosition(10, btnCollect->getContentSize().height - 10);
        uiReddot->setVisible(false);
        btnCollect->addChild(uiReddot);
        
        auto action1 = CallFuncN::create([=](Ref* sender){
            auto uiReddot = (UIReddot*)sender;
            
            bool isActivate = false;
            
            if(AdventureRelicManager::getInstance()->isRelicCollectReddotCondition())
                isActivate = true;
            
            uiReddot->setVisible(isActivate);
        });
        auto action2 = DelayTime::create(2);
        auto seq = Sequence::create(action1, action2, NULL);
        uiReddot->runAction(RepeatForever::create(seq));
    }
    
    double dEffect = AdventureRelicManager::getInstance()->getRelicCollectEffect(E_ADVENTURE_PROPERTY_TYPE::DUNGEON_ATK);
    std::string strEffect = GAME_TEXT("t_ui_advt_collection_bonus_1");
    strEffect += MafUtils::format(" +%s", AdventureManager::getPropertyEffectText(E_ADVENTURE_PROPERTY_TYPE::DUNGEON_ATK, dEffect).c_str());
    
    auto lbEffect = Label::createWithTTF(strEffect, GAME_FONT, 26);
    lbEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbEffect->setPosition(sprIcon->getPosition() + Vec2::RIGHT * (sprIcon->getContentSize().width * sprIcon->getScale() + 10));
    lbEffect->setDimensions(sprBox->getContentSize().width - (sprIcon->getContentSize().width * sprIcon->getScale() + btnCollect->getContentSize().width + 30), sprBox->getContentSize().height - 10);
    lbEffect->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    lbEffect->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbEffect->setColor(Color3B(128,128,128));
    if(dEffect > 0)
        lbEffect->setColor(COLOR_TITLE);
    sprBox->addChild(lbEffect);
}

#pragma mark -utils
void UILayerAdventureRelicList::onClickLevelup(Ref* sender)
{
    auto node = (Node*)sender;
    
    auto objRelic = AdventureRelicManager::getInstance()->getInfoRelic(node->getTag());
    if(objRelic == nullptr)
        return;
    
    if(!objRelic->isActive() || objRelic->getCurrentLv() >= AdventureRelicManager::getInstance()->getRelicMaxLv())
        return;
    
    if(AdventureRelicManager::getInstance()->getRelicLevelUpPrice(objRelic->getIdx(), 1) > objRelic->getCount())
    {
        Vec2 posToast = _lyTopUI->getParent()->convertToWorldSpace(_lyTopUI->getPosition());
        posToast.y -= _lyTopUI->getContentSize().height - 100;
        PopupToast::showImmediately(GAME_TEXT("t_ui_advt_relic_toast_1"), posToast.x, posToast.y);
        return;
    }
    
    _nUpgradeRelicIdx = objRelic->getIdx();
    
    PopupLoading::show();
    AdventureRelicManager::getInstance()->requestRelicLevelUp(CC_CALLBACK_2(UILayerAdventureRelicList::callbackLevelUP, this), objRelic->getIdx(), 1);
    
}

void UILayerAdventureRelicList::onClickGrind(Ref* sender)
{
    auto node = (Node*)sender;
    
    auto objRelic = AdventureRelicManager::getInstance()->getInfoRelic(node->getTag());
    if(objRelic == nullptr)
        return;
    
    if(objRelic->getCount() <= 0)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_advt_relic_grind_msg_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        return;
    }
    
    auto popup = PopupAdventureRelicGrind::create(objRelic->getIdx());
    popup->setCallbackRefresh(CC_CALLBACK_0(UILayerAdventureRelicList::refreshUI, this));
    popup->show();
}

void UILayerAdventureRelicList::onClickCollection(Ref* sender)
{
    auto popup = PopupAdventureRelicCollect::create();
    popup->setCallbackRefresh(CC_CALLBACK_0(UILayerAdventureRelicList::refreshUI, this));
    popup->show();
}

void UILayerAdventureRelicList::onClickTab(Ref* sender)
{
    auto node = (Node*)sender;
    _eType = (UILayerAdventureRelicList::E_TYPE)node->getTag();
    
    _listRelic = AdventureRelicManager::getInstance()->getListInfoRelicFromArea((int)_eType);
    std::sort(_listRelic.begin(), _listRelic.end(), UILayerAdventureRelicList::sortRelic);
    
    
    uiTop();
    _table->reloadData();
}
void UILayerAdventureRelicList::onClickRelicTotalSpec(Ref* sender)
{
    if(AdventureManager::getInstance()->getTutorialStep() <= 7)
        return;
    
    //test
    auto popup = PopupAdventureRelicTotalSpec::create();
    popup->show();
    return;
}

void UILayerAdventureRelicList::onClickAllLevelUp(Ref* sender)
{
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_advt_relic_levelup_all_notice_1"));
    popup->addButton(GAME_TEXT("t_ui_close"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref*){
        
        if ( _nCurrentLevelUpIdx >= _listRelic.size() )
        {
            return;
        }
        
        auto objRelic = _listRelic.at(_nCurrentLevelUpIdx);
        _nCurrentLevelUpIdx = 0;
        PopupLoadingSequentialUpgrade::show();
        PopupLoadingSequentialUpgrade::setTextKey(MafUtils::format("t_ui_advt_relic_name_%d", objRelic->getIdx()));
        PopupLoadingSequentialUpgrade::setClosingRemark(GAME_TEXT("t_ui_advt_relic_levelup_doing"));
        PopupLoadingSequentialUpgrade::setCurrentIdx(_nCurrentLevelUpIdx + 1);
        requestAllLevelUp();
    });
    popup->show();
    
}

void UILayerAdventureRelicList::requestAllLevelUp()
{
    for(; _nCurrentLevelUpIdx < _listRelic.size() -1; ++_nCurrentLevelUpIdx)
    {
        auto obj = _listRelic.at(_nCurrentLevelUpIdx);
        
        if(obj == nullptr || !obj->isActive())
            continue;
        
        int nMakeCount = AdventureRelicManager::getInstance()->getRelicLevelupMax(obj->getIdx());
        
        if(nMakeCount > 0)
        {
            PopupLoadingSequentialUpgrade::setTextKey(MafUtils::format("t_ui_advt_relic_name_%d", obj->getIdx()));
            PopupLoadingSequentialUpgrade::setClosingRemark(GAME_TEXT("t_ui_advt_relic_levelup_doing"));
            PopupLoadingSequentialUpgrade::setCurrentIdx(_nCurrentLevelUpIdx + 1);
            AdventureRelicManager::getInstance()->requestRelicLevelUp(CC_CALLBACK_2(UILayerAdventureRelicList::callbackAllLevelUp, this), obj->getIdx(), nMakeCount);
            return;
        }
    }
    
    if(_nCurrentLevelUpIdx >= _listRelic.size() - 1)
    {
        PopupLoadingSequentialUpgrade::hide();
        refreshUI();
    }
}
#pragma mark -Callback
void UILayerAdventureRelicList::callbackInfo(bool bResult, int nResult)
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
    
    refreshUI();
    
    showTutorial();
}

void UILayerAdventureRelicList::callbackLevelUP(bool bResult, int nResult)
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
    
    auto objRelic = AdventureRelicManager::getInstance()->getInfoRelic(_nUpgradeRelicIdx);
    if(objRelic == nullptr)
        return;
    
    auto popup = PopupAdventureUpgradeComplete::create(objRelic);
    popup->setCallbackHide(CC_CALLBACK_0(UILayerAdventureRelicList::refreshUI, this));
    if(AdventureManager::getInstance()->getTutorialStep() < 6)
        popup->setIsTutorial(true);
    popup->show();
    
    _nUpgradeRelicIdx = 0;
}

void UILayerAdventureRelicList::callbackAllLevelUp(bool bResult, int nResult)
{
    if(bResult == false)
    {
        PopupLoading::hide();
        
        std::string strMessageKey = "t_ui_error_30";
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        refreshUI();
        
        return;
    }
    auto func = CallFunc::create([=](){
        requestAllLevelUp();
    });
    
    runAction(Sequence::create(DelayTime::create(0.01f), func, NULL));
}
