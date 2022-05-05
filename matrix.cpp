#include <iostream>
#include <random>
#include <fstream>
#include <cmath>
#include "matrix.h"

Matrix::Matrix(int n, bool temp, bool fill){
    /*
    temp = false - ввод матрицы вручную
    temp = true - нулевая матрица (без ввода элементов вручную)
    fill = true - заполнение матрицы случайными вещественными числами от -3 до 3
    */
    this->n = n;
    exist_condition = true;
    arr = new double[n*n]{0};
    if(!temp){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                std::cout << "Введите [" << i << "][" << j << "]: ";
                std::cin >> arr[i*n+j];
            }
        }
    }
    if(fill && temp){
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_real_distribution<> distr(-3, 3);
        for(int i = 0; i < n*n; i++){
            arr[i] = distr(eng);
        }
    }
}
Matrix::Matrix(const Matrix &A){
    //std::cout << "Работает конструктор копирования" << std::endl;
    exist_condition = A.exist_condition;
    n = A.n;
    arr = new double[A.n * A.n];
    for(int i = 0; i < A.n * A.n; i++){
        arr[i] = A.arr[i];
    }
}
Matrix& Matrix::operator=(const Matrix &matrix){
    if(this == &matrix)
        return *this;
    arr = new double[matrix.n * matrix.n];
    exist_condition = true;
    for(int i = 0; i < matrix.n * matrix.n; i++){
        arr[i] = matrix.arr[i];
    }
    return *this;
}
bool operator==(const Matrix &matrix_1, const Matrix &matrix_2){
    if(matrix_1.get_size() != matrix_2.get_size())
        return false;
    int k = matrix_1.get_size();
    for(int i = 0; i < k; i++){
        for(int j = 0; j < k; j++){
            if(matrix_1.at(i, j) != matrix_2.at(i, j))
               return false;
        }
    }
    return true;
}
bool operator!=(const Matrix &matrix_1, const Matrix &matrix_2){
    return !(matrix_1 == matrix_2);
}
Matrix::Matrix(std::string name){
    std::ifstream f(name);
    if(!f.is_open()){
        //std::cout << "Файл не был открыт. Создастся матрица 1х1." << std::endl;
        arr = new double[1];
        exist_condition = false;
    }
    else{
        f >> n;
        arr = new double[n * n];
        for(int i = 0; i < n * n; i++){
            f >> arr[i];
        }
        f.close();
        exist_condition = true;
    }
}
Matrix::~Matrix(){
   //std::cout << "Работает деструктор" << std::endl;
   delete[] arr;
}
void Matrix::vivod(){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            std::cout << arr[i*n+j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
Matrix operator+(const Matrix &matrix_1, const Matrix &matrix_2){
    Matrix temp(matrix_1.n, true);
    for(int i = 0; i < matrix_1.n*matrix_1.n; i++)
        temp.arr[i] = matrix_1.arr[i] + matrix_2.arr[i];
    return temp;
}
Matrix operator-(const Matrix &matrix_1, const Matrix &matrix_2){
    Matrix temp(matrix_1.n, true);
    for(int i = 0; i < matrix_1.n*matrix_1.n; i++)
        temp.arr[i] = matrix_1.arr[i] - matrix_2.arr[i];
    return temp;
}
Matrix operator*(const Matrix &matrix_1, const Matrix &matrix_2){
    Matrix temp(matrix_1.n, true);
    for(int k = 0; k < matrix_1.n; k++){
        for(int i = 0; i < matrix_1.n; i++){
            for(int j = 0; j < matrix_1.n; j++){
                temp.arr[i * matrix_1.n + j] += matrix_1.arr[i * matrix_1.n + k] * matrix_2.arr[k * matrix_1.n + j];
            }
        }
    }
    return temp;
}
Matrix shtrassen(const Matrix &A, const Matrix &B){
    int n = A.get_size();
    if(n <= 64){
        return A * B;
    }
    Matrix A_11 = A.get_submatrix(0, 0, n/2), A_12 = A.get_submatrix(0, n/2, n/2), A_21 = A.get_submatrix(n/2, 0, n/2), A_22 = A.get_submatrix(n/2, n/2, n/2);
    Matrix B_11 = B.get_submatrix(0, 0, n/2), B_12 = B.get_submatrix(0, n/2, n/2), B_21 = B.get_submatrix(n/2, 0, n/2), B_22 = B.get_submatrix(n/2, n/2, n/2);
    Matrix P_1 = shtrassen(A_11 + A_22, B_11 + B_22);
    Matrix P_2 = shtrassen(A_21 + A_22, B_11);
    Matrix P_3 = shtrassen(A_11, B_12 - B_22);
    Matrix P_4 = shtrassen(A_22, B_21 - B_11);
    Matrix P_5 = shtrassen(A_11 + A_12, B_22);
    Matrix P_6 = shtrassen(A_21 - A_11, B_11 + B_12);
    Matrix P_7 = shtrassen(A_12 - A_22, B_21 + B_22);
    Matrix C_11 = P_1 + P_4 - P_5 + P_7;
    Matrix C_12 = P_3 + P_5;
    Matrix C_21 = P_2 + P_4;
    Matrix C_22 = P_1 - P_2 + P_3 + P_6;
    Matrix C(n, true);
    for(int i = 0; i < n/2; i++){
        for(int j = 0; j < n/2; j++){
            C.arr[i * n + j] = C_11.arr[i * n/2 + j];
            C.arr[i * n + j + n/2] = C_12.arr[i * n/2 + j];
            C.arr[(i + n/2) * n + j] = C_21.arr[i * n/2 + j];
            C.arr[(i+n/2)*n + j + n/2] = C_22.arr[i * n/2 + j];
        }
    }
    return C;
}
Matrix vinograd_shtrassen(const Matrix &A, const Matrix &B){
    int n = A.get_size();
    if(n <= 64)
        return A * B;
    Matrix A_11 = A.get_submatrix(0, 0, n/2), A_12 = A.get_submatrix(0, n/2, n/2), A_21 = A.get_submatrix(n/2, 0, n/2), A_22 = A.get_submatrix(n/2, n/2, n/2);
    Matrix B_11 = B.get_submatrix(0, 0, n/2), B_12 = B.get_submatrix(0, n/2, n/2), B_21 = B.get_submatrix(n/2, 0, n/2), B_22 = B.get_submatrix(n/2, n/2, n/2);
    Matrix S_1 = A_21 + A_22;
    Matrix S_2 = S_1 - A_11;
    Matrix S_3 = A_11 - A_21;
    Matrix S_4 = A_12 - S_2;
    Matrix S_5 = B_12 - B_11;
    Matrix S_6 = B_22 - S_5;
    Matrix S_7 = B_22 - B_12;
    Matrix S_8 = S_6 - B_21;
    Matrix P_1 = vinograd_shtrassen(S_2, S_6);
    Matrix P_2 = vinograd_shtrassen(A_11, B_11);
    Matrix P_3 = vinograd_shtrassen(A_12, B_21);
    Matrix P_4 = vinograd_shtrassen(S_3, S_7);
    Matrix P_5 = vinograd_shtrassen(S_1, S_5);
    Matrix P_6 = vinograd_shtrassen(S_4, B_22);
    Matrix P_7 = vinograd_shtrassen(A_22, S_8);
    Matrix T_1 = P_1 + P_2;
    Matrix T_2 = T_1 + P_4;
    Matrix C_11 = P_2 + P_3;
    Matrix C_12 = T_1 + P_5 + P_6;
    Matrix C_21 = T_2 - P_7;
    Matrix C_22 = T_2 + P_5;
    Matrix C(n, true);
    for(int i = 0; i < n/2; i++){
        for(int j = 0; j < n/2; j++){
            C.arr[i * n + j] = C_11.arr[i * n/2 + j];
            C.arr[i * n + j + n/2] = C_12.arr[i * n/2 + j];
            C.arr[(i + n/2) * n + j] = C_21.arr[i * n/2 + j];
            C.arr[(i+n/2)*n + j + n/2] = C_22.arr[i * n/2 + j];
        }
    }
    return C;
}
Matrix operator*(double a, const Matrix &matrix_1){
    Matrix temp(matrix_1.n, true);
    for(int i = 0; i < matrix_1.n*matrix_1.n; i++){
        temp.arr[i] = matrix_1.arr[i] * a;
    }
    return temp;
}
Matrix operator*(const Matrix &matrix_1, double a){
    Matrix temp(matrix_1.n, true);
    for(int i = 0; i < matrix_1.n*matrix_1.n; i++){
        temp.arr[i] = matrix_1.arr[i] * a;
    }
    return temp;
}
double Matrix::at(int i, int j) const{
    return arr[i*n+j];
}
void Matrix::set(double value, int i, int j){
    arr[i*n+j] = value;
}
void Matrix::resize(int m){
    if(m == n)
        return;
    else if(m > n){
        double *temp = new double[m*m]{0};
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                temp[i * m + j] = arr[i * n + j];
            }
        }
        n = m;
        delete arr;
        arr = temp;
    }
    else if(m < n){
        double *temp = new double[m*m];
        for(int i = 0; i < m; i++){
            for(int j = 0; j < m; j++){
                temp[i * m + j] = arr[i * n + j];
            }
        }
        n = m;
        delete arr;
        arr = temp;
    }
}
int Matrix::get_size() const{
    return n;
}
Matrix Matrix::get_submatrix(int i_t, int j_t, int m) const{
    Matrix temp(m, true);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < m; j++){
            temp.arr[i * m + j] = arr[(i+i_t) * n + j+j_t];
        }
    }
    return temp;
}
void Matrix::to_file(std::string name, std::string path_to_file){
    std::ofstream f(path_to_file+"/"+name+".txt");
    f << n << "\n";
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            f << arr[i*n+j] << " ";
        }
        f << std::endl;
    }
    f.close();
}
bool Matrix::get_exist(){
    return exist_condition;
}
