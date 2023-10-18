//
//  PopupFlashmanRankReward.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "PopupFlashmanRankReward.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIBuff.h"
#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupFlashmanRankReward* PopupFlashmanRankReward::create()
{
    PopupFlashmanRankReward* pRet = new(std::nothrow) PopupFlashmanRankReward();
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

PopupFlashmanRankReward::PopupFlashmanRankReward() :
_bInitUI(false),

//
_rank(0),

// widget
_uiContentsUIDefault(nullptr),
_uiContentsUIButtons(nullptr)
{
    
}

PopupFlashmanRankReward::~PopupFlashmanRankReward(void)
{
    
}

bool PopupFlashmanRankReward::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    setName("PopupFlashmanRankReward");
    
    //
    initVar();
    initUI();
    
    return true;
}

void PopupFlashmanRankReward::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiDefault();
        uiButtons();
    }
    
    //
    drawDefault();
    drawButtons();
}

#pragma mark - init
void PopupFlashmanRankReward::initVar()
{
    
}

void PopupFlashmanRankReward::initUI()
{
    // size
    Size size = Size(500, 444);

    _spriteContainer->setContentSize( size );
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_raidreward_icon"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupFlashmanRankReward::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 37.5);
    menu->addChild(itemClose);
}

#pragma mark - ui
void PopupFlashmanRankReward::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 0));
    _spriteContainer->addChild(layout);
    
    //
    _uiContentsUIButtons = ui::Layout::create();
    _uiContentsUIButtons->setContentSize( Size(size.width, 78) );
    _uiContentsUIButtons->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsUIButtons->setPosition( Vec2(size.width / 2, 25) );
    _uiContentsUIButtons->setLayoutType(ui::Layout::Type::HORIZONTAL);
    layout->addChild(_uiContentsUIButtons);
    
    _uiContentsUIDefault = ui::Layout::create();
    _uiContentsUIDefault->setContentSize( Size(size.width, size.height - 75 - 25 - _uiContentsUIButtons->getContentSize().height - 25) );
    _uiContentsUIDefault->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIDefault->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsUIDefault->setPosition( Vec2(size.width / 2, size.height - 75) );
    layout->addChild(_uiContentsUIDefault);
}

void PopupFlashmanRankReward::uiDefault()
{
    auto layout = _uiContentsUIDefault;
    Size size = layout->getContentSize();
    
    auto uiTitleBG = ui::ImageView::create("Assets/ui/race_event/race_reward_title.png");
    uiTitleBG->setScale9Enabled(true);
    uiTitleBG->setCapInsets(Rect(67, 0, 22, 40));
    uiTitleBG->setContentSize(Size(size.width - 40, uiTitleBG->getContentSize().height));
    uiTitleBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiTitleBG->setPosition(Vec2(size.width / 2, size.height - 25));
    layout->addChild(uiTitleBG);
    {
        auto uiRank = ui::CText::create(GAME_TEXTFORMAT("t_ui_rank_now", 0), GAME_FONT, 24);
        uiRank->setPosition(Vec2(uiTitleBG->getContentSize().width / 2, uiTitleBG->getContentSize().height / 2));
        uiRank->setTextAreaSize(Size(uiTitleBG->getContentSize().width - 10, uiRank->getContentSize().height));
        uiRank->setTextOverflow(Label::Overflow::SHRINK);
        uiRank->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiRank->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiRank->setName("UI_RANK");
        uiTitleBG->addChild(uiRank);
    }
    
    // reward
    auto uiLayoutReward = ui::Layout::create();
    uiLayoutReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayoutReward->setPosition(Vec2(size.width / 2, 110));
    uiLayoutReward->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiLayoutReward->setName("UI_REWARD");
    layout->addChild(uiLayoutReward);
    
    //
    auto uiText = ui::CText::create(GAME_TEXT("t_ui_randombox_msg_4"), GAME_FONT, 24);
    uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiText->setPosition(Vec2(size.width / 2, 0));
    uiText->setTextAreaSize(Size(size.width - 10, uiText->getContentSize().height));
    uiText->setTextOverflow(Label::Overflow::SHRINK);
    uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    layout->addChild(uiText);
}

void PopupFlashmanRankReward::uiButtons()
{
    auto layout = _uiContentsUIButtons;
    Size size = layout->getContentSize();
    
    auto uiBtn = ui::Button::create("Assets/ui_common/btn_ok.png");
    uiBtn->addClickEventListener(CC_CALLBACK_1(PopupFlashmanRankReward::onClickClose, this));
    layout->addChild(uiBtn);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_confirm"), GAME_FONT, 30);
        uiText->setPosition(Vec2(uiBtn->getContentSize().width / 2, uiBtn->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(uiBtn->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiBtn->addChild(uiText);
    }
}

#pragma mark - draw
void PopupFlashmanRankReward::drawDefault()
{
    auto layout = _uiContentsUIDefault;
    
    
    if ( auto uiRank = utils::findChild<ui::CText*>(layout, "UI_RANK"); uiRank != nullptr )
    {
        uiRank->setString(GAME_TEXTFORMAT("t_ui_rank_now", _rank));
    }
    
    //
    if ( auto uiReward = layout->getChildByName<ui::Layout*>("UI_REWARD"); uiReward != nullptr )
    {
        for ( int i = 0; i < _listReward.size(); i++ )
        {
            auto objItems = _listReward.at(i);

            auto uiRewardBG = ui::ImageView::create("Assets/ui_common/box_reward.png");
            uiReward->addChild(uiRewardBG);
            {
                if ( i != 0 )
                {
                    auto params = ui::LinearLayoutParameter::create();
                    params->setMargin(ui::Margin(10, 0, 0, 0));
                    uiRewardBG->setLayoutParameter(params);
                }

                auto uiImgItemIcon = ui::ImageView::create(objItems->getPath());
                uiImgItemIcon->setPosition(Vec2(uiRewardBG->getContentSize().width / 2, uiRewardBG->getContentSize().height / 2 + 10));
                uiRewardBG->addChild(uiImgItemIcon);

                std::string rewardCount = objItems->getCount();
                if ( objItems->isString() == true )
                {
                    MafUtils::convertNumberToShort(rewardCount);
                }
                rewardCount.insert(0, "x");

                auto uiTextReward = ui::CText::create(rewardCount, GAME_FONT, 24);
                uiTextReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiTextReward->setPosition(Vec2(uiRewardBG->getContentSize().width / 2, 10));
                uiTextReward->enableOutline(Color4B::BLACK, 2);
                uiRewardBG->addChild(uiTextReward);
            }
        }
    
        auto sizeReward = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiReward);
        uiReward->setContentSize(sizeReward);
        uiReward->requestDoLayout();
    }
    
}

void PopupFlashmanRankReward::drawButtons()
{
    auto layout = _uiContentsUIButtons;
    
   
    //
    auto size = UtilsWidget::getInstance()->getLayoutAccumulatedSize(layout);
    layout->setContentSize(size);
    layout->requestDoLayout();
}

 
#pragma mark - set, get
void PopupFlashmanRankReward::setRank(int rank)
{
    _rank = rank;
    
    if ( _bInitUI == true )
    {
        drawDefault();
        drawButtons();
    }
}

void PopupFlashmanRankReward::setRewrad(std::string reward)
{
    _listReward.clear();
    _listReward = ItemsManager::getInstance()->getConvertArray(reward);
    
    if ( _bInitUI == true )
    {
        drawDefault();
        drawButtons();
    }
}

#pragma mark - callback

#pragma mark - schedule

#pragma mark - click
void PopupFlashmanRankReward::onClickClose(Ref* sender)
{
    hide();
}
