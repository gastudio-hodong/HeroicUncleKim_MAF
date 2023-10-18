//
//  PopupPrisonAchievement.cpp
//  FantasyClicker-mobile
//
//  Created by TK on 2020/06/03.
//

#include "PopupPrisonAchievement.h"

#include "Common/ConfigGameNormal.h"

#include "ManagerGame/AchievementManager.h"

USING_NS_CC;

#pragma mark - set, get, other
static bool sortAchieve(ObjAchievement* a, ObjAchievement* b)
{
    bool bSort = a->isComplete() < b->isComplete();
    if ( a->isComplete() == false )
    {
        if ( a->getCount() >= a->getCountMax() )
        {
            bSort = true;
        }
    }
    return bSort;
    
}

PopupPrisonAchievement* PopupPrisonAchievement::create()
{
    PopupPrisonAchievement* pRet = new(std::nothrow) PopupPrisonAchievement();
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

PopupPrisonAchievement::PopupPrisonAchievement(void) :
_onCallbackReward(nullptr),

_layerContainerProgress(nullptr),
_layerContainerList(nullptr),
_table(nullptr)
{
    
}

PopupPrisonAchievement::~PopupPrisonAchievement(void)
{
    
}

bool PopupPrisonAchievement::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
     
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - override : table
void PopupPrisonAchievement::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupPrisonAchievement::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, C_BAR_MISSION_SIZE);
}

extension::TableViewCell* PopupPrisonAchievement::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    
    // obj
    auto obj = _listAchievement.at(idx);
    auto objReward = obj->getReward();
    
    //
    auto spriteBg = ui::Scale9Sprite::create("Assets/ui/mission/mission_special_2.png", Rect::ZERO, Rect(20,50,2,2));
    spriteBg->setContentSize(Size(size.width, size.height*0.95));
    spriteBg->setPosition(size.width / 2, size.height / 2);
    cell->addChild(spriteBg);
    
    //
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    spriteBg->addChild(menu);
    
    //
    auto spriteRewardBG = Sprite::create("Assets/ui/mission/task_box_special.png");
    spriteRewardBG->setPosition(spriteBg->getContentSize().width*0.09, spriteBg->getContentSize().height / 2);
    spriteBg->addChild(spriteRewardBG);
    if ( objReward != nullptr )
    {
        auto spriteRewardIcon = Sprite::create(objReward->getPath());
        spriteRewardIcon->setPosition(spriteRewardBG->getContentSize().width / 2, spriteRewardBG->getContentSize().height / 2 + 10);
        spriteRewardIcon->setScale(1.5);
        spriteRewardIcon->getTexture()->setTexParameters(texParams);
        spriteRewardBG->addChild(spriteRewardIcon);
        
        auto labelCount = Label::createWithTTF(MafUtils::format("%s", objReward->getCount().c_str()), GAME_FONT, 24);
        labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        labelCount->setPosition(spriteRewardBG->getContentSize().width / 2, 5);
        labelCount->setColor(COLOR_COIN);
        labelCount->enableBold();
        spriteRewardBG->addChild(labelCount);
    }
    
    auto labelTitle = Label::createWithTTF(obj->getText(), GAME_FONT, 28);
    labelTitle->setAnchorPoint(Vec2(0,0));
    labelTitle->setPosition(spriteBg->getContentSize().width*0.2, spriteBg->getContentSize().height * 0.47);
    labelTitle->setColor(Color3B(92, 203, 255));
    labelTitle->setDimensions(400, labelTitle->getContentSize().height);
    labelTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    labelTitle->setOverflow(Label::Overflow::SHRINK);
    spriteBg->addChild(labelTitle);
    if ( obj->isComplete() == true )
    {
        labelTitle->enableStrikethrough();
    }

    auto spriteProgressBG = ui::Scale9Sprite::create("Assets/ui/prison/quest_gauge_bg.png", Rect::ZERO, Rect(0,0,1,1));
    spriteProgressBG->setContentSize(Size(330, spriteProgressBG->getContentSize().height));
    spriteProgressBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    spriteProgressBG->setPosition(spriteBg->getContentSize().width*0.2, 10);
    spriteBg->addChild(spriteProgressBG);
    {
        int nRate = (double)obj->getCount() / obj->getCountMax() * 100;
        if ( nRate > 100 )
        {
            nRate = 100;
        }
        
        auto progressRate = ProgressTimer::create(Sprite::create("Assets/ui/prison/quest_gauge_c.png"));
        progressRate->setPosition(spriteProgressBG->getContentSize().width / 2, spriteProgressBG->getContentSize().height / 2);
        progressRate->setType(ProgressTimer::Type::BAR);
        progressRate->setMidpoint( Vec2::ANCHOR_MIDDLE_LEFT );
        progressRate->setBarChangeRate( Vec2(1,0) );
        progressRate->setPercentage(nRate);
        progressRate->setScaleX(1.8);
        spriteProgressBG->addChild(progressRate);

        auto labelProgress = Label::createWithTTF(MafUtils::format("%d / %d", obj->getCount(), obj->getCountMax()), GAME_FONT, 20);
        labelProgress->setPosition(spriteProgressBG->getContentSize().width / 2, spriteProgressBG->getContentSize().height / 2);
        spriteProgressBG->addChild(labelProgress);
    }
    
    
    auto itemReceive = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/mission/btn_acheive_1.png"), Sprite::create("Assets/ui/mission/btn_acheive_1.png"), nullptr, CC_CALLBACK_1(PopupPrisonAchievement::onClickReward, this));
    itemReceive->setPosition(spriteBg->getContentSize().width * 0.86, spriteBg->getContentSize().height / 2);
    itemReceive->setTag((int)idx);
    itemReceive->setVisible(false);
    menu->addChild(itemReceive);
    if ( obj->isComplete() == false && obj->getCount() >= obj->getCountMax() )
    {
        itemReceive->setVisible(true);
    }
    
    return cell;
}

