#include <KApplication>
#include <KCmdLineArgs>
#include <KAboutData>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    KAboutData aboutData("ktrayshortcut", 0, ki18n("ktrayshortcut"), "0.1");
    KCmdLineArgs::init(argc, argv, &aboutData);
    KApplication app;

    MainWindow main;
    main.show();
    app.setTopWidget(&main);

    return app.exec();
}
