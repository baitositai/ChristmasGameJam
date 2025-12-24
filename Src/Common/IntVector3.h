#pragma once

struct IntVector3
{
public:

	int x;
	int y;
	int z;

	// コンストラクタ
	IntVector3(void);
	// コンストラクタ
	IntVector3(int vX, int vY, int vZ);
	// デストラクタ
	~IntVector3(void);

	//演算
	const IntVector3 operator+(const IntVector3 value)const;
	const IntVector3 operator+(const int value)const;
	void operator+=(const IntVector3 value);
	const IntVector3 operator-(const IntVector3 value)const;
	void operator-=(const IntVector3 value);
	const IntVector3 operator*(const int value)const;
	void operator*=(const int value);
	const IntVector3 operator/(const int value)const;
	void operator/=(const int value);

private:
};

