//
//  MafMath.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/06.
//

#ifndef MafMath_hpp
#define MafMath_hpp

#include <stdio.h>
#include <cmath>
#include "cocos2d.h"

#pragma mark -Global Num

#define UE_PI                     (3.1415926535897932f)    /* Extra digits if needed: 3.1415926535897932384626433832795f */
#define UE_SMALL_NUMBER            (1.e-8f)
#define UE_KINDA_SMALL_NUMBER    (1.e-4f)
#define UE_BIG_NUMBER            (3.4e+38f)
#define UE_EULERS_NUMBER        (2.71828182845904523536f)
#define UE_GOLDEN_RATIO            (1.6180339887498948482045868343656381f)    /* Also known as divine proportion, golden mean, or golden section - related to the Fibonacci Sequence = (1 + sqrt(5)) / 2 */
#define UE_FLOAT_NON_FRACTIONAL (8388608.f) /* All single-precision floating point numbers greater than or equal to this have no fractional value. */


#define UE_DOUBLE_PI                    (3.141592653589793238462643383279502884197169399)
#define UE_DOUBLE_SMALL_NUMBER            (1.e-8)
#define UE_DOUBLE_KINDA_SMALL_NUMBER    (1.e-4)
#define UE_DOUBLE_BIG_NUMBER            (3.4e+38)
#define UE_DOUBLE_EULERS_NUMBER            (2.7182818284590452353602874713526624977572)
#define UE_DOUBLE_GOLDEN_RATIO            (1.6180339887498948482045868343656381)    /* Also known as divine proportion, golden mean, or golden section - related to the Fibonacci Sequence = (1 + sqrt(5)) / 2 */
#define UE_DOUBLE_NON_FRACTIONAL        (4503599627370496.0) /* All double-precision floating point numbers greater than or equal to this have no fractional value. 2^52 */

// Copied from float.h
#define UE_MAX_FLT 3.402823466e+38F

// Aux constants.
#define UE_INV_PI            (0.31830988618f)
#define UE_HALF_PI            (1.57079632679f)
#define UE_TWO_PI            (6.28318530717f)
#define UE_PI_SQUARED        (9.86960440108f)

#define UE_DOUBLE_INV_PI        (0.31830988618379067154)
#define UE_DOUBLE_HALF_PI        (1.57079632679489661923)
#define UE_DOUBLE_TWO_PI        (6.28318530717958647692)
#define UE_DOUBLE_PI_SQUARED    (9.86960440108935861883)

// Common square roots
#define UE_SQRT_2        (1.4142135623730950488016887242097f)
#define UE_SQRT_3        (1.7320508075688772935274463415059f)
#define UE_INV_SQRT_2    (0.70710678118654752440084436210485f)
#define UE_INV_SQRT_3    (0.57735026918962576450914878050196f)
#define UE_HALF_SQRT_2    (0.70710678118654752440084436210485f)
#define UE_HALF_SQRT_3    (0.86602540378443864676372317075294f)

#define UE_DOUBLE_SQRT_2        (1.4142135623730950488016887242097)
#define UE_DOUBLE_SQRT_3        (1.7320508075688772935274463415059)
#define UE_DOUBLE_INV_SQRT_2    (0.70710678118654752440084436210485)
#define UE_DOUBLE_INV_SQRT_3    (0.57735026918962576450914878050196)
#define UE_DOUBLE_HALF_SQRT_2    (0.70710678118654752440084436210485)
#define UE_DOUBLE_HALF_SQRT_3    (0.86602540378443864676372317075294)


// Magic numbers for numerical precision.
#define UE_DELTA        (0.00001f)
#define UE_DOUBLE_DELTA    (0.00001 )

/**
 * Lengths of normalized vectors (These are half their maximum values
 * to assure that dot products with normalized vectors don't overflow).
 */
#define UE_FLOAT_NORMAL_THRESH            (0.0001f)
#define UE_DOUBLE_NORMAL_THRESH            (0.0001)

//
// Magic numbers for numerical precision.
//
#define UE_THRESH_POINT_ON_PLANE                (0.10f)        /* Thickness of plane for front/back/inside test */
#define UE_THRESH_POINT_ON_SIDE                    (0.20f)        /* Thickness of polygon side's side-plane for point-inside/outside/on side test */
#define UE_THRESH_POINTS_ARE_SAME                (0.00002f)    /* Two points are same if within this distance */
#define UE_THRESH_POINTS_ARE_NEAR                (0.015f)    /* Two points are near if within this distance and can be combined if imprecise math is ok */
#define UE_THRESH_NORMALS_ARE_SAME                (0.00002f)    /* Two normal points are same if within this distance */
#define UE_THRESH_UVS_ARE_SAME                    (0.0009765625f)/* Two UV are same if within this threshold (1.0f/1024f) */
                                                            /* Making this too large results in incorrect CSG classification and disaster */
#define UE_THRESH_VECTORS_ARE_NEAR                (0.0004f)    /* Two vectors are near if within this distance and can be combined if imprecise math is ok */
                                                            /* Making this too large results in lighting problems due to inaccurate texture coordinates */
