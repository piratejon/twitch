/**
  meaningful_error.c
  MessageBox with the string of the error code
  **/

#include <windows.h>

#include "meaningful_error.h"

void mb_error ( DWORD error, void * title ) {
  // adapted from <http://msdn.microsoft.com/en-us/library/windows/desktop/ms680582%28v=vs.85%29.aspx>, 12/21/2011
  LPVOID lpMsgBuf;
  FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
      NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL );
  MessageBox ( NULL, lpMsgBuf, title, MB_OK );
  LocalFree(lpMsgBuf);
}

