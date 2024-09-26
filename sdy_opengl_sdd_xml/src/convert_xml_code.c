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
#include "iconv.h"
#include "common_definitions.h"

/* --------------------------- 宏定义 -------------------------------*/

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/
static char code_buffer[MAX_CODE_BUFFER];

/* --------------------------- 函数定义 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static int code_convert(char* from_charset, char* to_charset, char* inbuf,
  int inlen, char* outbuf, int outlen)
{
	int r;
  iconv_t cd;
  char** pin = &inbuf;  
  char** pout = &outbuf;
  cd = iconv_open(to_charset,from_charset);  
  if(cd == 0)
    return -1;

  memset(outbuf,0,outlen);  

  if(iconv(cd,(const char**)pin,(unsigned int *)&inlen,pout,(unsigned int*)&outlen) == -1)
  {
  	r = -1;
  }
  else
  {
  	r = 0;
  }

  iconv_close(cd);

  return r;  
}

/**
 **********************************************************************
 *
 * @功能描述
 *  UNICODE码转为GB2312码  
 *
 **********************************************************************
*/
char* u2g(char *inbuf)  
{
  int nOutLen = 2 * strlen(inbuf) - 1;
  char* szOut = code_buffer;
  if (nOutLen > MAX_CODE_BUFFER)
  {
    fatal_error("MAX_CODE_BUFFER is not enough\n");
  }
  if (-1 == code_convert("utf-8", "gb2312", inbuf, strlen(inbuf), code_buffer, nOutLen))
  {
    fatal_error("u2g is failed for converting \"%s\"\n", inbuf);
    szOut = NULL;
  }
  return szOut;
}  
/**
 **********************************************************************
 *
 * @功能描述
 * GB2312码转为UNICODE码  
 *
 **********************************************************************
*/
char* g2u(char *inbuf)  
{
  int nOutLen = 2 * strlen(inbuf) - 1;
  char* szOut = code_buffer;
  if (nOutLen > MAX_CODE_BUFFER)
  {
    fatal_error("MAX_CODE_BUFFER is not enough\n");
  }
  if (-1 == code_convert("gb2312", "utf-8", inbuf, strlen(inbuf), code_buffer, nOutLen))
  {
    fatal_error("u2g is failed for converting \"%s\"\n", inbuf);
    szOut = NULL;
  }
  return szOut;
}  