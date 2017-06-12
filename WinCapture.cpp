#include "WinCapture.h"
#include <time.h>

//
// ��ʼ���б��캯��
//
WinCapture::WinCapture() : m_bModeFirstTime(true),
	m_IsCapturing(WINCAPTURE_FALSE),
	m_WinList(new WINCAPTURE_WINLIST),
	m_CaptureSetting(new WINCAPTURE_SETTING),
	m_CaptureMode(WINCAPTURE_MODE_FULLSCREEN),
	m_FrameData(new WINCAPTURE_FRAMEDATA)
{
}

//
// ��������
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
// ���ûص�
//
WResult WinCapture::SetCaptureCallback(WINCAPTURE_FRAMEDATA* pwcFrameData)
{
	pwcFrameData = m_FrameData;
	return WINCAPTURE_SUCCESS;
}

//
// ��ʼ����
//
WResult WinCapture::StartCapture()
{
	// �������ģʽ��0(��ȫ������ģʽ)
	if (!m_CaptureMode) {
		m_CaptureSetting->TargetRect.left = 0;
		m_CaptureSetting->TargetRect.right = GetSystemMetrics(SM_CXSCREEN);
		m_CaptureSetting->TargetRect.top = 0;
		m_CaptureSetting->TargetRect.bottom = GetSystemMetrics(SM_CYSCREEN);
	}

	// �����HWND��ʾģʽ, ���Ǵ��ڷ����˱仯
	if (WINCAPTURE_MODE_WINID & m_CaptureMode) {
		if (!IsWindowVisible(m_CaptureSetting->WinID)) {
			// �����Ƿ��ٴ򿪴���, ����ʾ����
			OutputDebugString("�����ѹر�\n");
			m_CaptureMode |= WINCAPTURE_MODE_CLOSED;
		}
		else if (IsIconic(m_CaptureSetting->WinID)) {
			// ��ʱ��ʾ����, �ȴ��ڴ򿪺�����ʾ����
			OutputDebugString("���ڴ�����С��\n");
			m_CaptureMode |= WINCAPTURE_MODE_ICONIC;
		}
		else {
			OutputDebugString("���ڴ�������״̬\n");
			if (m_CaptureMode & WINCAPTURE_MODE_ICONIC) {
				// ���ڴ���С���ָ�
				m_CaptureMode &= ~WINCAPTURE_MODE_ICONIC;
				OutputDebugString("���ڴ���С���ָ�\n");
			}
		}
	}

	// ��ʾ��ͼģʽ
	if (m_CaptureMode & WINCAPTURE_MODE_FULLSCREEN)
		OutputDebugString("ȫ��\t");
	if (m_CaptureMode & WINCAPTURE_MODE_WINID)
		OutputDebugString("HWND\t");
	if (m_CaptureMode & WINCAPTURE_MODE_RECT)
		OutputDebugString("RECT\t");
	if (m_CaptureMode & WINCAPTURE_MODE_XYWH)
		OutputDebugString("XYWH\t");	
	if (m_CaptureMode & WINCAPTURE_MODE_ICONIC)
		OutputDebugString("(������С��)\t");
	if (m_CaptureMode & WINCAPTURE_MODE_CLOSED)
		OutputDebugString("(���ڹر�)\t");
	OutputDebugString("\n");

	// ����״̬Ϊ���ڲ���
	m_IsCapturing = WINCAPTURE_TURE;

	// ��ʼ��ͼ����
	if (_GetSnapShotByRect(m_CaptureSetting->TargetRect)) {	// ��ͼʧ��
		OutputDebugString("-----��ͼʧ��\n");
		return WINCAPTURE_ERROR_EXPECTED;
	}
	else  {																						// ��ͼ�ɹ�
		OutputDebugString("+++++��ͼ�ɹ�\n");
		return WINCAPTURE_SUCCESS;
	}
}

//
// ֹͣ����
//
WResult WinCapture::StopCapture()
{
	// ����״̬Ϊֹͣ����
	m_IsCapturing = WINCAPTURE_FALSE;

	return WINCAPTURE_SUCCESS;
}

