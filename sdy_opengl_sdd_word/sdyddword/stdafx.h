// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            //  从 Windows 头文件中排除极少使用的信息
#endif

#include <afx.h>
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>



// TODO: 在此处引用程序需要的其他头文件

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
