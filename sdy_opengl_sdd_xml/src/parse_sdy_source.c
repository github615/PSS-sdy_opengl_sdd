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
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsifal.h"
#include "sdy_config.h"
#include "sdy_object.h"
#include "sdy_source.h"
#include "process_file_and_dir.h"
#include "process_sdy_code.h"
#include "scade_display_dir.h"

/* --------------------------- 宏定义 -------------------------------*/
#define MASK_STACK_SIZE 50

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/
const str_and_enum_t str_and_tag_list[] =
{
  {"visible", TAG_VISIBLE},
  {"origin", TAG_ORIGIN},
  {"x", TAG_X},
  {"y", TAG_Y},
  {"u", TAG_U},
  {"v", TAG_V},
  {"priority", TAG_PRIORITY},
  {"scale", TAG_SCALE},
  {"rotate", TAG_ROTATE},
  {"angle", TAG_ANGLE},
  {"orientation", TAG_ORIENTATION},
  {"static", TAG_STATIC}, //SDY R16
  {"maskActivity", TAG_MASK_ACTIVITY},
  {"points", TAG_POINTS},
  {"point", TAG_POINT},
  {"arcSegment", TAG_ARCSEGMENT},
  {"firstPoint", TAG_FIRST_POINT},
  {"thirdPoint", TAG_THIRD_POINT},
  {"firstArc", TAG_FIRST_ARC},
  {"secondArc", TAG_SECOND_ARC},
  {"thirdArc", TAG_THIRD_ARC},
  {"fourthArc", TAG_FOURTH_ARC},
  {"lineWidth", TAG_LINE_WIDTH},
  {"lineStipple", TAG_LINE_STIPPLE},
  {"lineCap", TAG_LINE_CAP},
  {"haloColor", TAG_HALOCOLOR},
  {"haloing", TAG_HALOING},
  {"outlineColor", TAG_OUTLINE_COLOR},
  {"outlineOpacity", TAG_OUTLINE_OPACITY},
  {"fillColor", TAG_FILL_COLOR},
  {"fillOpacity", TAG_FILL_OPACITY},
  {"polygonSmooth", TAG_POLYGON_SMOOTH},
  {"texture", TAG_TEXTURE},
  {"textureId", TAG_TEXTURE_ID},
  {"position", TAG_POSITION},
  {"maxLength", TAG_MAX_LENGTH},
  {"textValue", TAG_TEXT_VALUE},
  {"font", TAG_FONT},
  {"horizAlign", TAG_HORIZ_ALIGN},
  {"vertAlign", TAG_VERT_ALIGN},
  {"id", TAG_ID},
  {"clipStartPoint", TAG_CLIP_START_POINT},
  {"clipAngle", TAG_CLIP_ANGLE},
  {"clipInside", TAG_CLIP_INSIDE},
  {"refPoint", TAG_REFPOINT},
  {"startRotationAngle", TAG_START_ROTATION_ANGLE},
  {"endRotationAngle", TAG_END_ROTATION_ANGLE},
  {"startRotationValue", TAG_START_ROTATION_VALUE},
  {"endRotationValue", TAG_END_ROTATION_VALUE},
  {"startRotationLocked", TAG_START_ROTATION_LOCKED},
  {"endRotationLocked", TAG_END_ROTATION_LOCKED},
  {"functionalRotationValue", TAG_FUNCTIONAL_ROTATION_VALUE},
  {"startTranslationPoint", TAG_START_TRANSLATION_POINT},
  {"endTranslationPoint", TAG_END_TRANSLATION_POINT},
  {"startTranslationValue", TAG_START_TRANSLATION_VALUE},
  {"endTranslationValue", TAG_END_TRANSLATION_VALUE},
  {"startTranslationLocked", TAG_START_TRANSLATION_LOCKED},
  {"endTranslationLocked", TAG_END_TRANSLATION_LOCKED},
  {"functionalTranslationValue", TAG_FUNCTIONAL_TRANSLATION_VALUE},
  {"value", TAG_VALUE},
  {"format", TAG_FORMAT},
  {"integralPart", TAG_INTEGRAL_PART},
  {"fractionalPart", TAG_FRACTIONAL_PART},
  {"secondFontPos", TAG_SECOND_FONT_POS},
  {"leadingZeros", TAG_LEADING_ZEROS},
  {"displaySign", TAG_DISPLAY_SIGN},
  {"firstLineWidth", TAG_FIRST_LINE_WIDTH},
  {"firstFont", TAG_FIRST_FONT},
  {"secondLineWidth", TAG_SECOND_LINE_WIDTH},
  {"secondFont", TAG_SECOND_FONT},
  {"eventId", TAG_EVENT_ID},
  {"inside", TAG_INSIDE},
  {"pressed", TAG_PRESSED},
  {"released", TAG_RELEASED},
  {"button", TAG_BUTTON},
  {"modifiers", TAG_MODIFIERS},
  {"textureControl", TAG_TEXTURE_CONTROL},
  {"startAngle", TAG_START_ANGLE},
  {"endAngle", TAG_END_ANGLE},
  {"center", TAG_CENTER},
  {"radius", TAG_RADIUS},
  {"interp", TAG_INTERP},
  {"multiline", TAG_MULTILINE},
  {"thickness", TAG_THICKNESS},
  {"refAngle", TAG_REFANGLE},
  {"enable", TAG_ENABLE},
  {"memory", TAG_MEMORY},
  {"restoreStates", TAG_RESTORE_STATES},
  {"height", TAG_HEIGHT},
  {"width", TAG_WIDTH},
  {"keyCode", TAG_KEYCODE},
  {"pointerId", TAG_POINTER_ID},
  {"percentHeight", TAG_PERCENT_HEIGHT},
  {"percentWidth", TAG_PERCENT_WIDTH},
  {"percent", TAG_PERCENT},
  {"inactiveMasks", TAG_INACTIVEMASKS},
  {"mask", TAG_MASK},
  {"modulate", TAG_MODULATE},
  {"tessellate", TAG_TESSELLATE},
  {"horzRadius", TAG_HORZ_RADIUS},
  {"vertRadius", TAG_VERT_RADIUS},
  //{"", TAG_},

  {"IDE", TAG_IDE},
  {"project", TAG_PROJECT},
  {"environment_id", TAG_ENVIRONMENT_ID},
  {"export", TAG_EXPORT},
  {"codeIntegration", TAG_CODEINTEGRATION},
  {"scade", TAG_SCADE},
  {"animation", TAG_ANIMATION},
  {"symbology", TAG_SYMBOLOGY},
  {"ratio", TAG_RATIO},
  {"dimension", TAG_DIMENSION},
  {"colorMode", TAG_COLOR_MODE},
  {"requirements", TAG_REQUIREMENTS},
  {"declaration", TAG_DECLARATION},
  {"inputs", TAG_INPUTS},
  {"outputs", TAG_OUTPUTS},
  {"crc", TAG_CRC},
  {"codegen", TAG_CODEGEN},
  {"include", TAG_INCLUDE},
  {"source", TAG_SOURCE},
  {"path", TAG_PATH},
  {"constants", TAG_CONSTANTS},

  {"children", TAG_CHILDREN},
  {"properties", TAG_PROPERTIES},
  {"comment", TAG_COMMENT},
  {"pathFromRoot", TAG_PATH_FROM_ROOT},

  {"referenceContainer", TAG_REFERENCE_CONTAINER},
  {"nplicatorContainer", TAG_NPLICATOR_CONTAINER},
  {"file", TAG_FILE},

  {"container", TAG_CONTAINER},
  {"translationContainer", TAG_TRANSLATION_CONTAINER},
  {"condContainer", TAG_COND_CONTAINER},
  {"panelContainer", TAG_PANEL_CONTAINER},
  {"rotationContainer", TAG_ROTATION_CONTAINER},
  {"filterTranslationContainer", TAG_FILTER_TRANSLATION_CONTAINER},
  {"filterRotationContainer", TAG_FILTER_ROTATION_CONTAINER},
  {"clipPlane", TAG_CLIP_PLANE},
  {"clipBox", TAG_CLIP_BOX},
  {"stencil", TAG_STENCIL},

  {"rectangle", TAG_RECTANGLE},
  {"rectangleArea", TAG_RECTANGLE_AREA},
  {"line", TAG_LINE},
  {"arc", TAG_ARC},
  {"arcEllipse", TAG_ARCELLIPSE},
  {"circle", TAG_CIRCLE},
  {"circleArea", TAG_CIRCLE_AREA},
  {"crown", TAG_CROWN},
  {"shape", TAG_SHAPE},
  {"ellipse", TAG_ELLIPSE},
  {"text", TAG_TEXT},
  {"textArea", TAG_TEXT_AREA},
  {"richText", TAG_RICHTEXT},
  {"biFont", TAG_BIFONT},
  {"bitmap", TAG_BITMAP},
  {"pointerEventListener", TAG_POINTER_EVENT_LISTENER},
  {"keyboardEventListener", TAG_KEYBOARD_EVENT_LISTENER},
  {"imported", TAG_IMPORTED},
  {"traceability", TAG_TRACEABILITY},
  {"preview", TAG_PREVIEW},
  {"raw", TAG_RAW},
  {"hook", TAG_HOOK},
  {"function", TAG_FUNCTION},

  {"expr", TAG_EXPR},
  {"index", TAG_INDEX},
  {"indexes", TAG_INDEXES},
  {"inputParameters", TAG_INPUT_PARAMETERS},
  {"outputParameters", TAG_OUTPUT_PARAMETERS},
  {"parameter", TAG_PARAMETER},

  {"init", TAG_INIT},
  {"input", TAG_INPUT},
  {"output", TAG_OUTPUT},
  {"local", TAG_LOCAL},
  {"localConstant", TAG_LOCAL_CONSTANT},
  {"constant", TAG_CONSTANT},
  {"control", TAG_CONTROL},

  {"layer", TAG_LAYER},
  {"specification", TAG_SPECIFICATION},
  {"object", TAG_OBJECT},
  {NULL, },
};

