//
//  PopupStoryDownload.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupStoryDownload.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/Other/Story/PopupStory.h"

#include "ManagerGame/StoryManager.h"

USING_NS_CC;

PopupStoryDownload* PopupStoryDownload::create(int nIdx)
{
    PopupStoryDownload* pRet = new(std::nothrow) PopupStoryDownload();
    if ( pRet && pRet->init(nIdx) )
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

PopupStoryDownload::PopupStoryDownload(void) :
_nIdx(0),
_nDownloadTotal(0),
_nDownloadSuccess(0),
_nDownloadError(0),
_nDownloadPercent(0),
_nTimeHide(0),
_nTimeHideMax(0),

_layerContainerContents(nullptr),
_layerContainerButton(nullptr),
_progressStory(nullptr),
_labelProgressProgress(nullptr),
_labelProgressCount(nullptr),
_labelButtonClose(nullptr)
{
    
}

PopupStoryDownload::~PopupStoryDownload(void)
{
    
}

bool PopupStoryDownload::init(int nIdx)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    _nIdx = nIdx;
    
    // init
    initVar();
    initUi();
    
    return true;
}

void PopupStoryDownload::onEnter()
{
    PopupBase::onEnter();
    //
    this->schedule(schedule_selector(PopupStoryDownload::scheduleHide), 1.0);
}

#pragma mark - override
void PopupStoryDownload::show()
{
    auto scene = UtilsScene::getInstance()->getRunningScene();
    if ( scene == nullptr )
    {
        return;
    }
    
    if ( scene != nullptr )
    {
        scene->addChild(this, ZORDER_SCENE_POPUP_STORY, TAG_SCENE_POPUP);
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

#pragma mark - init
void PopupStoryDownload::initVar()
{
    //
    _nTimeHide = 6;
    _nTimeHideMax = 6;
}

void PopupStoryDownload::initUi()
{
    Size size = Size(497, 400);

    //
    _spriteContainer->setContentSize( size );
    
    //
    _layerContainerButton = Layer::create();
    _layerContainerButton->setContentSize( Size(size.width - 50, 89) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_layerContainerButton);
    
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, size.height - 75 - 25 - 25 - _layerContainerButton->getContentSize().height - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25) );
    _spriteContainer->addChild(_layerContainerContents);
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_store"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    
    //
    uiContents();
    uiButton();
}


#pragma mark - ui
void PopupStoryDownload::uiContents()
{
    //
    _layerContainerContents->removeAllChildren();
  
    auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_story_msg_2"), GAME_FONT, 24);
    labelText->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height - 40);
    labelText->setDimensions(_layerContainerContents->getContentSize().width * 0.9, labelText->getContentSize().height);
    labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelText->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(labelText);
    
    // progress
    auto spriteProgressBG = Sprite::create("Assets/ui/story/comic_down_progressbar_1.png");
    spriteProgressBG->setPosition(_layerContainerContents->getContentSize().width / 2, 50);
    _layerContainerContents->addChild(spriteProgressBG);
    {
        _progressStory = ProgressTimer::create(Sprite::create("Assets/ui/story/comic_down_progressbar_2.png"));
        _progressStory->setPosition(spriteProgressBG->getContentSize()/2);
        _progressStory->setType(ProgressTimer::Type::BAR);
        _progressStory->setPercentage(0);
        _progressStory->setBarChangeRate(Vec2(1,0));
        _progressStory->setMidpoint(Vec2(0, 0.5));
        spriteProgressBG->addChild(_progressStory);
        
        _labelProgressProgress = Label::createWithTTF("0%", GAME_FONT, 16);
        _labelProgressProgress->setPosition(spriteProgressBG->getContentSize().width / 2, spriteProgressBG->getContentSize().height / 2);
        spriteProgressBG->addChild(_labelProgressProgress);
        
        std::string strProgressCount = MafUtils::format("(0/%d)", StoryManager::getInstance()->getStoryDonwnloadFileCount(_nIdx));
        _labelProgressCount = Label::createWithTTF(strProgressCount, GAME_FONT, 16);
        _labelProgressCount->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        _labelProgressCount->setPosition(spriteProgressBG->getContentSize().width, 0);
        spriteProgressBG->addChild(_labelProgressCount);
    }
}

