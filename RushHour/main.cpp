#include "main.h"

#pragma comment(lib, "winmm.lib")

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	SetCurrentDirectory(_T("../Release"));

	GlobalInit();

	WNDCLASSEX wcex;
	HWND hWnd;
	MSG msg;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Rhythm Runner"),
			MB_OK);

		return 1;
	}

	hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, //���ô�����ʽ�����ɸı��С���������
		CW_USEDEFAULT, CW_USEDEFAULT,
		WNDWIDTH, WNDHEIGHT + WNDTITLEBARHEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Mega Plane"),
			MB_OK);

		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		//��ʼ��
		WindowInit(hWnd, wParam, lParam);
		break;
	case WM_PAINT:
		//����
		Render(hWnd);
		break;
	case WM_KEYDOWN:
		//���̰����¼�
		KeyDown(hWnd, wParam, lParam);
		break;
	case WM_KEYUP:
		//�����ɿ��¼�
		break;
	case WM_LBUTTONDOWN:
		//������¼�
		LButtonDown(hWnd, wParam, lParam);
		break;
	case WM_TOUCH:
		TouchEvent(hWnd, wParam, lParam);
		break;
	case WM_TIMER:
		//��ʱ���¼�
		TimerUpdate(hWnd, wParam, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}
