#include "pch.h"
#include "MyBigInt.h"
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cctype>

void MyBigInt::ErrorHandling(ErrorTypes et, const std::string &function)
{
	switch (et)
	{
	case ErrorTypes::InvalidString:
	{
		std::cerr << "Initialization Error! Could not construct MyBigInt with that value. Please use an integer value for MyBigInt class.\n";
		errorFlag = ErrorTypes::InvalidString;
	}
	break;
	case ErrorTypes::InvalidOperation:
	{
		std::cerr << "Operation Error! " << function << " operation on invalid MyBigInt is not allowed. Please use a different value.\n";
		errorFlag = ErrorTypes::InvalidOperation;
	}
	break;
	case ErrorTypes::InconclusiveComparison:
	{
		std::cerr << "Comparison Error! Comparison was inconclusive during " << function << " operation. Operation terminated.\n";
		errorFlag = ErrorTypes::InconclusiveComparison;
	}
	break;
	case ErrorTypes::FailedOperation:
	{
		std::cerr << "Operation Error! Result from " << function << " operation was not a valid value. Operation Failed.\n";
		errorFlag = ErrorTypes::FailedOperation;
	}
	break;
	case ErrorTypes::InvalidConversion:
	{
		std::cerr << "Conversion Error! Cannot convert MyBigInt to " << function << " type. Value of MyBigInt is either too large or not a valid value.\n";
		errorFlag = ErrorTypes::InvalidConversion;
	}
	break;
	case ErrorTypes::InvalidUnsignedConversion:
	{
		std::cerr << "Conversion Error! Cannot convert a negative MyBigInt to an unsigned " << function << " type.\n";
		errorFlag = ErrorTypes::InvalidUnsignedConversion;
	}
	break;
	case ErrorTypes::DivisionByZero:
	{
		std::cerr << "Division by zero is not allowed due to undefined behavior.\n";
		errorFlag = ErrorTypes::DivisionByZero;
	}
	break;
	case ErrorTypes::InvalidOutput:
	{
		std::cerr << "Output Error! Invalid output for << operator.\n";
	}
	break;
	}
}

std::ostream & operator << (std::ostream &out, MyBigInt &k)
{
	if (k.valid)
	{
		if (k.isNegative())
			out << "-";
		out << k.sBigInt << std::endl;
	}
	else if (!k.valid && k.errorFlag == ErrorTypes::none)
	{
		k.ErrorHandling(ErrorTypes::InvalidOutput);
	}
	return out;
}


Inequalities MyBigInt::compareUnsigned(const MyBigInt &k, const MyBigInt &b) const
{
	if (k.GetLength() > b.GetLength())
		return Inequalities::Greater;
	else if (k.GetLength() < b.GetLength())
		return Inequalities::Lesser;
	else
	{
		if (k.sBigInt > b.sBigInt)
			return Inequalities::Greater;
		else if (k.sBigInt < b.sBigInt)
			return Inequalities::Lesser;
		else if (k.sBigInt == b.sBigInt)
			return Inequalities::Equal;
	}
	return Inequalities::Inconclusive;
}

Inequalities MyBigInt::compareStrings(const std::string &k, const std::string &b) const
{
	if (k.size() > b.size())
		return Inequalities::Greater;
	else if (k.size() < b.size())
		return Inequalities::Lesser;
	else
	{
		if (k > b)
			return Inequalities::Greater;
		else if (k < b)
			return Inequalities::Lesser;
		else if (k == b)
			return Inequalities::Equal;
	}
	return Inequalities::Inconclusive;
}

Inequalities MyBigInt::compareSigned(const MyBigInt &k, const MyBigInt &b) const
{
	if (!k.isNegative() && b.isNegative())
		return Inequalities::Greater;
	else if (k.isNegative() && !b.isNegative())
		return Inequalities::Lesser;
	else if (k.isNegative() == b.isNegative())
	{
		if (k.GetLength() > b.GetLength())
			return Inequalities::Greater;
		else if (k.GetLength() < b.GetLength())
			return Inequalities::Lesser;
		else
		{
			if (k.sBigInt > b.sBigInt)
				return Inequalities::Greater;
			else if (k.sBigInt < b.sBigInt)
				return Inequalities::Lesser;
			else if (k.sBigInt == b.sBigInt)
				return Inequalities::Equal;
		}
	}
	
	return Inequalities::Inconclusive;
}

