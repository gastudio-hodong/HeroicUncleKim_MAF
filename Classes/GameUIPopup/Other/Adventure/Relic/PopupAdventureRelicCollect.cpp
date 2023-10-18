//
//  PopupAdventureRelicCollect.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/12.
//

#include "PopupAdventureRelicCollect.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoItems.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/Adventure/Guild/PopupAdventureMemberUpgrade.h"

#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/AdventureRelicManager.h"

bool PopupAdventureRelicCollect::sortCondition(InfoAdventureRelicCollect* const a, InfoAdventureRelicCollect* const b)
{
    if(a == nullptr)
        return true;
    else if(b == nullptr)
        return false;
    
    bool result = false;
    
    bool isRewardA = !a->isActive() && a->isReward();
    
    bool isRewardB = !b->isActive() && b->isReward();
    
    if(isRewardA && !isRewardB)
        result = true;
    else if(!isRewardA && isRewardB)
        result = false;
    else
        result = a->getIdx() < b->getIdx();
    
    return result;
}

PopupAdventureRelicCollect* PopupAdventureRelicCollect::create()
{
    PopupAdventureRelicCollect* pRet = new(std::nothrow) PopupAdventureRelicCollect();
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

PopupAdventureRelicCollect::PopupAdventureRelicCollect(void):
_lyTopUI(nullptr),
_lyTable(nullptr),
_callbackHide(nullptr),
_callbackRefresh(nullptr),
_table(nullptr)
{
    _listInfoCollect.clear();
}

PopupAdventureRelicCollect::~PopupAdventureRelicCollect(void)
{
    _listInfoCollect.clear();
}


#pragma mark - table
void PopupAdventureRelicCollect::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupAdventureRelicCollect::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 145);
}

