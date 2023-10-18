//
//  PopupBase.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 05/06/2019.
//

#include "PopupBase.h"

#include "ManagerGame/UserInfoManager.h"

USING_NS_CC;

PopupBase::PopupBase() :
_ePlace(E_PLACE::EMPTY),

_bHideBackKey(true),
_bHideAll(true),

_spriteContainer(nullptr),
_nextPopup(nullptr)
{
    
}

PopupBase::~PopupBase()
{
    if ( _nextPopup != nullptr )
    {
        _nextPopup->release();
        _nextPopup = nullptr;
    }
}

bool PopupBase::init(PopupBaseType eType)
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    
    //
    _ePlace = UserInfoManager::getInstance()->getCurrentPlace();
    
    
    // event
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *event)->bool {
        cocos2d::Rect rect = cocos2d::Rect(0, 0, this->getContentSize().width, this->getContentSize().height);
        if (rect.containsPoint(this->convertToNodeSpace(touch->getLocation())))
            return true;
        
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    if ( eType == PopupBaseType::BASE )
    {
        _spriteContainer = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale.png", Rect::ZERO, Rect(70,110,10,10));
        _spriteContainer->setPosition( Vec2(getContentSize().width / 2, getContentSize().height / 2) );
        addChild(_spriteContainer);
    }
    
    return true;
}

void PopupBase::onExit()
{
    LayerColor::onExit();
}

void PopupBase::backkey()
{
    if ( _bHideBackKey == false )
    {
        return;
    }
    
    hide();
}

void PopupBase::show()
{
    auto scene = UtilsScene::getInstance()->getRunningScene();
    if ( scene == nullptr )
    {
        return;
    }
    
    if ( scene != nullptr )
    {
        scene->addChild(this, ZORDER_SCENE_POPUP, TAG_SCENE_POPUP);
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

void PopupBase::hide()
{
    if ( _nextPopup != nullptr )
    {
        _nextPopup->show();
        
        //
        _nextPopup->release();
        _nextPopup = nullptr;
    }
    
    if ( getParent() != nullptr )
    {
        removeFromParent();
    }
}

void PopupBase::setHideBackKey(bool bBack)
{
    _bHideBackKey = bBack;
}

bool PopupBase::isHideBackKey()
{
    return _bHideBackKey;
}

void PopupBase::setHideAll(bool bBack)
{
    _bHideAll = bBack;
}

bool PopupBase::isHideAll()
{
    return _bHideAll;
}

void PopupBase::setNextPopup(PopupBase* popup)
{
    if(_nextPopup != nullptr)
    {
        _nextPopup->setNextPopup(popup);
        return;
    }
    _nextPopup = popup;
    _nextPopup->retain();
}