MyBigInt& MyBigInt::addBigInts(MyBigInt &k, const MyBigInt &b)
{
	bool overloaded = false;
	std::string lhstr = k.sBigInt;
	std::string rhstr = b.sBigInt;
	std::string ret;
	int n;

	//Determines the largest number of digits between the two strings, then resizes the smaller
	//string and return value string to that number. The double reversing is needed so trailing
	//zeroes are not added, altering the value of the string.
	if (k.GetLength() > b.GetLength())
	{
		n = k.GetLength();
		std::reverse(rhstr.begin(), rhstr.end());
		rhstr.resize(n, '0');
		std::reverse(rhstr.begin(), rhstr.end());
	}
	else
	{
		n = b.GetLength();
		std::reverse(lhstr.begin(), lhstr.end());
		lhstr.resize(n, '0');
		std::reverse(lhstr.begin(), lhstr.end());
	}
	ret.resize(n, '0');
	for (int i = n - 1; i >= 0; i--)
	{
		//checks if previous digit addition was greater than 9
		if (overloaded)
		{
			//converts string digits starting from the end to ints for the actual addition
			int ltemp = lhstr[i] - '0';
			int rtemp = rhstr[i] - '0';

			//due to being overloaded(greater than 9), 1 is added;
			int temp = ltemp + rtemp + 1;
			
			//checks if overloaded
			if (temp > 9)
			{
				overloaded = true;
				temp %= 10;
			}
			else
				overloaded = false;
			ret[i] = '0' + temp;
		}
		else
		{
			int ltemp = lhstr[i] - '0';
			int rtemp = rhstr[i] - '0';
			int temp = ltemp + rtemp;
			
			if (temp > 9)
			{
				overloaded = true;
				temp %= 10;
			}
			else
				overloaded = false;
			ret[i] = '0' + temp;
		}
	}
	
	if (overloaded)
		ret = "1" + ret;
	k.sBigInt = ret;
	return k;
}

MyBigInt& MyBigInt::subBigInts(MyBigInt &k, const MyBigInt &b)
{
	std::string ret;
	bool underloaded = false;
	std::string lhstr;
	std::string rhstr;
	int n;
	Inequalities eq = compareUnsigned(k, b);
	//Used to make sure the larger number is the left side operand
	//being subtracted from. Use the -= operator to determine the 
	//correct sign of the output.
	if (eq == Inequalities::Greater)
	{
		n = k.GetLength();
		lhstr = k.sBigInt;
		rhstr = b.sBigInt;
		//Double reverse is used so there are no trailing zeroes added to the string,
		//which could greatly alter the value store in the string.
		std::reverse(rhstr.begin(), rhstr.end());
		rhstr.resize(n, '0');
		std::reverse(rhstr.begin(), rhstr.end());
	}
	else if (eq == Inequalities::Lesser)
	{
		n = b.GetLength();
		lhstr = b.sBigInt;
		rhstr = k.sBigInt;
		std::reverse(rhstr.begin(), rhstr.end());
		rhstr.resize(n, '0');
		std::reverse(rhstr.begin(), rhstr.end());
	}
	else if (eq == Inequalities::Equal)
	{
		k.sBigInt = "0";
		k.negative = false;
		return k;
	}
	else if (eq == Inequalities::Inconclusive)
	{
		std::string name = "subBigInts";
		ErrorHandling(ErrorTypes::InconclusiveComparison, name);
		return k;
	}
	ret.resize(n, '0');
	for (int i = n - 1; i >= 0; i--)
	{
		int lhdigit, rhdigit, temp;
		//If the current digit of lhstr is less than that of rhstr, then the 
		//next digit will be underloaded(have 1 subtracted from it).
		if (lhstr[i] < rhstr[i])
		{
			//if,else used here for proper char to int conversion.
			if (lhstr[i] == '0')
				lhdigit = 10;
			else
				lhdigit = (lhstr[i] - '0') + 10;//10 added due to lhstr[i] being less than rhstr[i]
			rhdigit = rhstr[i] - '0';
			temp = lhdigit - rhdigit;
			if (underloaded)
				temp--;
			ret[i] = '0' + temp;
			underloaded = true;
		}
		else if (lhstr[i] > rhstr[i])
		{
			lhdigit = lhstr[i] - '0';
			//if,else used here for proper char to int conversion
			if (rhstr[i] == '0' || rhstr[i] == '\0')
				rhdigit = 0;
			else
				rhdigit = rhstr[i] - '0';
			temp = lhdigit - rhdigit;
			if (underloaded)
				temp--;
			if (temp == 0)
				ret[i] = '0';
			else
				ret[i] = '0' + temp;
			underloaded = false;
		}
		else if (lhstr[i] == rhstr[i])
		{
			if (underloaded)
			{
				ret[i] = '9';
				underloaded = true;
			}
			else
				ret[i] = '0';
		}
	}

	k.sBigInt = ret;
	return k;
}

