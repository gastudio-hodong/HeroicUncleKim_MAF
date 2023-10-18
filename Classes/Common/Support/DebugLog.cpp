//
//  DebugLog.cpp
//
//

#include "DebugLog.h"

#include "Common/Cocos2d/CText.h"

USING_NS_CC;

cocos2d::Vector<ui::Layout*> DebugLog::_listLog;

DebugLog::DebugLog(void) :
_bInitUI(false),

//
_bScreen(false),

_bButtonExpandMove(false),
_paddingButtonExpand(100),

// widget
_uiContainerScreen(nullptr),
_uiContainerFunction(nullptr),
_uiContainerLog(nullptr),

_uiInfoLog(nullptr),

_uiBtnExpand(nullptr)
{
    
}

DebugLog::~DebugLog(void)
{
    
}

bool DebugLog::init()
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    Director * director = Director::getInstance();
    setContentSize(director->getWinSize());
    
    // event init
    onInit();
    
    // enter callback
    setOnEnterTransitionDidFinishCallback([=](){
        //
        if ( _bInitUI == false )
        {
            // event start
            onStart();
            
            // set init
            _bInitUI = true;
        }
        
        // event refresh
        onRefresh();
    });
    
    return true;
}

#pragma mark - override
void DebugLog::onInit()
{
    
}

void DebugLog::onStart()
{
    _uiBtnExpand = ui::Layout::create();
    _uiBtnExpand->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _uiBtnExpand->setBackGroundColor(Color3B::GRAY);
    _uiBtnExpand->setContentSize(Size(140, 50));
    _uiBtnExpand->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _uiBtnExpand->setPosition(Vec2(_uiBtnExpand->getContentSize().width / 2, getContentSize().height - _paddingButtonExpand - _uiBtnExpand->getContentSize().height / 2));
    _uiBtnExpand->setTouchEnabled(true);
    _uiBtnExpand->addTouchEventListener(CC_CALLBACK_2(DebugLog::onTouchExpand, this));
    _uiBtnExpand->addClickEventListener(CC_CALLBACK_1(DebugLog::onClickExpand, this));
    addChild(_uiBtnExpand);
    {
        auto text = ui::CText::create("DEBUG", "Helvetica", 24);
        text->setPosition(Vec2(_uiBtnExpand->getContentSize().width / 2, _uiBtnExpand->getContentSize().height / 2));
        _uiBtnExpand->addChild(text);
    }
    
    //
    uiInitScreen();
    uiInitFunction();
    uiInitLog();
}

void DebugLog::onRefresh()
{

}

#pragma mark - ui : init
void DebugLog::uiInitScreen()
{
    _uiContainerScreen = ui::Layout::create();
    _uiContainerScreen->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _uiContainerScreen->setBackGroundColor(Color3B::BLACK);
    _uiContainerScreen->setVisible(false);
    _uiContainerScreen->setOpacity(150);
    addChild(_uiContainerScreen);
}

void DebugLog::uiInitFunction()
{
    _uiContainerFunction = ui::Layout::create();
    _uiContainerFunction->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _uiContainerFunction->setBackGroundColor(Color3B::BLACK);
    _uiContainerFunction->setOpacity(150);
    _uiContainerScreen->addChild(_uiContainerFunction);
    {
        std::string listText[] = {"Clear"};
        for ( int i = 0; i < sizeof(listText) / sizeof(std::string); i++ )
        {
            std::string text = listText[i];
            
            auto uiText = ui::CText::create(text, "Helvetica", 24);
            uiText->setPositionType(PositionType::PERCENT);
            uiText->setPositionPercent(Vec2::ANCHOR_MIDDLE);
            
            auto uiBtn = ui::Layout::create();
            uiBtn->setContentSize(Size(uiText->getContentSize().width + 20, 50));
            uiBtn->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
            uiBtn->setBackGroundColor(Color3B::GRAY);
            uiBtn->setTouchEnabled(true);
            uiBtn->addClickEventListener(CC_CALLBACK_1(DebugLog::onClickFunc, this));
            uiBtn->setName(text);
            uiBtn->addChild(uiText);
            _uiContainerFunction->addChild(uiBtn);
        }
    }
}

