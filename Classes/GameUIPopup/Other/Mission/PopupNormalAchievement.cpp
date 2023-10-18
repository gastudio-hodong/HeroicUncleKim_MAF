//
//  PopupNormalAchievement.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/29.
//

#include "PopupNormalAchievement.h"

#include "Common/ConfigGameNormal.h"

#include "GameScene/HelloWorldScene.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Common/MenuPopup.h"
#include "GameUIPopup/Other/Costume/PopupCostumeDefault.h"
#include "GameUIPopup/Other/Friend/PopupFriend.h"
#include "GameUIPopup/Other/Pass/DungeonPass/PopupDungeonPass.h"
#include "GameUIPopup/Other/Mission/PopupMissionDefault.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidDefault.h"
#include "GameUIPopup/Other/SubFunction/PopupSubFunction.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/TutorialManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ServerMissionManager.h"
#include "ManagerGame/AchievementManager.h"

USING_NS_CC;

static bool sortAchieve(ObjAchievement* a, ObjAchievement* b)
{
    bool result = false;

    if(a->isRewardCondition() && !b->isRewardCondition())
    {
        result = true;
    }
    else if(!a->isRewardCondition() && b->isRewardCondition())
    {
        result = false;
    }
    else
    {
        result = a->getIdx() < b->getIdx();
    }
    
    if(a->isComplete() && !b->isComplete())
    {
        result = false;
    }
    else if(!a->isComplete() && b->isComplete())
    {
        result = true;
    }
    else if(a->isComplete() && b->isComplete())
    {
        result = a->getIdx() < b->getIdx();
    }
    
    
    return result;
}

PopupNormalAchievement* PopupNormalAchievement::create()
{
    PopupNormalAchievement* pRet = new(std::nothrow) PopupNormalAchievement();
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
PopupNormalAchievement::PopupNormalAchievement():
_layerContainerContents(nullptr),
_layerContainerTopUI(nullptr),
_table(nullptr),
_itemRewardAll(nullptr),

_eType(E_ACHIEVE_NORMAR_TYPE::ACHIEVE_PET)
{

}

PopupNormalAchievement::~PopupNormalAchievement()
{
    _vecAchievement.clear();
}


#pragma mark - table
void PopupNormalAchievement::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{

}

Size PopupNormalAchievement::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    return Size(655, C_BAR_MISSION_SIZE);
}

