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
#include "common_definitions.h"
#include "sdy_config.h"
#include "scade_display_dir.h"

/* --------------------------- 宏定义 -------------------------------*/
#define FONT_TABLE_BASE_SIZE 20

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/
char **font_table;
int font_count;
static int font_table_size;
extern resource_table_t resource_table;

/* --------------------------- 函数定义 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void parse_font_table(char *sdy_source_dir_in)
{
  xmlDocPtr doc;
  xmlNodePtr curNode;
  xmlAttrPtr attrPtr;
  char *szDocName = resource_table.font_table_file;
  int font_table_size;

  font_table = NULL;
  font_table_size = 0;
  font_count = 0;

  doc = xmlReadFile(szDocName,"UTF-8",XML_PARSE_RECOVER);
  if (NULL == doc)
  {  
    fatal_error("%s is not parsed successfully\n", szDocName);    
  }

  curNode = xmlDocGetRootElement(doc);
  if (NULL == curNode)
  {
    xmlFreeDoc(doc);
    fatal_error("%s is empty document\n", szDocName);
  }

  if (xmlStrcmp(curNode->name, BAD_CAST "fontTable"))
  {
    xmlFreeDoc(doc);
    fatal_error("document of the wrong type, root node != fontTable\n");
  }

  curNode = curNode->xmlChildrenNode;
  while(curNode != NULL)
  {
    if ((!xmlStrcmp(curNode->name, (const xmlChar *)"fontDefinition")))
    {
      attrPtr = curNode->properties;
      while (attrPtr != NULL)
      {
        if (!xmlStrcmp(attrPtr->name, BAD_CAST "path"))
        {
          xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "path");
          adjust_mem((void **)&font_table, 
            &font_count, 
            &font_table_size,
            FONT_TABLE_BASE_SIZE, sizeof(char *));
          if (*((char *)szAttr) == '.') //相对路径
          {
            font_table[font_count] =  (char *)get_mem(strlen(sdy_source_dir_in) + strlen((char *)szAttr) + 2);
            sprintf(font_table[font_count], "%s\\%s", sdy_source_dir_in, (char *)szAttr);
          }
          else if (strstr((char *)szAttr, "$(SCADEDISPLAY_HOME)"))
          {
            char temp[FILE_NAME_LENGTH];
            sprintf(temp, "%s%s", get_scade_display_dir(), (char *)szAttr + 20);
            font_table[font_count] =  (char *)get_mem(strlen(temp) + 1);
            strcpy(font_table[font_count], temp);
          }
          else if (strchr((char *)szAttr, '/')) //绝对路径
          {
            font_table[font_count] =  (char *)get_mem(strlen((char *)szAttr) + 1);
            strcpy(font_table[font_count], (char *)szAttr);
          }
          else //以下适用于SCADE R16.2.1\SCADE Display\KCG66\oglx\examples\pfd
          {
            char temp[FILE_NAME_LENGTH];
            sprintf(temp, "%s\\%s", sdy_source_dir_in, (char *)szAttr);
            font_table[font_count] =  (char *)get_mem(strlen(temp) + 1);
            strcpy(font_table[font_count], temp);
          }
          font_count++;
          xmlFree(szAttr);
        }
        attrPtr = attrPtr->next;
      }
    }
    curNode = curNode->next;
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
void free_font_table(void)
{
  int i;
  for (i = 0; i < font_count; i++)
  {
    free_mem(font_table[i]);
  }
  free_mem(font_table);
}