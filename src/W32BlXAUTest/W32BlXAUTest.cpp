// File: W32B1MngTest.cpp 
// Littleflute's Windows32 App
/*
2017.05.03 11:47pm bjt 
littleflute

../Bin/Chk/W32BlXAUTest.exe
*/

#include "windows.h"
#include "xdBmp.h"
#include "BlConsole.h"
#include "commctrl.h"

#include "../Common/inc/BlAPI.h"
class CMyDll  
	{
	public:
		CMyDll(){
			m_hModule = LoadLibrary("B1/BlXAU.dll");
			if(NULL== m_hModule)
			{
				::MessageBox(NULL,"can't find BlXAU.dll!","xd dbg",IDOK);
			}
		}
		~CMyDll(){
			if(m_hModule)
			{
				FreeLibrary (m_hModule);
				m_hModule = NULL;
			}
		} 
		BLSTATUS OnBlCallLibFun(HWND h,UINT m,WPARAM w,LPARAM l)
		{
			BLSTATUS r = prCallLibFun(h,m,w,l);
			return r;
		}
	private: 
		HMODULE		m_hModule;

		
		
		BLSTATUS pvCallLibFun(HWND h,char *szFun,UINT m,WPARAM w,LPARAM l)
		{
			BLSTATUS r = BL_STATUS_NOT_SUPPORT_INTERFACE; 
			
			if(NULL != m_hModule)
			{
				FARPROC pFunc = GetProcAddress(m_hModule, szFun);
				if (pFunc ) {
					r = (* (OnBLAPI *)pFunc)(h,m,w,l);   
				}
			}
			else
			{
				r = BL_STATUS_NOT_FOUND_LIBARY;
			}
			return r;
		}
		BLSTATUS prCallLibFun(HWND h,UINT m,WPARAM w,LPARAM l)
		{
			BLSTATUS r = BL_STATUS_NOT_SUPPORT_INTERFACE; 
			r = pvCallLibFun(h,"OnBlApi",m,w,l);
			return r;
		} 
	};

class CXdDllTest
{
public:
	CXdDllTest::CXdDllTest():m_nTimes(0){
		CBlConsole c;
		c.InitConsole();
		strcpy(m_szDev,"dev:");
		strcpy(m_szText,"Text:");
		printf("CXdDllTest::CXdDllTest()\n");
	}
	CXdDllTest::~CXdDllTest(){}
	void CXdDllTest::onLBtnDown(HWND hWnd){
		m_nTimes++;
		::InvalidateRect(hWnd,NULL,NULL);
	}
	void CXdDllTest::onHotKey(HWND hWnd,WPARAM w,LPARAM l){
		if(w==444)
		{ 
			printf("hot key: 444\n");

	//		::SetWindowPos(hWnd,NULL,50,50,300,300,SWP_SHOWWINDOW);
			::ShowWindow(hWnd,SW_RESTORE); 
			::SetFocus(hWnd);

		}

	}
	void CXdDllTest::dataIsComing(HWND hWnd,WPARAM w,LPARAM l){
		HDC hDC = ::GetDC(hWnd);
			int n = (int)l;
			BYTE* pByte = (BYTE*)w;
		 	RECT rect;
			rect.left	= 560;
			rect.top	= 110;
			rect.right	= rect.left + 300;
			rect.bottom = rect.top + 300;
			::Rectangle(hDC,rect.left,rect.top,rect.right,rect.bottom);
			CXdBmp xdbmp;
			xdbmp.drawByByteData(hDC,pByte,n,&rect);
		
			
		char s[100];
		sprintf(s,"n=%d",n);
		TextOut(hDC,100,450,s,strlen(s));
		::ReleaseDC(hWnd,hDC);
	}

