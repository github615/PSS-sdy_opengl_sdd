#ifndef __COMMON_DEFINITIONS_H__
#define __COMMON_DEFINITIONS_H__

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
#include "utilities.h"

/* --------------------------- 宏定义 -------------------------------*/
#define ATTR_LENGTH 60
#define CODEGEN_LIST_BASE_SIZE 10
#define CONDITION_INDEX_LIST_BASE_SIZE 10
#define CONDITION_OBJECT_BASE_SIZE 10
#define FATHER_LIST_BASE_SIZE 50
#define FILE_NAME_LENGTH 500
#define CMD_LENGTH (FILE_NAME_LENGTH*100)
#define IMAGE_NAME_LENGTH 20
#define INPUT_OUTPUT_INFO_LIST_BASE_SIZE 500
#define VARIABLE_DICTIONARY_PROP_LENGTH 64
#define INIT_VALUE_LENGTH 64
#define LINE_LENGTH 1000
#define LAYER_LIST_BASE_SIZE 5
#define LEVEL_NUMBER_LENGTH 64
#define MAX_TEXT_BUFFER 50000
#define MAX_CODE_BUFFER (MAX_TEXT_BUFFER*2)
#define OBJECT_NAME_LENGTH 64
#define OBJECT_FULL_NAME_LENGTH 1000
#define OBJECT_LIST_BASE_SIZE 2000
#define OBJECT_TYPE_LENGTH 64
#define OGFX_INFO_BUFFER_BASE_SIZE 100
#define OGFX_INFO_LIST_BASE_SIZE 200
#define PARAMETER_PROP_LENGTH 64
#define REF_PARAMETER_INFO_LIST_BASE_SIZE 500
#define INACTIVE_MASK_LIST_BASE_SIZE 6

#define INDEXES_OTHER 7777777 /*假设用户不可能把index设为该值*/

/* --------------------------- 类型定义 -----------------------------*/
typedef enum {
  ENGLISH,
  CHINESE
} language_t;