MyBigInt& MyBigInt::multiplyBigInts(MyBigInt &k, const MyBigInt &b)
{
	std::string lhstr;
	std::string rhstr;;
	int digits1, digits2, carry, lhindex, rhindex;
	carry = lhindex = rhindex = 0;

	//lhindex and rhindex represent which digit place the loop is currently on.
	//For example rhindex = 0 is at the 1s place, and lhindex = 5 would be at the 
	//100,000 place.

	lhstr = k.sBigInt;
	rhstr = b.sBigInt;
	digits1 = lhstr.size();
	digits2 = rhstr.size();
	
	std::vector<int> ret(digits2 + digits1, 0);

	//Multiply every digit of lhstr by every digit of rhstr
	for (int i = digits2 - 1; i >= 0; i--)
	{
		int lhdigit, rhdigit;
		rhdigit = rhstr[i] - '0';
		lhindex = 0;

		for (int j = digits1 - 1; j >= 0; j--)
		{
			lhdigit = lhstr[j] - '0';
			int sum = lhdigit * rhdigit + ret[lhindex + rhindex] + carry;
			carry = sum / 10;

			ret[lhindex + rhindex] = sum % 10;

			lhindex++;
		}
		if (carry > 0)
			ret[lhindex + rhindex] += carry;
		rhindex++;
	}
	
	int i = ret.size() - 1;
	while (i >= 0 && ret[i] == 0)
		i--;
	lhstr = "";
	while (i >= 0)
		lhstr += std::to_string(ret[i--]);
	k = lhstr;
	return k;
}

MyBigInt& MyBigInt::divideBigInts(MyBigInt &k, const MyBigInt &b, char op)
{
	std::string name = "dividBigInt";
	std::string lhstr(k.sBigInt);
	std::string rhstr(b.sBigInt);
	std::string quotient, remainder, temp;
	quotient = remainder = temp = "";

	if (compareStrings(lhstr, rhstr) == Inequalities::Lesser)
	{
		if (op == '/')
		{
			k.sBigInt = "0";
		}
		else if (op == '%')
			k.sBigInt = lhstr;
	}
	else if (compareStrings(lhstr, rhstr) == Inequalities::Inconclusive)
	{
		ErrorHandling(ErrorTypes::InconclusiveComparison, name);
		return k;
	}
	else
	{
		int n = lhstr.size();
		for (int i = 0; i < n; i++)
		{
			remainder += lhstr[i];
			//carry more of lhstr's left most digits pushed onto it until carry 
			//is greater that rhstr. If carry is less than rhstr, than 0 is pushed
			//onto ret.
			if (compareStrings(remainder, rhstr) == Inequalities::Greater || compareStrings(remainder, rhstr) == Inequalities::Equal)
			{
				int quotdigit = 1;
				temp = StringTimesInt(rhstr, quotdigit);
				while (compareStrings(temp, remainder) == Inequalities::Lesser)
				{
					quotdigit++;
					temp = StringTimesInt(rhstr, quotdigit);
				}
				//temp is a temporary variable used to hold the value of rhstr multiplied
				//by the currently calcutated digit of the quotient. temp is then subtracted
				//from carry to get the remainder.
				temp = StringTimesInt(rhstr, --quotdigit);
				remainder = StrMinusStr(remainder, temp);

				quotient = quotient + std::to_string(quotdigit);
			}
			else if (compareStrings(remainder, rhstr) == Inequalities::Lesser)
			{
				quotient = quotient + "0";
			}
			else if (compareStrings(remainder, rhstr) == Inequalities::Inconclusive)
			{
				ErrorHandling(ErrorTypes::InconclusiveComparison, name);
				return k;
			}
		}
		//if,else used result for either division or modulo
		if (op == '/')
		{
			//These two while loops are to remove any leading zeros before
			//the string is put into the BigInt variable and the function returns.
			unsigned int index = 0;
			while (index < quotient.size() && quotient[index] == '0')
				index++;
			lhstr = "";
			while (index < quotient.size())
			{
				lhstr += quotient[index++];
			}
			k = lhstr;
		}
		else if (op == '%')
		{
			k = remainder;
		}
	}
	return k;
}

