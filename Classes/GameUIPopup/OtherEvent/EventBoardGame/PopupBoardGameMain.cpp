//
//  PopupBoardGameMain.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/15.
//

#include "PopupBoardGameMain.h"

#include "GameObject/InfoQuest.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/OtherEvent/EventBoardGame/PopupBoardGame.h"
#include "GameUIPopup/OtherEvent/Event/PopupEventSkinShop.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidDefault.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerEvent/EventBoardGameManager.h"
#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/CashManager.h"


PopupBoardGameMain* PopupBoardGameMain::create()
{
    PopupBoardGameMain* pRet = new(std::nothrow) PopupBoardGameMain();
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

PopupBoardGameMain::PopupBoardGameMain(void) :

_layerContainerTop(nullptr),
_layerContainerInfo(nullptr),
_layerContainerQuest(nullptr),
_layerContainerBoard(nullptr),

_spriteFishing(nullptr),
_uiTextAdsCount(nullptr),

_table(nullptr)
{
    
}

PopupBoardGameMain::~PopupBoardGameMain(void)
{
    
}

bool PopupBoardGameMain::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    initVar();
    initUI();
    
    
    return true;
    
}

void PopupBoardGameMain::onEnter()
{
    PopupBase::onEnter();
    
    PopupLoading::show();
    EventBoardGameManager::getInstance()->requestBoardInfo(CC_CALLBACK_2(PopupBoardGameMain::callbackInfo, this));
}

#pragma mark - override : table
void PopupBoardGameMain::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
 
}

void PopupBoardGameMain::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupBoardGameMain::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, C_BAR_SIZE_RAID_QUEST);
}

extension::TableViewCell* PopupBoardGameMain::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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

ssize_t PopupBoardGameMain::numberOfCellsInTableView(extension::TableView *table)
{
    return _listQuest.size();
}

#pragma mark - override : refresh
void PopupBoardGameMain::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    
}

#pragma mark - ui
void PopupBoardGameMain::initVar()
{
    
}

