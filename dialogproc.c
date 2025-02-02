#include <windows.h>
#include <strsafe.h>

#include "dialogproc.h"
#include "dialog.h"

#define TEST_TIMER_ID 999
#define REAL_TIMER_ID 123
#define TEST_TIMER_INTERVAL 100

typedef struct _tag_global {
  int started;
  int left_twitch_px;
  int right_twitch_px;
  int interval_s;
  int test_counter;
  UINT_PTR test_timer;
  UINT_PTR real_timer;
} Global; // global state

Global G = {0};

// sets the control values based on the global state
void state_to_controls ( HWND dlg, Global * g ) {
  TCHAR buf[10]; // ridiculous

  StringCchPrintf(buf, 10, "%d", g->left_twitch_px);
  SetDlgItemText(dlg, IDE_LEFT, buf);

  StringCchPrintf(buf, 10, "%d", g->right_twitch_px);
  SetDlgItemText(dlg, IDE_RIGHT, buf);

  StringCchPrintf(buf, 10, "%d", g->interval_s);
  SetDlgItemText(dlg, IDE_INTERVAL, buf);

  if ( g->started ) {
    SetDlgItemText(dlg, IDB_STARTSTOP, "Stop");
  } else {
    SetDlgItemText(dlg, IDB_STARTSTOP, "Start");
  }
}

// sets the global state based on the control values
void controls_to_state ( HWND dlg, Global * g ) {
  TCHAR buf[10]; // ridiculous

  GetDlgItemText(dlg, IDE_LEFT, buf, 10);
  g->left_twitch_px = atoi(buf);

  GetDlgItemText(dlg, IDE_RIGHT, buf, 10);
  g->right_twitch_px = atoi(buf);

  GetDlgItemText(dlg, IDE_INTERVAL, buf, 10);
  g->interval_s = atoi(buf);

  // we ought to know startstop already
}

void move_mouse ( LONG dx ) {
  INPUT i;
  MOUSEINPUT * mi;
 
  mi = &(i.mi);

  i.type = INPUT_MOUSE;
  mi->dx = dx;
  mi->dy = 0;
  mi->mouseData = 0;
  mi->dwFlags = MOUSEEVENTF_MOVE;
  mi->time = 0;
  mi->dwExtraInfo = 0;

  SendInput(1, &i, sizeof i);
}

void do_twitch ( Global * g ) {
  move_mouse ( -g->left_twitch_px );
  Sleep(50); // called from a timer callback, should we really be sleeping here?
  move_mouse ( g->right_twitch_px );
}

void EnableItems ( HWND hwnd, DWORD f ) {
  EnableWindow(GetDlgItem(hwnd, IDE_LEFT), f);
  EnableWindow(GetDlgItem(hwnd, IDE_RIGHT), f);
  EnableWindow(GetDlgItem(hwnd, IDE_INTERVAL), f);
}

/**
Source:
<http://msdn.microsoft.com/en-us/library/windows/desktop/ms644907%28v=vs.85%29.aspx>
12/21/2011
**/
VOID CALLBACK timerproc(
  __in  HWND hwnd,
  __in  UINT uMsg,
  __in  UINT_PTR idEvent,
  __in  DWORD dwTime
) {
  if ( TEST_TIMER_ID == idEvent ) {
    do_twitch ( &G );
    if ( -- G.test_counter <= 0 ) {
      KillTimer ( hwnd, TEST_TIMER_ID );
      EnableWindow ( GetDlgItem(hwnd, IDB_TEST), TRUE );
      EnableItems ( hwnd, !G.started );
    }
  } else if ( REAL_TIMER_ID == idEvent ) {
    do_twitch ( &G );
  } else {
    // this should never happen! no messagebox because we don't know how often it shouldn't be happening
  }
}

void toggle_startstop ( HWND hwnd, Global * g ) {
  g->started = !g->started;

  if ( g->started ) {
    if ( g->interval_s ) {
      EnableItems ( hwnd, FALSE );
      g->real_timer = SetTimer ( hwnd, REAL_TIMER_ID, g->interval_s * 1000, timerproc );
    } else {
      MessageBox ( hwnd, "Require non-zero interval", "Whoops", MB_OK ); 
      g->started = FALSE;
    }
  } else {
    KillTimer ( hwnd, g->real_timer );
    g->real_timer = 0;
    EnableItems ( hwnd, TRUE );
  }
}

void test_twitch ( HWND hwnd, Global * g ) {
  EnableWindow(GetDlgItem(hwnd, IDB_TEST), FALSE);
  EnableItems(hwnd, FALSE);
  g->test_counter = 15; // how many iterations to test
  g->test_timer = SetTimer ( hwnd, TEST_TIMER_ID, TEST_TIMER_INTERVAL, timerproc );
}

/**
Source:
<http://msdn.microsoft.com/en-us/library/windows/desktop/ms645469%28v=vs.85%29.aspx>
12/21/2011

More help:
<http://www.winprog.org/tutorial/dialogs.html>
12/21/2011

More help:
<http://win32assembly.online.fr/tut10.html>
12/21/2011

  **/
INT_PTR CALLBACK dlgproc(
  __in  HWND hwndDlg,
  __in  UINT uMsg,
  __in  WPARAM wParam,
  __in  LPARAM lParam
) {

  switch (uMsg) {
    case WM_INITDIALOG:

      // set initial values in the global state
      G.started = FALSE;
      G.left_twitch_px = 2;
      G.right_twitch_px = 3;
      G.interval_s = 99;

      // populate the controls based on that
      state_to_controls(hwndDlg, &G);

      break;

    case WM_COMMAND:
      switch (LOWORD(wParam)) {

        case IDB_STARTSTOP:
          controls_to_state(hwndDlg, &G);
          toggle_startstop(hwndDlg, &G);
          state_to_controls(hwndDlg, &G);
          break;

        case IDB_TEST:
          controls_to_state(hwndDlg, &G);
          test_twitch(hwndDlg, &G);
          break;
      }
      break;
    case WM_CLOSE:
      EndDialog(hwndDlg, NULL);
      break;
    default:
      return FALSE;
  }

  return TRUE;
}

