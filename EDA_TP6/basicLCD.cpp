#include <iostream>

#include "basicLCD.h"


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