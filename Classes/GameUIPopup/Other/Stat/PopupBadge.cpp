//
//  PopupBadge.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/13.
//

#include "PopupBadge.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoBadge.h"

#include "GameUIPopup/Other/Stat/PopupBadgeDetail.h"

#include "ManagerGame/BadgeManager.h"

USING_NS_CC;

PopupBadge* PopupBadge::create()
{
    PopupBadge* pRet = new(std::nothrow) PopupBadge();
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

PopupBadge::PopupBadge(void) :
_bEdit(false),

_layerContainerBadgeSelect(nullptr),
_layerContainerBadgeSelectInBox(nullptr),
_layerContainerBadgeHave(nullptr),
_layerContainerButton(nullptr),

_labelTotalBadgeAtkBuff(nullptr)
{
}

PopupBadge::~PopupBadge(void)
{
}

bool PopupBadge::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    initVar();
    initUi();
    
    // 로그 이벤트
    MafAnalyticsManager::LogEvent(kAnalEventTokenEnter, kRepeatTrue);

    return true;
}

void PopupBadge::onEnter()
{
    PopupBase::onEnter();
    
    // network
    PopupLoading::show();
    BadgeManager::getInstance()->requestInfo(CC_CALLBACK_1(PopupBadge::callbackInfo, this));
}

#pragma mark - init
void PopupBadge::initVar()
{
    
}

void PopupBadge::initUi()
{
    // size
    _spriteContainer->setContentSize( Size(680, 1000) );
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_badge_title"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupBadge::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    _layerContainerBadgeSelect = Layer::create();
    _layerContainerBadgeSelect->setContentSize( Size(_spriteContainer->getContentSize().width, 300) );
    _layerContainerBadgeSelect->setIgnoreAnchorPointForPosition(false);
    _layerContainerBadgeSelect->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerBadgeSelect->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_layerContainerBadgeSelect);
    
    _layerContainerButton = Layer::create();
    _layerContainerButton->setContentSize( Size(_spriteContainer->getContentSize().width, 100) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition(_spriteContainer->getContentSize().width / 2, 25);
    _layerContainerButton->setVisible(false);
    _spriteContainer->addChild(_layerContainerButton);
    
    _layerContainerBadgeHave = Layer::create();
    _layerContainerBadgeHave->setContentSize( Size(_spriteContainer->getContentSize().width, _spriteContainer->getContentSize().height - 75 - _layerContainerBadgeSelect->getContentSize().height - 25) );
    _layerContainerBadgeHave->setIgnoreAnchorPointForPosition(false);
    _layerContainerBadgeHave->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerBadgeHave->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - _layerContainerBadgeSelect->getContentSize().height);
    _spriteContainer->addChild(_layerContainerBadgeHave);
    
    //
    uiBadgeSelect();
    uiBadgeHave();
    uiButton();
}