void DebugLog::uiInitLog()
{
    _uiContainerLog = ui::Layout::create();
    _uiContainerLog->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _uiContainerLog->setBackGroundColor(Color3B::GRAY);
    _uiContainerLog->setOpacity(150);
    _uiContainerScreen->addChild(_uiContainerLog);
    {
        _uiInfoLog = ui::ScrollView::create();
        _uiInfoLog->setBounceEnabled(true);
        _uiInfoLog->setDirection(ui::ScrollView::Direction::VERTICAL);
        _uiInfoLog->setScrollBarPositionFromCorner(Vec2(4, 4));
        _uiInfoLog->setScrollBarColor(Color3B::YELLOW);
        _uiContainerLog->addChild(_uiInfoLog);
        
        _uiInfoLog->setLayoutType(Type::VERTICAL);
        
        auto action1 = DelayTime::create(0.1f);
        auto action2 = CallFuncN::create([=](Node* sender){
            
            if ( _bScreen == false )
            {
                return;
            }
            
            if ( _listLog.size() <= 0 )
            {
                return;
            }
            
            auto uiInfoLog = (ui::ScrollView*)sender;
            
            auto uiLayout = (ui::Layout*)_listLog.at(0);
            auto uiTextTime = uiLayout->getChildByName<ui::CText*>("UI_TIME");
            auto uiTextLog = uiLayout->getChildByName<ui::CText*>("UI_LOG");
            uiTextLog->setContentSize(Size(uiInfoLog->getContentSize().width - uiTextTime->getContentSize().width, 0));
            uiTextLog->setTextAreaSize(Size(uiInfoLog->getContentSize().width - uiTextTime->getContentSize().width, 0));
            uiLayout->setContentSize(Size(uiInfoLog->getContentSize().width, std::max(uiTextTime->getContentSize().height, uiTextLog->getContentSize().height)));
            uiInfoLog->addChild(uiLayout);
            
            _listLog.erase(0);
            
            float height = 0;
            for (auto& child : uiInfoLog->getChildren())
            {
                height += child->getContentSize().height;
            }
            
            auto pos = _uiInfoLog->getInnerContainerPosition();
            _uiInfoLog->setInnerContainerSize(Size(_uiInfoLog->getContentSize().width, height));
            _uiInfoLog->setInnerContainerPosition(pos);

        });
        auto seq = Sequence::create(action1, action2, nullptr);
        _uiInfoLog->runAction(RepeatForever::create(seq));
    }
}

#pragma mark - ui : draw
void DebugLog::uiDrawScreen()
{
    if ( _bScreen == false )
    {
        _uiContainerScreen->setVisible(false);
    }
    else
    {
        Size sizeView = getContentSize();
        Size size = Size(sizeView.width, _uiBtnExpand->getPositionY() - _uiBtnExpand->getContentSize().height / 2);
        
        _uiContainerScreen->setVisible(true);
        _uiContainerScreen->setContentSize(size);
    }
}

void DebugLog::uiDrawFunction()
{
    Size sizeView = getContentSize();
    Size size = _uiContainerScreen->getContentSize();
    
    if ( sizeView.width > sizeView.height )
    {// landscape
        _uiContainerFunction->setContentSize(Size(size.width * 0.25, size.height));
        _uiContainerFunction->setAnchorPoint(Vec2::ZERO);
        _uiContainerFunction->setPosition(Vec2::ZERO);
        
        for (auto& child : _uiContainerFunction->getChildren() )
        {
            double width = _uiContainerFunction->getContentSize().width;
            double height = child->getContentSize().height;
            child->setContentSize(Size(width, height));
        }
    }
    else
    {// portrait
        _uiContainerFunction->setContentSize(Size(size.width, 50));
        _uiContainerFunction->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        _uiContainerFunction->setPosition(Vec2(0, size.height));
    }
}

void DebugLog::uiDrawLog()
{
    //
    Size sizeView = getContentSize();
    Size size = _uiContainerScreen->getContentSize();
    
    if ( sizeView.width > sizeView.height )
    {// landscape
        _uiContainerLog->setContentSize(Size(size.width * 0.75, size.height));
        _uiContainerLog->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        _uiContainerLog->setPosition(Vec2(size.width, 0));
    }
    else
    {// portrait
        _uiContainerLog->setContentSize(Size(size.width, size.height - 50));
        _uiContainerLog->setAnchorPoint(Vec2::ZERO);
        _uiContainerLog->setPosition(Vec2::ZERO);
    }
    
    //
    _uiInfoLog->setContentSize(_uiContainerLog->getContentSize());
    _uiContainerLog->setVisible(true);
}

#pragma mark - ui : click
void DebugLog::onTouchExpand(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
    auto uiExpand = dynamic_cast<ui::Layout*>(sender);
    switch (type)
    {
        case ui::Widget::TouchEventType::BEGAN:
            _bButtonExpandMove = false;
            break;

        case ui::Widget::TouchEventType::MOVED:
        {
            auto nDiff = uiExpand->getTouchBeganPosition().distance(uiExpand->getTouchMovePosition());
            if ( nDiff > 10 )
            {
                _bButtonExpandMove = true;
            }
            
            if ( _bButtonExpandMove == true )
            {
                double posY = uiExpand->getTouchMovePosition().y;
                posY = posY > getContentSize().height - _paddingButtonExpand ? getContentSize().height - _paddingButtonExpand : posY;
                posY = posY < _paddingButtonExpand ? _paddingButtonExpand : posY;
                uiExpand->setPositionY(posY);
                
                if ( _bScreen == true )
                {
                    _uiContainerFunction->setVisible(false);
                    _uiContainerLog->setVisible(false);
                    
                    uiDrawScreen();
                }
            }
            break;
        }
        case ui::Widget::TouchEventType::ENDED:
        case ui::Widget::TouchEventType::CANCELED:
        {
            if ( _bScreen == true  )
            {
                _uiContainerFunction->setVisible(true);
                
                uiDrawScreen();
                uiDrawFunction();
                uiDrawLog();
            }
            break;
        }
        default:
            break;
    }
}

