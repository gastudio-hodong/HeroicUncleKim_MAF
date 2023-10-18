//
//  PopupJelly.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupJelly.h"

#include "Common/ConfigKey.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/Ads/PopupAdsWatch.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/StoryManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/PrisonManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

PopupJelly* PopupJelly::create()
{
    PopupJelly* pRet = new(std::nothrow) PopupJelly();
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

PopupJelly::PopupJelly(void) :
_typeSelected(E_JELLY::JELLY_TIME),
_timeElapse(0),

_onCallbackHeart(nullptr),

_uiContentsUIInfo(nullptr),
_uiContentsUIContentBuy(nullptr),
_uiContentsUIContentList(nullptr),
_uiContentsUIButton(nullptr),

_uiImgInfoBG(nullptr),
_uiImgInfoGummy(nullptr),
_uiTextInfo(nullptr),

_uiBtnAds(nullptr),
_uiBtnBuy(nullptr),
_uiBtnReward(nullptr),
_table(nullptr)
{
    
}

PopupJelly::~PopupJelly(void)
{
    
}

bool PopupJelly::init()
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

void PopupJelly::onEnter()
{
    PopupBase::onEnter();
    
    // network
    PopupLoading::show();
    PrisonManager::getInstance()->requestJellyInfo(CC_CALLBACK_1(PopupJelly::callbackInfo, this));
    
    // story
    StoryManager::getInstance()->showStory((int)E_COMIC::JELLY);
}

#pragma mark - init
void PopupJelly::initVar()
{
    //
    for ( int i = 0; i < sizeof(_listItemLevel) / sizeof(int); i++ )
    {
        _listItemLevel[i] = 0;
    }
}

void PopupJelly::initUi()
{
    Size size = Size(732, 1017);
    
    //
    _spriteContainer->setContentSize( size );
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(0);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_prison_jelly_name"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupJelly::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    {
        double width = 710;
        double heightUIInfo = 414;
        double heightUIButton = 110;
        double heightUIContent = size.height - 75 - heightUIInfo - heightUIButton - 25 - 20;
        
        
        _uiContentsUIInfo = ui::Layout::create();
        _uiContentsUIInfo->setContentSize(Size(width, heightUIInfo));
        _uiContentsUIInfo->setIgnoreAnchorPointForPosition(false);
        _uiContentsUIInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _uiContentsUIInfo->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
        _spriteContainer->addChild(_uiContentsUIInfo);
        
        _uiContentsUIButton = ui::Layout::create();
        _uiContentsUIButton->setContentSize(Size(width, heightUIButton));
        _uiContentsUIButton->setIgnoreAnchorPointForPosition(false);
        _uiContentsUIButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _uiContentsUIButton->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 25));
        _uiContentsUIButton->setLayoutType(ui::Layout::Type::HORIZONTAL);
        _spriteContainer->addChild(_uiContentsUIButton);
        
        _uiContentsUIContentBuy = ui::Layout::create();
        _uiContentsUIContentBuy->setContentSize(Size(width, heightUIContent));
        _uiContentsUIContentBuy->setIgnoreAnchorPointForPosition(false);
        _uiContentsUIContentBuy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _uiContentsUIContentBuy->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _uiContentsUIButton->getPositionY() + _uiContentsUIButton->getContentSize().height));
        _spriteContainer->addChild(_uiContentsUIContentBuy);
        
        _uiContentsUIContentList = ui::Layout::create();
        _uiContentsUIContentList->setContentSize(Size(width, heightUIContent));
        _uiContentsUIContentList->setIgnoreAnchorPointForPosition(false);
        _uiContentsUIContentList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _uiContentsUIContentList->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _uiContentsUIButton->getPositionY() + _uiContentsUIButton->getContentSize().height));
        _spriteContainer->addChild(_uiContentsUIContentList);
    }
    
    //
    uiInfo();
    uiContentBuy();
    uiContentList();
    
    uiButtons();
}

#pragma mark - table
void PopupJelly::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupJelly::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 125);
}

