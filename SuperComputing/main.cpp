#include "SuperComputing.h"
#include <QtWidgets/QApplication>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SuperComputing window;

    window.show();
    return app.exec();
}
