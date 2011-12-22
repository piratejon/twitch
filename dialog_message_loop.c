/**
  boilerplate for a basic WinMain + CreateDialog + DlgProc
  **/

#include <windows.h>

#include "meaningful_error.h"

#include "dialogproc.h"
#include "dialog.h"

/**
Source:
  <http://msdn.microsoft.com/en-us/library/windows/desktop/ms633559%28v=vs.85%29.aspx>
  12/21/2011
  **/
int CALLBACK WinMain(
  __in  HINSTANCE hInstance,
  __in  HINSTANCE hPrevInstance,
  __in  LPSTR lpCmdLine,
  __in  int nCmdShow
) {

  return DialogBoxParam ( GetModuleHandle(NULL), ID_DLG, NULL, dlgproc, NULL );
}

