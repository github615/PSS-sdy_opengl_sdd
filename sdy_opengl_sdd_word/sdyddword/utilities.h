#pragma once

/*-------------------------- FILE PROLOGUE --------------------------*/

/**
 **********************************************************************
 * @notice
 * (C) 2018 SHAVIATION
 *
 * @brief
 *
 **********************************************************************
*/

/*------------------------- FILE INCLUSION --------------------------*/
#include "stdafx.h"

/* ----------------------- MACRO DEFINITIONS ------------------------*/
#define SET_BREAK printf("break\n")

/* ----------------------- TYPE DEFINITIONS -------------------------*/

/* ---------------------- OBJECT DECLARATIONS -----------------------*/

/* --------------------- FUNCTION DECLARATIONS ----------------------*/
void init_lcid();
void check_file_existence(TCHAR *file_name);
void change_to_output_dir(TCHAR *output_dir);
void open_log_file(TCHAR *outputDirName, TCHAR *logFileName);
void close_log_file();
void dump_memory(BYTE *memory, int bytes);
void dump_log_file(TCHAR *format_string, ...);
void fatal_error(TCHAR *format_string, ...);
void *get_cmem(size_t size);
void free_mem(void *mem);
