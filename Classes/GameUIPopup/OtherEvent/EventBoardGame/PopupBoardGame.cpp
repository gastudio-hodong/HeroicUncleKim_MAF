//
//  PopupBoardGame.cpp
//  FantasyClicker-mobile
//
//  Created by TG on 2020/07/08.
//

#include "PopupBoardGame.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoBoard.h"

#include "GameUIPopup/OtherEvent/EventBoardGame/PopupDice.h"
#include "GameUIPopup/OtherEvent/EventBoardGame/PopupBoardReward.h"
#include "GameUIPopup/OtherEvent/EventBoardGame/PopupBoardGameRandomBox.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerEvent/EventBoardGameManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupBoardGame* PopupBoardGame::create()
{
    PopupBoardGame* pRet = new(std::nothrow) PopupBoardGame();
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

PopupBoardGame::PopupBoardGame(void) :
_nBoardPosition(0),

_nResultDice(0),
_strResultReward(""),

_callbackClose(nullptr),

_layerContainerBoard(nullptr),
_layerContainerInfo(nullptr),

_labelPoint(nullptr),
_labelPearl(nullptr),
_labelDicePrice(nullptr),

_spriteCharacter(nullptr)
{
    
}

PopupBoardGame::~PopupBoardGame(void)
{
    
}

bool PopupBoardGame::init()
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

void PopupBoardGame::onEnter()
{
    PopupBase::onEnter();
    
    //
    PopupLoading::show();
    EventBoardGameManager::getInstance()->requestBoardInfo(CC_CALLBACK_2(PopupBoardGame::callbackInfo, this));
}

#pragma mark - init
void PopupBoardGame::initVar()
{
    _listBoard = EventBoardGameManager::getInstance()->getListBoard();
}

void PopupBoardGame::initUi()
{
    Size size = Size(750, 1302);
    
    _spriteContainer->setContentSize(size);
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(0);
    
    auto spriteBG = Sprite::create("Assets/ui/event/event_bg_2_1.png");
    spriteBG->getTexture()->setTexParameters(texParams);
    spriteBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    spriteBG->setScale(4);
    spriteBG->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height/2 + 5);
    _spriteContainer->addChild(spriteBG);

    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_boardgame_logo"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    _layerContainerInfo = Layer::create();
    _layerContainerInfo->setContentSize( Size(_spriteContainer->getContentSize().width, 250) );
    _layerContainerInfo->setIgnoreAnchorPointForPosition(false);
    _layerContainerInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerInfo->setPosition(_spriteContainer->getContentSize().width / 2, 0);
    _spriteContainer->addChild(_layerContainerInfo);
    
    _layerContainerBoard = Layer::create();
    _layerContainerBoard->setContentSize( Size(_spriteContainer->getContentSize().width, _spriteContainer->getContentSize().height - 75 - _layerContainerInfo->getContentSize().height) );
    _layerContainerBoard->setIgnoreAnchorPointForPosition(false);
    _layerContainerBoard->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerBoard->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_layerContainerBoard);
     
    //
    uiBoard();
    uiInfo();
}

