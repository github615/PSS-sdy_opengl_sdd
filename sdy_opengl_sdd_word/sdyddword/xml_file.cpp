/* --------------------------- 序言 ---------------------------------*/

/**
**********************************************************************
*
* (C) 2018 上海爱韦讯信息技术有限公司。
*
* @功能描述
*
**********************************************************************
*/

/* --------------------------- 文件包含 -----------------------------*/
#include "stdafx.h"

/* --------------------------- 宏定义 -------------------------------*/

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/

/* --------------------------- 函数定义 -----------------------------*/
/**
**********************************************************************
*
* @功能描述
*
**********************************************************************
*/
static void get_heading_val(CXMLDOMElement &element, VARIANT &headingVal)
{
  CString headingStr;
  CXMLDOMAttribute headingAttr;
  headingAttr.AttachDispatch(element.getAttributeNode(_T("heading")));
  headingVal = headingAttr.get_nodeValue();
}
/**
**********************************************************************
*
* @功能描述
*
**********************************************************************
*/
static void get_items(CXMLDOMElement &reqListElement, vector<CString> &listItemVector)
{
  CXMLDOMElement listItem;
  listItem.AttachDispatch(reqListElement.get_firstChild());
  while (1)
  {
    CString text = listItem.get_text();
    listItemVector.push_back(text);
    LPDISPATCH p = listItem.get_nextSibling();
    if (!p) break;
    listItem.AttachDispatch(p);
  }
}
/**
**********************************************************************
*
* @功能描述
*
**********************************************************************
*/
static void get_item_list(CXMLDOMElement &reqStatementElement, vector<CString> &listItemVector)
{
  CXMLDOMElement reqListElement;
  reqListElement.AttachDispatch(reqStatementElement.get_nextSibling());
  get_items(reqListElement, listItemVector);
}
/**
**********************************************************************
*
* @功能描述
*
**********************************************************************
*/
static void parse_section_filelist(CXMLDOMElement &sectionFileListElement)
{
  VARIANT headingVal;
  get_heading_val(sectionFileListElement, headingVal);
  compose_section_heading(headingVal.bstrVal, 2, false);

  CXMLDOMElement reqStatementElement;
  reqStatementElement.AttachDispatch(sectionFileListElement.get_firstChild());
  CString text = reqStatementElement.get_text();
  compose_requirement_paragraph(text);
  compose_item_list(reqStatementElement);
}
/**
**********************************************************************
*
* @功能描述
*
**********************************************************************
*/
static void parse_section_layer_list(CXMLDOMElement &sectionLayerListElement)
{
  VARIANT headingVal;
  get_heading_val(sectionLayerListElement, headingVal);
  compose_section_heading(headingVal.bstrVal, 2, false);

  CXMLDOMElement reqStatementElement;
  reqStatementElement.AttachDispatch(sectionLayerListElement.get_firstChild());
  CString text = reqStatementElement.get_text();
  compose_requirement_paragraph(text);
  compose_item_list(reqStatementElement);
}
/**
**********************************************************************
*
* @功能描述
*
**********************************************************************
*/
static void parse_section_hierarchical_structure(CXMLDOMElement &sectionHierarchyElement)
{
  VARIANT headingVal;
  get_heading_val(sectionHierarchyElement, headingVal);
  compose_section_heading(headingVal.bstrVal, 2, false);

  CXMLDOMElement reqStatementElement;
  reqStatementElement.AttachDispatch(sectionHierarchyElement.get_firstChild());

  while (1)
  {
    CString text = reqStatementElement.get_text();
    compose_requirement_paragraph(text);

    CXMLDOMElement hierarchyFigureElement;
    hierarchyFigureElement.AttachDispatch(reqStatementElement.get_nextSibling());
    text = hierarchyFigureElement.get_text();
    compose_hierarchy_figure(text);
    compose_figure_number();

    LPDISPATCH p = hierarchyFigureElement.get_nextSibling();
    if (!p) break;
    reqStatementElement.AttachDispatch(p);
  }
}
/**
**********************************************************************
*
* @功能描述
*
**********************************************************************
*/
static void parse_chapter_architecture(CXMLDOMElement &chapterArchitectureElement)
{
  VARIANT headingVal;
  get_heading_val(chapterArchitectureElement, headingVal);
  compose_section_heading(headingVal.bstrVal, 1, true);

  CXMLDOMElement sectionFileListElement;
  sectionFileListElement.AttachDispatch(chapterArchitectureElement.get_firstChild());
  parse_section_filelist(sectionFileListElement);

  CXMLDOMElement sectionLayerListElement;
  sectionLayerListElement.AttachDispatch(sectionFileListElement.get_nextSibling());
  parse_section_layer_list(sectionLayerListElement);

  CXMLDOMElement sectionHierarchyElement;
  sectionHierarchyElement.AttachDispatch(sectionLayerListElement.get_nextSibling());
  parse_section_hierarchical_structure(sectionHierarchyElement);
}
/**
**********************************************************************
*
* @功能描述
*
**********************************************************************
*/
static void get_cell_list(CXMLDOMElement &columnElement, vector<CString> &cellVector)
{
  CXMLDOMElement cellElement;
  cellElement.AttachDispatch(columnElement.get_firstChild());
  while (1)
  {
    CString text = cellElement.get_text();
    cellVector.push_back(text);
    LPDISPATCH p = cellElement.get_nextSibling();
    if (!p) break;
    cellElement.AttachDispatch(p);
  }
}
/**
**********************************************************************
*
* @功能描述
*
**********************************************************************
*/
static void parse_variable_requirement(CXMLDOMElement &variableElement)
{
  CString text = variableElement.get_text();

  CXMLDOMElement tableVariableElement;
  tableVariableElement.AttachDispatch(variableElement.get_nextSibling());

  CXMLDOMElement rowElement;
  rowElement.AttachDispatch(tableVariableElement.get_firstChild());

  compose_variable_table(text, rowElement);
}
/**
**********************************************************************
*
* @功能描述
*
**********************************************************************
*/
static void parse_variable_list(CXMLDOMElement &sectionVariableListElement)
{
  CXMLDOMElement reqStatementElement;
  reqStatementElement.AttachDispatch(sectionVariableListElement.get_firstChild());
  CString text = reqStatementElement.get_text();

  vector<CString> listItemVector;
  get_item_list(reqStatementElement, listItemVector);

  CXMLDOMElement variableElement;
  reqStatementElement.AttachDispatch(reqStatementElement.get_nextSibling()); 
  variableElement.AttachDispatch(reqStatementElement.get_nextSibling()); //skip var list

  int varCount = listItemVector.size();
  while (1)
  {
    parse_variable_requirement(variableElement);
    varCount--;
    if (varCount == 0) break;
    variableElement.AttachDispatch(variableElement.get_nextSibling()); //skip var table
    variableElement.AttachDispatch(variableElement.get_nextSibling()); 
  }
}
/**
**********************************************************************
*
* @功能描述
*
**********************************************************************
*/
static void parse_chapter_interface(CXMLDOMElement &chapterInterfaceElement)
{
  VARIANT headingVal;
  get_heading_val(chapterInterfaceElement, headingVal);
  compose_section_heading(headingVal.bstrVal, 1, true);

  CXMLDOMElement sectionSGFXElement;
  sectionSGFXElement.AttachDispatch(chapterInterfaceElement.get_firstChild());
  get_heading_val(sectionSGFXElement, headingVal);
  compose_section_heading(headingVal.bstrVal, 2, false);

  CXMLDOMElement sectionLayerElement;
  sectionLayerElement.AttachDispatch(sectionSGFXElement.get_firstChild());

  LPDISPATCH p;
  while (1)
  {
    get_heading_val(sectionLayerElement, headingVal);
    compose_section_heading(headingVal.bstrVal, 3, false);

    p = sectionLayerElement.get_firstChild();
    if (p)
    {
      CXMLDOMElement sectionVariableListElement;
      sectionVariableListElement.AttachDispatch(p);
      get_heading_val(sectionVariableListElement, headingVal);
      compose_section_heading(headingVal.bstrVal, 4, false);

      parse_variable_list(sectionVariableListElement);

      p = sectionVariableListElement.get_nextSibling();
      if (p)
      {
        sectionVariableListElement.AttachDispatch(p);
        get_heading_val(sectionVariableListElement, headingVal);
        compose_section_heading(headingVal.bstrVal, 4, false);
        parse_variable_list(sectionVariableListElement);
      }
    }

    p = sectionLayerElement.get_nextSibling();
    if (!p) break;
    sectionLayerElement.AttachDispatch(p);
  }

  p = sectionSGFXElement.get_nextSibling();
  if (p)
  {
    CXMLDOMElement sectionAllOGFXElement;
    sectionAllOGFXElement.AttachDispatch(sectionSGFXElement.get_nextSibling());
    get_heading_val(sectionAllOGFXElement, headingVal);
    compose_section_heading(headingVal.bstrVal, 2, false);

    CXMLDOMElement sectionOneOGFXElement;
    sectionOneOGFXElement.AttachDispatch(sectionAllOGFXElement.get_firstChild());

    while (1)
    {
      get_heading_val(sectionOneOGFXElement, headingVal);
      compose_section_heading(headingVal.bstrVal, 3, false);

      p = sectionOneOGFXElement.get_firstChild();
      if (p)
      {
        CXMLDOMElement sectionVariableListElement;
        sectionVariableListElement.AttachDispatch(p);
        get_heading_val(sectionVariableListElement, headingVal);
        compose_section_heading(headingVal.bstrVal, 4, false);
        parse_variable_list(sectionVariableListElement);
        
        p = sectionVariableListElement.get_nextSibling();
        if (p)
        {
          sectionVariableListElement.AttachDispatch(p);
          get_heading_val(sectionVariableListElement, headingVal);
          compose_section_heading(headingVal.bstrVal, 4, false);
          parse_variable_list(sectionVariableListElement);
        }
      }

      p = sectionOneOGFXElement.get_nextSibling();
      if (!p) break;
      sectionOneOGFXElement.AttachDispatch(p);
    }
  }
}
/**
**********************************************************************
*
* @功能描述
*
**********************************************************************
*/
static void parse_object_requirement(CXMLDOMElement &firstObjectElement)
{
  CString text;
  CXMLDOMElement objectElement = firstObjectElement;
  LPDISPATCH p;
  while (1)
  {
    CString nodeName = objectElement.get_nodeName();
    if (nodeName == _T("req_statement"))
    {
      compose_requirement_paragraph(objectElement.get_text());

      CXMLDOMElement tableElement;
      tableElement.AttachDispatch(objectElement.get_nextSibling());
      CXMLDOMAttribute typeAttr;
      typeAttr.AttachDispatch(tableElement.getAttributeNode(_T("type")));
      BSTR typeVal = typeAttr.get_nodeValue().bstrVal;
      CXMLDOMAttribute columnAAttr;
      columnAAttr.AttachDispatch(tableElement.getAttributeNode(_T("column_a")));
      BSTR columnAVal = columnAAttr.get_nodeValue().bstrVal;

      int rowCount;
      p = tableElement.getAttributeNode(_T("row_count"));
      if (p)
      {
        CXMLDOMAttribute rowCountAttr;
        rowCountAttr.AttachDispatch(p);
        VARIANT rowVal = rowCountAttr.get_nodeValue();
        CString str(rowVal.bstrVal);
        TCHAR *stopstr;
        rowCount = _tcstol(str, &stopstr, 10);
      }
      else
      {
        rowCount = 0;
      }
      
      CXMLDOMElement rowElement;
      rowElement.AttachDispatch(tableElement.get_firstChild());
      
      compose_object_requirement_table(
        typeVal,
        columnAVal,
        rowCount,
        rowElement);
      p = tableElement.get_nextSibling();
    }
    else if (nodeName == _T("req_image"))
    {
      compose_comment_paragraph(objectElement.get_text());
      
      VARIANT fileVal;
      CXMLDOMAttribute fileAttr;
      fileAttr.AttachDispatch(objectElement.getAttributeNode(_T("file")));
      fileVal = fileAttr.get_nodeValue();
      compose_figure_with_number(fileVal.bstrVal);

      p = objectElement.get_nextSibling();
    }
    else if (nodeName == _T("children_comment"))
    {
      compose_comment_paragraph(objectElement.get_text());
      compose_item_list(objectElement);

      p = objectElement.get_nextSibling();
      objectElement.AttachDispatch(p); //skip item_list
      p = objectElement.get_nextSibling();
    }
    else if (nodeName == _T("applicable_masks"))
    {
      compose_requirement_paragraph(objectElement.get_text());

      CXMLDOMElement tableElement;
      tableElement.AttachDispatch(objectElement.get_nextSibling());

      CXMLDOMElement rowElement;
      rowElement.AttachDispatch(tableElement.get_firstChild());

      compose_mask_table(rowElement);

      p = tableElement.get_nextSibling();//notice: it is true to get nest sibling of the table
    }
    else if (nodeName == _T("aliased_comment"))
    {
      compose_comment_paragraph(objectElement.get_text());
      p = objectElement.get_nextSibling();
    }
    if (!p) break;
    objectElement.AttachDispatch(p);
  }
}
/**
**********************************************************************
*
* @功能描述
*
**********************************************************************
*/
static void parse_object_list(CXMLDOMElement &sectionObjectListElement)
{
  CXMLDOMElement sectionObjectElement = sectionObjectListElement;
  while (1)
  {
    VARIANT headingVal;
    get_heading_val(sectionObjectElement, headingVal);
    compose_section_heading(headingVal.bstrVal, 4, false);

    CXMLDOMElement objectElement;
    objectElement.AttachDispatch(sectionObjectElement.get_firstChild());
    parse_object_requirement(objectElement);
    
    LPDISPATCH p = sectionObjectElement.get_nextSibling();
    if (!p) break;
    sectionObjectElement.AttachDispatch(p); 
  }
}
/**
**********************************************************************
*
* @功能描述
*
**********************************************************************
*/
static void parse_chapter_detail(CXMLDOMElement &chapterDetailElement)
{
  LPDISPATCH p;
  VARIANT headingVal;
  get_heading_val(chapterDetailElement, headingVal);
  compose_section_heading(headingVal.bstrVal, 1, true);

  CXMLDOMElement sectionSGFXElement;
  sectionSGFXElement.AttachDispatch(chapterDetailElement.get_firstChild());
  get_heading_val(sectionSGFXElement, headingVal);
  compose_section_heading(headingVal.bstrVal, 2, false);

  CXMLDOMElement sectionLayerElement;
  sectionLayerElement.AttachDispatch(sectionSGFXElement.get_firstChild());
  while (1)
  {
    get_heading_val(sectionLayerElement, headingVal);
    compose_section_heading(headingVal.bstrVal, 3, false);

    CXMLDOMElement sectionObjectListElement;
    sectionObjectListElement.AttachDispatch(sectionLayerElement.get_firstChild());
    parse_object_list(sectionObjectListElement);

    p = sectionLayerElement.get_nextSibling();
    if (!p) break;
    sectionLayerElement.AttachDispatch(p);
  }

  p = sectionSGFXElement.get_nextSibling();
  if (p)
  {
    CXMLDOMElement sectionAllOGFXElement;
    sectionAllOGFXElement.AttachDispatch(p);
    get_heading_val(sectionAllOGFXElement, headingVal);
    compose_section_heading(headingVal.bstrVal, 2, false);

    CXMLDOMElement sectionOGFXElement;
    sectionOGFXElement.AttachDispatch(sectionAllOGFXElement.get_firstChild());
    while (1)
    {
      get_heading_val(sectionOGFXElement, headingVal);
      compose_section_heading(headingVal.bstrVal, 3, false);

      CXMLDOMElement sectionObjectListElement;
      sectionObjectListElement.AttachDispatch(sectionOGFXElement.get_firstChild());
      parse_object_list(sectionObjectListElement);

      p = sectionOGFXElement.get_nextSibling();
      if (!p) break;
      sectionOGFXElement.AttachDispatch(p);
    }
  }
}
/**
**********************************************************************
*
* @功能描述
*
**********************************************************************
*/
void xml_to_word(TCHAR *xmlFileName)
{
  VARIANT vFileName;
  CXMLDOMDocument2 doc;
  CXMLDOMElement rootElement;
  CString tagName;

  VariantInit(&vFileName);
  V_BSTR(&vFileName) = SysAllocString(xmlFileName);
  V_VT(&vFileName) = VT_BSTR;

  if (!doc.CreateDispatch(__uuidof(DOMDocument)))
  {
    fatal_error(_T("Create XML service fail\n"));
  }
  doc.put_async(FALSE);
  BOOL r = doc.load(vFileName);

  rootElement.AttachDispatch(doc.get_documentElement());

  CXMLDOMElement chapterArchitectureElement;
  chapterArchitectureElement.AttachDispatch(rootElement.get_firstChild());
  parse_chapter_architecture(chapterArchitectureElement);

  CXMLDOMElement chapterInterfaceElement;
  chapterInterfaceElement.AttachDispatch(chapterArchitectureElement.get_nextSibling());
  parse_chapter_interface(chapterInterfaceElement);

  CXMLDOMElement chapterDetailElement;
  chapterDetailElement.AttachDispatch(chapterInterfaceElement.get_nextSibling());
  parse_chapter_detail(chapterDetailElement);
}