typedef enum {
  TAG_NONE,
  TAG_ANGLE,
  TAG_ANIMATION,
  TAG_ARC,
  TAG_ARCELLIPSE,
  TAG_ARCSEGMENT,
  TAG_BIFONT,
  TAG_BITMAP,
  TAG_BUTTON,
  TAG_CENTER,
  TAG_CHILDREN,
  TAG_CIRCLE,
  TAG_CIRCLE_AREA,
  TAG_CLIP_ANGLE,
  TAG_CLIP_BOX,
  TAG_CLIP_INSIDE,
  TAG_CLIP_PLANE,
  TAG_CLIP_START_POINT,
  TAG_CODEGEN,
  TAG_CODEINTEGRATION,
  TAG_COMMENT,
  TAG_COND_CONTAINER,
  TAG_CONSTANT,
  TAG_CONSTANTS,
  TAG_CONTAINER,
  TAG_COLOR_MODE,
  TAG_CONTROL,
  TAG_CRC,
  TAG_CROWN,
  TAG_DECLARATION,
  TAG_DIMENSION,
  TAG_DISPLAY_SIGN,
  TAG_ELLIPSE,
  TAG_ENABLE,
  TAG_END_ANGLE,
  TAG_END_ROTATION_ANGLE,
  TAG_END_ROTATION_LOCKED,
  TAG_END_ROTATION_VALUE,
  TAG_END_TRANSLATION_LOCKED,
  TAG_END_TRANSLATION_POINT,
  TAG_END_TRANSLATION_VALUE,
  TAG_ENVIRONMENT_ID,
  TAG_EVENT_ID,
  TAG_EXPORT,
  TAG_EXPR,
  TAG_REFPOINT,
  TAG_FILE,
  TAG_FILL_COLOR,
  TAG_FILL_OPACITY,
  TAG_FILTER_ROTATION_CONTAINER,
  TAG_FILTER_TRANSLATION_CONTAINER,
  TAG_FIRST_ARC,
  TAG_FIRST_FONT,
  TAG_FIRST_LINE_WIDTH,
  TAG_FIRST_POINT,
  TAG_FONT,
  TAG_FORMAT,
  TAG_FOURTH_ARC,
  TAG_FRACTIONAL_PART,
  TAG_FUNCTION,
  TAG_FUNCTIONAL_ROTATION_VALUE,
  TAG_FUNCTIONAL_TRANSLATION_VALUE,
  TAG_HALOCOLOR, //SDY R16
  TAG_HALOING,
  TAG_HEIGHT,
  TAG_HOOK,
  TAG_HORIZ_ALIGN,
  TAG_HORZ_RADIUS,
  TAG_ID,
  TAG_IDE,
  TAG_IMPORTED,
  TAG_INACTIVEMASKS,
  TAG_INCLUDE,
  TAG_INDEX,
  TAG_INDEXES,
  TAG_INIT,
  TAG_INPUT,
  TAG_INPUTS,
  TAG_INPUT_PARAMETERS,
  TAG_INSIDE,
  TAG_INTEGRAL_PART,
  TAG_INTERP,
  TAG_KEYBOARD_EVENT_LISTENER,
  TAG_KEYCODE,
  TAG_LAYER,
  TAG_LEADING_ZEROS,
  TAG_LINE,
  TAG_LINE_CAP,
  TAG_LINE_WIDTH,
  TAG_LINE_STIPPLE,
  TAG_LOCAL,
  TAG_LOCAL_CONSTANT,
  TAG_MASK,
  TAG_MASK_ACTIVITY,
  TAG_MAX_LENGTH,
  TAG_MEMORY,
  TAG_MODIFIERS,
  TAG_MODULATE, //SDY R16
  TAG_MULTILINE,
  TAG_NPLICATOR_CONTAINER,
  TAG_OBJECT,
  TAG_OUTLINE_COLOR,
  TAG_OUTLINE_OPACITY,
  TAG_OUTPUT,
  TAG_OUTPUTS,
  TAG_OUTPUT_PARAMETERS,
  TAG_ORIENTATION,
  TAG_ORIGIN,
  TAG_PANEL_CONTAINER,
  TAG_PARAMETER,
  TAG_PATH,
  TAG_PATH_FROM_ROOT,
  TAG_PERCENT,
  TAG_PERCENT_HEIGHT,
  TAG_PERCENT_WIDTH,
  TAG_POINT,
  TAG_POINTS,
  TAG_POINTER_EVENT_LISTENER,
  TAG_POINTER_ID,
  TAG_POLYGON_SMOOTH,
  TAG_POSITION,
  TAG_PRESSED,
  TAG_PREVIEW,
  TAG_PRIORITY,
  TAG_PROJECT, //SDY R16
  TAG_PROPERTIES,
  TAG_REQUIREMENTS,
  TAG_RADIUS,
  TAG_RATIO,
  TAG_RAW,
  TAG_RECTANGLE,
  TAG_RECTANGLE_AREA,
  TAG_REFANGLE,
  TAG_REFERENCE_CONTAINER,
  TAG_RELEASED,
  TAG_RESTORE_STATES,
  TAG_RICHTEXT,
  TAG_ROTATE,
  TAG_ROTATION_CONTAINER,
  TAG_SCADE,
  TAG_SCALE,
  TAG_SECOND_ARC,
  TAG_SECOND_FONT,
  TAG_SECOND_FONT_POS,
  TAG_SECOND_LINE_WIDTH,
  TAG_SHAPE,
  TAG_SOURCE,
  TAG_SPECIFICATION,
  TAG_SYMBOLOGY,
  TAG_START_ANGLE,
  TAG_START_ROTATION_ANGLE,
  TAG_START_ROTATION_LOCKED,
  TAG_START_ROTATION_VALUE,
  TAG_START_TRANSLATION_LOCKED,
  TAG_START_TRANSLATION_POINT,
  TAG_START_TRANSLATION_VALUE,
  TAG_STATIC, //SDY R16
  TAG_STENCIL,
  TAG_TESSELLATE, //SDY R16
  TAG_TEXT,
  TAG_TEXT_AREA,
  TAG_TEXT_VALUE,
  TAG_TEXTURE,
  TAG_TEXTURE_CONTROL,
  TAG_TEXTURE_ID,
  TAG_THICKNESS,
  TAG_THIRD_POINT,
  TAG_THIRD_ARC,
  TAG_TRANSLATION_CONTAINER,
  TAG_TRACEABILITY,
  TAG_U,
  TAG_V,
  TAG_VALUE,
  TAG_VERT_ALIGN,
  TAG_VERT_RADIUS,
  TAG_VISIBLE,
  TAG_WIDTH,
  TAG_X,
  TAG_Y,
} sdy_source_tag_t;