#pragma mark - ui
void PopupBoardGame::uiBoard()
{
    _layerContainerBoard->removeAllChildren();
    
    double nWidth = 93;
    double nHeight = 86;
    
    int nBoardTileColume = 6;
    
    double posStartX = _layerContainerBoard->getContentSize().width / 2 - (nWidth * nBoardTileColume  / 2);
    double posStartY = _layerContainerBoard->getContentSize().height - 90;
    for ( int i = 0; i < _listBoard.size(); i++ )
    {
        auto objBoard = _listBoard.at(i);
        auto objItems = objBoard->getItems();
        
        int nCol = objBoard->getTilePosX();
        int nRow = objBoard->getTilePosY();
        double nPosX = posStartX + nCol * nWidth + nWidth / 2;
        double nPosY = posStartY - nRow * nHeight - nHeight / 2;
    
        
        //
        std::string strTilePath = "Assets/ui/event/event_game_block1_1.png";
        if ( objItems != nullptr )
        {
            strTilePath = "Assets/ui/event/event_game_block1_2.png";
        }
        auto spriteTile = Sprite::create(strTilePath);
        spriteTile->setPosition(nPosX, nPosY);
        _layerContainerBoard->addChild(spriteTile);
        
        //
        objBoard->setTile(spriteTile);
        
        //
        if ( objItems != nullptr )
        {
            std::string strPath = objItems->getPath();
            
            if ( i == _listBoard.size() - 1 )
            {
                strPath = "Assets/ui/event/event_game_block_reward.png";
            }
            
            
            auto spriteIcon = Sprite::create(strPath);
            spriteIcon->setPosition(spriteTile->getContentSize().width / 2, spriteTile->getContentSize().height / 2);
            spriteIcon->getTexture()->setTexParameters(texParams);
            spriteTile->addChild(spriteIcon);
            
            if ( objItems->getIdx() == (int)E_ITEMS::COSTUME_COIN || objItems->getIdx() == (int)E_ITEMS::MATERIAL )
            {
                auto spriteEffect = Sprite::create("Assets/ui/event/event_fx2_1.png");
                spriteEffect->setPosition(spriteTile->getContentSize().width / 2, spriteTile->getContentSize().height / 2);
                spriteTile->addChild(spriteEffect, -1);
            }
            
            if ( i == _listBoard.size() - 1 )
            {
                
            }
            else if ( objItems->getIdx() == 15002 )
            {
                auto btnBox = ui::Button::create("Assets/ui/event/event_game_block1_4.png");
                btnBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                btnBox->setPosition(spriteIcon->getPosition());
                btnBox->addClickEventListener([=](Ref*){
                    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_event_boardgame_reward_box_info"));
                    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                    popup->setContentSize(Size(655, 750));
                    popup->setContentAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                    popup->show();
                });
                btnBox->setZoomScale(0.05f);
                spriteTile->addChild(btnBox);
                
                auto fadeIn = FadeIn::create(0.5f);
                auto fadeOut = FadeOut::create(0.5f);
                
                btnBox->runAction(RepeatForever::create(Sequence::create(fadeIn, fadeOut, NULL)));
            }
            else
            {
                spriteIcon->setScale(2);
            }
        }
    }
    
    
    //
    _spriteCharacter = Sprite::create("Assets/ui/event/event_game_bs1_1.png");
    _spriteCharacter->setPosition(Vec2(0, 0));
    _spriteCharacter->getTexture()->setTexParameters(texParams);
    _spriteCharacter->setScale(2.7f);
    _layerContainerBoard->addChild(_spriteCharacter);
    
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.13f);
    for ( int i = 0; i < 3; i++ )
    {
        std::string strPath = MafUtils::format("Assets/ui/event/event_game_bs1_%d.png",i+2);
        
        animation->addSpriteFrameWithFile(strPath);
        animation->getFrames().at(i)->getSpriteFrame()->getTexture()->setTexParameters(texParams);
    }
    _spriteCharacter->runAction(RepeatForever::create(Animate::create(animation)));
}

