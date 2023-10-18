//
//  PopupAdventureStage.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/26.
//

#include "PopupAdventureStage.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoItems.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/Adventure/Guild/PopupAdventureMemberUpgrade.h"
#include "GameUIPopup/Other/Adventure/Guild/PopupAdventureFurnitureUpgrade.h"

#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupAdventureStage* PopupAdventureStage::create(const int nCurrentAdventureID, const int nCurrentArea, bool isFriend)
{
    PopupAdventureStage* pRet = new(std::nothrow) PopupAdventureStage();
    if ( pRet && pRet->init(nCurrentAdventureID, nCurrentArea, isFriend) )
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

PopupAdventureStage::PopupAdventureStage(void):
_isFriend(false),
_nCurrentAdventureID(0),
_nCurrentArea(0),

_infoMap(nullptr),

_callbackHide(nullptr),
_callbackRefresh(nullptr),

//
_lyTopUI(nullptr),
_lyStage(nullptr),
_table(nullptr)
{
    
}

PopupAdventureStage::~PopupAdventureStage(void)
{
}


#pragma mark - table
void PopupAdventureStage::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupAdventureStage::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 159 + 10);
}

extension::TableViewCell* PopupAdventureStage::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if ( cell == nullptr )
    {
        cell = new extension::TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();
    
    Size size = tableCellSizeForIndex(table, idx);
    Size sizeLayout = Size(696, 159);
    
    // size
    cell->setContentSize(size);

    
    //
    enum class E_PARTY_STATE
    {
        SEAL,
        OPEN,
        RUNNING,
        REWARD,
        COMPLETE
    };
    
    int nStageIdx = (int)(idx + 1);
    if ( _infoMap->getIdx() == 1 && AdventureManager::getInstance()->getTutorialStep() == 1 )
    {
        nStageIdx = (int)idx;
    }
    
    auto objStage = _infoMap->getStage(nStageIdx);
    if ( objStage == nullptr )
    {
        return cell;
    }
    
    E_PARTY_STATE eType = E_PARTY_STATE::SEAL;
    if ( objStage->isOpen() )
    {
        eType = E_PARTY_STATE::OPEN;
    }
    
    if ( objStage->isRunning() )
    {
        eType = E_PARTY_STATE::RUNNING;
        auto time = objStage->getEndTime() - UtilsDate::getInstance()->getTime();
        
        if(time < 0)
        {
            eType = E_PARTY_STATE::REWARD;
        }
    }
    
    if ( objStage->isComplete() )
        eType = E_PARTY_STATE::COMPLETE;
    
    double decreseTimePer = 0;
    if ( _isFriend )
    {
        auto objFriend = AdventureManager::getInstance()->getInfoFriendFromUserIdx(_nCurrentAdventureID);
        decreseTimePer = objFriend->getAbilityEffect(E_ADVENTURE_PROPERTY_TYPE::PARTY_TIME_DECREASE) * 0.01;
    }
    else
        decreseTimePer = AdventureManager::getInstance()->getEffectDecreseTime(_nCurrentAdventureID, true) * 0.01;
    
    //
    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(sizeLayout);
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    cell->addChild(uiLayout);
    
    //
    std::string strBgPath = "Assets/ui/adventure/adventure_place_%d_%d.png";
    std::string strBtnPath = "Assets/ui_common/btn_upgrade_bg_2_max.png";
    if(eType == E_PARTY_STATE::OPEN)
        strBtnPath = "Assets/ui_common/btn_upgrade_bg_2.png";
    else if(eType == E_PARTY_STATE::REWARD)
        strBtnPath = "Assets/ui_common/btn_weaponkeep.png";
    
    if(objStage->getIdx() == 0)
        strBgPath = MafUtils::format(strBgPath.c_str(), objStage->getArea(), 1);
    else
        strBgPath = MafUtils::format(strBgPath.c_str(), objStage->getArea(), objStage->getIdx());
    
    auto sprBox = Sprite::create(strBgPath);
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setPosition(sizeLayout.width / 2, sizeLayout.height / 2);
    sprBox->getTexture()->setTexParameters(texParams);
    sprBox->setScale(3);
    uiLayout->addChild(sprBox);
    if(eType == E_PARTY_STATE::SEAL)
    {
        sprBox->setColor(Color3B(128, 128, 128));
    }
    
    auto sprStageBox = Sprite::create("Assets/ui_common/base_text_2_7.png");
    sprStageBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    sprStageBox->setPosition(0, sizeLayout.height);
    uiLayout->addChild(sprStageBox);
    {
        auto lbStage = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_advt_worldmap_place_%d_%d", objStage->getArea(), objStage->getIdx())), GAME_FONT, 26);
        lbStage->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbStage->setPosition( 10, sprStageBox->getContentSize().height/2);
        lbStage->setDimensions(sprStageBox->getContentSize().width - 10, sprStageBox->getContentSize().height-5);
        lbStage->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbStage->setOverflow(Label::Overflow::SHRINK);
        sprStageBox->addChild(lbStage);
    }
    
    auto btnRunning = ui::Button::create(strBtnPath);
    btnRunning->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnRunning->setPosition(Vec2(sizeLayout.width - 10, sizeLayout.height / 2));
    btnRunning->setZoomScale(0.05f);
    btnRunning->setTag(objStage->getIdx());
    uiLayout->addChild(btnRunning);
    switch (eType)
    {
        case E_PARTY_STATE::SEAL:
        {
            auto sprLock = Sprite::create("Assets/ui_common/tap_lock_2.png");
            sprLock->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            sprLock->setPosition(btnRunning->getContentSize()/2);
            btnRunning->addChild(sprLock);
                
            auto lbOpenLv = Label::createWithTTF(MafUtils::format("Lv.%d", objStage->getRequireLv()), GAME_FONT, 24);
            lbOpenLv->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            lbOpenLv->setPosition( btnRunning->getContentSize().width/2, btnRunning->getContentSize().height/2 - 5 );
            lbOpenLv->setDimensions(btnRunning->getContentSize().width - 10, btnRunning->getContentSize().height/2 - 10);
            lbOpenLv->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
            lbOpenLv->setOverflow(Label::Overflow::SHRINK);
            btnRunning->addChild(lbOpenLv);
            
            btnRunning->addClickEventListener(CC_CALLBACK_1(PopupAdventureStage::onClickFurnitureUpgrade, this));
        }break;
        case E_PARTY_STATE::OPEN:
        {
            auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_launch_ready_bt"), GAME_FONT, 24);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            lbText->setPosition( Vec2(btnRunning->getContentSize()/2) + Vec2::UP * 10);
            lbText->setDimensions(btnRunning->getContentSize().width - 10, btnRunning->getContentSize().height/2 - 10);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
            lbText->setOverflow(Label::Overflow::SHRINK);
            btnRunning->addChild(lbText);
            
            btnRunning->addClickEventListener(CC_CALLBACK_1(PopupAdventureStage::onClickRunning, this));
            auto sprTimer = Sprite::create("Assets/ui/offline/icon_offline_time.png");
            sprTimer->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            sprTimer->setScale(0.5f);
            sprTimer->setPosition(15, btnRunning->getContentSize().height/2 - 10);
            btnRunning->addChild(sprTimer);
            
            auto time = objStage->getTime();
            time -= time * decreseTimePer;
            
            int hour = time / 3600;
            int min = time % 3600 / 60;
            int sec = time % 3600 % 60;
            
            if(hour < 0)
                hour = 0;
            if(min < 0)
                min = 0;
            if(sec < 0)
                sec = 0;
            
            auto lbTimer = Label::createWithTTF(MafUtils::format("%02d:%02d:%02d", hour, min, sec), GAME_FONT, 24);
            lbTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            lbTimer->setPosition( sprTimer->getPosition() + Vec2::DOWN * (sprTimer->getContentSize().height/2 * sprTimer->getScale()) + Vec2::RIGHT * (sprTimer->getContentSize().width * sprTimer->getScale() + 10));
            lbTimer->setDimensions(90, btnRunning->getContentSize().height/2 - 10);
            lbTimer->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
            lbTimer->setOverflow(Label::Overflow::SHRINK);
            btnRunning->addChild(lbTimer);
            {
                if(decreseTimePer > 0)
                    lbTimer->setColor(Color3B(0,255,26));
            }
        }break;
        case E_PARTY_STATE::RUNNING:
        {
            auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_launch_ing_bt"), GAME_FONT, 24);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            lbText->setPosition( Vec2(btnRunning->getContentSize()/2) + Vec2::UP * 10);
            lbText->setDimensions(btnRunning->getContentSize().width - 10, btnRunning->getContentSize().height/2 - 10);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
            lbText->setOverflow(Label::Overflow::SHRINK);
            btnRunning->addChild(lbText);
            
            auto sprTimer = Sprite::create("Assets/ui/offline/icon_offline_time.png");
            sprTimer->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            sprTimer->setScale(0.5f);
            sprTimer->setPosition(25, btnRunning->getContentSize().height/2 - 10);
            btnRunning->addChild(sprTimer);
            
            auto lbTimer = Label::createWithTTF("", GAME_FONT, 24);
            lbTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            lbTimer->setPosition( sprTimer->getPosition() + Vec2::DOWN * (sprTimer->getContentSize().height/2 * sprTimer->getScale()) + Vec2::RIGHT * (sprTimer->getContentSize().width * sprTimer->getScale() + 10));
            lbTimer->setDimensions(70, btnRunning->getContentSize().height/2 - 10);
            lbTimer->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
            lbTimer->setOverflow(Label::Overflow::SHRINK);
            btnRunning->addChild(lbTimer);
            {
                if(decreseTimePer > 0)
                    lbTimer->setColor(Color3B(0,255,26));
            }
            
            auto callFunc = CallFunc::create([&, lbTimer, objStage](){
                auto time = objStage->getEndTime() - UtilsDate::getInstance()->getTime();
                
                int hour = time / 3600;
                int min = time % 3600 / 60;
                int sec = time % 3600 % 60;
                
                if(hour < 0)
                    hour = 0;
                if(min < 0)
                    min = 0;
                if(sec < 0)
                    sec = 0;
                
                lbTimer->setString(MafUtils::format("%02d:%02d:%02d", hour, min, sec));
                
                if(time < 0)
                {
                    lbTimer->stopAllActions();
                    return;
                }
            });
            auto delayTime = DelayTime::create(1);
            auto seq = Sequence::create(callFunc, delayTime, nullptr);
            lbTimer->runAction(RepeatForever::create(seq));
        }break;
        case E_PARTY_STATE::REWARD:
        {
            btnRunning->addClickEventListener(CC_CALLBACK_1(PopupAdventureStage::onClickReward, this));
            
            auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_launch_reward_bt"), GAME_FONT, 24);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbText->setPosition( btnRunning->getContentSize()/2);
            lbText->setDimensions(btnRunning->getContentSize().width - 10, btnRunning->getContentSize().height - 10);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbText->setOverflow(Label::Overflow::SHRINK);
            btnRunning->addChild(lbText);
            
            auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
            uiReddot->setPosition(10, btnRunning->getContentSize().height - 10);
            btnRunning->addChild(uiReddot);
            
            auto sprEffect = ui::Scale9Sprite::create("Assets/ui/adventure/adventure_box_on.png");
            sprEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprEffect->setContentSize(btnRunning->getContentSize());
            sprEffect->setPosition(btnRunning->getContentSize()/2);
            btnRunning->addChild(sprEffect);

            auto fadeOut = FadeOut::create(1.f);
            auto fadeIn = FadeIn::create(1.f);
            auto sequience = Sequence::create(fadeOut, fadeIn, nullptr);
            sprEffect->runAction(RepeatForever::create(sequience));
        }break;
        case E_PARTY_STATE::COMPLETE:
        {
            auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_launch_reward_bt"), GAME_FONT, 24);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbText->setPosition( btnRunning->getContentSize()/2);
            lbText->setDimensions(btnRunning->getContentSize().width - 10, btnRunning->getContentSize().height - 10);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbText->setOverflow(Label::Overflow::SHRINK);
            btnRunning->addChild(lbText);
            
            btnRunning->addClickEventListener([=](Ref*){
                Vec2 posToast = _spriteContainer->getParent()->convertToWorldSpace(_spriteContainer->getPosition());
                PopupToast::showImmediately(GAME_TEXT("t_ui_advt_worldmap_error_title_3"), posToast.x, posToast.y);
            });
        }break;
        default : break;
    }
    
    auto lyReward = LayerColor::create();
    lyReward->setContentSize( Size(sizeLayout.width - 180, 58));
    lyReward->setIgnoreAnchorPointForPosition(false);
    lyReward->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    lyReward->setPosition(10, 2);
    uiLayout->addChild(lyReward);
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
        
        auto listItems = objStage->getListReward();
        if ( objStage->getExp() > 0 )
        {
            auto objItemExp = ItemsManager::getInstance()->getInfoItems(E_ITEMS::ADVENTURE_EXP);
            if ( objItemExp != nullptr )
            {
                objItemExp->setCount(objStage->getExp());
                
                listItems.insert(0, objItemExp);
            }
        }
        
        for ( int i = 0; i < listItems.size(); i++ )
        {
            auto objItem = listItems.at(i);
            
            double posX = 58 * i + 5 * i;
            double posY = layerScroll->getContentSize().height / 2;
            innerWidth = posX + 58;
            
            auto pathBoxBG = "Assets/ui_common/box_item.png";
            if ( objItem->getIdx() == (int)E_ITEMS::ADVENTURE_EXP )
            {
                pathBoxBG = "Assets/ui_common/box_item_2_1.png";
            }
            
            auto spriteItemBG = ui::Scale9Sprite::create(pathBoxBG);
            spriteItemBG->setContentSize(Size(58, 58));
            spriteItemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            spriteItemBG->setPosition(posX, posY);
            layerScroll->addChild(spriteItemBG);
            
            auto spriteItems = Sprite::create(objItem->getPath());
            if ( spriteItems != nullptr )
            {
                switch((E_ITEMS)objItem->getIdx())
                {
                    case E_ITEMS::BADGE : spriteItems->setScale(1); break;
                    case E_ITEMS::INFINITE_ARTIFACT : spriteItems->setScale(1); break;
                        
                    default:
                        break;
                }
                
                spriteItems->setPosition(spriteItemBG->getContentSize().width / 2, spriteItemBG->getContentSize().height / 2 + 5);
                spriteItems->getTexture()->setTexParameters(texParams);
                spriteItemBG->addChild(spriteItems);
                if ( !objStage->isOpen() )
                {
                    spriteItems->setColor(Color3B(90,90,90));
                }
            }
            
            std::string strItemsCount = objItem->getCount();
            if ( objItem != nullptr && objItem->isString() == true )
            {
                MafUtils::convertNumberToShort(strItemsCount);
            }

            auto labelCount = ui::CText::create(strItemsCount, GAME_FONT, 14);
            labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            labelCount->setPosition(Vec2(spriteItemBG->getContentSize().width / 2, 0));
            labelCount->enableOutline(Color4B::BLACK, 2);
            spriteItemBG->addChild(labelCount);
            if(!objStage->isOpen())
            {
                labelCount->setColor(Color3B(90,90,90));
            }
        }
        layerScroll->setInnerContainerSize(Size(innerWidth, innerHeight));
        
        if(!objStage->isOpen())
        {
            lyReward->setColor(Color3B(90,90,90));
        }
    }
    
    auto lyRewardBoss = LayerColor::create();
    lyRewardBoss->setContentSize( Size(sizeLayout.width - 180, 46));
    lyRewardBoss->setIgnoreAnchorPointForPosition(false);
    lyRewardBoss->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    lyRewardBoss->setPosition(10, 62);
    uiLayout->addChild(lyRewardBoss);
    {
        auto layerScroll = ui::ScrollView::create();
        layerScroll->setBounceEnabled(false);
        layerScroll->setDirection(ui::ScrollView::Direction::HORIZONTAL);
        layerScroll->setContentSize(lyRewardBoss->getContentSize());
        layerScroll->setIgnoreAnchorPointForPosition(false);
        layerScroll->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        layerScroll->setPosition(Vec2(0, lyRewardBoss->getContentSize().height / 2));
        layerScroll->setSwallowTouches(false);
        lyRewardBoss->addChild(layerScroll);
        
        double innerWidth = 0;
        double innerHeight = layerScroll->getContentSize().height;
        
        auto listItems = objStage->getListRewardBossPoint();
        
        if ( AdventureManager::getInstance()->isAdventureBullet(objStage->getArea(), objStage->getIdx()) == true )
        {
            if ( auto objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::ADVENTURE_BOSS_BULLET_ALL); objItem != nullptr )
            {
                objItem->setCount(1);
                
                listItems.pushBack(objItem);
            }
        }
        
        for ( int i = 0; i < listItems.size(); i++ )
        {
            auto objItem = listItems.at(i);
            
            double posX = 46 * i + 5 * i;
            double posY = layerScroll->getContentSize().height / 2;
            innerWidth = posX + 46;

            auto spriteItemBG = ui::Scale9Sprite::create("Assets/ui_common/btn_2_6_on.png");
            spriteItemBG->setContentSize(Size(46, 46));
            spriteItemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            spriteItemBG->setPosition(posX, posY);
            layerScroll->addChild(spriteItemBG);
            
            auto spriteItems = Sprite::create(objItem->getPath());
            if ( spriteItems != nullptr )
            {
                spriteItems->setPosition(spriteItemBG->getContentSize().width / 2, spriteItemBG->getContentSize().height / 2 + 5);
                spriteItems->getTexture()->setTexParameters(texParams);
                spriteItemBG->addChild(spriteItems);
                if ( !objStage->isOpen() )
                {
                    spriteItems->setColor(Color3B(90,90,90));
                }
            }
            
            std::string strItemsCount = objItem->getCount();
            if ( objItem != nullptr && objItem->isString() == true )
            {
                MafUtils::convertNumberToShort(strItemsCount);
            }

            auto labelCount = ui::CText::create(strItemsCount, GAME_FONT, 14);
            labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            labelCount->setPosition(Vec2(spriteItemBG->getContentSize().width / 2, 0));
            labelCount->enableOutline(Color4B::BLACK, 2);
            spriteItemBG->addChild(labelCount);
            if(!objStage->isOpen())
            {
                labelCount->setColor(Color3B(90,90,90));
            }
        }
        layerScroll->setInnerContainerSize(Size(innerWidth, innerHeight));
        
        if(!objStage->isOpen())
        {
            lyRewardBoss->setColor(Color3B(90,90,90));
        }
    }
    
    int guildLevel = AdventureManager::getInstance()->getLevel();
    int guildLevelCondition = AdventureManager::getInstance()->getSetting("boss_open_map_lv")->getValueToInt();
    if ( guildLevel < guildLevelCondition )
    {
        lyRewardBoss->setVisible(false);
    }
    
    return cell;
}

