//
//  PopupHelp.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/17.
//

#include "PopupHelp.h"

USING_NS_CC;

PopupHelp* PopupHelp::create(const int nMaxPage, const std::string strTitle, const std::string strContent)
{
    PopupHelp* pRet = new(std::nothrow) PopupHelp();
    if ( pRet && pRet->init(nMaxPage, strTitle, strContent) )
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

PopupHelp::PopupHelp(void) :
_nMaxPage(0),
_nCurrentPage(1),
_strTitle(""),
_strContent(""),
_layerContainerContents(nullptr),
_lyTextContainer(nullptr),
_lbRuleContent(nullptr),
_lbRuleTitle(nullptr)
{
}

PopupHelp::~PopupHelp(void)
{
}

#pragma mark -init

bool PopupHelp::init(const int nMaxPage, const std::string strTitle, const std::string strContent)
{
    if ( !PopupBase::init())
    {
        return false;
    }
    
    _nMaxPage = nMaxPage;
    _strTitle = strTitle;
    _strContent = strContent;
    
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void PopupHelp::initVar()
{
    //
    Size size = Size(726, 620);
    _spriteContainer->setContentSize( size );
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_costume_info"), GAME_FONT, 30);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"),nullptr, CC_CALLBACK_1(PopupHelp::onClick, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize());
    itemClose->setName("CLOSE");
    menu->addChild(itemClose);
}

void PopupHelp::initUi()
{
    uiContents();
    uiRedraw();
}

#pragma mark - ui
void PopupHelp::uiContents()
{
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize(Size(650, 400));
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    
    _layerContainerContents->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 100);
    
    _spriteContainer->addChild(_layerContainerContents);
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_1.png");
    sprBox->setContentSize(_layerContainerContents->getContentSize());
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setPosition(_layerContainerContents->getContentSize()/2);
    _layerContainerContents->addChild(sprBox);
    
    _lyTextContainer = Layer::create();
    _lyTextContainer->setIgnoreAnchorPointForPosition(false);
    _lyTextContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lyTextContainer->setContentSize(_layerContainerContents->getContentSize());
    _lyTextContainer->setPosition(_lyTextContainer->getContentSize().width/2, 0);
    
    _lbRuleContent = Label::createWithTTF(GAME_TEXT(""), GAME_FONT, 24);
    _lbRuleContent->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    _lbRuleContent->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _lbRuleContent->setPosition(0, _lbRuleContent->getContentSize().height);
    _lbRuleContent->setDimensions(_lyTextContainer->getContentSize().width, _lbRuleContent->getContentSize().height);
    _lbRuleContent->setOverflow(Label::Overflow::SHRINK);
    _lyTextContainer->addChild(_lbRuleContent);
    
    _lyTextContainer->setContentSize(Size(_lyTextContainer->getContentSize().width, _lbRuleContent->getContentSize().height));
    
   _table = extension::ScrollView::create( Size(_layerContainerContents->getContentSize().width, _layerContainerContents->getContentSize().height));
    _table->setDirection(extension::ScrollView::Direction::VERTICAL);
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition( Vec2(_layerContainerContents->getContentSize().width / 2, 0));
    _table->setContainer(_lyTextContainer);
    _table->setViewSize(_layerContainerContents->getContentSize() + Size(0, -60));
    _layerContainerContents->addChild(_table);
    
    _table->setContentOffset(_table->minContainerOffset());
    
    auto sprTitleBox = Sprite::create("Assets/ui_common/base_text_2_4.png");
    sprTitleBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    sprTitleBox->setPosition(0, _layerContainerContents->getContentSize().height - 5);
    _layerContainerContents->addChild(sprTitleBox);
    
    _lbRuleTitle = Label::createWithTTF("", GAME_FONT, 30);
    _lbRuleTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _lbRuleTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    _lbRuleTitle->setPosition(Vec2(10, sprTitleBox->getContentSize().height/2));
    sprTitleBox->addChild(_lbRuleTitle);
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    
    _spriteContainer->addChild(menu);
    
    auto itemPrev = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/arrow_L02.png"), nullptr, CC_CALLBACK_1(PopupHelp::onClick, this));
    itemPrev->setName("PREV");
    itemPrev->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    itemPrev->setPosition(_layerContainerContents->getPositionX() - _layerContainerContents->getContentSize().width/2, 20);
    itemPrev->setTouchRect(itemPrev->getContentSize() + Size(15, 40));
    menu->addChild(itemPrev);
    
    auto itemNext = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/arrow_R02.png"), nullptr, CC_CALLBACK_1(PopupHelp::onClick, this));
    itemNext->setName("NEXT");
    itemNext->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    itemNext->setPosition(_layerContainerContents->getPositionX() + _layerContainerContents->getContentSize().width/2, 20);
    itemNext->setTouchRect(itemNext->getContentSize() + Size(15, 40));
    menu->addChild(itemNext);
    
    int size = _nMaxPage;
    
    float width = (_spriteContainer->getContentSize().width - 120) / size - 10;
    
    if(width >= 50)
        width = 50;
    float posX = _spriteContainer->getContentSize().width/2 - width * (size/2);
    
    for(int i = 0; i < _nMaxPage; ++i)
    {
        auto sprPageBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_2.png");
        sprPageBox->setContentSize(Size(width, width));
        
        auto itemPage = MafNode::MafMenuItemSprite::create(nullptr, nullptr, sprPageBox, CC_CALLBACK_1(PopupHelp::onClick, this));
        itemPage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        itemPage->setContentSize(Size(sprPageBox->getContentSize()));
        itemPage->setTouchRect(sprPageBox->getContentSize());
        
        itemPage->setPosition(posX, itemNext->getPositionY() + itemNext->getContentSize().height/2);
        itemPage->setTag(i + 1);
        itemPage->setName("PAGE");
        menu->addChild(itemPage);
        
        if(_nCurrentPage == i+1)
            itemPage->setEnabled(false);
        
        auto lbPage = Label::createWithTTF(MafUtils::toString(i+1), GAME_FONT, 24);
        lbPage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbPage->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbPage->setDimensions(itemPage->getContentSize().width, itemPage->getContentSize().height);
        lbPage->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbPage->setPosition(itemPage->getContentSize()/2);
        itemPage->addChild(lbPage);
        
        _vecItemPage.pushBack(itemPage);
        
        if(size % 2 == 0)
            itemPage->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        
        posX += width;
    }
    
    
}

