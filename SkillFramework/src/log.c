/* 
 * Copyright (c) 2011, ASMlove. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list ofconditions and the following disclaimer.
 * 
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materialsprovided with the
 *    distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/timeb.h>
#if _WIN32 || _WIN64
  #include <io.h>
  #include <direct.h>
#else
  #include <unistd.h>
  #include <sys/stat.h>
  #include <sys/types.h>
#endif
#include "../inc/log.h"


#if _WIN32
  #define inline __inline
  #if defined(_MSC_VER) && (_MSC_VER >= 1400)
    #pragma warning(disable:4996)
  #endif
#endif



#define LOGDIR_DEF  ("000")

enum {
  LOG_TYPE_DEBUG    = 0, 
  LOG_TYPE_MESSAGE  = 1, 
  LOG_TYPE_WARNNING = 2, 
  LOG_TYPE_ERROR    = 3
};

struct log_time_t {
  unsigned short year;
  unsigned char  mon;
  unsigned char  day;
  unsigned char  hour;
  unsigned char  min;
  unsigned char  sec;
  unsigned short millitm;
};



static inline void 
__get_log_time(struct log_time_t* lt)
{
  struct timeb tb;
  struct tm* local;

  if (NULL != lt) {
    ftime(&tb);
    local = localtime(&tb.time);
    lt->year = (unsigned short)local->tm_year + 1900;
    lt->mon  = (unsigned char)local->tm_mon + 1;
    lt->day  = (unsigned char)local->tm_mday;
    lt->hour = (unsigned char)local->tm_hour;
    lt->min  = (unsigned char)local->tm_min;
    lt->sec  = (unsigned char)local->tm_sec;
    lt->millitm = tb.millitm;
  }
}

static int 
__log_access(const char* path, int mode)
{
  return access(path, mode);
}

static int 
__log_mkdir(const char* path)
{
#if _WIN32 || _WIN64
  return mkdir(path);
#else
  return mkdir(path, S_IRWXU);
#endif
}

static int 
__log_vsnprintf(char* dest, size_t count, const char* fmt, va_list ap)
{
#if defined(_WIN32) && defined(_MSC_VER) && (_MSC_VER < 1400)
  return _vsnprintf(dest, count, fmt, ap);
#else
  return vsnprintf(dest, count, fmt, ap);
#endif
}

static int 
__log_snprintf(char* dest, size_t count, const char* fmt, ...)
{
  int ret = 0;
  va_list ap;

  va_start(ap, fmt);
  ret = __log_vsnprintf(dest, count, fmt, ap);
  va_end(ap);

  return ret;
}

static void 
__log(const char* dir, int severity, const char* message)
{
  const char* severity_str;
  FILE* stream;
  char  fname[128] = {0};
  char  log_dir[64] = {0};
  struct log_time_t lt;

  switch (severity) {
  case LOG_TYPE_DEBUG:
    severity_str = "debug"; break;
  case LOG_TYPE_MESSAGE:
    severity_str = "msg";   break;
  case LOG_TYPE_WARNNING:
    severity_str = "warn";  break;
  case LOG_TYPE_ERROR:
    severity_str = "err";   break;
  default:
    severity_str = "???";
  }

  __get_log_time(&lt);
  __log_snprintf(log_dir, sizeof(log_dir), "./[%s]log", dir);
  __log_snprintf(fname, sizeof(fname), "%s/%04d%02d%02d.%s.log", log_dir, lt.year, lt.mon, lt.day, severity_str);
  if (0 != __log_access(log_dir, 0))
    __log_mkdir(log_dir);
  stream = fopen(fname, "a+");

  if (NULL != stream) {
    fprintf(stream, "[%02d:%02d:%02d:%03d] - %s", 
      lt.hour, lt.min, lt.sec, lt.millitm, message);
    fclose(stream);
  }
}

static void 
__log_helper(const char* dir, int severity, int log_errno, const char* fmt, va_list ap)
{
  char buf[1024] = {0};

  if (NULL != fmt)
    __log_vsnprintf(buf, sizeof(buf), fmt, ap);
  if (log_errno > 0) {
    size_t len = strlen(buf);
    if (len < sizeof(buf) - 3) 
      __log_snprintf(buf + len, sizeof(buf) - len, ": %s", strerror(log_errno));
  }

  __log(dir, severity, buf);
}



void 
log_err(const char* dir, int code, const char* fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  __log_helper((NULL != dir ? dir : LOGDIR_DEF), LOG_TYPE_ERROR, errno, fmt, ap);
  va_end(ap);

  exit(code);
}

void 
log_warn(const char* dir, const char* fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  __log_helper((NULL != dir ? dir : LOGDIR_DEF), LOG_TYPE_WARNNING, errno, fmt, ap);
  va_end(ap);
}

void 
log_errx(const char* dir, int code, const char* fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  __log_helper((NULL != dir ? dir : LOGDIR_DEF), LOG_TYPE_ERROR, errno, fmt, ap);
  va_end(ap);

  exit(code);
}

void 
log_warnx(const char* dir, const char* fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  __log_helper((NULL != dir ? dir : LOGDIR_DEF), LOG_TYPE_WARNNING, errno, fmt, ap);
  va_end(ap);
}

void 
log_msgx(const char* dir, const char* fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  __log_helper((NULL != dir ? dir : LOGDIR_DEF), LOG_TYPE_MESSAGE, errno, fmt, ap);
  va_end(ap);
}

void 
log_debugx(const char* dir, const char* fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  __log_helper((NULL != dir ? dir : LOGDIR_DEF), LOG_TYPE_DEBUG, errno, fmt, ap);
  va_end(ap);
}
