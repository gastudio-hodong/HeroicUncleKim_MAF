//
//  LayerNormalDungeon.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/09/07.
//

#include "LayerNormalDungeon.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "GameUIPopup/Other/BattlePrison/PopupPrison.h"
#include "GameUIPopup/Other/BattleFrost/PopupFrost.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidDefault.h"
#include "GameUIPopup/Other/Adventure/Guild/PopupAdventureGuild.h"
#include "GameUIPopup/Other/Flashman/PopupFlashman.h"
#include "GameUIPopup/Other/Defense/PopupDefenseDefault.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/TutorialManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/WeaponManager.h"
#include "ManagerGame/DefenseManager.h"
#include "ManagerGame/DefenseManaAMPManager.h"
#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/PrisonManager.h"
#include "ManagerGame/PrisonFrostManager.h"
#include "ManagerGame/FlashmanManager.h"
#include "ManagerGame/SaveManager.h"

#include "ManagerEvent/EventManager.h"

USING_NS_CC;

LayerNormalDungeon* LayerNormalDungeon::create(E_PLACE ePlace)
{
    LayerNormalDungeon* pRet = new(std::nothrow) LayerNormalDungeon();
    if ( pRet && pRet->init(ePlace) )
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

LayerNormalDungeon::LayerNormalDungeon(void) :
_isSceneMove(false),
_bInitUI(false),

_ePlace(E_PLACE::EMPTY),
_uiContentsUIList(nullptr),
_uiList(nullptr)
{
    _dungeonList.clear();
}

LayerNormalDungeon::~LayerNormalDungeon(void)
{
    _dungeonList.clear();
}

bool LayerNormalDungeon::init(E_PLACE ePlace)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    _ePlace = ePlace;
    updateDungeonList();
    
    // init
    initVar();
    initUi();
    
    return true;
}

void LayerNormalDungeon::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiList();
    }
    
    // draw
    drawList();
    
}

#pragma mark - override DelegateListView
void LayerNormalDungeon::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
{
    switch (type) {
        case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
            break;
        case ui::ScrollView::EventType::SCROLL_TO_TOP:
            break;
        default:
            break;
    }
}

Size LayerNormalDungeon::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 155;

    return Size(width, height);
}