void PopupBoardGame::uiInfo()
{
    _layerContainerInfo->removeAllChildren();
    
    // text
//    auto labelGuide = Label::createWithTTF(GAME_TEXT("t_ui_event_dice_guide"), GAME_FONT, 28);
//    labelGuide->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
//    labelGuide->setPosition(_layerContainerInfo->getContentSize().width / 2, _layerContainerInfo->getContentSize().height);
//    labelGuide->setDimensions(_layerContainerInfo->getContentSize().width - 20, 100);
//    labelGuide->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
//    labelGuide->setOverflow(Label::Overflow::SHRINK);
//    labelGuide->enableOutline(Color4B::BLACK, 1);
//    _layerContainerInfo->addChild(labelGuide);
    
    
    auto layerBG = Layer::create();
    layerBG->setContentSize(Size(716, 140));
    layerBG->setIgnoreAnchorPointForPosition(false);
    layerBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layerBG->setPosition(_layerContainerInfo->getContentSize().width / 2, 10);
    _layerContainerInfo->addChild(layerBG);
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    layerBG->addChild(menu);
    
    // left
    { // 116
        auto sprBox1 = Sprite::create("Assets/ui/event/event_assets_box1_1.png");
        sprBox1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        sprBox1->setPosition(116, layerBG->getContentSize().height / 2 + 5);
        layerBG->addChild(sprBox1);
        {//포인트
            auto spriteIcon = Sprite::create("Assets/ui/event/icon_event_dice.png");
            spriteIcon->setPosition(spriteIcon->getContentSize().width/2, sprBox1->getContentSize().height / 2);
            sprBox1->addChild(spriteIcon);

            _labelPoint = MafNode::createBmFont("0", GAME_BM_FONT_AL, 25);
            _labelPoint->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            _labelPoint->setPosition(spriteIcon->getContentSize().width, spriteIcon->getContentSize().height/2);
            _labelPoint->setColor(Color3B(255,212,0));
            spriteIcon->addChild(_labelPoint);
        }

        auto sprBox2 = Sprite::create("Assets/ui/event/event_assets_box1_1.png");
        sprBox2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        sprBox2->setPosition(116, layerBG->getContentSize().height / 2 - 5);
        layerBG->addChild(sprBox2);
        {//완주 티켓
            std::string iconPath = "Assets/ui/event/icon_event_map.png";
            if ( auto objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::EVENT_HALLOWEEN_CANDY); objItem != nullptr )
            {
                iconPath = objItem->getPath();
            }
            
            auto spriteIcon = Sprite::create(iconPath);
            spriteIcon->setPosition(spriteIcon->getContentSize().width/2, sprBox2->getContentSize().height / 2);
            sprBox2->addChild(spriteIcon);

            _labelPearl = MafNode::createBmFont("0", GAME_BM_FONT_AL, 25);
            _labelPearl->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            _labelPearl->setPosition(spriteIcon->getContentSize().width, spriteIcon->getContentSize().height/2);
            _labelPearl->setColor(Color3B(45,191,239));
            spriteIcon->addChild(_labelPearl);
        }
    }
    
    // center
    {
        auto spriteDice = ui::Scale9Sprite::create("Assets/ui/event/event_btn_02.png");
        spriteDice->setContentSize(Size(232, 110));
        
        auto itemDice = MafNode::MafMenuItemSprite::create(spriteDice, nullptr, CC_CALLBACK_1(PopupBoardGame::onClickDice, this));
        itemDice->setPosition(layerBG->getContentSize().width / 2, layerBG->getContentSize().height / 2);
        menu->addChild(itemDice);
        {
            auto lbDiceTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_dice_run_btn"), GAME_FONT, 30);
            lbDiceTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            lbDiceTitle->setPosition(itemDice->getContentSize().width / 2, itemDice->getContentSize().height / 2);
            lbDiceTitle->setDimensions(itemDice->getContentSize().width - 10, lbDiceTitle->getContentSize().height);
            lbDiceTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbDiceTitle->setOverflow(Label::Overflow::SHRINK);
            itemDice->addChild(lbDiceTitle);

            auto sprPoint = Sprite::create("Assets/ui/event/icon_event_dice.png");
            sprPoint->setPosition(itemDice->getContentSize().width * 0.35, itemDice->getContentSize().height * 0.25);
            sprPoint->setScale(1.5f);
            itemDice->addChild(sprPoint);
            {
                _labelDicePrice = MafNode::createBmFont(MafUtils::toString(ParameterManager::getInstance()->getParameterInteger("event_board_dice_price")), GAME_BM_FONT_AL, 22);
                _labelDicePrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                _labelDicePrice->setPosition(sprPoint->getContentSize().width - 10, sprPoint->getContentSize().height / 2 + 5);
                _labelDicePrice->setColor(Color3B(255,212,0));
                sprPoint->addChild(_labelDicePrice);
            }
        }
    }
    
    // right
    {
        auto spriteClose = ui::Scale9Sprite::create("Assets/ui_common/btn_2_2_on.png");
        spriteClose->setContentSize(Size(232, 110));
        
        auto itemClose = MafNode::MafMenuItemSprite::create(spriteClose, nullptr, CC_CALLBACK_1(PopupBoardGame::onClickClose, this));
        itemClose->setPosition(600, layerBG->getContentSize().height / 2);
        menu->addChild(itemClose);
        {
            auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_close"), GAME_FONT, 35);
            labelText->setPosition(itemClose->getContentSize().width / 2, itemClose->getContentSize().height / 2);
            labelText->setDimensions(itemClose->getContentSize().width - 10, labelText->getContentSize().height - 10);
            labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelText->setOverflow(Label::Overflow::SHRINK);
            itemClose->addChild(labelText);
        }
    }
    
    //
    drawInfo();
}

#pragma mark - draw
void PopupBoardGame::drawInfo()
{
    _labelPoint->setString(MafUtils::toString(EventBoardGameManager::getInstance()->getPointCount()));
    _labelPearl->setString(MafUtils::toString(EventBoardGameManager::getInstance()->getTotalCandy()));
    
    _labelDicePrice->setString(MafUtils::toString(ParameterManager::getInstance()->getParameterInteger("event_board_dice_price")));
}

void PopupBoardGame::drawBoard()
{
    
}

void PopupBoardGame::drawBoardCharacter()
{
    auto objBoard = _listBoard.at(_nBoardPosition);
    if ( objBoard == nullptr )
    {
        return;
    }
    
    auto spriteTile = objBoard->getTile();
    if ( spriteTile == nullptr )
    {
        return;
    }
    if((_nBoardPosition >= 6 && _nBoardPosition <= 12)||
       (_nBoardPosition >= 22 && _nBoardPosition <= 31))
    {
        _spriteCharacter->setFlippedX(true);
    }
    else
        _spriteCharacter->setFlippedX(false);
    //
    _spriteCharacter->setPosition(spriteTile->getPosition());
}