extension::TableViewCell* PopupAdventureRelicCollect::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    
    auto objCollect = _listInfoCollect.at(idx);
    if(objCollect == nullptr)
        return cell;
    
    enum class E_COLLECT_STATE
    {
        NONE,
        REWARD,
        COLLECT
    };
    
    E_COLLECT_STATE eType = E_COLLECT_STATE::NONE;
    if(objCollect->isActive())
    {
        eType = E_COLLECT_STATE::COLLECT;
    }
    if(!objCollect->isActive() && objCollect->isReward())
    {
        eType = E_COLLECT_STATE::REWARD;
    }
    
    std::string strBgPath = "Assets/ui_common/list_bg_s_2_1.png";
    std::string strBtnPath = "Assets/ui_common/btn_upgrade_bg_2_max.png";
    if(eType == E_COLLECT_STATE::COLLECT)
    {
        strBgPath = "Assets/ui_common/list_bg_s_2_2.png";
    }
    
    auto sprBox = ui::Scale9Sprite::create(strBgPath);
    sprBox->setContentSize(Size(718,130));
    sprBox->setPosition(cell->getContentSize()/2);
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->getTexture()->setTexParameters(texParams);
    cell->addChild(sprBox);
    
    switch (eType)
    {
        case E_COLLECT_STATE::REWARD:
        {
            auto menu = MafNode::MafMenu::create();
            menu->setPosition(Vec2::ZERO);
            sprBox->addChild(menu);
            
            auto itemCollect = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/revive_btn_no.png"), nullptr,  CC_CALLBACK_1(PopupAdventureRelicCollect::onClickCollect, this));
            itemCollect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            itemCollect->setPosition(Vec2(sprBox->getContentSize().width - 8, sprBox->getContentSize().height/2));
            itemCollect->setTag(objCollect->getIdx());
            menu->addChild(itemCollect);
            {
                auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_complete"), GAME_FONT, 25);
                lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                lbText->setPosition(itemCollect->getContentSize()/2);
                lbText->setDimensions(itemCollect->getContentSize().width - 10, itemCollect->getContentSize().height - 10);
                lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
                lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
                itemCollect->addChild(lbText);
            }
            
            auto sprEffect = ui::Scale9Sprite::create("Assets/ui_common/list_bg_s_2_0.png");
            sprEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprEffect->setContentSize(sprBox->getContentSize());
            sprEffect->setPosition(sprBox->getContentSize()/2);
            sprBox->addChild(sprEffect);
            
            auto fadeOut = FadeOut::create(1.f);
            auto fadeIn = FadeIn::create(1.f);
            auto seq = Sequence::create(fadeOut,fadeIn, NULL);
            sprEffect->runAction(RepeatForever::create(seq));
        }break;
        case E_COLLECT_STATE::COLLECT:
        {
            auto sprCheck = Sprite::create("Assets/ui_common/btn_check_01.png");
            sprCheck->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprCheck->setPosition(sprBox->getContentSize().width - 74, sprBox->getContentSize().height/2);
            sprBox->addChild(sprCheck);
        }break;
    }
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_advt_relic_collection_name_%d", objCollect->getIdx())), GAME_FONT, 20);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbTitle->setPosition(15, sprBox->getContentSize().height - sprBox->getContentSize().height/4);
    lbTitle->setDimensions(sprBox->getContentSize().width/2, lbTitle->getContentSize().height);
    lbTitle->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    lbTitle->setOverflow(cocos2d::Label::Overflow::SHRINK);
    sprBox->addChild(lbTitle);
    
    auto lyCollect = LayerColor::create();
    lyCollect->setContentSize( Size(280, 70));
    lyCollect->setIgnoreAnchorPointForPosition(false);
    lyCollect->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    lyCollect->setPosition(15, 5);
    sprBox->addChild(lyCollect);
    {
        auto layerScroll = ui::ScrollView::create();
        layerScroll->setBounceEnabled(false);
        layerScroll->setDirection(ui::ScrollView::Direction::HORIZONTAL);
        layerScroll->setContentSize(lyCollect->getContentSize());
        layerScroll->setIgnoreAnchorPointForPosition(false);
        layerScroll->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        layerScroll->setPosition(Vec2(0, lyCollect->getContentSize().height / 2));
        layerScroll->setSwallowTouches(false);
        lyCollect->addChild(layerScroll);
        
        double innerWidth = 0;
        double innerHeight = layerScroll->getContentSize().height;
        
        auto listCondition = objCollect->getListCondition();
        for ( int i = 0; i < listCondition.size(); i++ )
        {
            auto objCondition = listCondition[i];
            auto objRelic = AdventureRelicManager::getInstance()->getInfoRelic(objCondition.first);
            if(objRelic == nullptr)
                continue;
            
            double posX = 58 * i + 10 * i;
            double posY = layerScroll->getContentSize().height / 2;
            innerWidth = posX + 58;
            
            auto spriteItemBG = ui::Scale9Sprite::create(MafUtils::format("Assets/ui/adventure/adventure_box_%02d.png", objRelic->getGrade()));
            spriteItemBG->setContentSize(Size(58,58));
            spriteItemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            spriteItemBG->setPosition(posX, posY);
            layerScroll->addChild(spriteItemBG);
            
            auto sprRelic = Sprite::create(MafUtils::format("Assets/ui/adventure/adventure_relic_%d.png", objRelic->getIdx()));
            if ( sprRelic != nullptr )
            {
                sprRelic->setPosition(spriteItemBG->getContentSize().width / 2, spriteItemBG->getContentSize().height / 2 + 10);
                sprRelic->getTexture()->setTexParameters(texParams);

                if(objRelic->getCurrentLv() < objCondition.second)
                {
                    sprRelic->setColor(Color3B(90,90,90));
                    spriteItemBG->setColor(Color3B(90,90,90));
                }
                sprRelic->setScale((spriteItemBG->getContentSize().width - 20) / sprRelic->getContentSize().width,
                                   (spriteItemBG->getContentSize().height - 20) / sprRelic->getContentSize().height);
                spriteItemBG->addChild(sprRelic);
            }
            if(objRelic->getCurrentLv() >= objCondition.second)
            {
                auto sprEffect = ui::Scale9Sprite::create("Assets/ui_common/box_item_1_1.png");
                sprEffect->setContentSize(Size(58,58));
                sprEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                sprEffect->setPosition(spriteItemBG->getContentSize()/2);
                spriteItemBG->addChild(sprEffect);
            }

            auto labelLv = Label::createWithTTF(MafUtils::format("Lv.%02d", objCondition.second), GAME_FONT, 16);
            labelLv->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            labelLv->setPosition(spriteItemBG->getContentSize().width / 2, 0);
            spriteItemBG->addChild(labelLv);
        }
        
        layerScroll->setInnerContainerSize(Size(innerWidth, innerHeight));
    }
    
    auto lyReward = LayerColor::create();
    lyReward->setContentSize( Size(200, 70));
    lyReward->setIgnoreAnchorPointForPosition(false);
    lyReward->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    lyReward->setPosition(sprBox->getContentSize().width - sprBox->getContentSize().width/3, 5);
    sprBox->addChild(lyReward);
    {
        auto layerScroll = ui::ScrollView::create();
        layerScroll->setBounceEnabled(false);
        layerScroll->setDirection(ui::ScrollView::Direction::HORIZONTAL);
        layerScroll->setContentSize(lyReward->getContentSize());
        layerScroll->setIgnoreAnchorPointForPosition(false);
        layerScroll->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        layerScroll->setPosition(Vec2(lyReward->getContentSize() / 2));
        layerScroll->setSwallowTouches(false);
        lyReward->addChild(layerScroll);
        
        double innerWidth = 0;
        double innerHeight = layerScroll->getContentSize().height;
        
        auto listItems = objCollect->getListReward();
        for ( int i = 0; i < listItems.size(); i++ )
        {
            auto objItem = listItems.at(i);
            
            double posX = 58 * i + 10 * i;
            double posY = layerScroll->getContentSize().height / 2;
            innerWidth = posX + 58;
            
            auto spriteItemBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
            spriteItemBG->setContentSize(Size(58,58));
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
                }
                spriteItemBG->addChild(spriteItems);
                if(eType == E_COLLECT_STATE::COLLECT)
                {
                    spriteItems->setColor(Color3B(90,90,90));
                }
            }
            
            std::string strItemsCount = objItem->getCount();
            if ( objItem != nullptr && objItem->isString() == true )
            {
                MafUtils::convertNumberToShort(strItemsCount);
            }

            auto labelCount = Label::createWithTTF(strItemsCount, GAME_FONT, 16);
            labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            labelCount->setPosition(spriteItemBG->getContentSize().width / 2, 0);
            spriteItemBG->addChild(labelCount);
            if(eType == E_COLLECT_STATE::COLLECT)
            {
                labelCount->setColor(Color3B(90,90,90));
            }
        }
        
        if(innerWidth < layerScroll->getContentSize().width)
            layerScroll->setContentSize(Size(innerWidth, innerHeight));
        
        layerScroll->setInnerContainerSize(Size(innerWidth, innerHeight));
        
        if(eType == E_COLLECT_STATE::COLLECT)
        {
            lyReward->setColor(Color3B(90,90,90));
        }
    }
    
    std::string strEffect = GAME_TEXT(MafUtils::format("t_ui_advt_property_type_%d", (int)objCollect->getType()));
    
    strEffect += MafUtils::format(" +%s", AdventureManager::getPropertyEffectText(objCollect->getType(), objCollect->getAbilityEffect()).c_str());
    
    
    auto lbEffect = Label::createWithTTF(strEffect, GAME_FONT, 20);
    lbEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbEffect->setPosition(sprBox->getContentSize().width - sprBox->getContentSize().width/3, sprBox->getContentSize().height - sprBox->getContentSize().height/4);
    lbEffect->setDimensions(lyReward->getContentSize().width, sprBox->getContentSize().height/2 - 10);
    lbEffect->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    lbEffect->setOverflow(cocos2d::Label::Overflow::SHRINK);
    sprBox->addChild(lbEffect);
    if(eType == E_COLLECT_STATE::COLLECT)
        lbEffect->setColor(COLOR_TITLE);
    else if(eType == E_COLLECT_STATE::NONE)
        lbEffect->setColor(Color3B(128,128,128));
        
    
    return cell;
}

