//
//  MafNode.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 9. 12..
//
//

#include "MafNode.h"

//const unsigned int    kCurrentItem = 0xc0c05001;
const unsigned int    kZoomActionTag = 0xc0c05002;

const unsigned int    kNormalTag = 0x1;
const unsigned int    kSelectedTag = 0x2;
const unsigned int    kDisableTag = 0x3;


USING_NS_CC;
USING_NS_CC_EXT;
using namespace MafNode;

#pragma mark-
int64_t MafNode::MafMenu::_activate = 0;
MafMenu* MafNode::MafMenu::create()
{
    auto ret = new MafNode::MafMenu();
    Vector<MafMenuItem*> item;
    if (ret && ret->initWithArray(item)){
        ret->autorelease();
    }
    else{
        CC_SAFE_DELETE(ret);
    }
    
    return ret;
}

bool MafNode::MafMenu::initWithArray(const Vector<MafMenuItem*>& arrayOfItems)
{
    if (Layer::init())
    {
        _enabled = true;
        // menu in the center of the screen
        Size s = Director::getInstance()->getWinSize();
        
        this->setIgnoreAnchorPointForPosition(true);
        //this->ignoreAnchorPointForPosition(true);
        setAnchorPoint(Vec2(0.5f, 0.5f));
        this->setContentSize(s);
        
        setPosition(Vec2(s.width/2, s.height/2));
        
        int z=0;
        
        for (auto& item : arrayOfItems)
        {
            this->addChild(item, z);
            z++;
        }
        
        _selectedItem = nullptr;
        _state = Menu::State::WAITING;
        
        // enable cascade color and opacity on menus
        setCascadeColorEnabled(true);
        setCascadeOpacityEnabled(true);
        
        
        _touchListener = EventListenerTouchOneByOne::create();
        _touchListener->setSwallowTouches(true);
        
        _touchListener->onTouchBegan = CC_CALLBACK_2(MafMenu::onTouchBegan, this);
        _touchListener->onTouchMoved = CC_CALLBACK_2(MafMenu::onTouchMoved, this);
        _touchListener->onTouchEnded = CC_CALLBACK_2(MafMenu::onTouchEnded, this);
        _touchListener->onTouchCancelled = CC_CALLBACK_2(MafMenu::onTouchCancelled, this);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
        
        //
        _touchesStart = Vec2::ZERO;
        _touchesEnd = Vec2::ZERO;
        return true;
    }
    return false;
}