typedef enum {
  NO_SGL_CODE,
  PRIMITIVE_OBJECT,
  BI_PRIMITIVE_OBJECT,
  CASE_PRIMITIVE_OBJECT,
  COMPONENT_OBJECT,
} object_class_t;

typedef struct  
{
  char *value;
  char *expr;
} prop_value_and_expr_t;

typedef struct _object_prop
{
  char *name;
  prop_value_and_expr_t value_and_expr;
  struct _object_prop *next;
} object_prop_t;

typedef enum
{
  CONDITION_INDEX_INIT_NORMAL_VALUE,
  CONDITION_INDEX_INIT_DEFAULT,
  CONDITION_INDEX_INIT_OTHER
} condition_index_init_type_t;

typedef struct  
{
  condition_index_init_type_t index_init_type;
  int index_init_value;
  int *condition_index_list;
  int condition_index_list_size;
  int condition_index_count;
  int default_index;
} indexes_info_t;

typedef enum
{
  SCOPE_NONE,
  SCOPE_INPUT,
  SCOPE_INPUTPARAMETERS,
  SCOPE_LOCAL,
  SCOPE_LOCAL_CONSTANT,
  SCOPE_OUTPUT,
  SCOPE_OUTPUTPARAMETERS,
  SCOPE_PROPERTIES,
  SCOPE_CONSTANT,
} init_and_expr_scope_t;

typedef struct _mask_info
{
  struct _object *mask;
  struct _mask_info *next_mask_info;
  bool inactivity;
} mask_info_t;

typedef struct _object
{
  sdy_source_tag_t object_tag;
  char object_name[OBJECT_NAME_LENGTH + 1];
  char full_name_with_level_number[OBJECT_FULL_NAME_LENGTH];
  char *full_name_with_slash;
  char level_number[LEVEL_NUMBER_LENGTH];
  char image_name[IMAGE_NAME_LENGTH];
  char *oid;
  char *comment;
  char *traceinfo;
  unsigned non_primitive_sibling_order;
  int primitive_sibling_order;
  int primitive_son_count;
  int id;
  struct _object *next_sibling;
  struct _object *prev_sibling;
  struct _object *son;
  struct _object **father_list;
  int father_list_size;
  int father_count;

  int inactive_mask_count;
  int inactive_mask_list_size;
  struct _object **inactive_mask_list;//2018
  mask_info_t *mask_info;//2018

  object_prop_t *prop_head;
  object_prop_t *prop_tail;

  int ref_ogfx_id;//for REF_CONTAINER
  /*below are defined for the object containing inputParameters and outputParameters*/
  int first_input_ref_parameter_info_id;
  int input_ref_parameter_count;
  int first_output_ref_parameter_info_id;
  int output_ref_parameter_count;

  /*below are defined for COND_CONTAINER*/
  indexes_info_t *indexes_info;
  int indexes_id;
  //delete int input_output_info_id_of_condition_expr;
  //delete char *condition_expr;

  /*below is defined for son of COND_CONTAINER*/
  int expected_condition_index;

  object_class_t object_class;
  int variable_dictionary_scope_id;
  bool to_handle;
} object_t;

