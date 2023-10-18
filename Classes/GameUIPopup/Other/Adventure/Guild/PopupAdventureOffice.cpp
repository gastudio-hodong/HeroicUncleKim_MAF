//
//  PopupOfficeUpgradeMain.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/19.
//

#include "PopupAdventureOffice.h"

#include "GameObject/Adventure/InfoAdventureUpgradeAbility.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Common/PopupButtonForceTutorial.h"
#include "GameUIPopup/Other/Adventure/PopupAdventureTutorial.h"
#include "GameUIPopup/Other/Adventure/Guild/PopupAdventureFurnitureUpgrade.h"

#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/MineralManager.h"
#include "ManagerGame/ItemsMoneyManager.h"

USING_NS_CC;

PopupAdventureOffice* PopupAdventureOffice::create()
{
    PopupAdventureOffice* pRet = new(std::nothrow) PopupAdventureOffice();
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

PopupAdventureOffice::PopupAdventureOffice(void):
_lyTopUI(nullptr),
_lyFurniture(nullptr),
_btnTutorial(nullptr),
_table(nullptr)
{
    _listFurniture.clear();
}

PopupAdventureOffice::~PopupAdventureOffice(void)
{
    _listFurniture.clear();
}


#pragma mark - table
void PopupAdventureOffice::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupAdventureOffice::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 160);
}

extension::TableViewCell* PopupAdventureOffice::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if ( cell == nullptr )
    {
        cell = new extension::TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();
    
    auto objFurniture = _listFurniture.at(idx);
    
    if(objFurniture == nullptr)
        return cell;
    
    auto objProperty = objFurniture->getInfoUpgradeAbility();
    auto objPropertyNext = objFurniture->getInfoUpgradeAbility(objFurniture->getCurrentLv() + 1);
    
    // size
    Size size = tableCellSizeForIndex(table, idx);
    cell->setContentSize(size);
    
    std::string strTitle = GAME_TEXT(MafUtils::format("t_ui_advt_office_%d", objFurniture->getIdx()));
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/list_bg_1.png");
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setPosition(cell->getContentSize()/2);
    sprBox->setContentSize(Size(712, 150));
    cell->addChild(sprBox);
    
    auto sprIconBox = Sprite::create("Assets/ui_common/box_quest.png");
    sprIconBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprIconBox->setPosition(10, sprBox->getContentSize().height/2);
    sprBox->addChild(sprIconBox);
    
    auto sprIcon = Sprite::create(MafUtils::format("Assets/ui/adventure/adventure_furniture_%02d_%d.png", objFurniture->getIdx(), objFurniture->getImageGrade()));
    sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprIcon->setPosition(sprIconBox->getContentSize()/2);
    sprIcon->getTexture()->setTexParameters(texParams);
    sprIcon->setScale(2.f);
    sprIconBox->addChild(sprIcon);
    
    auto lbTitle = Label::createWithTTF(strTitle + MafUtils::format("Lv.%d", objFurniture->getCurrentLv()), GAME_FONT, 26);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbTitle->setPosition(sprIconBox->getPosition().x + sprIconBox->getContentSize().width + 20, sprBox->getContentSize().height - 25);
    lbTitle->setDimensions(sprBox->getContentSize().width - 320, lbTitle->getContentSize().height);
    lbTitle->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    lbTitle->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbTitle->setColor(COLOR_TITLE);
    sprBox->addChild(lbTitle);
    
    if(objFurniture->getCurrentLv() < objFurniture->getMaxLv())
    {
        auto lbUpgradeConditionLv = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_advt_require_lv_1", objFurniture->getLevelupCondition(objFurniture->getCurrentLv() + 1)) , GAME_FONT, 20);
        lbUpgradeConditionLv->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbUpgradeConditionLv->setPosition(lbTitle->getPosition() + Vec2::DOWN * 50);
        lbUpgradeConditionLv->setDimensions(sprBox->getContentSize().width - 320, lbUpgradeConditionLv->getContentSize().height);
        lbUpgradeConditionLv->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        lbUpgradeConditionLv->setOverflow(cocos2d::Label::Overflow::SHRINK);
        sprBox->addChild(lbUpgradeConditionLv);
    }
    
    double fCurrentEffect = 0;
    
    if(objProperty != nullptr)
        fCurrentEffect = objProperty->getEffect();
    
    std::string strEffect = GAME_TEXT(MafUtils::format("t_ui_advt_property_type_%d", (int)objFurniture->getType()));
    
    strEffect += MafUtils::format(" %s", AdventureManager::getPropertyEffectText(objFurniture->getType(), fCurrentEffect).c_str());
    
    
    auto lbEffect = Label::createWithTTF(strEffect, GAME_FONT, 20);
    lbEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbEffect->setPosition(lbTitle->getPosition() + Vec2::DOWN * 90);
    lbEffect->setDimensions(sprBox->getContentSize().width - 320, lbEffect->getContentSize().height);
    lbEffect->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    lbEffect->setOverflow(cocos2d::Label::Overflow::SHRINK);
    sprBox->addChild(lbEffect);
    
    if(objPropertyNext != nullptr)
    {
        std::string strPath = "Assets/ui_common/btn_upgrade_bg.png";
        
        if(!isUpgradeCondition(objFurniture->getIdx()))
        {
            strPath = "Assets/ui_common/btn_upgrade_max.png";
        }
        
        auto btnUpgrade = ui::Button::create(strPath);
        btnUpgrade->setScale9Enabled(true);
        btnUpgrade->setContentSize(Size(150,116));
        btnUpgrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        btnUpgrade->setPosition(Vec2(sprBox->getContentSize().width - 15, sprBox->getContentSize().height/2));
        btnUpgrade->setZoomScale(0.05f);
        btnUpgrade->setTag(objFurniture->getIdx());
        btnUpgrade->addClickEventListener(CC_CALLBACK_1(PopupAdventureOffice::onClickUpgrade, this));
        btnUpgrade->setTitleText("LEVEL UP");
        btnUpgrade->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        btnUpgrade->getTitleLabel()->setDimensions(btnUpgrade->getContentSize().width, btnUpgrade->getContentSize().height);
        btnUpgrade->setTitleFontSize(25);
        sprBox->addChild(btnUpgrade);
        
        if(AdventureManager::getInstance()->getTutorialStep() == 5 && objFurniture->getIdx() == 1)
            _btnTutorial = btnUpgrade;
    }
    
    return cell;
}