void LayerNormalDungeon::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    Size sizeLayout = Size(size.width, 150);

    auto dungeon = _dungeonList[idx];
    
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(sizeLayout.width, sizeLayout.height));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    childview->addChild(uiLayout);
    
    std::string strImagePath = "";
    std::string strNameKey = "";
    Vector<InfoItems*> listItems;
    
    switch (dungeon)
    {
        case E_DUNGEON::D_NEW_RAID:
        {
            strImagePath = "Assets/ui/dungeoninput/btn_dragon_slayer.png";
            strNameKey = "t_ui_new_raid_title";
            
            auto item = ItemsManager::getInstance()->getInfoItems(E_ITEMS::WEAPON_SPIRIT);
            item->setTypeParams(1);
            
            listItems.pushBack(item);
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::NEW_RAID_DRAGON_BALL));
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::SPIRIT_STON));
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::MATERIAL));
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::HONOR));
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::POINT));
        }
            break;
        case E_DUNGEON::D_PRISON:
        {
            strImagePath = "Assets/ui/dungeoninput/btn_move_prison.png";
            strNameKey = "t_ui_prison";
            
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::COSTUME_COIN));
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::KEY));
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::POINT));
        }
            break;
        case E_DUNGEON::D_FLASHMAN:
        {
            strImagePath = "Assets/ui/dungeoninput/btn_flashman.png";
            strNameKey = "t_ui_minigame_title_1";
            
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::LOOT_PIECE_1));
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::LOOT_PIECE_2));
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::LOOT_PIECE_3));
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::LOOT_PIECE_4));
        }
            break;
        case E_DUNGEON::D_PRISON_FROST:
        {
            strImagePath = "Assets/ui/dungeoninput/btn_frost_prison.png";
            strNameKey = "t_ui_frost_prison_title";
            
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::FROST_PRISON_POINT));
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::NEW_RAID_DRAGON_BALL));
        }
            break;
        case E_DUNGEON::D_DUNGEON_NORMAL:
        {
            strImagePath = "Assets/ui/dungeoninput/btn_move_normal.png";
            strNameKey = "t_ui_dungeon_normal";
            
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::KEY));
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::GEM));
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::POINT));
        }
            break;
        case E_DUNGEON::D_DUNGEON_DEVIL:
        {
            strImagePath = "Assets/ui/dungeoninput/btn_move_devil.png";
            strNameKey = "t_ui_dungeon_devil";
            
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::GEM));
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::POINT));
        }
            break;
        case E_DUNGEON::D_ADVENTURE:
        {
            strImagePath = "Assets/ui/dungeoninput/btn_adventure.png";
            strNameKey = "t_ui_advt_info_title_1";
            
            
            auto itemCore = ItemsManager::getInstance()->getInfoItems(E_ITEMS::ADVENTURE_CORE);
            itemCore->setTypeParams(5);
            listItems.pushBack(itemCore);
            auto itemRelic = ItemsManager::getInstance()->getInfoItems(E_ITEMS::ADVENTURE_RELIC);
            itemRelic->setTypeParams(0);
            listItems.pushBack(itemRelic);
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::MINE_BOX));
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::GEM));
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::POINT));
        }
            break;
        case E_DUNGEON::D_DEFENSE:
        {
            strImagePath = "Assets/ui/dungeoninput/btn_move_defense.png";
            strNameKey = "t_ui_defense_title_1";
            
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::DEFENSE_COIN));
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::DEFENSE_CHARACTER));
            listItems.pushBack(ItemsManager::getInstance()->getInfoItems(E_ITEMS::DEFENSE_WEAPON));
            
            auto manaAMP = InfoItems::create(E_ITEMS::NONE);
            manaAMP->setPath("Assets/icon/icon_defense_power_amplifier.png");
            listItems.pushBack(manaAMP);
        }
            break;
    }
    
    auto spriteBg = ui::Scale9Sprite::create(PIXEL, Rect(0,0,getContentSize().width * 0.98, 142));
    spriteBg->setColor(Color3B(29,41,51));
    spriteBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    spriteBg->setPosition(uiLayout->getContentSize() / 2);
    uiLayout->addChild(spriteBg);
    
    auto BtnBg = ui::Button::create(strImagePath);
    BtnBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    BtnBg->setPosition(uiLayout->getContentSize() / 2);
    BtnBg->setZoomScale(0.01f);
    BtnBg->setTag((int)dungeon);
    BtnBg->addClickEventListener(CC_CALLBACK_1(LayerNormalDungeon::onClickDungeon, this));
    uiLayout->addChild(BtnBg);
    
    auto labelTitle = Label::createWithTTF(GAME_TEXT(strNameKey), GAME_FONT, 34);
    labelTitle->setDimensions(BtnBg->getContentSize().width - 280, BtnBg->getContentSize().height/2);
    labelTitle->setOverflow(Label::Overflow::SHRINK);
    labelTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::BOTTOM);
    labelTitle->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    labelTitle->setPosition(150, BtnBg->getContentSize().height / 2);
    labelTitle->enableBold();
    BtnBg->addChild(labelTitle);
    
    for ( int i = 0; i < listItems.size(); i++ )
    {
        auto objReward = listItems.at(i);
        std::string strPath = objReward->getPath();
        if ( strPath.length() != 0 )
        {
            auto spriteRewardBox = ui::Scale9Sprite::create("Assets/ui_common/box_item.png", Rect::ZERO, Rect(5,30,10,10));
            spriteRewardBox->setContentSize(Size(62, 62));
            spriteRewardBox->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
            spriteRewardBox->setPosition(150 + i * spriteRewardBox->getContentSize().width + i * 5, BtnBg->getContentSize().height / 2);
            BtnBg->addChild(spriteRewardBox);
            
            auto spriteReward = Sprite::create(strPath);
            spriteReward->setPosition(spriteRewardBox->getBoundingBox().size.width / 2,spriteRewardBox->getBoundingBox().size.height / 2);
            spriteRewardBox->addChild(spriteReward);
        }
    }
    
    //
    auto spriteInput = Sprite::create("Assets/ui/dungeoninput/btn_move.png");
    spriteInput->setPosition(BtnBg->getContentSize().width * 0.87, BtnBg->getContentSize().height / 2);
    BtnBg->addChild(spriteInput);
    
    //
    if (dungeon == D_NEW_RAID)
    {
        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
        uiReddot->setPosition(20, BtnBg->getContentSize().height - 20);
        uiReddot->setVisible(false);
        BtnBg->addChild(uiReddot);
        
        auto action1 = CallFuncN::create([=](Ref* sender){
            auto uiReddot = (UIReddot*)sender;
            
            bool isActivate = false;
            
            if((NewRaidManager::getInstance()->getAttackCount() >= 1 ||
                NewRaidManager::getInstance()->isLevelUpPossible() ||
                NewRaidManager::getInstance()->isMissionComplete()) && NewRaidManager::getInstance()->isRaidEnterCondition())
            {
                isActivate = true;
            }
            
            uiReddot->setVisible(isActivate);
        });
        auto action2 = DelayTime::create(2);
        auto seq = Sequence::create(action1, action2, NULL);
        uiReddot->runAction(RepeatForever::create(seq));
        
    }
    else if( dungeon == D_PRISON )
    {
        double nPosTempX = 400;
        double nPosTempY = BtnBg->getContentSize().height / 2;
        
        //
        {
            auto spriteIcon = Sprite::create("Assets/icon/icon_freepass_2.png");
            spriteIcon->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
            spriteIcon->setPosition(nPosTempX, nPosTempY);
            spriteIcon->setVisible(false);
            BtnBg->addChild(spriteIcon);
            {
                auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
                uiReddot->setPosition(-5, spriteIcon->getContentSize().height);
                spriteIcon->addChild(uiReddot);
                
                auto action1 = CallFuncN::create([=](Node* sender){
                    
                    if ( PrisonManager::getInstance()->getHeart() >= PrisonManager::getInstance()->getHeartMax() )
                    {
                        sender->setVisible(true);
                    }
                    else
                    {
                        sender->setVisible(false);
                    }
                    
                });
                auto action2 = DelayTime::create(1.0);
                auto seq = Sequence::create(action1, action2, NULL);
                spriteIcon->runAction(RepeatForever::create(seq));
            }
            nPosTempX += spriteIcon->getContentSize().width + 5;
        }
        
        //
        {
            auto spriteIcon = Sprite::create("Assets/icon/icon_jelly58.png");
            spriteIcon->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
            spriteIcon->setPosition(nPosTempX, nPosTempY);
            spriteIcon->setVisible(false);
            BtnBg->addChild(spriteIcon);
            {
                auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
                uiReddot->setPosition(-5, spriteIcon->getContentSize().height);
                spriteIcon->addChild(uiReddot);
                
                auto action1 = CallFuncN::create([=](Node* sender){
                    
                    if ( PrisonManager::getInstance()->isJellyRewardMax() == true )
                    {
                        sender->setVisible(true);
                    }
                    else
                    {
                        sender->setVisible(false);
                    }
                    
                });
                auto action2 = DelayTime::create(1.0);
                auto seq = Sequence::create(action1, action2, NULL);
                spriteIcon->runAction(RepeatForever::create(seq));
            }
            nPosTempX += spriteIcon->getContentSize().width + 10;
        }
        if ( EventManager::getInstance()->isActivatedEvent(10) )
        {
            auto uiReddot = UIReddot::create(E_REDDOT_TYPE::EVENT, Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiReddot->setPosition(BtnBg->getContentSize().width * 0.87, BtnBg->getContentSize().height - 30);
            BtnBg->addChild(uiReddot);
        }
    }
    else if (dungeon == D_ADVENTURE)
    {
        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
        uiReddot->setPosition(20, BtnBg->getContentSize().height - 20);
        uiReddot->setVisible(false);
        BtnBg->addChild(uiReddot);
        
        auto action1 = CallFuncN::create([=](Ref* sender){
            auto uiReddot = (UIReddot*)sender;
            
            bool isActivate = AdventureManager::getInstance()->isAdventureComplete();
            uiReddot->setVisible(isActivate);
        });
        auto action2 = DelayTime::create(2);
        auto seq = Sequence::create(action1, action2, NULL);
        uiReddot->runAction(RepeatForever::create(seq));
        
    }
    else if (dungeon == D_DEFENSE)
    {
        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
        uiReddot->setPosition(20, BtnBg->getContentSize().height - 20);
        uiReddot->setVisible(false);
        BtnBg->addChild(uiReddot);
            
        auto action1 = CallFuncN::create([=](Ref* sender){
            auto uiReddot = (UIReddot*)sender;
            
            bool isActivate = false;
            if(!DefenseManager::getInstance()->isFirstGift())
                return;
            
            if(DefenseManager::getInstance()->isReddotCondition() || DefenseManaAMPManager::getInstance()->isReddotCondition())
            {
                isActivate = true;
            }
            
            uiReddot->setVisible(isActivate);
        });
        auto action2 = DelayTime::create(2);
        auto seq = Sequence::create(action1, action2, NULL);
        uiReddot->runAction(RepeatForever::create(seq));
            
    }
    
}