extension::TableViewCell* PopupJelly::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    
    E_JELLY listType[] = {E_JELLY::JELLY_TIME, E_JELLY::JELLY_DOUBLE, E_JELLY::JELLY_EXPAND};
    E_JELLY type = listType[idx];
    
    int level = _listItemLevel[type];
    int levelMax = DataManager::GetJellyMaxLevel(type);
    int effect = DataManager::GetJellyEffect(type, level);
    int effectDiff = DataManager::GetJellyEffect(type, level+1) - effect;
    
    
    //
    auto uiBG = ui::ImageView::create("Assets/ui_common/list_bg_s_3_1.png");
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(Size(size.width, 120));
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    uiBG->setCapInsets(Rect(30, 50, 10, 10));
    cell->addChild(uiBG);
     
    // button 150 * 116
    auto uiBtnBuy = ui::Button::create("Assets/ui_common/btn_weaponkeep_2.png");
    uiBtnBuy->setPosition(Vec2(uiBG->getContentSize().width - 10 - 75, uiBG->getContentSize().height / 2));
    uiBtnBuy->addClickEventListener(CC_CALLBACK_1(PopupJelly::onClickItemBuy, this));
    uiBtnBuy->setTag(type);
    uiBG->addChild(uiBtnBuy);
    if ( level >= levelMax )
    {
        uiBtnBuy->setVisible(false);
    }
    else
    {
        std::string strEffect = MafUtils::toString(effectDiff);
        if ( effectDiff > 0 )
        {
            strEffect = std::string("+").append(strEffect);
        }
        
        auto labelEffect = Label::createWithTTF(strEffect, GAME_FONT, 22);
        labelEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        labelEffect->setPosition(uiBtnBuy->getContentSize().width / 2, uiBtnBuy->getContentSize().height / 2 + 10);
        uiBtnBuy->addChild(labelEffect);
        
        std::string strIcon = "Assets/icon/icon_coin_2.png";
        if ( type == E_JELLY::JELLY_DOUBLE )
        {
            strIcon = "Assets/icon/evilcrystal.png";
        }
        auto spritIcon = Sprite::create(strIcon);
        spritIcon->setPosition(uiBtnBuy->getContentSize().width * 0.25, uiBtnBuy->getContentSize().height / 2 - 20);
        uiBtnBuy->addChild(spritIcon);
        
        int needCurrency = DataManager::GetJellyUpgrade(type, level);
        std::string strPrice = MafUtils::format("x %d", needCurrency);
        auto labelPrice = Label::createWithTTF(strPrice, GAME_FONT, 22);
        labelPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelPrice->setPosition(uiBtnBuy->getContentSize().width * 0.45, uiBtnBuy->getContentSize().height / 2 - 20);
        labelPrice->setColor(COLOR_COIN);
        uiBtnBuy->addChild(labelPrice);
        
        if ( type == E_JELLY::JELLY_TIME || type == E_JELLY::JELLY_EXPAND )
        {
            if ( std::atol(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str()) < needCurrency )
            {
                uiBtnBuy->loadTextureNormal("Assets/ui_common/btn_weaponkeep_off.png");
            }
        }
        else if ( type == E_JELLY::JELLY_DOUBLE )
        {
            if ( std::atol(ItemsManager::getInstance()->getItems(E_ITEMS::MATERIAL).c_str()) < needCurrency )
            {
                uiBtnBuy->loadTextureNormal("Assets/ui_common/btn_weaponkeep_off.png");
            }
        }
    }
    
    // content
    Size sizeContent = uiBG->getContentSize();
    if ( uiBtnBuy->isVisible() == true )
    {
        sizeContent.width = sizeContent.width - uiBtnBuy->getContentSize().width - 20;
    }
    auto uiContent = ui::Layout::create();
    uiContent->setContentSize(sizeContent);
    uiBG->addChild(uiContent);
    {
        auto uiImgTitleIcon = ui::ImageView::create(DataManager::GetJellyIcon(type));
        uiImgTitleIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiImgTitleIcon->setPosition(Vec2(80, uiContent->getContentSize().height - 19));
        uiContent->addChild(uiImgTitleIcon);
        
        auto uiTextTitle = ui::CText::create(DataManager::GetJellyName(type), GAME_FONT, 27);
        uiTextTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiTextTitle->setPosition(Vec2(80, uiContent->getContentSize().height - 19));
        uiTextTitle->setTextColor(Color4B(COLOR_TITLE));
        uiContent->addChild(uiTextTitle);
       
        //
        std::string strLevel = StringUtils::format("LV.%d(Max%d)", level, levelMax);
        if ( level >= levelMax )
        {
            strLevel = StringUtils::format("LV.%d(Max)", level);
        }
        
        auto uiTextLevel = ui::CText::create(strLevel, GAME_FONT, 20);
        uiTextLevel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiTextLevel->setPosition(Vec2(25, 75));
        uiContent->addChild(uiTextLevel);
        
        //
        auto uiTextInfo = ui::CText::create(DataManager::GetJellyExplain(type), GAME_FONT, 18);
        uiTextInfo->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiTextInfo->setPosition(Vec2(25, 45));
        uiTextInfo->setTextAreaSize(Size(sizeContent.width - 50, uiTextInfo->getContentSize().height));
        uiTextInfo->setTextOverflow(Label::Overflow::SHRINK);
        uiTextInfo->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiTextInfo->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiContent->addChild(uiTextInfo);

        //
        std::string strOption = GAME_TEXTFORMAT(DataManager::GetJellyOptionKey(type), effect);
        if ( type == E_JELLY::JELLY_DOUBLE && PrisonManager::getInstance()->isGummy() == true )
        {
            strOption.append("{#ff005a:(+10%)}");
        }
        
        auto uiTextOption = ui::CText::create(strOption, GAME_FONT, 18);
        uiTextOption->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiTextOption->setPosition(Vec2(25, 25));
        uiContent->addChild(uiTextOption);
 
        if ( type == E_JELLY::JELLY_DOUBLE && PrisonManager::getInstance()->isGummy() == true )
        {
            auto uiImgGummy = ui::ImageView::create("Assets/icon/icon_gummy58.png");
            uiImgGummy->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            uiImgGummy->setPosition(Vec2(uiTextOption->getPositionX() + uiTextOption->getContentSize().width, 25));
            uiImgGummy->setScale(0.5);
            uiContent->addChild(uiImgGummy);
        }
    }
    
    return cell;
}

