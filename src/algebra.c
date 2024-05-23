#include "algebra.h"
#include <stdio.h>
#include <math.h>

Matrix create_matrix(int row, int col)
{
    Matrix m;
    m.rows = row;
    m.cols = col;
    return m;
}

Matrix add_matrix(Matrix a, Matrix b)
{
    // ToDo
    Matrix m;
    int i, k;
    if (a.rows == b.rows && a.cols == b.cols)
    {
        m = create_matrix(a.rows, a.cols);
        for (i = 0; i < a.rows; i++)
        {
            for (k = 0; k < a.cols; k++)
            {
                m.data[i][k] = b.data[i][k] + a.data[i][k];
            }
        }
        return m;
    }
    else
    {
        printf("Error: Matrix a and b must have the same rows and cols.\n");
        return create_matrix(0, 0);
    }
}

Matrix sub_matrix(Matrix a, Matrix b)
{
    // ToDo
    Matrix m;
    int i, k;
    if (a.rows == b.rows && a.cols == b.cols)
    {
        m = create_matrix(a.rows, a.cols);
        for (i = 0; i < a.rows; i++)
        {
            for (k = 0; k < a.cols; k++)
            {
                m.data[i][k] = a.data[i][k] - b.data[i][k];
            }
        }
        return m;
    }
    else
    {
        printf("Error: Matrix a and b must have the same rows and cols.\n");
        return create_matrix(0, 0);
    }
}

Matrix mul_matrix(Matrix a, Matrix b)
{
    // ToDo
    Matrix m;
    double m2;
    int i, k, j;
    if (a.cols == b.rows)
    {
        m = create_matrix(a.rows, b.cols);
        for (i = 0; i < a.rows; i++)
        {
            for (j = 0; j < b.cols; j++)
            {
                for (k = 0; k < a.cols; k++)
                {
                    m2 = a.data[i][k] * b.data[k][j];
                    m.data[i][j] = m.data[i][j] + m2;
                    m2 = 0;
                }
            }
        }
        return m;
    }
    else
    {
        printf("Error: The number of cols of matrix a must be equal to the number of rows of matrix b.\n");

        return create_matrix(0, 0);
    }
}

Matrix scale_matrix(Matrix a, double k)
{
    // ToDo
    Matrix m = create_matrix(a.rows, a.cols);
    int i, j;
    for (i = 0; i < a.rows; i++)
    {
        for (j = 0; j < a.cols; j++)
        {
            m.data[i][j] = a.data[i][j] * k;
        }
    }
    return m;
}

Matrix transpose_matrix(Matrix a)
{
    // ToDo
    Matrix m = create_matrix(a.cols, a.rows);
    int i, j;
    for (i = 0; i < a.rows; i++)
    {
        for (j = 0; j < a.cols; j++)
        {
            m.data[j][i] = a.data[i][j];
        }
    }
    return m;
}
// 做到这里
double det_matrix(Matrix a)
{
    // ToDo
    double result;
    if (a.cols == a.rows)
    {
        int order = a.cols;             // 矩阵阶数
        result = determinant(a, order); // 调用一个行列式计算函数determinant
        return result;
    }
    else
    {
        printf("Error: The matrix must be a square matrix.\n");
        return 0;
    }
}
// 建立一个行列式计算函数determinant
double determinant(Matrix a, int order)
{
    double result = 0, cofactor;
    int sign = 1, i; // cofactor是余子式，sign和代数余子式符号相关，向下一行，sign乘-1
    if (order == 1)
        result = a.data[0][0]; // 矩阵1阶，则行列式为00项
    else
        for (i = 0; i < order; i++)
        {
            cofactor = laplace_expansion(a, i, 0, order);     // 定义一个计算余子式值的函数
            result = sign * a.data[i][0] * cofactor + result; // 按第一列展开，逐行向下累加
            sign = sign * (-1);                               // 代数余子式符号正负交替
        }
    return result;
}
// 建立一个余子式计算函数lapalce_expansion
double laplace_expansion(Matrix a, int r, int c, int order) //(r,c)表示现在以第r行，第c列的项展开
{
    double result;
    Matrix m = create_matrix(order - 1, order - 1); // 定义一个存放余子式的矩阵m
    int i, j;
    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
            m.data[i][j] = a.data[i][j];
    }
    for (i = r; i < order - 1; i++)
    {
        for (j = 0; j < c; j++)
            m.data[i][j] = a.data[i + 1][j];
    }
    for (i = 0; i < r; i++)
    {
        for (j = c; j < order - 1; j++)
            m.data[i][j] = a.data[i][j + 1];
    }
    for (i = r; i < order - 1; i++)
    {
        for (j = c; j < order - 1; j++)
            m.data[i][j] = a.data[i + 1][j + 1];
    } // 余子式各项与原来矩阵关系，构建出余子式的矩阵
    result = determinant(m, order - 1); // 这个余子式需重新调用determinant函数，阶为order-1，进行递归
    return result;
}

