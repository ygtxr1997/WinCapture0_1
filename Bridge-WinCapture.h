#pragma once

#include "CommonTypes.h"
#include "ThreadManager.h"
#include "WinCapture.h"

typedef const unsigned int CUI;

static ThreadManager* threadManager;

void InitWinCapture()
{
	threadManager = new ThreadManager();
	threadManager->EnableCursorDisplay(true);
	// threadManager->SetCaptureCallback();
}

void AddProducer()
{
	threadManager->AddProducer();
}

void AddConsumer()
{
	threadManager->AddConsumer();
}

void StopProducers()
{
	threadManager->StopProducers();
}

void RestartTasks()
{
	threadManager->ReStartTasks();
}

void SetCaptureCallback(ICallback* pCallback)
{
	threadManager->SetCaptureCallback(pCallback);
}

bool IsCapturing()
{
	return threadManager->IsCapturing() ? true : false;
}

void SetFPS(unsigned int uFPS)
{
	threadManager->SetFPS(uFPS);
}

void SetCaptureTargetByWinText(const char * pcWinText)
{
	threadManager->SetCaptureTarget(std::string(pcWinText));
}

void SetCaptureTargetByRECT(RECT rt, bool bFollowupCursor, POINT ptAnchor)
{
	threadManager->SetCaptureTarget(rt, bFollowupCursor, ptAnchor);
}

void SetCaptureTargetByXYWH(CUI x, CUI y, CUI width, CUI height)
{
	threadManager->SetCaptureTarget(x, y, width, height);
}

void SetEnableCursorDisplay(bool bDisplay)
{
	threadManager->EnableCursorDisplay(bDisplay);
}

//
// callback
//
void OnFinishedOneFrame(WINCAPTURE_FRAMEDATA* outFrameData)
{
	threadManager->OnFinishedOneFrame(outFrameData);
}