bool MyBigInt::operator== (const MyBigInt &k)
{
	if (this->valid && k.valid)
	{
		if (compareSigned(*this, k) == Inequalities::Equal)
			return true;
		else
			return false;
	}
	else if (this->errorFlag != ErrorTypes::none || k.errorFlag != ErrorTypes::none)
		return false;
	else
	{
		std::string name = "==";
		ErrorHandling(ErrorTypes::InvalidOperation, name);
		return false;
	}
}

bool MyBigInt::operator!= (const MyBigInt &k)
{
	if (this->valid && k.valid)
	{
		if (compareSigned(*this, k) != Inequalities::Equal)
			return true;
		else
			return false;
	}
	else if (this->errorFlag != ErrorTypes::none || k.errorFlag != ErrorTypes::none)
		return false;
	else
	{
		std::string name = "!=";
		ErrorHandling(ErrorTypes::InvalidOperation, name);
		return false;
	}
}

bool MyBigInt::operator> (const MyBigInt &k)
{
	if (this->valid && k.valid)
	{
		if (compareSigned(*this, k) == Inequalities::Greater)
			return true;
		else
			return false;
	}
	else if (this->errorFlag != ErrorTypes::none || k.errorFlag != ErrorTypes::none)
		return false;
	else
	{
		std::string name = ">";
		ErrorHandling(ErrorTypes::InvalidOperation, name);
		return false;
	}
}

bool MyBigInt::operator< (const MyBigInt &k)
{
	if (this->valid && k.valid)
	{
		if (compareSigned(*this, k) == Inequalities::Lesser)
			return true;
		else
			return false;
	}
	else if (this->errorFlag != ErrorTypes::none || k.errorFlag != ErrorTypes::none)
		return false;
	else
	{
		std::string name = "<";
		ErrorHandling(ErrorTypes::InvalidOperation, name);
		return false;
	}
}

bool MyBigInt::operator>= (const MyBigInt &k)
{
	if (this->valid && k.valid)
	{
		Inequalities temp = compareSigned(*this, k);
		if (temp == Inequalities::Greater || temp == Inequalities::Equal)
			return true;
		else
			return false;
	}
	else if (this->errorFlag != ErrorTypes::none || k.errorFlag != ErrorTypes::none)
		return false;
	else
	{
		std::string name = ">=";
		ErrorHandling(ErrorTypes::InvalidOperation, name);
		return false;
	}
}

bool MyBigInt::operator<= (const MyBigInt &k)
{
	if (this->valid && k.valid)
	{
		Inequalities temp = compareSigned(*this, k);
		if (temp == Inequalities::Lesser || temp == Inequalities::Equal)
			return true;
		else
			return false;
	}
	else if (this->errorFlag != ErrorTypes::none || k.errorFlag != ErrorTypes::none)
		return false;
	else
	{
		std::string name = "<=";
		ErrorHandling(ErrorTypes::InvalidOperation, name);
		return false;
	}
}

bool MyBigInt::checkString(const std::string k) const
{
	int n = 0;
	if (k[0] == '-')
		n++;
	return !k.empty() && std::find_if(k.begin() + n,
		k.end(), [](char c) {
		return !std::isdigit(c); }) == k.end();
}

