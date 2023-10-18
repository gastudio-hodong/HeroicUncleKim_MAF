#ifndef _BIG_INT_H_
#define _BIG_INT_H_

#include <vector>
#include <string>
#include <iostream>

class TIntegerBig
{
public:
	TIntegerBig();
	~TIntegerBig();
	TIntegerBig(int n);
	TIntegerBig(unsigned int n);
	TIntegerBig(long long int n);
	TIntegerBig(unsigned long long int n);
	explicit TIntegerBig(const std::string& n);
	TIntegerBig(const TIntegerBig& other);

	//사칙 연산
	const TIntegerBig add(const TIntegerBig& other) const;
	const TIntegerBig multiply(const TIntegerBig& other) const;
	const TIntegerBig divide(const TIntegerBig& other) const;
	const TIntegerBig mod(const TIntegerBig& other) const;

	//비교 연산 - 음수: 왼쪽이 작음 0: 동일 양수 : 오른쪽이 작음
	int compare(const TIntegerBig& other) const;

	//부호 변경
	const TIntegerBig signSwitch() const;

	//비트 연산
	const TIntegerBig bit_leftShift(unsigned int n) const;
	const TIntegerBig bit_rightShift(unsigned int n) const;
	const TIntegerBig bit_or(const TIntegerBig& other) const;
	const TIntegerBig bit_and(const TIntegerBig& other) const;
	const TIntegerBig bit_xor(const TIntegerBig& other) const;
	const TIntegerBig bit_not() const;

	//부호 연산
	const TIntegerBig operator -() const;
	const TIntegerBig operator +() const;

	//비트 연산
	const TIntegerBig operator <<(unsigned int n) const;
	const TIntegerBig operator >>(unsigned int n) const;
	const TIntegerBig operator ~() const;

	//대입 연산
	TIntegerBig& operator =(const TIntegerBig& other);
	TIntegerBig& operator +=(const TIntegerBig& other);
	TIntegerBig& operator -=(const TIntegerBig& other);
	TIntegerBig& operator *=(const TIntegerBig& other);
	TIntegerBig& operator /=(const TIntegerBig& other);
	TIntegerBig& operator %=(const TIntegerBig& other);
	TIntegerBig& operator <<=(unsigned int n);
	TIntegerBig& operator >>=(unsigned int n);
	TIntegerBig& operator &=(const TIntegerBig& other);
	TIntegerBig& operator |=(const TIntegerBig& other);
	TIntegerBig& operator ^=(const TIntegerBig& other);
	
	const std::string asString() const;
	int asInt() const;
	unsigned int asUnsigned() const;
	long long int asInt64() const;
	unsigned long long int asUnsigned64() const;
	bool isPositive() const;

private:
	inline unsigned int highWord(unsigned int n) const;
	inline unsigned int lowWord(unsigned int n) const;
	void divideWithRemainder(const TIntegerBig& divisor, const TIntegerBig& dividend,
								TIntegerBig* quot, TIntegerBig* rem, int bit) const;

	void stringToValue(const std::string& n);
	const std::string valueToString() const;
	void normalize();

	std::vector<unsigned int>		m_Value;
	bool							m_Positive;
};


//관련 전역 함수들
TIntegerBig abs(const TIntegerBig& ainteger);

//연산자 오버로딩

//사칙 연산
const TIntegerBig operator +(const TIntegerBig& lhs, const TIntegerBig& rhs);
const TIntegerBig operator -(const TIntegerBig& lhs, const TIntegerBig& rhs);
const TIntegerBig operator *(const TIntegerBig& lhs, const TIntegerBig& rhs);
const TIntegerBig operator /(const TIntegerBig& lhs, const TIntegerBig& rhs);
const TIntegerBig operator %(const TIntegerBig& lhs, const TIntegerBig& rhs);

//비교 연산

bool operator ==(const TIntegerBig& lhs, const TIntegerBig& rhs);
bool operator !=(const TIntegerBig& lhs, const TIntegerBig& rhs);
bool operator >(const TIntegerBig& lhs, const TIntegerBig& rhs);
bool operator >=(const TIntegerBig& lhs, const TIntegerBig& rhs);
bool operator <(const TIntegerBig& lhs, const TIntegerBig& rhs);
bool operator <=(const TIntegerBig& lhs, const TIntegerBig& rhs);


//비트 연산
const TIntegerBig operator &(const TIntegerBig& lhs, const TIntegerBig& rhs);
const TIntegerBig operator |(const TIntegerBig& lhs, const TIntegerBig& rhs);
const TIntegerBig operator ^(const TIntegerBig& lhs, const TIntegerBig& rhs);

//출력

std::ostream& operator <<(std::ostream& os, const TIntegerBig& ainteger);
std::istream& operator >>(std::istream& is, TIntegerBig& ainteger);

#endif