void PopupBoardGameMain::initUI()
{
    double nWidth = 750;
    double nHeight = getContentSize().height;
    if ( nHeight > 1302 )
        nHeight = 1302;
    
    // size
    _spriteContainer->setContentSize( Size(nWidth, nHeight) );
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(0);
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_boardgame_logo"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupBoardGameMain::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    _layerContainerTop = Layer::create();
    _layerContainerTop->setContentSize( Size(_spriteContainer->getContentSize().width, 676) );
    _layerContainerTop->setIgnoreAnchorPointForPosition(false);
    _layerContainerTop->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerTop->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_layerContainerTop);
    
    _layerContainerInfo = Layer::create();
    _layerContainerInfo->setContentSize( Size(_spriteContainer->getContentSize().width, 156) );
    _layerContainerInfo->setIgnoreAnchorPointForPosition(false);
    _layerContainerInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerInfo->setPosition(_spriteContainer->getContentSize().width / 2, _layerContainerTop->getPositionY() - _layerContainerTop->getContentSize().height);
    _spriteContainer->addChild(_layerContainerInfo);
    
    _layerContainerBoard = Layer::create();
    _layerContainerBoard->setContentSize( Size(_spriteContainer->getContentSize().width, 120) );
    _layerContainerBoard->setIgnoreAnchorPointForPosition(false);
    _layerContainerBoard->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerBoard->setPosition(_spriteContainer->getContentSize().width / 2, 0);
    _spriteContainer->addChild(_layerContainerBoard);
    
    _layerContainerQuest = Layer::create();
    _layerContainerQuest->setContentSize( Size(_spriteContainer->getContentSize().width - 5, _spriteContainer->getContentSize().height - 75 - _layerContainerTop->getContentSize().height - _layerContainerInfo->getContentSize().height - _layerContainerBoard->getContentSize().height) );
    _layerContainerQuest->setIgnoreAnchorPointForPosition(false);
    _layerContainerQuest->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerQuest->setPosition(_spriteContainer->getContentSize().width / 2, _layerContainerBoard->getContentSize().height);
    _spriteContainer->addChild(_layerContainerQuest);
    
    
    //
    uiTop();
    uiInfo();
    uiQuest();
    uiBoard();
}
//
#pragma mark - ui
void PopupBoardGameMain::uiTop()
{
    _layerContainerTop->removeAllChildren();

    _spriteFishing = Sprite::create("Assets/ui/event/event_bg_21halloween.png");
    _spriteFishing->setPosition(_layerContainerTop->getContentSize().width / 2, _layerContainerTop->getContentSize().height / 2);
    _spriteFishing->getTexture()->setTexParameters(texParams);
    _spriteFishing->setScale(4);
    _layerContainerTop->addChild(_spriteFishing);

    //
    
    auto sprBox = Sprite::create("Assets/ui_common/base_text_1_2.png");
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBox->setPosition(_layerContainerTop->getContentSize().width/2, _layerContainerTop->getContentSize().height);
    _layerContainerTop->addChild(sprBox);
    
    
    {
        std::string text = "";
        if ( EventManager::getInstance()->isActivatedEvent(26) == true )
        {
            int64_t nEventEndTime = EventManager::getInstance()->getActivatedEvent(26)->getEventEnd();
            int64_t nTimeNow = UtilsDate::getInstance()->getTime();
                
            int nTimeDiff = (int)(nEventEndTime - nTimeNow);
            
            int d = nTimeDiff / 24 / 3600;
            int h = nTimeDiff / 3600;
            
            if(d >= 1 ) h %= 24;
            if(d <= 0)
                d = 0;
            if(h <= 0)
                h = 0;
            
            text = GAME_TEXTFORMAT("t_ui_event_bingo_timer_1", d, h);
        }
        else
        {
            text = GAME_TEXT("t_ui_draw_msg_1");
        }
        auto lbContent = Label::createWithTTF(text, GAME_FONT, 16);
        lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbContent->setPosition(sprBox->getContentSize()/2);
        lbContent->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbContent->setDimensions(sprBox->getContentSize().width, sprBox->getContentSize().height - 5);
        lbContent->setOverflow(Label::Overflow::SHRINK);
        sprBox->addChild(lbContent);
    }
    
    //
    auto btnSkin = ui::Button::create("Assets/ui/event/event_halloween_skinbox.png");
    btnSkin->setScale9Enabled(true);
    btnSkin->addClickEventListener([=](Ref*){
        auto popup = PopupEventSkinShop::create();
        popup->show();
    });
    btnSkin->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    btnSkin->setPosition(Vec2(15, 70));
    btnSkin->setZoomScale(0);
    _layerContainerTop->addChild(btnSkin);
    {
        std::vector<std::string> vecPath;
        std::string strEventOtherText = EventManager::getInstance()->getEventOhterDataByEventIdx(26);
        if ( strEventOtherText.empty() == false )
        {
            rapidjson::Document jsonParser;
            jsonParser.ParseInsitu<0>((char*)strEventOtherText.c_str());
            if( jsonParser.HasParseError() == false )
            {
                for ( int i = 0; i < jsonParser["info_skin"].Size(); i++ )
                {
                    vecPath.push_back(MafUtils::format("Assets/icon_hero/skin_%d.png", jsonParser["info_skin"][i].GetInt()));
                }
            }
        }
        
        for(int i = 0; i < vecPath.size(); ++i)
        {
            auto sprIcon = Sprite::create(vecPath[i]);
            sprIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            sprIcon->setPosition(Vec2(22 + 74 * i, btnSkin->getContentSize().height - 20));
            btnSkin->addChild(sprIcon);
        }
        
        auto lbBuySkin = Label::createWithTTF(GAME_TEXT("t_ui_event_boardgame_main_reward_bt_goto"), GAME_FONT, 16);
        lbBuySkin->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        lbBuySkin->setPosition(btnSkin->getContentSize().width/2, 10);
        lbBuySkin->setDimensions(btnSkin->getContentSize().width, lbBuySkin->getContentSize().height);
        lbBuySkin->setOverflow(Label::Overflow::SHRINK);
        lbBuySkin->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
        btnSkin->addChild(lbBuySkin);
        
        auto sprEffect = ui::Scale9Sprite::create("Assets/ui/event/event_halloween_skinbox_eff.png");
        sprEffect->setPosition(btnSkin->getContentSize()/2);
        sprEffect->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1.f), FadeOut::create(1.f), NULL)));
        btnSkin->addChild(sprEffect);
        
        auto sprNewMark = Sprite::create("Assets/ui/event/event_new.png");
        sprNewMark->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        sprNewMark->setPosition(btnSkin->getContentSize().width, btnSkin->getContentSize().height);
        btnSkin->addChild(sprNewMark);
    }
    
    _uiBtnAds = ui::Button::create("Assets/ui_common/btn_ad_1.png");
    _uiBtnAds->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _uiBtnAds->setPosition(Vec2(_layerContainerTop->getContentSize().width - 15, 70));
    _uiBtnAds->addClickEventListener(CC_CALLBACK_1(PopupBoardGameMain::onClickAds, this));
    _uiBtnAds->setZoomScale(0);
    _layerContainerTop->addChild(_uiBtnAds);
    {
        auto uiIconAD = ui::ImageView::create("Assets/icon/icon_ad_1_2.png");
        uiIconAD->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiIconAD->setPosition(Vec2(0, _uiBtnAds->getContentSize().height - 10));
        _uiBtnAds->addChild(uiIconAD);
        
        auto uiDice = UtilsWidget::getInstance()->createIconText("Assets/ui/event/icon_event_dice.png", "x1", _uiBtnAds->getContentSize().width - 10, 22, 5);
        uiDice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiDice->setPosition(Vec2(_uiBtnAds->getContentSize().width / 2, _uiBtnAds->getContentSize().height / 2 - 10));
        _uiBtnAds->addChild(uiDice);
        
        auto uiCount = UtilsWidget::getInstance()->createIconText("Assets/icon/icon_ad_1_1.png", "0/0", _uiBtnAds->getContentSize().width - 10, 22, 0);
        uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiCount->setPosition(Vec2(_uiBtnAds->getContentSize().width / 2, _uiBtnAds->getContentSize().height / 2 + 10));
        _uiBtnAds->addChild(uiCount);
        
        _uiTextAdsCount = (ui::CText*)uiCount->getChildByName("WIDGET_TEXT");
    }
    
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::COSTUME_COIN);
    list.push_back(E_ITEMS::EVENT_HALLOWEEN_CANDY);
    list.push_back(E_ITEMS::EVENT_DICE_POINT);

    auto uiCurrency = UICurrency::create(list, Size(_layerContainerTop->getContentSize().width - 10, 50));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiCurrency->setPosition(Vec2(_layerContainerTop->getContentSize().width/2, 0));
    _layerContainerTop->addChild(uiCurrency);

}