MyBigInt& MyBigInt::operator= (const MyBigInt &k)
{
	if (this->valid && k.valid)
	{
		negative = k.negative;

		sBigInt = k.sBigInt;
		valid = k.valid;
	}
	else if (this->errorFlag != ErrorTypes::none || k.errorFlag != ErrorTypes::none)
	{
		std::string name = "= MyBigInt";
		ErrorHandling(ErrorTypes::InvalidOperation, name);
		negative = false;
		sBigInt = "";
		valid = false;
	}
	return *this;
}

MyBigInt& MyBigInt::operator= (int &k)
{
	if (k < 0)
	{
		negative = true;
		k *= -1;
	}
	sBigInt = std::to_string(k);
	valid = true;
	return *this;
}

MyBigInt& MyBigInt::operator= (unsigned int &k)
{
	sBigInt = std::to_string(k);
	valid = true;
	return *this;
}

MyBigInt& MyBigInt::operator= (long &k)
{
	if (k < 0)
	{
		negative = true;
		k *= -1;
	}
	sBigInt = std::to_string(k);
	valid = true;
	return *this;
}
MyBigInt& MyBigInt::operator= (unsigned long &k)
{
	sBigInt = std::to_string(k);
	valid = true;
	return *this;
}

MyBigInt& MyBigInt::operator= (short &k)
{
	if (k < 0)
	{
		negative = true;
		k *= -1;
	}
	sBigInt = std::to_string(k);
	valid = true;
	return *this;
}

MyBigInt& MyBigInt::operator= (unsigned short &k)
{
	sBigInt = std::to_string(k);
	valid = true;
	return *this;
}

MyBigInt& MyBigInt::operator= (long long &k)
{
	if (k < 0)
	{
		negative = true;
		k *= -1;
	}
	sBigInt = std::to_string(k);
	valid = true;
	return *this;
}

MyBigInt& MyBigInt::operator= (unsigned long long &k)
{
	sBigInt = std::to_string(k);
	valid = true;
	return *this;
}

MyBigInt& MyBigInt::operator= (std::string &k)
{
	if (k[0] == '-')
	{
		negative = true;
		k.erase(0, 1);
	}
	if (checkString(k))
	{
		sBigInt = k;
		valid = true;
	}
	else if (!checkString(k))
	{
		ErrorHandling(ErrorTypes::InvalidString);
	}
	return *this;
}

//The +=, -=, *=, /=, and %= operators all determine the sign of the resulting calculation,
//before calling the appropiate function.
MyBigInt& MyBigInt::operator+= (const MyBigInt &k)
{
	if (this->valid && k.valid)
	{

		if (!this->isNegative() && k.isNegative())
		{
			if (compareUnsigned(*this, k) == Inequalities::Lesser)
				this->negative = true;
			*this = subBigInts(*this, k);
		}
		else if (this->isNegative() && !k.isNegative())
		{
			if (compareUnsigned(*this, k) == Inequalities::Lesser)
				this->negative = false;
			*this = subBigInts(*this, k);
		}
		else if (this->isNegative() && k.isNegative())
		{
			*this = addBigInts(*this, k);
		}
		else
		{
			*this = addBigInts(*this, k);
		}

		if (!checkString(sBigInt))
		{
			std::string name = "+=";
			ErrorHandling(ErrorTypes::FailedOperation, name);
			valid = false;
		}
	}
	else if(this->errorFlag != ErrorTypes::none || k.errorFlag != ErrorTypes::none)
	{
		std::string name = "+=";
		ErrorHandling(ErrorTypes::InvalidOperation, name);
	}
	return *this;
}

MyBigInt& MyBigInt::operator-= (const MyBigInt &k)
{
	if (this->valid && k.valid)
	{
		Inequalities eq = compareUnsigned(*this, k);
		if (!this->isNegative() && !k.isNegative())
		{
			if (eq == Inequalities::Lesser)
				this->negative = true;
			*this = subBigInts(*this, k);
		}
		else if (!this->isNegative() && k.isNegative())
		{
			*this = addBigInts(*this, k);
		}
		else if (this->isNegative() && k.isNegative())
		{
			if (eq == Inequalities::Lesser)
				this->negative = false;
			*this = subBigInts(*this, k);
		}
		else if (this->isNegative() && !k.isNegative())
		{
			*this = addBigInts(*this, k);
		}

		if (!checkString(sBigInt))
		{
			std::string name = "-=";
			ErrorHandling(ErrorTypes::FailedOperation, name);
			valid = false;
		}
	}
	else if (this->errorFlag != ErrorTypes::none || k.errorFlag != ErrorTypes::none)
	{
		std::string name = "-=";
		ErrorHandling(ErrorTypes::InvalidOperation, name);
	}
	return *this;
}