//
// �Ƿ����ڲ���
//
WINCAPTURE_ISCAPTURING WinCapture::IsCapturing()
{
	return m_IsCapturing;
}

//
// ����FPS
//
WResult WinCapture::SetFPS(unsigned int uFPS = 15)
{
	if (uFPS > WINCAPTURE_MAX_FPS)
		return WINCAPTURE_ERROR_EXPECTED;
	m_CaptureSetting->FPS = uFPS;
	return WINCAPTURE_SUCCESS;
}

//
// ��ȡ�����б�
//
std::vector<std::string> WindowTitle;
/// --- WinCapture::GetWindowList() ������:
/// --- ö�ٴ���
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
// -- WinCapture::GetWindowList() ����
WResult WinCapture::GetWindowList()
{
	this->FreeWindowList();
	WindowTitle.clear();

	OutputDebugString("----------------------��ʼ��ʾ�����б�-------------------\n");

	EnumWindows(EnumWindowsProc, NULL);

	for (UINT i = 0; i < WindowTitle.size(); ++i) {
			m_WinList->Title.push_back(WindowTitle[i]);
			if (WindowTitle[i][0]) {
				OutputDebugString(WindowTitle[i].c_str());
				OutputDebugString("\n");
			}
	}

	OutputDebugString("-----------------------�����б���ʾ���--------------------\n");
	return WINCAPTURE_SUCCESS;
}

//
// �ͷŴ����б�
//
void WinCapture::FreeWindowList()
{
	if (m_WinList) {
		delete m_WinList;
		m_WinList = new WINCAPTURE_WINLIST;
	}
}

//
// ����WinID(WinTitle)���ò���Ŀ��
// 
WResult WinCapture::SetCaptureTarget(std::string WinText)
{
	// ��������Ϊ��һ�β���
	m_bModeFirstTime = true;

	// ��ȡ�����б�
	// if (!m_WinList->Title.size())
		this->GetWindowList();

	// Ѱ��ָ������
	bool findFlag = false;
	unsigned int index = 0;
	for (; index < m_WinList->Title.size() && !findFlag; index++) {
		if (m_WinList->Title[index] == WinText)
			findFlag = true;
	}

	// δ�ҵ�����ID
	if (!findFlag) {
		OutputDebugString("���ڲ�����\n");
		return WINCAPTURE_ERROR_EXPECTED;
	}

	// ��ȡWinID��Ӧ�ľ��
	HWND wantedWindow = FindWindow(NULL, (m_WinList->Title[index - 1]).data());

	// ���ݾ�����þ��η�Χ
	if (!GetWindowRect(wantedWindow, (LPRECT)&(m_CaptureSetting->TargetRect)))
	{
		OutputDebugString("����δ�ҵ�\n");
		return WINCAPTURE_ERROR_EXPECTED;
	} 
	else {
		m_CaptureSetting->WinID = wantedWindow;
		if (IsIconic(wantedWindow)) {		// ���ڴ�����С��
			m_CaptureMode = WINCAPTURE_MODE_ICONIC | WINCAPTURE_MODE_WINID;
			OutputDebugString("���ڴ�����С��\n");
			return WINCAPTURE_ERROR_EXPECTED;
		}
		else {
			// ���ڳ�����Ļ��ʾ��Χ������ֵ��������
			if (m_CaptureSetting->TargetRect.left < 0) m_CaptureSetting->TargetRect.left = 0;
			if (m_CaptureSetting->TargetRect.top < 0) m_CaptureSetting->TargetRect.top = 0;
			if (m_CaptureSetting->TargetRect.right > GetSystemMetrics(SM_CXSCREEN)) m_CaptureSetting->TargetRect.right = GetSystemMetrics(SM_CXSCREEN);
			if (m_CaptureSetting->TargetRect.bottom > GetSystemMetrics(SM_CYSCREEN)) m_CaptureSetting->TargetRect.bottom = GetSystemMetrics(SM_CYSCREEN);

			// �жϴ����ܷ�HWND��ʽ�ػ�
			m_CaptureMode = WINCAPTURE_MODE_WINID;
			OutputDebugString("�ɹ��ҵ�����\n");
			return WINCAPTURE_SUCCESS;
		}
	}
}