int object_id = 0;
object_t *root;

int object_count_for_main_source = 0;
int object_count = 0;
 object_t **object_list = NULL;
static int object_list_size = 0;

int layer_info_count;
layer_info_t *layer_info_list;
static int layer_info_list_size = 0;

int global_ogfx_info_count = 0;
 ogfx_info_t *global_ogfx_info_list = NULL;
static int global_ogfx_info_list_size = 0;

int variable_dictionary_info_count = 0;
 variable_dictionary_info_t *variable_dictionary_info_list = NULL;
static int variable_dictionary_info_list_size;

int ref_parameter_info_count = 0;
 ref_parameter_info_t *ref_parameter_info_list = NULL;
static int ref_parameter_info_list_size;

int indexes_count = 0;

int codegen_include_path_count = 0;
 char **codegen_include_path_list = NULL;
static int codegen_include_path_list_size = 0;

int codegen_source_path_count = 0;
 char **codegen_source_path_list = NULL;
static int codegen_source_path_list_size = 0;

static char requirements_trace_file_name[FILE_NAME_LENGTH];

char sgfx_ratio_horizontal[ATTR_LENGTH];//2018-12-7
char sgfx_ratio_vertical[ATTR_LENGTH];//2018-12-7
char sgfx_dimension_height[ATTR_LENGTH];//2018-12-7
char sgfx_dimension_width[ATTR_LENGTH];//2018-12-7