extension::TableViewCell* PopupNormalAchievement::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
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

    auto acheive = _vecAchievement.at(idx);


    std::string strPathBackground   = "";
    std::string strPathRewardBox    = "";
    std::string strTextTop          = "";
    std::string strTextCenter       = "";
    std::string strTextBottom       = "";
    std::string strTitle            = "";

    Color3B colorTitle = Color3B(255,255,255);

    int maxCount = acheive->getGradeClearCount();
    int curCount = acheive->getGradeCurrentCount();


    InfoItems* objItems = acheive->getItem();

    strTitle            = GAME_TEXTFORMAT(acheive->getTextKey(), acheive->getGradeClearCount());
    strPathBackground   = "Assets/ui/mission/mission_normal_1.png";
    strPathRewardBox    = "Assets/ui/mission/task_box_special.png";

    strTextBottom = MafUtils::format("%s +%d",objItems->getText().c_str(), std::atoi(objItems->getCount().c_str()));

    colorTitle = Color3B(92,203,255);

    //
    auto sprBackground = ui::Scale9Sprite::create(strPathBackground);
    sprBackground->setPosition(size.width / 2, size.height / 2);
    sprBackground->setTag((int)idx);
    cell->addChild(sprBackground);

    auto lbTitle = Label::createWithTTF(strTitle, GAME_FONT, 30);//28로 맞출지는 보고 확인
    lbTitle->setAnchorPoint(Vec2(0,0));
    
    lbTitle->setPosition(Vec2(sprBackground->getContentSize().width*0.2, sprBackground->getContentSize().height*0.47) + Vec2::UP * 5);
    lbTitle->setDimensions(370, lbTitle->getContentSize().height);
    lbTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbTitle->setOverflow(Label::Overflow::SHRINK);
    lbTitle->setColor(colorTitle);
    
    sprBackground->addChild(lbTitle);

    auto lbTextTop = Label::createWithTTF(strTextTop, GAME_FONT, 20);
    lbTextTop->setAnchorPoint(Vec2(0,0));
    lbTextTop->setPosition(Vec2(sprBackground->getContentSize().width*0.2, sprBackground->getContentSize().height*0.74));
    lbTextTop->setDimensions(400, lbTextTop->getContentSize().height);
    lbTextTop->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbTextTop->setOverflow(Label::Overflow::SHRINK);
    sprBackground->addChild(lbTextTop);

    auto lbTextCenter = Label::createWithTTF(strTextBottom, GAME_FONT, 22);
    lbTextCenter->setAnchorPoint(Vec2(0,0));
    
    lbTextCenter->setPosition(sprBackground->getContentSize().width*0.2, sprBackground->getContentSize().height*0.32);
    lbTextCenter->setDimensions(200, lbTextCenter->getContentSize().height);
    lbTextCenter->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbTextCenter->setOverflow(Label::Overflow::SHRINK);
    sprBackground->addChild(lbTextCenter);

    auto lbTextBottom = Label::createWithTTF(strTextBottom, GAME_FONT, 19); //타이틀 내용은 비어있어야 한다.
    lbTextBottom->setColor(Color3B(167, 188, 208));
    lbTextBottom->setAnchorPoint(Vec2(0,0));
    lbTextBottom->setPosition(Vec2(sprBackground->getContentSize().width*0.2, sprBackground->getContentSize().height*0.1));
    lbTextBottom->setDimensions(400, lbTextBottom->getContentSize().height);
    lbTextBottom->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbTextBottom->setOverflow(Label::Overflow::SHRINK);
    sprBackground->addChild(lbTextBottom);

    Sprite* sprRewardBox = Sprite::create(strPathRewardBox);
    sprRewardBox->setPosition(Vec2(sprBackground->getContentSize().width*0.09, sprBackground->getContentSize().height*0.5));
    sprBackground->addChild(sprRewardBox);

    MafNode::MafMenu *menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    sprBackground->addChild(menu);

    auto itemReceive = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/mission/btn_shortcuts_1.png"), Sprite::create("Assets/ui/mission/btn_shortcuts_1.png"), Sprite::create("Assets/ui/mission/btn_shortcuts_1.png"), CC_CALLBACK_1(PopupNormalAchievement::onClick, this));
    itemReceive->setPosition(Vec2(sprBackground->getContentSize().width*0.86, sprBackground->getContentSize().height*0.5));
    itemReceive->setName("RECEIVE");
    itemReceive->setTag(acheive->getIdx());
    menu->addChild(itemReceive);

    Sprite* spReward = Sprite::create(objItems->getPath());
    spReward->setPosition(itemReceive->getContentSize()*0.5);
    sprRewardBox->addChild(spReward);
    
    spReward->setScale(87 / spReward->getContentSize().width);
    
    Sprite* sprBarBackground = Sprite::create("Assets/ui/prison/quest_gauge_bg.png");
    sprBarBackground->setScaleX(1.8f);
    sprBarBackground->setPosition(Vec2(sprBackground->getContentSize().width*0.45 - (sprBarBackground->getContentSize().width * sprBarBackground->getScaleX()/2),
                               sprBackground->getContentSize().height*0.15));
    sprBarBackground->setAnchorPoint(Vec2(0,0));
    
    sprBackground->addChild(sprBarBackground);

    Sprite *sprBar = Sprite::create("Assets/ui/prison/quest_gauge_c.png");
    sprBar->setAnchorPoint(Vec2(0,0.5));
    
    ProgressTimer *bar = ProgressTimer::create(sprBar);
    bar->setType(ProgressTimer::Type::BAR);
    bar->setPercentage(0);
    bar->setMidpoint(Vec2(0,0.5));
    bar->setBarChangeRate(Vec2(1,0));
    bar->setAnchorPoint(Vec2(0,0));
    sprBarBackground->addChild(bar);
    
    Label *lbProgress = Label::createWithTTF(strPathBackground, GAME_FONT, 20);
    lbProgress->setColor(Color3B::WHITE);
    lbProgress->setPosition(sprBarBackground->getPosition().x+sprBarBackground->getContentSize().width*sprBarBackground->getScaleX()/2,
                             sprBarBackground->getPosition().y+sprBarBackground->getContentSize().height/2);
    lbProgress->setAnchorPoint(Vec2(0.5,0.5));
    sprBackground->addChild(lbProgress);
    
    if(curCount > maxCount)
    {
        curCount = maxCount;
    }
    
    if (curCount == 0)
    {
        bar->setPercentage(0);
    }
    else
    {
        bar->setPercentage(((float)curCount/(float)maxCount)*100);
    }

    strTextBottom = "";
    lbTitle->disableEffect(LabelEffect::STRIKETHROUGH);
    
    sprBarBackground->setVisible(true);
    lbProgress->setVisible(true);
    lbTextBottom->setString(strTextBottom);
    lbProgress->setString(MafUtils::format("%d / %d", curCount,maxCount));
    
    
    if(acheive->isComplete())
    {
        lbTitle->enableStrikethrough();
        itemReceive->setName("RECEIVE_NORMAL");
        itemReceive->setNormalImage(Sprite::create("Assets/ui/mission/btn_acheive_2.png"));
        itemReceive->setDisabledImage(Sprite::create("Assets/ui/mission/btn_acheive_2.png"));
        itemReceive->setSelectedImage(Sprite::create("Assets/ui/mission/btn_acheive_2.png"));
        itemReceive->setVisible(false);
    }
    else
    {
        if(acheive->isRewardCondition())
        {
            itemReceive->setName("REWARD");
            
            itemReceive->setNormalImage(Sprite::create("Assets/ui/mission/btn_acheive_1.png"));
            itemReceive->setDisabledImage(Sprite::create("Assets/ui/mission/btn_acheive_1.png"));
            itemReceive->setSelectedImage(Sprite::create("Assets/ui/mission/btn_acheive_1.png"));
        }
    }
    
    return cell;
}

