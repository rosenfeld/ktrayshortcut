#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <KShortcutsEditor>
#include <KAction>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    KAction* addCustomAction(const QString &name);

public slots:
    void toggle();
    void findApplicationClick();

private:
    Ui::MainWindow *ui;
    KShortcutsEditor *shortcutsEditor;
};

#endif // MAINWINDOW_H
