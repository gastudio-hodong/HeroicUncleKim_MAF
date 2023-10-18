//
//  PopupAdventureFriend.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/16.
//

#include "PopupAdventureFriend.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/Adventure/Guild/PopupAdventureMemberUpgrade.h"
#include "GameUIPopup/Other/Adventure/Map/PopupAdventureWorldMap.h"
#include "GameUIPopup/Other/Friend/PopupFriend.h"

#include "ManagerGame/AdventureManager.h"

USING_NS_CC;

PopupAdventureFriend* PopupAdventureFriend::create()
{
    PopupAdventureFriend* pRet = new(std::nothrow) PopupAdventureFriend();
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

PopupAdventureFriend::PopupAdventureFriend(void):
_lyTopUI(nullptr),
_lyFriends(nullptr),
_lyBottomUI(nullptr),
_table(nullptr)
{
    _listFriend.clear();
}

PopupAdventureFriend::~PopupAdventureFriend(void)
{
    _listFriend.clear();
}


#pragma mark - table
void PopupAdventureFriend::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupAdventureFriend::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 170);
}

extension::TableViewCell* PopupAdventureFriend::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    cell->setContentSize(size);
    
    auto objFriend = _listFriend.at(idx);
    if(objFriend == nullptr)
        return cell;
    
    auto sprBox = Sprite::create("Assets/ui/adventure/adventure_friend_list_01.png");
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setPosition(Vec2(cell->getContentSize()/2));
    sprBox->getTexture()->setTexParameters(texParams);
    sprBox->setScale(3);
    cell->addChild(sprBox);
    {
        Vec2 pos = Vec2(10, 2);
        for(int i = 0; i < 3; ++i)
        {
            std::string strPath = "Assets/ui/adventure/adventure_crew_%d_%d_1.png";
            
            auto sprMember = Sprite::create(MafUtils::format(strPath.c_str(), i+2, objFriend->getGrade()));
            sprMember->setPosition(pos);
            sprMember->getTexture()->setTexParameters(texParams);
            sprMember->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            sprBox->addChild(sprMember);
            {
                auto animation = Animation::create();
                animation->setDelayPerUnit(0.25f);
                for ( int j = 1; j <= 2; j++ )
                {
                    auto sprFrame = Sprite::create(MafUtils::format("Assets/ui/adventure/adventure_crew_%d_%d_%d.png", i+2, objFriend->getGrade(), j));
                    sprFrame->getTexture()->setTexParameters(texParams);
                    animation->addSpriteFrame(sprFrame->getSpriteFrame());
                }
                sprMember->runAction(RepeatForever::create(Animate::create(animation)));
            }
            pos += Vec2::RIGHT * (sprMember->getContentSize().width / 2 + 12);
        }
    }
    
    std::string strFriendName = "Lv.%02d %s";
    strFriendName = MafUtils::format(strFriendName.c_str(), objFriend->getLevel(), objFriend->getNickName().c_str());
    
    auto lbFriendName = Label::createWithTTF(strFriendName, GAME_FONT, 24);
    lbFriendName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbFriendName->setPosition(cell->getContentSize().width/2, cell->getContentSize().height - 24);
    lbFriendName->setDimensions(cell->getContentSize().width - 10,42);
    lbFriendName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lbFriendName->setOverflow(Label::Overflow::SHRINK);
    cell->addChild(lbFriendName);
    
    auto btnRunning = ui::Button::create("Assets/ui_common/btn_upgrade_bg_2.png");
    btnRunning->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnRunning->setPosition(Vec2(cell->getContentSize().width - 30 , cell->getContentSize().height/2));
    btnRunning->setZoomScale(0.05f);
    btnRunning->setTag(objFriend->getUseridx());
    btnRunning->addClickEventListener(CC_CALLBACK_1(PopupAdventureFriend::onClickRunning, this));
    btnRunning->setTitleText(GAME_TEXT("t_ui_advt_slot_friend"));
    btnRunning->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    btnRunning->getTitleLabel()->setDimensions(btnRunning->getContentSize().width - 10, btnRunning->getContentSize().height - 10);
    btnRunning->setTitleFontSize(30);
    btnRunning->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
    cell->addChild(btnRunning);
    if(objFriend->isAdventure())
    {
        btnRunning->setEnabled(false);
        btnRunning->setTitleText(GAME_TEXT("t_ui_advt_launch_reward_bt"));
    }
    
    
    return cell;
}

ssize_t PopupAdventureFriend::numberOfCellsInTableView(extension::TableView *table)
{
    return _listFriend.size();
}