ssize_t PopupNormalAchievement::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return _vecAchievement.size();
}

#pragma mark -init

bool PopupNormalAchievement::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    _vecAchievement = AchievementManager::getInstance()->getAchievementNormalListFromType(_eType);
    //
    initVar();
    initUI();

    
    return true;
}

void PopupNormalAchievement::initVar()
{
    double nHeight = getContentSize().height;
    if ( nHeight > 1150 )
        nHeight = 1150;

    _spriteContainer->setContentSize(Size(729, nHeight));
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(0);
}

void PopupNormalAchievement::initUI()
{
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_achievements"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);

    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);

    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupNormalAchievement::onClick, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setName("CLOSE");
    menu->addChild(itemClose);
    
    auto itemBack = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_back1_1.png"), nullptr, CC_CALLBACK_1(PopupNormalAchievement::onClick, this));
    itemBack->setPosition( Vec2(itemBack->getContentSize().width*2, _spriteContainer->getContentSize().height - 37.5) );
    itemBack->setName("BACK");
    menu->addChild(itemBack);
    
    _layerContainerTopUI = Layer::create();
    _layerContainerTopUI->setContentSize(Size(_spriteContainer->getContentSize().width - 80, 130));
    
    _layerContainerTopUI->setIgnoreAnchorPointForPosition(false);
    _layerContainerTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    
    _layerContainerTopUI->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
    
    _spriteContainer->addChild(_layerContainerTopUI);;
    
    
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize(Size(_spriteContainer->getContentSize().width - 80, C_BAR_MISSION_SIZE * 6));
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    
    _layerContainerContents->setPosition(_layerContainerTopUI->getPosition() + Vec2::DOWN * (_layerContainerTopUI->getContentSize().height + 40));
    
    _spriteContainer->addChild(_layerContainerContents);
    

    initTopUI();
    initBottomUI();
    redrawPopupUI();
}