#pragma mark - ui
void PopupBadge::uiBadgeSelect()
{
    _layerContainerBadgeSelect->removeAllChildren();
    
    auto spriteTitleBG = Sprite::create("Assets/ui_common/base_text_2_1.png");
    spriteTitleBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    spriteTitleBG->setPosition(0, _layerContainerBadgeSelect->getContentSize().height);
    _layerContainerBadgeSelect->addChild(spriteTitleBG);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_badge_title_select"), GAME_FONT, 28);
        labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelText->setPosition(20, spriteTitleBG->getContentSize().height / 2);
        labelText->setDimensions(spriteTitleBG->getContentSize().width * 0.95, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        spriteTitleBG->addChild(labelText);
    }
    
    auto labelInfo = Label::createWithTTF(GAME_TEXT("t_ui_badge_msg_1"), GAME_FONT, 20);
    labelInfo->setAnchorPoint(Vec2::ZERO);
    labelInfo->setPosition(20, 25);
    labelInfo->setDimensions(_layerContainerBadgeSelect->getContentSize().width * 0.95, labelInfo->getContentSize().height);
    labelInfo->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    labelInfo->setOverflow(Label::Overflow::SHRINK);
    _layerContainerBadgeSelect->addChild(labelInfo);
    
    _labelTotalBadgeAtkBuff = Label::createWithTTF("", GAME_FONT, 20);
    _labelTotalBadgeAtkBuff->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _labelTotalBadgeAtkBuff->setPosition(_layerContainerBadgeSelect->getContentSize().width/2, labelInfo->getPositionY() + 70);
    _labelTotalBadgeAtkBuff->setDimensions(_layerContainerBadgeSelect->getContentSize().width * 0.95, _labelTotalBadgeAtkBuff->getContentSize().height);
    _labelTotalBadgeAtkBuff->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
    _labelTotalBadgeAtkBuff->setOverflow(Label::Overflow::SHRINK);
    _layerContainerBadgeSelect->addChild(_labelTotalBadgeAtkBuff);
    
    //
    _layerContainerBadgeSelectInBox = Layer::create();
    _layerContainerBadgeSelectInBox->setContentSize(Size(632, 116));
    _layerContainerBadgeSelectInBox->setIgnoreAnchorPointForPosition(false);
    _layerContainerBadgeSelectInBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _layerContainerBadgeSelectInBox->setPosition(_layerContainerBadgeSelect->getContentSize().width / 2, _layerContainerBadgeSelect->getContentSize().height - spriteTitleBG->getContentSize().height);
    _layerContainerBadgeSelect->addChild(_layerContainerBadgeSelectInBox);
}

void PopupBadge::uiBadgeHave()
{
    _layerContainerBadgeHave->removeAllChildren();
 
    //
    auto layerScroll = extension::ScrollView::create(Size(_layerContainerBadgeHave->getContentSize().width, _layerContainerBadgeHave->getContentSize().height) );
    layerScroll->setDirection(extension::ScrollView::Direction::VERTICAL);
    layerScroll->setIgnoreAnchorPointForPosition(false);
    layerScroll->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    layerScroll->setPosition(Vec2::ZERO);
    layerScroll->setName("SCROLLVIEW");
    _layerContainerBadgeHave->addChild(layerScroll);

    auto layerContainer = Layer::create();
    layerContainer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    layerScroll->setContainer(layerContainer);
    
    double nTotalWidth = 0;
    double nTotalHeight = 0;
    double posX = 0;
    double posY = 0;

    std::vector<std::string> listGroup = BadgeManager::getInstance()->getListGroup();

    for ( int i = 0; i < listGroup.size(); i++ )
    {
        std::string strGroup = listGroup.at(i);

        double nWidth = layerScroll->getViewSize().width;
        double nHeight = 50 + 100;
        
        auto layerBadgeDetail = uiBadgeDetail(nWidth, nHeight, strGroup);
        layerBadgeDetail->setPosition(posX, posY);
        layerContainer->addChild(layerBadgeDetail);
        
        posY += layerBadgeDetail->getContentSize().height;
        
        nTotalWidth = nWidth;
        nTotalHeight = posY;
    }
    layerContainer->setContentSize(Size(nTotalWidth, nTotalHeight));
    layerScroll->setContentOffset(layerScroll->minContainerOffset());
}

void PopupBadge::uiButton()
{
    _layerContainerButton->removeAllChildren();
    
    auto menuBox = MafNode::MafMenu::create();
    menuBox->setAnchorPoint(Vec2::ZERO);
    menuBox->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menuBox);
    {
        auto spriteCancel = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
        spriteCancel->setContentSize(Size(212, 88));
        
        auto itemCancel = MafNode::MafMenuItemSprite::create(spriteCancel, nullptr, CC_CALLBACK_1(PopupBadge::onClickEditApply, this));
        itemCancel->setTag(1);
        menuBox->addChild(itemCancel);
        {
            auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_cancel"), GAME_FONT, 24);
            labelText->setPosition(itemCancel->getContentSize().width / 2, itemCancel->getContentSize().height / 2);
            labelText->setDimensions(itemCancel->getContentSize().width * 0.95, labelText->getContentSize().height);
            labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelText->setOverflow(Label::Overflow::SHRINK);
            itemCancel->addChild(labelText);
        }
        
        auto spriteConfirm = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
        spriteConfirm->setContentSize(Size(212, 88));
        
        auto itemConfirm = MafNode::MafMenuItemSprite::create(spriteConfirm, nullptr, CC_CALLBACK_1(PopupBadge::onClickEditApply, this));
        itemConfirm->setTag(2);
        menuBox->addChild(itemConfirm);
        {
            auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_confirm"), GAME_FONT, 24);
            labelText->setPosition(itemConfirm->getContentSize().width / 2, itemConfirm->getContentSize().height / 2);
            labelText->setDimensions(itemConfirm->getContentSize().width * 0.95, labelText->getContentSize().height);
            labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelText->setOverflow(Label::Overflow::SHRINK);
            itemConfirm->addChild(labelText);
        }
        
        menuBox->alignItemsHorizontallyWithPadding(20);
    }
}

