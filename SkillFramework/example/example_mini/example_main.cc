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
#include "example_mini.h"

#define unreference_arg(arg) { (arg) = (arg); }


int 
main(int argc, char* argv[], char* envp[])
{
  unreference_arg(argc);
  unreference_arg(argv);
  unreference_arg(envp);

  char ch = 0;
  char cmd[100] = {0};
  fprintf(stdout, "Welcome, please type 'exit' to quit or 'help' for help menu\n");

  for ( ; ; )
  {
    fprintf(stdout, "> ");
    int i = 0;
    for ( ; ; )
    {
      ch = (char)fgetc(stdin);
      if ('\r' == ch || '\n' == ch)
      {
        cmd[i] = 0;
        break;
      }
      cmd[i++] = ch;
    }

    if (0 == i)
      continue;
    else if (0 == stricmp("exit", cmd)) 
      break;
    example_mini::Example* pEx = example_mini::ExampleMgr::singleton().getExample(cmd);
    if (NULL != pEx)
      pEx->do_Example();
    else 
    {
      fprintf(stdout, "You typed wrong command, please type 'exit' to quit or 'help' for help menu\n");
      continue;
    }
  }

  return 0;
}
