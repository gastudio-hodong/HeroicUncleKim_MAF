//
//  PopupFishing.cpp
//  FantasyClicker-mobile
//
//  Created by TG on 2020/07/08.
//

#include "PopupFishing.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoQuest.h"
#include "GameObject/InfoItems.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/OtherEvent/EventFishingBoardGame/PopupFishingReward.h"
#include "GameUIPopup/OtherEvent/EventFishingBoardGame/PopupBaitCharge.h"
#include "GameUIPopup/OtherEvent/EventFishingBoardGame/PopupFishingBoardGame.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerEvent/EventFishingManager.h"

USING_NS_CC;

PopupFishing* PopupFishing::create()
{
    PopupFishing* pRet = new(std::nothrow) PopupFishing();
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

PopupFishing::PopupFishing(void) :

_layerContainerFishing(nullptr),
_layerContainerBait(nullptr),
_layerContainerQuest(nullptr),
_layerContainerBoard(nullptr),

_spriteFishing(nullptr),
_labelBaitCount(nullptr),
_labelBaitBuy(nullptr),

_table(nullptr)
{
    
}

PopupFishing::~PopupFishing(void)
{
    
}

bool PopupFishing::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    initVar();
    initUI();
    
    //
    RefreshManager::getInstance()->addUI(E_REFRESH::CURRENCY, this);

    /*
     // story
     // 6.1.43 버전부터 스토리 삭제
     StoryManager::getInstance()->showStory((int)E_COMIC::EVENT_SUMMER_FISHING);
     */
    
    return true;
    
}

void PopupFishing::onEnter()
{
    PopupBase::onEnter();
    
    PopupLoading::show();
    EventFishingManager::getInstance()->requestInfo(CC_CALLBACK_2(PopupFishing::callbackInfo, this));
}

#pragma mark - override : table
void PopupFishing::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
 
}

void PopupFishing::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupFishing::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, C_BAR_SIZE_RAID_QUEST);
}

