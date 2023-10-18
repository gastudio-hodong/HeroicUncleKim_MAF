//
//  IntPoint.h
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/05.
//

#ifndef IntPoint_h
#define IntPoint_h

#include "cocos2d.h"
#include "ManagerSystem/TextManager.h"
#include "Libs/maf/MafUtils.h"
struct IntPoint
{
    union
    {
        struct
        {
            /** Holds the point's x-coordinate. */
            int X;

            /** Holds the point's y-coordinate. */
            int Y;
        };
        int XY[2];
    };

    /** An integer point with zeroed values. */
    static const IntPoint ZeroValue;

    /** Default constructor (no initialization). */
    IntPoint() = default;

    /**
     * Create and initialize a new instance with the specified coordinates.
     *
     * @param InX The x-coordinate.
     * @param InY The y-coordinate.
     */
    IntPoint(int InX, int InY)
        : X(InX)
        , Y(InY)
    {
    }

    /**
     * Create and initialize a new instance with a single int.
     * Both X and Y will be initialized to this value
     *
     * @param InXY The x and y-coordinate.
     */
    IntPoint(int InXY)
        : X(InXY)
        , Y(InXY)
    {
    }
    /**
     * Get specific component of a point.
     *
     * @param PointIndex Index of point component.
     * @return const reference to component.
     */
    const int& operator()(int PointIndex) const
    {
        return XY[PointIndex];
    }

    /**
     * Get specific component of a point.
     *
     * @param PointIndex Index of point component
     * @return reference to component.
     */
    int& operator()(int PointIndex)
    {
        return XY[PointIndex];
    }

    /**
     * Compare two points for equality.
     *
     * @param Other The other int point being compared.
     * @return true if the points are equal, false otherwise.
     */
    bool operator==(const IntPoint& Other) const
    {
        return X == Other.X && Y == Other.Y;
    }

    /**
     * Compare two points for inequality.
     *
     * @param Other The other int point being compared.
     * @return true if the points are not equal, false otherwise.
     */
    bool operator!=(const IntPoint& Other) const
    {
        return (X != Other.X) || (Y != Other.Y);
    }
    
    bool operator<(const IntPoint& Other) const
    {
        if (X == Other.X)
        {
            return Y < Other.Y;
        }
        return X < Other.X;
    }

    bool operator>(const IntPoint& Other) const
    {
        if (X == Other.X)
        {
            return Y > Other.Y;
        }
        return X > Other.X;
    }

    /**
     * Scale this point.
     *
     * @param Scale What to multiply the point by.
     * @return Reference to this point after multiplication.
     */
    IntPoint& operator*=(int Scale)
    {
        X *= Scale;
        Y *= Scale;

        return *this;
    }

    /**
     * Divide this point by a scalar.
     *
     * @param Divisor What to divide the point by.
     * @return Reference to this point after division.
     */
    IntPoint& operator/=(int Divisor)
    {
        X /= Divisor;
        Y /= Divisor;

        return *this;
    }

    /**
     * Add another point component-wise to this point.
     *
     * @param Other The point to add to this point.
     * @return Reference to this point after addition.
     */
    IntPoint& operator+=(const IntPoint& Other)
    {
        X += Other.X;
        Y += Other.Y;

        return *this;
    }

    /**
     * Multiply another point component-wise from this point.
     *
     * @param Other The point to multiply with this point.
     * @return Reference to this point after multiplication.
     */
    IntPoint& operator*=(const IntPoint& Other)
    {
        X *= Other.X;
        Y *= Other.Y;

        return *this;
    }

    /**
     * Subtract another point component-wise from this point.
     *
     * @param Other The point to subtract from this point.
     * @return Reference to this point after subtraction.
     */
    IntPoint& operator-=(const IntPoint& Other)
    {
        X -= Other.X;
        Y -= Other.Y;

        return *this;
    }

    /**
     * Divide this point component-wise by another point.
     *
     * @param Other The point to divide with.
     * @return Reference to this point after division.
     */
    IntPoint& operator/=(const IntPoint& Other)
    {
        X /= Other.X;
        Y /= Other.Y;

        return *this;
    }

    /**
     * Assign another point to this one.
     *
     * @param Other The point to assign this point from.
     * @return Reference to this point after assignment.
     */
    IntPoint& operator=(const IntPoint& Other)
    {
        X = Other.X;
        Y = Other.Y;

        return *this;
    }

    /**
     * Get the result of scaling on this point.
     *
     * @param Scale What to multiply the point by.
     * @return A new scaled int point.
     */
    IntPoint operator*(int Scale) const
    {
        return IntPoint(*this) *= Scale;
    }

    /**
     * Get the result of division on this point.
     *
     * @param Divisor What to divide the point by.
     * @return A new divided int point.
     */
    IntPoint operator/(int Divisor) const
    {
        return IntPoint(*this) /= Divisor;
    }

