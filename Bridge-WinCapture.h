#pragma once

#include "CommonTypes.h"
#include "ThreadManager.h"
#include "WinCapture.h"

static ThreadManager* threadManager;

void InitWinCapture()
{
	threadManager = new ThreadManager();
	threadManager->EnableCursorDisplay(false);
}

void AddProducer()
{
	threadManager->AddProducer();
}

void AddConsumer()
{
	threadManager->AddConsumer();
}