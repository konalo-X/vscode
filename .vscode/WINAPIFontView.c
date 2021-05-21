#include<windows.h>
#include<tchar.h>
#define IDC_LISTBOX 100
#define IDC_CHECKBOX 101
#define IDC_SAMPLE 102
#define IDC_PUSHBUTTON 103
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void filllistbox(HWND);
void onsel(HWND);
int CALLBACK EnumFontFamProc(ENUMLOGFONT *lpelf, NEWTEXTMETRIC *lpntm, DWORD nFontType, LPARAM lparam);
//int EnumFontFamilies(HDC dc, LPCTSTR lpszFamily, FONTENUMPROC*lpEnumFontFamProc, LPARAM lparam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{

	WNDCLASS wndclass;
	HWND hwnd;
	MSG msg;
	static TCHAR strClassName[] = _T("FontView");
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
	wndclass.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wndclass.hIcon = LoadIcon(hInstance, IDI_WINLOGO);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = strClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_VREDRAW | CS_HREDRAW ;


	RegisterClass(&wndclass);

	hwnd = CreateWindow(strClassName, _T("FontView"), WS_OVERLAPPED  | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, 0, 0, 0))
	{
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int m_cxChar;
	static int m_cyChar;
	TEXTMETRIC tm;
	HDC dc;
	static HWND m_hwndListBox;
	static HWND m_hwndLBTitle;
	static HWND m_hwndPushButton;
	static HWND m_hwndCheckBox;
	static HWND m_hwndGroupBox;
	static HWND m_hwndSampleText;
	PAINTSTRUCT ps;
	HFONT m_mainFont,m_fontSample;
	HFONT hOldFont;
	m_fontSample = NULL;
	switch (message)
	{
	case WM_CREATE:
		dc = GetDC(hWnd);
		
		GetTextMetrics(dc, &tm);
		//SelectObject(dc, hOldFont);
		ReleaseDC(hWnd, dc);
		m_cxChar = tm.tmAveCharWidth;
		m_cyChar = tm.tmHeight + tm.tmExternalLeading;
		MoveWindow(hWnd, 0, 0, m_cxChar*72, m_cyChar*30, FALSE);


		
		m_hwndLBTitle = CreateWindow(_T("static"), _T("Typefaces"), WS_CHILD | WS_VISIBLE | SS_LEFT,
			2 * m_cxChar, m_cyChar, 14 * m_cxChar, m_cyChar*5/4, hWnd, NULL, NULL, NULL);
		m_hwndListBox = CreateWindowEx(WS_EX_CLIENTEDGE,_T("listbox"), NULL, WS_VISIBLE | WS_CHILD | LBS_STANDARD, 
			2 * m_cxChar, m_cyChar * 9 / 4, 46 * m_cxChar, 15 * m_cyChar, hWnd, (HMENU)IDC_LISTBOX, NULL, NULL);

		m_hwndCheckBox = CreateWindow(_T("BUTTON"), _T("Show True Type fonts only"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 
			2 * m_cxChar, 17 * m_cyChar, 46 * m_cxChar,  2*m_cyChar, hWnd, (HMENU)IDC_CHECKBOX, NULL, NULL);
		m_hwndGroupBox = CreateWindow(_T("BUTTON"), _T("Sample"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
			2 * m_cxChar, 20 * m_cyChar, 64 * m_cxChar, 5 * m_cyChar,hWnd,NULL,NULL,NULL);
		m_hwndPushButton= CreateWindow(_T("BUTTON"), _T("Print Sample"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON| WS_DISABLED,
			49 * m_cxChar, m_cyChar * 9 / 4, 19 * m_cxChar, 9 * m_cyChar/4, hWnd, (HMENU)IDC_PUSHBUTTON, NULL, NULL);

		
		m_hwndSampleText = CreateWindow(_T("static"), _T(""), WS_VISIBLE | WS_CHILD | SS_CENTER, 6 * m_cxChar/2, 85 * m_cyChar/4, 62 * m_cxChar, 13 * m_cyChar/4,
			hWnd, (HMENU)IDC_SAMPLE, NULL, NULL);
		
		m_mainFont = CreateFont(13, 0, NULL, NULL,
			FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
			OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH
			, _T("MS Sans Serif"));

		SendMessage(m_hwndLBTitle,WM_SETFONT,(WPARAM)m_mainFont,(LPARAM)TRUE);
		SendMessage(m_hwndListBox,WM_SETFONT,(WPARAM)m_mainFont,(LPARAM)TRUE);
		SendMessage(m_hwndCheckBox,WM_SETFONT,(WPARAM)m_mainFont,(LPARAM)TRUE);
		SendMessage(m_hwndPushButton,WM_SETFONT,(WPARAM)m_mainFont,(LPARAM)TRUE);
		SendMessage(m_hwndGroupBox,WM_SETFONT,(WPARAM)m_mainFont,(LPARAM)TRUE);
		filllistbox(hWnd);
		//onsel(hWnd, m_cyChar);
		break;
	case WM_COMMAND:

		switch (HIWORD(wParam))
		{
		case LBN_SELCHANGE:
			EnableWindow(GetDlgItem(hWnd, IDC_PUSHBUTTON), TRUE);
			onsel(hWnd);
			
			//InvalidateRect(GetDlgItem(hWnd, IDC_SAMPLE),NULL,
			break;
		case BN_CLICKED:
			filllistbox(hWnd);
			onsel(hWnd);
			//if(HIWORD(wParam)==IDC_CHECKBOX)
			MessageBox(NULL,_T("This features has not implemented yet!"),_T("提示"),MB_OK|MB_ICONEXCLAMATION);
			break;
		
		}

		break;


	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


void filllistbox(HWND hwnd)
{
	
	HWND hwndListbox = GetDlgItem(hwnd, IDC_LISTBOX);
	int iCount = SendMessage(hwndListbox, LB_GETCOUNT, 0, 0);
	if (iCount)
	{
		SendMessage(hwndListbox, LB_RESETCONTENT, 0, 0);
	}
	HDC dc = GetDC(hwndListbox);
	EnumFontFamilies(dc, NULL, (FONTENUMPROC)EnumFontFamProc, (LPARAM)hwnd);
	ReleaseDC(hwndListbox, dc);
}
void onsel(HWND hwnd)
{
	HWND hwndlistbox = GetDlgItem(hwnd, IDC_LISTBOX);
	int nIndex = SendMessage(hwndlistbox, LB_GETCURSEL, 0, 0);
	if (nIndex != LB_ERR)
	{
		EnableWindow(GetDlgItem(hwnd, IDC_PUSHBUTTON), TRUE);

	}
	int length = SendMessage(hwndlistbox, LB_GETTEXTLEN, nIndex, 0);
	TCHAR strFaceName[100];
		SendMessage(hwndlistbox, LB_GETTEXT, nIndex, (LPARAM)strFaceName);
		

		HWND hwndSample = GetDlgItem(hwnd, IDC_SAMPLE);
		HDC dc = GetDC(hwndSample);
		int m_cyChar=HIWORD(GetDialogBaseUnits());
		HFONT m_fontSample = CreateFont(-m_cyChar * 2, 0, 0, 0, FW_NORMAL,
			0, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
			CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH |
			FF_DONTCARE, strFaceName);
		
		SetWindowText(hwndSample, _T("AaBbCcDdEeFfGg"));
		SendMessage(hwndSample,WM_SETFONT,(WPARAM)m_fontSample,(LPARAM)TRUE);
		ReleaseDC(hwndSample, dc);
		DeleteObject(m_fontSample);
		
}
int CALLBACK EnumFontFamProc(ENUMLOGFONT *lpelf, NEWTEXTMETRIC *lpntm, DWORD nFontType, LPARAM lparam)
{
	HWND hwnd=(HWND)lparam;
	int iCheck = (int)SendMessage(GetDlgItem(hwnd, IDC_CHECKBOX), BM_GETCHECK, 0, 0);
	if (iCheck || nFontType & TRUETYPE_FONTTYPE)
		SendMessage(GetDlgItem(hwnd, IDC_LISTBOX), LB_INSERTSTRING,(WPARAM)-1,(LPARAM)(lpelf->elfLogFont.lfFaceName));
	return 1;
}

