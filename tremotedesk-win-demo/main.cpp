#include "tremotedeskwindemo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    tremotedeskwindemo w;
    w.show();
    return a.exec();
}