ssize_t PopupJelly::numberOfCellsInTableView(extension::TableView *table)
{
    return 3;
}


#pragma mark - ui
void PopupJelly::uiInfo()
{
    auto layout = _uiContentsUIInfo;
    auto size = _uiContentsUIInfo->getContentSize();
    
    //
    _uiImgInfoBG = ui::ImageView::create("Assets/ui/prison/prison_bg_jelly_1.png");
    _uiImgInfoBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiImgInfoBG->setPosition(Vec2(size.width / 2, size.height));
    _uiImgInfoBG->setScale(3);
    ((ui::Scale9Sprite*)_uiImgInfoBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    layout->addChild(_uiImgInfoBG);
    
    //
    _uiImgInfoGummy = ui::ImageView::create("Assets/ui/prison/jelly/icon_gummy.png");
    _uiImgInfoGummy->setPosition(Vec2(40, size.height - 40));
    _uiImgInfoGummy->setVisible(false);
    _uiImgInfoGummy->setTouchEnabled(true);
    _uiImgInfoGummy->addClickEventListener([=](Ref* sender){
        
        //
        PopupToast::show(GAME_TEXT("t_ui_prison_gumi_info_toast"));
        
        //
        UserDefault::getInstance()->setStringForKey(KEY_JELLY_NEW, "1");
        _uiImgInfoGummy->removeChildByName("UIReddot");
    });
    layout->addChild(_uiImgInfoGummy);
    
    
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::TICKET);
    
    auto uiCurrency = UICurrency::create(list, Size(130, 50));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    uiCurrency->setPosition(Vec2(size.width, size.height - 246));
    layout->addChild(uiCurrency);
    
    //
    auto uiImgInfoBoard = ui::ImageView::create("Assets/ui_common/box_item.png");
    uiImgInfoBoard->setScale9Enabled(true);
    uiImgInfoBoard->setContentSize(Size(size.width, 168));
    uiImgInfoBoard->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiImgInfoBoard->setPosition(Vec2(size.width / 2, 0));
    layout->addChild(uiImgInfoBoard);
    {
        _uiTextInfo = ui::CText::create(GAME_TEXT("t_ui_prison_jelly_contents"), GAME_FONT, 36);
        _uiTextInfo->setPosition(Vec2(uiImgInfoBoard->getContentSize().width / 2, uiImgInfoBoard->getContentSize().height / 2));
        _uiTextInfo->setTextAreaSize(Size(650, 118));
        _uiTextInfo->setTextOverflow(Label::Overflow::SHRINK);
        _uiTextInfo->setTextHorizontalAlignment(TextHAlignment::LEFT);
        _uiTextInfo->setTextVerticalAlignment(TextVAlignment::CENTER);
        layout->addChild(_uiTextInfo);
    }
    
    // draw
    drawInfo();
}

