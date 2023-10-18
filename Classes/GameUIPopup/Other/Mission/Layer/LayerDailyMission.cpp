//
//  LayerDailyMission.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/24.
//

#include "LayerDailyMission.h"

#include "GameScene/HelloWorldScene.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Common/MenuPopup.h"
#include "GameUIPopup/Other/SubFunction/PopupSubFunction.h"
#include "GameUIPopup/Other/Costume/PopupCostumeDefault.h"
#include "GameUIPopup/Other/Friend/PopupFriend.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidDefault.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ServerMissionManager.h"
#include "ManagerGame/PassManager.h"
#include "ManagerGame/WeaponManager.h"

USING_NS_CC;

LayerDailyMission* LayerDailyMission::create(const E_MISSION_RESET_TYPE type, const Size size)
{
    LayerDailyMission* pRet = new(std::nothrow) LayerDailyMission();
    if ( pRet && pRet->init(type, size) )
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
LayerDailyMission::LayerDailyMission():
_type(E_MISSION_RESET_TYPE::NONE)
,_isPlayTimeMissionComplete(false)
,_lyTable(nullptr)
,_table(nullptr)
,_callbackHide(nullptr)
,_callbackRefresh(nullptr)
{
    _vecDailyInfoMission.clear();
}
LayerDailyMission::~LayerDailyMission()
{
    _vecDailyInfoMission.clear();
}
bool LayerDailyMission::init(const E_MISSION_RESET_TYPE type, const Size size)
{
    if ( !LayerColor::initWithColor(Color4B()) )
    {
        return false;
    }
    
    setIgnoreAnchorPointForPosition(false);
    _type = type;
    setContentSize(size);
    _vecDailyInfoMission = ServerMissionManager::getInstance()->getListServerMissionInfo(_type);
    setServerMissionOder();
    
    initVar();
    initUI();
    
    return true;
}
#pragma mark - table
void LayerDailyMission::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
    
}

Size LayerDailyMission::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    return Size(_lyTable->getContentSize().width - 10, C_BAR_MISSION_SIZE + 10);
}