#define UE_THRESH_SPLIT_POLY_WITH_PLANE            (0.25f)        /* A plane splits a polygon in half */
#define UE_THRESH_SPLIT_POLY_PRECISELY            (0.01f)        /* A plane exactly splits a polygon */
#define UE_THRESH_ZERO_NORM_SQUARED                (0.0001f)    /* Size of a unit normal that is considered "zero", squared */
#define UE_THRESH_NORMALS_ARE_PARALLEL            (0.999845f)    /* Two unit vectors are parallel if abs(A dot B) is greater than or equal to this. This is roughly cosine(1.0 degrees). */
#define UE_THRESH_NORMALS_ARE_ORTHOGONAL        (0.017455f)    /* Two unit vectors are orthogonal (perpendicular) if abs(A dot B) is less than or equal this. This is roughly cosine(89.0 degrees). */

#define UE_THRESH_VECTOR_NORMALIZED                (0.01f)        /** Allowed error for a normalized vector (against squared magnitude) */
#define UE_THRESH_QUAT_NORMALIZED                (0.01f)        /** Allowed error for a normalized quaternion (against squared magnitude) */

// Double precision values
#define UE_DOUBLE_THRESH_POINT_ON_PLANE            (0.10)        /* Thickness of plane for front/back/inside test */
#define UE_DOUBLE_THRESH_POINT_ON_SIDE            (0.20)        /* Thickness of polygon side's side-plane for point-inside/outside/on side test */
#define UE_DOUBLE_THRESH_POINTS_ARE_SAME        (0.00002)    /* Two points are same if within this distance */
#define UE_DOUBLE_THRESH_POINTS_ARE_NEAR        (0.015)        /* Two points are near if within this distance and can be combined if imprecise math is ok */
#define UE_DOUBLE_THRESH_NORMALS_ARE_SAME        (0.00002)    /* Two normal points are same if within this distance */
#define UE_DOUBLE_THRESH_UVS_ARE_SAME            (0.0009765625)/* Two UV are same if within this threshold (1.0/1024.0) */
                                                            /* Making this too large results in incorrect CSG classification and disaster */
#define UE_DOUBLE_THRESH_VECTORS_ARE_NEAR        (0.0004)    /* Two vectors are near if within this distance and can be combined if imprecise math is ok */
                                                            /* Making this too large results in lighting problems due to inaccurate texture coordinates */
#define UE_DOUBLE_THRESH_SPLIT_POLY_WITH_PLANE    (0.25)        /* A plane splits a polygon in half */
#define UE_DOUBLE_THRESH_SPLIT_POLY_PRECISELY    (0.01)        /* A plane exactly splits a polygon */
#define UE_DOUBLE_THRESH_ZERO_NORM_SQUARED        (0.0001)    /* Size of a unit normal that is considered "zero", squared */
#define UE_DOUBLE_THRESH_NORMALS_ARE_PARALLEL    (0.999845)    /* Two unit vectors are parallel if abs(A dot B) is greater than or equal to this. This is roughly cosine(1.0 degrees). */
#define UE_DOUBLE_THRESH_NORMALS_ARE_ORTHOGONAL    (0.017455)    /* Two unit vectors are orthogonal (perpendicular) if abs(A dot B) is less than or equal this. This is roughly cosine(89.0 degrees). */

#define UE_DOUBLE_THRESH_VECTOR_NORMALIZED        (0.01)        /** Allowed error for a normalized vector (against squared magnitude) */
#define UE_DOUBLE_THRESH_QUAT_NORMALIZED        (0.01)        /** Allowed error for a normalized quaternion (against squared magnitude) */


class MafMath
{
public:
    template< class T >
    static constexpr inline T GridSnap(T Location, T Grid)
    {
        return (Grid == T{}) ? Location : (floor((Location + (Grid/(T)2)) / Grid) * Grid);
    }
    
    static float Fmod(float X, float Y)
    {
        const float AbsY = std::abs(Y);
        if (AbsY <= UE_SMALL_NUMBER) // Note: this constant should match that used by VectorMod() implementations
        {
            return 0.0;
        }

        return fmodf(X, Y);
    }
    
    static constexpr inline float FloatSelect( float Comparand, float ValueGEZero, float ValueLTZero )
    {
        return Comparand >= 0.f ? ValueGEZero : ValueLTZero;
    }
    
    static inline cocos2d::Vec2 SnapVectorToVector(const cocos2d::Vec2 v1, const cocos2d::Vec2 v2)
    {
        const float X = MafMath::GridSnap(v1.x, v2.x);
        const float Y = MafMath::GridSnap(v1.y, v2.y);
        return cocos2d::Vec2(X, Y);
    }
    
    static inline bool IsFloatEven(const double Value)
    {
        return MafMath::Fmod(Value, 2.f) == 0;
    }

    static inline bool IsIntEven(const int Value)
    {
        return (Value % 2) == 0;
    }
    
    static inline float InvSqrt(float InValue)
      {
        union {
          float f;
          uint32_t i;
        } conv;

        float x2;
        const float threehalfs = 1.5F;

        x2 = InValue * 0.5F;
        conv.f = InValue;
        conv.i = 0x5f3759df - ( conv.i >> 1 );
        conv.f = conv.f * ( threehalfs - ( x2 * conv.f * conv.f ) );
        return conv.f;
      }
    
    static inline cocos2d::Vec2 getSsfeNormal(cocos2d::Vec2 origin)
    {
        cocos2d::Vec2 result;
        
        const float SquareSum = origin.x*origin.x + origin.y*origin.y;
        if(SquareSum > 1.e-8f)
        {
            const float Scale = InvSqrt(SquareSum);
            return cocos2d::Vec2(origin.x*Scale, origin.y*Scale);
        }
        
        return cocos2d::Vec2(0.f, 0.f);
    }

};

#endif /* MafMath_hpp */
