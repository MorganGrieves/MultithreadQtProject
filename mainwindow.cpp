#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>

static const int PROGRESS_BAR_HEIGHT_PX = 30;

static const QStringList TABLE_COLUMN_LABELS = QStringList() << "Progress";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->procedureTableWidget->setColumnCount( TABLE_COLUMN_LABELS.count() );
    ui->procedureTableWidget->setHorizontalHeaderLabels( TABLE_COLUMN_LABELS );
    ui->procedureTableWidget->horizontalHeader()->setStretchLastSection( true );
    ui->procedureTableWidget->setItemDelegateForColumn( 0, new ProgressBarDelegate );


    connect(ui->addButton, &QPushButton::released,
            this, &MainWindow::onAddButtonRelease);
}

void MainWindow::addRow()
{
    int currentRow = ui->procedureTableWidget->rowCount();
    ui->procedureTableWidget->insertRow(currentRow);
    if (QTableWidgetItem *item = new QTableWidgetItem("0"))
    {       
        item->setFlags( item->flags() ^ Qt::ItemIsEditable );
        Matrix *matrix = new Matrix(3, 3);
        matrix->matrixData =
        {
            {1, 0, 5},
            {2, 1, 6},
            {3, 4, 0}
        };
        ResourceIntensiveTask *task = new ResourceIntensiveTask(matrix);
        qDebug() << currentRow;
        task->setProperty("row", currentRow);
        task->setProperty("column", 0);
        connect(task, &ResourceIntensiveTask::procedureProgressChanged,
                this, &MainWindow::onProcedureChange);

        QThread *thread = new QThread;
        task->moveToThread(thread);
        connect(thread, &QThread::started,
                task, &ResourceIntensiveTask::start);
        connect(task, &ResourceIntensiveTask::procedureFinished,
                thread, &QThread::quit);
        connect(task, &ResourceIntensiveTask::procedureFinished,
                this, &MainWindow::onProcedureFinish);
        connect(thread, &QThread::finished,
                thread, &QThread::deleteLater);

        item->setData(Qt::UserRole, task->operationsNum());

        ui->procedureTableWidget->setItem(currentRow, 0, item);

        thread->start();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAddButtonRelease()
{
    addRow();
}

void MainWindow::onProcedureChange(quint64 value)
{
    if(ResourceIntensiveTask* task = qobject_cast<ResourceIntensiveTask*>(sender()))
    {
        if (QTableWidgetItem* item = ui->procedureTableWidget->item( task->property("row").toInt(), 0 ) )
            item->setData( Qt::DisplayRole, value );
    }
}

void MainWindow::onProcedureFinish(Matrix &matrix)
{
    qDebug() << "finished" << sender();
    if(ResourceIntensiveTask* task = qobject_cast<ResourceIntensiveTask*>(sender()))
    {
        if (QTableWidgetItem* item = ui->procedureTableWidget->item( task->property("row").toInt(), 0 ) )
        {
            qDebug() << "item " << item << task->property("row").toInt();
            item->setData( Qt::DisplayRole, item->data(Qt::UserRole).toULongLong());

        }
        update();
    }
}

ProgressBarDelegate::ProgressBarDelegate(QObject *parent) : QStyledItemDelegate(parent)
{}

void ProgressBarDelegate::paint(
        QPainter *painter,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    int progress = index.data().toULongLong();
    quint64 maxProgressValue = index.data(Qt::UserRole).toULongLong();
    QStyleOptionProgressBar progressBarOption;
    QRect r = option.rect;
    r.setHeight( PROGRESS_BAR_HEIGHT_PX );
    r.moveCenter( option.rect.center() );
    progressBarOption.rect = r;
    progressBarOption.textAlignment = Qt::AlignCenter;
    progressBarOption.minimum = 0;
    progressBarOption.maximum = 100;
    progressBarOption.progress = qRound(progress * 100.0 / maxProgressValue);
    progressBarOption.text = QString::number( qRound(progress * 100.0 / maxProgressValue) ) + "%";
    progressBarOption.textVisible = true;

    QStyledItemDelegate::paint( painter, option, QModelIndex() );
    QApplication::style()->drawControl( QStyle::CE_ProgressBar, &progressBarOption, painter );
}
