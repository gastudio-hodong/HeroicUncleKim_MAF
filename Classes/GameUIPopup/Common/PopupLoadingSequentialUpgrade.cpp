//
//  PopupLoadingSpiritRankUp.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/12/07.
//

#include "PopupLoadingSequentialUpgrade.h"

USING_NS_CC;

int PopupLoadingSequentialUpgrade::_nCountVisible = 0;
int PopupLoadingSequentialUpgrade::_nCurrentIdx = 0;
std::string PopupLoadingSequentialUpgrade::_strTextKey = "";
std::string PopupLoadingSequentialUpgrade::_strClosingRemark = "";

PopupLoadingSequentialUpgrade::PopupLoadingSequentialUpgrade(void) :
_labelText(nullptr)
{
    
}

PopupLoadingSequentialUpgrade::~PopupLoadingSequentialUpgrade(void)
{

}

bool PopupLoadingSequentialUpgrade::init(void)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    setContentSize(UtilsRect::getVisibleSize());
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setBackGroundColor(Color3B::BLACK);
    setBackGroundColorOpacity(165);
    setTouchEnabled(true);
    
    // ui
    uiLoading();
    
    return true;
}

#pragma mark - set, get

#pragma mark - ui
void PopupLoadingSequentialUpgrade::uiLoading()
{
    // container
    auto container = Node::create();
    container->setContentSize( Size(getContentSize().width, getContentSize().height) );
    container->setAnchorPoint( Vec2(0.5f, 0.5f) );
    container->setPosition( Vec2(getContentSize().width / 2, getContentSize().height / 2) );
    addChild(container);
    
    // run
    auto aniframeRun = Animation::create();
    aniframeRun->addSpriteFrameWithFile("Assets/ui_common/loading1.png");
    aniframeRun->addSpriteFrameWithFile("Assets/ui_common/loading2.png");
    aniframeRun->addSpriteFrameWithFile("Assets/ui_common/loading3.png");
    aniframeRun->addSpriteFrameWithFile("Assets/ui_common/loading4.png");
    aniframeRun->setDelayPerUnit(0.1f);
    
    auto aniRun = Animate::create(aniframeRun);
    
    auto sprintRun = Sprite::create("Assets/ui_common/loading1.png");
    sprintRun->setPosition( Vec2(getContentSize().width / 2, getContentSize().height / 2) );
    sprintRun->runAction(RepeatForever::create(aniRun));
    container->addChild(sprintRun);
    
    // text
    _labelText = Label::createWithTTF(GAME_TEXTFORMAT(PopupLoadingSequentialUpgrade::_strTextKey, PopupLoadingSequentialUpgrade::_nCurrentIdx) + PopupLoadingSequentialUpgrade::_strClosingRemark, GAME_FONT, 36);
    _labelText->setAnchorPoint( Vec2(0.5, 1) );
    _labelText->setPosition( Vec2(getContentSize().width / 2, getContentSize().height * 0.45) );
    container->addChild(_labelText);
//    {
//        auto func = CallFunc::create([&, labelText](){
//            labelText->setString(GAME_TEXTFORMAT("t_ui_spirit_rank_up_mag_3", PopupLoadingSpiritRankUp::_nIdxSpirit));
//        });
//        labelText->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1), func, nullptr)));
//    }
}
void PopupLoadingSequentialUpgrade::uiReload()
{
    _labelText->setString(GAME_TEXTFORMAT(PopupLoadingSequentialUpgrade::_strTextKey, PopupLoadingSequentialUpgrade::_nCurrentIdx) + " " +PopupLoadingSequentialUpgrade::_strClosingRemark);
}
#pragma mark - show, lock, hide
bool PopupLoadingSequentialUpgrade::isShow()
{
    bool ret = false;
    
    // notification remove
    auto node = Director::getInstance()->getNotificationNode();
    if ( node != nullptr && node->getChildByTag(TAG_NOTI_LOADING_SPIRIT_SUMMON) != nullptr )
    {
        ret = true;
    }
    
    return ret;
}

void PopupLoadingSequentialUpgrade::show()
{
    // notification
    auto node = Director::getInstance()->getNotificationNode();
    if ( node == nullptr )
    {
        return;
    }
    
    if ( node->getChildByTag(TAG_NOTI_LOADING_SPIRIT_SUMMON) != nullptr )
    {
        PopupLoadingSequentialUpgrade::_nCountVisible++;
        return;
    }
    
    PopupLoadingSequentialUpgrade::_nCountVisible = 1;
    
    // notification add
    auto popup = PopupLoadingSequentialUpgrade::create();
    node->addChild(popup, ZORDER_NOTI_LOADING, TAG_NOTI_LOADING_SPIRIT_SUMMON);
}

void PopupLoadingSequentialUpgrade::hide(bool bAll)
{
    // visible
    if ( bAll == false )
    {
        PopupLoadingSequentialUpgrade::_nCountVisible--;
        if ( PopupLoadingSequentialUpgrade::_nCountVisible > 0 )
            return;
    }
    
    PopupLoadingSequentialUpgrade::_nCountVisible = 0;
    PopupLoadingSequentialUpgrade::_strTextKey = "";
    PopupLoadingSequentialUpgrade::_strClosingRemark = "";
    
    // notification remove
    auto node = Director::getInstance()->getNotificationNode();
    if ( node != nullptr && node->getChildByTag(TAG_NOTI_LOADING_SPIRIT_SUMMON) != nullptr )
    {
        // event
        node->removeChildByTag(TAG_NOTI_LOADING_SPIRIT_SUMMON);
    }
}

void PopupLoadingSequentialUpgrade::setCurrentIdx(int value)
{
    PopupLoadingSequentialUpgrade::_nCurrentIdx = value;
    
    auto node = Director::getInstance()->getNotificationNode();
    if ( node == nullptr )
    {
        return;
    }
    auto popup = node->getChildByTag(TAG_NOTI_LOADING_SPIRIT_SUMMON);
    if ( node->getChildByTag(TAG_NOTI_LOADING_SPIRIT_SUMMON) != nullptr )
    {
        ((PopupLoadingSequentialUpgrade*) popup)->uiReload();
    }
}

void PopupLoadingSequentialUpgrade::setTextKey(std::string value)
{
    PopupLoadingSequentialUpgrade::_strTextKey = value;
    
    auto node = Director::getInstance()->getNotificationNode();
    if ( node == nullptr )
    {
        return;
    }
    auto popup = node->getChildByTag(TAG_NOTI_LOADING_SPIRIT_SUMMON);
    if ( node->getChildByTag(TAG_NOTI_LOADING_SPIRIT_SUMMON) != nullptr )
    {
        ((PopupLoadingSequentialUpgrade*) popup)->uiReload();
    }
}

void PopupLoadingSequentialUpgrade::setClosingRemark(std::string value)
{
    PopupLoadingSequentialUpgrade::_strClosingRemark = value;
    
    auto node = Director::getInstance()->getNotificationNode();
    if ( node == nullptr )
    {
        return;
    }
    auto popup = node->getChildByTag(TAG_NOTI_LOADING_SPIRIT_SUMMON);
    if ( node->getChildByTag(TAG_NOTI_LOADING_SPIRIT_SUMMON) != nullptr )
    {
        ((PopupLoadingSequentialUpgrade*) popup)->uiReload();
    }
}
