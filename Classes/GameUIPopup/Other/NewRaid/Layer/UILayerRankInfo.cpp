//
//  UILayerRankInfo.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/01/27.
//

#include "UILayerRankInfo.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/RankingManager.h"

USING_NS_CC;

UILayerRankInfo* UILayerRankInfo::create(cocos2d::Size size, std::vector<InfoRaidTierInfo*> list)
{
    UILayerRankInfo* pRet = new(std::nothrow) UILayerRankInfo();
    if ( pRet && pRet->init(size, list) )
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

UILayerRankInfo::UILayerRankInfo(void):
_table(nullptr)
{
    _vecInfo.clear();
}

UILayerRankInfo::~UILayerRankInfo(void)
{
    _vecInfo.clear();
}

bool UILayerRankInfo::init(cocos2d::Size size, std::vector<InfoRaidTierInfo*> list)
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    setContentSize(size);
    _vecInfo = list;
    setIgnoreAnchorPointForPosition(false);
    
    //
    
    // init
    initVar();
    initUi();
    
    return true;
}


#pragma mark - table
void UILayerRankInfo::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size UILayerRankInfo::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 70);
}

extension::TableViewCell* UILayerRankInfo::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    
    std::string strBadgePath = "";
    std::string strRewardTitle = "";
    std::string strRewardBuff = "";
    std::string strRewardCondition = "";
    bool isMyTier = false;
    RankInfo* infoMyRankInfo = RankingManager::getInstance()->getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::NEW_RAID);
    auto infoRankTier = _vecInfo[idx];
    InfoBadge* infoBadge = infoRankTier->getInfoBadge();
    
    int effect = 0;
    if(infoBadge != nullptr)
    {
        strBadgePath = infoBadge->getPath();
        strRewardTitle = infoBadge->getName();
        effect = infoBadge->getBuffCount(true) * 100;
        if(effect == 0)
            strRewardBuff = "-";
        else
            strRewardBuff = GAME_TEXTFORMAT("t_badge_spec", effect) + "%";
    }
    
    switch (infoRankTier->getType())
    {
        case PopupRaidRankRewardInfo::E_RAID_TIER_RANK_TYPE::RANK:
        {
            if(infoRankTier->getCondition1() == infoMyRankInfo->getRank())
            {
                isMyTier = true;
            }
            strRewardCondition = GAME_TEXTFORMAT("t_ui_team_raid_reward_single", infoRankTier->getCondition1());
        }break;
        case PopupRaidRankRewardInfo::E_RAID_TIER_RANK_TYPE::RANK_RANGE:
        {
            if(infoRankTier->getCondition1() <= infoMyRankInfo->getRank() &&
               infoRankTier->getCondition2() >= infoMyRankInfo->getRank())
            {
                isMyTier = true;
    
            }
    
            strRewardCondition = GAME_TEXTFORMAT("t_ui_team_raid_reward_range", infoRankTier->getCondition2());
        }break;
        case PopupRaidRankRewardInfo::E_RAID_TIER_RANK_TYPE::TIER:
        {
            if(infoMyRankInfo->getPercentage() >= infoRankTier->getCondition1() &&
               infoMyRankInfo->getPercentage() <= infoRankTier->getCondition2())
            {
                isMyTier = true;
            }
    
            strRewardCondition = GAME_TEXTFORMAT("t_ui_team_raid_reward_range", infoRankTier->getCondition2()) + "%";
        }break;
    }
    
    if(isMyTier == true)
    {
        auto sprBG = Sprite::create("Assets/ui/ranking/ranking_bar1_1.png");
        sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprBG->setPosition(size/2);
        cell->addChild(sprBG);
    }
    
    auto lyBadge = Layer::create();
    lyBadge->setIgnoreAnchorPointForPosition(false);
    lyBadge->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lyBadge->setPosition(size.width/6, size.height/2);
    lyBadge->setContentSize(Size(100, 100));
    cell->addChild(lyBadge);
    
    auto sprBadge = Sprite::create(strBadgePath);
    sprBadge->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBadge->setPosition(lyBadge->getContentSize()/2);
    sprBadge->setScale(0.7f);
    lyBadge->addChild(sprBadge);
    
    auto lbTitle = Label::createWithTTF(strRewardTitle, GAME_FONT, 25);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbTitle->setDimensions(lbTitle->getContentSize().width, size.height/2);
    lbTitle->setOverflow(Label::Overflow::SHRINK);
    lbTitle->setPosition(lyBadge->getPositionX() + lyBadge->getContentSize().width/2 , size.height - size.height/3);
    cell->addChild(lbTitle);
    
    auto lbBuffEffect = Label::createWithTTF(strRewardBuff, GAME_FONT, 20);
    lbBuffEffect->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbBuffEffect->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    lbBuffEffect->setDimensions(lbBuffEffect->getContentSize().width, size.height/2);
    lbBuffEffect->setOverflow(Label::Overflow::SHRINK);
    lbBuffEffect->setPosition(lbTitle->getPosition() + Vec2::DOWN * lbTitle->getContentSize().height/2.4);
    if(isMyTier == true && effect != 0)
    {
        lbBuffEffect->setColor(Color3B(255, 255, 0));
    }
    cell->addChild(lbBuffEffect);
    
    auto lbCondition = Label::createWithTTF(strRewardCondition, GAME_FONT, 24);
    lbCondition->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbCondition->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lbCondition->setDimensions(size.width/3, size.height);
    lbCondition->setOverflow(Label::Overflow::SHRINK);
    lbCondition->setPosition(size.width - size.width/4, size.height/2);
    cell->addChild(lbCondition);
    
    auto layerScroll = ui::ScrollView::create();
    layerScroll->setBounceEnabled(false);
    layerScroll->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    layerScroll->setContentSize(Size(140, cell->getContentSize().height - 10));
    layerScroll->setIgnoreAnchorPointForPosition(false);
    layerScroll->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    layerScroll->setPosition(Vec2(cell->getContentSize() / 2) + Vec2::LEFT * 20);
    layerScroll->setSwallowTouches(false);
    cell->addChild(layerScroll);
    
    double innerWidth = 0;
    double innerHeight = layerScroll->getContentSize().height;
    
    auto listItems = infoRankTier->getListReward();
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
    }
    if(innerWidth < layerScroll->getContentSize().width)
        layerScroll->setContentSize(Size(innerWidth, innerHeight));
    
    layerScroll->setInnerContainerSize(Size(innerWidth, innerHeight));
    
    return cell;
}

ssize_t UILayerRankInfo::numberOfCellsInTableView(extension::TableView *table)
{
    return _vecInfo.size();
}

#pragma mark -init
void UILayerRankInfo::initVar()
{
    
}

void UILayerRankInfo::initUi()
{
    _table = extension::TableView::create(this, Size(getContentSize().width, getContentSize().height));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    _table->setPosition(getContentSize().width / 2, getContentSize().height / 2);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    addChild(_table);
    
    //
    _table->reloadData();
}
