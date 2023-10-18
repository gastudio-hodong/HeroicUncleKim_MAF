//
//  PopupButtonForceTutorial.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/25.
//

#include "PopupButtonForceTutorial.h"

USING_NS_CC;

PopupButtonForceTutorial* PopupButtonForceTutorial::create()
{
    PopupButtonForceTutorial* pRet = new(std::nothrow) PopupButtonForceTutorial();
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

PopupButtonForceTutorial::PopupButtonForceTutorial(void):
_btnButton(nullptr)
,_itemButton(nullptr)
,_pOriginParent(nullptr)
,_posOrigin(Vec2::ZERO)
,_callbackListener(nullptr)
{
}

PopupButtonForceTutorial::~PopupButtonForceTutorial(void)
{
}

#pragma mark -init
bool PopupButtonForceTutorial::init()
{
    if ( !PopupBase::init())
    {
        return false;
    }
    
    _bHideBackKey = false;
    
    initVar();
    
    return true;
}

void PopupButtonForceTutorial::initVar()
{
    _spriteContainer->setVisible(false);
}
void PopupButtonForceTutorial::show()
{
    PopupBase::show();
    
    this->setLocalZOrder(ZORDER_SCENE_POPUP + 10);
}

#pragma mark -utils
void PopupButtonForceTutorial::setCallbackListener(const std::function<void(Ref*)>& callback)
{
    _callbackListener = callback;
}
void PopupButtonForceTutorial::setButton(cocos2d::ui::Button* const btnButton)
{
    _btnButton = btnButton;
    if(_btnButton == nullptr)
        return;
    
    _posOrigin = _btnButton->getPosition();
    _pOriginParent = _btnButton->getParent();
    Node* pTemp = _btnButton;
    Vec2 pos = _pOriginParent->convertToWorldSpace(pTemp->getPosition());
    
    _btnButton->retain();
    _btnButton->removeFromParent();
    _btnButton->setPosition(pos);
    addChild(_btnButton);
    _btnButton->release();
    
    _btnButton->addClickEventListener(CC_CALLBACK_1(PopupButtonForceTutorial::onClick, this));
    uiButtonEffect(_btnButton);
}
void PopupButtonForceTutorial::setButton(MafNode::MafMenuItemSprite* const itemButton)
{
    _itemButton = itemButton;
    if(_itemButton == nullptr)
        return;
    
    _posOrigin = _itemButton->getPosition();
    _pOriginParent = _itemButton->getParent();
    Node* pTemp = _itemButton;
    Vec2 pos = _pOriginParent->convertToWorldSpace(pTemp->getPosition());
    
    _itemButton->retain();
    _itemButton->removeFromParent();
    _itemButton->setPosition(pos);
    addChild(_itemButton);
    _itemButton->release();
    
    _itemButton->setCallback(CC_CALLBACK_1(PopupButtonForceTutorial::onClick, this));
    uiButtonEffect(_itemButton);
}

void PopupButtonForceTutorial::uiButtonEffect(Node* const node)
{
    auto sprEffect = ui::Scale9Sprite::create("Assets/ui/adventure/adventure_box_on.png");
    sprEffect->setAnchorPoint(node->getAnchorPoint());
    sprEffect->setContentSize(node->getContentSize());
    sprEffect->setPosition(node->getPosition());
    addChild(sprEffect);

    auto fadeOut = FadeOut::create(1.f);
    auto fadeIn = FadeIn::create(1.f);
    auto sequience = Sequence::create(fadeOut, fadeIn, nullptr);
    sprEffect->runAction(RepeatForever::create(sequience));
}
#pragma mark -click
void PopupButtonForceTutorial::onClick(Ref* const pSender)
{
    if(_callbackListener != nullptr)
    {
        if(_btnButton != nullptr)
        {
            _btnButton->addClickEventListener(_callbackListener);
            _btnButton->retain();
            _btnButton->removeFromParent();
            _pOriginParent->addChild(_btnButton);
            _btnButton->setPosition(_posOrigin);
            _btnButton->release();
            _callbackListener(_btnButton);
        }
        else if(_itemButton != nullptr)
        {
            _itemButton->setCallback(_callbackListener);
            _itemButton->retain();
            _itemButton->removeFromParent();
            _pOriginParent->addChild(_itemButton);
            _itemButton->setPosition(_posOrigin);
            _itemButton->release();
            _callbackListener(_itemButton);
        }
    }
    _callbackListener = nullptr;
    hide();
}