void PopupNormalAchievement::initTopUI()
{
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    menu->setAnchorPoint(Vec2::ZERO);
    _layerContainerTopUI->addChild(menu);
    
    std::vector<E_ACHIEVE_NORMAR_TYPE> list = {E_ACHIEVE_NORMAR_TYPE::ACHIEVE_PET,
        E_ACHIEVE_NORMAR_TYPE::ACHIEVE_COSTUME, E_ACHIEVE_NORMAR_TYPE::ACHIEVE_ARTIFACT,
        E_ACHIEVE_NORMAR_TYPE::ACHIEVE_BUILDING, E_ACHIEVE_NORMAR_TYPE::ACHIEVE_MYHOME,
        E_ACHIEVE_NORMAR_TYPE::ACHIEVE_NEW_RAID
    };
    
    for(int i = 0; i < list.size(); ++i)
    {
        E_ACHIEVE_NORMAR_TYPE type = (E_ACHIEVE_NORMAR_TYPE) list[i];
        
        auto sprCheckBoxOff = Sprite::create("Assets/ui/mission/achievements_icon_btn_off.png");
        
        auto sprCheckBoxOn = Sprite::create("Assets/ui/mission/achievements_icon_btn_on.png");
        
        auto itemCheckBox = MafNode::MafMenuItemSprite::create(sprCheckBoxOff, nullptr,sprCheckBoxOn, CC_CALLBACK_1(PopupNormalAchievement::onClick, this));
        itemCheckBox->setTag((int) type);
        itemCheckBox->setName("CHECK_BOX");
        itemCheckBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        itemCheckBox->setPosition(5 + itemCheckBox->getContentSize().width/2 + i * _layerContainerTopUI->getContentSize().width/(C_NORMAL_ACHIEVE_COUNT), _layerContainerTopUI->getContentSize().height/2);
        menu->addChild(itemCheckBox);
        _vecAchieveTab.pushBack(itemCheckBox);
        
        if(AchievementManager::getInstance()->isRewardCondition(type))
        {
            auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_RIGHT);
            uiReddot->setPosition(itemCheckBox->getContentSize().width, itemCheckBox->getContentSize().height - 5);
            itemCheckBox->addChild(uiReddot);
        }
        
        std::string strIconPath = "";
        
        switch (type)
        {
            case E_ACHIEVE_NORMAR_TYPE::ACHIEVE_PET: strIconPath = "Assets/ui/menu_bottom/menu_btn_pet.png"; break;
            case E_ACHIEVE_NORMAR_TYPE::ACHIEVE_COSTUME: strIconPath = "Assets/ui/menu_bottom/menu_btn_costume.png"; break;
            case E_ACHIEVE_NORMAR_TYPE::ACHIEVE_ARTIFACT: strIconPath = "Assets/ui/menu_bottom/menu_btn_treasure.png"; break;
            case E_ACHIEVE_NORMAR_TYPE::ACHIEVE_BUILDING: strIconPath = "Assets/ui/menu_bottom/menu_btn_tower.png"; break;
            case E_ACHIEVE_NORMAR_TYPE::ACHIEVE_MYHOME: strIconPath = "Assets/ui/menu_bottom/menu_btn_home.png"; break;
            case E_ACHIEVE_NORMAR_TYPE::ACHIEVE_NEW_RAID: strIconPath = "Assets/ui/menu_bottom/menu_btn_raid_3.png"; break;
        }
        
        auto sprIcon = Sprite::create(strIconPath);
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprIcon->setPosition(itemCheckBox->getContentSize()/2);
        itemCheckBox->addChild(sprIcon);
    }
    auto item = getCheckBox(E_ACHIEVE_NORMAR_TYPE::ACHIEVE_PET);
    
    if(item != nullptr)
    {
        item->setEnabled(false);
    }
    
}

