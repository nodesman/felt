#include <QApplication>
#include <QIcon>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setApplicationName(QStringLiteral("Felt"));
    app.setApplicationDisplayName(QStringLiteral("Felt"));
    app.setOrganizationName(QStringLiteral("Nodesman"));
    app.setOrganizationDomain(QStringLiteral("nodesman.com"));

    const QIcon appIcon(QStringLiteral(":/icons/icon.png"));
    app.setWindowIcon(appIcon);

    MainWindow window;
    window.setWindowIcon(appIcon);
    window.show();

    return app.exec();
}