void PopupJelly::uiContentBuy()
{
    auto layout = _uiContentsUIContentBuy;
    auto size = _uiContentsUIContentBuy->getContentSize();
    
    auto uiImgBG = ui::ImageView::create("Assets/ui_common/list_bg_s_3_1.png");
    uiImgBG->setScale9Enabled(true);
    uiImgBG->setContentSize(Size(710, 300));
    uiImgBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiImgBG->setPosition(Vec2(layout->getContentSize().width / 2, layout->getContentSize().height));
    uiImgBG->setCapInsets(Rect(30, 50, 10, 10));
    layout->addChild(uiImgBG);
    
    
    {
        auto uiImgIcon = ui::ImageView::create("Assets/icon/icon_jelly58.png");
        uiImgIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiImgIcon->setPosition(Vec2(80, uiImgBG->getContentSize().height - 19));
        uiImgBG->addChild(uiImgIcon);
        
        auto uiTextName = ui::CText::create(GAME_TEXT("t_ui_prison_jelly_msg8"), GAME_FONT, 28);
        uiTextName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiTextName->setPosition(Vec2(100, uiImgBG->getContentSize().height - 19));
        uiTextName->setTextColor(Color4B(COLOR_TITLE));
        uiImgBG->addChild(uiTextName);
        
        auto uiTextDetail = ui::CText::create(GAME_TEXT("t_ui_prison_jelly_msg10"), GAME_FONT, 28);
        uiTextDetail->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiTextDetail->setPosition(Vec2(20, 131));
        uiTextDetail->setTextAreaSize(Size(650, 200));
        uiTextDetail->setTextOverflow(Label::Overflow::SHRINK);
        uiTextDetail->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiTextDetail->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiImgBG->addChild(uiTextDetail);
    }
}

