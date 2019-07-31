#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <windowsx.h>

wchar_t ttext[10];
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	const wchar_t CLASS_NAME[]  = L"Sample Window Class";

	WNDCLASS wc = { };

	wc.lpfnWndProc   = WindowProc;
	wc.hInstance     = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClass(&wc);

	// Create the window.

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Learn to Program Windows",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

										// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

HWND medit,cedit,finish;
#define MEDIT 101
#define CEDIT 102
#define FINISH 103
int gx,gy,i=0;
RECT rect;
BOOL BUTTON_DOWN=FALSE,DRAWN=FALSE;
struct t
{
	float m,c;
	RECT ret;
	t* next;
} *rtemp,*rstart;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CREATE:
		rstart=NULL;
		medit=CreateWindow(L"edit",NULL,WS_CHILD|WS_BORDER,0,0,50,20,hwnd,(HMENU)MEDIT,NULL,NULL);
		cedit=CreateWindow(L"edit",NULL,WS_CHILD|WS_BORDER,0,0,50,20,hwnd,(HMENU)CEDIT,NULL,NULL);
		finish=CreateWindow(L"button",L"Draw",WS_CHILD|WS_BORDER,0,0,90,20,hwnd,(HMENU)FINISH,NULL,NULL);
		break;

	case WM_SIZE:
	{
		RECT r;
		GetClientRect(hwnd, &r);
		if (DRAWN == TRUE){
			rect.left=(float)rect.left/gx*r.right;
			rect.right=(float)rect.right/gx*r.right;
			rect.top=(float)rect.top/gy*r.bottom;
			rect.bottom=(float)rect.bottom/gy*r.bottom;
		}
		t* rtt=rstart;
		while (rtt != NULL)
		{
			rtt->ret.left=(rtt->ret.left/static_cast <float>(gx))*r.right;
			rtt->ret.right=rtt->ret.right/static_cast <float>(gx)*r.right;
			rtt->ret.top=rtt->ret.top/static_cast <float>(gy)*r.bottom;
			rtt->ret.bottom=rtt->ret.bottom/static_cast <float>(gy)*r.bottom;
			rtt=rtt->next;
		}
		gx = r.right;
		gy = r.bottom;
		SetWindowPos(medit, NULL, gx / 4 - 75, gy - 50, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		SetWindowPos(cedit, NULL, 2 * gx / 4 - 25, gy - 50, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		SetWindowPos(finish, NULL, 3 * gx / 4 + 25, gy - 50, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		InvalidateRect(hwnd, &r, FALSE);
		break;
	}

	case WM_LBUTTONDOWN:
		if (GET_X_LPARAM(lParam) > 10 && GET_X_LPARAM(lParam) < gx - 10 && GET_Y_LPARAM(lParam) > 10 && GET_Y_LPARAM(lParam) < gy - 80){
			rect.left = GET_X_LPARAM(lParam); rect.top = GET_Y_LPARAM(lParam);
			rect.right = GET_X_LPARAM(lParam); rect.bottom = GET_Y_LPARAM(lParam); DRAWN = TRUE; BUTTON_DOWN=TRUE;
		}
		break;

	/*case WM_MOUSEMOVE:
	{
		if(BUTTON_DOWN==TRUE) InvalidateRect(hwnd,&rect,FALSE);
		break;
	}*/

	case WM_LBUTTONUP:
		if (BUTTON_DOWN==TRUE&&GET_X_LPARAM(lParam) > 10 && GET_X_LPARAM(lParam) < gx - 10 && GET_Y_LPARAM(lParam) > 10 && GET_Y_LPARAM(lParam) < gy - 80){
			rect.right = GET_X_LPARAM(lParam); rect.bottom = GET_Y_LPARAM(lParam);
			InvalidateRect(hwnd, NULL, FALSE);
			BUTTON_DOWN=FALSE;
		}
		break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		    case FINISH:
			{
				rtemp = new t;
				if (rstart == NULL){ rstart = rtemp; rtemp->next = NULL; }
				else{ rtemp->next = rstart; rstart = rtemp; }
				int n = GetWindowTextLength(medit) + 1;
				GetWindowText(medit, (LPWSTR)ttext, n);
				rtemp->m = _wtof((LPWSTR)ttext);
				n = GetWindowTextLength(cedit) + 1;
				GetWindowText(cedit, (LPWSTR)ttext, n);
				rtemp->c = _wtof((LPWSTR)ttext);
				int l = 0;
				if ((float)(5 - rtemp->c) / rtemp->m >= -5 && (float)(5 - rtemp->c) / rtemp->m <= 5){
					if (l == 0){ rtemp->ret.left = gx / 2 + (float)(5 - rtemp->c) / rtemp->m*(gx - 20) / 10; rtemp->ret.top = 10; l++; }
					else{ rtemp->ret.right = gx / 2 + (float)(5 - rtemp->c) / rtemp->m*(gx - 20) / 10; rtemp->ret.bottom = 10; }
				}
				if ((float)(-5 - rtemp->c) / rtemp->m >= -5 && (float)(-5 - rtemp->c) / rtemp->m <= 5){
					if (l == 0){ rtemp->ret.left = gx / 2 + (float)(-5 - rtemp->c) / rtemp->m*(gx - 20) / 10; rtemp->ret.top = gy - 80; l++; }
					else{ rtemp->ret.right = gx / 2 + (float)(-5 - rtemp->c) / rtemp->m*(gx - 20) / 10; rtemp->ret.bottom = gy - 80; }
				}
				if (rtemp->m*-5 + rtemp->c > -5 && rtemp->m*-5 + rtemp->c < 5){
					if (l == 0){ rtemp->ret.left = 10; rtemp->ret.top = 10 + (float)(gy - 90) / 2 - (rtemp->m*-5 + rtemp->c) / 10 * (gy - 90); l++; }
					else{ rtemp->ret.right = 10; rtemp->ret.bottom = 10 + (float)(gy - 90) / 2 - (rtemp->m*-5 + rtemp->c) / 10 * (gy - 90); }
				}
				if (rtemp->m * 5 + rtemp->c > -5 && rtemp->m * 5 + rtemp->c < 5){
					if (l == 0){ rtemp->ret.left = gx - 10; rtemp->ret.top = 10 + (float)(gy - 90) / 2 - (rtemp->m * 5 + rtemp->c) / 10 * (gy - 90); l++; }
					else{ rtemp->ret.right = gx - 10; rtemp->ret.bottom = 10 + (float)(gy - 90) / 2 - (rtemp->m * 5 + rtemp->c) / 10 * (gy - 90); }
				}

				RECT r;
				GetClientRect(hwnd, &r);
				InvalidateRect(hwnd, &r, FALSE);
			}
			break;
		}
		break;
	}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		HBRUSH h;
		HPEN p,pold;
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		h = CreateSolidBrush(RGB(200, 200, 255));
		RECT r = {10,10,gx - 10,gy - 80};
		FillRect(hdc, &r, h);
		p = CreatePen(PS_SOLID, 1, RGB(250, 250, 250));
		pold = (HPEN)SelectObject(hdc, p);
		int w = gy - 90;
		for (int i = 0; i < 11; i++)
		{
			MoveToEx(hdc, 10, 10 + i*w / 10, NULL);
			LineTo(hdc, gx - 10, 10 + i*w / 10);
		}
		w = gx - 20;
		for (int i = 0; i < 11; i++)
		{
			MoveToEx(hdc, 10 + i*w / 10, 10, NULL);
			LineTo(hdc, 10 + i*w / 10, gy - 80);
		}
		SelectObject(hdc, pold);
		DeleteObject(p);
		MoveToEx(hdc, gx / 2, 11, NULL);
		LineTo(hdc, gx / 2, gy - 80);
		MoveToEx(hdc, 11, 10 + (gy - 90) / 2, NULL);
		LineTo(hdc, gx - 10, 10 + (gy - 90) / 2);
		TextOut(hdc, gx / 4 - 140, gy - 50, L"Enter m", 7);
		TextOut(hdc, 2 * gx / 4 - 80, gy - 50, L"Enter c", 7);
		p=CreatePen(PS_SOLID,1,RGB(255,100,100));
		SelectObject(hdc,p);
		MoveToEx(hdc,rect.left,rect.top,NULL);
		LineTo(hdc,rect.left,rect.bottom);
		LineTo(hdc,rect.right,rect.bottom);
		LineTo(hdc,rect.right,rect.top);
		LineTo(hdc,rect.left,rect.top);

		t* rtt=rstart;
		while (rtt != NULL)
		{
			p=CreatePen(PS_SOLID,1,RGB(100,100,255));
			SelectObject(hdc,p);
			MoveToEx(hdc,rtt->ret.left,rtt->ret.top,NULL);
			LineTo(hdc,rtt->ret.right,rtt->ret.bottom);
			p=CreatePen(PS_SOLID,1,RGB(0,0,0));
			SelectObject(hdc,p);
			int l; float y,x,bx,by,bxx,byy;
			bx=(rtt->ret.left-static_cast <float>(gx)/2)/(gx-20)*10;
			by=(10+(static_cast <float>(gy)-90)/2-rtt->ret.top)/(gy-90)*10;
			bxx=(rtt->ret.right-static_cast <float>(gx)/2)/(gx-20)*10;
			byy=(10+(static_cast <float>(gy)-90)/2-rtt->ret.bottom)/(gy-90)*10;
			y=by;
			x=(y-static_cast <float>(rtt->c))/rtt->m;
			if(x>=bx&&x<=bxx)
			{
				TextOut(hdc,100,100,L"Hello",5);
				wchar_t text[20];
				char t[10],ty[10];
				_gcvt(x,5,t);
				_gcvt(y,5,ty);
				l=strlen(t); t[l]=','; strcat(t,ty);
			    mbstowcs(text,t,strlen(t));
				TextOut(hdc,bx-10,rtt->ret.top-5,text,wcslen(text));
			}

			rtt=rtt->next;
		}

		EndPaint(hwnd, &ps);
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}