ssize_t PopupPrisonAchievement::numberOfCellsInTableView(extension::TableView *table)
{
    return _listAchievement.size();
}

#pragma mark - init
void PopupPrisonAchievement::initVar()
{
    _listAchievement = AchievementManager::getInstance()->getList(0);
    
    std::sort(_listAchievement.begin(), _listAchievement.end(), sortAchieve);
}

void PopupPrisonAchievement::initUi()
{
    
    Size size = Size(732, 962);

    //
    _spriteContainer->setContentSize( size );
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(100);
    
    //
    _layerContainerProgress = Layer::create();
    _layerContainerProgress->setContentSize( Size(size.width, 150) );
    _layerContainerProgress->setIgnoreAnchorPointForPosition(false);
    _layerContainerProgress->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerProgress->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_layerContainerProgress);
    
    //
    _layerContainerList = Layer::create();
    _layerContainerList->setContentSize( Size(size.width, _spriteContainer->getContentSize().height - 75 - _layerContainerProgress->getContentSize().height) );
    _layerContainerList->setIgnoreAnchorPointForPosition(false);
    _layerContainerList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerList->setPosition(_spriteContainer->getContentSize().width / 2, 0);
    _spriteContainer->addChild(_layerContainerList);
    
    
    //
    auto spriteTitle = Sprite::create("Assets/ui/prison/achievements_title.png");
    spriteTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    spriteTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 10);
    _spriteContainer->addChild(spriteTitle);
    
    
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
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupPrisonAchievement::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    uiProgress();
    uiList();
    
    //
    drawProgress();
    drawList();
}


 
#pragma mark - ui
void PopupPrisonAchievement::uiProgress()
{
    _layerContainerProgress->removeAllChildren();
    
    auto labelRate = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_achievements_msg_1", 0), GAME_FONT, 28);
    labelRate->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    labelRate->setPosition(25, 80);
    labelRate->setName("LABEL");
    _layerContainerProgress->addChild(labelRate);
    
    auto spriteRateBG = Sprite::create("Assets/ui/prison/achievements_progress_bar1_1.png");
    spriteRateBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    spriteRateBG->setPosition(25, 80);
    _layerContainerProgress->addChild(spriteRateBG);
    
    auto progressRate = ProgressTimer::create(Sprite::create("Assets/ui/prison/achievements_progress_bar1_2.png"));
    progressRate->setType(ProgressTimer::Type::BAR);
    progressRate->setPercentage(0);
    progressRate->setMidpoint( Vec2::ANCHOR_MIDDLE_LEFT );
    progressRate->setBarChangeRate( Vec2(1,0) );
    progressRate->setName("PROGRESS");
    progressRate->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    progressRate->setPosition(25, 80);
    _layerContainerProgress->addChild(progressRate);

    //
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    _layerContainerProgress->addChild(menu);

    auto itemComplete = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/prison/icon_reward1_1.png"), Sprite::create("Assets/ui/prison/icon_reward1_1.png"), nullptr, CC_CALLBACK_1(PopupPrisonAchievement::onClickComplete, this));
    itemComplete->setPosition(_layerContainerProgress->getContentSize().width - 50, 80);
    menu->addChild(itemComplete);
}