ssize_t PopupAdventureRelicCollect::numberOfCellsInTableView(extension::TableView *table)
{
    return _listInfoCollect.size();
}

void PopupAdventureRelicCollect::setCallbackHide(const std::function<void(void)>& callback)
{
    _callbackHide = callback;
}
void PopupAdventureRelicCollect::setCallbackRefresh(const std::function<void(void)>& callback)
{
    _callbackRefresh = callback;
}
#pragma mark -init
bool PopupAdventureRelicCollect::init()
{
    if ( !PopupBase::init())
    {
        return false;
    }
    
    _listInfoCollect = AdventureRelicManager::getInstance()->getListCollect();
    std::sort(_listInfoCollect.begin(), _listInfoCollect.end(), PopupAdventureRelicCollect::sortCondition);
    initVar();
    initUI();
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE, this);
    
    return true;
}

void PopupAdventureRelicCollect::onEnter()
{
    PopupBase::onEnter();
    
    PopupLoading::show();
    AdventureRelicManager::getInstance()->requestInfoRelicCollect(CC_CALLBACK_2(PopupAdventureRelicCollect::callbackInit, this));
}
#pragma mark - Override
void PopupAdventureRelicCollect::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if(eType == E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE)
        this->setLocalZOrder(ZORDER_SCENE_POPUP);
}
void PopupAdventureRelicCollect::show()
{
    PopupBase::show();
    
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        this->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
}
void PopupAdventureRelicCollect::initVar()
{
    _spriteContainer->setContentSize(Size(750, 1104));
}

