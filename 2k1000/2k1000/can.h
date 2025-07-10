#ifndef CAN_H
#define CAN_H
#include "qcustomplot.h"
#include <QMainWindow>
#include <QStandardItemModel>

class QToolButton;
class QFrame;
namespace Ui {
class CAN;
}

class CAN : public QMainWindow
{
    Q_OBJECT

public:
    explicit CAN(QWidget *parent = nullptr);
    ~CAN();
    void updatePlot();
    void initial_customplot();
    void initial_ui();
    void setupToolButton(QToolButton *button, const QString &iconPath, const QString &text, QFrame *leftLineFrame, QFrame *rightLineFrame = nullptr);
    void initial_tableView();
    void insertSampleData();
signals:
    void back_mainWindow();
    void back_OscilloscopeWindow();
    void back_recordWindow();
    void back_devstatusWindow();
    void sendMessage(uint32_t number,QString t_time,uint32_t point,QString emergency,QString t_type,QString suggestion);

private slots:
    void on_toolButton_mainWindow_clicked();

    void on_toolButton_Oscilloscope_clicked();

    void on_toolButton_Record_clicked();

    void on_toolButton_deviceStatus_clicked();

    void on_pushButton_clicked();

private:
    Ui::CAN *ui;
    QCustomPlot *customPlot1;
    QCPAxisRect *axisRect1;                     // 第一个图表
    QStandardItemModel *theModel;
    QTimer *dataTimer;                          // 更新数据定时器
};

#endif