void PopupJelly::uiContentList()
{
    auto layout = _uiContentsUIContentList;
    auto size = _uiContentsUIContentList->getContentSize();
    
    _table = extension::TableView::create(this, layout->getContentSize());
    _table->setDirection(extension::ScrollView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition( Vec2(size.width / 2, 0) );
    _table->setDelegate(this);
    _table->setBounceable(false);
    layout->addChild(_table);
}

void PopupJelly::uiButtons()
{
    auto layout = _uiContentsUIButton;
    auto size = _uiContentsUIButton->getContentSize();
    
    {
        _uiBtnAds = ui::Button::create("Assets/ui_common/btn_1_7_on.png");
        _uiBtnAds->setScale9Enabled(true);
        _uiBtnAds->setContentSize(Size(214, 110));
        _uiBtnAds->addClickEventListener(CC_CALLBACK_1(PopupJelly::onClickAds, this));
        layout->addChild(_uiBtnAds);
        {
            auto uiImgIcon = ui::ImageView::create("Assets/ui/prison/icon_freepass.png");
            uiImgIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiImgIcon->setPosition(Vec2(_uiBtnAds->getContentSize().width / 2, _uiBtnAds->getContentSize().height * 0.70));
            _uiBtnAds->addChild(uiImgIcon);
            
            auto uiTextCount = ui::CText::create("x2", GAME_FONT, 28);
            uiTextCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiTextCount->setPosition(Vec2(_uiBtnAds->getContentSize().width / 2 + 10, _uiBtnAds->getContentSize().height * 0.70));
            _uiBtnAds->addChild(uiTextCount);
            
            auto uiImgAds = ui::ImageView::create("Assets/ui_common/btn_ads_play.png");
            uiImgAds->setPosition(Vec2(_uiBtnAds->getContentSize().width / 2, _uiBtnAds->getContentSize().height * 0.25));
            _uiBtnAds->addChild(uiImgAds);
        }
    }
    
    {
        _uiBtnBuy = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
        _uiBtnBuy->setScale9Enabled(true);
        _uiBtnBuy->setContentSize(Size(214, 110));
        _uiBtnBuy->addClickEventListener(CC_CALLBACK_1(PopupJelly::onClickJellyBuy, this));
        layout->addChild(_uiBtnBuy);
        {
            auto uiImgIcon = ui::ImageView::create("Assets/icon/icon_coin_2.png");
            uiImgIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiImgIcon->setPosition(Vec2(_uiBtnBuy->getContentSize().width / 2 - 10, _uiBtnBuy->getContentSize().height * 0.70));
            _uiBtnBuy->addChild(uiImgIcon);
            
            auto uiTextCount = ui::CText::create(MafUtils::toString(PrisonManager::getInstance()->getJellyPrice()), GAME_FONT, 28);
            uiTextCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiTextCount->setPosition(Vec2(_uiBtnBuy->getContentSize().width / 2 - 10, _uiBtnBuy->getContentSize().height * 0.70));
            uiTextCount->setTextColor(Color4B(COLOR_COIN));
            _uiBtnBuy->addChild(uiTextCount);
            
            auto uiTextName = ui::CText::create(GAME_TEXT("t_ui_prison_jelly_msg6"), GAME_FONT, 24);
            uiTextName->setPosition(Vec2(_uiBtnBuy->getContentSize().width / 2, _uiBtnBuy->getContentSize().height * 0.25));
            _uiBtnBuy->addChild(uiTextName);
        }
        
        auto params = ui::LinearLayoutParameter::create();
        params->setMargin(ui::Margin(10, 0, 0, 0));
        _uiBtnBuy->setLayoutParameter(params);
    }
    
    {
        _uiBtnReward = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
        _uiBtnReward->setScale9Enabled(true);
        _uiBtnReward->setContentSize(Size(214, 110));
        _uiBtnReward->addClickEventListener(CC_CALLBACK_1(PopupJelly::onClickReward, this));
        layout->addChild(_uiBtnReward);
        {
            auto uiImgIcon = ui::ImageView::create("Assets/ui/prison/icon_freepass.png");
            uiImgIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiImgIcon->setPosition(Vec2(_uiBtnReward->getContentSize().width / 2, _uiBtnReward->getContentSize().height * 0.70));
            _uiBtnReward->addChild(uiImgIcon);
            
            auto uiTextTime = ui::CText::create("00:00", GAME_FONT, 18);
            uiTextTime->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            uiTextTime->setPosition(Vec2(_uiBtnReward->getContentSize().width / 2 + 10, _uiBtnReward->getContentSize().height * 0.70));
            uiTextTime->setTextColor(Color4B(COLOR_TITLE));
            uiTextTime->setName("UI_TEXT_TIME");
            _uiBtnReward->addChild(uiTextTime);
            
            auto uiTextCount = ui::CText::create("0/0", GAME_FONT, 24);
            uiTextCount->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            uiTextCount->setPosition(Vec2(_uiBtnReward->getContentSize().width / 2 + 10, _uiBtnReward->getContentSize().height * 0.70 + 10));
            uiTextCount->setName("UI_TEXT_COUNT");
            _uiBtnReward->addChild(uiTextCount);
            
            auto uiTextName = ui::CText::create(GAME_TEXT("t_ui_prison_jelly_reward_bt"), GAME_FONT, 24);
            uiTextName->setPosition(Vec2(_uiBtnReward->getContentSize().width / 2, _uiBtnReward->getContentSize().height * 0.25));
            _uiBtnReward->addChild(uiTextName);
        }
        
        auto params = ui::LinearLayoutParameter::create();
        params->setMargin(ui::Margin(10, 0, 0, 0));
        _uiBtnReward->setLayoutParameter(params);
    }
    
    // draw
    drawButtons();
}

#pragma mark - ui draw
void PopupJelly::drawInfo()
{
    auto layout = _uiContentsUIInfo;
    
    std::string pathBG = "Assets/ui/prison/prison_bg_jelly_1.png";
    if ( PrisonManager::getInstance()->isGummy() )
    {
        pathBG = "Assets/ui/prison/prison_bg_jelly_2.png";
    }
    _uiImgInfoBG->loadTexture(pathBG);
    ((ui::Scale9Sprite*)_uiImgInfoBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    
    //
    if ( PrisonManager::getInstance()->isGummy() )
    {
        _uiImgInfoGummy->setVisible(true);

        std::string jellyNew = UserDefault::getInstance()->getStringForKey(KEY_JELLY_NEW);
        if ( jellyNew.empty() == true )
        {
            auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_RIGHT);
            uiReddot->setPosition(_uiImgInfoGummy->getContentSize().width, _uiImgInfoGummy->getContentSize().height + 10);
            _uiImgInfoGummy->addChild(uiReddot);
        }
    }
    else
    {
        _uiImgInfoGummy->setVisible(false);
    }
    
    // text
    if ( PrisonManager::getInstance()->isJelly() == true )
    {
        _uiTextInfo->setString(GAME_TEXT("t_ui_prison_jelly_contents"));
    }
    else
    {
        _uiTextInfo->setString(GAME_TEXT("t_ui_prison_jelly_msg7"));
    }
    _uiTextInfo->setTextAreaSize(Size(650, 118));
    _uiTextInfo->setTextOverflow(Label::Overflow::SHRINK);
}

void PopupJelly::drawButtons()
{
    auto layout = _uiContentsUIButton;
    
    if ( PrisonManager::getInstance()->isJelly() == true )
    {
        _uiBtnBuy->setVisible(false);
        _uiBtnReward->setVisible(true);
    }
    else
    {
        _uiBtnBuy->setVisible(true);
        _uiBtnReward->setVisible(false);
    }
    
    // ads
    {
        if ( PrisonManager::getInstance()->isAds() == false )
        {
            _uiBtnAds->loadTextureNormal("Assets/ui_common/btn_1_off.png");
        }
        else
        {
            _uiBtnAds->loadTextureNormal("Assets/ui_common/btn_1_7_on.png");
        }
    }
    
    // reward
    {
        int level = _listItemLevel[E_JELLY::JELLY_TIME];
        
        int rewardNow = PrisonManager::getInstance()->getJellyReward();
        int rewardMax = PrisonManager::getInstance()->getJellyRewardMax();
        int rewardTimeMax = DataManager::GetJellyEffect(E_JELLY::JELLY_TIME, level);
        
        auto uiTextCount = _uiBtnReward->getChildByName<ui::CText*>("UI_TEXT_COUNT");
        uiTextCount->setString(MafUtils::format("%d/%d", rewardNow, rewardMax));
        
        auto uiTextTime = _uiBtnReward->getChildByName<ui::CText*>("UI_TEXT_TIME");
        
        uiTextTime->stopAllActions();
        uiTextTime->setString(MafUtils::format("%02d:%02d", rewardTimeMax/60, rewardTimeMax%60));
        if ( rewardNow < rewardMax )
        {
            auto action1 = CallFuncN::create([=](Node* sender){
            
                auto uiTextTime = (ui::CText*)sender;
                
                //
                int64_t timeElapseDiff = UtilsDate::getInstance()->getTime() - _timeElapse;
                int64_t rewardTime = PrisonManager::getInstance()->getJellyRewardTime() + timeElapseDiff;
                int rewardTimeMax = DataManager::GetJellyEffect(E_JELLY::JELLY_TIME, level);
                
                int timeResult = rewardTimeMax - (rewardTime % rewardTimeMax);
                if ( timeResult <= 1 )
                {
                    timeResult = rewardTimeMax;
                    
                    uiTextTime->stopAllActions();
                    
                    // network
                    PopupLoading::show();
                    PrisonManager::getInstance()->requestJellyInfo(CC_CALLBACK_1(PopupJelly::callbackInfo, this));
                }

                uiTextTime->setString(MafUtils::format("%02d:%02d", timeResult/60, timeResult%60));
            });
            auto action2 = DelayTime::create(1.0);
            auto seq = Sequence::create(action1, action2, NULL);
            uiTextTime->runAction(RepeatForever::create(seq));
        }
    }
    
    //
    auto size = UtilsWidget::getInstance()->getLayoutAccumulatedSize(layout);
    layout->setContentSize(size);
    layout->setPositionX(layout->getParent()->getContentSize().width / 2);
    layout->requestDoLayout();
}

void PopupJelly::drawContentList()
{
    Vec2 pos = _table->getContentOffset();
    _table->reloadData();
    _table->setContentOffset(pos);
}

#pragma mark - set, get, other
void PopupJelly::setCallbackHeart(const std::function<void(void)>& callback)
{
    _onCallbackHeart = callback;
}

#pragma mark - click
void PopupJelly::onClickClose(Ref* sender)
{
    hide();
}

void PopupJelly::onClickJellyBuy(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    if ( std::atol(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str()) < PrisonManager::getInstance()->getJellyPrice() )
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::COSTUME_COIN);
        popup->show();
        return;
    }
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_msg_buy"));
    popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_yes"), false, [=](Ref* sender){
        
        // network
        requestJellyBuy();
    });
    popup->show();
}