void PopupAdventureRelicCollect::initUI()
{
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_advt_info_title_9"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAdventureRelicCollect::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //650
    Size size = _spriteContainer->getContentSize();
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize( Size(size.width - 10, 250));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTopUI->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(_lyTopUI);
    
    _lyTable = LayerColor::create();
    _lyTable->setContentSize( Size(size.width - 10, _spriteContainer->getContentSize().height - 75 - _lyTopUI->getContentSize().height - 10));
    _lyTable->setIgnoreAnchorPointForPosition(false);
    _lyTable->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTable->setPosition( _lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    _spriteContainer->addChild(_lyTable);
    
    uiTop();
    uiTable();
}

#pragma mark -ui
void PopupAdventureRelicCollect::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    double dEffect = AdventureRelicManager::getInstance()->getRelicCollectEffect(E_ADVENTURE_PROPERTY_TYPE::DUNGEON_ATK);
    
    std::string strEffect = GAME_TEXT("t_ui_advt_property_type_17");
    strEffect += MafUtils::format(" +%s", AdventureManager::getPropertyEffectText(E_ADVENTURE_PROPERTY_TYPE::DUNGEON_ATK, dEffect).c_str());
    
    auto sprEffectBox = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
    sprEffectBox->setContentSize(Size(718,90));
    sprEffectBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprEffectBox->setPosition(Vec2(_lyTopUI->getContentSize()/2) + Vec2::UP * 25);
    _lyTopUI->addChild(sprEffectBox);
    
    auto lbEffect = Label::createWithTTF(strEffect, GAME_FONT, 30);
    lbEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbEffect->setPosition(sprEffectBox->getContentSize()/2);
    lbEffect->setDimensions(sprEffectBox->getContentSize().width - 20, sprEffectBox->getContentSize().height - 10);
    lbEffect->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    lbEffect->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbEffect->setColor(Color3B(128,128,128));
    if(dEffect > 0)
        lbEffect->setColor(COLOR_TITLE);
    sprEffectBox->addChild(lbEffect);
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/box.png");
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprBox->setPosition(_lyTopUI->getContentSize().width/2, 0);
    sprBox->setContentSize(Size(718, 50));
    _lyTopUI->addChild(sprBox);
    {
        auto lbCondition = Label::createWithTTF(GAME_TEXT("t_ui_bonsikdex_bottomtext_1"), GAME_FONT, 24);
        lbCondition->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbCondition->setPosition(110, sprBox->getContentSize().height/2);
        lbCondition->setDimensions(280, sprBox->getContentSize().height - 5);
        lbCondition->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbCondition->setOverflow(cocos2d::Label::Overflow::SHRINK);
        sprBox->addChild(lbCondition);
        
        auto lbReward = Label::createWithTTF(GAME_TEXT("t_ui_team_raid_reward_item"), GAME_FONT, 24);
        lbReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbReward->setPosition(480, sprBox->getContentSize().height/2);
        lbReward->setDimensions(150, sprBox->getContentSize().height - 5);
        lbReward->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbReward->setOverflow(cocos2d::Label::Overflow::SHRINK);
        sprBox->addChild(lbReward);
        
        auto lbCollect = Label::createWithTTF(GAME_TEXT("t_ui_complete"), GAME_FONT, 24);
        lbCollect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbCollect->setPosition(645, sprBox->getContentSize().height/2);
        lbCollect->setDimensions(130, sprBox->getContentSize().height - 5);
        lbCollect->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbCollect->setOverflow(cocos2d::Label::Overflow::SHRINK);
        sprBox->addChild(lbCollect);
    }
}
void PopupAdventureRelicCollect::uiTable()
{
    _lyTable->removeAllChildren();
    
    _table = extension::TableView::create(this, Size(_lyTable->getContentSize().width, _lyTable->getContentSize().height - 10));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _table->setPosition(_lyTable->getContentSize().width / 2, _lyTable->getContentSize().height - 5);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lyTable->addChild(_table);
    
    _table->reloadData();
}


