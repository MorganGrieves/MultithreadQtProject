#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStyledItemDelegate>
#include <QTableWidgetItem>
#include <QWidget>

#include "resourceintesivetask.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);   
    void addRow();
    ~MainWindow();

private slots:
    void onAddButtonRelease();
    void onProcedureChange(quint64 value);
    void onProcedureFinish(Matrix &matrix);

private:
    Ui::MainWindow *ui;
};

class ProgressBarDelegate : public QStyledItemDelegate
{
public:
    ProgressBarDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


#endif // MAINWINDOW_H
