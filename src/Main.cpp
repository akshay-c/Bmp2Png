#include "include/bmp2png.hpp"
#include "include/mainwindow.hpp"

int main(int argc, char** argv)
{
	::QApplication app(argc, argv);

	MainWindow *Mw = new MainWindow();
	Mw->showMaximized();
	return app.exec();

}