cocos2d::Layer* PopupBadge::uiBadgeDetail(double nWidth, double nHeight, std::string strGroup)
{
    auto layerChild = Layer::create();
    layerChild->setContentSize(Size(nWidth, nHeight));
    layerChild->setIgnoreAnchorPointForPosition(false);
    layerChild->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    
    auto listHave = BadgeManager::getInstance()->getListFromGroup(strGroup);
    if (listHave.size() <= 0)
    {
        auto spriteTitleBG = Sprite::create("Assets/ui_common/base_text_2_1.png");
        spriteTitleBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        spriteTitleBG->setPosition(0, layerChild->getContentSize().height);
        layerChild->addChild(spriteTitleBG);
        {
            std::string strTextKey = MafUtils::format("t_ui_badge_title_have_%s", strGroup.c_str());
            std::transform(strTextKey.cbegin(), strTextKey.cend(), strTextKey.begin(), ::tolower);
            
            auto labelText = Label::createWithTTF(GAME_TEXT(strTextKey), GAME_FONT, 28);
            labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labelText->setPosition(20, spriteTitleBG->getContentSize().height / 2);
            labelText->setDimensions(spriteTitleBG->getContentSize().width * 0.95, labelText->getContentSize().height);
            labelText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
            labelText->setOverflow(Label::Overflow::SHRINK);
            spriteTitleBG->addChild(labelText);
        }
        
        auto labelNodata = Label::createWithTTF(GAME_TEXT("t_ui_msg_nodata"), GAME_FONT, 20);
        labelNodata->setPosition(layerChild->getContentSize().width / 2, (layerChild->getContentSize().height - 50) / 2);
        labelNodata->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelNodata->setOverflow(Label::Overflow::SHRINK);
        labelNodata->setName("LABEL_NODATA");
        layerChild->addChild(labelNodata);
    }
    else
    {
        int drawCol = 6;
        int drawRow = (int)listHave.size() / drawCol;
        if ( listHave.size() % drawCol != 0 )
        {
            drawRow++;
        }
        nHeight = 50 + (drawRow * 120);
        layerChild->setContentSize(Size(nWidth, nHeight));
        
        //
        auto spriteTitleBG = Sprite::create("Assets/ui_common/base_text_2_1.png");
        spriteTitleBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        spriteTitleBG->setPosition(0, layerChild->getContentSize().height);
        layerChild->addChild(spriteTitleBG);
        {
            std::string strTextKey = MafUtils::format("t_ui_badge_title_have_%s", strGroup.c_str());
            std::transform(strTextKey.cbegin(), strTextKey.cend(), strTextKey.begin(), ::tolower);
            
            auto labelText = Label::createWithTTF(GAME_TEXT(strTextKey), GAME_FONT, 28);
            labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labelText->setPosition(20, spriteTitleBG->getContentSize().height / 2);
            labelText->setDimensions(spriteTitleBG->getContentSize().width * 0.95, labelText->getContentSize().height);
            labelText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
            labelText->setOverflow(Label::Overflow::SHRINK);
            spriteTitleBG->addChild(labelText);
        }
        
        //
        double nCellWidth = 100;
        double nCellHeight = 100;
        double nPaddingX = (layerChild->getContentSize().width - (nCellWidth * drawCol)) / (drawCol+1);
        double nPaddingY = 10;
        
        auto menu = MafNode::MafMenu::create();
        menu->setPosition(Vec2::ZERO);
        layerChild->addChild(menu);
        for ( int i= 0; i < listHave.size(); i++)
        {
            auto objHave = listHave.at(i);
            auto listHaveBadge = BadgeManager::getInstance()->getListHaveFromBadgeIdx(objHave->getIdx());
            
            int x = i % drawCol;
            int y = i / drawCol;
            double posX = (x * nCellWidth) + (x * nPaddingX) + nPaddingX + (nCellWidth / 2);
            double posY = layerChild->getContentSize().height - 50 - (y * nCellHeight) - (y * nPaddingY) - nCellHeight / 2;

            auto spriteIcon = Sprite::create(objHave->getPath());
            if(spriteIcon == nullptr)
            {
                spriteIcon = Sprite::create("Assets/ui/stat/badge_000.png");
            }
             
            auto item = MafNode::MafMenuItemSprite::create(spriteIcon, nullptr, CC_CALLBACK_1(PopupBadge::onClickSelect, this));
            item->setPosition(posX, posY);
            item->setTag(objHave->getIdx());
            menu->addChild(item);
            
            auto spriteCountBG = Sprite::create("Assets/ui_common/base_box_1_4.png");
            spriteCountBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            spriteCountBG->setPosition(item->getContentSize().width - 10, 10);
            spriteCountBG->setScale(0.7f);
            item->addChild(spriteCountBG);
            {
                auto labelCount = Label::createWithTTF(MafUtils::toString(listHaveBadge.size()), GAME_FONT, 32);
                labelCount->setColor(COLOR_TITLE);
                labelCount->setPosition(spriteCountBG->getContentSize().width / 2, spriteCountBG->getContentSize().height / 2);
                spriteCountBG->addChild(labelCount);
            }
            
            auto spritePlus = Sprite::create("Assets/ui_common/plus_3.png");
            spritePlus->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            spritePlus->setPosition(item->getContentSize().width - 10, item->getContentSize().height - 10);
            spritePlus->setVisible(false);
            spritePlus->setScale(1.2f);
            spritePlus->setName("SPRITE_PLUS");
            item->addChild(spritePlus);
            
            //
            _listBadgeHaveItem.pushBack(item);
        }
    }
    
    
    return layerChild;
}
 