ssize_t PopupAdventureStage::numberOfCellsInTableView(extension::TableView *table)
{
    int size = _infoMap->getListStage().size();
    
    if(_infoMap->getIdx() == 1 && AdventureManager::getInstance()->getTutorialStep() > 1)
        size -= 1;
    
    return size;
}

void PopupAdventureStage::setCallbackHide(const std::function<void(void)>& callback)
{
    _callbackHide = callback;
}
void PopupAdventureStage::setCallbackRefresh(const std::function<void(void)>& callback)
{
    _callbackRefresh = callback;
}
#pragma mark -init
bool PopupAdventureStage::init(const int nCurrentAdventureID, const int nCurrentArea, bool isFriend)
{
    if ( !PopupBase::init())
    {
        return false;
    }
    _nCurrentAdventureID = nCurrentAdventureID;
    _nCurrentArea = nCurrentArea;
    _isFriend = isFriend;
    
    _infoMap = AdventureManager::getInstance()->getInfoMap(_nCurrentArea);
    
    initVar();
    initUI();
    _bHideBackKey = false;
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE, this);
    
    return true;
}

void PopupAdventureStage::onEnter()
{
    PopupBase::onEnter();
    
}
#pragma mark - Override
void PopupAdventureStage::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if(eType == E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE)
        this->setLocalZOrder(ZORDER_SCENE_POPUP);
}
void PopupAdventureStage::show()
{
    PopupBase::show();
    
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        this->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
}
void PopupAdventureStage::initVar()
{
    _spriteContainer->setContentSize(Size(750, 1030));
}