extension::TableViewCell* LayerDailyMission::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
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
    
    auto objMission = _vecDailyInfoMission.at(idx);
    
    if(objMission == nullptr)
        return cell;
    auto objItem = objMission->getListMissionReward().at(0);
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui/mission/mission_normal_1.png");
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setPosition(cell->getContentSize()/2);
    sprBox->setContentSize(Size(size.width, C_BAR_MISSION_SIZE));
    cell->addChild(sprBox);
    
    auto sprIconBox = Sprite::create("Assets/ui/mission/task_box_normal.png");
    sprIconBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprIconBox->setPosition(15, sprBox->getContentSize().height/2);
    sprBox->addChild(sprIconBox);
    if(objItem != nullptr)
    {
        auto spReward = Sprite::create(objItem->getPath());
        spReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        spReward->setPosition(sprIconBox->getContentSize()/2);
        spReward->setScale(87 / spReward->getContentSize().width);
        sprIconBox->addChild(spReward);
    }
    
    auto lbTitle = Label::createWithTTF(objMission->getTitle(), GAME_FONT, 26);//28로 맞출지는 보고 확인
    lbTitle->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbTitle->setPosition(sprIconBox->getPositionX() + sprIconBox->getContentSize().width + 30, sprBox->getContentSize().height - 5);
    lbTitle->setDimensions(370, lbTitle->getContentSize().height);
    lbTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    lbTitle->setOverflow(Label::Overflow::SHRINK);
    sprBox->addChild(lbTitle);
    
    auto lbTextCenter = Label::createWithTTF("", GAME_FONT, 20);
    lbTextCenter->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbTextCenter->setPosition(lbTitle->getPosition() + Vec2::DOWN * (lbTitle->getContentSize().height - 10));
    lbTextCenter->setDimensions(sprBox->getContentSize().width - 150, lbTextCenter->getContentSize().height);
    lbTextCenter->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    lbTextCenter->setOverflow(Label::Overflow::SHRINK);
    sprBox->addChild(lbTextCenter);
    if(objItem != nullptr)
    {
        std::string strCount = objItem->getCount();
        if ( objItem->getType().compare("BUFF") == 0 )
        {
            if ( objItem->getTypeDetail().compare("REVIVAL") == 0 )
            {
                strCount = GAME_TEXTFORMAT("t_ui_hour", atoi(objItem->getCount().c_str()));
            }
            else
            {
                strCount = GAME_TEXTFORMAT("t_ui_second", atoi(objItem->getCount().c_str()));
            }
        }
        lbTextCenter->setString(MafUtils::format("%s +%s", objItem->getText().c_str(), strCount.c_str()));
    }
    
    auto lbPassPoint = Label::createWithTTF(GAME_TEXTFORMAT("t_server_dailymission_passpoint", objMission->getPassPoint()), GAME_FONT, 18);
    lbPassPoint->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbPassPoint->setPosition(lbTextCenter->getPosition() + Vec2::DOWN * 22);
    lbPassPoint->setDimensions(200, lbPassPoint->getContentSize().height);
    lbPassPoint->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    lbPassPoint->setOverflow(Label::Overflow::SHRINK);
    lbPassPoint->setColor(Color3B(51, 226, 242));
    sprBox->addChild(lbPassPoint);
    
    auto lbTextBottom = Label::createWithTTF("", GAME_FONT, 19); //타이틀 내용은 비어있어야 한다.
    lbTextBottom->setColor(Color3B(167, 188, 208));
    lbTextBottom->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbTextBottom->setPosition(lbPassPoint->getPosition() + Vec2::DOWN * lbPassPoint->getContentSize().height);
    lbTextBottom->setDimensions(400, lbTextBottom->getContentSize().height);
    lbTextBottom->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    lbTextBottom->setOverflow(Label::Overflow::SHRINK);
    sprBox->addChild(lbTextBottom);
    
    Sprite* sprBarBackground = Sprite::create("Assets/ui/prison/quest_gauge_bg.png");
    sprBarBackground->setScaleX(1.8f);
    sprBarBackground->setPosition(Vec2(sprIconBox->getPositionX() + sprIconBox->getContentSize().width + 30, sprBox->getContentSize().height*0.15));
    sprBarBackground->setAnchorPoint(Vec2(0,0));
    
    sprBox->addChild(sprBarBackground);

    Sprite *sprBar = Sprite::create("Assets/ui/prison/quest_gauge_c.png");
    sprBar->setAnchorPoint(Vec2(0,0.5));
    
    ProgressTimer *bar = ProgressTimer::create(sprBar);
    bar->setType(ProgressTimer::Type::BAR);
    bar->setPercentage(0);
    bar->setMidpoint(Vec2(0,0.5));
    bar->setBarChangeRate(Vec2(1,0));
    bar->setAnchorPoint(Vec2(0,0));
    sprBarBackground->addChild(bar);
    
    Label *lbProgress = Label::createWithTTF("", GAME_FONT, 20);
    lbProgress->setColor(Color3B::WHITE);
    lbProgress->setPosition(sprBarBackground->getPosition().x+sprBarBackground->getContentSize().width*sprBarBackground->getScaleX()/2,
                             sprBarBackground->getPosition().y+sprBarBackground->getContentSize().height/2);
    lbProgress->setAnchorPoint(Vec2(0.5,0.5));
    sprBox->addChild(lbProgress);
    
    int maxCount = objMission->getCompleteCount();
    int curCount = objMission->getCurrentCount();
    if(curCount > maxCount)
    {
        curCount = maxCount;
    }
    
    if (curCount == 0) {
        bar->setPercentage(0);
    }else{
        bar->setPercentage(((float)curCount/(float)maxCount)*100);
    }

    std::string strTextBottom = "";
    lbTitle->disableEffect(LabelEffect::STRIKETHROUGH);
    if(objMission->isEventMission())
    {
        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::EVENT, Vec2::ANCHOR_MIDDLE);
        uiReddot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiReddot->setPosition(sprIconBox->getContentSize().width/2, 0);
        sprIconBox->addChild(uiReddot);
    }

    sprBarBackground->setVisible(true);
    lbProgress->setVisible(true);
    lbTextBottom->setString(strTextBottom);
    if(objMission->getMissionType() == E_DAILY_QUEST::PLAYTIME_S)
    {
        auto callFunc = CallFunc::create([&, lbProgress, objMission, bar](){
            if(!_isPlayTimeMissionComplete && objMission->isComplete() &&
               !objMission->isServerComplete())
            {
                _isPlayTimeMissionComplete = true;
                requestUpdate();
                return;
            }
            
            int maxCount = objMission->getCompleteCount();
            int curCount = objMission->getCurrentCount();
            if(curCount > maxCount)
            {
                curCount = maxCount;
            }
            
            int curHr = curCount / 3600;
            int curMin;
            if(curHr >= 1)
                curMin = curCount % 3600 / 60;
            else
                curMin = curCount / 60;
            int curSec = curCount % 60;
            
            
            int maxHr = maxCount / 3600;
            int maxMin;
            if(maxHr >= 1)
                 maxMin = maxCount % 3600 / 60;
            else
                maxMin = maxCount / 60;
            int maxSec = maxCount % 60;
            
            if(maxHr >= 1)
                lbProgress->setString(MafUtils::format("%02d : %02d : %02d / %02d : %02d : %02d", curHr, curMin, curSec, maxHr, maxMin, maxSec));
            else
                lbProgress->setString(MafUtils::format("%02d : %02d / %02d : %02d", curMin, curSec, maxMin, maxSec));
            
            if (curCount == 0) {
                bar->setPercentage(0);
            }else{
                bar->setPercentage(((float)curCount/(float)maxCount)*100);
            }
        });
        auto seq = Sequence::create(callFunc, DelayTime::create(1), nullptr);
        lbProgress->runAction(RepeatForever::create(seq));
    }
    else
    {
        lbProgress->setString(MafUtils::format("%d / %d", curCount,maxCount));
    }
    
    //
    
    MafNode::MafMenu *menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    sprBox->addChild(menu);
    
    std::string btnPath = "Assets/ui/mission/btn_shortcuts_1.png";
    auto func = CC_CALLBACK_1(LayerDailyMission::onClickShortCut, this);
    if(objMission->isComplete() && !objMission->isServerComplete())
    {
        btnPath = "Assets/ui/mission/btn_acheive_1.png";
        func = CC_CALLBACK_1(LayerDailyMission::onClickComplete, this);
    }
    auto btnReceive = MafNode::MafMenuItemSprite::create(Sprite::create(btnPath), Sprite::create(btnPath), Sprite::create(btnPath), func);
    btnReceive->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnReceive->setPosition(Vec2(sprBox->getContentSize().width - 25, sprBox->getContentSize().height/2));
    btnReceive->setTag((int)idx);
    menu->addChild(btnReceive);
    
    if(objMission->isServerComplete())
    {
        lbTitle->enableStrikethrough();
        btnReceive->setVisible(false);
        
    }
    
    if(objMission->hasCondition())
    {
        if(!ServerMissionManager::getInstance()->checkMissionCondition(objMission->getMissionType()))
        {
            btnReceive->setVisible(false);
            
            auto spriteBlack = ui::Scale9Sprite::create(PIXEL,Rect(0,0,sprBox->getContentSize().width, sprBox->getContentSize().height));
            spriteBlack->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            spriteBlack->setPosition(0, sprBox->getContentSize().height);
            spriteBlack->setColor(Color3B::BLACK);
            spriteBlack->setOpacity(150);
            sprBox->addChild(spriteBlack);
            
            auto sprLock = Sprite::create("Assets/ui_common/tap_lock_2.png");
            sprLock->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprLock->setPosition(sprIconBox->getPositionX() - sprIconBox->getContentSize().width/2 + sprLock->getContentSize().width * 1 / 4, sprIconBox->getPositionY()+sprIconBox->getContentSize().height/2 - sprLock->getContentSize().height * 1 / 3   );
            sprBox->addChild(sprLock);
            
            auto lbCondition = Label::createWithTTF("", GAME_FONT, 25);
            lbCondition->setPosition(btnReceive->getPosition());
            lbCondition->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbCondition->setDimensions(btnReceive->getContentSize().width * 1.2, btnReceive->getContentSize().height);
            lbCondition->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbCondition->setOverflow(Label::Overflow::SHRINK);
            switch (objMission->getMissionType())
            {
                case E_DAILY_QUEST::HELLTRIUM_GET:
                    lbCondition->setString(GAME_TEXT("t_server_dailymission_block2"));
                    break;
                case E_DAILY_QUEST::MINERAL_BUY:
                    lbCondition->setString(GAME_TEXT("t_server_dailymission_block3"));
                    break;
                default:
                    break;
            }
            sprBox->addChild(lbCondition);
        }
    }
    return cell;
}

