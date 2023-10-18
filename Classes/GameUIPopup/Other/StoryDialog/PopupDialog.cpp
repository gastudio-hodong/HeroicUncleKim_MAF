//
//  PopupDialog.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/19.
//

#include "PopupDialog.h"

#include "ManagerGame/DialogManager.h"

USING_NS_CC;

PopupDialog* PopupDialog::create(std::string strSceneName)
{
    PopupDialog* pRet = new(std::nothrow) PopupDialog();
    if ( pRet && pRet->init(strSceneName) )
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

PopupDialog::PopupDialog() :
_isEndDialog(false)
,_strSceneName("")
,_currentScene(nullptr)
,_lyImage(nullptr)
,_lyText(nullptr)
,_uiDialogText(nullptr)
,_sprNameBox(nullptr)
{
    _listInfoDialogScene.clear();
}

PopupDialog::~PopupDialog(void)
{
    _listInfoDialogScene.clear();
}
bool PopupDialog::init(std::string strSceneName)
{
    if (!PopupBase::init())
    {
        return false;
    }
    
    //
    setName("POPUP_STORY");
    
    //
    _strSceneName = strSceneName;
    _listInfoDialogScene = DialogManager::getInstance()->getListDialogScene(_strSceneName);
    _currentScene = getDialogScene(1);
    
    _eventDispatcher->removeEventListenersForTarget(this);
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *event)->bool {
        cocos2d::Rect rect = cocos2d::Rect(0, 0, this->getContentSize().width, this->getContentSize().height);
        if (rect.containsPoint(this->convertToNodeSpace(touch->getLocation())))
        {
            return true;
        }
        
        return false;
    };
    listener->onTouchEnded = [=](cocos2d::Touch *touch, cocos2d::Event *event)->void {
        onClickScreen(nullptr);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // init
    initVar();
    initUI();
    
    return true;
}

#pragma mark - override
void PopupDialog::show()
{
    PopupBase::show();
    setTag(TAG_SCENE_POPUP);
    setLocalZOrder(ZORDER_SCENE_POPUP_STORY);
}
#pragma mark - ui
void PopupDialog::initVar()
{
    _spriteContainer->setVisible(false);
}

void PopupDialog::initUI()
{
    auto lyBG = LayerColor::create(Color4B::BLACK);
    lyBG->setContentSize( getContentSize() );
    lyBG->setIgnoreAnchorPointForPosition(false);
    lyBG->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    lyBG->setPosition( Vec2(getContentSize().width / 2, 0) );
    lyBG->setOpacity(100);
    addChild(lyBG);
    
    auto ly = LayerColor::create();
    ly->setContentSize(Size(getContentSize().width, getContentSize().height - 385));

    auto clipper = ClippingNode::create(ly);
    clipper->setContentSize(ly->getContentSize());
    clipper->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    clipper->setPosition( getContentSize().width/2, getContentSize().height);
    clipper->setIgnoreAnchorPointForPosition(false);
    addChild(clipper);
    
    _lyImage = LayerColor::create();
    _lyImage->setContentSize( Size(getContentSize().width, getContentSize().height - 385) );
    _lyImage->setAnchorPoint( Vec2::ANCHOR_MIDDLE);
    _lyImage->setPosition(clipper->getContentSize()/2);
    _lyImage->setIgnoreAnchorPointForPosition(false);
    clipper->addChild(_lyImage);
    
    _lyText = LayerColor::create();
    _lyText->setContentSize( Size(getContentSize().width, 270) );
    _lyText->setIgnoreAnchorPointForPosition(false);
    _lyText->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _lyText->setPosition(getContentSize().width/2, 115);
    addChild(_lyText);
    
    auto btnClose = ui::Button::create("Assets/ui_common/btn_basic_box.png");
    btnClose->setScale9Enabled(true);
    btnClose->getRendererNormal()->setCapInsets(Rect(11, 10, 20, 20));
    btnClose->setContentSize(Size(140, 56));
    btnClose->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    btnClose->setPosition(Vec2(getContentSize().width - 5, getContentSize().height - 10));
    btnClose->setZoomScale(0.02f);
    btnClose->addClickEventListener(CC_CALLBACK_1(PopupDialog::onClickSkip, this));
    btnClose->setTitleText("SKIP");
    btnClose->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    btnClose->getTitleLabel()->setDimensions(btnClose->getContentSize().width - 10, btnClose->getContentSize().height - 10);
    btnClose->setTitleFontSize(20);
    btnClose->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
    addChild(btnClose);
    
    uiText();
    startScene();
}

void PopupDialog::uiText()
{
    _lyText->removeAllChildren();
    
    auto sprTextBox = ui::Scale9Sprite::create("Assets/ui_common/ui_talk_box.png", Rect::ZERO, Rect(13, 13, 19, 19));
    sprTextBox->setContentSize(Size(750, 284));
    sprTextBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprTextBox->setPosition(_lyText->getContentSize().width/2, _lyText->getContentSize().height);
    _lyText->addChild(sprTextBox);
    
    _uiDialogText = UIDialogueText::create(Size(sprTextBox->getContentSize().width - 20, sprTextBox->getContentSize().height - 60));
    _uiDialogText->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _uiDialogText->setPosition(Vec2(20, sprTextBox->getContentSize().height - 35));
    _uiDialogText->setTextAlignment(cocos2d::TextVAlignment::TOP, cocos2d::TextHAlignment::LEFT);
    _uiDialogText->setTextAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _uiDialogText->setTextPosition(Vec2(0, _uiDialogText->getContentSize().height));
    _uiDialogText->setCallbackEndEvent(CC_CALLBACK_0(PopupDialog::onEventNextStep, this));
    sprTextBox->addChild(_uiDialogText);
    
    auto lbNext = Label::createWithTTF("NEXT", GAME_FONT, 24);
    lbNext->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    lbNext->setPosition(sprTextBox->getContentSize().width - 50, 15);
    lbNext->setDimensions(100, lbNext->getContentSize().height);
    lbNext->setAlignment(TextHAlignment::RIGHT, TextVAlignment::BOTTOM);
    lbNext->setOverflow(Label::Overflow::SHRINK);
    sprTextBox->addChild(lbNext);
    {
        auto sprArrow = Sprite::create("Assets/ui_common/ui_talk_arrow.png");
        sprArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprArrow->setPosition(lbNext->getContentSize().width + 5, lbNext->getContentSize().height/2);
        lbNext->addChild(sprArrow);
        
        auto action1 = CallFunc::create([=](){
            if((_uiDialogText->isLineEnd() && !_currentScene->isEndMotionStart()) || _isEndDialog)
                lbNext->setVisible(true);
            else
                lbNext->setVisible(false);
        });
        auto action2 = DelayTime::create(0.1f);
        auto seq = Sequence::create(action1, action2, NULL);
        lbNext->runAction(RepeatForever::create(seq));
    }
    
    _sprNameBox = Sprite::create("Assets/ui_common/ui_talk_name_box.png");
    _sprNameBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _sprNameBox->setPosition(10, sprTextBox->getContentSize().height);
    sprTextBox->addChild(_sprNameBox);
    {
        auto lbName = Label::createWithTTF("", GAME_FONT, 25);
        lbName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbName->setPosition(_sprNameBox->getContentSize()/2);
        lbName->setDimensions(_sprNameBox->getContentSize().width - 50, _sprNameBox->getContentSize().height - 5);
        lbName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbName->setOverflow(Label::Overflow::SHRINK);
        lbName->setColor(COLOR_TITLE);
        lbName->setName("NAME");
        _sprNameBox->addChild(lbName);
    }
}

#pragma mark - scene
void PopupDialog::startScene()
{
    setCurrentName();
    setCurrentText();
    setCurrentActor();
}

void PopupDialog::nextScene()
{
    if(_currentScene->getSceneIdx() >= _listInfoDialogScene.size())
    {
        _isEndDialog = true;
        return;
    }
    
    _currentScene = getDialogScene(_currentScene->getSceneIdx() + 1);
    startScene();
}
void PopupDialog::setCurrentName()
{
    auto parent = _sprNameBox->getParent();
    
    if(_currentScene->getSpeakerPosition().compare("LEFT") == 0)
    {
        _sprNameBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        _sprNameBox->setPosition(30, parent->getContentSize().height);
    }
    else if(_currentScene->getSpeakerPosition().compare("RIGHT") == 0)
    {
        _sprNameBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _sprNameBox->setPosition(parent->getContentSize().width - 30, parent->getContentSize().height);
    }
    
    auto lbName = (Label*)_sprNameBox->getChildByName("NAME");
    if(lbName != nullptr)
    {
        lbName->setString(_currentScene->getSpeakerName());
    }
    
}
void PopupDialog::setCurrentText()
{
    _uiDialogText->setListText(_currentScene->getListText());
    _uiDialogText->startDialogue();
}
void PopupDialog::setCurrentActor()
{
    _lyImage->removeAllChildren();
    
    if(auto objSpeaker = DialogManager::getInstance()->getInfoActor(_currentScene->getSpeakerID()))
    {
        if(auto sprSpeaker = Sprite::create(objSpeaker->getImagePath()))
        {
            sprSpeaker->getTexture()->setTexParameters(texParams);
            sprSpeaker->setScale(4.f);
            sprSpeaker->setName("SPEAKER");
            _lyImage->addChild(sprSpeaker, 2);
            
            if(_currentScene->getSpeakerPosition().compare("LEFT") == 0)
            {
                sprSpeaker->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                sprSpeaker->setPosition(Vec2(-(20 * sprSpeaker->getScale()), -10 * sprSpeaker->getScale()));
            }
            else if(_currentScene->getSpeakerPosition().compare("RIGHT") == 0)
            {
                sprSpeaker->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
                sprSpeaker->setPosition(Vec2(_lyImage->getContentSize().width + (20 * sprSpeaker->getScale()), -10 * sprSpeaker->getScale()));
                sprSpeaker->setFlippedX(true);
            }
        }
    }
    
    if(auto objListner = DialogManager::getInstance()->getInfoActor(_currentScene->getListenerID()))
    {
        if(auto sprListner = Sprite::create(objListner->getImagePath()))
        {
            sprListner->getTexture()->setTexParameters(texParams);
            sprListner->setScale(4.f);
            sprListner->setName("LISTNER");
            _lyImage->addChild(sprListner, 1);
            
            if(_currentScene->getListenerPosition().compare("LEFT") == 0)
            {
                sprListner->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                sprListner->setPosition(Vec2(-(20 * sprListner->getScale()), -10 * sprListner->getScale()));
            }
            else if(_currentScene->getListenerPosition().compare("RIGHT") == 0)
            {
                sprListner->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
                sprListner->setPosition(Vec2(_lyImage->getContentSize().width + (20 * sprListner->getScale()), -10 * sprListner->getScale()));
                sprListner->setFlippedX(true);
            }
            sprListner->setColor(Color3B(100,100,100));
        }
    }
    
    setActorMotion("START");
}

void PopupDialog::setActorMotion(const std::string strType)
{
    if(strType.compare("START") == 0)
    {
        if(auto nodeSpeaker = _lyImage->getChildByName("SPEAKER"))
        {
            std::string strAction = _currentScene->getSpeakerStartMotionType();
            actionActorMotion(nodeSpeaker, strAction, strType);
        }
        if(auto nodeListner = _lyImage->getChildByName("LISTNER"))
        {
            std::string strAction = _currentScene->getListenerStartMotionType();
            actionActorMotion(nodeListner, strAction, strType);
        }
    }
    else if(strType.compare("END") == 0)
    {
        if(auto nodeSpeaker = _lyImage->getChildByName("SPEAKER"))
        {
            std::string strAction = _currentScene->getSpeakerEndMotionType();
            actionActorMotion(nodeSpeaker, strAction, strType);
        }
        if(auto nodeListner = _lyImage->getChildByName("LISTNER"))
        {
            std::string strAction = _currentScene->getListenerEndMotionType();
            actionActorMotion(nodeListner, strAction, strType);
        }
        _currentScene->setIsEndMotionStart(true);
    }
}

void PopupDialog::actionActorMotion(Node* node, const std::string strAction, const std::string strType)
{
    FiniteTimeAction* action = nullptr;
    
    if(strAction.compare("FADE_IN") == 0)
    {
        node->setOpacity(0);
        action = DialogManager::getInstance()->getAction(strAction, 0.3f);
    }
    if(strAction.compare("FADE_OUT") == 0)
    {
        action = DialogManager::getInstance()->getAction(strAction, 0.3f);
    }
    else if(strAction.compare("SHAKE") == 0)
    {
        action = DialogManager::getInstance()->getAction(strAction);
    }
    else if(strAction.compare("SLIDE_RIGHT_IN") == 0)
    {
        action = DialogManager::getInstance()->getAction("SLIDE_IN", 0.3f, Vec2(-node->getContentSize().width * node->getScale(), -10 * node->getScale()), node->getPosition());
    }
    else if(strAction.compare("SLIDE_LEFT_IN") == 0)
    {
        action = DialogManager::getInstance()->getAction("SLIDE_IN", 0.3f, Vec2(_lyImage->getContentSize().width + node->getContentSize().width * node->getScale(), -10 * node->getScale()), node->getPosition());
    }
    else if(strAction.compare("SLIDE_RIGHT_OUT") == 0)
    {
        action = DialogManager::getInstance()->getAction("SLIDE_OUT", 0.3f, node->getPosition(),  Vec2(_lyImage->getContentSize().width + node->getContentSize().width * node->getScale(), -10 * node->getScale()));
    }
    else if(strAction.compare("SLIDE_LEFT_OUT") == 0)
    {
        action = DialogManager::getInstance()->getAction("SLIDE_OUT", 0.3f, node->getPosition(), Vec2(-node->getContentSize().width * node->getScale(), -10 * node->getScale()));
    }
    
    if(strType.compare("END") == 0)
    {
        auto callEnd = CallFunc::create([=](){
           if(node->getName().compare("SPEAKER") == 0)
               _currentScene->setIsSpeakerEndMotionComplete(true);
            else if(node->getName().compare("LISTNER") == 0)
                _currentScene->setIsListenerEndMotionComplete(true);
            
            if(_currentScene->isEndMotionComplete())
            {
                if(_currentScene->getSceneIdx() >= _listInfoDialogScene.size())
                    _isEndDialog = true;
                onEventNextStep();
            }
        });
        
        action = Sequence::create(action, callEnd, nullptr);
    }
    if(action != nullptr)
        node->runAction(action);
}
#pragma mark - click
void PopupDialog::onClickSceneSkip(Ref* sender)
{
    if(_currentScene->getSceneIdx() >= _listInfoDialogScene.size())
    {
        hide();
        return;
    }
    
    nextScene();
}
void PopupDialog::onClickSkip(Ref* sender)
{
    hide();
}
void PopupDialog::onClickScreen(Ref* sender)
{
    onEventNextStep();
}

void PopupDialog::onEventNextStep()
{
    if(_isEndDialog)
    {
        hide();
        return;
    }
    
    if(_uiDialogText->isEnd())
    {
        if(_currentScene->isEndMotionComplete() || _currentScene->isEndMotionStart())
            nextScene();
        else
            setActorMotion("END");
    }
    else
    {
        _uiDialogText->onEventNextLine();
    }
}

#pragma mark - utils
InfoDialogScene* PopupDialog::getDialogScene(const int nIdx)
{
    InfoDialogScene* result = nullptr;
    
    for(auto obj : _listInfoDialogScene)
    {
        if(obj->getSceneIdx() == nIdx)
        {
            result = obj;
            break;
        }
    }
    return result;
}