Matrix inv_matrix(Matrix a)
{
    // ToDo
    if (a.rows == a.cols)
    {
        double m = det_matrix(a);
        if (m == 0)
        {
            printf("Error: The matrix is singular.\n");
            return create_matrix(0, 0);
        }
        else // m为行列式的值不为0，只需求伴随矩阵即可，伴随矩阵第r行第c列是矩阵第c行第r列的代数余子式
        {
            Matrix A = create_matrix(a.rows, a.cols); // 定义一个矩阵作为伴随矩阵,注意要确定其规模，否则运算无结果r
            Matrix B = create_matrix(a.rows, a.cols); // 定义一个矩阵作为逆矩阵
            if (a.rows == 1)                          // 阶为1，直接算
                B.data[0][0] = 1 / a.data[0][0];
            else // 阶不为1
            {
                int r, c, i, j;
                for (r = 0; r < a.rows; r++)
                {
                    for (c = 0; c < a.cols; c++)
                    {
                        int t = ((r + c) % 2 ? -1 : 1);
                        A.data[r][c] = t * laplace_expansion(a, c, r, a.rows);
                    }
                }
                for (i = 0; i < a.rows; i++)
                {
                    for (j = 0; j < a.cols; j++)
                    {
                        B.data[i][j] = A.data[i][j] / m;
                    }
                }
            }
            return B;
        }
    }
    else
    {
        printf("Error: The matrix must be a square matrix.\n");
        return create_matrix(0, 0);
    }
    return create_matrix(0, 0);
}

int rank_matrix(Matrix a)
{
    // ToDo
    float times; // 倍数
    int i, j, k;
    for (i = 0; i < a.rows - 1; i++)
    {
        for (k = i + 1; k < a.rows; k++)
        {
            j = 0;
            while (a.data[i][j] == 0)
                j++; // 每一行找第一个非零元素
            if (a.data[i][j] != 0)
            {
                times = a.data[k][j] / a.data[i][j];
                for (j = 0; j < a.cols; j++)
                    a.data[k][j] -= a.data[i][j] * times;
            } // 将该元素下方元素全部变为零
        }
    }
    for (i = 0; i < a.rows; i++)
    {
        j = 0;
        while (a.data[i][j] == 0)
            j++;
        if (a.data[i][j] != 0)
        {
            times = a.data[i][j];
            for (j = 0; j < a.cols; j++)
                a.data[i][j] /= times;
        }
    } // 将每一行的第一个非零数化为1
    // 变化为标准的阶梯型，第一个非零数前的零越多，越靠后
    int total[120] = {0}; // 用于存放每一行的零的个数
    for (i = 0; i < a.rows; i++)
        for (j = 0; j < a.cols; j++)
            if (a.data[i][j] == 0)
                total[i]++;
            else
                break;
    // 进行冒泡法排序
    int m;
    double n; // 存放元素
    for (m = a.rows - 1; m > 0; m--)
        for (i = 0; i < m; i++)
            if (total[m] < total[i])
                for (j = 0; j < a.cols; j++)
                {
                    n = a.data[m][j];
                    a.data[m][j] = a.data[i][j];
                    a.data[i][j] = n;
                }
    // 进行约化，将每一个阶梯头（1）上方元素全变为0
    for (i = 0; i < a.rows; i++)
    {
        j = 0;
        while (a.data[i][j] == 0)
            j++;
        if (a.data[i][j] != 0)
            for (k = 0; k < i; k++)
            {
                times = a.data[k][j] / a.data[i][j];
                for (m = 0; m < a.cols; m++)
                    a.data[k][m] -= times * a.data[i][m];
            }
    }
    int count = 0, none_zero = 0; // 计数  秩，每一行往下，有1秩就加1
    for (i = 0; i < a.rows; i++)
    {
        for (j = 0; j < a.cols; j++)
        {
            if (a.data[i][j] != 0)
            {
                none_zero = 1;
                break;
            }
            if (none_zero == 1)
                count++;
            none_zero = 0;
        }
    }
    return count + 1;
}

double trace_matrix(Matrix a)
{
    // ToDo
    double trace = 0;
    int i;
    if (a.cols == a.rows)
    {
        for (i = 0; i < a.rows; i++)
        {
            trace = trace + a.data[i][i];
        }
        return trace;
    }
    else
    {
        printf("Error: The matrix must be a square matrix.\n");
        return 0;
    }
}

void print_matrix(Matrix a)
{
    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            // 按行打印，每个元素占8个字符的宽度，小数点后保留2位，左对齐
            printf("%-8.2f", a.data[i][j]);
        }
        printf("\n");
    }
}
