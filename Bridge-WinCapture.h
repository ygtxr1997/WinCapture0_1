#pragma once

#include "CommonTypes.h"
#include "ThreadManager.h"
#include "WinCapture.h"

typedef const unsigned int CUI;

namespace ZEGO {

	namespace Capture {

		void InitWinCapture();

		void AddProducer();

		void AddConsumer();

		void StopProducers();

		void RestartTasks();

		void SetCaptureCallback(ICallback** ppCallback);

		bool IsCapturing();

		void SetFPS(unsigned int uFPS);

		void SetCaptureTargetByWinText(const char * pcWinText);

		void SetCaptureTargetByRECT(RECT rt, bool bFollowupCursor, POINT ptAnchor);

		void SetCaptureTargetByXYWH(CUI x, CUI y, CUI width, CUI height);

		void SetEnableCursorDisplay(bool bDisplay);

		void GetWindowList(WINCAPTURE_WINLIST* outWinList);

		void FreeWindowList(WINCAPTURE_WINLIST* inWinList);
	}
}