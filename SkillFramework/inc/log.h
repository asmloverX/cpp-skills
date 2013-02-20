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
#ifndef __S_LOG_HEADER_H__
#define __S_LOG_HEADER_H__

#ifndef _SLOG_
#define _SLOG_

#ifdef __cplusplus
extern "C" {
#endif


extern void log_err(const char* dir, int code, const char* fmt, ...);
extern void log_warn(const char* dir, const char* fmt, ...);
extern void log_errx(const char* dir, int code, const char* fmt, ...);
extern void log_warnx(const char* dir, const char* fmt, ...);
extern void log_msgx(const char* dir, const char* fmt, ...);
extern void log_debugx(const char* dir, const char* fmt, ...);


#if _WIN32 || _WIN64
  #if _DEBUG
    #define log_msg   log_msgx
    #define log_debug log_debugx
  #else
    #define log_msg
    #define log_debug
  #endif
#else
  #ifndef NDEBUG
    #define log_msg   log_msgx
    #define log_debug log_debugx
  #else
    #define log_debug
  #endif
#endif


#ifdef __cplusplus
}
#endif

#endif  /* _SLOG_ */

#endif  /* __S_LOG_HEADER_H__ */