bool MafNode::MafMenu::onTouchBegan(Touch* touch, Event* event)
{
    if (_state != Menu::State::WAITING || ! _visible || !_enabled)
    {
        return false;
    }
    
    for (Node *c = this->_parent; c != nullptr; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
    
    /** item activate lock */
    int64_t diff = utils::getTimeInMilliseconds() - _activate;
    if ( _activate != 0 && (diff >= 0 && diff < 100) )
    {
        return false;
    }
    
    //
    _touchListener->setSwallowTouches(true);
    _touchesStart = touch->getStartLocation();
    
    //
    auto parent = getParent();
    while ( parent != nullptr )
    {
        auto scrollview = dynamic_cast<ScrollView*>(parent);
        if ( scrollview != nullptr )
        {
            Vec2 local = scrollview->convertToNodeSpace(touch->getStartLocation());
            
            Rect r = scrollview->getBoundingBox();
            r.origin = Vec2::ZERO;
            
            if (!r.containsPoint(local))
            {
                return false;
            }
            
            _touchListener->setSwallowTouches(false);
            break;
        }
        
        parent = parent->getParent();
    }
    
    _selectedItem = this->getItemForTouch(touch);
    if (_selectedItem)
    {
        /** item activate lock */
        _activate = utils::getTimeInMilliseconds();
        
        _state = Menu::State::TRACKING_TOUCH;
        _selectedItem->selected();
        return true;
    }
    
    return false;
}

void MafNode::MafMenu::onTouchEnded(Touch* touch, Event* event)
{
    _touchesEnd = touch->getLocation();
    double diff = std::abs(_touchesStart.x - _touchesEnd.x) > std::abs(_touchesStart.y - _touchesEnd.y) ? std::abs(_touchesStart.x - _touchesEnd.x) : std::abs(_touchesStart.y - _touchesEnd.y);
    
    if ( _touchListener->isSwallowTouches() == false && diff > 20.0f )
    {
        cocos2d::Menu::onTouchCancelled(touch, event);
    }
    else
    {
        cocos2d::Menu::onTouchEnded(touch, event);
    }
}

void MafNode::MafMenu::onTouchCancelled(Touch* touch, Event* event)
{
    cocos2d::Menu::onTouchCancelled(touch, event);
}

void MafNode::MafMenu::onTouchMoved(Touch* touch, Event* event)
{
    Vec2 touchesMove = touch->getLocation();
    double diff = std::abs(_touchesStart.x - touchesMove.x) > std::abs(_touchesStart.y - touchesMove.y) ? std::abs(_touchesStart.x - touchesMove.x) : std::abs(_touchesStart.y - touchesMove.y);
    
    if ( _touchListener->isSwallowTouches() == false && diff > 20.0f)
    {
        if (_selectedItem)
        {
            _selectedItem->unselected();
        }
    }
    
    //
    MenuItem *currentItem = this->getItemForTouch(touch);
    if (currentItem != _selectedItem)
    {
        if (_selectedItem)
        {
            _selectedItem->unselected();
        }
        _selectedItem = currentItem;
        if (_selectedItem)
        {
            _selectedItem->selected();
        }
    }
}

MafMenuItem* MafNode::MafMenu::getItemForTouch(Touch *touch)
{
    Vec2 touchLocation = touch->getLocation();
    
    if (!_children.empty())
    {
        for (auto iter = _children.crbegin(); iter != _children.crend(); ++iter)
        {
            MafNode::MafMenuItem* child = dynamic_cast<MafMenuItem*>(*iter);
            if (child && child->isVisible() && child->isEnabled())
            {
                Vec2 local = this->convertToNodeSpace(touchLocation);
                Rect r = child->getTouchRect();
                if (r.containsPoint(local))
                {
                    return child;
                }
            }
        }
    }
    
    return nullptr;
}
#pragma mark-

MafMenuItem::MafMenuItem(){
    
    _touchSize = Size::ZERO;
}
MafMenuItem* MafNode::MafMenuItem::create(){
    
    auto ret = new MafNode::MafMenuItem();
    //MenuItem *ret = new MenuItem();
    ret->initWithCallback(nullptr);
    ret->autorelease();
    return ret;
}

void MafNode::MafMenuItem::setTouchRect(Size size){
    
    _touchSize = size;
    
}
Rect MafNode::MafMenuItem::getTouchRect(){
    
    if(_touchSize.equals(Size::ZERO))
        return this->rect();
    
    Rect rect = Rect( _position.x - _touchSize.width * _anchorPoint.x,
                     _position.y - _touchSize.height * _anchorPoint.y,
                     _touchSize.width,
                     _touchSize.height);
//    Rect rect = Rect( 0,0,
//                     _touchSize.width, _touchSize.height);
    
    return rect;
}

#pragma mark-

void MafNode::MafMenuItemSprite::setNormalImage(Node* image)
{
    if (image != _normalImage)
    {
        if (image)
        {
            addChild(image, 0, kNormalTag);
            image->setAnchorPoint(Vec2(0, 0));
        }
        
        if (_normalImage)
        {
            removeChild(_normalImage, true);
        }
        
        _normalImage = image;
        this->setContentSize(_normalImage->getContentSize());
        this->updateImagesVisibility();
    }
}

void MafNode::MafMenuItemSprite::setSelectedImage(Node* image)
{
    if (image != _normalImage)
    {
        if (image)
        {
            addChild(image, 0, kSelectedTag);
            //image->setAnchorPoint(Vec2(0, 0));
            image->setAnchorPoint(Vec2(0.5, 0.5));
            image->setScale(1.1f);
            
            Size size = image->getContentSize();
            image->setPosition(Vec2(image->getPosition().x + size.width/2,image->getPosition().y + size.height/2));
            
        }
        
        if (_selectedImage)
        {
            removeChild(_selectedImage, true);
        }
        
        _selectedImage = image;
        this->updateImagesVisibility();
    }
}

void MafNode::MafMenuItemSprite::setDisabledImage(Node* image)
{
    if (image != _normalImage)
    {
        if (image)
        {
            addChild(image, 0, kDisableTag);
            image->setAnchorPoint(Vec2(0, 0));
        }
        
        if (_disabledImage)
        {
            removeChild(_disabledImage, true);
        }
        
        _disabledImage = image;
        this->updateImagesVisibility();
    }
}


MafNode::MafMenuItemSprite * MafNode::MafMenuItemSprite::create(Node* normalSprite, Node* selectedSprite, const ccMenuCallback& callback)
{
    return MafNode::MafMenuItemSprite::create(normalSprite, selectedSprite, nullptr, callback);
}

MafNode::MafMenuItemSprite * MafNode::MafMenuItemSprite::create(Node *normalSprite, Node *selectedSprite, Node *disabledSprite, const ccMenuCallback& callback)
{
    MafNode::MafMenuItemSprite *ret = new MafNode::MafMenuItemSprite();
    ret->initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, callback);
    ret->autorelease();
    return ret;
}