void PopupAdventureStage::initUI()
{
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_advt_worldmap_title_1"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAdventureStage::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //650
    Size size = _spriteContainer->getContentSize();
    
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize( Size(size.width - 10, 70));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTopUI->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(_lyTopUI);
    
    _lyStage = LayerColor::create();
    _lyStage->setContentSize( Size(size.width - 10, _spriteContainer->getContentSize().height - 75 - _lyTopUI->getContentSize().height - 25));
    _lyStage->setIgnoreAnchorPointForPosition(false);
    _lyStage->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lyStage->setPosition(size.width / 2, 25);
    _spriteContainer->addChild(_lyStage);
    
    uiTop();
    uiTable();
}

#pragma mark -ui
void PopupAdventureStage::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    auto listTab = AdventureManager::getInstance()->getListInfoMap();
    int size = listTab.size();
    
    float width = _lyTopUI->getContentSize().width / size - 5;
    float posX = _lyTopUI->getContentSize().width/2 - width * (size/2) - 5 * (size/2);
    
    for(int i = 0; i < size; ++i)
    {
        auto obj = listTab.at(i);
        
        std::string strPath = "Assets/ui_common/btn_tap_off.png";
        std::string strNumberPath = "Assets/ui/adventure/adventure_no%d_2.png";
        if(obj->getIdx() == _nCurrentArea)
        {
            strPath = "Assets/ui_common/btn_tap_on.png";
            strNumberPath = "Assets/ui/adventure/adventure_no%d_1.png";
        }
        
        auto btnTab = ui::Button::create(strPath);
        btnTab->setScale9Enabled(true);
        btnTab->setContentSize(Size(width, 64));
        btnTab->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        btnTab->setPosition(Vec2(posX, 0));
        btnTab->addClickEventListener(CC_CALLBACK_1(PopupAdventureStage::onClickTab, this));
        btnTab->setZoomScale(0.05f);
        btnTab->setTag((int)obj->getIdx());
        _lyTopUI->addChild(btnTab);
        {
            if(obj->isOpen())
            {
                auto sprNumber = Sprite::create(MafUtils::format(strNumberPath.c_str(), i+1));
                sprNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                sprNumber->setPosition(Vec2(btnTab->getContentSize()/2));
                btnTab->addChild(sprNumber);
            }
            else
            {
                auto sprSealIcon = Sprite::create("Assets/ui_common/tap_lock.png");
                sprSealIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                sprSealIcon->setPosition(btnTab->getContentSize()/2);
                btnTab->addChild(sprSealIcon);
            }
            
        }
        
        if(size % 2 == 0)
            btnTab->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        
        posX += width + 5;
    }
}
void PopupAdventureStage::uiTable()
{
    _lyStage->removeAllChildren();
    
    _table = extension::TableView::create(this, Size(_lyStage->getContentSize().width, _lyStage->getContentSize().height));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition(_lyStage->getContentSize().width / 2, 0);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lyStage->addChild(_table);
    
    _table->reloadData();
}

