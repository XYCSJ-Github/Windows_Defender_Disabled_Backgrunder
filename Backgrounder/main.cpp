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
				int m = MessageBox(0, "�޷��ͷ���Դ wdc.exe", "WDDB", MB_RETRYCANCEL | MB_ICONERROR);
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

		check_ = FreeResFile(IDR_BAT2, "BAT", "ǿ�ƽ�������.bat");
		if (check_ == FALSE)
		{
			if (a > 5)
			{
				int m = MessageBox(0, "�޷��ͷ���Դ ǿ�ƽ�������.bat", "WDDB", MB_RETRYCANCEL | MB_ICONERROR);
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


		check_ = FreeResFile(IDR_BAT3, "BAT", "�Ƴ���������.bat");
		if (check_ == FALSE)
		{
			if (a > 5)
			{
				int m = MessageBox(0, "�޷��ͷ���Դ �Ƴ���������.bat", "WDDB", MB_RETRYCANCEL | MB_ICONERROR);
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


		check_ = FreeResFile(IDR_BAT1, "BAT", "��ӿ�������.bat");
		if (check_ == FALSE)
		{
			if (a > 5)
			{
				int m = MessageBox(0, "�޷��ͷ���Դ ��ӿ�������.bat", "WDDB", MB_RETRYCANCEL | MB_ICONERROR);
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

		c = DeleteFile("�Ƴ���������.bat");
		if (c == 0)
		{
			Sleep(500);
			goto redel;
		}

		c = DeleteFile("��ӿ�������.bat");
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
	HMODULE hInstance = ::GetModuleHandle(NULL);//�õ�����ʵ�����  

	HRSRC hResID = ::FindResource(hInstance, MAKEINTRESOURCE(dwResName), lpResType);//������Դ  
	HGLOBAL hRes = ::LoadResource(hInstance, hResID);//������Դ  
	LPVOID pRes = ::LockResource(hRes);//������Դ  

	if (pRes == NULL)//����ʧ��  
	{
		return FALSE;
	}
	DWORD dwResSize = ::SizeofResource(hInstance, hResID);//�õ����ͷ���Դ�ļ���С  
	HANDLE hResFile = CreateFile(lpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);//�����ļ�  

	if (INVALID_HANDLE_VALUE == hResFile)
	{
		//TRACE("�����ļ�ʧ�ܣ�");  
		return FALSE;
	}

	DWORD dwWritten = 0;//д���ļ��Ĵ�С     
	WriteFile(hResFile, pRes, dwResSize, &dwWritten, NULL);//д���ļ�  
	CloseHandle(hResFile);//�ر��ļ����  

	return (dwResSize == dwWritten);//��д���С�����ļ���С�����سɹ�������ʧ��  
}