void PopupJelly::onClickItemBuy(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto item = (MafNode::MafMenuItemSprite*)sender;
    _typeSelected = (E_JELLY)item->getTag();
    
    int level = _listItemLevel[_typeSelected];
    int levelMax = DataManager::GetJellyMaxLevel(_typeSelected);
    if ( level >= levelMax )
    {
        return;
    }
    
    auto needCurrency = DataManager::GetJellyUpgrade(_typeSelected, level);
    if ( _typeSelected == E_JELLY::JELLY_TIME || _typeSelected == E_JELLY::JELLY_EXPAND )
    {
        if ( std::atol(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str()) < needCurrency )
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::COSTUME_COIN);
            popup->show();
            return;
        }
    }
    else if ( _typeSelected == E_JELLY::JELLY_DOUBLE )
    {
        if ( std::atol(ItemsManager::getInstance()->getItems(E_ITEMS::MATERIAL).c_str()) < needCurrency )
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::MATERIAL);
            popup->show();
            return;
        }
    }
    else
    {
        return;
    }

    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_msg_buy"));
    popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_yes"), false, [=](Ref* sender){
        
        // network
        requestItemBuy();
    });
    popup->show();
}

void PopupJelly::onClickReward(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_jelly_msg5"));
    popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_yes"), false, [=](Ref* sender){
        
        // network
        requestReward();
    });
    popup->show();
}