#pragma mark -click
void PopupAdventureStage::onClickClose(Ref* const pSender)
{
    if(AdventureManager::getInstance()->getTutorialStep() == 1)
        return;
    hide();
}

void PopupAdventureStage::onClickTab(Ref* const pSender)
{
    auto node = (Node*)pSender;
    
    int nArea = node->getTag();
    
    auto objMap = AdventureManager::getInstance()->getInfoMap(nArea);
    
    if(!objMap->isOpen())
        return;
    
    _nCurrentArea = nArea;
    _infoMap = objMap;
    
    uiTop();
    _table->reloadData();
}

void PopupAdventureStage::onClickRunning(Ref* const pSender)
{
    auto node = (Node*)pSender;
    
    
    PopupLoading::show();
    if(_isFriend)
    {
        auto objFriend = AdventureManager::getInstance()->getInfoFriendFromUserIdx(_nCurrentAdventureID);
        AdventureManager::getInstance()->requestAdventureRunning(CC_CALLBACK_2(PopupAdventureStage::callbackRunning, this), AdventureManager::getInstance()->getInfoStage(_nCurrentArea, node->getTag()), objFriend->getPartyIdx(), objFriend->getUseridx(), objFriend->getPlatform());
    }
    else
    {
        AdventureManager::getInstance()->requestAdventureRunning(CC_CALLBACK_2(PopupAdventureStage::callbackRunning, this), AdventureManager::getInstance()->getInfoStage(_nCurrentArea, node->getTag()), _nCurrentAdventureID);
    }
}