MyBigInt& MyBigInt::operator*= (const MyBigInt &k)
{
	if (this->valid && k.valid)
	{
		if (this->negative != k.negative)
			this->negative = true;
		else
			this->negative = false;
		*this = multiplyBigInts(*this, k);
		if (!checkString(sBigInt))
		{
			std::string name = "*=";
			ErrorHandling(ErrorTypes::FailedOperation, name);
			valid = false;
		}
	}
	else if (this->errorFlag != ErrorTypes::none || k.errorFlag != ErrorTypes::none)
	{
		std::string name = "*=" ;
		ErrorHandling(ErrorTypes::InvalidOperation, name);
	}
	return *this;
}

MyBigInt& MyBigInt::operator/= (const MyBigInt &k)
{
	if (this->valid && k.valid)
	{
		if (this->negative != k.negative)
			this->negative = true;
		else
			this->negative = false;
		*this = divideBigInts(*this, k, '/');

		if (!checkString(sBigInt))
		{
			std::string name = "/=";
			ErrorHandling(ErrorTypes::FailedOperation, name);
			valid = false;
		}
		else if (k.sBigInt == "0")
		{
			ErrorHandling(ErrorTypes::DivisionByZero);
		}
	}
	else if (this->errorFlag != ErrorTypes::none || k.errorFlag != ErrorTypes::none)
	{
		std::string name = "/=";
		ErrorHandling(ErrorTypes::InvalidOperation, name);
	}
	return *this;
}

MyBigInt& MyBigInt::operator%= (const MyBigInt &k)
{
	if (this->valid && k.valid)
	{
		if (this->negative != k.negative)
			this->negative = true;
		else
			this->negative = false;
		*this = divideBigInts(*this, k, '%');

		if (!checkString(sBigInt))
		{
			std::string name = "%=";
			ErrorHandling(ErrorTypes::FailedOperation, name);
			valid = false;
		}
		else if (k.sBigInt == "0")
		{
			ErrorHandling(ErrorTypes::DivisionByZero);
		}
	}
	else if (this->errorFlag != ErrorTypes::none || k.errorFlag != ErrorTypes::none)
	{
		std::string name = "%=";
		ErrorHandling(ErrorTypes::InvalidOperation, name);
	}
	return *this;
}

MyBigInt MyBigInt::operator+ (const MyBigInt &k)
{
	MyBigInt returnInt(*this);
	return returnInt += k;
}

MyBigInt MyBigInt::operator- (const MyBigInt &k)
{
	MyBigInt ret(*this);
	return ret -= k;
}

MyBigInt MyBigInt::operator*(const MyBigInt &k)
{
	MyBigInt ret(*this);
	return ret *= k;
}

MyBigInt MyBigInt::operator/ (const MyBigInt &k)
{
	MyBigInt ret(*this);
	return ret /= k;
}

MyBigInt MyBigInt::operator% (const MyBigInt &k)
{
	MyBigInt ret(*this);
	return ret %= k;
}

MyBigInt::operator short()
{
	std::string temp = std::to_string(SHRT_MAX);
	Inequalities eq = compareStrings(this->sBigInt, temp);
	short ret = 0;
	if (eq == Inequalities::Greater || eq == Inequalities::Inconclusive)
	{
		std::string name = "short";
		ErrorHandling(ErrorTypes::InvalidConversion, name);
	}
	else
	{
		ret = std::stoi(sBigInt);
	}	
	return ret;
}

MyBigInt::operator unsigned short()
{
	std::string temp = std::to_string(USHRT_MAX);
	Inequalities eq = compareStrings(this->sBigInt, temp);
	unsigned short ret = 0;
	if (eq == Inequalities::Greater || eq == Inequalities::Inconclusive)
	{
		std::string name = "short";
		ErrorHandling(ErrorTypes::InvalidConversion, name);
	}
	else if (isNegative())
	{
		std::string name = "short";
		ErrorHandling(ErrorTypes::InvalidUnsignedConversion, name);
	}
	else
	{
		ret = std::stoi(sBigInt);
	}
	return ret;
}