void PopupBoardGameMain::uiInfo()
{
    _layerContainerInfo->removeAllChildren();

    auto spriteBG = ui::Scale9Sprite::create("Assets/ui_common/box_02.png");
    spriteBG->setContentSize(_layerContainerInfo->getContentSize());
    spriteBG->setPosition(_layerContainerInfo->getContentSize().width / 2, _layerContainerInfo->getContentSize().height / 2);
    _layerContainerInfo->addChild(spriteBG);

    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    _layerContainerInfo->addChild(menu);

    //
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_boardgame_point_guide_1"), GAME_FONT, 24);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    lbTitle->setPosition(20, spriteBG->getContentSize().height - 50);
    lbTitle->setDimensions(480, lbTitle->getContentSize().height);
    lbTitle->setOverflow(Label::Overflow::SHRINK);
    lbTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbTitle->setColor(Color3B(0, 154, 250));
    spriteBG->addChild(lbTitle);

    auto lbContent = Label::createWithTTF(GAME_TEXT("t_ui_event_boardgame_point_guide_2"), GAME_FONT, 20);
    lbContent->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbContent->setPosition(Vec2(20, spriteBG->getContentSize().height - 50));
    lbContent->setDimensions(480, 100);
    lbContent->setOverflow(Label::Overflow::SHRINK);
    lbContent->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    spriteBG->addChild(lbContent);

    //
    auto sprItem = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png");
    sprItem->setContentSize(Size(200, 110));

    auto itemEnterRaid = MafNode::MafMenuItemSprite::create(sprItem, nullptr, CC_CALLBACK_1(PopupBoardGameMain::onClickRaidEnter, this));
    itemEnterRaid->setPosition(spriteBG->getContentSize().width - 120, spriteBG->getContentSize().height / 2);
    menu->addChild(itemEnterRaid);
    {
        auto lbEnterRaid = Label::createWithTTF(GAME_TEXT("t_ui_event_boardgame_btn_goto_raid"), GAME_FONT, 25);
        lbEnterRaid->setPosition(itemEnterRaid->getContentSize().width/2, itemEnterRaid->getContentSize().height/2);
        lbEnterRaid->setDimensions(itemEnterRaid->getContentSize().width - 10, lbEnterRaid->getContentSize().height);
        lbEnterRaid->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbEnterRaid->setOverflow(Label::Overflow::SHRINK);
        lbEnterRaid->setColor(Color3B(255, 212, 0));
        itemEnterRaid->addChild(lbEnterRaid);
    }
}

void PopupBoardGameMain::uiQuest()
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

void PopupBoardGameMain::uiBoard()
{
    _layerContainerBoard->removeAllChildren();

    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    _layerContainerBoard->addChild(menu);

    auto itemBoard = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/event/event_btn_roulette1_1.png"), nullptr, CC_CALLBACK_1(PopupBoardGameMain::onClickBanner, this));
    itemBoard->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    itemBoard->setPosition(_layerContainerBoard->getContentSize().width / 2, _layerContainerBoard->getContentSize().height);
    menu->addChild(itemBoard);
    {
        auto lbBannerTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_boardgame_btn_text_1"), GAME_FONT, 32);
        lbBannerTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbBannerTitle->setPosition(50, itemBoard->getContentSize().height * 0.75);
        lbBannerTitle->setDimensions(480, lbBannerTitle->getContentSize().height);
        lbBannerTitle->setOverflow(Label::Overflow::SHRINK);
        lbBannerTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbBannerTitle->setColor(Color3B(255,240,105));
        itemBoard->addChild(lbBannerTitle);
        
        auto lbBannerContent = Label::createWithTTF(GAME_TEXT("t_ui_event_dice_btn_text_2"), GAME_FONT, 26);
        lbBannerContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbBannerContent->setPosition(40, itemBoard->getContentSize().height * 0.3);
        lbBannerContent->setDimensions(422, lbBannerContent->getContentSize().height);
        lbBannerContent->setOverflow(Label::Overflow::SHRINK);
        lbBannerContent->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        itemBoard->addChild(lbBannerContent);
    }
}

