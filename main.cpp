#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setApplicationName(QStringLiteral("Felt"));
    app.setApplicationDisplayName(QStringLiteral("Felt"));
    app.setOrganizationName(QStringLiteral("Nodesman"));
    app.setOrganizationDomain(QStringLiteral("nodesman.com"));

    MainWindow window;
    window.show();

    return app.exec();
}
