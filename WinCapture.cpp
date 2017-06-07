#include "WinCapture.h"
#include <time.h>

//
// 初始化列表构造函数
//
WinCapture::WinCapture() : m_IsCapturing(WINCAPTURE_FALSE),
	m_WinList(new WINCAPTURE_WINLIST),
	m_CaptureSetting(new WINCAPTURE_SETTING),
	m_CaptureMode(WINCAPTURE_MODE_FULLSCREEN),
	m_FrameData(new WINCAPTURE_FRAMEDATA)
{
}

//
// 析构函数
//
WinCapture::~WinCapture()
{
	if (m_WinList) {
		delete m_WinList;
		m_WinList = nullptr;
	}
	if (m_CaptureSetting) {
		delete m_CaptureSetting;
		m_CaptureSetting = nullptr;
	}
	if (m_FrameData) {
		delete m_FrameData;
		m_FrameData = nullptr;
	}
}

//
// 设置回调
//
WResult WinCapture::SetCaptureCallback()
{
	return WINCAPTURE_SUCCESS;
}

//
// 开始捕获
//
WResult WinCapture::StartCapture()
{
	// 如果捕获模式是0(即全屏捕获模式)
	if (!m_CaptureMode) {
		m_CaptureSetting->TargetRect.left = 0;
		m_CaptureSetting->TargetRect.right = GetSystemMetrics(SM_CXSCREEN);
		m_CaptureSetting->TargetRect.top = 0;
		m_CaptureSetting->TargetRect.bottom = GetSystemMetrics(SM_CYSCREEN);
	}

	// 设置状态为正在捕获
	m_IsCapturing = WINCAPTURE_TURE;

	// 开始截图捕获
	if (_GetSnapShotByRect(m_CaptureSetting->TargetRect)) {	// 截图失败
		OutputDebugString("-----截图失败\n");
		return WINCAPTURE_ERROR_EXPECTED;
	}
	else  {																						// 截图成功
		OutputDebugString("+++++截图成功\n");
		return WINCAPTURE_SUCCESS;
	}
}

//
// 停止捕获
//
WResult WinCapture::StopCapture()
{
	// 设置状态为停止捕获
	m_IsCapturing = WINCAPTURE_FALSE;

	return WINCAPTURE_SUCCESS;
}

//
// 是否正在捕获
//
WINCAPTURE_ISCAPTURING WinCapture::IsCapturing()
{
	return m_IsCapturing;
}

//
// 设置FPS
//
WResult WinCapture::SetFPS(unsigned int uFPS = 15)
{
	if (uFPS > WINCAPTURE_MAX_FPS)
		return WINCAPTURE_ERROR_EXPECTED;
	m_CaptureSetting->FPS = uFPS;
	return WINCAPTURE_SUCCESS;
}

//
// 获取窗口列表
//
std::vector<std::string> WindowTitle;
/// --- WinCapture::GetWindowList() 辅助用:
/// --- 枚举窗口
/// ---
BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
		if (GetParent(hWnd) == NULL && IsWindowVisible(hWnd)) {	// GetParent(hWnd) == NULL && IsWindowVisible(hWnd)
			char tempTitle[100] = { 0 };
			::GetWindowText(hWnd, tempTitle, 100);
			WindowTitle.push_back(tempTitle);
		}
		return TRUE;
}
// -- WinCapture::GetWindowList() 本体
WResult WinCapture::GetWindowList()
{
	this->FreeWindowList();
	WindowTitle.clear();

	OutputDebugString("----------------------开始显示窗口列表-------------------\n");

	EnumWindows(EnumWindowsProc, NULL);

	for (UINT i = 0; i < WindowTitle.size(); ++i) {
			m_WinList->Title.push_back(WindowTitle[i]);
			if (WindowTitle[i][0]) {
				OutputDebugString(WindowTitle[i].c_str());
				OutputDebugString("\n");
			}
	}

	OutputDebugString("-----------------------窗口列表显示完毕--------------------\n");
	return WINCAPTURE_SUCCESS;
}

//
// 释放窗口列表
//
void WinCapture::FreeWindowList()
{
	if (m_WinList) {
		delete m_WinList;
		m_WinList = new WINCAPTURE_WINLIST;
	}
}