extension::TableViewCell* PopupFishing::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    auto obj = (InfoQuest*)_listQuest.at(idx);

    //
    auto spriteBg = ui::Scale9Sprite::create("Assets/ui/prison/quest_bar.png", Rect::ZERO, Rect(5,5,2,2));
    spriteBg->setContentSize(Size(size.width, size.height*0.95));
    spriteBg->setPosition(size.width / 2, size.height / 2);
    cell->addChild(spriteBg);
    
    auto labelTitle = Label::createWithTTF(obj->getTitle(), GAME_FONT, 24);
    labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    labelTitle->setPosition(0, spriteBg->getContentSize().height / 2);
    labelTitle->setDimensions(230, labelTitle->getContentSize().height);
    labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelTitle->setOverflow(Label::Overflow::SHRINK);
    spriteBg->addChild(labelTitle);
    
    //
    auto labelText = Label::createWithTTF(obj->getDesc(), GAME_FONT, 20);
    labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    labelText->setPosition(labelTitle->getContentSize().width + 100, spriteBg->getContentSize().height / 2);
    labelText->setDimensions(200, labelTitle->getContentSize().height);
    labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelText->setOverflow(Label::Overflow::SHRINK);
    spriteBg->addChild(labelText);
    
    auto spriteProgressBG = Sprite::create("Assets/ui/prison/quest_gauge_bg.png");
    spriteProgressBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    spriteProgressBG->setPosition(labelTitle->getContentSize().width + 100, spriteBg->getContentSize().height / 2);
    spriteBg->addChild(spriteProgressBG);
    {
        double nPercentage = (double)obj->getCountNow() / obj->getCountMax();
        nPercentage*=100;
        if ( nPercentage > 100 )
            nPercentage = 100;
        
        std::string strPathProgress = "";
        if ( obj->getType() == 1 )     strPathProgress = "Assets/ui/prison/quest_gauge_b.png";
        else                        strPathProgress = "Assets/ui/prison/quest_gauge_b.png";

        auto progress = ProgressTimer::create(Sprite::create(strPathProgress));
        progress->setAnchorPoint(Vec2::ZERO);
        progress->setPosition(Vec2::ZERO);
        progress->setType(ProgressTimer::Type::BAR);
        progress->setMidpoint(Vec2(0,0.5));
        progress->setBarChangeRate(Vec2(1,0));
        progress->setPercentage(nPercentage);
        spriteProgressBG->addChild(progress);
        
        std::string strText = "";
        if ( obj->isReceive() )
        {
            strText.append(GAME_TEXT("t_ui_complete"));
        }
        else
        {
            strText.append(MafUtils::toString(obj->getCountNow()));
            strText.append(" / ");
            strText.append(MafUtils::toString(obj->getCountMax()));
        }
        
        auto labelText = Label::createWithTTF(strText, GAME_FONT, 20);
        labelText->setPosition(spriteProgressBG->getContentSize().width / 2,spriteProgressBG->getContentSize().height / 2);
        spriteProgressBG->addChild(labelText);
    }
    
    //
    auto listReward = obj->getListReward();
    
    auto layerItems = Layer::create();
    layerItems->setContentSize(Size(120 * listReward.size(), spriteBg->getContentSize().height));
    layerItems->setIgnoreAnchorPointForPosition(false);
    layerItems->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layerItems->setPosition(600, spriteBg->getContentSize().height / 2);
    spriteBg->addChild(layerItems);
    for ( int i = 0; i < listReward.size(); i++ )
    {
        auto objItems = listReward.at(i);
        auto spriteItemsIcon = Sprite::create(objItems->getPath());
        if ( spriteItemsIcon == nullptr )
        {
            continue;
        }
        spriteItemsIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        spriteItemsIcon->setPosition(i * 120, layerItems->getContentSize().height / 2);
        layerItems->addChild(spriteItemsIcon);
        
        std::string strItemsCount = objItems->getCount();
        if ( objItems->isString() == true )
        {
            MafUtils::convertNumberToShort(strItemsCount);
        }
        
        Color3B colorItems = Color3B::WHITE;
        if ( objItems->getIdx() == 2 )          colorItems = COLOR_KEY;
        else if ( objItems->getIdx() == 4 )     colorItems = COLOR_COIN;
        else if ( objItems->getIdx() == 7 )     colorItems = COLOR_POINT;
        else if ( objItems->getIdx() == 5 )     colorItems = COLOR_MATERIAL;
        
        auto labelItemsCount = Label::createWithTTF(strItemsCount, GAME_FONT, 20);
        labelItemsCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelItemsCount->setPosition(i * 120 + 50, layerItems->getContentSize().height / 2);
        labelItemsCount->setColor(colorItems);
        layerItems->addChild(labelItemsCount);
        
        if ( obj->isReceive() )
        {
            spriteItemsIcon->setOpacity(100);
            labelItemsCount->setOpacity(100);
        }
    }

    if ( obj->isReceive() )
    {
        auto spriteComplete = Sprite::create("Assets/ui/prison/quest_complete.png");
        spriteComplete->setPosition(layerItems->getContentSize().width / 2, layerItems->getContentSize().height / 2);
        layerItems->addChild(spriteComplete);
    }
    
    return cell;
}

ssize_t PopupFishing::numberOfCellsInTableView(extension::TableView *table)
{
    return _listQuest.size();
}

#pragma mark - override : refresh
void PopupFishing::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::CURRENCY )
    {
        int nPointCount = EventFishingManager::getInstance()->getPointCount();
        _labelPointcount->setString(MafUtils::toString(nPointCount));
    }
}

#pragma mark - ui
void PopupFishing::initVar()
{
    
}