#pragma mark -click
void PopupAdventureRelicCollect::onClickClose(Ref* const pSender)
{
    hide();
}

void PopupAdventureRelicCollect::onClickCollect(Ref* const pSender)
{
    auto node = (Node*)pSender;
    
    PopupLoading::show();
    AdventureRelicManager::getInstance()->requestRewardRelicCollect(CC_CALLBACK_2(PopupAdventureRelicCollect::callbackCollect, this), node->getTag());
}
#pragma mark -callback
void PopupAdventureRelicCollect::callbackInit(const bool bResult, const int nResult)
{
    PopupLoading::hide();
    if ( bResult == false )
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if( nResult == 7 )
        {
            strMessageKey = "t_ui_advt_error_2";
        }
        else if( nResult == 8 )
        {
            strMessageKey = "t_ui_advt_error_3";
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        
        hide();
        return;
    }
    _listInfoCollect = AdventureRelicManager::getInstance()->getListCollect();
    uiTop();
    std::sort(_listInfoCollect.begin(), _listInfoCollect.end(), PopupAdventureRelicCollect::sortCondition);
    _table->reloadData();
}


void PopupAdventureRelicCollect::callbackCollect(const bool bResult, const int nResult)
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
        
        hide();
        return;
    }
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    if(AdventureManager::getInstance()->getTutorialStep() > 6)
        RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE);
    else
    {
        if(_callbackRefresh != nullptr)
            _callbackRefresh();
    }
    _listInfoCollect = AdventureRelicManager::getInstance()->getListCollect();
    uiTop();
    std::sort(_listInfoCollect.begin(), _listInfoCollect.end(), PopupAdventureRelicCollect::sortCondition);
    _table->reloadData();
}