//
// ���ݾ���RECT���ò���Χ
//
WResult WinCapture::SetCaptureTarget(RECT rt, bool bFollowupCursor, POINT ptAnchor)
{
	if (rt.left < rt.right && rt.bottom > rt.top
		&& rt.left >= 0 && rt.right <= GetSystemMetrics(SM_CXSCREEN)
		&& rt.top >= 0 && rt.bottom <= GetSystemMetrics(SM_CYSCREEN)) {
		m_CaptureSetting->TargetRect = rt;
		m_CaptureSetting->IsFollowCursor = bFollowupCursor;
		m_CaptureSetting->Anchor = ptAnchor;

		m_CaptureMode = WINCAPTURE_MODE_RECT;
		OutputDebugString("���η�Χ���óɹ�\n");
		return WINCAPTURE_SUCCESS;
	}
	else {
		OutputDebugString("���η�Χ���Ϸ�\n");
		return WINCAPTURE_ERROR_EXPECTED;
	}
}

//
// ����x, y, width, height���ò���Χ
//
WResult WinCapture::SetCaptureTarget(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned height, const bool bFollowupCursor, POINT ptAnchor)
{
	if (x <= (UINT)GetSystemMetrics(SM_CXSCREEN) && x >= 0
		&& x + width <= (UINT)GetSystemMetrics(SM_CXSCREEN)
		&& y <= (UINT)GetSystemMetrics(SM_CYSCREEN) && y >= 0
		&& y + height <= (UINT)GetSystemMetrics(SM_CYSCREEN) ){
		m_CaptureSetting->TargetRect.left = x;
		m_CaptureSetting->TargetRect.top = y;
		m_CaptureSetting->TargetRect.right = x + width;
		m_CaptureSetting->TargetRect.bottom = y + height;

		m_CaptureSetting->IsFollowCursor = bFollowupCursor;

		m_CaptureSetting->Anchor = ptAnchor;

		m_CaptureMode = WINCAPTURE_MODE_XYWH;
		OutputDebugString("XYWH���óɹ�\n");
		return WINCAPTURE_SUCCESS;
	}
	else {
		OutputDebugString("�������λ�����ò��Ϸ�\n");
		return WINCAPTURE_ERROR_EXPECTED;
	}
}

//
// �Ƿ���ʾ���
//
void WinCapture::EnableCursorDisplay(bool bDisplay)
{
	m_CaptureSetting->IsDisplay = bDisplay;
}

// ------------------------- Callback ----------------------------

//
// �������
//
void WinCapture::OnFinishedOneFrame(WINCAPTURE_FRAMEDATA* frameData)
{
	frameData->pData = m_FrameData->pData;
	frameData->BytesPerLine = m_FrameData->BytesPerLine;
	frameData->CursorPos = m_FrameData->CursorPos;
	frameData->uSize = m_FrameData->uSize;
}

void WinCapture::OnCapturedFrameAvailable(WINCAPTURE_FRAMEDATA* userFrameData, UINT64 uTimeStamp, POINT* ptMouse)
{
	userFrameData->pData = m_FrameData->pData;
	userFrameData->BytesPerLine = m_FrameData->BytesPerLine;
	userFrameData->CursorPos = m_FrameData->CursorPos;
	userFrameData->uSize = m_FrameData->uSize;

	uTimeStamp = GetTickCount64() + m_CaptureSetting->OffsetTimeStamp;

	ptMouse = m_FrameData->CursorPos;
}


/*****************************************************************************************/
/********************************* private ���� *****************************************/
/****************************************************************************************/

