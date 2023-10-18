//
//  UILayerAdventureMission.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/08.
//

#include "UILayerAdventureMission.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoQuest.h"
#include "GameObject/InfoItems.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/Adventure/Guild/PopupAdventureParty.h"

#include "ManagerGame/AdventureManager.h"

USING_NS_CC;

UILayerAdventureMission* UILayerAdventureMission::create(const cocos2d::Size size, const E_ADVENTURE_MISSION_GROUP type)
{
    UILayerAdventureMission* pRet = new(std::nothrow) UILayerAdventureMission();
    if ( pRet && pRet->init(size, type) )
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

UILayerAdventureMission::UILayerAdventureMission(void):
_eType(E_ADVENTURE_MISSION_GROUP::NONE)
,_lyTop(nullptr)
,_lyTable(nullptr)
,_table(nullptr)
{
    _listMission.clear();
}

UILayerAdventureMission::~UILayerAdventureMission(void)
{
    _listMission.clear();
    AdventureManager::getInstance()->unSubMissionEvent(this);
}

bool UILayerAdventureMission::init(const cocos2d::Size size, const E_ADVENTURE_MISSION_GROUP type)
{
    if (!UILayerAdventureParent::init())
    {
        return false;
    }
    setContentSize(size);
    _eType = type;
    _listMission = AdventureManager::getInstance()->getListInfoMissionFromGroup(_eType);
    AdventureManager::getInstance()->subMissionEvent(new MafFunction<void()>(CC_CALLBACK_0(UILayerAdventureMission::refreshUI, this), this));
    
    // init
    initVar();
    initUi();
    return true;
}

#pragma mark - init
void UILayerAdventureMission::initVar()
{
}

void UILayerAdventureMission::initUi()
{
    // UI
    _lyTop = LayerColor::create();
    _lyTop->setContentSize(Size(getContentSize().width, 60));
    _lyTop->setIgnoreAnchorPointForPosition(false);
    _lyTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTop->setPosition(getContentSize().width/2, getContentSize().height);
    addChild(_lyTop);
    
    _lyTable = LayerColor::create();
    _lyTable->setContentSize(Size(getContentSize().width, getContentSize().height - _lyTop->getContentSize().height));
    _lyTable->setIgnoreAnchorPointForPosition(false);
    _lyTable->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTable->setPosition(_lyTop->getPosition() + Vec2::DOWN * (_lyTop->getContentSize().height));
    addChild(_lyTable);
    
    uiTop();
    uiTable();
}

#pragma mark - override
void UILayerAdventureMission::refreshUI()
{
    _table->reloadData();
}

#pragma mark - table
void UILayerAdventureMission::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size UILayerAdventureMission::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 110);
}