typedef struct
{
  char file_name[FILE_NAME_LENGTH];
  char ogfx_name[OBJECT_NAME_LENGTH + 1];
  char full_name_with_level_number[OBJECT_FULL_NAME_LENGTH];
  object_t *ogfx_root;
  int this_variable_dictionary_info_id;
  int next_variable_dictionary_info_id;
} ogfx_info_t;

typedef struct
{
  int variable_dictionary_scope_id;
  init_and_expr_scope_t init_and_expr_scope;
  char memory[VARIABLE_DICTIONARY_PROP_LENGTH];
  char representation[VARIABLE_DICTIONARY_PROP_LENGTH];
  char variable_name[VARIABLE_DICTIONARY_PROP_LENGTH];
  char variable_oid[VARIABLE_DICTIONARY_PROP_LENGTH];
  char variable_type[VARIABLE_DICTIONARY_PROP_LENGTH];
  char variable_group[VARIABLE_DICTIONARY_PROP_LENGTH];
  char variable_memory[VARIABLE_DICTIONARY_PROP_LENGTH];
  char variable_control[VARIABLE_DICTIONARY_PROP_LENGTH];
  char *init_value;
  char *control_plug;
  char trace[VARIABLE_DICTIONARY_PROP_LENGTH];
} variable_dictionary_info_t;

typedef struct
{
  int object_id;
  int this_variable_dictionary_info_id;
  int next_variable_dictionary_info_id;
  char *layer_name;
  char ratio_horizontal[ATTR_LENGTH];//2018-12-7
  char ratio_vertical[ATTR_LENGTH];//2018-12-7
  char dimension_height[ATTR_LENGTH];//2018-12-7
  char dimension_width[ATTR_LENGTH];//2018-12-7
} layer_info_t;

typedef struct
{
  object_t *ref_object;
  int form_para_scope_id;
  int real_para_scope_id;
  char parameter_control[PARAMETER_PROP_LENGTH];
  char parameter_name[PARAMETER_PROP_LENGTH];
  char parameter_oid[PARAMETER_PROP_LENGTH];
  char parameter_representation[PARAMETER_PROP_LENGTH];
  char parameter_type[PARAMETER_PROP_LENGTH];
  char *real_para_name;//expr
  int form_para_info_id;
  int real_para_info_id;
  char *init_value;
} ref_parameter_info_t;

typedef struct
{
  int is_top_condition_expr;//yes=1 no=-1
  int real_para_info_id;
  int value;
} active_condition_expr_t;

typedef struct
{
  char character_data[MAX_TEXT_BUFFER];
  object_t *father_object;
  object_t *current_object;
  bool is_layer;
  bool is_ide;
  bool is_declaration;
  bool is_condition_indexes;
  bool is_condition_index;
  bool is_inactivemasks;
  int is_codegen_include_path;
  int is_codegen_source_path;

  ogfx_info_t *ogfx_info_buffer;
  int ogfx_info_buffer_size;
  int ogfx_info_count;
  int variable_dictionary_scope_id;

  init_and_expr_scope_t init_and_expr_scope;

  bool is_arc_segment;
  bool is_text_value;
  prop_value_and_expr_t *prop_value_and_expr_pointer;
  prop_value_and_expr_t *prop_value_and_expr_of_x_pointer;
  prop_value_and_expr_t *prop_value_and_expr_of_y_pointer;
  prop_value_and_expr_t *prop_value_and_expr_of_u_pointer;
  prop_value_and_expr_t *prop_value_and_expr_of_v_pointer;
  int point_number;

  char sdy_source_path[FILE_NAME_LENGTH];
} parsing_state_t;

/* --------------------------- 函数声明 -----------------------------*/

#endif    /*  __COMMON_DEFINITIONS_H__  */
