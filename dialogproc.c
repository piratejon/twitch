#include <windows.h>

#include "dialogproc.h"
// #include "dialog.h"

/**
Source:
<http://msdn.microsoft.com/en-us/library/windows/desktop/ms645469%28v=vs.85%29.aspx>
12/21/2011
  **/
INT_PTR CALLBACK dlgproc(
  __in  HWND hwndDlg,
  __in  UINT uMsg,
  __in  WPARAM wParam,
  __in  LPARAM lParam
) {

  if ( WM_QUIT == uMsg ) {
    SetWindowLong(hwndDlg, DWL_MSGRESULT, wParam);
    return TRUE;
  }

  return FALSE; // did not handle the message
}