#pragma mark - draw
void PopupBadge::drawBadgeSelect()
{
    _layerContainerBadgeSelectInBox->removeAllChildren();
    
    //
    auto spriteBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png", Rect::ZERO, Rect(5,30,10,10));
    spriteBG->setContentSize(_layerContainerBadgeSelectInBox->getContentSize());
    spriteBG->setPosition(_layerContainerBadgeSelectInBox->getContentSize().width / 2, _layerContainerBadgeSelectInBox->getContentSize().height / 2);
    _layerContainerBadgeSelectInBox->addChild(spriteBG);
    
    //
    if ( _bEdit == false )
    {// edit button
        auto menu = MafNode::MafMenu::create();
        menu->setPosition(Vec2::ZERO);
        _layerContainerBadgeSelectInBox->addChild(menu);
        
        auto item = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/stat/badge_edit.png"), nullptr, CC_CALLBACK_1(PopupBadge::onClickEdit, this));
        item->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        item->setPosition(_layerContainerBadgeSelectInBox->getContentSize().width, _layerContainerBadgeSelectInBox->getContentSize().height);
        menu->addChild(item);
    }
    
    //
    cocos2d::Vector<InfoBadgeHave*> listSelect = _listBadgeSelect;
    if ( _bEdit == true )
    {
        listSelect = _listBadgeSelectTemp;
    }
    
    auto menuBadge = MafNode::MafMenu::create();
    menuBadge->setPosition(_layerContainerBadgeSelectInBox->getContentSize().width / 2, _layerContainerBadgeSelectInBox->getContentSize().height / 2);
    _layerContainerBadgeSelectInBox->addChild(menuBadge);
    for ( int i = 0; i < 5; i++ )
    {
        auto labelBG = Layer::create();
        labelBG->setContentSize(Size(115, 115));
        
        auto item = MafNode::MafMenuItemSprite::create(labelBG, nullptr, CC_CALLBACK_1(PopupBadge::onClickUnSelect, this));
        menuBadge->addChild(item);
        
        auto spriteEmpty = Sprite::create("Assets/ui/stat/badge_plus1_2.png");
        spriteEmpty->setPosition(item->getContentSize().width / 2, item->getContentSize().height / 2);
        spriteEmpty->setVisible(false);
        item->addChild(spriteEmpty);
        
        //
        if ( listSelect.size() > i )
        {
            auto objBadgeHave = listSelect.at(i);
            
            //
            item->setTag(objBadgeHave->getIdx());
            item->setEnabled(true);
            //
            auto objBadge = BadgeManager::getInstance()->getBadge(objBadgeHave->getIdxBadge());
            if ( objBadge == nullptr )
            {
                spriteEmpty->setVisible(true);
                continue;
            }
            
            auto spriteIcon = Sprite::create(objBadge->getPath());
            if(spriteIcon == nullptr)
            {
                spriteIcon = Sprite::create("Assets/ui/stat/badge_000.png");
            }
            spriteIcon->setPosition(item->getContentSize().width / 2, item->getContentSize().height / 2);
            item->addChild(spriteIcon);
            
            if ( _bEdit == true )
            {
                auto spriteMinus = Sprite::create("Assets/ui_common/minus_1.png");
                spriteMinus->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                spriteMinus->setPosition(item->getContentSize().width - 10, item->getContentSize().height - 10);
                spriteMinus->setScale(1.2f);
                item->addChild(spriteMinus);
            }
        }
        else
        {
            spriteEmpty->setVisible(true);
            item->setEnabled(false);
        }
    }
    menuBadge->alignItemsHorizontallyWithPadding(0);
    
    float fAtkValue = BadgeManager::getInstance()->getBadgeBuffCount(E_BADGE_BUFF::BADGE_ATK);
    
    int nAtkValue = fAtkValue * 100;
    
    _labelTotalBadgeAtkBuff->setString(GAME_TEXTFORMAT("t_ui_badge_totalspec", nAtkValue));
}