int64_t LayerNormalDungeon::listviewNumberOfCells(ui::CListView *listview)
{
    return _dungeonList.size();
}

#pragma mark - init
void LayerNormalDungeon::initVar()
{
    
}

void LayerNormalDungeon::initUi()
{
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
}

#pragma mark - utill
void LayerNormalDungeon::updateDungeonList()
{
    _dungeonList.clear();
    
    switch(_ePlace)
    {
        case DG_NORMAL:
        {
            _dungeonList.push_back(E_DUNGEON::D_NEW_RAID);
            _dungeonList.push_back(E_DUNGEON::D_PRISON);
            _dungeonList.push_back(E_DUNGEON::D_PRISON_FROST);
            _dungeonList.push_back(E_DUNGEON::D_ADVENTURE);
            _dungeonList.push_back(E_DUNGEON::D_DEFENSE);
            _dungeonList.push_back(E_DUNGEON::D_DUNGEON_DEVIL);
            if ( FlashmanManager::getInstance()->getInfoSetting("minigame_switch")->getValueToBool() == true )
            {
                _dungeonList.insert(_dungeonList.begin() + 2, E_DUNGEON::D_FLASHMAN);
            }
            if(UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= 10000)
            {
                for(auto iter = _dungeonList.begin(); iter != _dungeonList.end(); iter++)
                {
                    if(*iter == E_DUNGEON::D_DEFENSE)
                    {
                        _dungeonList.erase(iter);
                        break;
                    }
                }
                _dungeonList.insert(_dungeonList.begin() + 2, E_DUNGEON::D_DEFENSE);
            }
        }break;
        case DG_DEVIL_DOM:
        {
            _dungeonList.push_back(E_DUNGEON::D_NEW_RAID);
            _dungeonList.push_back(E_DUNGEON::D_PRISON);
            _dungeonList.push_back(E_DUNGEON::D_PRISON_FROST);
            _dungeonList.push_back(E_DUNGEON::D_ADVENTURE);
            _dungeonList.push_back(E_DUNGEON::D_DUNGEON_NORMAL);
            if ( FlashmanManager::getInstance()->getInfoSetting("minigame_switch")->getValueToBool() == true )
            {
                _dungeonList.insert(_dungeonList.begin() + 2, E_DUNGEON::D_FLASHMAN);
            }
        }break;
    }
    
    drawList();
}