    /**
     * Get the result of addition on this point.
     *
     * @param Other The other point to add to this.
     * @return A new combined int point.
     */
    IntPoint operator+(const IntPoint& Other) const
    {
        return IntPoint(*this) += Other;
    }

    /**
     * Get the result of subtraction from this point.
     *
     * @param Other The other point to subtract from this.
     * @return A new subtracted int point.
     */
    IntPoint operator-(const IntPoint& Other) const
    {
        return IntPoint(*this) -= Other;
    }

    /**
     * Get the result of multiplication on this point.
     *
     * @param Other The point to multiply with this point.
     * @return A new multiplied int point
     */
    IntPoint operator*(const IntPoint& Other) const
    {
        return IntPoint(*this) *= Other;
    }

    /**
     * Get the result of division on this point.
     *
     * @param Other The other point to subtract from this.
     * @return A new subtracted int point.
     */
    IntPoint operator/(const IntPoint& Other) const
    {
        return IntPoint(*this) /= Other;
    }

    /**
    * Get specific component of the point.
    *
    * @param Index the index of point component
    * @return reference to component.
    */
    int& operator[](int Index)
    {
        return ((Index == 0) ? X : Y);
    }

    /**
    * Get specific component of the point.
    *
    * @param Index the index of point component
    * @return copy of component value.
    */
    int operator[](int Index) const
    {
        return ((Index == 0) ? X : Y);
    }

    /**
     * Get the component-wise min of two points.
     *
     * @see ComponentMax, GetMax
     */
    IntPoint ComponentMin(const IntPoint& Other) const
    {
        return IntPoint(MIN(X, Other.X), MAX(Y, Other.Y));
    }

    /**
     * Get the component-wise max of two points.
     *
     * @see ComponentMin, GetMin
     */
    IntPoint ComponentMax(const IntPoint& Other) const
    {
        return IntPoint(MAX(X, Other.X), MAX(Y, Other.Y));
    }

    /**
     * Get the larger of the point's two components.
     *
     * @return The maximum component of the point.
     * @see GetMin, Size, SizeSquared
     */
    int GetMax() const
    {
        return MAX(X, Y);
    }

    /**
     * Get the smaller of the point's two components.
     *
     * @return The minimum component of the point.
     * @see GetMax, Size, SizeSquared
     */
    int GetMin() const
    {
        return MIN(X, Y);
    }

    /**
     * Get the distance of this point from (0,0).
     *
     * @return The distance of this point from (0,0).
     * @see GetMax, GetMin, SizeSquared
     */
    int Size() const
    {
        int64_t LocalX64 = (int64_t)X;
        int64_t LocalY64 = (int64_t)Y;
        return int(std::sqrt(double(LocalX64 * LocalX64 + LocalY64 * LocalY64)));
    }

    /**
     * Get the squared distance of this point from (0,0).
     *
     * @return The squared distance of this point from (0,0).
     * @see GetMax, GetMin, Size
     */
    int SizeSquared() const
    {
        return X * X + Y * Y;
    }

    /**
     * Get a textual representation of this point.
     *
     * @return A string describing the point.
     */
    std::string ToString() const
    {
        return MafUtils::format("X=%d, Y=%d", X, Y);
    }

    
    //나중에 UE에서 가져오기
    /**
     * Divide an int point and round up the result.
     *
     * @param lhs The int point being divided.
     * @param Divisor What to divide the int point by.
     * @return A new divided int point.
     * @see DivideAndRoundDown
     */
//    static IntPoint DivideAndRoundUp(IntPoint lhs, int Divisor)
//    {
//        return IntPoint(FMath::DivideAndRoundUp(lhs.X, Divisor), FMath::DivideAndRoundUp(lhs.Y, Divisor));
//    }
//
//    static IntPoint DivideAndRoundUp(IntPoint lhs, IntPoint Divisor)
//    {
//        return IntPoint(FMath::DivideAndRoundUp(lhs.X, Divisor.X), FMath::DivideAndRoundUp(lhs.Y, Divisor.Y));
//    }

    /**
     * Divide an int point and round down the result.
     *
     * @param lhs The int point being divided.
     * @param Divisor What to divide the int point by.
     * @return A new divided int point.
     * @see DivideAndRoundUp
     */
//    static IntPoint DivideAndRoundDown(IntPoint lhs, int Divisor)
//    {
//        return IntPoint(FMath::DivideAndRoundDown(lhs.X, Divisor), FMath::DivideAndRoundDown(lhs.Y, Divisor));
//    }

    /**
     * Get number of components point has.
     *
     * @return number of components point has.
     */
    static int Num()
    {
        return 2;
    }
};

struct IntPointArray
{
    IntPointArray()
    {
        _pointArray.clear();
    }

    explicit IntPointArray(const std::vector<IntPoint> PointArray)
    {
        this->_pointArray = PointArray;
    }

public:
    std::vector<IntPoint> _pointArray;
};
#endif /* IntPoint_h */
