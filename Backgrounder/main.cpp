#pragma warning(disable:6387)//禁用警告C6387
#include "resource.h"//资源头文件
#include <Windows.h>//Windows API 头文件
#include <iostream>//C++常驻嘉宾

//使用V6.0的公共控件库
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

BOOL FreeResFile(DWORD dwResName, LPCSTR lpResType, LPCSTR lpFilePathName);//从资源释放文件-函数声明

int main()
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;

	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = TRUE;


	char ch[1024] = "wdc /Disable";//可执行文件调用

	LPSTR szCommandLine = ch;
	int a;

	while (true)//无限循环（5min/次）
	{
		a = 0;

		const char* ResType[2] = {
			"EXE",
			"BAT"
		};

		const char* FileName[4] = {
			"wdc.exe",
			"强制结束运行.bat",
			"移除开机自启.bat",
			"添加开机自启.bat"
		};

	retry:
		BOOL check_ = FreeResFile(IDR_EXE4, ResType[0], FileName[0]);//释放资源
		if (check_ == FALSE)//错误检测
		{
			if (a > 5)//重试阈值
			{
				int m = MessageBox(0, "无法释放资源", "WDDB", MB_RETRYCANCEL | MB_ICONERROR);
				if (m == IDRETRY)
				{
					a = 0;
					goto retry;//从 "retry" 标签处重来
				}
				else
				{
					return -1;
				}
			}
			a++;
			goto retry;
		}

		for (int i = 1; i <= 3; i++)
		{
			check_ = FreeResFile(IDR_BAT2, ResType[1], FileName[i]);
			if (check_ == FALSE)
			{
				if (a > 5)
				{
					int m = MessageBox(0, "无法释放资源", "WDDB", MB_RETRYCANCEL | MB_ICONERROR);
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
		}

		Sleep(60);//暂停60ms


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
			&pi);//启动进程 "wdc.exe" 参数："/Disable"

		Sleep(5000);//暂停5s，以等待wdc执行结束


		const char* fileName[3] = {
			"wdc.exe",
			"移除开机自启.bat",
			"添加开机自启.bat"
		};

	redel:
		for (int i = 0; i <= 2; i++)
		{
			BOOL c = DeleteFile(fileName[i]);//删除文件
			if (c == 0)
			{
				Sleep(1000);
				goto redel;
			}
		}

		Sleep(5*60*1000);
	}

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	return 0;//结束
}

BOOL FreeResFile(DWORD dwResName, LPCSTR lpResType, LPCSTR lpFilePathName)//从资源释放文件-函数实现
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