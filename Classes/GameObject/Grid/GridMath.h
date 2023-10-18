//
//  GridMath.h
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/05.
//

#ifndef GridMath_h
#define GridMath_h

namespace  GridMath {
    template<typename _Ty>
    _Ty clamp(const _Ty& val, const _Ty& min, const _Ty& max)
    {
        return std::max(min, std::min(max, val));
    }

    template < class _Ty >
    bool InBoundsEx(_Ty const& v, _Ty Min, _Ty Max)
    {
        if (Min > Max)
        {
            _Ty T = Min;
            Min = Max;
            Max = T;
        }
        return v<Min ? false : v>Max ? false : true;
    }

}

#endif /* GridMath_h */
