// Scramble.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Scramble.h"

#define MAX_LOADSTRING 100
#define NUM 300 

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	static int iKeep[NUM][4];
	HDC hdcScr, hdcMem;
	int cx, cy;
	HBITMAP hBitmap;
	HWND hwnd;
	int i, j, x1, y1, x2, y2;
	if (LockWindowUpdate(hwnd = GetDesktopWindow()))
	{
		hdcScr = GetDCEx(hwnd, NULL, DCX_CACHE |
			DCX_LOCKWINDOWUPDATE);
		hdcMem = CreateCompatibleDC(hdcScr);
		cx = GetSystemMetrics(SM_CXSCREEN) / 10;
		cy = GetSystemMetrics(SM_CYSCREEN) / 10;
		hBitmap = CreateCompatibleBitmap(hdcScr, cx, cy);

		SelectObject(hdcMem, hBitmap);
		srand((int)GetCurrentTime());

		for (i = 0; i < 2; i++)
			for (j = 0; j < NUM; j++)
			{
				if (i == 0)
				{
					iKeep[j][0] = x1 = cx * (rand() % 10);
					iKeep[j][1] = y1 = cy * (rand() % 10);
					iKeep[j][2] = x2 = cx * (rand() % 10);
					iKeep[j][3] = y2 = cy * (rand() % 10);
				}
				else
				{
					x1 = iKeep[NUM - 1 - j][0];
					y1 = iKeep[NUM - 1 - j][1];

					x2 = iKeep[NUM - 1 - j][2];
					y2 = iKeep[NUM - 1 - j][3];
				}
				BitBlt(hdcMem, 0, 0, cx, cy, hdcScr, x1, y1,
					SRCCOPY);
				BitBlt(hdcScr, x1, y1, cx, cy, hdcScr, x2, y2,
					SRCCOPY);
				BitBlt(hdcScr, x2, y2, cx, cy, hdcMem, 0, 0,
					SRCCOPY);

				Sleep(10);
			}

		DeleteDC(hdcMem);
		ReleaseDC(hwnd, hdcScr);
		DeleteObject(hBitmap);

		LockWindowUpdate(NULL);
	}
	return FALSE;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SCRAMBLE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SCRAMBLE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
