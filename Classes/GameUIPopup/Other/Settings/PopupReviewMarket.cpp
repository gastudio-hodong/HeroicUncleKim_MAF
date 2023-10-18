//
//  PopupReviewMarket.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupReviewMarket.h"

#include "Common/ConfigKey.h"

USING_NS_CC;

PopupReviewMarket* PopupReviewMarket::create()
{
    PopupReviewMarket* pRet = new(std::nothrow) PopupReviewMarket();
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

PopupReviewMarket::PopupReviewMarket(void) :

_layerContainerContent(nullptr)
{
    
}

PopupReviewMarket::~PopupReviewMarket(void)
{
    
}

bool PopupReviewMarket::init(void)
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

#pragma mark - init
void PopupReviewMarket::initVar()
{

}

void PopupReviewMarket::initUi()
{
    Size size = Size(497, 350);
    
    //
    _spriteContainer->setContentSize( size );
    
    //
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_recommend"), GAME_FONT, 32);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    labelTitle->setColor(COLOR_TITLE);
    _spriteContainer->addChild(labelTitle);
    
    uiContent();
}

#pragma mark - ui
void PopupReviewMarket::uiContent()
{
    if ( _layerContainerContent == nullptr )
    {
        _layerContainerContent = Layer::create();
        _layerContainerContent->setContentSize( Size(_spriteContainer->getContentSize().width, _spriteContainer->getContentSize().height  - 75) );
        _layerContainerContent->setIgnoreAnchorPointForPosition(false);
        _layerContainerContent->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
        _layerContainerContent->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75) );
        _spriteContainer->addChild(_layerContainerContent);
    }
    _layerContainerContent->removeAllChildren();
    
    auto labelMsg = Label::createWithTTF(GAME_TEXT("t_ui_review_market_contents_1"), GAME_FONT, 24);
    labelMsg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelMsg->setPosition(_layerContainerContent->getContentSize().width / 2, _layerContainerContent->getContentSize().height - 20);
    labelMsg->setDimensions(_layerContainerContent->getContentSize().width * 0.9, 130);
    labelMsg->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelMsg->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContent->addChild(labelMsg);
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(_layerContainerContent->getContentSize().width / 2, 60);
    _layerContainerContent->addChild(menu);
    
    auto itemNo = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/revive_btn_no.png"), nullptr, CC_CALLBACK_1(PopupReviewMarket::onClickReviewNo, this));
    menu->addChild(itemNo);
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_review_market_msg_1"), GAME_FONT, 18);
        label->setPosition(itemNo->getContentSize().width / 2, itemNo->getContentSize().height / 2);
        label->setDimensions(itemNo->getContentSize().width * 0.95, itemNo->getContentSize().height * 0.95);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        itemNo->addChild(label);
    }
    
    auto itemOk = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/revive_btn_no.png"), nullptr, CC_CALLBACK_1(PopupReviewMarket::onClickReviewOk, this));
    menu->addChild(itemOk);
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_review_market_msg_3"), GAME_FONT, 18);
        label->setPosition(itemOk->getContentSize().width / 2, itemOk->getContentSize().height / 2);
        label->setDimensions(itemOk->getContentSize().width * 0.95, itemOk->getContentSize().height * 0.95);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        itemOk->addChild(label);
    }
    
    menu->alignItemsHorizontally();
    menu->alignItemsHorizontallyWithPadding(15);
}

#pragma mark - set, get

#pragma mark - click
void PopupReviewMarket::onClickReviewNo(cocos2d::Ref* sender)
{
    // 창닫기
    hide();
}

void PopupReviewMarket::onClickReviewOk(cocos2d::Ref* sender)
{
    //
    UserDefault::getInstance()->setIntegerForKey(KEY_REVIEW_MARKET, 1);
    
    //
    MafGooglePlay::InAppReview([&](MafGooglePlay::RESULT result){
        if (result == MafGooglePlay::RESULT::FAIL)
        {
            MafNative::OpenUrl("https://mr-kim.onelink.me/d4hS/rating");
        }
    });
    
    
    // 창닫기
    hide();
}