void PopupNormalAchievement::initBottomUI()
{
    
    _table = extension::TableView::create(this, Size(_layerContainerContents->getContentSize().width, _layerContainerContents->getContentSize().height));
    _table->setDirection(extension::ScrollView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition( Vec2(_layerContainerContents->getContentSize().width / 2, 0));
    _table->setDelegate(this);
    _layerContainerContents->addChild(_table);
    
    _table->reloadData();
    _table->setContentOffset(_table->minContainerOffset());
    
    Sprite *sprInfo = Sprite::create("Assets/ui/mission/mission_list.png");
    sprInfo->setAnchorPoint(Vec2::Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprInfo->setPosition(_spriteContainer->getContentSize().width/2, _layerContainerContents->getPositionY() + 10);
    _spriteContainer->addChild(sprInfo);
    {
        auto labelTitle1 = Label::createWithTTF(GAME_TEXT("t_ui_mission_title"), GAME_FONT, 24);
        labelTitle1->setPosition( Vec2(210, sprInfo->getContentSize().height / 2) );
        sprInfo->addChild(labelTitle1);
        
        auto labelTitle2 = Label::createWithTTF(GAME_TEXT("t_ui_reward"), GAME_FONT, 24);
        labelTitle2->setPosition( Vec2(565, sprInfo->getContentSize().height / 2) );
        sprInfo->addChild(labelTitle2);
    }
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    menu->setAnchorPoint(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    auto sprRewardAll = ui::Scale9Sprite::create("Assets/ui_common/btn_1_2_on.png");
    sprRewardAll->setContentSize(Size(716, 96));
    
    //
    _itemRewardAll = MafNode::MafMenuItemSprite::create(sprRewardAll, nullptr, CC_CALLBACK_1(PopupNormalAchievement::onClick, this));
    _itemRewardAll->setName("REWARD_ALL");
    _itemRewardAll->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _itemRewardAll->setPosition(_spriteContainer->getContentSize().width/2, 30);
    menu->addChild(_itemRewardAll);
    {
        auto lbContent = Label::createWithTTF(GAME_TEXT("t_ui_attend_receive_all"), GAME_FONT, 30);
        lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbContent->setDimensions(_itemRewardAll->getContentSize().width - 10, _itemRewardAll->getContentSize().height/2);
        lbContent->setOverflow(Label::Overflow::SHRINK);
        lbContent->setPosition(_itemRewardAll->getContentSize()/2);
        lbContent->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        _itemRewardAll->addChild(lbContent);
    }
    
    if(AchievementManager::getInstance()->isRewardCondition(_eType))
    {
        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_RIGHT);
        uiReddot->setPosition(_itemRewardAll->getContentSize().width + 10, _itemRewardAll->getContentSize().height + 10);
        _itemRewardAll->addChild(uiReddot);
    }
}

#pragma mark -ui

void PopupNormalAchievement::redrawPopupUI()
{
    auto reddot = (UIReddot*)_itemRewardAll->getChildByName("UIReddot");
    
    if(reddot != nullptr)
    {
        if(!AchievementManager::getInstance()->isRewardCondition(_eType))
        {
            reddot->hide();
        }
    }
    else
    {
        if(AchievementManager::getInstance()->isRewardCondition(_eType))
        {
            auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_RIGHT);
            uiReddot->setPosition(_itemRewardAll->getContentSize().width + 10, _itemRewardAll->getContentSize().height + 10);
            _itemRewardAll->addChild(uiReddot);
        }
    }
    
    for(auto itemTab : _vecAchieveTab)
    {
        auto reddot = (UIReddot*)itemTab->getChildByName("UIReddot");
        
        if(AchievementManager::getInstance()->isRewardCondition((E_ACHIEVE_NORMAR_TYPE)itemTab->getTag()))
        {
            if(reddot == nullptr)
            {
                auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_RIGHT);
                uiReddot->setPosition(itemTab->getContentSize().width, itemTab->getContentSize().height - 5);
                itemTab->addChild(uiReddot);
            }
        }
        else
        {
            if(reddot != nullptr)
            {
                reddot->hide();
            }
        }
    }
    
    std::sort(_vecAchievement.begin(), _vecAchievement.end(), sortAchieve);
    _table->reloadData();
}

