// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
#endif

#include <afx.h>
#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>



// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <io.h>
#include <direct.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <iostream>
#include <map>
#include <vector>
using namespace std;

#include "CXMLDOMAttribute.h"
#include "CXMLDOMDocument2.h"
#include "CXMLDOMElement.h"
#include "CXMLDOMNode.h"
#include "CXMLDOMNodeList.h"

#include "CApplication.h"
#include "CBorder.h"
#include "CBorders.h"
#include "CCells.h"
#include "CCell.h"
#include "CColumns0.h"
#include "CColumn.h"
#include "CDocuments.h"
#include "CDocument0.h"
#include "CFind.h"
#include "CHyperlink.h"
#include "CHyperlinks.h"
#include "CnlineShape.h"
#include "CnlineShapes.h"
#include "CParagraphs.h"
#include "CParagraph.h"
#include "CParagraphFormat.h"
#include "CRange.h" 
#include "CRows.h"
#include "CRow.h"
#include "CSection.h"
#include "CSections.h"
#include "CSelection.h"
#include "CSentences.h"
#include "CStyle.h"
#include "CStyles.h"
#include "CTables0.h"
#include "CTable0.h"
#include "CWords.h"

#include "obj_template.h"
#include "sdd_file.h"
#include "word_file.h"
#include "utilities.h"
#include "xml_file.h"