//
// 根据WinID(WinTitle)设置捕获目标
// 
WResult WinCapture::SetCaptureTarget(std::string WinText)
{
	// 获取窗口列表
	if (!m_WinList->Title.size())
		this->GetWindowList();

	// 寻找指定窗口
	bool findFlag = false;
	unsigned int index = 0;
	for (; index < m_WinList->Title.size() && !findFlag; index++) {
		if (m_WinList->Title[index] == WinText)
			findFlag = true;
	}

	// 未找到窗口ID
	if (!findFlag) {
		OutputDebugString("窗口不存在\n");
		return WINCAPTURE_ERROR_EXPECTED;
	}

	// 获取WinID对应的句柄
	HWND wantedWindow = FindWindow(NULL, (m_WinList->Title[index - 1]).data());

	// 根据句柄设置矩形范围
	if (!GetWindowRect(wantedWindow, (LPRECT)&(m_CaptureSetting->TargetRect)))
	{
		OutputDebugString("窗口未找到\n");
		return WINCAPTURE_ERROR_EXPECTED;
	} 
	else {
		m_CaptureSetting->WinID = wantedWindow;
		if (IsIconic(wantedWindow)) {		// 窗口处于最小化
			m_CaptureMode = WINCAPTURE_MODE_FULLSCREEN;
			OutputDebugString("窗口处于最小化\n");
			return WINCAPTURE_ERROR_EXPECTED;
		}
		else {
			// 对于超出屏幕显示范围的坐标值进行修正
			if (m_CaptureSetting->TargetRect.left < 0) m_CaptureSetting->TargetRect.left = 0;
			if (m_CaptureSetting->TargetRect.top < 0) m_CaptureSetting->TargetRect.top = 0;
			if (m_CaptureSetting->TargetRect.right > GetSystemMetrics(SM_CXSCREEN)) m_CaptureSetting->TargetRect.right = GetSystemMetrics(SM_CXSCREEN);
			if (m_CaptureSetting->TargetRect.bottom > GetSystemMetrics(SM_CYSCREEN)) m_CaptureSetting->TargetRect.bottom = GetSystemMetrics(SM_CYSCREEN);

			m_CaptureMode = WINCAPTURE_MODE_WINID;
			OutputDebugString("成功找到窗口\n");
			return WINCAPTURE_SUCCESS;
		}
	}
}

//
// 根据矩形RECT设置捕获范围
//
WResult WinCapture::SetCaptureTarget(RECT rt, bool bFollowupCursor, POINT ptAnchor)
{
	if (rt.left < rt.right && rt.bottom < rt.top
		&& rt.left > 0 && rt.right < GetSystemMetrics(SM_CXSCREEN)
		&& rt.top > 0 && rt.bottom < GetSystemMetrics(SM_CYSCREEN)) {
		m_CaptureSetting->TargetRect = rt;
		m_CaptureSetting->IsFollowCursor = bFollowupCursor;
		m_CaptureSetting->Anchor = ptAnchor;

		m_CaptureMode = WINCAPTURE_MODE_RECT;
		OutputDebugString("设置成功\n");
		return WINCAPTURE_SUCCESS;
	}
	else {
		OutputDebugString("矩形范围不合法\n");
		return WINCAPTURE_ERROR_EXPECTED;
	}
}

//
// 根据x, y, width, height设置捕获范围
//
WResult WinCapture::SetCaptureTarget(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned height, const bool bFollowupCursor, POINT ptAnchor)
{
	if (x <= (UINT)GetSystemMetrics(SM_CXSCREEN)
		&& x + width <= (UINT)GetSystemMetrics(SM_CXSCREEN)
		&& y <= (UINT)GetSystemMetrics(SM_CYSCREEN)
		&& y + height <= (UINT)GetSystemMetrics(SM_CYSCREEN) ){
		m_CaptureSetting->TargetRect.left = x;
		m_CaptureSetting->TargetRect.top = y;
		m_CaptureSetting->TargetRect.right = x + width;
		m_CaptureSetting->TargetRect.bottom = y + height;

		m_CaptureSetting->IsFollowCursor = bFollowupCursor;

		m_CaptureSetting->Anchor = ptAnchor;

		m_CaptureMode = WINCAPTURE_MODE_XYWH;
		OutputDebugString("设置成功\n");
		return WINCAPTURE_SUCCESS;
	}
	else {
		OutputDebugString("坐标或者位置设置不合法\n");
		return WINCAPTURE_ERROR_EXPECTED;
	}
}

//
// 是否显示鼠标
//
void WinCapture::EnableCursorDisplay(bool bDisplay)
{
	m_CaptureSetting->IsDisplay = bDisplay;
}

// ------------------------- Callback ----------------------------

//
// 捕获完毕
//
void WinCapture::OnFinishedOneFrame(WINCAPTURE_FRAMEDATA* frameData)
{
	frameData->Data = m_FrameData->Data;
	frameData->BytesPerLine = m_FrameData->BytesPerLine;
	frameData->CursorPos = m_FrameData->CursorPos;
	// bytesPerLine = m_FrameData->BytesPerLine;
	// uTimeStamp = m_CaptureSetting->TimeStamp;
	// ptMousePos = { 0, 0 };
}


/*****************************************************************************************/
/********************************* private 方法 *****************************************/
/****************************************************************************************/