void PopupFishing::initUI()
{
    Size size = Size(750, 1302);
    
    _spriteContainer->setContentSize(size);
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(0);
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_fish_logo"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupFishing::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    _layerContainerFishing = Layer::create();
    _layerContainerFishing->setContentSize( Size(_spriteContainer->getContentSize().width, 676) );
    _layerContainerFishing->setIgnoreAnchorPointForPosition(false);
    _layerContainerFishing->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerFishing->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_layerContainerFishing);
    
    _layerContainerBait = Layer::create();
    _layerContainerBait->setContentSize( Size(_spriteContainer->getContentSize().width, 156) );
    _layerContainerBait->setIgnoreAnchorPointForPosition(false);
    _layerContainerBait->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerBait->setPosition(_spriteContainer->getContentSize().width / 2, _layerContainerFishing->getPositionY() - _layerContainerFishing->getContentSize().height);
    _spriteContainer->addChild(_layerContainerBait);
    
    _layerContainerBoard = Layer::create();
    _layerContainerBoard->setContentSize( Size(_spriteContainer->getContentSize().width, 111) );
    _layerContainerBoard->setIgnoreAnchorPointForPosition(false);
    _layerContainerBoard->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerBoard->setPosition(_spriteContainer->getContentSize().width / 2, 0);
    _spriteContainer->addChild(_layerContainerBoard);
    
    _layerContainerQuest = Layer::create();
    _layerContainerQuest->setContentSize( Size(_spriteContainer->getContentSize().width, _spriteContainer->getContentSize().height - 75 - _layerContainerFishing->getContentSize().height - _layerContainerBait->getContentSize().height - _layerContainerBoard->getContentSize().height) );
    _layerContainerQuest->setIgnoreAnchorPointForPosition(false);
    _layerContainerQuest->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerQuest->setPosition(_spriteContainer->getContentSize().width / 2, _layerContainerBoard->getContentSize().height);
    _spriteContainer->addChild(_layerContainerQuest);
    
    
    //
    uiFishing();
    uiBait();
    uiQuest();
    uiBoard();
}