void PopupPrisonAchievement::uiList()
{
    _layerContainerList->removeAllChildren();
    
    _table = extension::TableView::create(this, Size(655, _layerContainerList->getContentSize().height));
    _table->setDirection(extension::ScrollView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition( Vec2(_layerContainerList->getContentSize().width / 2, 0) );
    _table->setDelegate(this);
    _layerContainerList->addChild(_table);
}

#pragma mark - draw
void PopupPrisonAchievement::drawProgress()
{
    int nRate = AchievementManager::getInstance()->getCompleteRate(0);
    
    auto label = (Label*)_layerContainerProgress->getChildByName("LABEL");
    label->setString(GAME_TEXTFORMAT("t_ui_achievements_msg_1", nRate));
    
    auto progress = (ProgressTimer*)_layerContainerProgress->getChildByName("PROGRESS");
    progress->setPercentage(nRate);
}

void PopupPrisonAchievement::drawList()
{
    Vec2 pos = _table->getContentOffset();
    _table->reloadData();
    _table->setContentOffset(pos);
}

#pragma mark - set, get, other

void PopupPrisonAchievement::setCallbackReward(const std::function<void(void)>& callback)
{
    _onCallbackReward = callback;
}

#pragma mark - callback
void PopupPrisonAchievement::callbackReward(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if ( bResult == false || nResult != 1 )
    {
        std::string strError = GAME_TEXT("t_ui_error_30");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strError);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    drawProgress();
    drawList();
    
    if ( _onCallbackReward != nullptr )
    {
        _onCallbackReward();
    }
}

void PopupPrisonAchievement::callbackComplete(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if ( bResult == false || nResult != 1 )
    {
        std::string strError = GAME_TEXT("t_ui_error_30");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strError);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    drawProgress();
    drawList();
}

#pragma mark - click
void PopupPrisonAchievement::onClickClose(Ref* sender)
{
    hide();
}

void PopupPrisonAchievement::onClickReward(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    
    auto objAchievement = _listAchievement.at(item->getTag());
    
    if ( objAchievement->getCount() >= objAchievement->getCountMax() )
    {
        PopupLoading::show();
        AchievementManager::getInstance()->requestReward(objAchievement->getType(), objAchievement->getIdx(), CC_CALLBACK_2(PopupPrisonAchievement::callbackReward, this));
    }
}

void PopupPrisonAchievement::onClickComplete(cocos2d::Ref* sender)
{
    int nCompleteRate = AchievementManager::getInstance()->getCompleteRate(0);
    bool bCompleteReward = AchievementManager::getInstance()->isCompleteReward(0);
    if ( nCompleteRate < 100 )
    {
        return;
    }
    
    if ( bCompleteReward == true )
    {
        return;
    }
    
    PopupLoading::show();
    AchievementManager::getInstance()->requestComplete(0, CC_CALLBACK_2(PopupPrisonAchievement::callbackComplete, this));
}
