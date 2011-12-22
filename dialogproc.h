#ifndef _BOILERPLATE_DIALOGPROC_H
#define _BOILERPLATE_DIALOGPROC_H

#include <windows.h>

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
);

#endif // _BOILERPLATE_DIALOGPROC_H

