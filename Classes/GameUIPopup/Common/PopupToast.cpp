//
//  PopupToast.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 6. 4..
//

#include "PopupToast.h"

USING_NS_CC;

std::vector<PopupToast::elementToast> PopupToast::_listToast;
std::vector<PopupToast::elementToast> PopupToast::_listToastImmediately;
PopupToast* PopupToast::create(std::string strMessage, double nPosX, double nPosY)
{
    PopupToast* pRet = new(std::nothrow) PopupToast();
    if ( pRet && pRet->init(strMessage, nPosX, nPosY) )
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

PopupToast::PopupToast(void) :
_strMessage(""),
_nPosX(0),
_nPosY(0)
{
    
}

PopupToast::~PopupToast(void)
{
    
}

bool PopupToast::init(std::string strMessage, double nPosX, double nPosY)
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    _strMessage = strMessage;
    _nPosX = nPosX;
    _nPosY = nPosY;
    
    uiToast();
    return true;
}

#pragma mark - set, get
#pragma mark - ui
void PopupToast::uiToast()
{
    Size sizeBox = Size::ZERO;
    auto label = Label::createWithTTF(_strMessage, GAME_FONT, 28);
    if ( label->getContentSize().width > getContentSize().width - 100 )
    {
        label->setDimensions(getContentSize().width - 100, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
    }
    sizeBox = label->getContentSize();
    sizeBox.width += 40;
    sizeBox.height += 10;
    
    if ( _nPosX - sizeBox.width / 2 < 0 )
    {
        _nPosX = sizeBox.width / 2;
    }
    if ( _nPosX + sizeBox.width / 2 > getContentSize().width )
    {
        _nPosX = getContentSize().width - sizeBox.width / 2;
    }
    
    
    //
    auto spriteBg = ui::Scale9Sprite::create("Assets/ui_common/base_toast_1_1.png", Rect::ZERO, Rect(7,7,2,2));
    spriteBg->setContentSize(sizeBox);
    spriteBg->setPosition(_nPosX, _nPosY);
    spriteBg->setCascadeOpacityEnabled(true);
    spriteBg->setOpacity(0);
    addChild(spriteBg);
    
    //
    label->setPosition(spriteBg->getContentSize().width / 2, spriteBg->getContentSize().height / 2);
    spriteBg->addChild(label);
    
    // action
    auto seq = Sequence::create(FadeIn::create(0.15f), DelayTime::create(1.5f), FadeOut::create(0.15f), nullptr);
    spriteBg->runAction(seq);
    
    auto seqParent = Sequence::create(DelayTime::create(1.8), RemoveSelf::create(), nullptr);
    runAction(seqParent);
}

#pragma mark - show, lock, hide
void PopupToast::show(std::string strMessage, double nPosX, double nPosY)
{
    if ( nPosX == 0 )
    {
        nPosX = UtilsRect::getVisibleSize().width / 2;
    }
    
    if ( nPosY == 0 )
    {
        nPosY = UtilsRect::getVisibleSize().height - 400;
    }
    
    // notification
    auto node = UtilsScene::getInstance()->getRunningScene();
    if ( node == nullptr )
    {
        return;
    }
    
    if ( node->getChildByTag(TAG_SCENE_TOAST) == nullptr )
    {
        _listToast.clear();
        _listToastImmediately.clear();
    }
    
    if ( _listToast.size() != 0 )
    {
        elementToast element = _listToast.back();
        if ( element.strMessage.compare(strMessage) == 0 )
        {
            return;
        }
    }
    
    // push
    _listToast.push_back( {strMessage, nPosX,  nPosY} );
    
    // toast add
    auto action1 = DelayTime::create((_listToast.size() - 1) * 0.5);
    auto action2 = CallFunc::create([=](){
        
        elementToast element = _listToast.at(0);
        std::string strMessage = element.strMessage;
        double nPosX = element.nPosX;
        double nPosY = element.nPosY;
        
        //
        auto popup = PopupToast::create(strMessage, nPosX, nPosY);
        node->addChild(popup, ZORDER_SCENE_TOAST, TAG_SCENE_TOAST);
    });
    auto action3 = DelayTime::create(0.5);
    auto action4 = CallFunc::create([=](){
        //
        _listToast.erase(_listToast.begin());
    });
    auto seq = Sequence::create(action1, action2, action3, action4, nullptr);
    node->runAction(seq);
}

void PopupToast::showImmediately(std::string strMessage, double nPosX, double nPosY)
{
    if ( nPosX == 0 )
    {
        nPosX = UtilsRect::getVisibleSize().width / 2;
    }
    if ( nPosY == 0 )
    {
        nPosY = UtilsRect::getVisibleSize().height - 400;
    }
    
    // notification
    auto node = UtilsScene::getInstance()->getRunningScene();
    if ( node == nullptr )
    {
        return;
    }
    
    if ( node->getChildByTag(TAG_SCENE_TOAST) == nullptr )
    {
        _listToast.clear();
        _listToastImmediately.clear();
    }
    
    if ( _listToastImmediately.size() != 0 )
    {
        elementToast element = _listToastImmediately.back();
        if ( element.strMessage.compare(strMessage) == 0 && element.nPosX == nPosX && element.nPosY == nPosY )
        {
            return;
        }
    }
    
    

    //
    _listToastImmediately.push_back( {strMessage, nPosX,  nPosY} );
    
    // toast add
    auto popup = PopupToast::create(strMessage, nPosX,  nPosY);
    node->addChild(popup, ZORDER_SCENE_TOAST, TAG_SCENE_TOAST);
    
    auto action1 = DelayTime::create(0.5);
    auto action2 = CallFunc::create([=](){
    
        if ( _listToastImmediately.size() == 0 )
        {
            return;
        }
        
        // obj erase
        _listToastImmediately.erase(_listToastImmediately.begin());
    });
    auto seq = Sequence::create(action1, action2, nullptr);
    popup->runAction(seq);
}

#pragma mark - scheduler
