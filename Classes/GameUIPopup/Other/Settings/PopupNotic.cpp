//
//  lkjkl.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#include "PopupNotic.h"

#include "Common/ConfigGameNormal.h"
#include "Common/ConfigKey.h"

USING_NS_CC;

PopupNotic* PopupNotic::create()
{
    PopupNotic* pRet = new(std::nothrow) PopupNotic();
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

PopupNotic::PopupNotic(void) :
_strUrl(""),
_bCheck(false)
{
    
}

PopupNotic::~PopupNotic(void)
{
    
}

bool PopupNotic::init()
{
    if( !PopupBase::init() )
    {
        return false;
    }
    
    //
    setName("Popup_Notic");
    
    //
    initVar();
    initUi();
    
    return true;
}

void PopupNotic::hide()
{
    // save check
    setCheckSave();
    
    //
    PopupBase::hide();
}

void PopupNotic::show(bool bCheckIgnore)
{
    if ( bCheckIgnore == false )
    {
        std::string strTime = UserDefault::getInstance()->getStringForKey(KEY_TODAY_NOTICE, "0");
        
        int64_t nTimeCheck  = std::stoll(strTime);
        int64_t nTime = UtilsDate::getInstance()->getTime();
        if ( nTime < nTimeCheck )
        {
            return;
        }
    }
    
    auto scene = UtilsScene::getInstance()->getRunningScene();
    if ( scene == nullptr )
    {
        return;
    }
    
    if ( scene != nullptr )
    {
        scene->addChild(this, ZORDER_SCENE_POPUP_NOTIC, TAG_SCENE_POPUP);
    }
    
    runAction( FadeTo::create(0.4, 165) );
    
    if ( _spriteContainer != nullptr )
    {
        double scaleTo1 = (_spriteContainer->getContentSize().width - 5) / _spriteContainer->getContentSize().width;
        auto action1 = ScaleTo::create(0.05f, 1.0f,   scaleTo1);
        auto action2 = ScaleTo::create(0.05f, scaleTo1, scaleTo1);
        auto action3 = ScaleTo::create(0.05f, scaleTo1, 1.0f);
        auto action4 = ScaleTo::create(0.05f, 1.0f);
        auto seq = Sequence::create(action1, action2, action3, action4, nullptr);
        
        _spriteContainer->runAction(seq);
    }
}

#pragma mark - init
void PopupNotic::initVar()
{
    _strUrl = ParameterManager::getInstance()->getParameterString("notice_url");
    _strUrl = UtilsString::getInstance()->format(_strUrl.c_str(), TextManager::getInstance()->getLang().c_str());
}

void PopupNotic::initUi()
{
    // size
    _spriteContainer->setContentSize( Size(656, 1088) );
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_notice"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupNotic::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    _layerContainer = LayerColor::create();
    _layerContainer->setContentSize( Size(_spriteContainer->getContentSize().width - 12, _spriteContainer->getContentSize().height - 75 - 80) );
    _layerContainer->setIgnoreAnchorPointForPosition(false);
    _layerContainer->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainer->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_layerContainer);
    
    _layerContainerCheck = LayerColor::create();
    _layerContainerCheck->setContentSize( Size(_spriteContainer->getContentSize().width - 12, 80) );
    _layerContainerCheck->setIgnoreAnchorPointForPosition(false);
    _layerContainerCheck->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerCheck->setPosition(_spriteContainer->getContentSize().width / 2, 0);
    _spriteContainer->addChild(_layerContainerCheck);
    
    //
    uiDefaultWebView();
    uiDefaultCheck();
    
}

#pragma mark - ui
void PopupNotic::uiDefaultWebView()
{
    //
    _layerContainer->removeAllChildren();
    
    _nodeWebView = cocos2d::experimental::ui::WebView::create();
    _nodeWebView->setContentSize(_layerContainer->getContentSize());
    _nodeWebView->setPosition(Vec2(_layerContainer->getContentSize().width / 2, _layerContainer->getContentSize().height / 2));
    _nodeWebView->loadURL(_strUrl);
    _nodeWebView->setScalesPageToFit(true);
    _nodeWebView->setOnDidFinishLoading([=](cocos2d::experimental::ui::WebView *sender, const std::string &url){
        
        if ( _nodeWebView->canGoBack() )
        {
            _itemBack->setVisible(true);
        }
        else
        {
            _itemBack->setVisible(false);
        }
    });
    _layerContainer->addChild(_nodeWebView);
}

void PopupNotic::uiDefaultCheck()
{
    //
    _layerContainerCheck->removeAllChildren();
    
    double nPosX = 20;
    double nPosY = _layerContainerCheck->getContentSize().height / 2;
    
    //
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    _layerContainerCheck->addChild(menu);
    
    // left
    _itemCheck = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/costume/checkbox_off.png"), Sprite::create("Assets/ui/costume/checkbox_on.png"), CC_CALLBACK_1(PopupNotic::onClickCheck, this) );
    _itemCheck->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _itemCheck->setPosition(nPosX, nPosY);
    _itemCheck->getSelectedImage()->setScale(1);
    _itemCheck->setTouchRect(_itemCheck->getContentSize() * 1.5);
    menu->addChild(_itemCheck);
    nPosX += _itemCheck->getContentSize().width + 25;
    
    auto labelCheck = Label::createWithTTF(GAME_TEXT("t_ui_notice_check"), GAME_FONT, 22);
    labelCheck->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    labelCheck->setPosition(nPosX, nPosY);
    _spriteContainer->addChild(labelCheck);
    
    // right
    nPosX = _layerContainerCheck->getContentSize().width - 20;
    
    _itemBack = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_back1_2.png"), nullptr, CC_CALLBACK_1(PopupNotic::onClickBack, this) );
    _itemBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _itemBack->setPosition(nPosX, nPosY);
    _itemBack->setTouchRect(_itemBack->getContentSize() * 1.5);
    _itemBack->setVisible(false);
    menu->addChild(_itemBack);
}

#pragma mark - click
void PopupNotic::onClickClose(Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    
    setCheckSave();
    hide();
}

void PopupNotic::onClickCheck(Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
 
    _bCheck = !_bCheck;
    if ( _bCheck == true )
    {
        item->selected();
    }
    else
    {
        item->unselected();
    }
}

void PopupNotic::onClickBack(Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    
    if ( _nodeWebView->canGoBack() )
    {
        _nodeWebView->goBack();
    }
}


#pragma mark - callback

#pragma mark - game
void PopupNotic::setCheckSave()
{
    if ( _bCheck == false )
    {
        return;
    }
    
    int64_t nTime = UtilsDate::getInstance()->getTime() + 86400;
    UserDefault::getInstance()->setStringForKey(KEY_TODAY_NOTICE, std::to_string(nTime));
}