bool MafNode::MafMenuItemSprite::initWithNormalSprite(Node* normalSprite, Node* selectedSprite, Node* disabledSprite, const ccMenuCallback& callback)
{
    MenuItem::initWithCallback(callback);
    setNormalImage(normalSprite);
    setSelectedImage(selectedSprite);
    setDisabledImage(disabledSprite);
    
    if(_normalImage)
    {
        this->setContentSize(_normalImage->getBoundingBox().size);
    }
    
    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);
    
    return true;
}

/**
 @since v0.99.5
 */
void MafNode::MafMenuItemSprite::selected()
{
    MafNode::MafMenuItem::selected();
    
    if (_normalImage)
    {
        if (_disabledImage)
        {
            _disabledImage->setVisible(false);
        }
        
        if (_selectedImage)
        {
            _normalImage->setVisible(false);
            _selectedImage->setVisible(true);
        }
        else
        {
            _normalImage->setVisible(true);
        }
    }
    
    if ( _bLongClick == true )
    {
        _bLongSchedule = true;
        
        _nLongTime = 0.05;
        _nLongTimeMax = 0.05;
        Director::getInstance()->getScheduler()->schedule(schedule_selector(MafNode::MafMenuItemSprite::scheduleLongSelected), this, 0.0, kRepeatForever, 0.2, false);
    }
}

void MafNode::MafMenuItemSprite::unselected()
{
    MafNode::MafMenuItem::unselected();
    if (_normalImage)
    {
        _normalImage->setVisible(true);
        
        if (_selectedImage)
        {
            _selectedImage->setVisible(false);
        }
        
        if (_disabledImage)
        {
            _disabledImage->setVisible(false);
        }
    }
    
    if ( _bLongClick )
    {
        _bLongSchedule = false;
        unschedule(schedule_selector(MafNode::MafMenuItemSprite::scheduleLongSelected));
    }
}

void MafNode::MafMenuItemSprite::setEnabled(bool bEnabled)
{
    if( _enabled != bEnabled )
    {
        MafNode::MafMenuItem::setEnabled(bEnabled);
        this->updateImagesVisibility();
    }
}

// Helper
void MafNode::MafMenuItemSprite::updateImagesVisibility()
{
    if (_enabled)
    {
        if (_normalImage)   _normalImage->setVisible(true);
        if (_selectedImage) _selectedImage->setVisible(false);
        if (_disabledImage) _disabledImage->setVisible(false);
    }
    else
    {
        if (_disabledImage)
        {
            if (_normalImage)   _normalImage->setVisible(false);
            if (_selectedImage) _selectedImage->setVisible(false);
            if (_disabledImage) _disabledImage->setVisible(true);
        }
        else
        {
            if (_normalImage)   _normalImage->setVisible(true);
            if (_selectedImage) _selectedImage->setVisible(false);
            if (_disabledImage) _disabledImage->setVisible(false);
        }
    }
}

void MafNode::MafMenuItemSprite::setLongClick(bool bLong)
{
    _bLongClick = bLong;
}