MyBigInt::operator int()
{
	std::string temp = std::to_string(INT_MAX);
	Inequalities eq = compareStrings(this->sBigInt, temp);
	int ret = 0;
	if (eq == Inequalities::Greater || eq == Inequalities::Inconclusive)
	{
		std::string name = "int";
		ErrorHandling(ErrorTypes::InvalidConversion, name);
	}
	else
	{
		ret = std::stoi(sBigInt);
	}
	return ret;
}

MyBigInt::operator unsigned int()
{
	std::string temp = std::to_string(UINT_MAX);
	Inequalities eq = compareStrings(this->sBigInt, temp);
	unsigned int ret = 0;
	if (eq == Inequalities::Greater || eq == Inequalities::Inconclusive)
	{
		std::string name = "int";
		ErrorHandling(ErrorTypes::InvalidConversion, name);
	}
	else if (isNegative())
	{
		std::string name = "int";
		ErrorHandling(ErrorTypes::InvalidUnsignedConversion, name);
	}
	else
	{
		ret = std::stoi(sBigInt);
	}
	return ret;
}

MyBigInt::operator long()
{
	std::string temp = std::to_string(LONG_MAX);
	Inequalities eq = compareStrings(this->sBigInt, temp);
	long ret = 0;
	if (eq == Inequalities::Greater || eq == Inequalities::Inconclusive)
	{
		std::string name = "long";
		ErrorHandling(ErrorTypes::InvalidConversion, name);
	}
	else
	{
		ret = std::stoi(sBigInt);
	}
	return ret;
}

MyBigInt::operator unsigned long()
{
	std::string temp = std::to_string(ULONG_MAX);
	unsigned long ret = 0;
	Inequalities eq = compareStrings(this->sBigInt, temp);
	if (eq == Inequalities::Greater || eq == Inequalities::Inconclusive)
	{
		std::string name = "long";
		ErrorHandling(ErrorTypes::InvalidConversion, name);
	}
	else if (isNegative())
	{
		std::string name = "long";
		ErrorHandling(ErrorTypes::InvalidUnsignedConversion, name);
	}
	else
	{
		ret = std::stoi(sBigInt);
	}
	return ret;
}

MyBigInt::operator long long()
{
	std::string temp = std::to_string(LLONG_MAX);
	Inequalities eq = compareStrings(this->sBigInt, temp);
	long long ret = 0;
	if (eq == Inequalities::Greater || eq == Inequalities::Inconclusive)
	{
		std::string name = "long long";
		ErrorHandling(ErrorTypes::InvalidConversion, name);
	}
	else
	{
		ret = std::stoi(sBigInt);
	}
	return ret;
}

MyBigInt::operator unsigned long long()
{
	std::string temp = std::to_string(ULLONG_MAX);
	Inequalities eq = compareStrings(this->sBigInt, temp);
	unsigned long long ret = 0;
	if (eq == Inequalities::Greater || eq == Inequalities::Inconclusive)
	{
		std::string name = "long long";
		ErrorHandling(ErrorTypes::InvalidConversion, name);
	}
	else if (isNegative())
	{
		std::string name = "long long";
		ErrorHandling(ErrorTypes::InvalidUnsignedConversion, name);
	}
	else
	{
		ret = std::stoi(sBigInt);
	}
	return ret;
}

MyBigInt::operator std::string()
{
	return sBigInt;
}

std::string MyBigInt::StringTimesInt(std::string &k, const int &b)
{
	MyBigInt str(k);
	MyBigInt num(b);

	str *= num;
	return str.sBigInt;
}

std::string MyBigInt::StrMinusStr(std::string &k, std::string &b)
{
	MyBigInt str1(k);
	MyBigInt str2(b);
	str1 -= str2;

	//These two while loops remove any leading zeroes that could alter
	//calculations used by str1.
	int index = 0;
	while (index < str1.GetLength() && str1.sBigInt[index] == '0')
		index++;
	std::string temp = "";
	while (index < str1.GetLength())
	{
		temp += str1.sBigInt[index++];
	}
	str1.sBigInt = temp;
	return str1.sBigInt;
}