#pragma mark - ui
void LayerNormalDungeon::uiContainer()
{
    Size size = this->getContentSize();
    
    _uiContentsUIList = ui::Layout::create();
    _uiContentsUIList->setContentSize(Size(size.width, size.height));
    _uiContentsUIList->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsUIList->setPosition(Vec2(size.width / 2, 0));
    addChild(_uiContentsUIList);
}

void LayerNormalDungeon::uiList()
{
    auto layout = _uiContentsUIList;
    Size size = layout->getContentSize();
    
    _uiList = ui::CListView::create(this, Size(size.width, size.height), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(Vec2(size.width / 2, 0));
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    layout->addChild(_uiList);
}

#pragma mark - draw
void LayerNormalDungeon::drawList()
{
    if(_uiList == nullptr)
        return;
    
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}
 
#pragma mark - set, get, add

#pragma mark - callback

#pragma mark - click
void LayerNormalDungeon::onClickDungeon(cocos2d::Ref* sender)
{
    if ( AccountManager::getInstance()->getUserIdx() == 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto item = (MenuItem*)sender;
    if ( item->getTag() == E_DUNGEON::D_PRISON )
    {
        auto popup = PopupPrison::create();
        popup->show();
    }
    else if ( item->getTag() == E_DUNGEON::D_PRISON_FROST )
    {
        if ( PrisonFrostManager::getInstance()->getInfoSetting("frost_prison_switch")->getValueToBool() == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_error_2"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
            
        if ( WeaponManager::getInstance()->getEquipHigh() >= E_WEAPON::WEAPON_60 )
        {
            auto popup = PopupFrost::create();
            popup->show();
        }
        else
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_error_1"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
                        
        }
    }
    else if ( item->getTag() == E_DUNGEON::D_FLASHMAN )
    {
        if ( FlashmanManager::getInstance()->getInfoSetting("minigame_switch")->getValueToBool() == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_error_2"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
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
            
        if ( bEnter == true )
        {
            auto popup = PopupFlashman::create();
            popup->show();
        }
        else
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_minigame_condition_1"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
                        
        }
    }
    else if ( item->getTag() == E_DUNGEON::D_PRISON_FROST )
    {
        if ( PrisonFrostManager::getInstance()->getInfoSetting("frost_prison_switch")->getValueToBool() == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_error_2"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
            
        if ( WeaponManager::getInstance()->getEquipHigh() >= E_WEAPON::WEAPON_60 )
        {
            auto popup = PopupFrost::create();
            popup->show();
        }
        else
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_error_1"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
                        
        }
    }
    else if ( item->getTag() == E_DUNGEON::D_DUNGEON_NORMAL )
    {
        if(_isSceneMove)
            return;
        
        //
        bool bPlay = PrisonManager::getInstance()->isDungeonPlay();
        int nEnterAutoCount = PrisonManager::getInstance()->getEnterAutoCount();
        if ( bPlay == true || nEnterAutoCount > 0 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_close_check"));
            popup->addButton(GAME_TEXT("t_ui_cancel"), true, nullptr);
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                
                item->setEnabled(false);
                UtilsScene::getInstance()->sceneReplace(UtilsScene::eType::scene_dungeon_normal);
                
                //
                _isSceneMove = true;
                
            });
            popup->show();
        }
        else
        {
            item->setEnabled(false);
            UtilsScene::getInstance()->sceneReplace(UtilsScene::eType::scene_dungeon_normal);
            
            //
            _isSceneMove = true;
        }
    }
    else if ( item->getTag() == E_DUNGEON::D_DUNGEON_DEVIL )
    {
        if ( UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) < DEVIL_JOIN_FLOOR )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_dungeon_devil"), GAME_TEXT("t_ui_error_28"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
        if(_isSceneMove)
            return;
        
        //
        SaveManager::saveAllData();
        
        //
        bool bPlay = PrisonManager::getInstance()->isDungeonPlay();
        int nEnterAutoCount = PrisonManager::getInstance()->getEnterAutoCount();
        if ( bPlay == true || nEnterAutoCount > 0 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_close_check"));
            popup->addButton(GAME_TEXT("t_ui_cancel"), true, nullptr);
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                
                item->setEnabled(false);
                UtilsScene::getInstance()->sceneReplace(UtilsScene::eType::scene_dungeon_devil);
                
                //
                _isSceneMove = true;
                
            });
            popup->show();
        }
        else
        {
            item->setEnabled(false);
            UtilsScene::getInstance()->sceneReplace(UtilsScene::eType::scene_dungeon_devil);
            
            //
            _isSceneMove = true;
        }
    }
    else if ( item->getTag() == E_DUNGEON::D_NEW_RAID )
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
    }
    else if (item->getTag() == E_DUNGEON::D_ADVENTURE )
    {
        bool bEnter = true;
        if ( WeaponManager::getInstance()->getEquipHigh() < E_WEAPON::WEAPON_76_MINE)
        {
            bEnter = false;
        }
        
        if ( bEnter == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_advt_info_title_1"), GAME_TEXT("t_ui_error_12"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
        auto popup = PopupAdventureGuild::create();
        popup->show();
    }
    else if ( item->getTag() == E_DUNGEON::D_DEFENSE )
    {
        bool bEnter = false;
        
        if (UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= 10000)
        {
            bEnter = true;
        }
        
        if ( bEnter == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_defense_title_1"), GAME_TEXT("t_ui_error_28"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
        if(DefenseManager::getInstance()->isOpenContentsNow())
        {
            if(DefenseManager::getInstance()->isBackground())
            {
                DefenseManager::getInstance()->setIsBackground(false);
            }
        }
        else
        {
            auto popup = PopupDefenseDefault::create();
            popup->show();
        }
    }
}