	HANDLE CXdDllTest::init(HWND hWnd){
		HANDLE h = NULL;
		BL_LPARAM_CONTEXT c;
		strcpy(c.szVerify,"BEAUTIFULLOVER");
		
		BLSTATUS st = m_MyDll.OnBlCallLibFun(hWnd,BL_WM_INIT,NULL,(LPARAM)&c);
		if(BL_STATUS_OK != st)
		{
			if(BL_STATUS_PARAMETER_INVALID==st)
			{
				::MessageBox(NULL,"BL_WM_INIT error:\nBL_STATUS_PARAMETER_INVALID","xd dbg",IDOK);
			}
			else if(BL_STATUS_NOT_SUPPORT_INTERFACE==st)
			{
				printf("Bl_WM_INIT: st=%x\n",st);
				::MessageBox(NULL,"BL_WM_INIT error:\n BL_STATUS_NOT_SUPPORT_INTERFACE!","xd dbg",IDOK);
			}
			else
			{
				printf("Bl_WM_INIT: st=%x\n",st);
				::MessageBox(NULL,"BL_WM_INIT error:\n unknow!","xd dbg",IDOK);
			}
		}
		else
		{
			printf("Bl_WM_INIT: st=%x\n",st);
			::MessageBox(NULL,"BL_WM_INIT OK!","xd dbg",IDOK);
	
		}
		 
		return h;
	}
	void CXdDllTest::draw(HWND hWnd,HDC hDC)
	{ 
		char s[100];
		strcpy(s,"CXdDllTest: ");
		TextOut(hDC,100,100,s,strlen(s));
		strcpy(s,m_szDev);
		TextOut(hDC,100,130,s,strlen(s));
		
		strcpy(s,m_szText);
		TextOut(hDC,100,150,s,strlen(s));

		sprintf(s,"m_nTimes=%d",m_nTimes);
		TextOut(hDC,100,250,s,strlen(s));
	}
private:
	CMyDll	m_MyDll; 
	
	char	m_szText[1000];
	char	m_szDev[296];  
	int		m_nTimes;

};


class CXdApp
{
#pragma warning( disable : 4183 )
public:
	CXdApp::CXdApp(){
		strcpy(m_szTitle,getV());
		strcpy(m_szWindowClass,"XdW32"); 
	};
	CXdApp::~CXdApp(){};
	char* CXdApp::getV(){ return "CXdApp: V0.0.1";};
	int	CXdApp::go(HINSTANCE hInstance,
                     int       nCmdShow)
	{
		if (!InitInstance (hInstance, nCmdShow)) 
		{ 
			return FALSE; 
		}

		MSG msg;
		HACCEL hAccelTable = NULL;   
		
		while (GetMessage(&msg, NULL, 0, 0)) 
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		
		return msg.wParam;
		
	}
private:
	HINSTANCE	m_hInst; 
	TCHAR		m_szTitle[256]; 
	TCHAR		m_szWindowClass[256];
	ATOM CXdApp::MyRegisterClass(HINSTANCE hInstance)
	{
		WNDCLASSEX wcex;
		
		wcex.cbSize = sizeof(WNDCLASSEX); 
		
		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= (WNDPROC)WndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInstance; 
		wcex.hIcon			= LoadIcon(NULL,IDI_WINLOGO); 
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= NULL; 
		wcex.lpszClassName	= m_szWindowClass; 
		wcex.hIconSm		= LoadIcon(NULL,IDI_WINLOGO);
		
		return RegisterClassEx(&wcex);
	}
	BOOL CXdApp::InitInstance(HINSTANCE hInstance, int nCmdShow)
	{
		HWND hWnd;
		
		m_hInst = hInstance;  
		MyRegisterClass(m_hInst);
		hWnd = CreateWindow(m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
		
		if (!hWnd)
		{
			return FALSE;
		}
		
		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);
		
		return TRUE;
	}
	

	static	LRESULT CALLBACK	CXdApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		int			wmId, wmEvent;
		PAINTSTRUCT ps;
		HDC			hdc;
		TCHAR		szHello[256]; 
		strcpy(szHello,"Hello world!");
	    static	CXdDllTest	xdTest;
		
		switch (message) 
		{
		case WM_HOTKEY:
			xdTest.onHotKey(hWnd,wParam,lParam);
			break;
		case BL_WM_B0_0XB001: 
			//	pParent->m_viewDevList.plUser1(w,l);
//			printf("CXdDllTest:: BL_WM_B0_0XB001()\n");
			xdTest.dataIsComing(hWnd,wParam,lParam);
			break;
		case WM_CREATE:
			xdTest.init(hWnd);
			break;
			
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
			case 1:
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
			
			case WM_LBUTTONDOWN:
				//::MessageBox(NULL,"My Msg","My Title",MB_ICONINFORMATION|MB_YESNO);
				xdTest.onLBtnDown(hWnd);
				break;
			case WM_PAINT:
				hdc = BeginPaint(hWnd, &ps);
				// TODO: Add any drawing code here...
				RECT rt;
				GetClientRect(hWnd, &rt);
				DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
				xdTest.draw(hWnd,hdc);
				EndPaint(hWnd, &ps);
				break;
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
};
CXdApp g_myApp;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{ 
	return	g_myApp.go(hInstance,nCmdShow);
}