//
// 根据矩形范围获取截图
//
//
WResult WinCapture::_GetSnapShotByRect(RECT targetRECT)
{
	HWND testWin = (HWND)m_CaptureSetting->WinID;

	// 根据当前模式选择HDC为屏幕或者指定窗口
	HDC hDC = m_CaptureMode == 0 ? GetDC(NULL) : GetDC(testWin);

	// 算出长宽
	UINT width = targetRECT.right - targetRECT.left;
	UINT height = targetRECT.bottom - targetRECT.top;

	// 获取桌面DC
	// HDC hDC;
	// hDC = GetDC(GetDesktopWindow());

	// 桌面DC的适配器
	HDC hCompatibleDC = CreateCompatibleDC(hDC);

	// 新建桌面DC对应的位图句柄
	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, width, height);

	// 将位图传入DC适配器
	SelectObject(hCompatibleDC, hBitmap);

	clock_t t1, t2;	// just for test
	t1 = clock();		// just for test
	// 使用BitBlt
	BitBlt(hCompatibleDC, 0, 0, width, height, hDC, targetRECT.left, targetRECT.top, SRCCOPY);
	t2 = clock();		// just for test

	// 获取调色板
	HPALETTE hPalette = (HPALETTE)GetStockObject(DEFAULT_PALETTE);

	// bitmap实体与句柄绑定
	BITMAP _bitmap;
	GetObject(hBitmap, sizeof(_bitmap), (LPVOID)&_bitmap);

	// bitmap信息头
	BITMAPINFOHEADER bitmapInfoHeader;
	ZeroMemory(&bitmapInfoHeader, sizeof(BITMAPCOREHEADER));
	bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader.biWidth = _bitmap.bmWidth;
	bitmapInfoHeader.biHeight = _bitmap.bmHeight;
	bitmapInfoHeader.biPlanes = 1;
	bitmapInfoHeader.biBitCount = 24;
	bitmapInfoHeader.biCompression = BI_RGB;

	// 计算信息头及颜色表大小
	int nColors = (1 << bitmapInfoHeader.biBitCount);
	if (nColors > 256)
		nColors = 0;

	DWORD dwLength = bitmapInfoHeader.biSize + nColors * sizeof(RGBQUAD);

	// 临时DC
	HDC hTmpDC = GetDC(NULL);
	hPalette = SelectPalette(hTmpDC, hPalette, FALSE);
	RealizePalette(hTmpDC);

	// 为信息头和颜色表分配内存
	HANDLE hDIB = GlobalAlloc(GMEM_FIXED, dwLength);
	if (!hDIB) {
		return WINCAPTURE_ERROR_EXPECTED;
	}

	// 获取位图数据
	LPBITMAPINFOHEADER lpBitmapInfoHeader = (LPBITMAPINFOHEADER)hDIB;
	*lpBitmapInfoHeader = bitmapInfoHeader;

	// 调用GetBIDBITS
	BOOL b = GetDIBits(hTmpDC, hBitmap, 0L, (DWORD)bitmapInfoHeader.biHeight, (LPBYTE)NULL, (LPBITMAPINFO)lpBitmapInfoHeader, (DWORD)DIB_RGB_COLORS);

	bitmapInfoHeader = *lpBitmapInfoHeader;

	// 图像的每行都对齐32字节
	if (bitmapInfoHeader.biSizeImage == 0) {
		bitmapInfoHeader.biSizeImage = ((((bitmapInfoHeader.biWidth * bitmapInfoHeader.biBitCount) + 31) & ~31) / 8) *bitmapInfoHeader.biHeight;

		if (bitmapInfoHeader.biCompression != BI_RGB)
			bitmapInfoHeader.biSizeImage = (bitmapInfoHeader.biSizeImage * 3) / 2;
	}

	// 重新分配内存大小
	dwLength += bitmapInfoHeader.biSizeImage;
	HANDLE hMem = NULL;
	if (hMem = GlobalReAlloc(hDIB, dwLength, GMEM_MOVEABLE)) {
		hDIB = hMem;
	}
	else {
		GlobalFree(hDIB);
		SelectPalette(hTmpDC, hPalette, FALSE);
		ReleaseDC(NULL, hTmpDC);
		return WINCAPTURE_ERROR_EXPECTED;
	}

	lpBitmapInfoHeader = (LPBITMAPINFOHEADER)hDIB;


	b = GetDIBits(hTmpDC, hBitmap, 0L, (DWORD)bitmapInfoHeader.biHeight,
		(LPBYTE)lpBitmapInfoHeader + (bitmapInfoHeader.biSize + nColors * sizeof(RGBQUAD)),
		(LPBITMAPINFO)lpBitmapInfoHeader, (DWORD)DIB_RGB_COLORS);

	m_CaptureSetting->IsDisplay = true;
	// 是否需要绘制鼠标
	if (m_CaptureSetting->IsDisplay) {
		// 获取鼠标位置
		LPPOINT cursorPos = new POINT;
		RtlZeroMemory(cursorPos, sizeof(POINT));
		GetCursorPos(cursorPos);

		// 边界处理
		if (cursorPos->x < targetRECT.left)		cursorPos->x = targetRECT.left;
		if (cursorPos->y < targetRECT.top)		cursorPos->y = targetRECT.top;
		if (cursorPos->x > targetRECT.right)		cursorPos->x = targetRECT.right;
		if (cursorPos->y > targetRECT.bottom - 30)		cursorPos->y = targetRECT.bottom - 30;


		m_FrameData->CursorPos = cursorPos;

		// 绘制鼠标
		BYTE* p = (LPBYTE)hDIB;
		p += lpBitmapInfoHeader->biSize;
		for (int i = cursorPos->x - targetRECT.left; i < cursorPos->x - targetRECT.left + 20 && i >= 30 && i <= GetSystemMetrics(SM_CXSCREEN) - 30; i++) {
			for (int j = cursorPos->y - targetRECT.top; j < cursorPos->y - targetRECT.top + 30 && j >= 30 && j <= GetSystemMetrics(SM_CYSCREEN) - 30; j++) {
				if ((i < cursorPos->x - targetRECT.left + 3 || i > cursorPos->x - targetRECT.left + 17)
					&& (j < cursorPos->y - targetRECT.top + 3 || cursorPos->y - targetRECT.top + 27)) {
					if (p + 3 * ((bitmapInfoHeader.biHeight - j) * bitmapInfoHeader.biWidth + (i))) {
						*(p + 3 * ((bitmapInfoHeader.biHeight - j) * bitmapInfoHeader.biWidth + (i))) = 0;
						*(p + 1 + 3 * ((bitmapInfoHeader.biHeight - j) * bitmapInfoHeader.biWidth + (i))) = 0;
						*(p + 2 + 3 * ((bitmapInfoHeader.biHeight - j) * bitmapInfoHeader.biWidth + (i))) = 0;
					}
				}
				else {
					if (p + 3 * ((bitmapInfoHeader.biHeight - j) * bitmapInfoHeader.biWidth + (i))) {
						*(p + 3 * ((bitmapInfoHeader.biHeight - j) * bitmapInfoHeader.biWidth + (i))) = 218;
						*(p + 1 + 3 * ((bitmapInfoHeader.biHeight - j) * bitmapInfoHeader.biWidth + (i))) = 180;
						*(p + 2 + 3 * ((bitmapInfoHeader.biHeight - j) * bitmapInfoHeader.biWidth + (i))) = 18;
					}
				}
			}
		}
	}

	if (!b)
	{
		GlobalFree(hDIB);
		SelectPalette(hTmpDC, hPalette, FALSE);
		ReleaseDC(NULL, hTmpDC);
		return WINCAPTURE_ERROR_EXPECTED;
	}

	SelectPalette(hTmpDC, hPalette, FALSE);
	ReleaseDC(NULL, hTmpDC);

	// 文件操作
	if (WINCAPTURE_SAVE_FILE) {
		BITMAPFILEHEADER* bitmapFileHeader = new BITMAPFILEHEADER;
		ZeroMemory(bitmapFileHeader, sizeof(BITMAPFILEHEADER));
		bitmapFileHeader->bfType = (DWORD)0x4D42;
		bitmapFileHeader->bfSize = 54 + bitmapInfoHeader.biSizeImage;
		bitmapFileHeader->bfOffBits = 54;

		DWORD written = 0;

		HANDLE hFile = CreateFile("C:/Users/apple/Pictures/CapureDemo0_1.bmp", GENERIC_ALL, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			return WINCAPTURE_ERROR_EXPECTED;
		}

		b = WriteFile(hFile, bitmapFileHeader, sizeof(BITMAPFILEHEADER), &written, NULL);
		b = WriteFile(hFile, hDIB, bitmapFileHeader->bfSize, &written, NULL);

		FlushFileBuffers(hFile);
		CloseHandle(hFile);

		delete bitmapFileHeader;
		bitmapFileHeader = NULL;
	}

	ReleaseDC(NULL, hDC);
	GlobalFree(hDIB);
	DeleteObject(hPalette);
	DeleteObject(hCompatibleDC);
	DeleteObject(hMem);
	DeleteObject(hBitmap);
	DeleteObject(lpBitmapInfoHeader);

	hDC = NULL; hBitmap = NULL; hPalette = NULL; _bitmap = { 0 }; hDIB = NULL; hCompatibleDC = NULL; hMem = NULL; lpBitmapInfoHeader = NULL;

	return WINCAPTURE_SUCCESS;
}
