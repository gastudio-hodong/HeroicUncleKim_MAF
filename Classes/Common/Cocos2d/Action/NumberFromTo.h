//
//  NumberFromTo.h
//
//

#ifndef NumberFromTo_h
#define NumberFromTo_h

#include <cocos2d.h>

NS_CC_BEGIN

class NumberFromTo : public cocos2d::ActionInterval
{
public:
    enum class E_TYPE{
        NUMBER,
        TIME_M_S,
    };
    
public:
    static NumberFromTo* create(float duration, int from, int to, E_TYPE type = E_TYPE::NUMBER);
    
    virtual NumberFromTo* clone() const override;
    virtual NumberFromTo* reverse() const override;
    virtual void startWithTarget(cocos2d::Node *target) override;
    /**
     * @param time in seconds
     */
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
    NumberFromTo(void);
    virtual ~NumberFromTo(void);
    
    bool initWithDuration(float duration, int from, int to, E_TYPE type);
    
protected:
    int           _to;
    int           _from;
    
    E_TYPE     _type;
    
};

NS_CC_END

#endif