ssize_t PopupAdventureOffice::numberOfCellsInTableView(extension::TableView *table)
{
    return _listFurniture.size();
}

#pragma mark -init
bool PopupAdventureOffice::init()
{
    if ( !PopupBase::init())
    {
        return false;
    }
    
    _listFurniture = AdventureManager::getInstance()->getListInfoFurniture();
    initVar();
    initUI();
    
    showTutorial();
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE, this);
    return true;
}

void PopupAdventureOffice::onEnter()
{
    PopupBase::onEnter();
    
}
#pragma mark - Override
void PopupAdventureOffice::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if(eType == E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE)
        this->setLocalZOrder(ZORDER_SCENE_POPUP);
}
void PopupAdventureOffice::show()
{
    PopupBase::show();
    
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        this->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
}
#pragma mark - Tutorial
void PopupAdventureOffice::showButtonForceTutorial()
{
    if(_btnTutorial == nullptr)
        return;
    
    auto popup = PopupButtonForceTutorial::create();
    popup->setButton(_btnTutorial);
    if(AdventureManager::getInstance()->getTutorialStep() == 5)
             popup->setCallbackListener(CC_CALLBACK_1(PopupAdventureOffice::onClickUpgrade, this));
    popup->show();
    
    _btnTutorial = nullptr;
}

void PopupAdventureOffice::showTutorial()
{
    if(AdventureManager::getInstance()->getTutorialStep() != 5)
        return;
    
    auto popup = PopupAdventureTutorial::create(AdventureManager::getInstance()->getTutorialStep());
    popup->show();
}
void PopupAdventureOffice::initVar()
{
    _spriteContainer->setContentSize(Size(750, 1030));
}

