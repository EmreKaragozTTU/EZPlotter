
#include "EZPMainConsole.h"
#include "EZPRenderWidgetD3D10.h"

#include "QtWidgets\qapplication.h"
#include "QtWidgets\qmainwindow.h"

#ifdef WIN32
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

int main(int argc, char** argv)
{
	QApplication App(argc, argv);
	
	QMainWindow* pMainWindow = new QMainWindow();
	pMainWindow->setMinimumSize(640, 480);

	EZPMainConsole* pConsole = EZPMainConsole::getInstance(pMainWindow);
	pMainWindow->setFocusProxy((QWidget*)pConsole);
	pMainWindow->setCentralWidget((QWidget*)pConsole);
	pMainWindow->show();

	return App.exec();
}