void PopupJelly::onClickAds(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    if ( PrisonManager::getInstance()->isAds() == false )
    {
        return;
    }
    
    //
    if ( PrisonManager::getInstance()->getResetTime() <= 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_error_reset"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){

            PrisonManager::getInstance()->requestInfo(true, (int)PrisonManager::getInstance()->getPrisonType(), nullptr);
            
        });
        popup->setHideBackKey(false);
        popup->show();
        
        hide();
        return;
    }
    
    //
    if ( CashManager::getInstance()->isAdsfreeTime() == true )
    {
        callbackAds(ADS_RESULT::SUCCESS);
    }
    else
    {
        auto popup = PopupAdsWatch::create(E_ADS_WATCH::ITEM_TICKET, 2);
        popup->setCallbackResult([=](){
            MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupJelly::callbackAds, this));
        });
        popup->show();
    }
}

#pragma mark - network
void PopupJelly::callbackInfo(bool bResult)
{
    //
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        hide();
        return;
    }
    
    if ( PrisonManager::getInstance()->isJelly() == true )
    {
        _timeElapse = UtilsDate::getInstance()->getTime();
        
        _listItemLevel[E_JELLY::JELLY_TIME] = PrisonManager::getInstance()->getJellyLvTime();
        _listItemLevel[E_JELLY::JELLY_DOUBLE] = PrisonManager::getInstance()->getJellyLvDouble();
        _listItemLevel[E_JELLY::JELLY_EXPAND] = PrisonManager::getInstance()->getJellyLvExpand();
        
        
        
        _uiContentsUIContentBuy->setVisible(false);
        _uiContentsUIContentList->setVisible(true);
        
        // draw
        drawContentList();
    }
    else
    {
        _uiContentsUIContentBuy->setVisible(true);
        _uiContentsUIContentList->setVisible(false);
    }
    
    // draw
    drawInfo();
    drawButtons();
    
}