#pragma mark -utils
void PopupNormalAchievement::onClick(cocos2d::Ref* sender)
{
    auto item = (Node*)sender;
    
    if(item->getName().compare("CLOSE") == 0)
    {
        hide();
    }
    else if(item->getName().compare("REWARD") == 0)
    {
        onClickReward(item->getTag());
    }
    else if(item->getName().compare("REWARD_ALL") == 0)
    {
        onClickRewardAll();
    }
    else if(item->getName().compare("CHECK_BOX") == 0)
    {
        onClickTab(item->getTag());
    }
    else if(item->getName().compare("RECEIVE") == 0)
    {
        goThroughShortcut(item->getTag());
    }
    else if(item->getName().compare("BACK") == 0)
    {
        onClickBack();
    }
    
}

void PopupNormalAchievement::onClickReward(int idx)
{
    auto achieveMgr = AchievementManager::getInstance();
    auto achieve = achieveMgr->getObjAchieveNormal(idx);
    
    if(achieve == nullptr)
        return;
    
    achieveMgr->setCallbackRedraw(CC_CALLBACK_0(PopupNormalAchievement::callbackReward, this));
    achieveMgr->achievementNormalReward(achieve);
    
}
void PopupNormalAchievement::onClickRewardAll()
{
    PopupLoading::show();
    auto reddot = (UIReddot*)_itemRewardAll->getChildByName("UIReddot");
    
    if(reddot != nullptr)
    {
        reddot->hide();
    }
    
    auto achieveMgr = AchievementManager::getInstance();

    achieveMgr->setCallbackRedraw(CC_CALLBACK_0(PopupNormalAchievement::callbackReward, this));

    achieveMgr->achievementNormalRewardAll(_eType);
}
void PopupNormalAchievement::onClickTab(int idx)
{
    _eType = (E_ACHIEVE_NORMAR_TYPE)idx;
    auto item = getCheckBox(_eType);
    
    if(item == nullptr)
        return;
    
    for(auto obj : _vecAchieveTab)
    {
        obj->setEnabled(true);
    }
    item->setEnabled(false);
    
    _vecAchievement = AchievementManager::getInstance()->getAchievementNormalListFromType(_eType);
    
    redrawPopupUI();
    
}

void PopupNormalAchievement::onClickBack()
{
    E_MISSION eType = E_MISSION::SPECIAL;
    if ( ServerMissionManager::isServerMissionOpenCondition() )
    {
        if(!ServerMissionManager::getInstance()->isServerMissionCompleteDaily() && ServerMissionManager::getInstance()->isServerMissionCompleteWeekly())
            eType = E_MISSION::WEEK;
        else
            eType = E_MISSION::DAILY;
    }
    
    //
    auto popup = PopupMissionDefault::create(eType);
    popup->show();
    
    hide();
}

