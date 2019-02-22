#ifndef MYBIGINT_H
#define MYBIGINT_H

#include <iostream>
#include <string>

const int MAXVALUE = 1000000000;
const int MAXDIGITS = 10;
enum class Inequalities
{
	Equal = 1,
	Lesser = 2,
	Greater = 3,
	Inconclusive = 4
};

enum class ErrorTypes
{
	InvalidString = 1,
	InvalidOperation = 2,
	FailedOperation = 3,
	InconclusiveComparison = 4,
	InvalidConversion = 5,
	InvalidUnsignedConversion = 6,
	DivisionByZero = 7,
	InvalidOutput = 8,
	none = 9
};

class MyBigInt
{
	std::string sBigInt;
	bool negative = false;
	bool valid = false;
public:
	ErrorTypes errorFlag = ErrorTypes::none;

	MyBigInt() {}
	MyBigInt(int k)
	{
		if (k < 0)
		{
			negative = true;
			k *= -1;
		}
		sBigInt = std::to_string(k);
		valid = true;
		errorFlag = ErrorTypes::none;
	}
	MyBigInt(long k)
	{
		if (k < 0)
		{
			negative = true;
			k *= -1;
		}
		sBigInt = std::to_string(k);
		valid = true;
		errorFlag = ErrorTypes::none;
	}
	MyBigInt(long long k)
	{
		if (k < 0)
		{
			negative = true;
			k *= -1;
		}
		sBigInt = std::to_string(k);
		valid = true;
		errorFlag = ErrorTypes::none;
	}
	MyBigInt(short k)
	{
		if (k < 0)
		{
			negative = true;
			k *= -1;
		}
		sBigInt = std::to_string(k);
		valid = true;
		errorFlag = ErrorTypes::none;
	}

	MyBigInt(std::string &k)
	{
		if (k[0] == '-' && k.size() > 1)
		{
			negative = true;
			k.erase(0, 1);
		}
		if (checkString(k))
		{
			sBigInt = k;
			valid = true;
			errorFlag = ErrorTypes::none;
		}
		else if(!checkString(k))
		{
			ErrorHandling(ErrorTypes::InvalidString);
		}
	}

	MyBigInt(const MyBigInt &k): sBigInt(k.sBigInt), negative(k.negative), valid(k.valid), errorFlag(k.errorFlag)
	{
	}

	~MyBigInt(){}

	bool isNegative() const { return negative; }
	bool operator== (const MyBigInt &k);
	bool operator!= (const MyBigInt &k);
	bool operator> (const MyBigInt &k);
	bool operator< (const MyBigInt &k);
	bool operator>= (const MyBigInt &k);
	bool operator<= (const MyBigInt &k);
	bool checkString(const std::string k) const;
	friend std::ostream & operator <<(std::ostream &out, MyBigInt &k);

	MyBigInt& operator= (const MyBigInt &k);
	MyBigInt& operator=(int &k);
	MyBigInt& operator= (unsigned int &k);
	MyBigInt& operator=(long &k);
	MyBigInt& operator= (unsigned long &k);
	MyBigInt& operator=(short &k);
	MyBigInt& operator= (unsigned short &k);
	MyBigInt& operator=(long long &k);
	MyBigInt& operator= (unsigned long long &k);
	MyBigInt& operator= (std::string &k);
	MyBigInt& operator+= (const MyBigInt &k);
	MyBigInt& operator-= (const MyBigInt &k);
	MyBigInt& operator*= (const MyBigInt &k);
	MyBigInt& operator/= (const MyBigInt &k);
	MyBigInt& operator%= (const MyBigInt &k);
	MyBigInt operator/ (const MyBigInt &k);
	MyBigInt operator% (const MyBigInt &k);
	MyBigInt operator+ (const MyBigInt &k);
	MyBigInt operator- (const MyBigInt &k);
	MyBigInt operator* (const MyBigInt &k);
	operator short();
	operator unsigned short();
	operator int();
	operator unsigned int();
	operator long();
	operator unsigned long();
	operator long long();
	operator unsigned long long();
	operator std::string();

	int GetLength() const { return sBigInt.size(); }
	Inequalities compareSigned(const MyBigInt &k, const MyBigInt &b) const;
	Inequalities compareUnsigned(const MyBigInt &k, const MyBigInt &b) const;
	Inequalities compareStrings(const std::string &k, const std::string &b) const;

	void ErrorHandling(ErrorTypes et, const std::string &function = "");
private:
	MyBigInt& addBigInts(MyBigInt &k, const MyBigInt &b);
	MyBigInt& subBigInts(MyBigInt &k, const MyBigInt &b);
	MyBigInt& multiplyBigInts(MyBigInt &k, const MyBigInt &b);
	MyBigInt& divideBigInts(MyBigInt &k, const MyBigInt &b, char op);

	//Multiply a string with and integer. Used in divideBigInt function.
	std::string StringTimesInt(std::string &k, const int &b);
	//Subtract strings with numerals like integers
	std::string StrMinusStr(std::string &k, std::string &b);
};

#endif