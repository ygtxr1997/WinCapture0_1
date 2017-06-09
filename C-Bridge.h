#pragma once

#include "Bridge-WinCapture.h"

#define ZEGO_InitWinCapture()	InitWinCapture()

#define ZEGO_AddProducer()		AddProducer()

#define ZEGO_AddConsumer()	AddConsumer()

#define ZEGO_StopProducers()	StopProducers()

#define ZEGO_RestartTasks()		ReStartTasks()

#define ZEGO_SetCaptureCallback(pwcFrameData)		SetCaptureCallback(pwcFrameData)

#define ZEGO_IsCapturing()		IsCapturing()

#define ZEGO_SetFPS(uFPS)			SetFPS(uFPS)

#define ZEGO_SetCaptureTargetByWinText(pcWinText)	SetCaptureTargetByWinText(pcWinText)

#define ZEGO_SetCaptureTargetByRECT(rt, bFollowupCursor, ptAnchor)	SetCaptureTargetByRECT(rt, bFollowupCursor, ptAnchor)

#define ZEGO_SetCaptureTargetByXYWH(x, y, width, height)		SetCaptureTargetByXYWH(x, y, width, height)

#define ZEGO_SetEnableCursorDisplay(bDisplay)			SetEnableCursorDisplay(bDisplay)

#define ZEGO_OnFinishedOneFrame(outFrameData)		OnFinishedOneFrame(outFrameData)