void PopupBadge::drawBadgeHave()
{
    for ( int i = 0; i < _listBadgeHaveItem.size(); i++ )
    {
        auto item =  _listBadgeHaveItem.at(i);
        
        auto spritePlus = item->getChildByName("SPRITE_PLUS");
        spritePlus->setVisible(_bEdit);
    }
}
 
#pragma mark - set, get

#pragma mark - click
void PopupBadge::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupBadge::onClickEdit(cocos2d::Ref* sender)
{
    //
    _bEdit = !_bEdit;
    
    _listBadgeSelect.clear();
    _listBadgeSelect = BadgeManager::getInstance()->getListSelect();
    _listBadgeSelectTemp.clear();
    _listBadgeSelectTemp = BadgeManager::getInstance()->getListSelect();
    
    //
    drawBadgeSelect();
    drawBadgeHave();
    
    if ( _bEdit == false )
    {
        _layerContainerButton->setVisible(false);
        _layerContainerBadgeHave->setContentSize( Size(_spriteContainer->getContentSize().width, _spriteContainer->getContentSize().height - 75 - _layerContainerBadgeSelect->getContentSize().height - 25) );
    }
    else
    {
        _layerContainerButton->setVisible(true);
        _layerContainerBadgeHave->setContentSize( Size(_spriteContainer->getContentSize().width, _spriteContainer->getContentSize().height - 75 - _layerContainerBadgeSelect->getContentSize().height - 25 - _layerContainerButton->getContentSize().height) );
    }
    
    //
    auto layerScroll = (extension::ScrollView*)_layerContainerBadgeHave->getChildByName("SCROLLVIEW");
    layerScroll->setViewSize(Size(_layerContainerBadgeHave->getContentSize().width, _layerContainerBadgeHave->getContentSize().height));
    
    const Vec2 minOffset = layerScroll->minContainerOffset();
    const Vec2 maxOffset = layerScroll->maxContainerOffset();
    Vec2 offset = layerScroll->getContentOffset();
    offset.x = MAX(minOffset.x, MIN(maxOffset.x, offset.x));
    offset.y = MAX(minOffset.y, MIN(maxOffset.y, offset.y));
    layerScroll->setContentOffset(offset);
}