extension::TableViewCell* UILayerAdventureMission::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if ( cell == nullptr )
    {
        cell = new extension::TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();
    
    // size
    Size size = tableCellSizeForIndex(table, idx);
    cell->setContentSize(size);
    auto objMission = _listMission.at(idx);
    if(objMission == nullptr)
        return cell;
    
    auto btnMission = ui::Button::create("Assets/ui_common/list_bg_s_2_1.png");
    btnMission->setScale9Enabled(true);
    btnMission->setContentSize(Size(698, 102));
    btnMission->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    btnMission->setPosition(Vec2(cell->getContentSize()/2));
    btnMission->setTag(objMission->getIdx());
    btnMission->addClickEventListener(CC_CALLBACK_1(UILayerAdventureMission::onClickMissionComplete, this));
    btnMission->setZoomScale(0.015f);
    cell->addChild(btnMission);
    
    if(!objMission->isReceive() && objMission->getCountNow() >= objMission->getCountMax())
    {
        auto sprFx = ui::Scale9Sprite::create("Assets/ui_common/list_bg_s_2_0.png");
        sprFx->setContentSize(btnMission->getContentSize());
        sprFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprFx->setPosition(Vec2(btnMission->getContentSize()/2));
        btnMission->addChild(sprFx);
    }
    
    if(objMission->isReceive())
    {
        auto const spriteDisable = ui::Scale9Sprite::create(PIXEL, Rect(0, 0, btnMission->getContentSize().width, btnMission->getContentSize().height));
        spriteDisable->setPosition(btnMission->getContentSize().width / 2, btnMission->getContentSize().height / 2);
        spriteDisable->setColor(Color3B::BLACK);
        spriteDisable->setOpacity(150);
        btnMission->addChild(spriteDisable);
    }
    
    //text
    auto lbTitle = Label::createWithTTF(objMission->getTitle(), GAME_FONT, 24);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbTitle->setPosition(15, btnMission->getContentSize().height - btnMission->getContentSize().height/4);
    lbTitle->setDimensions(btnMission->getContentSize().width / 2, btnMission->getContentSize().height/2);
    lbTitle->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbTitle->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    btnMission->addChild(lbTitle);
    if(objMission->isReceive())
        lbTitle->enableStrikethrough();
    
    
    //progress
    float nPercent = 0;
    
    nPercent = (float)objMission->getCountNow() / (float)objMission->getCountMax() * 100;
    if ( nPercent > 100 )
        nPercent = 100;
    
    auto spriteTimerBg = Sprite::create("Assets/ui/adventure/adventure_mission_gauge_bg.png");
    spriteTimerBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    spriteTimerBg->setPosition(15, btnMission->getContentSize().height/4);
    btnMission->addChild(spriteTimerBg);
        
    auto timerCount = ProgressTimer::create(Sprite::create("Assets/ui/adventure/adventure_mission_gauge_orange.png"));
    timerCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    timerCount->setPosition( spriteTimerBg->getPosition() );
    timerCount->setMidpoint(Vec2(0, 0.5));
    timerCount->setBarChangeRate(Vec2(1,0));
    timerCount->setType(ProgressTimer::Type::BAR);
    timerCount->setPercentage(nPercent);
    btnMission->addChild(timerCount);
    {
        std::string strTimer = MafUtils::format("%d/%d", (int)objMission->getCountNow(), (int)objMission->getCountMax());
        
        auto labelTimer = Label::createWithTTF(strTimer, GAME_FONT, 18);
        labelTimer->setPosition( Vec2(timerCount->getContentSize().width / 2, timerCount->getContentSize().height / 2) );
        timerCount->addChild(labelTimer);
    }
    if(objMission->isReceive())
        timerCount->setColor(Color3B(90,90,90));
    
    //rewards
    
    auto lyReward = LayerColor::create();
    lyReward->setContentSize( Size(250, btnMission->getContentSize().height - 15));
    lyReward->setIgnoreAnchorPointForPosition(false);
    lyReward->setAnchorPoint( Vec2::ANCHOR_MIDDLE_RIGHT );
    lyReward->setPosition(btnMission->getContentSize().width - 5, cell->getContentSize().height/2);
    cell->addChild(lyReward);
    {
        auto layerScroll = ui::ScrollView::create();
        layerScroll->setBounceEnabled(false);
        layerScroll->setDirection(ui::ScrollView::Direction::HORIZONTAL);
        layerScroll->setContentSize(lyReward->getContentSize());
        layerScroll->setIgnoreAnchorPointForPosition(false);
        layerScroll->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        layerScroll->setPosition(Vec2(0, lyReward->getContentSize().height / 2));
        layerScroll->setSwallowTouches(false);
        lyReward->addChild(layerScroll);
        
        double innerWidth = 0;
        double innerHeight = layerScroll->getContentSize().height;
        
        auto listItems = objMission->getListReward();
        for ( int i = 0; i < listItems.size(); i++ )
        {
            auto objItem = listItems.at(i);
            
            double posX = 75 * i + 10 * i;
            double posY = layerScroll->getContentSize().height / 2;
            innerWidth = posX + 75;
            
            auto spriteItemBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
            spriteItemBG->setContentSize(Size(75,75));
            spriteItemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            spriteItemBG->setPosition(posX, posY);
            layerScroll->addChild(spriteItemBG);
            
            auto spriteItems = Sprite::create(objItem->getPath());
            if ( spriteItems != nullptr )
            {
                spriteItems->setPosition(spriteItemBG->getContentSize().width / 2, spriteItemBG->getContentSize().height / 2 + 10);
                spriteItems->getTexture()->setTexParameters(texParams);

                switch((E_ITEMS)objItem->getIdx())
                {
                    case E_ITEMS::BADGE : spriteItems->setScale(1); break;
                    case E_ITEMS::INFINITE_ARTIFACT : spriteItems->setScale(1); break;
                    case E_ITEMS::ADVENTURE_CORE : spriteItems->setScale(1.2f); break;
                    default: spriteItems->setScale(1.5f); break;
                }
                if(objMission->isReceive())
                {
                    spriteItems->setColor(Color3B(90,90,90));
                }
                spriteItemBG->addChild(spriteItems);
            }
            
            std::string strItemsCount = objItem->getCount();
            if ( objItem != nullptr && objItem->isString() == true )
            {
                MafUtils::convertNumberToShort(strItemsCount);
            }

            auto labelCount = Label::createWithTTF(strItemsCount, GAME_FONT, 18);
            labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            labelCount->setPosition(spriteItemBG->getContentSize().width / 2, 0);
            spriteItemBG->addChild(labelCount);
            if(objMission->isReceive())
            {
                labelCount->enableStrikethrough();
            }
        }
        layerScroll->setInnerContainerSize(Size(innerWidth, innerHeight));
        
        if(objMission->isReceive())
        {
            lyReward->setColor(Color3B(90,90,90));
        }
    }
    
    
    return cell;
}