void DebugLog::onClickExpand(cocos2d::Ref* sender)
{
    if ( _bButtonExpandMove == true )
    {
        return;
    }
    
    //
    _bScreen = !_bScreen;
    
    //
    uiDrawScreen();
    uiDrawFunction();
    uiDrawLog();
}

void DebugLog::onClickFunc(cocos2d::Ref* sender)
{
    auto layout = dynamic_cast<ui::Layout*>(sender);
    std::string text = layout->getName();
    
    // {"Clear"};
    if ( text.compare("Clear") == 0 )
    {
        _uiInfoLog->removeAllChildren();
        
        uiDrawLog();
    }
}

#pragma mark - game : log
void DebugLog::log(const char * format, ...)
{
    va_list args;
    std::string ret(512, '\0');

    va_start(args, format);
    int nret = vsnprintf(&ret.front(), ret.length() + 1, format, args);
    va_end(args);

    if (nret >= 0)
    {
        if ((unsigned int)nret < ret.length())
        {
            ret.resize(nret);
        }
        else if ((unsigned int)nret > ret.length()) { // VS2015/2017 or later Visual Studio Version
            ret.resize(nret);

            va_start(args, format);
            nret = vsnprintf(&ret.front(), ret.length() + 1, format, args);
            va_end(args);

            assert(nret == ret.length());
        }
        // else equals, do nothing.
    }
    else
    { // less or equal VS2013 and Unix System glibc implement.
        do
        {
            ret.resize(ret.length() * 3 / 2);

            va_start(args, format);
            nret = vsnprintf(&ret.front(), ret.length() + 1, format, args);
            va_end(args);

        } while (nret < 0);

        ret.resize(nret);
    }
    
    // ui log
    log(ret);
}

void DebugLog::log(std::string strLog)
{
    //
    cocos2d::log("%s", strLog.c_str());
    
#if !defined(COCOS2D_DEBUG) || (COCOS2D_DEBUG == 0)
    return;
#endif
    
    // log
    std::string log = strLog;
    std::string logTime =  "";
    {
        int32_t length = 1024 * 100;
        
        time_t t = time(nullptr);
        struct tm *info = localtime(&t);
        char* buf = (char*)malloc(length);
        
        strftime (buf, length, "%H:%M:%S", info);
        logTime = buf;
        free(buf);
    }
    logTime.append("   ");

    // log text
    auto uiLayout = ui::Layout::create();
    uiLayout->setLayoutType(Type::HORIZONTAL);
    _listLog.pushBack(uiLayout);
    if ( _listLog.size() > 10 )
    {
        _listLog.erase(0);
    }
    
    auto uiTextTime = ui::CText::create(logTime, "Helvetica", 24);
    uiTextTime->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiTextTime->setTextVerticalAlignment(TextVAlignment::TOP);
    uiTextTime->setName("UI_TIME");
    uiLayout->addChild(uiTextTime);
    
    auto uiTextLog = ui::CText::create(log, "Helvetica", 24);
    uiTextLog->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiTextLog->setTextVerticalAlignment(TextVAlignment::TOP);
    uiTextLog->setName("UI_LOG");
    uiTextLog->setTextOverflow(Label::Overflow::CLAMP);
    uiLayout->addChild(uiTextLog);
}

#pragma mark - game
std::string DebugLog::getHierarchy()
{
    //
    auto root = Director::getInstance()->getRunningScene();
    
    //
    std::string ret;
    for (const auto node : root->getChildren())
    {
        std::string name = node->getName();
//        if (name == "")
//            name = node->getDescription();
        
        if (name == "")
            name = "(no name)";
        
        ret += name;
        
        if (node->getChildrenCount() > 0)
            ret += "\n" + getTreeHierarchy(node, 1);
        else
            ret += "\n";
    }
    
    return ret;
}
std::string DebugLog::getTreeHierarchy(Node* tree, int depth)
{
    std::string ret;
    std::string tabs;
    for (int i = 0; i < depth; i++)
        tabs += "   ";
    
    for (const auto node : tree->getChildren())
    {
        std::string name = node->getName();
//        if (name == "")
//            name = node->getDescription();
        
        if (name == "")
            name = "(no name)";
        
        ret += tabs + name;
        
        if (node->getChildrenCount() > 0)
            ret += "\n" + getTreeHierarchy(node, depth + 1);
        else
            ret += "\n";
    }
    
    return ret;
}

bool DebugLog::isScreen()
{
    return _bScreen;
}