void PopupBadge::onClickEditApply(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    
    int nTag = item->getTag();
    if ( nTag == 1 )
    {
        _bEdit = !_bEdit;
        
        //
        _listBadgeSelect.clear();
        _listBadgeSelect = BadgeManager::getInstance()->getListSelect();
        _listBadgeSelectTemp.clear();
        _listBadgeSelectTemp = BadgeManager::getInstance()->getListSelect();
        
        //
        drawBadgeSelect();
        drawBadgeHave();
        
        if ( _bEdit == false )
        {
            _layerContainerButton->setVisible(false);
            _layerContainerBadgeHave->setContentSize( Size(_spriteContainer->getContentSize().width, _spriteContainer->getContentSize().height - 75 - _layerContainerBadgeSelect->getContentSize().height - 25) );
        }
        else
        {
            _layerContainerButton->setVisible(true);
            _layerContainerBadgeHave->setContentSize( Size(_spriteContainer->getContentSize().width, _spriteContainer->getContentSize().height - 75 - _layerContainerBadgeSelect->getContentSize().height - 25 - _layerContainerButton->getContentSize().height) );
        }
        
        //
        auto layerScroll = (extension::ScrollView*)_layerContainerBadgeHave->getChildByName("SCROLLVIEW");
        layerScroll->setViewSize(Size(_layerContainerBadgeHave->getContentSize().width, _layerContainerBadgeHave->getContentSize().height));
        
        const Vec2 minOffset = layerScroll->minContainerOffset();
        const Vec2 maxOffset = layerScroll->maxContainerOffset();
        Vec2 offset = layerScroll->getContentOffset();
        offset.x = MAX(minOffset.x, MIN(maxOffset.x, offset.x));
        offset.y = MAX(minOffset.y, MIN(maxOffset.y, offset.y));
        layerScroll->setContentOffset(offset);
    }
    else
    {
        std::vector<int> listTemp;
        for ( int i = 0; i < 5; i++ )
        {
            if ( _listBadgeSelectTemp.size() > i )
            {
                auto objBadgeHave = _listBadgeSelectTemp.at(i);
                listTemp.push_back(objBadgeHave->getIdx());
            }
            else
            {
                listTemp.push_back(0);
            }
        }
        
        //
        PopupLoading::show();
        BadgeManager::getInstance()->requestUpdate(listTemp, CC_CALLBACK_1(PopupBadge::callbackUpdate, this));
    }
}

void PopupBadge::onClickSelect(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    
    int nIdx = item->getTag();
    if ( _bEdit == false )
    {
        //
        auto popup = PopupBadgeDetail::create(nIdx);
        popup->show();
    }
    else
    {
        auto listBadgeHave = BadgeManager::getInstance()->getListHaveFromBadgeIdx(nIdx);
        if (listBadgeHave.size() <= 0)
        {
            return;
        }
        
        if (_listBadgeSelectTemp.size() >= 5)
        {
            return;
        }
        
        for ( int i = 0; i < listBadgeHave.size(); i++ )
        {
            auto objBadgeHave = listBadgeHave.at(i);
            
            bool bFind = false;
            for ( int i = 0; i < _listBadgeSelectTemp.size(); i++ )
            {
                auto objBadgeSelect = _listBadgeSelectTemp.at(i);
                if ( objBadgeSelect->getIdx() == objBadgeHave->getIdx() )
                {
                    bFind = true;
                    break;
                }
            }
            
            if (bFind == false)
            {
                _listBadgeSelectTemp.pushBack(objBadgeHave);
                break;
            }
            
        }
        
        //
        drawBadgeSelect();
    }
}