ssize_t LayerDailyMission::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return _vecDailyInfoMission.size();
}

#pragma mark -init
void LayerDailyMission::initVar()
{
}

void LayerDailyMission::initUI()
{
    _lyTable = LayerColor::create();
    _lyTable->setContentSize( getContentSize() - Size(10, 10));
    _lyTable->setIgnoreAnchorPointForPosition(false);
    _lyTable->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    _lyTable->setPosition(Vec2(getContentSize() / 2) + Vec2(5, 0) );
    addChild(_lyTable);
    
    uiTable();
}

#pragma mark -ui
void LayerDailyMission::uiTable()
{
    _lyTable->removeAllChildren();
    
    _table = extension::TableView::create(this, Size(_lyTable->getContentSize().width, _lyTable->getContentSize().height));
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

#pragma mark -utils
void LayerDailyMission::onClickShortCut(Ref* sender)
{
    auto node = (Node*)sender;
    
    auto objMission = _vecDailyInfoMission.at(node->getTag());
    if(objMission == nullptr)
        return;
    
    switch (objMission->getShortcut())
    {
        case SHORTCUT_NONE:
        {
        }
            break;
            
        case SHORTCUT_QUEST:
        {
            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                MafNode::MafMenuItemSprite* item = (MafNode::MafMenuItemSprite*)dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_menu_btn->getChildByName("QUEST");
                item->setName("QUEST");
                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onButtonTouched(item);
            }
            break;
        }
        case SHORTCUT_EQUIP:
        {
            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                MafNode::MafMenuItemSprite* item = (MafNode::MafMenuItemSprite*)dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_menu_btn->getChildByName("EQUIP");
                item->setName("EQUIP");
                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onButtonTouched(item);
            }
            break;
        }
        case SHORTCUT_PET:
        case SHORTCUT_PETEQUIP:
        {
            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                MafNode::MafMenuItemSprite* item = (MafNode::MafMenuItemSprite*)dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_menu_btn->getChildByName("PET");
                item->setName("PET");
                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onButtonTouched(item);
            }
            break;
        }
        case SHORTCUT_ARTIFACT_NORMAL:
        {
            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                MafNode::MafMenuItemSprite* item = (MafNode::MafMenuItemSprite*)dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_menu_btn->getChildByName("ARTIFACT");
                item->setName("ARTIFACT");
                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onButtonTouched(item);
               
            }
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                //dynamic_cast<HelloWorldScene *>(sceneGameLayer)->createArtifactTable(1);
                auto item = dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_cellArtifacTop;
                item->onClickTab(item->getChildByTag(1000)->getChildByTag(1));
                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_scroll[2]->setContentOffset(Vec2(0,0));

            }
            break;
        }
        case SHORTCUT_ARTIFACT_SPECIAL:
        {
            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                MafNode::MafMenuItemSprite* item = (MafNode::MafMenuItemSprite*)dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_menu_btn->getChildByName("ARTIFACT");
                item->setName("ARTIFACT");
                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onButtonTouched(item);
               
            }
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                //dynamic_cast<HelloWorldScene *>(sceneGameLayer)->createArtifactTable(1);
                auto item = dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_cellArtifacTop;
                item->onClickTab(item->getChildByTag(1000)->getChildByTag(0));

            }
            break;
        }
        case SHORTCUT_RELIC:
        {
            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                 MafNode::MafMenuItemSprite* item = (MafNode::MafMenuItemSprite*)dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_menu_btn->getChildByName("ARTIFACT");
                item->setName("ARTIFACT");
                
                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onButtonTouched(item);
                
            }
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                //dynamic_cast<HelloWorldScene *>(sceneGameLayer)->createArtifactTable(1);
                auto item = dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_cellArtifacTop;
                item->onClickTab(item->getChildByTag(1000)->getChildByTag(1000));

            }
            break;
        }
        case SHORTCUT_SHOP:
        {
            auto popup = PopupSubFunction::create(PopupSubFunction::E_TYPE::SHOPITEM);
            popup->show();
            break;
        }
        case SHORTCUT_PRESTIGE:
        {
            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onClickoRevive();
            }
            break;
        }
        case SHORTCUT_MINE:
        {
            if ( WeaponManager::getInstance()->getEquipHigh() >= E_WEAPON::WEAPON_76_MINE )
            {
                auto popup = PopupSubFunction::create(PopupSubFunction::E_TYPE::MINE);
                popup->show();
            }
            else
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_12"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
            }
            break;
        }
        case SHORTCUT_BUILDING:
        {
            auto popup = PopupSubFunction::create(PopupSubFunction::E_TYPE::TOWN);
            popup->show();
            break;
        }
        case SHORTCUT_MYHOME:
        {
            auto popup = PopupSubFunction::create(PopupSubFunction::E_TYPE::MYHOME);
            popup->show();
            break;
        }
        case SHORTCUT_MENU:
        {
            auto popup = MenuPopup::create();
            popup->show();
            break;
        }
        case SHORTCUT_GIFT:
        {
            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                auto item = MafNode::MafMenuItem::create();
                item->setName("GIFT");
                
                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onButtonTouched(item);
            }
            break;
        }
        case SHORTCUT_ATTEND:
        {
            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                auto item = MafNode::MafMenuItem::create();
                item->setName("ATTEND");
                
                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onButtonTouched(item);
            }
            break;
        }
        case SHORTCUT_COSTUME:
        {
            if ( AccountManager::getInstance()->getUserIdx() == 0 )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                return;
            }
            
            auto popup = PopupCostumeDefault::create();
            popup->show();
            break;
        }
        case SHORTCUT_PRISON:
        {
            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                auto item = MafNode::MafMenuItem::create();
                item->setName("DUNGEON_INPUT");
                
                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onButtonTouched(item);
            }
            break;
        }
        case SHORTCUT_RAID:
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
            
            break;
        }
        case SHORTCUT_FRIENDS:
        {
            if ( AccountManager::getInstance()->getUserIdx() == 0 )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                return;
            }
            
            auto popup = PopupFriend::create();
            popup->show();
            break;
        }
            
        default:
            break;
    }
    
    if(_callbackHide != nullptr)
    {
        _callbackHide();
    }
}
void LayerDailyMission::onClickComplete(Ref* sender)
{
    auto node = (Node*)sender;
    
    auto objMission = _vecDailyInfoMission.at(node->getTag());
    if(objMission == nullptr)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    PopupLoading::show();
    ServerMissionManager::getInstance()->requestMissionReward(objMission, CC_CALLBACK_2(LayerDailyMission::callbackComplete, this));
}

