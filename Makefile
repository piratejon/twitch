all:
	rc /fodialog.res dialog.rc
	cl /W4 dialog_message_loop.c dialogproc.c meaningful_error.c dialog.res user32.lib /Fetwitch.exe

clean:
	del *.exe *.obj *.res