void PopupStoryDownload::uiButton()
{
    //
    _layerContainerButton->removeAllChildren();
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr, CC_CALLBACK_1(PopupStoryDownload::onClickClose, this));
    menu->addChild(itemClose);
    {
        std::string strTemp = GAME_TEXT("t_ui_no");
        strTemp.append(MafUtils::format("(%d)", _nTimeHide));
        
        _labelButtonClose = Label::createWithTTF(strTemp, GAME_FONT, 24);
        _labelButtonClose->setPosition(itemClose->getContentSize().width / 2, itemClose->getContentSize().height / 2);
        itemClose->addChild(_labelButtonClose);
    }
    
    auto itemDownload = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr, CC_CALLBACK_1(PopupStoryDownload::onClickDownload, this));
    menu->addChild(itemDownload);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_yes"), GAME_FONT, 24);
        labelText->setPosition(itemDownload->getContentSize().width / 2, itemDownload->getContentSize().height / 2);
        itemDownload->addChild(labelText);
    }
    
    //
    menu->alignItemsHorizontallyWithPadding(10);
}

void PopupStoryDownload::drawProgress()
{
    //
    _progressStory->setPercentage(_nDownloadPercent);
    
    //
    _labelProgressProgress->setString(MafUtils::format("%d%%", (int)_nDownloadPercent));
    
    //
    _labelProgressCount->setString(MafUtils::format("(%d/%d)", _nDownloadSuccess, _nDownloadTotal));
    _labelProgressCount->setColor(Color3B::WHITE);
    if ( _nDownloadError > 0 )
    {
        _labelProgressCount->setColor(Color3B::RED);
    }
}

void PopupStoryDownload::drawButton()
{
    std::string strTemp = GAME_TEXT("t_ui_no");
    strTemp.append(MafUtils::format("(%d)", _nTimeHide));
    
    _labelButtonClose->setString(strTemp);
}

#pragma mark - set, get, other


#pragma mark - schedule
void PopupStoryDownload::scheduleHide(float dt)
{
    _nTimeHide -= 1;
    if ( _nTimeHide < 0 )
    {
        _nTimeHide = _nTimeHideMax;
        this->unschedule(schedule_selector(PopupStoryDownload::scheduleHide));
        
        //
        hide();
        return;
    }
    
    //
    drawButton();
}

#pragma mark - click
void PopupStoryDownload::onClickClose(cocos2d::Ref* sender)
{
    _nTimeHide = _nTimeHideMax;
    this->unschedule(schedule_selector(PopupStoryDownload::scheduleHide));
    
    //
    hide();
}

void PopupStoryDownload::onClickDownload(cocos2d::Ref* sender)
{
    _nTimeHide = _nTimeHideMax;
    this->unschedule(schedule_selector(PopupStoryDownload::scheduleHide));
    
    //
    StoryManager::getInstance()->download(_nIdx,
                                          CC_CALLBACK_1(PopupStoryDownload::callbackDownloadStart, this),
                                          CC_CALLBACK_1(PopupStoryDownload::callbackDownloadProgress, this),
                                          CC_CALLBACK_2(PopupStoryDownload::callbackDownloadCount, this),
                                          CC_CALLBACK_0(PopupStoryDownload::callbackDownloadEnd, this));
}

#pragma mark - callback
void PopupStoryDownload::callbackDownloadStart(int nTotal)
{
    _nDownloadTotal = nTotal;
    _nDownloadSuccess = 0;
    _nDownloadError = 0;

    //
    drawProgress();
}

void PopupStoryDownload::callbackDownloadProgress(double nPercent)
{
    _nDownloadPercent = nPercent;
    if ( _nDownloadPercent < 0 )
        _nDownloadPercent = 0;
    if ( _nDownloadPercent > 100 )
        _nDownloadPercent = 100;
    
    //
    drawProgress();
}

void PopupStoryDownload::callbackDownloadCount(int nSuccess, int nError)
{
    _nDownloadSuccess = nSuccess;
    _nDownloadError = nError;
    _nDownloadPercent = 100;
    
    //
    drawProgress();
}

void PopupStoryDownload::callbackDownloadEnd()
{
    if ( _nDownloadTotal == _nDownloadSuccess )
    {
        // open
        auto popup = PopupStory::create(_nIdx);
        popup->show();
        
        //
        hide();
    }
    else
    {
        //
        this->setLocalZOrder(ZORDER_SCENE_POPUP);
        
        //
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
}