void PopupAdventureStage::onClickReward(Ref* const pSender)
{
    auto node = (Node*)pSender;
    
    auto objStage = _infoMap->getStage(node->getTag());
    
    if(objStage == nullptr)
        return;
    
    int nPartyIdx = objStage->getPartyIdx();
    bool isFreind = nPartyIdx >= 100;
    PopupLoading::show();
    AdventureManager::getInstance()->requestAdventureReward(CC_CALLBACK_2(PopupAdventureStage::callbackReward, this), nPartyIdx, isFreind);
}

void PopupAdventureStage::onClickFurnitureUpgrade(Ref* const pSender)
{
    auto node = (Node*) pSender;
    
    auto objStage = _infoMap->getStage(node->getTag());
    int nOpenCondition = 0;
    if(objStage != NULL)
        nOpenCondition = objStage->getRequireLv();
    
    if(AdventureManager::getInstance()->getTutorialStep() > 6)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_advt_worldmap_error_title_2", nOpenCondition));
        popup->addButton(GAME_TEXT("t_ui_advt_bt_1"), true, [&](Ref*){
            auto popup = PopupAdventureFurnitureUpgrade::create(1);
            popup->setCallbackUpgrade(CC_CALLBACK_0(PopupAdventureStage::callbackRefresh, this));
            popup->show();
        });
        popup->addButton(GAME_TEXT("t_ui_close"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
    }
    else
    {
        Vec2 posToast = _spriteContainer->getParent()->convertToWorldSpace(_spriteContainer->getContentSize()/2);
        PopupToast::showImmediately(GAME_TEXT("t_ui_advt_error_7"), posToast.x, posToast.y);
    }
    
}
#pragma mark -callback
void PopupAdventureStage::callbackRunning(const bool bResult, const int nResult)
{
    PopupLoading::hide();
    if(nResult != 1 && nResult != 8)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if(nResult == 7)
            strMessageKey = "t_ui_advt_error_2";
        if(nResult == 4)
            strMessageKey = "t_ui_advt_error_4";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        
        if(nResult == 7)
        {
            ValueMap listData;
            listData["inspection_code"] = Value(nResult);
            RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE, listData);
        }
        
        if(_callbackHide != nullptr)
            _callbackHide();
        
        _callbackHide = nullptr;
        
        hide();
        return;
    }
    
    if(AdventureManager::getInstance()->getTutorialStep() == 1)
        AdventureManager::getInstance()->setTutorialStep(2);
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    if(_callbackHide != nullptr)
        _callbackHide();
    
    _callbackHide = nullptr;
    hide();
}

void PopupAdventureStage::callbackRefresh()
{
    uiTop();
    _table->reloadData();
    
    if(_callbackRefresh != nullptr)
        _callbackRefresh();
}

void PopupAdventureStage::callbackReward(const bool bResult, const int nResult)
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
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        
        if(_callbackHide != nullptr)
            _callbackHide();
        _callbackHide = nullptr;
        return;
    }
    callbackRefresh();
    //refreshUI();
    RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}
