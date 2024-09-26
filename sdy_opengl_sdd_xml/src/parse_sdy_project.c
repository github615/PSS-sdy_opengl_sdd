/* --------------------------- 序言 ---------------------------------*/

/**
 **********************************************************************
 *
 * (C) 2016, 2018 上海爱韦讯信息技术有限公司。
 *
 * @功能描述
 *
 **********************************************************************
*/
/* --------------------------- 文件包含 -----------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Shlwapi.h"
#include "libxml/parser.h"
#include "libxml/tree.h"
#include "scade_display_dir.h"
#include "sdy_config.h"
#include "sdy_source.h"

/* --------------------------- 宏定义 -------------------------------*/

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/
resource_table_t resource_table;
sdy_config_t sdy_config;

/* --------------------------- 函数定义 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void canonicalize_file_names(char *sdy_source_dir_in)
{
  char temp[FILE_NAME_LENGTH];
  sprintf(temp, "%s\\%s", sdy_source_dir_in, sdy_config.soure_file);
  PathCanonicalizeA(sdy_config.soure_file, temp);
  sprintf(temp, "%s\\%s", sdy_source_dir_in, resource_table.color_table_file);
  PathCanonicalizeA(resource_table.color_table_file, temp);
  sprintf(temp, "%s\\%s", sdy_source_dir_in, resource_table.font_table_file);
  PathCanonicalizeA(resource_table.font_table_file, temp);
  sprintf(temp, "%s\\%s", sdy_source_dir_in, resource_table.line_stipple_table_file);
  PathCanonicalizeA(resource_table.line_stipple_table_file, temp);
  sprintf(temp, "%s\\%s", sdy_source_dir_in, resource_table.line_width_table_file);
  PathCanonicalizeA(resource_table.line_width_table_file, temp);
  sprintf(temp, "%s\\%s", sdy_source_dir_in, resource_table.texture_table_file);
  PathCanonicalizeA(resource_table.texture_table_file, temp);
}
/**
 **********************************************************************
 *
 * @功能描述
 *  从ewo_file找Configuration的name。再在etp文件中根据Configuration的name
 *  找Configuration的id。
 * @注记
 *  Configuration的id不再有用，因此取消本函数。
 *
 **********************************************************************
*/
/*
static void parse_ewo_file(char *etp_file_full_name)
{
  char ewo_file_name[FILE_NAME_LENGTH];
  char *p;
  FILE *fin;
  char line[LINE_LENGTH];
  size_t l;

  strcpy(ewo_file_name, etp_file_full_name);
  p = strrchr(ewo_file_name, '.');
  if (p)
  {
    strcpy(p, ".ewo");
  }
  else
  {
    fatal_error("Invalid etp file name: %s\n", etp_file_full_name);
  }

  fin = fopen(ewo_file_name, "r");
  if (NULL == fin)
  {
    fatal_error("Can't open %s\n", ewo_file_name);
  }

  while (fgets(line, LINE_LENGTH, fin))
  {
    if (strstr(line, "Generation"))
    {
      break;
    }
  }
  fgets(line, LINE_LENGTH, fin);
  l = strlen(line);
  line[l-2] = 0;
  p = strrchr(line, '\"');
  strcpy(sdy_config.config_name, p + 1);
  if (strncmp(sdy_config.config_name, "Code", 4) && strncmp(sdy_config.config_name, "Windows", 7))
  {
    fatal_error("Invalid generator configuration selected: %s", sdy_config.config_name);
  }
}*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void save_resource_file_name(char *file_name)
{
  char *p;
  if (p = strstr(file_name, resource_table.color_table_value))
  {
    if (p == file_name || *(p-1) == '\\' || *(p-1) == '/')
      strcpy(resource_table.color_table_file, file_name);
  }
  else if (p = strstr(file_name, resource_table.font_table_value))
  {
    if (p == file_name || *(p-1) == '\\' || *(p-1) == '/')
      strcpy(resource_table.font_table_file, file_name);
  }
  else if (p = strstr(file_name, resource_table.line_stipple_table_value))
  {
    if (p == file_name || *(p-1) == '\\' || *(p-1) == '/')
      strcpy(resource_table.line_stipple_table_file, file_name);
  }
  else if (p = strstr(file_name, resource_table.line_width_table_value))
  {
    if (p == file_name || *(p-1) == '\\' || *(p-1) == '/')
      strcpy(resource_table.line_width_table_file, file_name);
  }
  else if (p = strstr(file_name, resource_table.color_table_value))
  {
    if (p == file_name || *(p-1) == '\\' || *(p-1) == '/')
      strcpy(resource_table.color_table_file, file_name);
  }
  else if (p = strstr(file_name, resource_table.texture_table_value))
  {
    if (p == file_name || *(p-1) == '\\' || *(p-1) == '/')
      strcpy(resource_table.texture_table_file, file_name);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void parse_etp_file(char *etp_file_full_name)
{
  xmlDocPtr doc;
  xmlNodePtr curNode;
  xmlNodePtr sonNode;
  xmlNodePtr propsNode;
  xmlNodePtr rootsNode;
  xmlNodePtr configsNode;
  xmlAttrPtr attrPtr;
  bool found;

  doc = xmlReadFile(etp_file_full_name, "UTF-8", XML_PARSE_RECOVER);
  if (NULL == doc)
  {  
    fatal_error("Can't open %s\n", etp_file_full_name);    
  }

  curNode = xmlDocGetRootElement(doc);
  if (NULL == curNode)
  {
    xmlFreeDoc(doc);
    fatal_error("%s is empty document\n", etp_file_full_name);
  }

  if (xmlStrcmp(curNode->name, BAD_CAST "Project"))
  {
    xmlFreeDoc(doc);
    fatal_error("document of the wrong type, root node != Project\n");
  }

  propsNode = curNode->xmlChildrenNode->next;
  rootsNode = propsNode->next->next;
  configsNode = rootsNode->next->next;

  /*不需要sdy_config.config_id
  curNode = configsNode->xmlChildrenNode->next;
  found = false;
  while(curNode != NULL && !found)
  {
    if ((!xmlStrcmp(curNode->name, (const xmlChar *)"Configuration")))
    {
      attrPtr = curNode->properties;
      while (attrPtr != NULL && !found)
      {
        if (!xmlStrcmp(attrPtr->name, BAD_CAST "name"))
        {
          xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "name");
          if(!strcmp((const char *)szAttr, sdy_config.config_name))
          {
            xmlFree(szAttr);
            szAttr = xmlGetProp(curNode, BAD_CAST "id");
            sdy_config.config_id = atoi((const char *)szAttr);
            found = true;
          }
          xmlFree(szAttr);
        }
        attrPtr = attrPtr->next;
      }
    }
    curNode = curNode->next;
  }
  */

  curNode = propsNode->children->next;
  found = false;
  while(curNode != NULL && !found)
  {
    if ((!xmlStrcmp(curNode->name, (const xmlChar *)"Prop")))
    {
      attrPtr = curNode->properties;
      while (attrPtr != NULL && !found)
      {

        if (!xmlStrcmp(attrPtr->name, BAD_CAST "name"))
        {
          xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "name");
#if 0
          if(!strcmp((const char *)szAttr, "@SDY:CONFSOURCE") && !found && curNode->children)
          {
            sonNode = curNode->children->next;
            while (sonNode != NULL && !found)
            {
              if ((!xmlStrcmp(sonNode->name, (const xmlChar *)"value")))
              {
                strcpy(sdy_config.source_value, (char*)xmlNodeGetContent(sonNode));
                found = true;
              }
              /*不能把(config_id == sdy_config.config_id)作为搜索sdy_config.source_value
              的结束条件
              else if ((!xmlStrcmp(sonNode->name, (const xmlChar *)"configuration")))
              {
                unsigned config_id = atoi((char*)xmlNodeGetContent(sonNode));
                if (config_id == sdy_config.config_id)
                {
                  found = true;
                }
              }*/
              sonNode = sonNode->next;
            }
          }
          else
#endif
          if(!strcmp((const char *)szAttr, "@SDY:DEFAULTCOLORTABLE"))
          {
            sonNode = curNode->children->next;
            while (sonNode != NULL)
            {
              if ((!xmlStrcmp(sonNode->name, (const xmlChar *)"value")))
              {
                strcpy(resource_table.color_table_value, (char*)xmlNodeGetContent(sonNode));
              }
              sonNode = sonNode->next;
            }
          }
          else if(!strcmp((const char *)szAttr, "@SDY:DEFAULTLWTABLE"))
          {
            sonNode = curNode->children->next;
            while (sonNode != NULL)
            {
              if ((!xmlStrcmp(sonNode->name, (const xmlChar *)"value")))
              {
                strcpy(resource_table.line_width_table_value, (char*)xmlNodeGetContent(sonNode));
              }
              sonNode = sonNode->next;
            }
          }
          else if(!strcmp((const char *)szAttr, "@SDY:DEFAULTLSTABLE"))
          {
            sonNode = curNode->children->next;
            while (sonNode != NULL)
            {
              if ((!xmlStrcmp(sonNode->name, (const xmlChar *)"value")))
              {
                strcpy(resource_table.line_stipple_table_value, (char*)xmlNodeGetContent(sonNode));
              }
              sonNode = sonNode->next;
            }
          }
          else if(!strcmp((const char *)szAttr, "@SDY:TEXTURETABLE"))
          {
            sonNode = curNode->children->next;
            while (sonNode != NULL)
            {
              if ((!xmlStrcmp(sonNode->name, (const xmlChar *)"value")))
              {
                strcpy(resource_table.texture_table_value, (char*)xmlNodeGetContent(sonNode));
              }
              sonNode = sonNode->next;
            }
          }
          else if(!strcmp((const char *)szAttr, "@SDY:FONTTABLE"))
          {
            sonNode = curNode->children->next;
            while (sonNode != NULL)
            {
              if ((!xmlStrcmp(sonNode->name, (const xmlChar *)"value")))
              {
                strcpy(resource_table.font_table_value, (char*)xmlNodeGetContent(sonNode));
              }
              sonNode = sonNode->next;
            }
          }
          xmlFree(szAttr);
        }
        attrPtr = attrPtr->next;
      }
    }
    curNode = curNode->next;
  }

  curNode = rootsNode->xmlChildrenNode->next;
  while(curNode != NULL)
  {
    if ((!xmlStrcmp(curNode->name, (const xmlChar *)"Folder")))
    {
      attrPtr = curNode->properties;
      while (attrPtr != NULL)
      {
        if (!xmlStrcmp(attrPtr->name, BAD_CAST "name"))
        {
          xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "name");
          if(!strcmp((const char *)szAttr, "Resource Files"))
          {
            sonNode = curNode->children->next; //<elements>
            sonNode = sonNode->children->next; //<FileRef
            while (sonNode != NULL)
            {
              if ((!xmlStrcmp(sonNode->name, (const xmlChar *)"FileRef")))
              {
                xmlAttrPtr attrPtr2 = sonNode->properties;
                while (attrPtr2 != NULL)
                {
                  if (!xmlStrcmp(attrPtr2->name, BAD_CAST "persistAs"))
                  {
                    xmlChar* szAttr2 = xmlGetProp(sonNode, BAD_CAST "persistAs");
                    save_resource_file_name((char *)szAttr2);
                    xmlFree(szAttr2);
                  }
                  attrPtr2 = attrPtr2->next;
                }
              }
              sonNode = sonNode->next;
            }
          }
          else if(!strcmp((const char *)szAttr, "Model Files"))
          {
            bool found = false;
            sonNode = curNode->children->next; //<elements>
            sonNode = sonNode->children->next; //<FileRef
            while (sonNode != NULL && !found)
            {
              if ((!xmlStrcmp(sonNode->name, (const xmlChar *)"FileRef")))
              {
                xmlAttrPtr attrPtr2 = sonNode->properties;
                while (attrPtr2 != NULL && !found)
                {
                  if (!xmlStrcmp(attrPtr2->name, BAD_CAST "persistAs"))
                  {
                    xmlChar* szAttr2 = xmlGetProp(sonNode, BAD_CAST "persistAs");
                    //char *p;
                    //if (p = strstr((char *)szAttr2, sdy_config.source_value))
                    //{
                      //if (*(p + strlen(sdy_config.source_value)) == 0)
                      //{
                        strcpy(sdy_config.soure_file, (char *)szAttr2);
                        found = true;
                      //}
                    //}
                    xmlFree(szAttr2);
                  }
                  attrPtr2 = attrPtr2->next;
                }
              }
              sonNode = sonNode->next;
            }
          }
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
void parse_sdy_project(char *sdy_source_dir_in, char *etp_file_name_in)
{
  char etp_file_full_name[FILE_NAME_LENGTH];
  sprintf(etp_file_full_name, "%s\\%s", sdy_source_dir_in, etp_file_name_in);
  //parse_ewo_file(etp_file_full_name);
  parse_etp_file(etp_file_full_name);
  //if (0 == sdy_config.source_value[0])
  //{
  //  fatal_error("sdy_config.source_value is not found in etp file\n");
  //}
  canonicalize_file_names(sdy_source_dir_in);
}