void PopupAdventureOffice::initUI()
{
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_advt_office_title_2"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAdventureOffice::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //650
    Size size = _spriteContainer->getContentSize();
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize( Size(size.width - 10, 170));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTopUI->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(_lyTopUI);
    
    _lyFurniture = LayerColor::create();
    _lyFurniture->setContentSize( Size(size.width - 10, _spriteContainer->getContentSize().height - 75 - _lyTopUI->getContentSize().height - 40));
    _lyFurniture->setIgnoreAnchorPointForPosition(false);
    _lyFurniture->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyFurniture->setPosition( _lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    _spriteContainer->addChild(_lyFurniture);
    
    uiTop();
    uiTable();
}

#pragma mark -ui
void PopupAdventureOffice::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    int nLv = AdventureManager::getInstance()->getLevel();
    auto infoLv = AdventureManager::getInstance()->getInfoBongLevel(nLv);
    int nTotalEXP = 0;
    int nPercent = 0;
    bool isMaxLv = false;
    
    //
    auto sprTitleBox = ui::Scale9Sprite::create("Assets/ui_common/base_text_1_2.png");
    sprTitleBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprTitleBox->setContentSize(Size(sprTitleBox->getContentSize().width * 1.2f, sprTitleBox->getContentSize().height * 1.6f));
    sprTitleBox->setPosition(_lyTopUI->getContentSize().width/2, _lyTopUI->getContentSize().height - 15);
    _lyTopUI->addChild(sprTitleBox);
    {
        auto lbText = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_advt_main_1", AccountManager::getInstance()->getUserNick().c_str(), nLv), GAME_FONT, 26);
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
        nPercent = 0;
        isMaxLv = true;
    }
    
    auto sprExpBg = ui::Scale9Sprite::create("Assets/ui/adventure/adventure_progressbar4_1.png");
    sprExpBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprExpBg->setPosition(sprTitleBox->getPosition() + Vec2::DOWN * (sprTitleBox->getContentSize().height + 15));
    _lyTopUI->addChild(sprExpBg);
    
    auto sprExpIcon = Sprite::create("Assets/icon/icon_exp_2.png");
    sprExpIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    sprExpIcon->setPosition(sprExpBg->getPosition() + Vec2::LEFT * (sprExpBg->getContentSize().width/2 + 10) + Vec2::DOWN * 10);
    _lyTopUI->addChild(sprExpIcon);
    
    std::string strProgressPath = "Assets/ui/adventure/adventure_progressbar4_2.png";
    auto sprExpFilling = ui::Scale9Sprite::create(strProgressPath);
    
    auto progreesExp = ProgressTimer::create(sprExpFilling);
    progreesExp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    progreesExp->setPosition( sprExpBg->getPosition() );
    progreesExp->setMidpoint(Vec2(0, 0.5));
    progreesExp->setBarChangeRate(Vec2(1,0));
    progreesExp->setType(ProgressTimer::Type::BAR);
    progreesExp->setPercentage(nPercent);
    _lyTopUI->addChild(progreesExp);
    
    std::string strTimer = MafUtils::format("%d/%d", AdventureManager::getInstance()->getExp(), nTotalEXP);
    
    if(isMaxLv == true)
        strTimer = "MAX";
    
    auto lbExp = Label::createWithTTF(strTimer, GAME_FONT, 24);
    lbExp->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbExp->setPosition(progreesExp->getContentSize()/2);
    lbExp->setDimensions(progreesExp->getContentSize().width - 100, lbExp->getContentSize().height);
    lbExp->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lbExp->setOverflow(Label::Overflow::SHRINK);
    lbExp->enableOutline(Color4B::BLACK, 2);
    progreesExp->addChild(lbExp);
    
    auto lbExpInfo = Label::createWithTTF(GAME_TEXT("t_ui_advt_office_info_1"), GAME_FONT, 24);
    lbExpInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    lbExpInfo->setPosition(_lyTopUI->getContentSize().width/2, 10);
    lbExpInfo->setDimensions(_lyTopUI->getContentSize().width - 30, lbExpInfo->getContentSize().height - 5);
    lbExpInfo->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
    lbExpInfo->setOverflow(Label::Overflow::SHRINK);
    _lyTopUI->addChild(lbExpInfo);
}
void PopupAdventureOffice::uiTable()
{
    _lyFurniture->removeAllChildren();
    
    _table = extension::TableView::create(this, Size(_lyFurniture->getContentSize().width, _lyFurniture->getContentSize().height - 10));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition(_lyFurniture->getContentSize().width / 2, 0);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lyFurniture->addChild(_table);
    
    _table->reloadData();
}
#pragma mark -click
void PopupAdventureOffice::onClickClose(Ref* const pSender)
{
    if(AdventureManager::getInstance()->getTutorialStep() == 6)
    {
        RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE);
    }
    hide();
}

void PopupAdventureOffice::onClickUpgrade(Ref* const pSender)
{
    auto node = (Node*)pSender;
    
    auto popup = PopupAdventureFurnitureUpgrade::create(node->getTag());
    popup->setCallbackUpgrade(CC_CALLBACK_0(PopupAdventureOffice::callbackUpgrade, this));
    popup->showButtonForceTutorial();
    popup->show();
}

#pragma mark -utils
bool PopupAdventureOffice::isUpgradeCondition(int nIdx)
{
    bool result = true;
    
    auto objInfo = AdventureManager::getInstance()->getInfoFurniture(nIdx);
    auto objProperty = objInfo->getInfoUpgradeAbility(objInfo->getCurrentLv() + 1);
    
    int nCondition = objInfo->getLevelupCondition(objInfo->getCurrentLv() + 1);
    
    if(AdventureManager::getInstance()->getLevel() < nCondition)
    {
        return false;
    }
    
    for ( int i = 0; i < C_COUNT_MINE_MINERAL; i++ )
    {
        E_MINERAL mineral = (E_MINERAL)(i+1);
        int needCount = 0;
        
        if(objProperty == nullptr)
            continue;
        auto objItems = objProperty->getInfoItems((int)E_ITEMS::MINE_STONE_IRON + i);
        
        if(objItems != nullptr)
            needCount = std::atoi(objItems->getCount().c_str());
        
        if ( MineralManager::getInstance()->getCount(mineral) < needCount )
        {
            return false;
        }
    }
    
    auto objItemCost = objProperty->getInfoItems((int)E_ITEMS::GEM);
    if(std::atoi(objItemCost->getCount().c_str()) > ItemsMoneyManager::getInstance()->getGem())
        return false;
    
    objItemCost = objProperty->getInfoItems((int)E_ITEMS::POINT);
    if(std::atoi(objItemCost->getCount().c_str()) > ItemsMoneyManager::getInstance()->getPoint())
        return false;
    
    return result;
}
#pragma mark -callback
void PopupAdventureOffice::callbackUpgrade()
{
    auto offset = _table->getContentOffset();
    _table->reloadData();
    _table->setContentOffset(offset);
}