#pragma mark - draw
void PopupBoardGameMain::drawQuest()
{
    _table->reloadData();
}

void PopupBoardGameMain::drawInfoAds()
{
    int adsNow = EventBoardGameManager::getInstance()->getAdsNow();
    int adsMax = EventBoardGameManager::getInstance()->getAdsMax();
    adsNow = adsMax - adsNow;
    if ( adsNow < 0 )
    {
        adsNow = 0;
    }
    
    std::string text = MafUtils::format("%d/%d", adsNow, adsMax);
    _uiTextAdsCount->setString(text);
    

    if ( adsNow <= 0 )
    {
        _uiBtnAds->loadTextureNormal("Assets/ui_common/btn_ad_gray.png");
    }
    else
    {
        _uiBtnAds->loadTextureNormal("Assets/ui_common/btn_ad_1.png");
    }
    
}

#pragma mark - click
void PopupBoardGameMain::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupBoardGameMain::onClickRaidEnter(cocos2d::Ref* sender)
{
    if(EventManager::getInstance()->isActivatedEvent(26) == false)
    {
        PopupToast::show(GAME_TEXT("t_ui_draw_msg_1"));
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
    
    if ( bEnter == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_new_raid_title"), GAME_TEXT("t_ui_new_raid_condition_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto popup = PopupRaidDefault::create();
    popup->show();
    
    hide();
}

void PopupBoardGameMain::onClickBanner(cocos2d::Ref* sender)
{
    if(EventManager::getInstance()->isActivatedEvent(26) == false)
    {
        PopupToast::show(GAME_TEXT("t_ui_draw_msg_1"));
        return;
    }

    auto popup = PopupBoardGame::create();
    popup->setCallbackClose(CC_CALLBACK_0(PopupBoardGameMain::callbackRedraw, this));
    popup->show();
}

void PopupBoardGameMain::onClickAds(cocos2d::Ref* sender)
{
    int adsNow = EventBoardGameManager::getInstance()->getAdsNow();
    int adsMax = EventBoardGameManager::getInstance()->getAdsMax();
    
    if ( adsNow >= adsMax )
    {
        return;
    }
    
    if ( EventManager::getInstance()->isActivatedEvent(26) == false )
    {
        PopupToast::show(GAME_TEXT("t_ui_draw_msg_1"));
        return;
    }
    
    if ( CashManager::getInstance()->isAdsfreeTime() == true )
    {
        callbackAds(ADS_RESULT::SUCCESS);
    }
    else
    {
        MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupBoardGameMain::callbackAds, this));
    }
}

#pragma mark - callback
void PopupBoardGameMain::callbackInfo(bool bResult, int nResult)
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
    _listQuest = EventBoardGameManager::getInstance()->getListQuest();
    
    //
    drawQuest();
    drawInfoAds();
}

void PopupBoardGameMain::callbackRedraw()
{
    //
    _listQuest.clear();
    _listQuest = EventBoardGameManager::getInstance()->getListQuest();
    
    //
    drawQuest();
    drawInfoAds();
}

void PopupBoardGameMain::callbackAds(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotBoardGame, CashManager::getInstance()->isAdsfreeTime());
        
        //
        PopupLoading::show();
        EventBoardGameManager::getInstance()->requestBoardAds(CC_CALLBACK_2(PopupBoardGameMain::callbackAdsReward, this));
    }
    else
    {
        if ( result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(PopupBoardGameMain::callbackAds, this));
            popup->setLogParams(kAnalEventAdsLoadParameterSlot);
            popup->setLogValue(kAdsSlotBoardGame);
            popup->show();
        }
        else
        {
            std::string strMessage = GAME_TEXT("t_ui_error_14");
            
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
    }
}

void PopupBoardGameMain::callbackAdsReward(bool bResult, int nResult)
{
    PopupLoading::hide();

    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 2 )
            strMessage = GAME_TEXT("t_ui_error_6");
        else if ( nResult == 4 )
            strMessage = GAME_TEXT("t_ui_error_15");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    
    //
    drawInfoAds();
}