#pragma mark -init
bool PopupAdventureFriend::init()
{
    if ( !PopupBase::init())
    {
        return false;
    }
    
    _listFriend = AdventureManager::getInstance()->getListInfoFriend();
    
    initVar();
    initUI();
    
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE, this);
    return true;
}

void PopupAdventureFriend::onEnter()
{
    PopupBase::onEnter();
    
}
#pragma mark - Override
void PopupAdventureFriend::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if(eType == E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE)
        this->setLocalZOrder(ZORDER_SCENE_POPUP);
}
void PopupAdventureFriend::show()
{
    PopupBase::show();
    
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        this->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
}
void PopupAdventureFriend::initVar()
{
    _spriteContainer->setContentSize(Size(750, 1172));
}

void PopupAdventureFriend::initUI()
{
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_advt_slot_friend"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAdventureFriend::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //650
    Size size = _spriteContainer->getContentSize();
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize( Size(size.width - 10, 140));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTopUI->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(_lyTopUI);
    
    _lyFriends = LayerColor::create();
    _lyFriends->setContentSize( Size(size.width - 10, _spriteContainer->getContentSize().height - 75 - _lyTopUI->getContentSize().height - 190));
    _lyFriends->setIgnoreAnchorPointForPosition(false);
    _lyFriends->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyFriends->setPosition( _lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    _spriteContainer->addChild(_lyFriends);
    
    _lyBottomUI = LayerColor::create();
    _lyBottomUI->setContentSize( Size(_spriteContainer->getContentSize().width - 10, _spriteContainer->getContentSize().height - _lyTopUI->getContentSize().height -_lyFriends->getContentSize().height - 75));
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyBottomUI->setPosition( _lyFriends->getPosition() + Vec2::DOWN * _lyFriends->getContentSize().height);
    _spriteContainer->addChild(_lyBottomUI);
    
    uiTop();
    uiTable();
    uiBottom();
}

#pragma mark -ui
void PopupAdventureFriend::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_friend_msg_1"), GAME_FONT, 28);
    lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbText->setPosition(_lyTopUI->getContentSize()/2);
    lbText->setDimensions(_lyTopUI->getContentSize().width - 10, _lyTopUI->getContentSize().height - 10);
    lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lbText->setOverflow(Label::Overflow::SHRINK);
    _lyTopUI->addChild(lbText);
    
}
void PopupAdventureFriend::uiTable()
{
    _lyFriends->removeAllChildren();
    
    _table = extension::TableView::create(this, Size(_lyFriends->getContentSize().width, _lyFriends->getContentSize().height - 10));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition(_lyFriends->getContentSize().width / 2, 0);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lyFriends->addChild(_table);
    
    _table->reloadData();
}

void PopupAdventureFriend::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    auto btnAddFriend = ui::Button::create("Assets/ui/adventure/adventure_friend_list_02.png");
    btnAddFriend->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    btnAddFriend->setPosition(Vec2(_lyBottomUI->getContentSize()/2));
    btnAddFriend->setScale(3);
    btnAddFriend->setZoomScale(0.05f);
    btnAddFriend->addClickEventListener(CC_CALLBACK_1(PopupAdventureFriend::onClickAddFriend, this));
    _lyBottomUI->addChild(btnAddFriend);
    
    auto sprPlust = Sprite::create("Assets/ui_common/plus_2.png");
    sprPlust->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprPlust->setPosition(Vec2(_lyBottomUI->getContentSize().width/3, _lyBottomUI->getContentSize().height/2));
    _lyBottomUI->addChild(sprPlust);
    
    auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_friend_add"), GAME_FONT, 24);
    lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbText->setPosition(sprPlust->getPosition() + Vec2::RIGHT * sprPlust->getContentSize().width);
    lbText->setDimensions(_lyBottomUI->getContentSize().width - _lyBottomUI->getContentSize().width/ 3, _lyBottomUI->getContentSize().height - 10);
    lbText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbText->setOverflow(Label::Overflow::SHRINK);
    _lyBottomUI->addChild(lbText);
}

#pragma mark -click
void PopupAdventureFriend::onClickClose(Ref* const pSender)
{
    hide();
}

void PopupAdventureFriend::onClickRunning(Ref* const pSender)
{
    auto node = (Node*)pSender;
    
    auto popup = PopupAdventureWorldMap::create(node->getTag(), true);
    popup->setCallbackHide(CC_CALLBACK_0(PopupAdventureFriend::callbackHide, this));
    popup->show();
}

void PopupAdventureFriend::onClickAddFriend(Ref* const pSender)
{
    auto popup = PopupFriend::create();
    popup->show();
}

#pragma mark -callback
void PopupAdventureFriend::callbackHide()
{
    hide();
}
