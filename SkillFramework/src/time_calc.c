/* 
 * Copyright (c) 2012, ASMlove. All rights reserved.
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
#include "../inc/time_calc.h"

enum TimeType {
  TimeType_Second   = 10000000,
};


enum ResultTimeDiff {
  Result_Success = 0, 
  Result_Failed  = 1,
};
static int timediff(SYSTEMTIME* beg, SYSTEMTIME* end, UINT64* result)
{
  ULARGE_INTEGER t1, t2;

  if (SystemTimeToFileTime(beg, (FILETIME*)&t1) 
      && SystemTimeToFileTime(end, (FILETIME*)&t2))
  {
    *result = (t2.QuadPart - t1.QuadPart);
    return Result_Success;
  }
  return Result_Failed;
}

DWORD timediff_tosecond(SYSTEMTIME* beg, SYSTEMTIME* end)
{
  UINT64 diff;
  if (Result_Success == timediff(beg, end, &diff))
    return (DWORD)(diff / TimeType_Second);
  return (DWORD)-1;
}