void PopupNormalAchievement::goThroughShortcut(int idx)
{
    auto achieveMgr = AchievementManager::getInstance();
    auto achieve = achieveMgr->getObjAchieveNormal(idx);
    auto shortcut = achieve->getShortcuts();
    auto tutoMgr = TutorialManager::getInstance();
    std::string strMessage = "";
    switch (shortcut)
    {
        case SHORTCUT_NONE:
            hide();
            break;
            
        case SHORTCUT_QUEST:
        {
            hide();
             
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
            hide();
            
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
            
            if(tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 35))
            {
                auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
                if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
                {
                    MafNode::MafMenuItemSprite* item = (MafNode::MafMenuItemSprite*)dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_menu_btn->getChildByName("PET");
                    item->setName("PET");
                    dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onButtonTouched(item);
                }
            }
            else
            {
                strMessage = GAME_TEXT("t_ui_mission_special_title");
                strMessage += " " + MafUtils::toString(35);
                PopupToast::showImmediately(strMessage, getContentSize().width/2, getContentSize().height - 200);
            }
            hide();
            break;
        }
        case SHORTCUT_ARTIFACT_NORMAL:
        {
            
            if(tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 34))
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
            }
            else
            {
                strMessage = GAME_TEXT("t_ui_mission_special_title");
                strMessage += " " + MafUtils::toString(34);
                PopupToast::showImmediately(strMessage, getContentSize().width/2, getContentSize().height - 200);
            }
            hide();
            break;
        }
        case SHORTCUT_ARTIFACT_SPECIAL:
        {
            
            if(tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 34))
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
            }
            else
            {
                strMessage = GAME_TEXT("t_ui_mission_special_title");
                strMessage += " " + MafUtils::toString(34);
                PopupToast::showImmediately(strMessage, getContentSize().width/2, getContentSize().height - 200);
            }
            hide();
            break;
        }
        case SHORTCUT_RELIC:
        {
            hide();
                    
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
            hide();

            auto popup = PopupSubFunction::create(PopupSubFunction::E_TYPE::SHOPITEM);
            popup->show();
            break;
        }
        case SHORTCUT_PRESTIGE:
        {
            hide();
                    
            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onClickoRevive();
            }
            break;
        }
        case SHORTCUT_MINE:
        {
            
            if(tutoMgr->getOpenData("WEAPON_76"))
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
            hide();
            break;
        }
        case SHORTCUT_BUILDING:
        {
            if(tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 34))
            {
                auto popup = PopupSubFunction::create(PopupSubFunction::E_TYPE::TOWN);
                popup->show();
            }
            else
            {
                strMessage = GAME_TEXT("t_ui_mission_special_title");
                strMessage += " " + MafUtils::toString(34);
                PopupToast::showImmediately(strMessage, getContentSize().width/2, getContentSize().height - 200);
            }
            hide();
            break;
        }
        case SHORTCUT_MYHOME:
        {
            hide();
                    
            auto popup = PopupSubFunction::create(PopupSubFunction::E_TYPE::MYHOME);
            popup->show();
            break;
        }
        case SHORTCUT_MENU:
        {
            hide();
                    
            auto popup = MenuPopup::create();
            popup->show();
            break;
        }
        case SHORTCUT_COSTUME:
        {
            
            if(tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 38))
            {
                if ( AccountManager::getInstance()->getUserIdx() == 0 )
                {
                    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
                    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                    popup->show();
                    return;
                }
                
                //
                auto popup = PopupCostumeDefault::create();
                popup->show();
                
            }
            else
            {
                strMessage = GAME_TEXT("t_ui_mission_special_title");
                strMessage += " " + MafUtils::toString(38);
                
                PopupToast::showImmediately(strMessage, getContentSize().width/2, getContentSize().height - 200);
            }
            hide();
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
            
            //
            hide();
            
            auto popup = PopupFriend::create();
            popup->show();
            break;
        }

        default:
            break;
    }
}

MafNode::MafMenuItemSprite* PopupNormalAchievement::getCheckBox(E_ACHIEVE_NORMAR_TYPE type)
{
    MafNode::MafMenuItemSprite* result = nullptr;
    
    for(auto item : _vecAchieveTab)
    {
        if((E_ACHIEVE_NORMAR_TYPE)item->getTag() == type)
        {
            result = item;
            break;
        }
    }
    
    return result;
}

#pragma mark -callback
void PopupNormalAchievement::callbackReward()
{
    if(PopupLoading::isShow())
        PopupLoading::hide();
    
    _vecAchievement = AchievementManager::getInstance()->getAchievementNormalListFromType(_eType);
    
    redrawPopupUI();
}