void MafNode::MafMenuItemSprite::scheduleLongSelected(float dt)
{
    if ( _bLongSchedule == false && isScheduled(schedule_selector(MafNode::MafMenuItemSprite::scheduleLongSelected)) )
    {
        unschedule(schedule_selector(MafNode::MafMenuItemSprite::scheduleLongSelected));
        return;
    }
    
    _nLongTime -= 0.01;
    if ( _nLongTime <= 0 )
    {
        _nLongTimeMax -= 0.01;
        if ( _nLongTimeMax <= 0 )
        {
            _nLongTimeMax = 0.01;
        }
        _nLongTime = _nLongTimeMax;
    
        if( _callback )
        {
            _callback(this);
        }
    }
}

#pragma mark-MafMenuItemLabel

//
//CCMenuItemLabel
//

void MafNode::MafMenuItemLabel::setLabel(Node* var)
{
    if (var)
    {
        var->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        setContentSize(var->getContentSize());
        addChild(var);
    }
    
    if (_label)
    {
        removeChild(_label, true);
    }
    
    _label = var;
}



MafMenuItemLabel * MafNode::MafMenuItemLabel::create(Node*label, const ccMenuCallback& callback)
{
    MafNode::MafMenuItemLabel *ret = new (std::nothrow) MafNode::MafMenuItemLabel();
    ret->initWithLabel(label, callback);
    ret->autorelease();
    return ret;
}

MafMenuItemLabel* MafNode::MafMenuItemLabel::create(Node *label)
{
    MafNode::MafMenuItemLabel *ret = new (std::nothrow) MafNode::MafMenuItemLabel();
    ret->initWithLabel(label, (const ccMenuCallback&) nullptr);
    ret->autorelease();
    return ret;
}

// FIXME:: deprecated
bool MafNode::MafMenuItemLabel::initWithLabel(Node* label, Ref* target, SEL_MenuHandler selector)
{
//    _target = target;
//    CC_SAFE_RETAIN(_target);
    return initWithLabel(label, std::bind(selector,target, std::placeholders::_1) );
}

bool MafNode::MafMenuItemLabel::initWithLabel(Node* label, const ccMenuCallback& callback)
{
    MafNode::MafMenuItem::initWithCallback(callback);
    _originalScale = 1.0f;
    _colorBackup = Color3B::WHITE;
    setDisabledColor(Color3B(126,126,126));
    this->setLabel(label);
    
    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);
    
    return true;
}


MafMenuItemLabel::~MafMenuItemLabel()
{
}

void MafNode::MafMenuItemLabel::setString(const std::string& label)
{
    dynamic_cast<LabelProtocol*>(_label)->setString(label);
    this->setContentSize(_label->getContentSize());
}

void MafNode::MafMenuItemLabel::activate()
{
    if(_enabled)
    {
        this->stopAllActions();
        this->setScale( _originalScale );
        MafNode::MafMenuItem::activate();
    }
}

void MafNode::MafMenuItemLabel::selected()
{
    // subclass to change the default action
    if(_enabled)
    {
        MafNode::MafMenuItem::selected();
        
        Action *action = getActionByTag(kZoomActionTag);
        if (action)
        {
            this->stopAction(action);
        }
        else
        {
            _originalScale = this->getScale();
        }
        
        Action *zoomAction = ScaleTo::create(0.1f, _originalScale * 1.2f);
        zoomAction->setTag(kZoomActionTag);
        this->runAction(zoomAction);
    }
}

void MafNode::MafMenuItemLabel::unselected()
{
    // subclass to change the default action
    if(_enabled)
    {
        MafNode::MafMenuItem::unselected();
        this->stopActionByTag(kZoomActionTag);
        Action *zoomAction = ScaleTo::create(0.1f, _originalScale);
        zoomAction->setTag(kZoomActionTag);
        this->runAction(zoomAction);
    }
}

void MafNode::MafMenuItemLabel::setEnabled(bool enabled)
{
    if( _enabled != enabled )
    {
        if(enabled == false)
        {
            _colorBackup = this->getColor();
            this->setColor(_disabledColor);
        }
        else
        {
            this->setColor(_colorBackup);
        }
    }
    MafNode::MafMenuItem::setEnabled(enabled);
}
