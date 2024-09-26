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
#include "parsifal.h"
#include "common_definitions.h"
#include "sdy_source.h"

/* --------------------------- 宏定义 -------------------------------*/
#define PROP_NAME_LENGTH 64
#define PROP_VALUE_LENGTH 128

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
void create_object_prop(object_t *object, char *prop_name)
{
  object_prop_t *prop = (object_prop_t *)get_cmem(sizeof(object_prop_t));
  create_string_item(&prop->name, prop_name);
  if (NULL == object->prop_head)
  {
    object->prop_head = prop;
  }
  else
  {
    object->prop_tail->next = prop;
  }
  object->prop_tail = prop;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void set_prop_init_value(parsing_state_t *state)
{
  /*2017-3-17 rich_text的初值不能都转换为ASCII码，因此取消以下代码，保留原始格式
  if (state->is_text_value)
  {
    char temp[PROP_VALUE_LENGTH];
    int i = 0;
    char *p = state->character_data;
    temp[i++] = '\"';
    while (p)
    {
      temp[i++] = atoi(p + 1);
      p = strchr(p + 1, ',');
    }
    temp[i-1] = '\"';
    temp[i] = 0;
    create_string_item(&state->prop_value_and_expr_pointer->value, temp);
  }
  else*/
  {
    create_string_item(&state->prop_value_and_expr_pointer->value, state->character_data);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void set_prop_expr(parsing_state_t *state)
{
  create_string_item(&state->prop_value_and_expr_pointer->expr, state->character_data);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_object_prop_xy(parsing_state_t *state, char *x_name, char *y_name)
{
  create_object_prop(state->current_object, x_name);
  state->prop_value_and_expr_of_x_pointer = &state->current_object->prop_tail->value_and_expr;
  create_object_prop(state->current_object, y_name);
  state->prop_value_and_expr_of_y_pointer = &state->current_object->prop_tail->value_and_expr;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_object_prop(parsing_state_t *state, char *name)
{
  create_object_prop(state->current_object, name);
  state->prop_value_and_expr_pointer = &state->current_object->prop_tail->value_and_expr;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_orientation(LPXMLVECTOR atts, parsing_state_t *state)
{
  LPXMLRUNTIMEATT att;
  int i;
  char *value;
  static char *counter_clockwise = "Counter Clockwise";
  static char *clockwise = "Clockwise";

  for (i = 0; i < atts->length; i++)
  {
    att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
    if (strcmp((const char*)att->qname, "clockwise") == 0)
    {
      if (!state->is_arc_segment)
      {
        create_object_prop(state->current_object, "Orientation");
      }
      if (strcmp("false", (const char*)att->value) == 0)
      {
        value = counter_clockwise;
      }
      else
      {
        value = clockwise;
      }
      break;
    }
  }

  if (state->is_arc_segment)
  {
    create_string_item(&state->prop_value_and_expr_of_y_pointer->value, value);
  }
  else
  {
    create_string_item(&state->current_object->prop_tail->value_and_expr.value, value);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_static(LPXMLVECTOR atts, parsing_state_t *state)
{
  LPXMLRUNTIMEATT att;
  int i;
  char temp[LINE_LENGTH];

  temp[0] = 0;
  for (i = 0; i < atts->length; i++)
  {
    att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
    sprintf(temp + strlen(temp), "%s=\"%s\" ", (const char*)att->qname, (const char*)att->value);
  }
  handle_object_prop(state, "Static properties");
  create_string_item(&state->prop_value_and_expr_pointer->value, temp);
  handle_object_prop(state, "Static init");
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_point(parsing_state_t *state)
{
  char temp[PROP_NAME_LENGTH];
  sprintf(temp, "Point%d.x", state->point_number+1);
  create_object_prop(state->current_object, temp);
  state->prop_value_and_expr_of_x_pointer = &state->current_object->prop_tail->value_and_expr;
  sprintf(temp, "Point%d.y", state->point_number+1);
  create_object_prop(state->current_object, temp);
  state->prop_value_and_expr_of_y_pointer = &state->current_object->prop_tail->value_and_expr;

  /*移到handle_u,handle_v
  if (state->current_object->object_tag == TAG_SHAPE)
  {
    sprintf(temp, "Point%d.u", state->point_number+1);
    create_object_prop(state->current_object, temp);
    state->prop_value_and_expr_of_u_pointer = &state->current_object->prop_tail->value_and_expr;
    sprintf(temp, "Point%d.v", state->point_number+1);
    create_object_prop(state->current_object, temp);
    state->prop_value_and_expr_of_v_pointer = &state->current_object->prop_tail->value_and_expr;
  }*/
  state->point_number++;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_x(parsing_state_t *state)
{
  state->prop_value_and_expr_pointer = state->prop_value_and_expr_of_x_pointer;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_y(parsing_state_t *state)
{
  state->prop_value_and_expr_pointer = state->prop_value_and_expr_of_y_pointer;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_u(parsing_state_t *state)
{
  char temp[PROP_NAME_LENGTH];
  sprintf(temp, "Point%d.u", state->point_number+1);
  create_object_prop(state->current_object, temp);
  state->prop_value_and_expr_of_u_pointer = &state->current_object->prop_tail->value_and_expr;

  state->prop_value_and_expr_pointer = state->prop_value_and_expr_of_u_pointer;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_v(parsing_state_t *state)
{
  char temp[PROP_NAME_LENGTH];
  sprintf(temp, "Point%d.v", state->point_number+1);
  create_object_prop(state->current_object, temp);
  state->prop_value_and_expr_of_v_pointer = &state->current_object->prop_tail->value_and_expr;

  state->prop_value_and_expr_pointer = state->prop_value_and_expr_of_v_pointer;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_arc_segment(parsing_state_t *state)
{
  char temp[PROP_NAME_LENGTH];
  state->is_arc_segment = true;
  sprintf(temp, "Arc segment%d.angle", state->point_number);
  create_object_prop(state->current_object, temp);
  state->prop_value_and_expr_pointer = &state->current_object->prop_tail->value_and_expr;
  sprintf(temp, "Arc segment%d.orientation", state->point_number);
  create_object_prop(state->current_object, temp);
  state->prop_value_and_expr_of_y_pointer = &state->current_object->prop_tail->value_and_expr;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_first_point(parsing_state_t *state)
{
  create_object_prop(state->current_object, "First point.x");
  state->prop_value_and_expr_of_x_pointer = &state->current_object->prop_tail->value_and_expr;
  create_object_prop(state->current_object, "First point.y");
  state->prop_value_and_expr_of_y_pointer = &state->current_object->prop_tail->value_and_expr;

  if (state->current_object->object_tag == TAG_SHAPE)
  {
    create_object_prop(state->current_object, "First point.u");
    state->prop_value_and_expr_of_u_pointer = &state->current_object->prop_tail->value_and_expr;
    create_object_prop(state->current_object, "First point.v");
    state->prop_value_and_expr_of_v_pointer = &state->current_object->prop_tail->value_and_expr;
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_third_point(parsing_state_t *state)
{
  create_object_prop(state->current_object, "Third point.x");
  state->prop_value_and_expr_of_x_pointer = &state->current_object->prop_tail->value_and_expr;
  create_object_prop(state->current_object, "Third point.y");
  state->prop_value_and_expr_of_y_pointer = &state->current_object->prop_tail->value_and_expr;

  if (state->current_object->object_tag == TAG_SHAPE)
  {
    create_object_prop(state->current_object, "Third point.u");
    state->prop_value_and_expr_of_u_pointer = &state->current_object->prop_tail->value_and_expr;
    create_object_prop(state->current_object, "Third point.v");
    state->prop_value_and_expr_of_v_pointer = &state->current_object->prop_tail->value_and_expr;
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_first_arc(parsing_state_t *state)
{
  state->is_arc_segment = true;
  create_object_prop(state->current_object, "First arc.angle");
  state->prop_value_and_expr_pointer = &state->current_object->prop_tail->value_and_expr;
  create_object_prop(state->current_object, "First arc.orientation");
  state->prop_value_and_expr_of_y_pointer = &state->current_object->prop_tail->value_and_expr;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_second_arc(parsing_state_t *state)
{
  state->is_arc_segment = true;
  create_object_prop(state->current_object, "Second arc.angle");
  state->prop_value_and_expr_pointer = &state->current_object->prop_tail->value_and_expr;
  create_object_prop(state->current_object, "Second arc.orientation");
  state->prop_value_and_expr_of_y_pointer = &state->current_object->prop_tail->value_and_expr;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_third_arc(parsing_state_t *state)
{
  state->is_arc_segment = true;
  create_object_prop(state->current_object, "Third arc.angle");
  state->prop_value_and_expr_pointer = &state->current_object->prop_tail->value_and_expr;
  create_object_prop(state->current_object, "Third arc.orientation");
  state->prop_value_and_expr_of_y_pointer = &state->current_object->prop_tail->value_and_expr;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_fourth_arc(parsing_state_t *state)
{
  state->is_arc_segment = true;
  create_object_prop(state->current_object, "Fourth arc.angle");
  state->prop_value_and_expr_pointer = &state->current_object->prop_tail->value_and_expr;
  create_object_prop(state->current_object, "Fourth arc.orientation");
  state->prop_value_and_expr_of_y_pointer = &state->current_object->prop_tail->value_and_expr;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_line_width(parsing_state_t *state)
{
  static char *line_width = "Line width";
  static char *font_line_width = "Font line width";
  char *name;
  if (state->current_object->object_tag == TAG_TEXT)
    name = font_line_width;
  else
    name = line_width;
  create_object_prop(state->current_object, name);
  state->prop_value_and_expr_pointer = &state->current_object->prop_tail->value_and_expr;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_texture(LPXMLVECTOR atts, parsing_state_t *state)
{
  LPXMLRUNTIMEATT att;
  int i;
  static char temp[MAX_TEXT_BUFFER];

  temp[0] = 0;
  for (i = 0; i < atts->length; i++)
  {
    att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
    sprintf(temp + strlen(temp), "%s=\"%s\" ", (char*)att->qname, (char *)att->value);
  }
  create_object_prop(state->current_object, "Texture");
  create_string_item(&state->current_object->prop_tail->value_and_expr.value, temp);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_ratio_prop(parsing_state_t *state, char *ratio_horizontal, char *ratio_vertical)
{
  create_object_prop(state->current_object, "Ratio.x");
  create_string_item(&state->current_object->prop_tail->value_and_expr.value, ratio_horizontal);
  create_object_prop(state->current_object, "Ratio.y");
  create_string_item(&state->current_object->prop_tail->value_and_expr.value, ratio_vertical);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_index_and_show_all(LPXMLVECTOR atts, parsing_state_t *state)
{
  LPXMLRUNTIMEATT att;
  int i;

  for (i = 0; i < atts->length; i++)
  {
    att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
    if (strcmp((const char*)att->qname, "allVisible") == 0)
    {
      create_object_prop(state->current_object, "Show all");
      create_string_item(&state->current_object->prop_tail->value_and_expr.value, (char *)att->value);
      break;
    }
  }
  create_object_prop(state->current_object, "Index");
  state->prop_value_and_expr_pointer = &state->current_object->prop_tail->value_and_expr;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void handle_format(LPXMLVECTOR atts, parsing_state_t *state)
{
  LPXMLRUNTIMEATT att;
  int i;

  for (i = 0; i < atts->length; i++)
  {
    att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
    if (strcmp((const char*)att->qname, "separator") == 0)
    {
      create_object_prop(state->current_object, "Format separator");
      create_string_item(&state->current_object->prop_tail->value_and_expr.value, (char *)att->value);
      break;
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
void handle_inactivemasks(LPXMLVECTOR atts, parsing_state_t *state)
{
  LPXMLRUNTIMEATT att;
  int i;

  for (i = 0; i < atts->length; i++)
  {
    att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
    if (strcmp((const char*)att->qname, "oid") == 0)
    {
      object_t *o = search_object_by_oid((char *)att->value);
      if (o)
      {
        adjust_mem((void **)&state->current_object->inactive_mask_list, 
          &state->current_object->inactive_mask_count, 
          &state->current_object->inactive_mask_list_size,
          INACTIVE_MASK_LIST_BASE_SIZE, sizeof(object_t *));
        state->current_object->inactive_mask_list[state->current_object->inactive_mask_count++] = o;
      }
      else
      {
        fatal_error("internal error: handle_inactivemasks: %s is not found\n", (char *)att->value);
      }
      break;
    }
  }
}
