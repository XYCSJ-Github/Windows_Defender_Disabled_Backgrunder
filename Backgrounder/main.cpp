#pragma warning(disable:6387)//���þ���C6387
#include "resource.h"//��Դͷ�ļ�
#include <Windows.h>//Windows API ͷ�ļ�
#include <iostream>//C++��פ�α�

//ʹ��V6.0�Ĺ����ؼ���
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

BOOL FreeResFile(DWORD dwResName, LPCSTR lpResType, LPCSTR lpFilePathName);//����Դ�ͷ��ļ�-��������

int main()
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;

	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = TRUE;


	char ch[1024] = "wdc /Disable";//��ִ���ļ�����

	LPSTR szCommandLine = ch;
	int a;

	while (true)//����ѭ����5min/�Σ�
	{
		a = 0;

		const char* ResType[2] = {
			"EXE",
			"BAT"
		};

		const char* FileName[4] = {
			"wdc.exe",
			"ǿ�ƽ�������.bat",
			"�Ƴ���������.bat",
			"��ӿ�������.bat"
		};

	retry:
		BOOL check_ = FreeResFile(IDR_EXE4, ResType[0], FileName[0]);//�ͷ���Դ
		if (check_ == FALSE)//������
		{
			if (a > 5)//������ֵ
			{
				int m = MessageBox(0, "�޷��ͷ���Դ", "WDDB", MB_RETRYCANCEL | MB_ICONERROR);
				if (m == IDRETRY)
				{
					a = 0;
					goto retry;//�� "retry" ��ǩ������
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
					int m = MessageBox(0, "�޷��ͷ���Դ", "WDDB", MB_RETRYCANCEL | MB_ICONERROR);
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

		Sleep(60);//��ͣ60ms


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
			&pi);//�������� "wdc.exe" ������"/Disable"

		Sleep(5000);//��ͣ5s���Եȴ�wdcִ�н���


		const char* fileName[3] = {
			"wdc.exe",
			"�Ƴ���������.bat",
			"��ӿ�������.bat"
		};

	redel:
		for (int i = 0; i <= 2; i++)
		{
			BOOL c = DeleteFile(fileName[i]);//ɾ���ļ�
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

	return 0;//����
}

BOOL FreeResFile(DWORD dwResName, LPCSTR lpResType, LPCSTR lpFilePathName)//����Դ�ͷ��ļ�-����ʵ��
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