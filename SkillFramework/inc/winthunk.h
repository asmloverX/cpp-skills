// Copyright (c) 2011, ASMlover. All rights reserved.
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
#ifndef __WINTHUNK_HEADER_H__
#define __WINTHUNK_HEADER_H__

#ifndef __cplusplus
  #error requires C++ compilation (use a .cpp or .cc suffix)
#endif


#pragma pack(push, 1)
//! @struct Thunk
//! @brief 一个Thunk结构, 利用Thunk技术封装窗口类
struct Thunk {
  DWORD mov_;     //!< 将this指针移到窗口句柄
  DWORD this_;    //!< 窗口类对象
  BYTE  jmp_;     //!< 跳转指令
  DWORD relproc_; //!< 消息处理过程

  void init(DWORD proc, void* thisp)
  {
    mov_  = 0x042444c7;
    this_ = PtrToUlong(thisp);
    jmp_  = 0xe9;
    relproc_ = (DWORD)((INT_PTR)proc - ((INT_PTR)this + sizeof(Thunk)));
    FlushInstructionCache(GetCurrentProcess(), this, sizeof(Thunk));
  }

  void* getAddress(void) 
  {
    return this;
  }
};
#pragma pack(pop)


//! @class WinThunk
//! @brief 这是一个窗口的基类, 内部窗口回调使用Thunk技术
struct WinThunk {
  HINSTANCE hInst_;   //!< 应用程序实例句柄
  HWND      hwnd_;    //!< 窗口句柄信息
  Thunk     thunk_;   //!< Thunk对象
public:
  WinThunk(void) 
    : hInst_(NULL), hwnd_(NULL)
  {}

  virtual ~WinThunk(void)
  {}

  virtual LRESULT vWndProc(UINT msg, WPARAM wp, LPARAM lp) = 0;

  virtual LPCTSTR getClass(void) const
  {
    return TEXT("WinThunk");
  }
public:
  //! 设置应用程序的实例句柄
  void setInstance(HINSTANCE hInst)
  {
    hInst_ = hInst;
  }

  //! 获取应用程序实例句柄
  HINSTANCE getInstance(void) const 
  {
    return hInst_;
  }

  //! 注册窗口类
  ATOM regClass(HINSTANCE hInst, LPCTSTR className)
  {
    setInstance(hInst);
    
    WNDCLASS wc = {0};
    wc.hbrBackground = (HBRUSH)(BLACK_BRUSH);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance     = getInstance();
    wc.lpfnWndProc   = (WNDPROC)&WinThunk::sInitProc;
    wc.lpszClassName = className;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    return RegisterClass(&wc);
  }

  //! 注册扩展风格的窗口类
  ATOM regClassEx(HINSTANCE hInst, LPCTSTR className)
  {
    setInstance(hInst);

    WNDCLASSEX wc = {0};
    wc.cbSize        = sizeof(wc);
    wc.hbrBackground = (HBRUSH)(BLACK_BRUSH);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance     = getInstance();
    wc.lpfnWndProc   = (WNDPROC)&WinThunk::sInitProc;
    wc.lpszClassName = className;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    return RegisterClassEx(&wc);
  }

  //! 创建窗口
  HWND create(HWND hParent, LPCTSTR className, LPCTSTR windowName, 
    DWORD style, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, 
    int width = CW_USEDEFAULT, int height = CW_USEDEFAULT, HMENU hMenu = NULL)
  {
    return (hwnd_ = CreateWindow(className, windowName, style, 
      x, y, width, height, hParent, hMenu, getInstance(), this));
  }

  //! 创建可扩展风格的窗口
  HWND createEx(HWND hParent, DWORD styleEx, LPCTSTR className, LPCTSTR windowName, 
    DWORD style, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, 
    int width = CW_USEDEFAULT, int height = CW_USEDEFAULT, HMENU hMenu = NULL)
  {
    return (hwnd_ = CreateWindowEx(styleEx, className, windowName, 
      style, x, y, width, height, hParent, hMenu, getInstance(), this));
  }

  //! 以cmdShow模式显示窗口
  void show(int cmdShow) 
  {
    assert(IsWindow(hwnd_));
    ShowWindow(hwnd_, cmdShow);
    UpdateWindow(hwnd_);
  }

  //! 将窗口显示到屏幕的正中央
  void center(void)
  {
    RECT rc;
    GetClientRect(hwnd_, &rc);

    int x = GetSystemMetrics(SM_CXSCREEN) / 2 - (rc.right - rc.left) / 2;
    int y = GetSystemMetrics(SM_CYSCREEN) / 2 - (rc.bottom - rc.top) / 2;
    SetWindowPos(hwnd_, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
  }
private:
  static LRESULT CALLBACK sInitProc(HWND h, UINT msg, WPARAM wp, LPARAM lp)
  {
    if (WM_NCCREATE == msg) {
      WinThunk* self = (WinThunk*)((LPCREATESTRUCT)lp)->lpCreateParams;
      if (NULL != self) {
        self->hwnd_ = h;
        self->thunk_.init((DWORD_PTR)&WinThunk::sWndProc, self);
        WNDPROC proc = (WNDPROC)self->thunk_.getAddress();

        assert(sInitProc == (WNDPROC)SetWindowLong(h, GWL_WNDPROC, (LONG_PTR)proc));
        return proc(h, msg, wp, lp);
      }
    }
    return 0L;
  }

  static LRESULT CALLBACK sWndProc(HWND h, UINT msg, WPARAM wp, LPARAM lp)
  {
    return ((WinThunk*)h)->vWndProc(msg, wp, lp);
  }
};

#endif  // __WINTHUNK_HEADER_H__
