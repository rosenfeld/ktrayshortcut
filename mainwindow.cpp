#include "mainwindow.h"
#include "ui_mainwindow.h"


#include "registeredapplication.h"
#include <KActionCollection>
#include <KSystemTrayIcon>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    shortcutsEditor = new KShortcutsEditor(this, KShortcutsEditor::GlobalAction);

    KAction *action = addCustomAction("main settings window");
    connect(action, SIGNAL(triggered()), SLOT(toggle()));

    ui->gridLayout->addWidget(shortcutsEditor);

    (new KSystemTrayIcon("configure", this))->show();

    connect(ui->findApplicationButton, SIGNAL(clicked()), this, SLOT(findApplicationClick()));
}

MainWindow::~MainWindow()
{
    delete shortcutsEditor;
    delete ui;
}

KAction* MainWindow::addCustomAction(const QString &name) {
    KActionCollection *ac = new KActionCollection(this);
    KAction* action = ac->addAction(name);
    action->setText(name);
    action->setGlobalShortcut(KShortcut(),
                              KAction::ActiveShortcut | KAction::DefaultShortcut,
                              KAction::NoAutoloading);
    shortcutsEditor->addCollection(ac);
    return action;
}

void MainWindow::toggle() {
    if (isHidden()) show();
    else hide();
}

void MainWindow::findApplicationClick() {
    (new RegisteredApplication(this))->grabWindow();
}
