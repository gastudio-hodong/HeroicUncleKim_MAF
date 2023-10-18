//
//  UIComboBox.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "UIComboBox.h"

USING_NS_CC;

UIComboBox* UIComboBox::create(cocos2d::ValueVector list, eType type, std::string strPathBtn, std::string strPathList)
{
    UIComboBox* pRet = new(std::nothrow) UIComboBox();
    if ( pRet && pRet->init(list, type, strPathBtn, strPathList) )
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

UIComboBox::UIComboBox(void) :
_strPathBtn(""),
_strPathList(""),
_strPathArrowUp(""),
_strPathArrowDown(""),
_isOpen(false),
_isEnable(true),
_strDataSelect(""),

_strTextListFontName(GAME_FONT),
_nTextListFontSize(18),
_colorTextListFontColor(Color3B::WHITE),

_callbackSelect(nullptr),

_spriteArrow(nullptr),
_spriteList(nullptr),
_labelContents(nullptr),
_table(nullptr)
{
    
}

UIComboBox::~UIComboBox(void)
{
    
}

bool UIComboBox::init(cocos2d::ValueVector list, eType type, std::string strPathBtn, std::string strPathList)
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    
    _listData = list;
    _type = type;
    
    _strPathBtn = strPathBtn;
    _strPathList = strPathList;
   
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint( Vec2(0.5f, 0.5f) );
    
    auto container = Sprite::create(_strPathBtn);
    if ( container != nullptr )
    {
        Size size = container->getContentSize();
        setContentSize(size);
        
        // init
        initVar();
        initUi();
    }
    
    return true;
}

#pragma mark - init
void UIComboBox::initVar()
{
    
}

void UIComboBox::initUi()
{
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    addChild(menu);
    
    // item box
    auto itemBtn = MafNode::MafMenuItemSprite::create(Sprite::create(_strPathBtn), nullptr, CC_CALLBACK_1(UIComboBox::onClickBox, this));
    itemBtn->setPosition( Vec2(getContentSize().width / 2, getContentSize().height / 2) );
    menu->addChild(itemBtn);
    
    // label contents
    _labelContents = Label::createWithTTF(" ", GAME_FONT, 20, getContentSize(), TextHAlignment::CENTER, TextVAlignment::CENTER);
    _labelContents->setAnchorPoint( Vec2(0.0f, 0.0f) );
    _labelContents->setPosition( Vec2(0.0f, 0.0f) );
    addChild(_labelContents);
    
    // list bg
    _spriteList = Sprite::create(_strPathList);
    _spriteList->setAnchorPoint( Vec2(0.5f, 0.0f) );
    _spriteList->setPosition( Vec2(getContentSize().width / 2, getContentSize().height) );
    _spriteList->setVisible(false);
    addChild(_spriteList, -1);
    {
        auto listener = cocos2d::EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *event)->bool {
            cocos2d::Rect rect = cocos2d::Rect(0, 0, _spriteList->getContentSize().width, _spriteList->getContentSize().height);
            if (rect.containsPoint(_spriteList->convertToNodeSpace(touch->getLocation())) == false )
            {
                _isOpen = false;
                drawComboBox();
            }
                
            return false;
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _spriteList);
    }
    
    
    if ( _type == UIComboBox::type_up )
    {
        _spriteList->setAnchorPoint( Vec2(0.5f, 0.0f) );
        _spriteList->setPosition( Vec2(getContentSize().width / 2, getContentSize().height) );
    }
    else if ( _type == UIComboBox::type_down )
    {
        _spriteList->setAnchorPoint( Vec2(0.5f, 1.0f) );
        _spriteList->setPosition( Vec2(getContentSize().width / 2, 0) );
    }
    
    // list table
    _table = extension::TableView::create(this, Size(_spriteList->getContentSize().width, _spriteList->getContentSize().height));
    _table->setDirection(extension::ScrollView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setPosition( Vec2(0, 0) );
    _table->setDelegate(this);
    _spriteList->addChild(_table);
    _table->reloadData();
        
}

