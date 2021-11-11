#ifndef RESOURCEINTESIVETASK_H
#define RESOURCEINTESIVETASK_H

#include <iostream>
#include <vector>

#include <QThread>
#include <QObject>
#include <QStringList>

struct Matrix
{
public:
    int maxRow;
    int maxCol;
    std::vector<std::vector<float>> matrixData;

    Matrix(int row, int col)
    {
        matrixData.resize(col, std::vector<float>(row, 0));
        maxRow = row;
        maxCol = col;
    }

    QStringList toStrinList()
    {
        QStringList result;
        for (auto col = matrixData.cbegin(); col != matrixData.cend(); ++col)
        {
            for (auto row = (*col).cbegin(); row != (*col).cend(); ++row)
            {
                result << QString::number(*row);
            }
            result << "\n";
        }
        return result;
    }
};

class ResourceIntensiveTask : public QObject
{
    Q_OBJECT

public:
    explicit ResourceIntensiveTask(Matrix *matrix, QObject *parent = nullptr);

    quint64 operationsNum();
    void start();
    ~ResourceIntensiveTask();

signals:
    void procedureFinished(Matrix &matrix);
    void procedureProgressChanged(quint64 value);

private:
    void countOperations();

    void getCofactor(const Matrix& A, Matrix& temp, int p, int q, int n);
    int determinant(Matrix& A, int n);
    void adjoint(Matrix& A, Matrix& adj);
    bool inverse(Matrix& A, Matrix& inverse);
    void display(Matrix A);

private:
    std::size_t mOperations = 0;

    Matrix *mRandomMatrix;
    Matrix *mInverseMatrix;

    std::size_t mProgressOperationNum = 0;

};

#endif // RESOURCEINTESIVETASK_H
