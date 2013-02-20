// Copyright (c) 2011, ASMlove. All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
//  * Redistributions of source code must retain the above copyright
//    notice, this list ofconditions and the following disclaimer.
// 
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materialsprovided with the
//    distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#include  "../stdafx.h"
#include "./example_mini/example_mini.h"
#include "../inc/winthunk.h"


class Example_WinThunk : public example_mini::Example {
public:
  virtual void do_Example(void);
};
REG_EXAMPLE("winthunk", "Example for class WinThunk", Example_WinThunk);


class ExampleWinThunk : public WinThunk {
public:
  virtual LRESULT vWndProc(UINT msg, WPARAM wp, LPARAM lp)
  {
    UNREFERENCED_PARAMETER(wp);
    UNREFERENCED_PARAMETER(lp);

    switch (msg)
    {
    case WM_DESTROY:
      PostQuitMessage(0); break;
    }
    return DefWindowProc(hwnd_, msg, wp, lp);
  }
};

void Example_WinThunk::do_Example(void)
{
  fprintf(stdout, __FUNCSIG__ "\n");

  ExampleWinThunk ew;

  ew.regClassEx(GetModuleHandle(NULL), TEXT("ExampleWinThunk"));
  ew.createEx(NULL, 0, TEXT("ExampleWinThunk"), TEXT("ExampleWinThunk"),  WS_OVERLAPPEDWINDOW);
  ew.show(SW_SHOWNORMAL);

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0) > 0)
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}