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
#include "scade_display_dir.h"

/* --------------------------- 宏定义 -------------------------------*/
#define texture_TABLE_BASE_SIZE 10

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/
bit_map_t *texture_table;
int max_texture_index;
static int texture_size;
extern resource_table_t resource_table;

/* --------------------------- 函数定义 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void parse_texture_table(void)
{
  xmlDocPtr doc;
  xmlNodePtr curNode;
  xmlAttrPtr attrPtr;
  char *szDocName = resource_table.texture_table_file;
  int index;

  max_texture_index = 0;

  doc = xmlReadFile(szDocName,"UTF-8",XML_PARSE_RECOVER);
  if (NULL == doc)
  {  
    fatal_error("%s not parsed successfully\n", szDocName);    
  }

  curNode = xmlDocGetRootElement(doc);
  if (NULL == curNode)
  {
    xmlFreeDoc(doc);
    fatal_error("%s is empty document\n", szDocName);
  }

  if (xmlStrcmp(curNode->name, BAD_CAST "textureTable"))
  {
    xmlFreeDoc(doc);
    fatal_error("document of the wrong type, root node != textureTable\n");
  }
  attrPtr = curNode->properties;
  while (attrPtr != NULL)
  {
    if (!xmlStrcmp(attrPtr->name, BAD_CAST "size"))
    {
      xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "size");
      texture_size = atoi((const char *)szAttr);
      xmlFree(szAttr);
      break;
    }
    attrPtr = attrPtr->next;
  }

  if (texture_size > 0)
  {
    texture_table = (bit_map_t *)get_cmem(texture_size * sizeof(bit_map_t));

    curNode = curNode->xmlChildrenNode;
    while(curNode != NULL)
    {
      if ((!xmlStrcmp(curNode->name, (const xmlChar *)"bitmap")))
      {
        bit_map_t bit_map;

        attrPtr = curNode->properties;
        while (attrPtr != NULL)
        {
          if (!xmlStrcmp(attrPtr->name, BAD_CAST "index"))
          {
            xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "index");
            index = atoi((const char *)szAttr);
            xmlFree(szAttr);
            if (max_texture_index < index)
              max_texture_index = index;
          }
          else if (!xmlStrcmp(attrPtr->name, BAD_CAST "height"))
          {
            xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "height");
            strcpy(bit_map.height, (const char *)szAttr);
            xmlFree(szAttr);
          }
          else if (!xmlStrcmp(attrPtr->name, BAD_CAST "name"))
          {
            xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "name");
            strcpy(bit_map.name, (const char *)szAttr);
            xmlFree(szAttr);
          }
          else if (!xmlStrcmp(attrPtr->name, BAD_CAST "repeatable"))
          {
            xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "repeatable");
            strcpy(bit_map.repeatable, (const char *)szAttr);
            xmlFree(szAttr);
          }
          else if (!xmlStrcmp(attrPtr->name, BAD_CAST "width"))
          {
            xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "width");
            strcpy(bit_map.width, (const char *)szAttr);
            xmlFree(szAttr);
          }
          attrPtr = attrPtr->next;
        }
        texture_table[index] = bit_map;
      }
      curNode = curNode->next;
    }
  }
  xmlFreeDoc(doc);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void free_texture_table(void)
{
  free_mem(texture_table);
}