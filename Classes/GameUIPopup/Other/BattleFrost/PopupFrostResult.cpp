//
//  PopupFrostResult.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "PopupFrostResult.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIBuff.h"
#include "GameUI/UI/UIReddot.h"

#include "ManagerGame/PrisonFrostManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupFrostResult* PopupFrostResult::create()
{
    PopupFrostResult* pRet = new(std::nothrow) PopupFrostResult();
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

PopupFrostResult::PopupFrostResult() :
_bInitUI(false),

_bResult(false),
_reward(""),

// widget
_uiContentsUIInfo(nullptr),
_uiContentsUIButtons(nullptr)
{
    
}

PopupFrostResult::~PopupFrostResult(void)
{
    
}

bool PopupFrostResult::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    setName("PopupFrostResult");
    
    //
    initVar();
    initUI();
    
    // event
    
    return true;
}

void PopupFrostResult::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiInfo();
        uiButtons();
    }
    
    //
    drawInfo();
    drawButtons();
}

#pragma mark - init
void PopupFrostResult::initVar()
{
    
}

void PopupFrostResult::initUI()
{
    // size
    Size size = Size(520, 570);

    _spriteContainer->setContentSize(size);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_prison_result"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
}

#pragma mark - ui
void PopupFrostResult::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 14;
    size.height = size.height - 75;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 0));
    _spriteContainer->addChild(layout);
    
    //
    _uiContentsUIButtons = ui::Layout::create();
    _uiContentsUIButtons->setContentSize( Size(426, 128) );
    _uiContentsUIButtons->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsUIButtons->setPosition( Vec2(size.width / 2, 0) );
    layout->addChild(_uiContentsUIButtons);
    
    _uiContentsUIInfo = ui::Layout::create();
    _uiContentsUIInfo->setContentSize( Size(426, size.height - _uiContentsUIButtons->getContentSize().height ) );
    _uiContentsUIInfo->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContentsUIInfo->setPosition( Vec2(size.width / 2, size.height) );
    layout->addChild(_uiContentsUIInfo);
}

void PopupFrostResult::uiInfo()
{
    auto layout = _uiContentsUIInfo;
    Size size = layout->getContentSize();
    
    //
    auto uiBG = ui::ImageView::create("Assets/ui/frost_prison/frost_prison_reward_success_img.png");
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiBG->setPosition(Vec2(size.width / 2, size.height - 25));
    uiBG->setScale(3);
    uiBG->setName("UI_BG");
    layout->addChild(uiBG);
    
    //
    auto uiReward = ui::Layout::create();
    uiReward->setContentSize(Size(size.width, 130));
    uiReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiReward->setPosition(Vec2(size.width / 2, 0));
    uiReward->setName("UI_REWARD");
    layout->addChild(uiReward);
}

void PopupFrostResult::uiButtons()
{
    auto layout = _uiContentsUIButtons;
    Size size = layout->getContentSize();
    
    auto uiBtnConfirm = ui::Button::create("Assets/ui_common/btn_ok.png");
    uiBtnConfirm->addClickEventListener(CC_CALLBACK_1(PopupFrostResult::onClickClose, this));
    layout->addChild(uiBtnConfirm);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_confirm"), GAME_FONT, 28);
        uiText->setPosition(Vec2(uiBtnConfirm->getContentSize().width / 2, uiBtnConfirm->getContentSize().height / 2));
        uiBtnConfirm->addChild(uiText);
    }
     
}
          