/* --------------------------- 函数定义 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void compose_full_name_with_slash(object_t *object)
{
  size_t l = strlen(object->object_name);
  if (object->father_list[0]->object_tag == TAG_REFERENCE_CONTAINER ||
      object->father_list[0]->object_tag == TAG_NPLICATOR_CONTAINER ||
      object->father_list[0]->full_name_with_slash == NULL)
  {
    object->full_name_with_slash = (char *)get_mem(l+1);
    strcpy(object->full_name_with_slash, object->object_name);
  }
  else
  {
    l = l + strlen(object->father_list[0]->full_name_with_slash) + 2;
    object->full_name_with_slash = (char *)get_mem(l);
    sprintf(object->full_name_with_slash, "%s/%s", 
      object->father_list[0]->full_name_with_slash, object->object_name);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static object_t *create_object(sdy_source_tag_t object_tag, object_t *father, int variable_dictionary_scope_id)
{
  object_t *object = (object_t *)get_cmem(sizeof(object_t));
  object->comment = NULL;
  object->oid = NULL;
  object->variable_dictionary_scope_id = variable_dictionary_scope_id;
  object->id = object_id++;
  object->object_tag = object_tag;
  adjust_mem((void **)&object->father_list, &object->father_count, &object->father_list_size, FATHER_LIST_BASE_SIZE, sizeof(object_t *));
  object->father_list[object->father_count++] = father;
  object->expected_condition_index = INDEXES_OTHER;
  object->first_input_ref_parameter_info_id = -1;
  object->first_output_ref_parameter_info_id = -1;
  object->object_class = NO_SGL_CODE;

  if (father)
  {
    if (father->son == NULL)
    {
      father->son = object;
    }
    else
    {
      object_t *last_son = father->son;
      while (last_son->next_sibling) last_son = last_son->next_sibling;
      last_son->next_sibling = object;
      object->prev_sibling = last_son;
    }
  }
  adjust_mem((void **)&object_list, &object_count, &object_list_size, OBJECT_LIST_BASE_SIZE, sizeof(object_t *));
  object_list[object_count++] = object;
  return object;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void set_first_input_ref_parameter_info_id(object_t *o)
{
  o->first_input_ref_parameter_info_id = ref_parameter_info_count;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void set_first_output_ref_parameter_info_id(object_t *o)
{
  o->first_output_ref_parameter_info_id = ref_parameter_info_count;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void create_variable_dictionary_info(int scope_id, 
  char *memory, 
  char *name,
  char *type,
  char *oid, 
  char *representation,
  char *group,
  char *control,
  init_and_expr_scope_t init_and_expr_scope)
{
  variable_dictionary_info_t *info;
  adjust_mem((void **)&variable_dictionary_info_list, &variable_dictionary_info_count, &variable_dictionary_info_list_size, INPUT_OUTPUT_INFO_LIST_BASE_SIZE, sizeof(variable_dictionary_info_t));
  info = &variable_dictionary_info_list[variable_dictionary_info_count];
  info->variable_dictionary_scope_id = scope_id;
  info->init_value = NULL;
  info->control_plug = NULL;
  info->trace[0] = '\0';
  info->init_and_expr_scope = init_and_expr_scope;
  strcpy(info->memory, memory);
  strcpy(info->representation, representation);
  strcpy(info->variable_name, name);
  strcpy(info->variable_type, type);
  strcpy(info->variable_oid, oid);
  strcpy(info->variable_group, group);
  strcpy(info->variable_control, group);
  variable_dictionary_info_count++;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void set_variable_dictionary_info_init_value(char *init_value)
{
  create_string_item(&variable_dictionary_info_list[variable_dictionary_info_count - 1].init_value, init_value);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void set_variable_dictionary_info_control_plug(LPXMLVECTOR atts, parsing_state_t *state)
{
  LPXMLRUNTIMEATT att;
  int i;

  for (i = 0; i < atts->length; i++)
  {
    att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
    if (strcmp((const char *)att->qname, "plug") == 0)
    {
      create_string_item(&variable_dictionary_info_list[variable_dictionary_info_count - 1].control_plug, (char *)att->value);
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
static int read_xml_file(BYTE *buf, int cbytes, int *cbytes_actual, void *input_data)
{
  int retval;
  FILE *xml_file = (FILE *)input_data;

  *cbytes_actual = (int)fread(buf, 1, cbytes, (FILE *)input_data); 
  if (ferror(xml_file))
  {
    retval = BIS_ERR_INPUT;
  }
  else
  {
    if (feof(xml_file))
    {
      retval = BIS_EOF;
    }
    else
    {
      retval = XML_OK;
    }
  }
  return (retval);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static sdy_source_tag_t convert_to_tag(char *el)
{
  return (sdy_source_tag_t)convert_str_to_enum(el, str_and_tag_list, STRCMP);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void handle_ratio(LPXMLVECTOR atts, parsing_state_t *state)
{
  LPXMLRUNTIMEATT att;
  int i;
  char *ratio_horizontal;
  char *ratio_vertical;

  for (i = 0; i < atts->length; i++)
  {
    att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
    if (strcmp((const char *)att->qname, "horizontal") == 0)
    {
      //2018-12-7
      ratio_horizontal = (char *)att->value;
      if (state->is_layer)
      {
        strcpy(layer_info_list[layer_info_count - 1].ratio_horizontal, (char *)att->value);
      }
      else if (NULL == state->father_object) //current is sgfx
      {
        strcpy(sgfx_ratio_horizontal, (char *)att->value);
      }
    }
    else if (strcmp((const char *)att->qname, "vertical") == 0)
    {
      //2018-12-7
      ratio_vertical = (char *)att->value;
      if (state->is_layer)
      {
        strcpy(layer_info_list[layer_info_count - 1].ratio_vertical, (char *)att->value);
      }
      else if (NULL == state->father_object) //current is sgfx
      {
        strcpy(sgfx_ratio_vertical, (char *)att->value);
      }
    }
  }
  handle_ratio_prop(state, ratio_horizontal, ratio_vertical);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void handle_dimension(LPXMLVECTOR atts, parsing_state_t *state)
{
  LPXMLRUNTIMEATT att;
  int i;

  for (i = 0; i < atts->length; i++)
  {
    att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
    if (strcmp((const char *)att->qname, "height") == 0)
    {
      //2018-12-7
      if (state->is_layer)
      {
        strcpy(layer_info_list[layer_info_count - 1].dimension_height, (char *)att->value);
      }
      else if (NULL == state->father_object) //current is sgfx
      {
        strcpy(sgfx_dimension_height, (char *)att->value);
      }
    }
    else if (strcmp((const char *)att->qname, "width") == 0)
    {
      //2018-12-7
      if (state->is_layer)
      {
        strcpy(layer_info_list[layer_info_count - 1].dimension_width, (char *)att->value);
      }
      else if (NULL == state->father_object) //current is sgfx
      {
        strcpy(sgfx_dimension_width, (char *)att->value);
      }
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
static void handle_environment_id(LPXMLVECTOR atts, parsing_state_t *state)
{
  LPXMLRUNTIMEATT att;
  int i;

  for (i = 0; i < atts->length; i++)
  {
    att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
    if (strcmp((const char *)att->qname, "value") == 0)
    {
      save_environment_id((char *)att->value);
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
static void handle_color_mode(LPXMLVECTOR atts, parsing_state_t *state)
{
  LPXMLRUNTIMEATT att;
  int i;

  for (i = 0; i < atts->length; i++)
  {
    att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
    if (strcmp((const char *)att->qname, "value") == 0)
    {
      save_color_mode((char *)att->value);
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
static void handle_children_start(parsing_state_t *state)
{
  state->father_object = state->current_object;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void handle_children_end(parsing_state_t *state)
{
  state->father_object = state->father_object->father_list[0];
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void handle_properties(LPXMLVECTOR atts, parsing_state_t *state)
{
  LPXMLRUNTIMEATT att;
  int i;

  state->init_and_expr_scope = SCOPE_PROPERTIES;
  for (i = 0; i < atts->length; i++)
  {
    att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
    if (strcmp((const char *)att->qname, "name") == 0)
    {
      strcpy(state->current_object->object_name, (const char *)att->value);
      compose_full_name_with_slash(state->current_object);
    }
    else if (strcmp((const char *)att->qname, "modular") == 0)
    {
      create_object_prop(state->current_object, "Modular");
      create_string_item(&state->current_object->prop_tail->value_and_expr.value, (char *)att->value);
    }
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *  以下property没有init，因此把name当作init
 *  <function name="undefined"/>
 *
 **********************************************************************
*/
static void handle_object_prop_name(LPXMLVECTOR atts, parsing_state_t *state)
{
  LPXMLRUNTIMEATT att;
  int i;

  for (i = 0; i < atts->length; i++)
  {
    att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
    if (strcmp((const char *)att->qname, "name") == 0)
    {
      create_string_item(&state->prop_value_and_expr_pointer->value, (char *)att->value);
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
static void save_ogfx_info(parsing_state_t *state, char *file_name, char *path_name)
{
  char *p;
  ogfx_info_t *ogfx_info;
  adjust_mem((void **)&state->ogfx_info_buffer, &state->ogfx_info_count, &state->ogfx_info_buffer_size, OGFX_INFO_BUFFER_BASE_SIZE, sizeof(ogfx_info_t));
  ogfx_info = &state->ogfx_info_buffer[state->ogfx_info_count++];
  if (strcmp(path_name, ".") == 0)
  {
    strcpy(ogfx_info->file_name, file_name);
  }
  else
  {
    p = path_name;
    while (*p)
    {
      if ((*p) == '/') (*p) = '\\';
      p++;
    }
    sprintf(ogfx_info->file_name, "%s\\%s", path_name, file_name);
  }
  strcpy(ogfx_info->ogfx_name, file_name);
  if (p = strstr(ogfx_info->ogfx_name, ".ogfx"))
  {
    *p = 0;
  }
  ogfx_info->ogfx_root = state->current_object;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void handle_file(LPXMLVECTOR atts, parsing_state_t *state)
{
  LPXMLRUNTIMEATT att;
  int i;
  char file_name[FILE_NAME_LENGTH];
  char path_name[FILE_NAME_LENGTH];

  for (i = 0; i < atts->length; i++)
  {
    att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
    if (strcmp((const char *)att->qname, "name") == 0)
    {
      strcpy(file_name, (const char *)att->value);
    }
    else if (strcmp((const char *)att->qname, "path") == 0)
    {
      strcpy(path_name, (const char *)att->value);
    }
  }

  if (path_name[0] == '$') /*e.g. path="$(SCADEDISPLAY_HOME)\examples\objects\Scales"*/
  {
    char temp[ATTR_LENGTH];
    char new_path_name[FILE_NAME_LENGTH];
    char *p;
    strcpy(temp, path_name+2);
    p = strchr(temp, ')');
    if (!p)
    {
      fatal_error("Unknown path: %s\n", path_name);
    }
    else
    {
      *p = 0;
      sprintf(new_path_name, "%s%s", getenv(temp), p+1);
      strcpy(path_name, new_path_name);
    }
  }

  if (state->current_object->object_tag == TAG_REFERENCE_CONTAINER ||
      state->current_object->object_tag == TAG_NPLICATOR_CONTAINER)
  {
    save_ogfx_info(state, file_name, path_name);
  }
  else
  {
    printf("unknown tag file\n");
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void handle_specification(parsing_state_t *state)
{
  char oid[ATTR_LENGTH];

  sprintf(oid, "oid-%s", get_specification_name());
  root = create_object(TAG_SPECIFICATION, NULL, 0);
  root->level_number[0] = 0;
  strcpy(root->full_name_with_level_number, get_specification_name());
  strcpy(root->object_name, "symbology_layer");
  state->current_object = root;
  create_string_item(&root->oid, oid);
}
/**
 **********************************************************************
 *
 * @功能描述
 *  <object>出现于ogfx文件头部。
 *
 **********************************************************************
*/
static void handle_object(parsing_state_t *state)
{
  if (state->variable_dictionary_scope_id == 0) //这表示ogfx文件是主文件
  {
    char oid[ATTR_LENGTH];

    sprintf(oid, "oid-%s", get_specification_name());
    root = create_object(TAG_LAYER, NULL, 0);
    root->level_number[0] = 0;
    strcpy(root->full_name_with_level_number, get_specification_name());
    strcpy(root->object_name, "symbology_layer");
    state->current_object = root;
    create_string_item(&root->oid, oid);

    adjust_mem((void **)&layer_info_list, &layer_info_count, &layer_info_list_size, LAYER_LIST_BASE_SIZE, sizeof(layer_info_t));
    layer_info_list[layer_info_count].this_variable_dictionary_info_id = variable_dictionary_info_count;
    layer_info_list[layer_info_count].next_variable_dictionary_info_id = -1;
    layer_info_list[layer_info_count++].object_id = state->current_object->id;
  }
  else
  {
    state->current_object = state->father_object;
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void handle_oid(LPXMLVECTOR atts, parsing_state_t *state)
{
  LPXMLRUNTIMEATT att;
  int i;

  for (i = 0; i < atts->length; i++)
  {
    att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
    if (strcmp((const char *)att->qname, "oid") == 0)
    {
      create_string_item(&state->current_object->oid, (char *)att->value);
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
static void handle_layer(LPXMLVECTOR atts, parsing_state_t *state)
{
  state->current_object = create_object(TAG_LAYER, state->father_object, state->variable_dictionary_scope_id);
  state->is_layer = true;
  adjust_mem((void **)&layer_info_list, &layer_info_count, &layer_info_list_size, LAYER_LIST_BASE_SIZE, sizeof(layer_info_t));
  layer_info_list[layer_info_count].this_variable_dictionary_info_id = variable_dictionary_info_count;
  layer_info_list[layer_info_count].next_variable_dictionary_info_id = -1;
  layer_info_list[layer_info_count++].object_id = state->current_object->id;
  handle_oid(atts, state);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void handle_containers(sdy_source_tag_t tag, LPXMLVECTOR atts, parsing_state_t *state)
{
  state->current_object = create_object(tag, state->father_object, state->variable_dictionary_scope_id);
  handle_oid(atts, state);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void handle_masks(sdy_source_tag_t tag, LPXMLVECTOR atts, parsing_state_t *state)
{
  state->current_object = create_object(tag, state->father_object, state->variable_dictionary_scope_id);
  handle_oid(atts, state);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void handle_primitives(sdy_source_tag_t tag, LPXMLVECTOR atts, parsing_state_t *state)
{
  state->current_object = create_object(tag, state->father_object, state->variable_dictionary_scope_id);
  handle_oid(atts, state);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void handle_condition_var_index(parsing_state_t *state)
{
  indexes_info_t *indexes = (indexes_info_t *)get_cmem(sizeof(indexes_info_t));

  state->current_object->indexes_info = indexes;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void handle_indexes(LPXMLVECTOR atts, parsing_state_t *state)
{
  LPXMLRUNTIMEATT att;
  int i;
  indexes_info_t *indexes = state->current_object->indexes_info;
  state->is_condition_indexes = state->current_object->object_tag == TAG_COND_CONTAINER;

  for (i = 0; i < atts->length; i++)
  {
    att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
    if (strcmp((const char *)att->qname, "default") == 0)
    {
      if (strcmp((const char *)att->value, "other") == 0)
      {
        indexes->default_index = INDEXES_OTHER;
      }
      else
      {
        sscanf((const char *)att->value, "%d", &indexes->default_index);
      }
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
static void handle_text_value(LPXMLVECTOR atts, parsing_state_t *state)
{
  LPXMLRUNTIMEATT att;
  int i;

  for (i = 0; i < atts->length; i++)
  {
    att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
    if (strcmp((const char *)att->qname, "type") == 0)
    {
      handle_object_prop(state, "Text representation");
      create_string_item(&state->prop_value_and_expr_pointer->value, (char *)att->value);
    }
  }
  handle_object_prop(state, "Text value");
  state->is_text_value = true;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void set_condition_index_var_init(parsing_state_t *state)
{
  indexes_info_t *indexes = state->current_object->indexes_info;
  if (strcmp(state->character_data, "other") == 0)
  {
    indexes->index_init_type = CONDITION_INDEX_INIT_OTHER;
  }
  else if (strcmp(state->character_data, "default") == 0)
  {
    indexes->index_init_type = CONDITION_INDEX_INIT_DEFAULT;
  }
  else
  {
    indexes->index_init_type = CONDITION_INDEX_INIT_NORMAL_VALUE;
    sscanf(state->character_data, "%d", &indexes->index_init_value);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void handle_condition_case_index(LPXMLVECTOR atts, parsing_state_t *state)
{
  LPXMLRUNTIMEATT att;
  int i;
  indexes_info_t *indexes = state->current_object->indexes_info;

  for (i = 0; i < atts->length; i++)
  {
    att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
    if (strcmp((const char *)att->qname, "value") == 0)
    {
      int value;
      sscanf((const char *)att->value, "%d", &value);
      adjust_mem((void **)&indexes->condition_index_list, &indexes->condition_index_count, &indexes->condition_index_list_size, CONDITION_INDEX_LIST_BASE_SIZE, sizeof(int));
      indexes->condition_index_list[indexes->condition_index_count++] = value;
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
static void handle_declaration(LPXMLVECTOR atts, parsing_state_t *state, init_and_expr_scope_t init_and_expr_scope)
{
  if (state->is_declaration)
  {
    LPXMLRUNTIMEATT att;
    int i;
    char memory[VARIABLE_DICTIONARY_PROP_LENGTH];
    char name[VARIABLE_DICTIONARY_PROP_LENGTH];
    char type[VARIABLE_DICTIONARY_PROP_LENGTH];
    char oid[VARIABLE_DICTIONARY_PROP_LENGTH];
    char representation[VARIABLE_DICTIONARY_PROP_LENGTH];
    char group[VARIABLE_DICTIONARY_PROP_LENGTH];
    char control[VARIABLE_DICTIONARY_PROP_LENGTH];

    state->init_and_expr_scope = init_and_expr_scope;
    memory[0] = 0;
    group[0] = 0;
    control[0] = 0;
    strcpy(representation, "n/a");
    for (i = 0; i < atts->length; i++)
    {
      att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
      if (strcmp((const char *)att->qname, "name") == 0)
      {
        strcpy(name, (const char *)att->value);
      }
      else if (strcmp((const char *)att->qname, "type") == 0)
      {
        strcpy(type, (const char *)att->value);
      }
      else if (strcmp((const char *)att->qname, "oid") == 0)
      {
        strcpy(oid, (const char *)att->value);
      }
      else if (strcmp((const char *)att->qname, "memory") == 0)
      {
        strcpy(memory, (const char *)att->value);
      }
      else if (strcmp((const char *)att->qname, "representation") == 0)
      {
        strcpy(representation, (const char *)att->value);
      }
      else if (strcmp((const char *)att->qname, "group") == 0)
      {
        strcpy(group, (const char *)att->value);
      }
      else if (strcmp((const char *)att->qname, "control") == 0)
      {
        strcpy(control, (const char *)att->value);
      }
    }

    create_variable_dictionary_info(
      state->variable_dictionary_scope_id,
      memory,
      name,
      type,
      oid,
      representation,
      group,
      control,
      init_and_expr_scope);

    if (state->variable_dictionary_scope_id == 0 && layer_info_count > 0)
    {
      layer_info_list[layer_info_count-1].next_variable_dictionary_info_id = variable_dictionary_info_count;
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
static void handle_parameter(LPXMLVECTOR atts, parsing_state_t *state)
{
  LPXMLRUNTIMEATT att;
  int i;
  ref_parameter_info_t *info;

  adjust_mem(&ref_parameter_info_list, &ref_parameter_info_count, &ref_parameter_info_list_size, REF_PARAMETER_INFO_LIST_BASE_SIZE, sizeof(ref_parameter_info_t));
  info = &ref_parameter_info_list[ref_parameter_info_count];
  info->parameter_control[0] = 0;
  info->parameter_representation[0] = 0;
  for (i = 0; i < atts->length; i++)
  {
    att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
    if (strcmp((const char *)att->qname, "name") == 0)
    {
      strcpy(info->parameter_name, (const char *)att->value);
    }
    else if (strcmp((const char *)att->qname, "oid") == 0)
    {
      strcpy(info->parameter_oid, (const char *)att->value);
    }
    else if (strcmp((const char *)att->qname, "type") == 0)
    {
      strcpy(info->parameter_type, (const char *)att->value);
    }
    else if (strcmp((const char *)att->qname, "control") == 0)
    {
      strcpy(info->parameter_control, (const char *)att->value);
    }
    else if (strcmp((const char *)att->qname, "representation") == 0)
    {
      strcpy(info->parameter_representation, (const char *)att->value);
    }
  }
  info->ref_object = state->current_object;
  info->real_para_scope_id = -1;
  info->init_value = NULL;
  info->real_para_name = NULL;
  if (state->init_and_expr_scope == SCOPE_INPUTPARAMETERS)
  {
    state->current_object->input_ref_parameter_count++;
  }
  else if (state->init_and_expr_scope == SCOPE_OUTPUTPARAMETERS)
  {
    state->current_object->output_ref_parameter_count++;
  }
  ref_parameter_info_count++; //<init> or <expr> may not exist, so ref_parameter_info_count should be increased here
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void set_parameter_init(char *init_value)
{
  ref_parameter_info_t *info = &ref_parameter_info_list[ref_parameter_info_count-1];
  create_string_item(&info->init_value, init_value);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void set_parameter_expr(char *real_para_name, int real_para_scope_id)
{
  ref_parameter_info_t *info = &ref_parameter_info_list[ref_parameter_info_count-1];
  create_string_item(&info->real_para_name, real_para_name);
  info->real_para_scope_id = real_para_scope_id;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/

static void compose_requirements_trace_file_name(LPXMLVECTOR atts, parsing_state_t *state)
{
  LPXMLRUNTIMEATT att;
  int i;
  char map_name[FILE_NAME_LENGTH];
  char path_name[FILE_NAME_LENGTH];

  map_name[0] = path_name[0] = 0;
  for (i = 0; i < atts->length; i++)
  {
    att = (LPXMLRUNTIMEATT) XMLVector_Get(atts, i);
    if (strcmp((const char *)att->qname, "name") == 0)
    {
      strcpy(map_name, (const char *)att->value);
    }
    else if (strcmp((const char *)att->qname, "path") == 0)
    {
      strcpy(path_name, (const char *)att->value);
    }
  }

  sprintf(requirements_trace_file_name, "%s\\%s", state->sdy_source_path, path_name);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static int
start_element_handler (void *user_data, const XMLCH *uri, const XMLCH *name, 
                       const XMLCH *el, LPXMLVECTOR atts)
{
  parsing_state_t *state = (parsing_state_t *)user_data;
  sdy_source_tag_t tag = convert_to_tag((char *)el);

  state->character_data[0] = '\0';
  switch (tag)
  {
    case TAG_IDE:
      state->is_ide = true;
      break;
    case TAG_CODEGEN:
      state->is_codegen_include_path++;
      state->is_codegen_source_path++;
      break;
    case TAG_INCLUDE:
      state->is_codegen_include_path++;
      break;
    case TAG_SOURCE:
      state->is_codegen_source_path++;
      break;
    case TAG_PATH:
      if (state->is_codegen_include_path == 2)
      {
        state->is_codegen_include_path++;
      }
      else if (state->is_codegen_source_path == 2)
      {
        state->is_codegen_source_path++;
      }
      break;
    case TAG_ENVIRONMENT_ID:
      handle_environment_id(atts, state);
      break;
    case TAG_DECLARATION:
      state->is_declaration = true;
      break;
    case TAG_RATIO:
      if ((state->is_ide || state->is_layer) && state->variable_dictionary_scope_id == 0)
      {
        handle_ratio(atts, state);
      }
      break;
    case TAG_DIMENSION:
      if (state->variable_dictionary_scope_id == 0)
      {
        handle_dimension(atts, state);
      }
      break;
    case TAG_COLOR_MODE:
      handle_color_mode(atts, state);
      break;
    case TAG_CHILDREN:
      handle_children_start(state);
      break;
    case TAG_PROPERTIES:
      handle_properties(atts, state);
      break;
    case TAG_FILE:
      handle_file(atts, state);
      break;
    case TAG_SPECIFICATION:
      handle_specification(state);
      break;
    case TAG_OBJECT:
      handle_object(state);
      break;
    case TAG_LAYER:
      handle_layer(atts, state);
      break;
    case TAG_REFERENCE_CONTAINER:
    case TAG_CONTAINER:
    case TAG_TRANSLATION_CONTAINER:
    case TAG_COND_CONTAINER:
    case TAG_PANEL_CONTAINER:
    case TAG_ROTATION_CONTAINER:
    case TAG_FILTER_TRANSLATION_CONTAINER:
    case TAG_FILTER_ROTATION_CONTAINER:
    case TAG_NPLICATOR_CONTAINER:
      handle_containers(tag, atts, state);
      break;
    case TAG_CLIP_BOX:
    case TAG_CLIP_PLANE:
    case TAG_STENCIL:
      handle_masks(tag, atts, state);
      break;
    case TAG_ARC:
    case TAG_ARCELLIPSE:
    case TAG_BIFONT:
    case TAG_BITMAP:
    case TAG_LINE:
    case TAG_CIRCLE:
    case TAG_CIRCLE_AREA:
    case TAG_CROWN:
    case TAG_SHAPE:
    case TAG_ELLIPSE:
    case TAG_RICHTEXT:
    case TAG_RECTANGLE:
    case TAG_RECTANGLE_AREA:
    case TAG_TEXT:
    case TAG_TEXT_AREA:
    case TAG_POINTER_EVENT_LISTENER:
    case TAG_KEYBOARD_EVENT_LISTENER:
    case TAG_IMPORTED:
    case TAG_HOOK:
      handle_primitives(tag, atts, state);
      break;
    case TAG_INDEX:
      if (state->is_condition_indexes)
      {
        handle_condition_case_index(atts, state);
      }
      else
      {
        state->is_condition_index = state->current_object->object_tag == TAG_COND_CONTAINER;
        if (state->is_condition_index)
        {
          handle_condition_var_index(state);
        }
        handle_index_and_show_all(atts, state);
      }
      break;
    case TAG_INDEXES:
      handle_indexes(atts, state);
      break;
    case TAG_INPUT:
      handle_declaration(atts, state, SCOPE_INPUT);
      break;
    case TAG_OUTPUT:
      handle_declaration(atts, state, SCOPE_OUTPUT);
      break;
    case TAG_LOCAL:
      handle_declaration(atts, state, SCOPE_LOCAL);
      break;
    case TAG_LOCAL_CONSTANT:
      handle_declaration(atts, state, SCOPE_LOCAL_CONSTANT);
      break;
    case TAG_CONSTANT:
      handle_declaration(atts, state, SCOPE_CONSTANT);
      break;
    case TAG_CONTROL:
      if (state->init_and_expr_scope == SCOPE_INPUT ||
        state->init_and_expr_scope == SCOPE_OUTPUT ||
        state->init_and_expr_scope == SCOPE_CONSTANT
        )
      {
        set_variable_dictionary_info_control_plug(atts, state);
      }
      break;
    case TAG_INPUT_PARAMETERS:
      state->init_and_expr_scope = SCOPE_INPUTPARAMETERS;
      set_first_input_ref_parameter_info_id(state->current_object);
      break;
    case TAG_OUTPUT_PARAMETERS:
      state->init_and_expr_scope = SCOPE_OUTPUTPARAMETERS;
      set_first_output_ref_parameter_info_id(state->current_object);
      break;
    case TAG_PARAMETER:
      /*不需要此条件，以允许其他未知的TAG也能有parameter
      if (state->current_object->object_tag == TAG_REFERENCE_CONTAINER ||
          state->current_object->object_tag == TAG_NPLICATOR_CONTAINER ||
          state->current_object->object_tag == TAG_IMPORTED
          )*/
      {
        handle_parameter(atts, state);
      }
      break;
    case TAG_REQUIREMENTS:
      compose_requirements_trace_file_name(atts, state);
      break;
    /*below are for object property */
    case TAG_VISIBLE:
      handle_object_prop(state, "Visibility");
      break;
    case TAG_ORIGIN:
      handle_object_prop_xy(state, "Origin.x", "Origin.y");
      break;
    case TAG_SCALE:
      handle_object_prop_xy(state, "Horizontal scale", "Vertical scale");
      break;
    case TAG_ROTATE:
      handle_object_prop(state, "Rotate angle");
      break;
    case TAG_ORIENTATION:
      handle_orientation(atts, state);
      break;
    case TAG_STATIC:
      handle_static(atts, state);
      break;
    case TAG_MASK_ACTIVITY:
      handle_object_prop(state, "Mask activity");
      break;
    case TAG_POINTS:
      state->point_number = 0;
      break;
    case TAG_POINT:
      handle_point(state);
      break;
    case TAG_X:
      handle_x(state);
      break;
    case TAG_Y:
      handle_y(state);
      break;
    case TAG_U:
      handle_u(state);
      break;
    case TAG_V:
      handle_v(state);
      break;
    case TAG_PRIORITY:
      handle_object_prop(state, "Priority");
      break;
    case TAG_ARCSEGMENT:
      handle_arc_segment(state);
      break;
    case TAG_FIRST_POINT:
      handle_first_point(state);
      break;
    case TAG_THIRD_POINT:
      handle_third_point(state);
      break;
    case TAG_FIRST_ARC:
      handle_first_arc(state);
      break;
    case TAG_SECOND_ARC:
      handle_second_arc(state);
      break;
    case TAG_THIRD_ARC:
      handle_third_arc(state);
      break;
    case TAG_FOURTH_ARC:
      handle_fourth_arc(state);
      break;
    case TAG_LINE_WIDTH:
      handle_line_width(state);
      break;
    case TAG_LINE_STIPPLE:
      handle_object_prop(state, "Line stipple");
      break;
    case TAG_LINE_CAP:
      handle_object_prop(state, "Line cap");
      break;
    case TAG_OUTLINE_COLOR:
      handle_object_prop(state, "Outline color");
      break;
    case TAG_OUTLINE_OPACITY:
      handle_object_prop(state, "Outline opacity");
      break;
    case TAG_FILL_COLOR:
      handle_object_prop(state, "Filled color");
      break;
    case TAG_FILL_OPACITY:
      handle_object_prop(state, "Filled opacity");
      break;
    case TAG_POLYGON_SMOOTH:
      handle_object_prop(state, "Polygon smooth");
      break;
    case TAG_TEXTURE:
      handle_texture(atts, state);
      break;
    case TAG_TEXTURE_ID:
      handle_object_prop(state, "Texture ID");
      break;
    case TAG_HALOCOLOR:
      handle_object_prop(state, "Halo Color");
      break;
    case TAG_HALOING:
      handle_object_prop(state, "Haloing");
      break;
    case TAG_POSITION:
      handle_object_prop_xy(state, "Position.x", "Position.y");
      break;
    case TAG_TEXT_VALUE:
      handle_text_value(atts, state);
      break;
    case TAG_MAX_LENGTH:
      handle_object_prop(state, "Text maximum length");
      break;
    case TAG_FONT:
      handle_object_prop(state, "Font id");
     break;
    case TAG_HORIZ_ALIGN:
      handle_object_prop(state, "Horizontal alignment");
      break;
    case TAG_VERT_ALIGN:
      handle_object_prop(state, "Vertical alignment");
      break;
    case TAG_ID:
      handle_object_prop(state, "ID");
      break;
    case TAG_CLIP_START_POINT:
      handle_object_prop_xy(state, "Start.x", "Start.y");
      break;
    case TAG_CLIP_ANGLE:
      handle_object_prop(state, "Angle");
      break;
    case TAG_CLIP_INSIDE:
      handle_object_prop(state, "Clip inside");
      break;
    case TAG_REFPOINT:
      handle_object_prop_xy(state, "Reference point.x", "Reference point.y");
      break;
    case TAG_START_ROTATION_ANGLE:
      handle_object_prop(state, "Start rotation angle");
      break;
    case TAG_END_ROTATION_ANGLE:
      handle_object_prop(state, "End rotation angle");
      break;
    case TAG_START_ROTATION_VALUE:
      handle_object_prop(state, "Start rotation value");
      break;
    case TAG_END_ROTATION_VALUE:
      handle_object_prop(state, "End rotation value");
      break;
    case TAG_START_ROTATION_LOCKED:
      handle_object_prop(state, "Start rotation locked");
      break;
    case TAG_END_ROTATION_LOCKED:
      handle_object_prop(state, "End rotation locked");
      break;
    case TAG_FUNCTIONAL_ROTATION_VALUE:
      handle_object_prop(state, "Functional rotation value");
      break;
    case TAG_START_TRANSLATION_POINT:
      handle_object_prop_xy(state, "Start translation point.x", "Start translation point.y");
      break;
    case TAG_END_TRANSLATION_POINT:
      handle_object_prop_xy(state, "End translation point.x", "End translation point.y");
      break;
    case TAG_START_TRANSLATION_VALUE:
      handle_object_prop(state, "Start translation value");
      break;
    case TAG_END_TRANSLATION_VALUE:
      handle_object_prop(state, "End translation value");
      break;
    case TAG_START_TRANSLATION_LOCKED:
      handle_object_prop(state, "Start translation locked");
      break;
    case TAG_END_TRANSLATION_LOCKED:
      handle_object_prop(state, "End translation locked");
      break;
    case TAG_FUNCTIONAL_TRANSLATION_VALUE:
      handle_object_prop(state, "Functional translation value");
      break;
    case TAG_VALUE:
      handle_object_prop(state, "Value");
      break;
    case TAG_FORMAT:
      handle_format(atts, state);
      break;
    case TAG_INTEGRAL_PART:
      handle_object_prop(state, "Format integral part");
      break;
    case TAG_FRACTIONAL_PART:
      handle_object_prop(state, "Format fractional part");
      break;
    case TAG_SECOND_FONT_POS:
      handle_object_prop(state, "Format second font pos");
      break;
    case TAG_LEADING_ZEROS:
      handle_object_prop(state, "Format leading zeros");
      break;
    case TAG_DISPLAY_SIGN:
      handle_object_prop(state, "Format display sign");
      break;
    case TAG_FIRST_LINE_WIDTH:
      handle_object_prop(state, "First line width");
      break;
    case TAG_FIRST_FONT:
      handle_object_prop(state, "First font");
      break;
    case TAG_SECOND_LINE_WIDTH:
      handle_object_prop(state, "Second line width");
      break;
    case TAG_SECOND_FONT:
      handle_object_prop(state, "Second font");
      break;
    case TAG_EVENT_ID:
      handle_object_prop(state, "Event ID");
      break;
    case TAG_INSIDE:
      handle_object_prop(state, "Output Inside");
      break;
    case TAG_PRESSED:
      handle_object_prop(state, "Output Pressed");
      break;
    case TAG_RELEASED:
      handle_object_prop(state, "Output Released");
      break;
    case TAG_BUTTON:
      handle_object_prop(state, "Output Button");
      break;
    case TAG_MODIFIERS:
      handle_object_prop(state, "Output Modifiers");
      break;
    case TAG_KEYCODE:
      handle_object_prop(state, "Output Keycode");
      break;
    case TAG_TEXTURE_CONTROL:
      handle_object_prop(state, "Texture control");
      break;
    case TAG_START_ANGLE:
      handle_object_prop(state, "Start angle");
      break;
    case TAG_END_ANGLE:
      handle_object_prop(state, "End angle");
      break;
    case TAG_CENTER:
      handle_object_prop_xy(state, "Center.x", "Center.y");
      break;
    case TAG_RADIUS:
      handle_object_prop(state, "Radius");
      break;
    case TAG_INTERP:
      handle_object_prop(state, "Interp");
      break;
    case TAG_MULTILINE:
      handle_object_prop(state, "Multiline");
      break;
    case TAG_THICKNESS:
      handle_object_prop(state, "Thickness");
      break;
    case TAG_REFANGLE:
      handle_object_prop(state, "Refangle");
      break;
    case TAG_ENABLE:
      handle_object_prop(state, "Enable");
      break;
    case TAG_MEMORY:
      handle_object_prop(state, "Memory");
      break;
    case TAG_RESTORE_STATES:
      handle_object_prop(state, "Restore states");
      break;
    case TAG_HEIGHT:
      handle_object_prop(state, "Height");
      break;
    case TAG_WIDTH:
      handle_object_prop(state, "width");
      break;
    case TAG_POINTER_ID:
      handle_object_prop(state, "PointerID");
      break;
    case TAG_PERCENT_HEIGHT:
      handle_object_prop(state, "Output PercentHeight");
      break;
    case TAG_PERCENT_WIDTH:
      handle_object_prop(state, "Output PercentWidth");
      break;
    case TAG_PERCENT:
      handle_object_prop(state, "Output Percent");
      break;
    case TAG_MODULATE:
      handle_object_prop(state, "Modulate");
      break;
    case TAG_TESSELLATE:
      handle_object_prop(state, "Tessellate");
      break;
    case TAG_HORZ_RADIUS:
      handle_object_prop(state, "HorzRadius");
      break;
    case TAG_VERT_RADIUS:
      handle_object_prop(state, "VertRadius");
      break;
    case TAG_FUNCTION:
      handle_object_prop(state, "Function");
      handle_object_prop_name(atts, state);
      break;
    case TAG_INACTIVEMASKS:
      state->is_inactivemasks = true;
      break;
    case TAG_MASK:
      if (state->is_inactivemasks)
      {
        handle_inactivemasks(atts, state);
      }
      break;
    case TAG_CODEINTEGRATION:
    case TAG_SCADE:
    case TAG_ANIMATION:
    case TAG_SYMBOLOGY:
    case TAG_INPUTS:
    case TAG_OUTPUTS:
    case TAG_CRC:
    case TAG_EXPORT:
    case TAG_TRACEABILITY:
    case TAG_PREVIEW:
    case TAG_RAW:
    case TAG_ANGLE:
    case TAG_EXPR:
    case TAG_INIT:
    case TAG_PATH_FROM_ROOT:
    case TAG_COMMENT:
    case TAG_PROJECT:
    case TAG_CONSTANTS:
      /*do nothing*/
      break;
    default:
#ifdef _DEBUG
      printf("un-handle tag str:%s\n", (char *)el);
#endif
      break;
  }
  return (XML_OK);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void handle_codegen_include_path(parsing_state_t *state)
{
  char temp[FILE_NAME_LENGTH];
  char full_path[FILE_NAME_LENGTH];
  sprintf(temp, "%s/%s", get_sdy_source_dir(), state->character_data);
  convert_dir_format_for_mingw(temp, full_path);
  adjust_mem((void **)&codegen_include_path_list, &codegen_include_path_count, &codegen_include_path_list_size, CODEGEN_LIST_BASE_SIZE, sizeof(char *));
  create_string_item(&codegen_include_path_list[codegen_include_path_count++], full_path);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void handle_codegen_source_path(parsing_state_t *state)
{
  char temp[FILE_NAME_LENGTH];
  char full_path[FILE_NAME_LENGTH];
  sprintf(temp, "%s/%s", get_sdy_source_dir(), state->character_data);
  convert_dir_format_for_mingw(temp, full_path);
  adjust_mem((void **)&codegen_source_path_list, &codegen_source_path_count, &codegen_source_path_list_size, CODEGEN_LIST_BASE_SIZE, sizeof(char *));
  create_string_item(&codegen_source_path_list[codegen_source_path_count++], full_path);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void handle_comment (parsing_state_t *state)
{
  create_string_item(&state->current_object->comment, state->character_data);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static int
end_element_handler (void *user_data, const XMLCH *uri, const XMLCH *name, const XMLCH *el)
{
  parsing_state_t *state = (parsing_state_t *)user_data;
  sdy_source_tag_t tag = convert_to_tag((char *)el);
  
  switch (tag)
  {
  case TAG_INCLUDE:
    state->is_codegen_include_path = 0;
    break;
  case TAG_SOURCE:
    state->is_codegen_source_path = 0;
    break;
  case TAG_PATH:
    if (state->is_codegen_include_path == 3)
    {
      handle_codegen_include_path(state);
    }
    else if (state->is_codegen_source_path == 3)
    {
      handle_codegen_source_path(state);
    }
    break;
  case TAG_CHILDREN:
    handle_children_end(state);
    break;
  case TAG_COMMENT:
    handle_comment(state);
    break;
  case TAG_INDEXES:
    state->is_condition_indexes = false;
    break;
  case TAG_INDEX:
    state->is_condition_index = false;
    break;
  case TAG_EXPR:
    if (state->init_and_expr_scope == SCOPE_INPUTPARAMETERS || state->init_and_expr_scope == SCOPE_OUTPUTPARAMETERS)
    {
      set_parameter_expr(state->character_data, state->variable_dictionary_scope_id);
    }
    else if (state->init_and_expr_scope == SCOPE_PROPERTIES)
    {
      set_prop_expr(state);
    }
    break;
  case TAG_IDE:
    state->is_ide = false;
    break;
  case TAG_DECLARATION:
    state->is_declaration = false;
    break;
  case TAG_LAYER:
    state->is_layer = false;
    break;
  case TAG_INPUT:
  case TAG_OUTPUT:
  case TAG_LOCAL:
  case TAG_LOCAL_CONSTANT:
  case TAG_INPUT_PARAMETERS:
  case TAG_OUTPUT_PARAMETERS:
  case TAG_IMPORTED:
  case TAG_PROPERTIES:
  case TAG_CONSTANT:
    state->init_and_expr_scope = SCOPE_NONE;
    break;
  case TAG_INIT:
    if (state->init_and_expr_scope == SCOPE_PROPERTIES)
    {
      if (state->is_condition_index)
      {
        set_condition_index_var_init(state);
      }
      set_prop_init_value(state);
    }
    else if (state->init_and_expr_scope == SCOPE_INPUT ||
      state->init_and_expr_scope == SCOPE_OUTPUT ||
      state->init_and_expr_scope == SCOPE_LOCAL ||
      state->init_and_expr_scope == SCOPE_LOCAL_CONSTANT
      )
    {
      set_variable_dictionary_info_init_value(state->character_data);
    }
    else if (state->init_and_expr_scope == SCOPE_INPUTPARAMETERS || state->init_and_expr_scope == SCOPE_OUTPUTPARAMETERS)
    {
      set_parameter_init(state->character_data);
    }
    break;
  case TAG_ARCSEGMENT:
  case TAG_FIRST_ARC:
  case TAG_SECOND_ARC:
  case TAG_THIRD_ARC:
  case TAG_FOURTH_ARC:
    state->is_arc_segment = false;
    break;
  case TAG_TEXT_VALUE:
    state->is_text_value = false;
    break;
  case TAG_INACTIVEMASKS:
    state->is_inactivemasks = false;
    break;
  }
  return (XML_OK);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static int character_data_handler (void *user_data, const XMLCH *s, int len)
{
  parsing_state_t *state = (parsing_state_t *)user_data;

  if ((strlen(state->character_data) + len) < MAX_TEXT_BUFFER)
  {
    strncat(state->character_data, (const char *)s, len);
  }
  /* 丢弃超长部分
  else
  {
    fatal_error("MAX_TEXT_BUFFER is not enough for character_data_handler\n");
  }*/
  return (XML_OK);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void parse_sdy_source(char *sdy_source_file_name, void *father_object, int input_scope_id)
{
  FILE *xml_file;
  LPXMLPARSER parser;
  parsing_state_t current_state;
  ogfx_info_t *ogfx_info;
  int i, j;
  char file_name[FILE_NAME_LENGTH];
  
  memset(&current_state, 0, sizeof(current_state));
  current_state.father_object = (object_t *)father_object;
  current_state.variable_dictionary_scope_id = input_scope_id;
  compose_sdy_source_path(sdy_source_file_name, current_state.sdy_source_path);

  if ((xml_file = fopen(sdy_source_file_name, "r")) == NULL)
  {
    fatal_error("Can't open %s\n", sdy_source_file_name);
  }

  /* Set up our XML parser. */
  XMLParser_Create(&parser);
  if (!parser)
  {
    fatal_error("XMLParser_Create is failed\n");
  }
  else
  {
    /* Set up elements and data handlers. */
    parser->UserData = &current_state;
    parser->startElementHandler = start_element_handler,
    parser->endElementHandler = end_element_handler;
    parser->charactersHandler = character_data_handler;
  }

  if (parser)
  {
    if (!XMLParser_Parse(parser, read_xml_file, xml_file, 0/*"ISO-8859-1"*/))
    {
      fatal_error("Parsing %s failed", sdy_source_file_name);
    }
    XMLParser_Free(parser);
    fclose(xml_file);
  }

  if (object_count_for_main_source == 0)
  {
    object_count_for_main_source = object_count;
  }

  if (requirements_trace_file_name[0])
  {
    parse_trace_file(requirements_trace_file_name);
    requirements_trace_file_name[0] = 0;
  }

  for (i = 0, ogfx_info = &current_state.ogfx_info_buffer[i]; i < current_state.ogfx_info_count; i++, ogfx_info++)
  {
    compose_ogfx_file_name(sdy_source_file_name, ogfx_info->file_name, file_name);
    if (_access(file_name, 0))
    {
      warning_msg("parse_sdy_source: Can't open ogfx file %s\n", file_name); 
      continue;
    }

    for (j = 0; j < global_ogfx_info_count && strcmp(file_name, global_ogfx_info_list[j].file_name); j++);
    if (j == global_ogfx_info_count)
    {
      ogfx_info_t *global_ogfx_info;//由于递归调用parse_sdy_source，必须定义此变量
      ogfx_info->ogfx_root->ref_ogfx_id = global_ogfx_info_count;
      adjust_mem(&global_ogfx_info_list, &global_ogfx_info_count, &global_ogfx_info_list_size, OGFX_INFO_LIST_BASE_SIZE, sizeof(ogfx_info_t));
      global_ogfx_info = &global_ogfx_info_list[global_ogfx_info_count];
      global_ogfx_info->this_variable_dictionary_info_id = variable_dictionary_info_count;
      //global_ogfx_info->first_indexes_id = indexes_count;
      strcpy(global_ogfx_info->file_name, file_name);
      strcpy(global_ogfx_info->ogfx_name, ogfx_info->ogfx_name);
      global_ogfx_info_count++;
      parse_sdy_source(file_name, ogfx_info->ogfx_root, global_ogfx_info_count);
      global_ogfx_info->ogfx_root = ogfx_info->ogfx_root->son;
      //嵌套调用时错：global_ogfx_info->next_input_output_info_id = input_output_info_count;//经过parse_sdy_source, input_info_count已指向新的最终点
    }
  }

  for (i = 0, ogfx_info = &current_state.ogfx_info_buffer[i]; i < current_state.ogfx_info_count; i++, ogfx_info++)
  {
    compose_ogfx_file_name(sdy_source_file_name, ogfx_info->file_name, file_name);
    for (j = 0; j < global_ogfx_info_count && strcmp(file_name, global_ogfx_info_list[j].file_name); j++);
    if (j < global_ogfx_info_count)
    {
      object_t *son = global_ogfx_info_list[j].ogfx_root;
      ogfx_info->ogfx_root->son = son;
      adjust_mem((void *)&son->father_list, &son->father_count, &son->father_list_size, FATHER_LIST_BASE_SIZE, sizeof(object_t *));
      son->father_list[son->father_count++] = ogfx_info->ogfx_root;
      ogfx_info->ogfx_root->ref_ogfx_id = j;
    }
  }
  free_mem(current_state.ogfx_info_buffer);
}