void PopupJelly::callbackAds(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotPrisonFree, CashManager::getInstance()->isAdsfreeTime());
        
        //
        PopupLoading::show();
        PrisonManager::getInstance()->requestAds(CC_CALLBACK_1(PopupJelly::callbackAdsTicket, this));
        
        // 지하감옥 무료 입장 새로고침
        if ( _onCallbackHeart != nullptr )
        {
            _onCallbackHeart();
        }
    }
    else
    {
        if ( result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(PopupJelly::callbackAds, this));
            popup->setLogParams(kAnalEventAdsLoadParameterSlot);
            popup->setLogValue(kAdsSlotPrisonFree);
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

void PopupJelly::callbackAdsTicket(bool bResult)
{
    //
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        return;
    }
    
    // draw
    drawInfo();
    drawButtons();
}

#pragma mark - network
void PopupJelly::requestJellyBuy()
{
    // network
    PopupLoading::show();
    
    std::string url = "/jelly/buy";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseJellyBuy(response,dd);
    };
    req->send(cb);
}

void PopupJelly::responseJellyBuy(network::HttpResponse* response,std::string &data)
{
    PopupLoading::hide();
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == false && jsonParser.HasMember("_coin") == false )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        return;
    }
    
    int nCoin = jsonParser["_coin"].GetInt();
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, nCoin);
    
    // ui
    PopupLoading::show();
    PrisonManager::getInstance()->requestJellyInfo(CC_CALLBACK_1(PopupJelly::callbackInfo, this));
}

void PopupJelly::requestItemBuy()
{
    // network
    PopupLoading::show();
    
    std::string url = "/jelly/skill";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_skillType", _typeSelected);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseItemBuy(response,dd);
    };
    req->send(cb);
}

void PopupJelly::responseItemBuy(network::HttpResponse* response,std::string &data)
{
    PopupLoading::hide();
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        return;
    }

    if ( jsonParser.HasMember("_coin") == true )
    {
        int nCoin = jsonParser["_coin"].GetInt();
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, nCoin);
    }
    
    if ( jsonParser.HasMember("_material") == true )
    {
        int nBong = jsonParser["_material"].GetInt();
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::MATERIAL, 0, nBong);
    }
    
    // network
    PopupLoading::show();
    PrisonManager::getInstance()->requestJellyInfo(CC_CALLBACK_1(PopupJelly::callbackInfo, this));
}

void PopupJelly::requestReward()
{
    // network
    PopupLoading::show();
    
    std::string url = "/jelly/explore/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseReward(response,dd);
    };
    req->send(cb);
}

void PopupJelly::responseReward(network::HttpResponse* response,std::string &data)
{
    PopupLoading::hide();
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == false && (jsonParser.HasMember("_new") == false || jsonParser.HasMember("_heart") == false) )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        return;
    }
    
    int nHeartNew = jsonParser["_new"].GetInt();
    int nHeart = jsonParser["_heart"].GetInt();

    if ( nHeartNew > 0 )
    {
        // network
        PopupLoading::show();
        PrisonManager::getInstance()->requestJellyInfo(CC_CALLBACK_1(PopupJelly::callbackInfo, this));
        
        // 지하감옥 무료 입장 새로고침
        if ( _onCallbackHeart != nullptr )
        {
            _onCallbackHeart();
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_msg_get_5", nHeartNew));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        //
        DataManager::getInstance()->requestPushInfo("jelly");
    }
    
}
