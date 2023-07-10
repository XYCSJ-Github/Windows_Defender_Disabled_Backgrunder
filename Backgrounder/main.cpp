#pragma warning(disable:6387)
#include "resource.h"
#include <Windows.h>
#include <iostream>

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

BOOL FreeResFile(DWORD dwResName, LPCSTR lpResType, LPCSTR lpFilePathName);

int main()
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;

	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = TRUE;


	char ch[1024] = "wdc /Disable";

	LPSTR szCommandLine = ch;
	int a;

	while (true)
	{
		a = 0;

	retry:
		BOOL check_ = FreeResFile(IDR_EXE4, "EXE", "wdc.exe");
		if (check_ == FALSE)
		{
			if (a > 5)
			{
				int m = MessageBox(0, "无法释放资源 wdc.exe", "WDDB", MB_RETRYCANCEL | MB_ICONERROR);
				if (m == IDRETRY)
				{
					a = 0;
					goto retry;
				}
				else
				{
					return -1;
				}
			}
			a++;
			goto retry;
		}

		check_ = FreeResFile(IDR_BAT2, "BAT", "强制结束运行.bat");
		if (check_ == FALSE)
		{
			if (a > 5)
			{
				int m = MessageBox(0, "无法释放资源 强制结束运行.bat", "WDDB", MB_RETRYCANCEL | MB_ICONERROR);
				if (m == IDRETRY)
				{
					a = 0;
					goto retry;
				}
				else
				{
					return -1;
				}
			}
			a++;
			goto retry;
		}


		check_ = FreeResFile(IDR_BAT3, "BAT", "移除开机自启.bat");
		if (check_ == FALSE)
		{
			if (a > 5)
			{
				int m = MessageBox(0, "无法释放资源 移除开机自启.bat", "WDDB", MB_RETRYCANCEL | MB_ICONERROR);
				if (m == IDRETRY)
				{
					a = 0;
					goto retry;
				}
				else
				{
					return -1;
				}
			}
			a++;
			goto retry;
		}


		check_ = FreeResFile(IDR_BAT1, "BAT", "添加开机自启.bat");
		if (check_ == FALSE)
		{
			if (a > 5)
			{
				int m = MessageBox(0, "无法释放资源 添加开机自启.bat", "WDDB", MB_RETRYCANCEL | MB_ICONERROR);
				if (m == IDRETRY)
				{
					a = 0;
					goto retry;
				}
				else
				{
					return -1;
				}
			}
			a++;
			goto retry;
		}

		Sleep(60);


		CreateProcess(
			NULL,
			szCommandLine,
			NULL,
			NULL,
			FALSE,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&si,
			&pi);

		Sleep(5000);


	redel:
		BOOL c = DeleteFile("wdc.exe");
		if (c == 0)
		{
			Sleep(1000);
			goto redel;
		}

		c = DeleteFile("移除开机自启.bat");
		if (c == 0)
		{
			Sleep(500);
			goto redel;
		}

		c = DeleteFile("添加开机自启.bat");
		if (c == 0)
		{
			Sleep(500);
			goto redel;
		}

		Sleep(5*60*1000);
	}

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	return 0;
}

BOOL FreeResFile(DWORD dwResName, LPCSTR lpResType, LPCSTR lpFilePathName)
{
	HMODULE hInstance = ::GetModuleHandle(NULL);//得到自身实例句柄  

	HRSRC hResID = ::FindResource(hInstance, MAKEINTRESOURCE(dwResName), lpResType);//查找资源  
	HGLOBAL hRes = ::LoadResource(hInstance, hResID);//加载资源  
	LPVOID pRes = ::LockResource(hRes);//锁定资源  

	if (pRes == NULL)//锁定失败  
	{
		return FALSE;
	}
	DWORD dwResSize = ::SizeofResource(hInstance, hResID);//得到待释放资源文件大小  
	HANDLE hResFile = CreateFile(lpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);//创建文件  

	if (INVALID_HANDLE_VALUE == hResFile)
	{
		//TRACE("创建文件失败！");  
		return FALSE;
	}

	DWORD dwWritten = 0;//写入文件的大小     
	WriteFile(hResFile, pRes, dwResSize, &dwWritten, NULL);//写入文件  
	CloseHandle(hResFile);//关闭文件句柄  

	return (dwResSize == dwWritten);//若写入大小等于文件大小，返回成功，否则失败  
}