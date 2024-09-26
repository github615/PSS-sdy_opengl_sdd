/* --------------------------- 序言 ---------------------------------*/

/**
 **********************************************************************
 *
 * (C) 2013, 2018 上海爱韦讯信息技术有限公司。
 *
 * @功能描述
 *
 **********************************************************************
*/

/* --------------------------- 文件包含 -----------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "aside_decision.h"
#include "aside_sdy_object_relation.h"
#include "FreeImage.h"
#include "target_configuration.h"

/* --------------------------- 宏定义 -------------------------------*/
#define FILE_NAME_LENGTH 500

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/
extern char *image_name_list[OBJECT_COUNT];
extern FILE *log_file;

static char image_file_dir_name[FILE_NAME_LENGTH];
static char bmp_file_name[FILE_NAME_LENGTH];

static GLint width;
static GLint height;

static char head[54]={
0x42, //0
0x4d, //1
0x66, //2
0x75, //3
0x00, //4
0x00, //5
0x00, //6
0x00, //7
0x00, //8
0x00, //9
0x36, //a
0x00, //b
0x00, //c
0x00, //d
0x28, //e
0x00,//f
0x00, //0
0x00, //1
0x64, //2
0x00, //3
0x00, //4
0x00, //5
0x64, //6
0x00, //7
0x00, //8
0x00, //9
0x01, //a
0x00, //b
0x18, //c
0x00, //d
0x00, //e
0x00,//f
0x00, //0
0x00, //1
0x30, //2
0x75//3
};

/* --------------------------- 函数声明 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void init_aside_opengl_gen_image(char *dir_name_in)
{
  size_t l;

  width = target_screen_width;
  height = target_screen_height;

  strcpy(image_file_dir_name, dir_name_in);
  l = strlen(image_file_dir_name);
  if (image_file_dir_name[l-1] == '\\')
  {
    image_file_dir_name[l-1] = 0;
  }
  sprintf(bmp_file_name, "%s\\temp.bmp", image_file_dir_name);
  reset_parameters();
  open_log_file(image_file_dir_name);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void grab_in_bmp(char *bmp_file_name)
{
  GLint pixelLength;
  GLubyte * pixelDate;
  FILE * wfile;
  //打开文件
  wfile = fopen(bmp_file_name, "wb" );
  fwrite( head, 54, 1, wfile );
  //更改grab.bmp的头文件中的高度和宽度
  fseek( wfile, 0x0012, SEEK_SET );
  fwrite(&width, sizeof(width), 1, wfile );
  fwrite(&height, sizeof(height ), 1, wfile );
  //为像素分配内存
  pixelLength = width * 3;
  if ( pixelLength % 4 != 0 )
  {
    pixelLength += 4 - pixelLength%4;
  }
  pixelLength *= height;
  pixelDate = (GLubyte *)malloc( pixelLength );
  if ( pixelDate == 0 )
  {
    printf( "/a/n分配内存失败!" );
  }
  //读取窗口像素并存储
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  /*VS2005的GL.h只有GL_BGR_EXT, Mingw的GL.h只有GL_BGR，两者值相同。Mingw的GLEXT.h有GL_BGR_EXT*/
  glReadPixels( 0, 0, width, height, 0x80E0/*GL_BGR_EXT*/, GL_UNSIGNED_BYTE, pixelDate );
  //写入像素数据
  fseek( wfile, 0, SEEK_END );
  fwrite( pixelDate, pixelLength, 1, wfile );
  fclose( wfile );
  free( pixelDate );
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void convert_bmp(char *bmp_file_name, char *new_file_name)
{
  FIBITMAP *bitmap;
  bitmap = FreeImage_Load(FIF_BMP, bmp_file_name, BMP_DEFAULT);
  if (bitmap)
  {
    FreeImage_Save(FIF_JPEG, bitmap, new_file_name, JPEG_DEFAULT);
    FreeImage_Unload(bitmap);
  }
}

/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void aside_opengl_gen_image(void)
{
  char new_file_name[FILE_NAME_LENGTH];
  int active_object_id = get_active_object_id();
  static int unreach_count = 0;

  /*在613例子中,unreach_count为7(即layer总数)时active_object_is_reached*/
  if (!get_active_object_is_reached())
  {
	unreach_count++;
	if (unreach_count < (LAYER_COUNT + 6))
	{
		return;
	}
  }
  if (!get_active_object_is_reached())
  {
    log_object_is_not_reached(active_object_id);
  }
  else
  {
	fprintf(log_file, "unreach_count=%d\n", unreach_count);
    log_object_summary(active_object_id);

    sprintf(new_file_name, "%s\\%s", image_file_dir_name, image_name_list[active_object_id]);
    grab_in_bmp(bmp_file_name);
    convert_bmp(bmp_file_name, new_file_name);
  }
  
  unreach_count = 0;

  active_object_id = increase_active_object_id();
  if (active_object_id >= OBJECT_COUNT_FOR_IMAGE) //if (active_object_id >= OBJECT_COUNT)
  {
    close_log_file();
    exit(0);
  }
  reset_parameters();
}

















