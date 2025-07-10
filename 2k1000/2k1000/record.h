#ifndef RECORD_H
#define RECORD_H

#include <QMainWindow>
#include <QStandardItemModel>

class QToolButton;
class QFrame;
namespace Ui {
class Record;
}

class Record : public QMainWindow
{
    Q_OBJECT

public:
    explicit Record(QWidget *parent = nullptr);
    ~Record();
    void initial_ui();
    void setupToolButton(QToolButton *button, const QString &iconPath, const QString &text, QFrame *leftLineFrame, QFrame *rightLineFrame = nullptr);

    void initial_tableView();
    void insertSampleData();
signals:
    void back_mainWindow();
    void back_OscilloscopeWindow();
    void back_CANWindow();
    void back_devstatusWindow();
public slots:
    void getMessage(uint32_t number,QString t_time,uint32_t point,QString emergency,QString t_type,QString suggestion);

private slots:
    void on_toolButton_mainWindow_clicked();

    void on_toolButton_Oscilloscope_clicked();

    void on_toolButton_CAN_clicked();

    void on_toolButton_deviceStatus_clicked();

private:
    Ui::Record *ui;
    QStandardItemModel *theModel;

};

#endif // RECORD_H