#pragma mark - draw
void PopupFrostResult::drawInfo()
{
    auto layout = _uiContentsUIInfo;
    
    //
    auto uiBG = layout->getChildByName<ui::ImageView*>("UI_BG");
    if ( _bResult == true )
    {
        uiBG->loadTexture("Assets/ui/frost_prison/frost_prison_reward_success_img.png");
    }
    else
    {
        uiBG->loadTexture("Assets/ui/frost_prison/frost_prison_reward_fail_img.png");
    }
    
    //
    auto uiReward = layout->getChildByName<ui::Layout*>("UI_REWARD");
    uiReward->removeAllChildren();
    if ( _bResult == true )
    {
        auto uiItems = ui::Layout::create();
        uiItems->setContentSize(uiReward->getContentSize());
        uiItems->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiItems->setPosition(Vec2(uiReward->getContentSize().width / 2, uiReward->getContentSize().height / 2 + 10));
        uiItems->setLayoutType(ui::Layout::Type::HORIZONTAL);
        uiReward->addChild(uiItems);
        
        auto listReward = ItemsManager::getInstance()->getConvertArray(_reward);
        for ( int i = 0; i < listReward.size(); i++ )
        {
            auto objItems = listReward.at(i);
            
            auto uiItemBG = ui::ImageView::create("Assets/ui_common/box_item.png");
            uiItemBG->setScale9Enabled(true);
            uiItemBG->setContentSize(Size(90, 90));
            uiItems->addChild(uiItemBG);
            {
                auto params = ui::LinearLayoutParameter::create();
                params->setMargin(ui::Margin(10, 0, 0, 0));
                uiItemBG->setLayoutParameter(params);
            }
            {
                auto uiItemIcon = ui::ImageView::create(objItems->getPath());
                uiItemIcon->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2 + 5));
                uiItemBG->addChild(uiItemIcon);

                std::string rewardCount = objItems->getCount();
                std::string rewardCountBonus = objItems->getCount();

                if ( objItems->isString() == true )
                {
                    MafUtils::convertNumberToShort(rewardCount);
                    MafUtils::convertNumberToShort(rewardCountBonus);
                }

                auto uiTextReward = ui::CText::create(rewardCount, GAME_FONT, 24);
                uiTextReward->setPosition(Vec2(uiItemBG->getContentSize().width / 2, -10));
                uiTextReward->enableOutline(Color4B::BLACK, 2);
                uiItemBG->addChild(uiTextReward);
            }
        }
        
        auto sizeReward = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiItems);
        uiItems->setContentSize(sizeReward);
        uiItems->requestDoLayout();
    }
    else
    {
        std::string hp = "0";
        
        int clearFloor = PrisonFrostManager::getInstance()->getClearFloor() + 1;
        auto objFloor = PrisonFrostManager::getInstance()->getInfoFloor(clearFloor);
        if ( objFloor != nullptr )
        {
            hp = objFloor->getHP();
            hp = MafUtils::convertNumberToShort(hp);
        }
        
        auto uiIcon = ui::ImageView::create("Assets/ui/frost_prison/mamon_transform_ani01.png");
        uiIcon->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiIcon->setPosition(Vec2(-135, -60));
        uiIcon->setScale(3);
        uiReward->addChild(uiIcon);
        
        auto uiBubble = ui::ImageView::create("Assets/ui/frost_prison/reward_speech_bubble.png");
        uiBubble->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiBubble->setPosition(Vec2(93, uiReward->getContentSize().height / 2));
        uiReward->addChild(uiBubble);
        {
            auto uiText = ui::CText::create(GAME_TEXTFORMAT("t_ui_frost_prison_small_talk_fail", hp.c_str()), GAME_FONT, 28);
            uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiText->setPosition(Vec2(36, uiBubble->getContentSize().height - 55));
            uiText->setTextAreaSize(Size(309, 104));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::LEFT);
            uiText->setTextColor(Color4B::BLACK);
            uiBubble->addChild(uiText);
            
        }
    }

}

void PopupFrostResult::drawButtons()
{
    auto layout = _uiContentsUIButtons;
    Size size = layout->getContentSize();
    
    // align
    double padding = 10;
    double widthButtons = -padding;
    for ( auto& uiBtn : layout->getChildren() )
    {
        widthButtons += uiBtn->getContentSize().width * uiBtn->getScaleX() + padding;
    }
    
    float posX = (size.width / 2.0f) - (widthButtons / 2.0f);
    float posY = size.height / 2.0f;
    for (const auto &child : layout->getChildren())
    {
        child->setPosition(posX + child->getContentSize().width * child->getScaleX() / 2.0f, posY);
        posX += child->getContentSize().width * child->getScaleX() + padding;
    }
}
 
#pragma mark - ui : set, get
void PopupFrostResult::setResult(bool bResult, std::string reward/* = ""*/)
{
    _bResult = bResult;
    _reward = reward;
}

#pragma mark - callback

#pragma mark - click
void PopupFrostResult::onClickClose(Ref* sender)
{
    hide();
}

#pragma mark - game
