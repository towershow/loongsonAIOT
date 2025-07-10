#ifndef SYS_STATUS_H
#define SYS_STATUS_H
#include <QToolButton>
#include <QMainWindow>
#include <QFrame>
#include <QStandardItemModel>
namespace Ui {
class sys_status;
}

class sys_status : public QMainWindow
{
    Q_OBJECT

public:
    explicit sys_status(QWidget *parent = nullptr);
    ~sys_status();

    void initial_ui();
    void setupToolButton(QToolButton *button, const QString &iconPath, const QString &text, QFrame *leftLineFrame, QFrame *rightLineFrame = nullptr);
    void initial_tableView();
    void insertSampleData();

signals:
    void back_mainWindow();
    void back_OscilloscopeWindow();
    void back_recordWindow();
    void back_CANWindow();
private slots:
    void on_toolButton_Record_clicked();

    void on_toolButton_CAN_clicked();

    void on_toolButton_Oscilloscope_clicked();

    void on_toolButton_mainWindow_clicked();

private:
    Ui::sys_status *ui;
    QStandardItemModel *theModel;

};

#endif // SYS_STATUS_H
