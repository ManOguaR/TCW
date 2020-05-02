// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ValidityFunctionLibrary.generated.h"

USTRUCT(BlueprintType)
struct FPasswordValidationRules
{
	GENERATED_BODY()

public:
	FPasswordValidationRules();
	FPasswordValidationRules(int minLength, int maxLength, bool canContainSymbols, bool mustContainSymbols, bool canContainNumbers, bool mustContainNumbers);
	UPROPERTY(BlueprintReadWrite, Category = "Password validation")
		int PasswordMinLength;
	UPROPERTY(BlueprintReadWrite, Category = "Password validation")
		int PasswordMaxLength;
	UPROPERTY(BlueprintReadWrite, Category = "Password validation")
		bool CanContainSymbols;
	UPROPERTY(BlueprintReadWrite, Category = "Password validation")
		bool MustContainSymbols;
	UPROPERTY(BlueprintReadWrite, Category = "Password validation")
		bool CanContainNumbers;
	UPROPERTY(BlueprintReadWrite, Category = "Password validation")
		bool MustContainNumbers;
};

/**
 *
 */
UCLASS()
class UValidityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Email Validation")
		static bool IsEmailValid(const FString& emailString);
	UFUNCTION(BlueprintPure, Category = "Password Validation")
		static bool IsPasswordValid(const FString& passwordString, const FPasswordValidationRules& rules);

};
