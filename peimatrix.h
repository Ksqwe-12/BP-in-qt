#ifndef PEIMATRIX_H
#define PEIMATRIX_H
#include<Eigen/Dense>
#include<Eigen/Geometry>
#include<Eigen/Eigen>
using namespace Eigen;

class PeiMatrix
{
public:
    PeiMatrix();
    ~PeiMatrix();

    MatrixXd m_input = MatrixXd::Random(1,8);//输入矩阵
    MatrixXd m1 = MatrixXd::Random(8,9);//运算矩阵1
    MatrixXd m2 = MatrixXd::Ones(9,9);//运算矩阵2
    MatrixXd m3 = MatrixXd::Random(9,6);//运算矩阵3
    MatrixXd b1 = MatrixXd::Random(1,9);//偏置1
    MatrixXd b2 = MatrixXd::Random(1,9);//偏置2
    MatrixXd b3 = MatrixXd::Random(1,6);//偏置3
    void Matrix_init();
};

#endif // PEIMATRIX_H
