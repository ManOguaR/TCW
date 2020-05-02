// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "ValidityFunctionLibrary.h"
#include <regex>
#include <string>

FPasswordValidationRules::FPasswordValidationRules() :
	FPasswordValidationRules(6, 16, true, true, true, true)
{
	//PRE - 29
}

FPasswordValidationRules::FPasswordValidationRules(int minLength, int maxLength, bool canContainSymbols, bool mustContainSymbols, bool canContainNumbers, bool mustContainNumbers) :
	PasswordMinLength(minLength),
	PasswordMaxLength(maxLength),
	CanContainSymbols(canContainSymbols),
	MustContainSymbols(mustContainSymbols),
	CanContainNumbers(canContainNumbers),
	MustContainNumbers(mustContainNumbers) 
{

}

bool UValidityFunctionLibrary::IsEmailValid(const FString& emailString)
{
	const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
	return std::regex_match(TCHAR_TO_UTF8(*emailString), pattern);
}

bool UValidityFunctionLibrary::IsPasswordValid(const FString& passwordText, const FPasswordValidationRules& rules)
{
	if (passwordText.Len() < rules.PasswordMinLength || passwordText.Len() > rules.PasswordMaxLength)
		return false;

	const std::string strPtrn("^ (? = .*[a - z])(? = .*[A - Z]).{%d, %d}$", rules.PasswordMinLength, rules.PasswordMaxLength);
	const std::regex pattern(strPtrn);
	const std::regex numPtrn("(? = .*\\d)");
	const std::regex symPtrn("(? = .*[#$ ^ += !*()@%&])");

	bool validLen = std::regex_match(TCHAR_TO_UTF8(*passwordText), pattern);

	bool hasNumbers = std::regex_match(TCHAR_TO_UTF8(*passwordText), numPtrn);
	bool hasSymbols = std::regex_match(TCHAR_TO_UTF8(*passwordText), symPtrn);
	return validLen && 
		((!hasNumbers && !rules.CanContainNumbers) || (hasNumbers && rules.MustContainNumbers) || !rules.MustContainNumbers) && 
		((!hasSymbols && !rules.CanContainSymbols) || (hasSymbols && rules.MustContainSymbols) || !rules.MustContainSymbols);
}
