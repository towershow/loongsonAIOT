#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QFrame>
class Oscilloscope;
class CAN;
class Record;
class sys_status;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initial_ui();
    void setupToolButton(QToolButton *button, const QString &iconPath, const QString &text, QFrame *leftLineFrame, QFrame *rightLineFrame = nullptr);

private slots:
    void on_toolButton_Oscilloscope_clicked();
    void on_toolButton_CAN_clicked();

    void on_toolButton_Record_clicked();

    void on_toolButton_deviceStatus_clicked();

private:
    Ui::MainWindow *ui;
    Oscilloscope *Oscilloscope_ui;
    CAN *can_ui;
    Record *record_ui;
    sys_status *sys_ui;
};
#endif // MAINWINDOW_H
