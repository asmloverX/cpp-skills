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
#ifndef __WINBASE_HEADER_H__
#define __WINBASE_HEADER_H__

#ifndef __cplusplus
  #error requires C++ compilation (use a .cpp or .cc suffix)
#endif


//! @class WinBase
//! @brief 该基类是一个使用GWL_USERDATA来封装的窗口类
struct WinBase 
{
  HINSTANCE hInst_; //!< 应用程序实例句柄
  HWND      hwnd_;  //!< 窗口句柄
public:
  WinBase(void) 
    : hInst_(NULL), hwnd_(NULL)
  {}

  virtual ~WinBase(void)
  {}

  virtual LRESULT vWndProc(UINT msg, WPARAM wp, LPARAM lp) = 0;

  virtual LPCTSTR getClass(void) const
  {
    return TEXT("WinBase");
  }
public:
  //! 设置应用程序实例句柄
  void setInstance(HINSTANCE hInst)
  {
    hInst_ = hInst;
  }

  //! 获取应用程序实例句柄
  HINSTANCE getInstance(void) const
  {
    return hInst_;
  }

  //! 注册一般类型的窗口类
  ATOM regClass(HINSTANCE hInst, LPCTSTR className)
  {
    setInstance(hInst);
    
    WNDCLASS wc = {0};
    wc.hbrBackground = (HBRUSH)(BLACK_BRUSH);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance     = getInstance();
    wc.lpfnWndProc   = (WNDPROC)&WinBase::sWndProc;
    wc.lpszClassName = className;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    return RegisterClass(&wc);
  }

  //! 注册可扩展风格的窗口类
  ATOM regClassEx(HINSTANCE hInst, LPCTSTR className)
  {
    setInstance(hInst);

    WNDCLASSEX wc = {0};
    wc.cbSize        = sizeof(wc);
    wc.hbrBackground = (HBRUSH)(BLACK_BRUSH);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance     = getInstance();
    wc.lpfnWndProc   = (WNDPROC)&WinBase::sWndProc;
    wc.lpszClassName = className;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    return RegisterClassEx(&wc);
  }

  //! 创建一般风格的窗口
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
  static LRESULT CALLBACK sWndProc(HWND h, UINT msg, WPARAM wp, LPARAM lp)
  {
    WinBase* self = NULL;
    if (WM_NCCREATE == msg) {
      self = (WinBase*)((LPCREATESTRUCT)lp)->lpCreateParams;
      self->hwnd_ = h;
      SetWindowLong(h, GWL_USERDATA, (LONG_PTR)self);
    }
    if ((NULL != (self = (WinBase*)GetWindowLong(h, GWL_USERDATA))) && (self->hwnd_ == h))
      self->vWndProc(msg, wp, lp);
    return DefWindowProc(h, msg, wp, lp);
  }
};

#endif  // __WINBASE_HEADER_H__
