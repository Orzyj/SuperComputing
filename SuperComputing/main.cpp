#include "SuperComputing.h"
#include <QtWidgets/QApplication>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    app.setStyleSheet(
        "QMainWindow { background-color: #F5F7FA; }"
        "QLabel { color: #2C3E50; font-size: 13px; font-family: 'Segoe UI', Arial; }"
        "QPushButton { background-color: #3498DB; color: white; border-radius: 5px; padding: 8px 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #2980B9; }"
        "QPushButton:pressed { background-color: #1F618D; }"
        "QLineEdit { border: 2px solid #BDC3C7; border-radius: 5px; padding: 4px; }"
        "QLineEdit:focus { border: 2px solid #3498DB; }"
        "QTabWidget::pane { border: 1px solid #BDC3C7; background-color: white; border-radius: 5px; margin-top: -1px; }"
        "QTabBar::tab { background-color: #ECF0F1; color: #7F8C8D; border: 1px solid #BDC3C7; padding: 8px 20px; border-top-left-radius: 5px; border-top-right-radius: 5px; margin-right: 2px; }"
        "QTabBar::tab:selected { background-color: white; color: #2C3E50; border-bottom-color: white; font-weight: bold; }"
        "QCheckBox { color: #2C3E50; font-size: 13px; spacing: 8px; }"
        "QCheckBox:hover { color: #3498DB; }"
    );

    SuperComputing window;

    window.show();
    return app.exec();
}