#pragma mark - set, get, sort, add
void PopupBoardGame::setCallbackClose(const std::function<void(void)>& callback)
{
    _callbackClose = callback;
}

#pragma mark - click
void PopupBoardGame::onClickClose(Ref* sender)
{
    if(_callbackClose != nullptr)
        _callbackClose();
    
    hide();
}

void PopupBoardGame::onClickDice(Ref* sender)
{
    if(EventManager::getInstance()->isActivatedEvent(26) == false)
    {
        hide();
        return;
    }
    
    if ( EventBoardGameManager::getInstance()->getPointCount() < ParameterManager::getInstance()->getParameterInteger("event_board_dice_price") )
    {
        auto popup = PopupDice::create(0);
        popup->setCallbackClose([=](){
            if(_callbackClose != nullptr)
                _callbackClose();
            
            hide();
        });
        popup->show();
        return;
    }
    
    PopupLoading::show();
    EventBoardGameManager::getInstance()->requestBoardReward(CC_CALLBACK_4(PopupBoardGame::callbackDice, this));
}
 
#pragma mark - callback
void PopupBoardGame::callbackInfo(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_2");
        if ( nResult == 2 )
            strMessage = GAME_TEXT("t_ui_error_6");
            
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){

            hide();

        });
        popup->show();
        return;
    }
    
    //
    _nBoardPosition = EventBoardGameManager::getInstance()->getBoardPosition();
    
    //
    drawInfo();
    drawBoard();
    drawBoardCharacter();
}

void PopupBoardGame::callbackDice(bool bResult, int nResult, int nDice, std::string strReward)
{
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_2");
        if ( nResult == 2 )
            strMessage = GAME_TEXT("t_ui_error_6");
            
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){

            hide();

        });
        popup->show();
        return;
    }
    
    //
    _nResultDice = nDice;
    _strResultReward = strReward;
    
    //
    drawInfo();
    drawBoard();
    
    //
    PopupLoadingLock::show();
    
    //
    auto popup = PopupDice::create(nDice);
    popup->setCallbackClose([=](){
        
        Vector<FiniteTimeAction*> listAction;
        
        //
        int nPosition = _nBoardPosition;
        _nBoardPosition = EventBoardGameManager::getInstance()->getBoardPosition();
        for ( int i = 0; i < _nResultDice; i++ )
        {
            nPosition += 1;
    
            if ( _listBoard.size() <= nPosition )
                continue;
            
            auto objBoard = _listBoard.at(nPosition);
            if ( objBoard == nullptr )
                continue;
    
            auto spriteTile = objBoard->getTile();
            if ( spriteTile == nullptr )
                continue;
    
            auto actionMove = MoveTo::create(0.3, spriteTile->getPosition());
            
            listAction.pushBack(actionMove);
            
            if((nPosition >= 6 && nPosition <= 12)||
               (nPosition >= 22 && nPosition <= 31))
            {
                listAction.pushBack(FlipX::create(true));
            }
            else
                listAction.pushBack(FlipX::create(false));
        }
        
        auto actionCall = CallFunc::create([=](){

            //
            PopupLoadingLock::hide();
            
            if(_listBoard.size() > nPosition)
            {
                auto objBoard = _listBoard.at(nPosition);
                //이 위치에 랜덤 박스
                if(objBoard != nullptr && objBoard->getItems() != nullptr)
                {
                    if(objBoard->getItems()->getIdx() == 15002)
                    {
                        auto listItems = ItemsManager::getInstance()->getConvertArray(strReward);
                        InfoItems* item = nullptr;
                        if ( listItems.size() != 0 )
                        {
                            item = listItems.at(0);
                        }
                        
                        auto popup = PopupBoardGameRandomBox::create(item);
                        popup->setCallbackClose(CC_CALLBACK_0(PopupBoardGame::callbackRandBox, this));
                        popup->show();
                        return;
                    }
                }
            }
            
            bool bFinish = false;
            if ( _listBoard.size() - 1 <= _nBoardPosition )
            {
                bFinish = true;
            }
            
            auto popup = PopupBoardReward::create(_strResultReward, bFinish);
            popup->show();
            
            drawBoardCharacter();
        });
        listAction.pushBack(actionCall);

        auto seq = Sequence::create(listAction);
        _spriteCharacter->runAction(seq);
    });
    popup->show();
}

void PopupBoardGame::callbackRandBox()
{
    drawInfo();
    drawBoard();
    drawBoardCharacter();
}