void PopupHelp::uiRedraw()
{
    std::string strTitle = GAME_TEXT(MafUtils::format(_strTitle.c_str(), _nCurrentPage));
    
    std::string strContent = GAME_TEXT(MafUtils::format(_strContent.c_str(), _nCurrentPage));
    
    _lbRuleContent->setString(strContent);
    _lbRuleTitle->setString(strTitle);
    
    _lyTextContainer->setContentSize(Size(_lyTextContainer->getContentSize().width, _lbRuleContent->getContentSize().height));
    _lbRuleContent->setPosition(0, _lbRuleContent->getContentSize().height);
    _table->setContentOffset(_table->minContainerOffset());
    
    for(auto item : _vecItemPage)
    {
        if(item->getTag() == _nCurrentPage)
        {
            item->setEnabled(false);
        }
        else
        {
            if(!item->isEnabled())
            {
                item->setEnabled(true);
            }
        }
    }
    
}

#pragma mark - click
void PopupHelp::onClick(Ref* sender)
{
    auto item = (Node*)sender;
    
    if(item->getName().compare("CLOSE") == 0)
    {
        hide();
    }
    else if(item->getName().compare("PREV") == 0)
    {
        onClickPage(_nCurrentPage - 1);
    }
    else if(item->getName().compare("NEXT") == 0)
    {
        onClickPage(_nCurrentPage + 1);
    }
    else if(item->getName().compare("PAGE") == 0)
    {
        onClickPage(item->getTag());
    }
}

void PopupHelp::onClickPage(int nPage)
{
    if(nPage <= 0 || nPage > _nMaxPage)
        return;
    
    _nCurrentPage = nPage;
    
    uiRedraw();
}
