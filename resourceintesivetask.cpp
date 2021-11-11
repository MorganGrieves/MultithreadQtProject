#include "resourceintesivetask.h"
#include <QDebug>
#include <QApplication>

ResourceIntensiveTask::ResourceIntensiveTask(Matrix *matrix, QObject *parent) : QObject(parent)
{
    mRandomMatrix = matrix;
    mRandomMatrix->matrixData = matrix->matrixData;

    mInverseMatrix = new Matrix(matrix->maxCol, matrix->maxCol);
    countOperations();

}

void ResourceIntensiveTask::start()
{
    mProgressOperationNum = 0;
    if (inverse(*mRandomMatrix, *mInverseMatrix))
    {
        this->moveToThread(QApplication::instance()->thread());
        emit procedureFinished(*mInverseMatrix);
        return;
    }
    emit procedureFinished(*mInverseMatrix);
}

quint64 ResourceIntensiveTask::operationsNum()
{
    return mOperations;
}

ResourceIntensiveTask::~ResourceIntensiveTask()
{
    delete mInverseMatrix;
    delete mRandomMatrix;
}

void ResourceIntensiveTask::countOperations()
{
    int matSize = mRandomMatrix->maxCol;

    mOperations = 0;

    //inverse func
    for (int i = 1; i < matSize; i++)
    {
        //determinant func
        int determinant = 1; //determinant function amount
        for (int j = 0; j < matSize; j++)
            mOperations += matSize * matSize; //getCofactor

        for (int j = matSize; j > 2; j--)
            determinant *= j;
        mOperations += determinant;

        //adjoint func
        determinant = 1; //determinant function amount
        for (int j = 0; j < matSize; j++)
            mOperations += matSize * matSize; //getCofactor

        for (int j = matSize; j > 2; j--)
            determinant *= j;
        mOperations += determinant;
    }

    mOperations += matSize * matSize;
}

void ResourceIntensiveTask::getCofactor(const Matrix& A, Matrix& temp, int p, int q, int n)
{
    int i = 0, j = 0;

    for (int row = 0; row < n; row++)
        for (int col = 0; col < n; col++)
        {
            emit procedureProgressChanged(++mProgressOperationNum);
            if (row != p && col != q)
            {
                temp.matrixData[i][j++] = A.matrixData[row][col];

                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
}

int ResourceIntensiveTask::determinant(Matrix& A, int n)
{
    int D = 0;

    if (n == 2)
        return ((A.matrixData[0][0] * A.matrixData[1][1]) - (A.matrixData[1][0] * A.matrixData[0][1]));

    Matrix temp(A.maxRow - 1, A.maxCol - 1);

    int sign = 1;

    for (int f = 0; f < n; f++)
    {
        QThread::msleep(1000);

        emit procedureProgressChanged(++mProgressOperationNum);

        getCofactor(A, temp, 0, f, n);
        D += sign * A.matrixData[0][f] * determinant(temp, n - 1);

        sign = -sign;
    }

    return D;
}

void ResourceIntensiveTask::adjoint(Matrix& A, Matrix& adj)
{
    if (A.matrixData.size() == 1)
    {
        adj.matrixData[0][0] = 1;
        return;
    }

    int sign = 1;

    Matrix temp(A.maxRow, A.maxCol);

    for (std::size_t i = 0; i < A.matrixData.size(); i++)
    {
        for (std::size_t j = 0; j < A.matrixData.size(); j++)
        {
            emit procedureProgressChanged(++mProgressOperationNum);

            getCofactor(A, temp, i, j, A.matrixData.size());
            sign = ((i + j) % 2 == 0) ? 1 : -1;

            adj.matrixData[i][j] = (sign) * (determinant(temp, A.matrixData.size() - 1));
        }
    }
}

bool ResourceIntensiveTask::inverse(Matrix& A, Matrix& inverse)
{
    int det = determinant(A, A.matrixData.size());

    if (det == 0)
    {
        std::cout << "Singular matrix, can't find its inverse";
        return false;
    }

    Matrix adj(A.maxRow, A.maxCol);

    adjoint(A, adj);

    for (std::size_t i = 0; i < A.matrixData.size(); i++)
        for (std::size_t j = 0; j < A.matrixData.size(); j++)
        {
            emit procedureProgressChanged(++mProgressOperationNum);

            inverse.matrixData[i][j] = adj.matrixData[i][j] / float(det);
        }
    return true;
}

void ResourceIntensiveTask::display(Matrix A)
{
    for (std::size_t i = 0; i < A.matrixData.size(); i++)
    {
        for (std::size_t j = 0; j < A.matrixData.size(); j++)
            std::cout << A.matrixData[i][j] << " ";
        std::cout << std::endl;
    }
}