//
// ���ݾ��η�Χ��ȡ��ͼ
//
WResult WinCapture::_GetSnapShotByRect(RECT targetRECT)
{
	HWND testWin = (HWND)m_CaptureSetting->WinID;

	// ���ݵ�ǰģʽѡ��HDCΪ��Ļ����ָ������
	HDC hDC = ((m_CaptureMode & WINCAPTURE_MODE_WINID) && !(m_CaptureMode & WINCAPTURE_MODE_RECT)) ? GetWindowDC(testWin) : GetWindowDC(NULL);

	// �������, �������ΪWINID��ʽ, ��Ҫ�޸�
	UINT width = targetRECT.right - targetRECT.left;
	UINT height = (targetRECT.bottom - targetRECT.top);
	if (m_CaptureMode & WINCAPTURE_MODE_WINID) {
		RECT fullRect = { 0 };
		GetWindowRect(testWin, &fullRect);
		if (!IsIconic(testWin)) {
			width = fullRect.right - fullRect.left;
			height = fullRect.bottom - fullRect.top;
		}
		if (width != targetRECT.right - targetRECT.left && !(m_CaptureMode & WINCAPTURE_MODE_ICONIC)) {
			OutputDebugString("���ڳߴ緢���仯\n");
			m_CaptureSetting->TargetRect = fullRect;
		}
	}

	// �����Ӱ��Χ
	UINT shadow = 10;
	if (m_CaptureMode & WINCAPTURE_MODE_WINID ) {
		width = width - shadow - shadow;
		height -= shadow;
	}

	// ����DC��������
	HDC hCompatibleDC = CreateCompatibleDC(hDC);

	// �½�����DC��Ӧ��λͼ���
	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, width, height);

	// ��λͼ����DC������
	SelectObject(hCompatibleDC, hBitmap);

	clock_t t1, t2;	// just for test
	t1 = clock();		// just for test
	// ʹ��BitBlt
	if ((m_CaptureMode & WINCAPTURE_MODE_WINID) && !(m_CaptureMode & WINCAPTURE_MODE_RECT)) {
		BitBlt(hCompatibleDC, 0, 0, width, height, hDC, 0 + shadow, 0, SRCCOPY | CAPTUREBLT);
	}
	else if ((m_CaptureMode & WINCAPTURE_MODE_WINID) && (m_CaptureMode & WINCAPTURE_MODE_RECT)) {
		BitBlt(hCompatibleDC, 0, 0, width, height, hDC, m_CaptureSetting->TargetRect.left + shadow, m_CaptureSetting->TargetRect.top + shadow, SRCCOPY);
	}
	else {
		BitBlt(hCompatibleDC, 0, 0, width, height, hDC, targetRECT.left, targetRECT.top, SRCCOPY);
	}
	t2 = clock();		// just for test

	// ��ȡ��ɫ��
	HPALETTE hPalette = (HPALETTE)GetStockObject(DEFAULT_PALETTE);

	// bitmapʵ��������
	BITMAP _bitmap;
	GetObject(hBitmap, sizeof(_bitmap), (LPVOID)&_bitmap);

	// bitmap��Ϣͷ
	BITMAPINFOHEADER bitmapInfoHeader;
	ZeroMemory(&bitmapInfoHeader, sizeof(BITMAPCOREHEADER));
	bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader.biWidth = _bitmap.bmWidth;
	bitmapInfoHeader.biHeight = _bitmap.bmHeight;
	bitmapInfoHeader.biPlanes = 1;
	bitmapInfoHeader.biBitCount = 24;
	bitmapInfoHeader.biCompression = BI_RGB;

	// ������Ϣͷ����ɫ���С
	int nColors = (1 << bitmapInfoHeader.biBitCount);
	if (nColors > 256)
		nColors = 0;

	DWORD dwLength = bitmapInfoHeader.biSize + nColors * sizeof(RGBQUAD);

	// ��ʱDC
	HDC hTmpDC = GetDC(NULL);
	hPalette = SelectPalette(hTmpDC, hPalette, FALSE);
	RealizePalette(hTmpDC);

	// Ϊ��Ϣͷ����ɫ������ڴ�
	HANDLE hDIB = GlobalAlloc(GMEM_FIXED, dwLength);
	if (!hDIB) {
		return WINCAPTURE_ERROR_EXPECTED;
	}

	// ��ȡλͼ����
	LPBITMAPINFOHEADER lpBitmapInfoHeader = (LPBITMAPINFOHEADER)hDIB;
	*lpBitmapInfoHeader = bitmapInfoHeader;

	// ����GetBIDBITS
	BOOL b = GetDIBits(hTmpDC, hBitmap, 0L, (DWORD)bitmapInfoHeader.biHeight, (LPBYTE)NULL, (LPBITMAPINFO)lpBitmapInfoHeader, (DWORD)DIB_RGB_COLORS);

	bitmapInfoHeader = *lpBitmapInfoHeader;

	// ͼ���ÿ�ж�����32�ֽ�
	if (bitmapInfoHeader.biSizeImage == 0) {
		bitmapInfoHeader.biSizeImage = ((((bitmapInfoHeader.biWidth * bitmapInfoHeader.biBitCount) + 31) & ~31) / 8) *bitmapInfoHeader.biHeight;

		if (bitmapInfoHeader.biCompression != BI_RGB)
			bitmapInfoHeader.biSizeImage = (bitmapInfoHeader.biSizeImage * 3) / 2;
	}

	// ���·����ڴ��С, ������������ڴ�����С��ʱ, ֱ����biSizeImage = 0
	if (IsIconic(testWin)) {
		/*bitmapInfoHeader.biSizeImage = 0;*/
	}

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
	
	// ����m_FrameData->Data�ڴ�
	if (m_FrameData->uSize != 0) 
		delete m_FrameData->pData;
	(m_FrameData->pData) = new BYTE[dwLength];
	memcpy(m_FrameData->pData, reinterpret_cast<void*>(hMem), dwLength);
	m_FrameData->uSize = dwLength;

	b = GetDIBits(hTmpDC, hBitmap, 0L, (DWORD)bitmapInfoHeader.biHeight,
		(LPBYTE)lpBitmapInfoHeader + (bitmapInfoHeader.biSize + nColors * sizeof(RGBQUAD)),
		(LPBITMAPINFO)lpBitmapInfoHeader, (DWORD)DIB_RGB_COLORS);

	// ����ǵ�һ�β���, ����hMemȫ��, ���ҵ�ǰ�����Ƿ���С���ǹر�״̬, ˵��BitBlt�޷���ȡ��ǰ���ʹ��ڵĻ�, ��Ҫʹ�ò���ȫ���ķ�ʽ��ͼ
	if (m_bModeFirstTime && (m_CaptureMode & WINCAPTURE_MODE_WINID) && (m_CaptureMode < WINCAPTURE_MODE_ICONIC)) {
		m_bModeFirstTime = false;		// �Ѿ����ǵ�һ�β�����
		BYTE* p = reinterpret_cast<BYTE*>(hMem);
		bool bChangeFlag = true;
		for (int i = lpBitmapInfoHeader->biSize; i < dwLength; i++) {
			if (*(p + i) != 0 && *(p+i) != 255) {
				bChangeFlag = false;
				break;
			}
		}
		if (bChangeFlag) {
			m_CaptureMode = WINCAPTURE_MODE_RECT | WINCAPTURE_MODE_WINID;
			OutputDebugString("���Ĳ���ģʽΪRECT\n");
			return WINCAPTURE_ERROR_EXPECTED;
		}
	}

	// m_CaptureSetting->IsDisplay = true;
	// �Ƿ���Ҫ�������
	if (m_CaptureSetting->IsDisplay) {
		// ��ȡ���λ��
		LPPOINT cursorPos = new POINT;
		RtlZeroMemory(cursorPos, sizeof(POINT));
		GetCursorPos(cursorPos);

		// �߽紦��
		if (cursorPos->x < targetRECT.left)		cursorPos->x = targetRECT.left;
		if (cursorPos->y < targetRECT.top)		cursorPos->y = targetRECT.top;
		if (cursorPos->x > targetRECT.right)		cursorPos->x = targetRECT.right;
		if (cursorPos->y > targetRECT.bottom - 30)		cursorPos->y = targetRECT.bottom - 30;


		m_FrameData->CursorPos = cursorPos;

		// �������
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

	// �ļ�����
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