#pragma mark - table
void UIComboBox::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
    ssize_t idx = cell->getIdx();
    
    // contents
    _strDataSelect = _listData[idx].asString();
    
    if ( _labelContents != nullptr )
    {
        _labelContents->setString(_strDataSelect);
    }
    
    // close
    _isOpen = false;
    
    if ( _spriteList != nullptr )
        _spriteList->setVisible(false);
    
    if ( _spriteArrow != nullptr )
        _spriteArrow->setTexture(_strPathArrowUp);
    
    if ( _callbackSelect != nullptr )
    {
        _callbackSelect(_strDataSelect);
    }
}

cocos2d::Size UIComboBox::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    auto label = Label::createWithTTF("TEST", _strTextListFontName, _nTextListFontSize);
    
    return Size(table->getContentSize().width, label->getContentSize().height + 13);
}

cocos2d::extension::TableViewCell* UIComboBox::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
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
    
    // contents
    std::string strContents = _listData[idx].asString();
    
    
    auto label = Label::createWithTTF(strContents, _strTextListFontName, _nTextListFontSize);
    label->setPosition( Vec2(size.width / 2, size.height / 2) );
    label->setDimensions(size.width - 10, label->getContentSize().height);
    label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    label->setOverflow(Label::Overflow::SHRINK);
    label->setColor(_colorTextListFontColor);
    label->setTag(UIComboBox::tag_table_contents);
    cell->addChild(label);
    
    return cell;
}

ssize_t UIComboBox::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return _listData.size();
}

#pragma mark - set, get, add
void UIComboBox::addData(std::string str)
{
    _listData.push_back(Value(str));
    
    if ( _table != nullptr )
        _table->reloadData();
}

void UIComboBox::setDataSelect(std::string str)
{
    _strDataSelect = str;
    
    if ( _labelContents != nullptr )
    {
        _labelContents->setString(_strDataSelect);
    }
}

std::string UIComboBox::getDataSelect()
{
    return _strDataSelect;
}

void UIComboBox::setIconArrow(std::string strPathUp, std::string strPathDown)
{
    _strPathArrowUp = strPathUp;
    _strPathArrowDown = strPathDown;
    
    // sprite arrow add
    _spriteArrow = Sprite::create(_strPathArrowUp);
    _spriteArrow->setAnchorPoint( Vec2(1.0f, 0.5f) );
    _spriteArrow->setPosition( Vec2(getContentSize().width, getContentSize().height / 2) );
    addChild(_spriteArrow);
    
    // label resize
    if ( _labelContents != nullptr )
    {
        _labelContents->setWidth(_labelContents->getContentSize().width - _spriteArrow->getContentSize().width);
    }
}

void UIComboBox::setEnable(bool enable)
{
    _isEnable = enable;
    
    if ( _isEnable == false )
    {
        // 닫기
        _isOpen = false;
        
        if ( _spriteList != nullptr )
            _spriteList->setVisible(false);
        
        if ( _spriteArrow != nullptr )
            _spriteArrow->setTexture(_strPathArrowUp);
    }
}

void UIComboBox::setTextListFont(std::string strName, int nSize, Color3B color)
{
    _strTextListFontName = strName;
    _nTextListFontSize = nSize;
    _colorTextListFontColor = color;
    
    //
    _table->reloadData();
}
 

void UIComboBox::setCallbackSelect(const std::function<void(std::string)>& callback)
{
    _callbackSelect = callback;
}

#pragma mark - click
void UIComboBox::drawComboBox()
{
    if ( _isOpen == true )
    {
        // 열기
        if ( _spriteList != nullptr )
            _spriteList->setVisible(true);
        
        if ( _spriteArrow != nullptr )
            _spriteArrow->setTexture(_strPathArrowDown);
    }
    else
    {// 닫기

        if ( _spriteList != nullptr )
            _spriteList->setVisible(false);
        
        if ( _spriteArrow != nullptr )
            _spriteArrow->setTexture(_strPathArrowUp);
    }
}

#pragma mark - click
void UIComboBox::onClickBox(cocos2d::Ref* sender)
{
    if ( _isEnable == false )
        return;
    
    if ( _isOpen == true )
    {// 닫기
        _isOpen = false;
    }
    else
    {// 열기
        _isOpen = true;
    }
    
    //
    drawComboBox();
}