void PopupBadge::onClickUnSelect(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    
    int nIdx = item->getTag();
    if ( _bEdit == false )
    {
        auto objBadgeHave = BadgeManager::getInstance()->getBadgeHave(nIdx);
        auto objBadge = BadgeManager::getInstance()->getBadge(objBadgeHave->getIdxBadge());
        
        int nBonusValue = objBadge->getBuffCount() * 100;
        
        std::string strTime = UtilsDate::getInstance()->getTimeString(objBadgeHave->getCreated(), UtilsDate::eType::date_real_Ymd);
        std::string strText = GAME_TEXTFORMAT("t_ui_badge_created", strTime.c_str());
        strText.append("\n");
        strText.append(objBadge->getDesc());
        strText.append("\n");
        strText.append(GAME_TEXTFORMAT("t_ui_badge_spec", nBonusValue));
        
        Vec2 posToast = item->getParent()->convertToWorldSpace(item->getPosition());
        PopupToast::showImmediately(strText, posToast.x, posToast.y);
        
        auto action1 = DelayTime::create(1.0f);
        auto action2 = CallFuncN::create([=](Node* sender){
        
            auto item = (MafNode::MafMenuItemSprite*)sender;
            item->setEnabled(true);
        });
        auto seq = Sequence::create(action1, action2, NULL);
        item->runAction(seq);
        item->setEnabled(false);
    }
    else
    {
        for ( int i = 0; i < _listBadgeSelectTemp.size(); i++ )
        {
            auto objBadgeHave = _listBadgeSelectTemp.at(i);
            if ( objBadgeHave->getIdx() == nIdx )
            {
                _listBadgeSelectTemp.erase(_listBadgeSelectTemp.begin() + i);
                break;
            }
        }
        
        //
        drawBadgeSelect();
        drawBadgeHave();
    }
}

#pragma mark - network

#pragma mark - callback
void PopupBadge::callbackInfo(bool bResult)
{
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
        return;
    }
    
    _listBadgeSelect.clear();
    _listBadgeSelect = BadgeManager::getInstance()->getListSelect();
    _listBadgeSelectTemp.clear();
    _listBadgeSelectTemp = BadgeManager::getInstance()->getListSelect();
    
    //
    drawBadgeSelect();
    drawBadgeHave();
}

void PopupBadge::callbackUpdate(bool bResult)
{
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    _bEdit = false;
    
    //
    _listBadgeSelect.clear();
    _listBadgeSelect = BadgeManager::getInstance()->getListSelect();
    _listBadgeSelectTemp.clear();
    _listBadgeSelectTemp = BadgeManager::getInstance()->getListSelect();
    
    //
    if ( _bEdit == false )
    {
        _layerContainerButton->setVisible(false);
        _layerContainerBadgeHave->setContentSize( Size(_spriteContainer->getContentSize().width, _spriteContainer->getContentSize().height - 75 - _layerContainerBadgeSelect->getContentSize().height - 25) );
    }
    else
    {
        _layerContainerButton->setVisible(true);
        _layerContainerBadgeHave->setContentSize( Size(_spriteContainer->getContentSize().width, _spriteContainer->getContentSize().height - 75 - _layerContainerBadgeSelect->getContentSize().height - 25 - _layerContainerButton->getContentSize().height) );
    }
    
    //
    auto layerScroll = (extension::ScrollView*)_layerContainerBadgeHave->getChildByName("SCROLLVIEW");
    layerScroll->setViewSize(Size(_layerContainerBadgeHave->getContentSize().width, _layerContainerBadgeHave->getContentSize().height));
    
    const Vec2 minOffset = layerScroll->minContainerOffset();
    const Vec2 maxOffset = layerScroll->maxContainerOffset();
    Vec2 offset = layerScroll->getContentOffset();
    offset.x = MAX(minOffset.x, MIN(maxOffset.x, offset.x));
    offset.y = MAX(minOffset.y, MIN(maxOffset.y, offset.y));
    layerScroll->setContentOffset(offset);
    
    //
    drawBadgeSelect();
    drawBadgeHave();
}
