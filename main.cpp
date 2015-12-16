#include "probegui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProbeGUI w;
    w.show();

    return a.exec();
}
