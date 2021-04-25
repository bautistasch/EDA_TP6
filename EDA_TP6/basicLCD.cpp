#include <iostream>

#include "basicLCD.h"

using namespace std;

lcdError::lcdError(string name, string description, unsigned long code) : errorName(name), errorDescription(description), errorCode(code){}

std::string lcdError::getErrorName()
{
	return errorName;
}
std::string lcdError::getErrorDescription()
{
	return errorDescription;
}
unsigned long lcdError::getErrorCode()
{
	return errorCode;
}