void LayerDailyMission::requestUpdate()
{
    ServerMissionManager::getInstance()->requestUpdateMission(true, _type, CC_CALLBACK_2(LayerDailyMission::callbackUpdate, this));
}
void LayerDailyMission::setServerMissionOder()
{
    Vector<InfoServerMission*> vecTemp;
    
    for (auto iter = _vecDailyInfoMission.rbegin(); iter != _vecDailyInfoMission.rend(); ++iter)
    {
        auto obj = (InfoServerMission*)*iter;
        
        if(obj->isComplete() && obj->isServerComplete())
        {
            vecTemp.pushBack(obj);
            _vecDailyInfoMission.eraseObject(obj);
        }
    }
    
    for(auto info : vecTemp)
    {
        _vecDailyInfoMission.pushBack(info);
    }
    
}
void LayerDailyMission::setCallbackHide(const std::function<void()>& pCallback)
{
    _callbackHide = pCallback;
}
void LayerDailyMission::setCallbackRefresh(const std::function<void()>& pCallback)
{
    _callbackRefresh = pCallback;
}
#pragma mark -callback
void LayerDailyMission::callbackUpdate(bool bResult, int nResult)
{    
    if(bResult == false || nResult != 1)
    {
        std::string text = "t_ui_error_2";
        if(nResult == 10)
            text = "t_ui_error_1";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(text));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
    }
    
    setServerMissionOder();
    _table->reloadData();
}

void LayerDailyMission::callbackComplete(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if(bResult == false || nResult != 1)
    {
        std::string text = "t_ui_error_2";
        if(nResult == 10)
            text = "t_ui_error_1";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(text));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
    }
    
    setServerMissionOder();
    _table->reloadData();
    
    PopupLoading::show();
    PassManager::getInstance()->requestInfo([=](bool bResult){
        PopupLoading::hide();
        if(bResult == false)
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
        if(_callbackRefresh != nullptr)
            _callbackRefresh();
    });
    
    //
    ServerMissionManager::getInstance()->requestUpdateMission(true, E_MISSION_RESET_TYPE::DAILY, nullptr);
    ServerMissionManager::getInstance()->requestUpdateMission(true, E_MISSION_RESET_TYPE::WEEK, nullptr);
}