ssize_t UILayerAdventureMission::numberOfCellsInTableView(extension::TableView *table)
{
    return _listMission.size();
}

#pragma mark ui
void UILayerAdventureMission::uiTop()
{
    _lyTop->removeAllChildren();
    
    auto sprTitleBox = Sprite::create("Assets/ui/adventure/adventure_mission_title_box.png");
    sprTitleBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprTitleBox->setPosition(_lyTop->getContentSize()/2);
    sprTitleBox->setScale(2);
    sprTitleBox->getTexture()->setTexParameters(texParams);
    _lyTop->addChild(sprTitleBox);
    
    std::string strKey = "t_ui_advt_mission_daily_1";
    if(_eType == E_ADVENTURE_MISSION_GROUP::WEEKLY)
        strKey = "t_ui_advt_mission_weekly_1";
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT(strKey), GAME_FONT, 24);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbTitle->setPosition(30, _lyTop->getContentSize().height/2);
    lbTitle->setDimensions(_lyTop->getContentSize().width, _lyTop->getContentSize().height);
    lbTitle->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbTitle->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    _lyTop->addChild(lbTitle);
}

void UILayerAdventureMission::uiTable()
{
    _lyTable->removeAllChildren();
    
    _table = extension::TableView::create(this, Size(_lyTable->getContentSize().width, _lyTable->getContentSize().height));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition(_lyTable->getContentSize().width / 2, 0);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lyTable->addChild(_table);
    _table->reloadData();
}

#pragma mark click
void UILayerAdventureMission::onClickMissionComplete(Ref* const sender)
{
    auto node = (Node*) sender;
    
    auto objMission = AdventureManager::getInstance()->getInfoMission((int)_eType, node->getTag());
    if(objMission == nullptr ||
       objMission->isReceive() ||
       (!objMission->isReceive() && objMission->getCountNow() < objMission->getCountMax()))
        return;
    
    PopupLoading::show();
    AdventureManager::getInstance()->requestMissionComplete(CC_CALLBACK_2(UILayerAdventureMission::callbackMissionComplete, this), _eType, node->getTag());
}

#pragma mark callback
void UILayerAdventureMission::callbackMissionComplete(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if(nResult != 1 && nResult != 8)
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
        
        return;
    }
}
