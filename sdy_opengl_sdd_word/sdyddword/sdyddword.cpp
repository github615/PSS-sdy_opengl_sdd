// ChkReqConsole.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "sdyddword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

extern void SetOption(TCHAR* ObjectTypeReqIn, TCHAR* ObjectTypeComIn, TCHAR* ReqTextColumnIdIn, TCHAR* ObjTypeColumnIdIn, TCHAR* DerivedColumnIdIn, TCHAR* RationalColumnIdIn);

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: ���Ĵ�������Է���������Ҫ
			_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
      if (argc < 6)
      {
        _tprintf(_T("Usage: sdyddword <output path> <sdd file name> <sdd template file name> <obj template file name> <xml file name>\n"));
        nRetCode = 1;
      }
      else
      {
        HRESULT hr = CoInitializeEx(NULL,COINIT_MULTITHREADED|COINIT_SPEED_OVER_MEMORY);
        if (FAILED(hr))
        {
          _tprintf(_T("CoInitializeEx fail\n"));
          return 1;
        }
        std::locale::global(std::locale(""));

        open_log_file(argv[1], _T("sdyddword.log"));
        init_word_file(argv[0], argv[1], argv[2], argv[3], argv[4]);
        init_word_app();
        init_obj_template();
        
        DWORD dwAttrib = GetFileAttributes(argv[5]);
        if (INVALID_FILE_ATTRIBUTES == dwAttrib)
        {
          fatal_error(_T("Cant open xml file %s\n"), argv[5]);
        }

        init_sdd_file(argv[5]);
        xml_to_word(argv[5]);
        close_log_file();
        close_obj_template();
        close_sdd_file();
        release_word_app();
      }
		}
	}
	else
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: GetModuleHandle ʧ��\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
