// ChkReqConsole.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "sdyddword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

extern void SetOption(TCHAR* ObjectTypeReqIn, TCHAR* ObjectTypeComIn, TCHAR* ReqTextColumnIdIn, TCHAR* ObjTypeColumnIdIn, TCHAR* DerivedColumnIdIn, TCHAR* RationalColumnIdIn);

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: 更改错误代码以符合您的需要
			_tprintf(_T("错误: MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: 在此处为应用程序的行为编写代码。
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
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: GetModuleHandle 失败\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
