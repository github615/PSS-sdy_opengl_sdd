/* --------------------------- 序言 ---------------------------------*/

/**
 **********************************************************************
 *
 * (C) 2016 上海爱韦讯信息技术有限公司。
 *
 * @功能描述
 *
 **********************************************************************
*/
/* --------------------------- 文件包含 -----------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libxml/parser.h"
#include "libxml/tree.h"
#include "sdy_config.h"
#include "sdy_source.h"

/* --------------------------- 宏定义 -------------------------------*/
#define TRACEINFO_BASE_SIZE 60

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
void parse_trace_file(char *trace_file_name_in)
{
  xmlDocPtr doc;
  xmlNodePtr curNode;
  xmlAttrPtr attrPtr;
  char *szDocName = trace_file_name_in;

  doc = xmlReadFile(szDocName, "UTF-8", XML_PARSE_RECOVER);
  if (NULL == doc)
  {  
    warning_msg("parse_trace_file: Can't open %s\n", szDocName); 
    return;
  }

  curNode = xmlDocGetRootElement(doc);
  if (NULL == curNode)
  {
    xmlFreeDoc(doc);
    warning_msg("parse_trace_file: %s is empty document\n", szDocName);
    return;
  }

  if (xmlStrcmp(curNode->name, BAD_CAST "traceability"))
  {
    xmlFreeDoc(doc);
    warning_msg("parse_trace_file: document of the wrong type, root node != traceability\n");
    return;
  }

  curNode = curNode->xmlChildrenNode;
  while(curNode != NULL)
  {
    if ((!xmlStrcmp(curNode->name, (const xmlChar *)"object")))
    {
      xmlNodePtr sonNode = curNode->xmlChildrenNode;
      object_t *object = NULL;
      variable_dictionary_info_t *input_info = NULL;
      char *traceinfo = (char *)get_cmem(TRACEINFO_BASE_SIZE);
      int traceinfo_alloc_length = TRACEINFO_BASE_SIZE;

      attrPtr = curNode->properties;
      while (attrPtr != NULL)
      {
        if (!xmlStrcmp(attrPtr->name, BAD_CAST "id"))
        {
          xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "id");
          object = search_object_by_oid((char *)szAttr);
          if (object == NULL)
          {
            input_info = search_input_info_by_oid((char *)szAttr);
            if (input_info == NULL)
            {
              warning_msg("Can't find oid %s for traceinfo\n", (char *)szAttr);
            }
          }
          xmlFree(szAttr);
        }
        attrPtr = attrPtr->next;
      }

      while (sonNode != NULL)
      {
        if ((!xmlStrcmp(sonNode->name, (const xmlChar *)"requirement")))
        {

          attrPtr = sonNode->properties;
          while (attrPtr != NULL)
          {
            if (!xmlStrcmp(attrPtr->name, BAD_CAST "id"))
            {
              xmlChar* szAttr = xmlGetProp(sonNode, BAD_CAST "id");
              int traceinfo_used_length = strlen(traceinfo);
              int l = strlen((const char*)szAttr);
              if ((traceinfo_used_length + l + 2) > traceinfo_alloc_length)
              {
                traceinfo_alloc_length += TRACEINFO_BASE_SIZE;
                traceinfo = (char *)reget_mem(traceinfo, traceinfo_alloc_length);
              }
              if (traceinfo_used_length > 0)
              {
                strcat(traceinfo, ",");
              }
              strcat(traceinfo, (const char*)szAttr);
              xmlFree(szAttr);
            }
            attrPtr = attrPtr->next;
          }
        }
        sonNode = sonNode->next;
      }
      if (object)
      {
        object->traceinfo = traceinfo;
      }
      else
      {
        if (input_info)
        {
          if (input_info->trace[0])
          {
            strcat(input_info->trace, ",");
          }
          strcat(input_info->trace, traceinfo);
        }
        free_mem(traceinfo);
      }
    }
    curNode = curNode->next;
  }
  xmlFreeDoc(doc);
}
