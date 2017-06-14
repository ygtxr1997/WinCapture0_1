#include "Bridge-WinCapture.h"

typedef const unsigned int CUI;

namespace ZEGO {

	namespace Capture {

		static ThreadManager* threadManager;

		void InitWinCapture()
		{
			threadManager = new ThreadManager;
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

		void SetCaptureCallback(ICallback** ppCallback)
		{
			threadManager->SetCaptureCallback(ppCallback);
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

		void GetWindowList(WINCAPTURE_WINLIST* outWinList)
		{
			threadManager->GetWindowList(outWinList);
		}

		void FreeWindowList(WINCAPTURE_WINLIST* inWinList)
		{
			threadManager->FreeWindowList(inWinList);
		}
	}
}