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
