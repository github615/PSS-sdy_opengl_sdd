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
static void get_item_list(CXMLDOMElement &reqStatementElement, vector<CString> &listItemVector)
{
  CXMLDOMElement reqListElement;
  reqListElement.AttachDispatch(reqStatementElement.get_nextSibling());
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
static void parse_section_filelist(CXMLDOMElement &sectionFileListElement)
{
  VARIANT headingVal;
  get_heading_val(sectionFileListElement, headingVal);
  compose_section_heading(headingVal.bstrVal, 2, false);

  CXMLDOMElement reqStatementElement;
  reqStatementElement.AttachDispatch(sectionFileListElement.get_firstChild());
  CString text = reqStatementElement.get_text();

  vector<CString> listItemVector;
  get_item_list(reqStatementElement, listItemVector);

  compose_requirement_with_list(text, listItemVector);
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

  vector<CString> listItemVector;
  get_item_list(reqStatementElement, listItemVector);

  compose_requirement_with_list(text, listItemVector);
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
    compose_requirement(text);

    CXMLDOMElement hierarchyFigureElement;
    hierarchyFigureElement.AttachDispatch(reqStatementElement.get_nextSibling());
    text = hierarchyFigureElement.get_text();
    compose_list_item(text);
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

  CXMLDOMElement columnAElement;
  columnAElement.AttachDispatch(tableVariableElement.get_firstChild());
  CXMLDOMElement columnBElement;
  columnBElement.AttachDispatch(columnAElement.get_nextSibling());
  vector<CString> columnBCellVector;
  get_cell_list(columnBElement, columnBCellVector);

  compose_variable_table(text, columnBCellVector);
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
  while (1)
  {
    text = objectElement.get_text();

    CXMLDOMElement tableElement;
    tableElement.AttachDispatch(objectElement.get_nextSibling());
    VARIANT typeVal;
    CXMLDOMAttribute typeAttr;
    typeAttr.AttachDispatch(tableElement.getAttributeNode(_T("type")));
    typeVal = typeAttr.get_nodeValue();
    template_table_id_t tabelId = lookup_template_table_id(typeVal.bstrVal);

    CXMLDOMElement columnAElement;
    columnAElement.AttachDispatch(tableElement.get_firstChild());
    vector<CString> columnACellVector;
    if (tabelId == TEMPLATE_TABLE_SHAPE ||
        tabelId == TEMPLATE_TABLE_INTERFACE_PARAMETERS ||
        tabelId == TEMPLATE_TABLE_UNKNOWN)
    {
      get_cell_list(columnAElement, columnACellVector);
    }

    CXMLDOMElement columnBElement;
    columnBElement.AttachDispatch(columnAElement.get_nextSibling());
    vector<CString> columnBCellVector;
    get_cell_list(columnBElement, columnBCellVector);

    CXMLDOMElement columnCElement;
    columnCElement.AttachDispatch(columnBElement.get_nextSibling());
    vector<CString> columnCCellVector;
    get_cell_list(columnCElement, columnCCellVector);

    if (tabelId == TEMPLATE_TABLE_SHAPE)
    {
      compose_requirement_with_shape_table(text, 
        columnACellVector,
        columnBCellVector,
        columnCCellVector);
    }
    else if (tabelId == TEMPLATE_TABLE_INTERFACE_PARAMETERS)
    {
      compose_requirement_with_interface_parameters_table(text,
        columnACellVector,
        columnBCellVector,
        columnCCellVector);
    }
    else if (tabelId == TEMPLATE_TABLE_UNKNOWN)
    {
      compose_requirement_with_unknown_table(text,
        typeVal.bstrVal,
        columnACellVector,
        columnBCellVector,
        columnCCellVector);
    }
    else
    {
      compose_requirement_with_known_table(tabelId, text, columnBCellVector, columnCCellVector);
    }

    LPDISPATCH p = tableElement.get_nextSibling();
    if (!p) break;

    CXMLDOMElement imageElement;
    imageElement.AttachDispatch(p);
    if (imageElement.get_nodeName() == _T("req_image"))
    {
      text = imageElement.get_text();
      compose_requirement(text);

      VARIANT fileVal;
      CXMLDOMAttribute fileAttr;
      fileAttr.AttachDispatch(imageElement.getAttributeNode(_T("file")));
      fileVal = fileAttr.get_nodeValue();
      compose_figure_with_number(fileVal.bstrVal);

      break;
    }
    else
    {
      objectElement = imageElement;
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