#pragma mark - ui
void PopupFishing::uiFishing()
{
    _layerContainerFishing->removeAllChildren();
    
    _spriteFishing = Sprite::create("Assets/ui/event/event_bg_1_1.png");
    _spriteFishing->setPosition(_layerContainerFishing->getContentSize().width / 2, _layerContainerFishing->getContentSize().height / 2);
    _spriteFishing->getTexture()->setTexParameters(texParams);
    _spriteFishing->setScale(4);
    _layerContainerFishing->addChild(_spriteFishing);
    
    //
    {
        auto menu = MafNode::MafMenu::create();
        menu->setPosition(_layerContainerFishing->getContentSize().width / 2, 150);
        _layerContainerFishing->addChild(menu);
        
        //
        auto itemFishingX1 = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/event/event_btn_01.png"), nullptr, CC_CALLBACK_1(PopupFishing::onClickFishing, this));
        itemFishingX1->setTag(1);
        menu->addChild(itemFishingX1);
        {
            auto lbBtn = Label::createWithTTF(GAME_TEXT("t_ui_event_fish_fishing_btn_1"), GAME_FONT, 30);
            lbBtn->setPosition(itemFishingX1->getContentSize().width/2, itemFishingX1->getContentSize().height / 2);
            lbBtn->setDimensions(itemFishingX1->getContentSize().width - 10, lbBtn->getContentSize().height);
            lbBtn->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbBtn->setOverflow(Label::Overflow::SHRINK);
            itemFishingX1->addChild(lbBtn);
        }
        
        auto itemFishingX10 = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/event/event_btn_01.png"), nullptr, CC_CALLBACK_1(PopupFishing::onClickFishing, this));
        itemFishingX10->setTag(10);
        menu->addChild(itemFishingX10);
        {
            auto lbBtn = Label::createWithTTF(GAME_TEXT("t_ui_event_fish_fishing_btn_3"), GAME_FONT, 30);
            lbBtn->setPosition(itemFishingX10->getContentSize().width/2, itemFishingX10->getContentSize().height / 2);
            lbBtn->setDimensions(itemFishingX10->getContentSize().width - 10, lbBtn->getContentSize().height);
            lbBtn->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbBtn->setOverflow(Label::Overflow::SHRINK);
            itemFishingX10->addChild(lbBtn);
        }
        
        //
        menu->alignItemsHorizontallyWithPadding(10);
    }
    
    
    //
    auto sprBait = Sprite::create("Assets/ui/event/icon_event_bobber.png");
    sprBait->setPosition(_layerContainerFishing->getContentSize().width / 2 - 20, 240);
    sprBait->getTexture()->setTexParameters(texParams);
    sprBait->setScale(1.2f);
    _layerContainerFishing->addChild(sprBait);
    {
        _labelBaitCount = MafNode::createBmFont(MafUtils::format("X%d", EventFishingManager::getInstance()->getBaitCount()), GAME_BM_FONT_AL, 25);
        _labelBaitCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        _labelBaitCount->setPosition(sprBait->getContentSize().width - 5, sprBait->getContentSize().height/2 - 5);
        _labelBaitCount->setColor(Color3B(255, 56, 56));
        sprBait->addChild(_labelBaitCount);
    }
    
    //
    for(int i = 0; i < 4; ++i)
    {
        std::string path = MafUtils::format("Assets/ui/event/icon_event_1_%d.png", i + 1);
        
        auto sprFish = Sprite::create(path);
        sprFish->setPosition(sprFish->getContentSize().width, _layerContainerFishing->getContentSize().height - 50 - i * (sprFish->getContentSize().height + 10));
        _layerContainerFishing->addChild(sprFish);
        {
            auto lbScore = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_event_fish_reward_point_%d", i+1)), GAME_FONT, 20);
            lbScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            lbScore->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbScore->setPosition(sprFish->getContentSize().width + 10, sprFish->getContentSize().height/2);
            lbScore->enableOutline(Color4B::BLACK, 1);
            sprFish->addChild(lbScore);
        }
    }
    
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::COSTUME_COIN);
    list.push_back(E_ITEMS::EVENT_BAIT);
    list.push_back(E_ITEMS::EVENT_BLACK_PEARL);
    
    auto uiCurrency = UICurrency::create(list, Size(562.5, 50));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiCurrency->setPosition(Vec2(0, 0));
    _layerContainerFishing->addChild(uiCurrency);
    
    {//포인트
        int nWidthChild = 187;
        int nHeightChild = 50;
        
        auto layer = Layer::create();
        layer->setContentSize(Size(nWidthChild, nHeightChild));
        layer->setIgnoreAnchorPointForPosition(false);
        layer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        layer->setPosition(_layerContainerFishing->getContentSize().width, 0);
        _layerContainerFishing->addChild(layer);
        
        // icon
        auto spriteIcon = Sprite::create("Assets/ui/event/icon_event_fishing_point.png");
        spriteIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        spriteIcon->setPosition(0, layer->getContentSize().height / 2);
        layer->addChild(spriteIcon);
        
        //
        _labelPointcount = MafNode::createBmFont("0", GAME_BM_FONT_AL, 24);
        _labelPointcount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        _labelPointcount->setPosition(spriteIcon->getBoundingBox().size.width - 10, layer->getContentSize().height / 2);
        _labelPointcount->setColor(Color3B(255, 212, 56));
        _labelPointcount->enableShadow(Color4B::BLACK,Size(3,-3),5);
        layer->addChild(_labelPointcount);
        
        float nScaleCount = nWidthChild * 0.7 / _labelPointcount->getContentSize().width;
        if ( nScaleCount < 1 )
        {
            _labelPointcount->setScale(nScaleCount);
        }
    }
    
    //
    drawFishing();
}
void PopupFishing::uiBait()
{
    _layerContainerBait->removeAllChildren();
    
    auto spriteBG = Sprite::create("Assets/ui/event/event_box_01.png");
    spriteBG->setPosition(_layerContainerBait->getContentSize().width / 2, _layerContainerBait->getContentSize().height / 2);
    _layerContainerBait->addChild(spriteBG);
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    _layerContainerBait->addChild(menu);
    
    //
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_fish_bait_guide_1"), GAME_FONT, 24);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    lbTitle->setPosition(20, spriteBG->getContentSize().height - 50);
    lbTitle->setDimensions(480, lbTitle->getContentSize().height);
    lbTitle->setOverflow(Label::Overflow::SHRINK);
    lbTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbTitle->setColor(Color3B(69, 157, 0));
    spriteBG->addChild(lbTitle);
    
    auto lbContent = Label::createWithTTF(GAME_TEXT("t_ui_event_fish_bait_guide_2"), GAME_FONT, 20);
    lbContent->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbContent->setPosition(Vec2(20, spriteBG->getContentSize().height - 50));
    lbContent->setDimensions(480, 100);
    lbContent->setOverflow(Label::Overflow::SHRINK);
    lbContent->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    spriteBG->addChild(lbContent);
    
    //
    auto sprItem = ui::Scale9Sprite::create("Assets/ui_common/btn_2_3_on.png");
    sprItem->setContentSize(Size(200, 110));
    
    auto itemBuyBait = MafNode::MafMenuItemSprite::create(sprItem, nullptr, CC_CALLBACK_1(PopupFishing::onClickBuyBait, this));
    itemBuyBait->setPosition(spriteBG->getContentSize().width - 120, spriteBG->getContentSize().height / 2);
    menu->addChild(itemBuyBait);
    {
        _labelBaitBuy = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_event_fish_buybait_btn", EventFishingManager::getInstance()->getBaitBuyRemaining(), EventFishingManager::getInstance()->getBaitBuyMax()), GAME_FONT, 25);
        _labelBaitBuy->setPosition(itemBuyBait->getContentSize().width/2, itemBuyBait->getContentSize().height/2);
        _labelBaitBuy->setDimensions(itemBuyBait->getContentSize().width - 10, _labelBaitBuy->getContentSize().height);
        _labelBaitBuy->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        _labelBaitBuy->setOverflow(Label::Overflow::SHRINK);
        _labelBaitBuy->setColor(Color3B(255, 212, 0));
        itemBuyBait->addChild(_labelBaitBuy);
    }
}
void PopupFishing::uiQuest()
{
    _layerContainerQuest->removeAllChildren();
    
    //
    auto spriteTableBar = ui::Scale9Sprite::create("Assets/ui/prison/list_top.png", Rect::ZERO, Rect(0,0,720,34));
    spriteTableBar->setContentSize(Size(_layerContainerQuest->getContentSize().width, 34));
    spriteTableBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    spriteTableBar->setPosition(_layerContainerQuest->getContentSize().width / 2, _layerContainerQuest->getContentSize().height);
    _layerContainerQuest->addChild(spriteTableBar);
    {
        auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_title"), GAME_FONT, 24);
        labelTitle->setPosition( Vec2(105, spriteTableBar->getContentSize().height / 2) );
        spriteTableBar->addChild(labelTitle);
        
        auto labelContents = Label::createWithTTF(GAME_TEXT("t_ui_contents"), GAME_FONT, 24);
        labelContents->setPosition( Vec2(330, spriteTableBar->getContentSize().height / 2) );
        spriteTableBar->addChild(labelContents);
        
        auto labelReward = Label::createWithTTF(GAME_TEXT("t_ui_reward"), GAME_FONT, 24);
        labelReward->setPosition( Vec2(600, spriteTableBar->getContentSize().height / 2) );
        spriteTableBar->addChild(labelReward);
    }
    
    _table = extension::TableView::create(this, Size(_layerContainerQuest->getContentSize().width, _layerContainerQuest->getContentSize().height - 34));
    _table->setDirection(extension::ScrollView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition( Vec2(_layerContainerQuest->getContentSize().width / 2, 0) );
    _table->setDelegate(this);
    _layerContainerQuest->addChild(_table);
}
void PopupFishing::uiBoard()
{
    _layerContainerBoard->removeAllChildren();
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    _layerContainerBoard->addChild(menu);
    
    auto itemBoard = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/event/event_btn_roulette1_1.png"), nullptr, CC_CALLBACK_1(PopupFishing::onClickBanner, this));
    itemBoard->setPosition(_layerContainerBoard->getContentSize().width / 2, _layerContainerBoard->getContentSize().height / 2);
    menu->addChild(itemBoard);
    {
        auto lbBannerTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_dice_btn_text_1"), GAME_FONT, 32);
        lbBannerTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbBannerTitle->setPosition(50, itemBoard->getContentSize().height * 0.75);
        lbBannerTitle->setDimensions(480, lbBannerTitle->getContentSize().height);
        lbBannerTitle->setOverflow(Label::Overflow::SHRINK);
        lbBannerTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbBannerTitle->setColor(Color3B(255,240,105));
        itemBoard->addChild(lbBannerTitle);
        
        auto sprBannerContent = Sprite::create("Assets/ui/event/icon_event_fishing_point.png");
        sprBannerContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprBannerContent->setPosition(40, itemBoard->getContentSize().height * 0.3);
        itemBoard->addChild(sprBannerContent);
        {
            auto lbBannerContent = Label::createWithTTF(GAME_TEXT("t_ui_event_dice_btn_text_2"), GAME_FONT, 26);
            lbBannerContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            lbBannerContent->setPosition(sprBannerContent->getContentSize().width, sprBannerContent->getContentSize().height / 2);
            lbBannerContent->setDimensions(422, lbBannerContent->getContentSize().height);
            lbBannerContent->setOverflow(Label::Overflow::SHRINK);
            lbBannerContent->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
            sprBannerContent->addChild(lbBannerContent);
        }
    }
}

  

void PopupFishing::initCurrency()
{
//    {//재화
//        //미끼
//        auto sprBait = Sprite::create("Assets/ui/event/icon_event_bobber.png");
//        sprBait->setPosition(sprBait->getContentSize().width, _layerContainerCurrency->getContentSize().height / 2);
//        sprBait->setScale(1.2f);
//        _layerContainerCurrency->addChild(sprBait);
//        
//        float startX = sprBait->getContentSize().width;
//        
//        _lbTotalBait = MafNode::createBmFont("", GAME_BM_FONT_AL, 22);
//        _lbTotalBait->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//        _lbTotalBait->setPosition( Vec2(sprBait->getContentSize().width, sprBait->getContentSize().height/2));
//        _lbTotalBait->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
//        _lbTotalBait->setDimensions(90, 25);
//        _lbTotalBait->setOverflow(Label::Overflow::SHRINK);
//        _lbTotalBait->setColor(Color3B(255, 56, 56));
//        sprBait->addChild(_lbTotalBait);
//        
//
//        

//        
//        //완주 티켓
//        auto sprTicket = Sprite::create("Assets/ui/event/icon_event_black_pearl.png");
//        sprTicket->setPosition(startX + 187*2, _layerContainerCurrency->getContentSize().height / 2);
//        sprTicket->setScale(1.2f);
//        _layerContainerCurrency->addChild(sprTicket);
//        
//        _lbTotalTicket = MafNode::createBmFont("", GAME_BM_FONT_AL, 22);
//        _lbTotalTicket->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//        _lbTotalTicket->setPosition( Vec2(sprTicket->getContentSize().width, sprTicket->getContentSize().height/2));
//        _lbTotalTicket->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
//        _lbTotalTicket->setDimensions(90, 25);
//        _lbTotalTicket->setOverflow(Label::Overflow::SHRINK);
//        _lbTotalTicket->setColor(Color3B(99, 219, 255));
//        sprTicket->addChild(_lbTotalTicket);
//        
//        //마코
//        auto sprCoin = Sprite::create("Assets/icon/icon_coin_2.png");
//        sprCoin->setPosition(startX + 187*3, _layerContainerCurrency->getContentSize().height / 2);
//        sprCoin->setScale(1.2f);
//        _layerContainerCurrency->addChild(sprCoin);
//        
//        _lbTotalCoin = MafNode::createBmFont("", GAME_BM_FONT_AL, 22);
//        _lbTotalCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//        _lbTotalCoin->setPosition( Vec2(sprCoin->getContentSize().width, sprCoin->getContentSize().height/2));
//        _lbTotalCoin->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
//        _lbTotalCoin->setDimensions(90, 25);
//        _lbTotalCoin->setOverflow(Label::Overflow::SHRINK);
//        _lbTotalCoin->setColor(Color3B(255, 212, 56));
//        sprCoin->addChild(_lbTotalCoin);
//    }
}

 

#pragma mark - draw
void PopupFishing::drawFishing()
{
    auto bgAnim = Animation::create();
    bgAnim->setDelayPerUnit(0.18f);

    for(int i = 0; i < 3; ++i)
    {
        std::string sprPath = MafUtils::format("Assets/ui/event/event_bg_1_%d.png", i+2);

        bgAnim->addSpriteFrameWithFile(sprPath);
        bgAnim->getFrames().at(i)->getSpriteFrame()->getTexture()->setTexParameters(texParams);
    }
    auto animate = Animate::create(bgAnim);
    auto repeat = RepeatForever::create(animate);
    _spriteFishing->stopAllActions();
    _spriteFishing->runAction(repeat);
    
    //
    _labelBaitCount->setString(MafUtils::format("X%d", EventFishingManager::getInstance()->getBaitCount()));
}

void PopupFishing::drawBait()
{
    _labelBaitBuy->setString(GAME_TEXTFORMAT("t_ui_event_fish_buybait_btn", EventFishingManager::getInstance()->getBaitBuyRemaining(), EventFishingManager::getInstance()->getBaitBuyMax()));
}

void PopupFishing::drawQuest()
{
    _table->reloadData();
}

#pragma mark - click
void PopupFishing::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupFishing::onClickBuyBait(cocos2d::Ref* sender)
{
    if(EventManager::getInstance()->isActivatedEvent(17) == false)
    {
        hide();
        return;
    }
    
    if ( EventFishingManager::getInstance()->getBaitBuyRemaining() <= 0)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_event_fish_buybait_pop_3"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    else
    {
        auto popup = PopupBaitCharge::create();
        popup->setCallbackResult(CC_CALLBACK_0(PopupFishing::callbackRedraw, this));
        popup->show();
    }
}

void PopupFishing::onClickBanner(cocos2d::Ref* sender)
{
    if(EventManager::getInstance()->isActivatedEvent(17) == false)
    {
        hide();
        return;
    }
    
    auto popup = PopupFishingBoardGame::create();
    popup->setCallbackClose(CC_CALLBACK_0(PopupFishing::callbackRedraw, this));
    popup->show();
}

void PopupFishing::onClickFishing(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    int nCount = item->getTag();
    
    if(EventManager::getInstance()->isActivatedEvent(17) == false)
    {
        hide();
        return;
    }
    
    int nBaitCount = EventFishingManager::getInstance()->getBaitCount();
    if ( nBaitCount < nCount )
        return;
    
    //
    PopupLoadingLock::show();
    
    //
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.15f);
    for ( int i = 0; i < 4; i++ )
    {
        std::string strPath = MafUtils::format("Assets/ui/event/event_bg_1_%d.png",i+5);
        
        animation->addSpriteFrameWithFile(strPath);
        animation->getFrames().at(i)->getSpriteFrame()->getTexture()->setTexParameters(texParams);
    }

    auto action1_1 = DelayTime::create(1.0);
    auto action1_2 = CallFunc::create([=](){
        
        EventFishingManager::getInstance()->requestFishing(nCount, CC_CALLBACK_3(PopupFishing::callbackFishing, this));
        
    });
    
    auto seq = Sequence::create(action1_1, action1_2, NULL);
    _spriteFishing->stopAllActions();
    _spriteFishing->runAction(RepeatForever::create(Animate::create(animation)));
    _spriteFishing->runAction(seq);
}

 
#pragma mark - callback
void PopupFishing::callbackInfo(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_2");
        if ( nResult == 2 )
            strMessage = GAME_TEXT("t_ui_error_6");
            
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){

            hide();

        });
        popup->show();
        return;
    }
    
    //
    _listQuest.clear();
    _listQuest = EventFishingManager::getInstance()->getListQuest();
    
    //
    drawFishing();
    drawBait();
    drawQuest();
}

void PopupFishing::callbackFishing(bool bResult, int nResult, std::string strReward)
{
    PopupLoadingLock::hide();
    
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_2");
        if ( nResult == 2 )
            strMessage = GAME_TEXT("t_ui_error_6");
            
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){

            hide();

        });
        popup->show();
        return;
    }
    
    drawFishing();
    drawBait();
    drawQuest();
    
    //
    auto popup = PopupFishingReward::create(strReward);
    popup->show();
}

void PopupFishing::callbackRedraw()
{
    //
    _listQuest.clear();
    _listQuest = EventFishingManager::getInstance()->getListQuest();
    
    drawFishing();
    drawBait();
    drawQuest();
}

