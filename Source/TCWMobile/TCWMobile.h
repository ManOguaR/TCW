// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#ifndef __NETWORKSESSIONTEST_H__
#define __NETWORKSESSIONTEST_H__

#include "Engine.h"
#include "Net/UnrealNetwork.h"
#include "Online.h"
#endif

//General Log
DECLARE_LOG_CATEGORY_EXTERN(TCWLog, Log, All);
//Logging during game startup
DECLARE_LOG_CATEGORY_EXTERN(TCWLogInit, Log, All);
//Logging for your AI system
DECLARE_LOG_CATEGORY_EXTERN(TCWLogAI, Log, All);
//Logging for Critical Errors that must always be addressed
DECLARE_LOG_CATEGORY_EXTERN(TCWLogErrors, Error, All);

#define GETENUMSTRING(etype, evalue) ((FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true) != nullptr) ? FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true)->GetNameStringByIndex((int32)evalue) : FString("Invalid - are you sure enum uses UENUM() macro?"))
#define GETENUMTEXT(etype, evalue) (FText::FromString(GETENUMSTRING(etype, evalue)))