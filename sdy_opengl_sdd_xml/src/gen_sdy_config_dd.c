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
#include "Shlwapi.h"
#include "libxml/parser.h"
#include "libxml/tree.h"
#include "sdy_source.h"
#include "sdy_config.h"
#include "process_file_and_dir.h"
#include "process_sdy_code.h"

/* --------------------------- 宏定义 -------------------------------*/

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/
static char environment_id[ATTR_LENGTH];
static char color_mode[ATTR_LENGTH];
char *bgcolor = "bgcolor=\"#CCCCCC\"";

/* --------------------------- 函数定义 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void save_environment_id(char *environment_id_in)
{
  strcpy(environment_id, environment_id_in);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void save_color_mode(char *color_mode_in)
{
  strcpy(color_mode, color_mode_in);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void gen_ogfx_file_names(char *content_buffer)
{
  int i;
  int count;
  ogfx_info_t *ogfx = global_ogfx_info_list;

  count = (get_including_ogfx_children() == 1) ? global_ogfx_info_count : 0;
  for (i = 0; i < count; i++, ogfx++)
  {
    sprintf(content_buffer + strlen(content_buffer), "<br />%s", PathFindFileNameA(ogfx->file_name));
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
/*
void gen_general_config_content(char *content_buffer)
{
  sprintf(content_buffer + strlen(content_buffer), 
    "<table width=\"500px\" border=\"1\"><tr><td %s>Property</td><td %s>Value</td></tr>\n",
    bgcolor, bgcolor);

  sprintf(content_buffer + strlen(content_buffer), "<tr><td>File name</td><td>%s", PathFindFileNameA(get_sgfx_file_name()));
  gen_ogfx_file_names(content_buffer);
  sprintf(content_buffer + strlen(content_buffer), "</td></tr>\n");

  sprintf(content_buffer + strlen(content_buffer), "<tr><td>Window dimension (pixel)</td><td>%s</td></tr>\n", get_dimension_width_and_height());
  sprintf(content_buffer + strlen(content_buffer), "<tr><td>Environment id</td><td>%s</td></tr>\n", environment_id);
  sprintf(content_buffer + strlen(content_buffer), "<tr><td>Color Mode</td><td>%s</td></tr>\n", color_mode);
  strcpy(content_buffer + strlen(content_buffer), "</table>\n");
}*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void gen_color_table_content(char *content_buffer)
{
  int i;
  color_def_t *color_def = color_table;

  sprintf(content_buffer + strlen(content_buffer), 
    "<table width=\"500px\" border=\"1\"><tr><td %s>Color index</td><td %s>Color name</td><td %s>Color value(%s)</td><td %s>Color</td></tr>\n",
    bgcolor, bgcolor, bgcolor, color_mode, bgcolor);
  for (i = 0; i <= max_color_index; i++, color_def++)
  {
    if (color_def->name[0])
    {
      int rgb;
      sscanf(color_def->rgb + 2, "%x", &rgb);
      sprintf(content_buffer + strlen(content_buffer), "<tr><td>%d</td><td>%s</td><td>%d/%d/%d</td><td bgcolor=\"#%06x\"></tr>\n", 
        i, color_def->name, (rgb >> 16) & 0xff, (rgb >> 8) & 0xff, rgb & 0xff, rgb);
    }
  }
  strcpy(content_buffer + strlen(content_buffer), "</table>\n");
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void parse_font_file(char *szDocName, char *font_name, char *font_line_width)
{
  xmlDocPtr doc;
  xmlNodePtr curNode;
  xmlAttrPtr attrPtr;

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

  if (xmlStrcmp(curNode->name, BAD_CAST "font"))
  {
    xmlFreeDoc(doc);
    fatal_error("document of the wrong type, root node != font\n");
  }

  attrPtr = curNode->properties;
  while (attrPtr != NULL)
  {
    if (!xmlStrcmp(attrPtr->name, BAD_CAST "name"))
    {
      xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "name");
      strcpy(font_name, (char *)szAttr);
      xmlFree(szAttr);
      break;
    }
    attrPtr = attrPtr->next;
  }

  curNode = curNode->xmlChildrenNode;
  while(curNode != NULL)
  {
    if (!xmlStrcmp(curNode->name, (const xmlChar *)"line_widths"))
    {
      strcpy(font_line_width, (char *)xmlNodeGetContent(curNode));
      break;
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
void gen_font_table_content(char *content_buffer)
{
  int i;
  char font_name[ATTR_LENGTH];
  char font_line_width[ATTR_LENGTH];

  sprintf(content_buffer + strlen(content_buffer), 
    "<table width=\"500px\" border=\"1\"><tr><td %s>Font id</td><td %s>Font name</td><td %s>Font line width</td></tr>\n",
    bgcolor, bgcolor, bgcolor);
  for (i = 0; i < font_count; i++)
  {
    parse_font_file(font_table[i], font_name, font_line_width);
    sprintf(content_buffer + strlen(content_buffer), "<tr><td>%d</td><td>%s</td><td>%s</td></tr>\n", 
        i, font_name, font_line_width);
  }
  strcpy(content_buffer + strlen(content_buffer), "</table>\n");
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void gen_line_stipple_table_content(char *content_buffer)
{
  int i;
  line_stipple_def_t *line_stipple_def = line_stipple_table;

  sprintf(content_buffer + strlen(content_buffer), 
    "<table width=\"500px\" border=\"1\"><tr><td %s>Line stipple Index</td><td %s>Type</td><td %s>Stipple value</td></tr>\n",
    bgcolor, bgcolor, bgcolor);
  for (i = 0; i <= max_line_stipple_index; i++, line_stipple_def++)
  {
    sprintf(content_buffer + strlen(content_buffer), "<tr><td>%d</td><td>%s</td>", 
        i, line_stipple_def->stipple_type);
    if (line_stipple_def->line_stipple_length_attr_count == 0)
    {
      strcat(content_buffer, "<td>continuous line</td></tr>\n");
    }
    else
    {
      int j;
      line_stipple_length_attr_t * attr = line_stipple_def->line_stipple_length_attr_table;
      strcat(content_buffer, "<td>");
      for (j = 0; j < line_stipple_def->line_stipple_length_attr_count; j++, attr++)
      {
        sprintf(content_buffer + strlen(content_buffer), "%d %s", attr->value, strcmp(attr->visible, "true") ? "invisible" : "visible");
        if (j < line_stipple_def->line_stipple_length_attr_count - 1)
        {
          strcat(content_buffer, " / ");
        }
      }
      strcat(content_buffer, "</td></tr>\n");
    }
  }
  strcpy(content_buffer + strlen(content_buffer), "</table>\n");
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void gen_line_width_table_content(char *content_buffer)
{
  int i;
  line_width_t *line_width = line_width_table;

  sprintf(content_buffer + strlen(content_buffer), 
    "<table width=\"500px\" border=\"1\"><tr><td %s>Line width Index</td><td %s>Name</td><td %s>Line width</td><td %s>Color width</td><td %s>Halo width</td></tr>\n",
    bgcolor, bgcolor, bgcolor, bgcolor, bgcolor);
  for (i = 0; i <= max_line_width_index; i++, line_width++)
  {
    sprintf(content_buffer + strlen(content_buffer), "<tr><td>%d</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>\n", 
        i, line_width->name, line_width->width, line_width->color_width, line_width->halo_width);
  }
  strcpy(content_buffer + strlen(content_buffer), "</table>\n");
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void gen_texture_table_content(char *content_buffer)
{
  int i;
  bit_map_t *bit_map = texture_table;

  sprintf(content_buffer + strlen(content_buffer), 
    "<table width=\"500px\" border=\"1\"><tr><td %s>Name</td><td>Height</td><td %s>Width</td><td %s>Repeatable</td></tr>\n",
    bgcolor, bgcolor, bgcolor);
  for (i = 0; i <= max_texture_index; i++, bit_map++)
  {
    sprintf(content_buffer + strlen(content_buffer), "<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>\n", 
        bit_map->name, bit_map->height, bit_map->width, bit_map->repeatable);
  }
  strcpy(content_buffer + strlen(content_buffer), "</table>\n");
}
