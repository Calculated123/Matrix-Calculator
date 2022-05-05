#include <iostream>

class Matrix{
public:
    //Конструктор (размер квадратной матрицы, заполнение вручную или случайными числами);
    Matrix(int n, bool temp=false, bool fill=false);
    //Конструктор (загрузка из файла)
    Matrix(std::string name);
    //Конструктор копирования
    Matrix(const Matrix &A);
    //Присваивание
    Matrix& operator=(const Matrix &matrix);
    //Равенство
    friend bool operator==(const Matrix &matrix_1, const Matrix &matrix_2);
    friend bool operator!=(const Matrix &matrix_1, const Matrix &matrix_2);
    //Деструктор
    ~Matrix();
    //Существование матрицы
    bool get_exist();
    //Размер матрицы
    int get_size() const;
    //Вывод матрицы
    void vivod();
    //Получение элемента квадратной матрицы по индексу
    double at(int i, int j) const;
    //Присваивание по индексу
    void set(double value, int i, int j);
    //Изменение размера матрицы
    void resize(int m);
    //Блок матрицы
    Matrix get_submatrix(int i_t, int j_t, int m) const;
    //Загрузка матрицы в файл
    void to_file(std::string name, std::string path_to_file);
    //Перегрузка сложения
    friend Matrix operator+(const Matrix &matrix_1, const Matrix &matrix_2);
    //Перегрузка вычитания
    friend Matrix operator-(const Matrix &matrix_1, const Matrix &matrix_2);
    //Умножение матриц
    friend Matrix operator*(const Matrix &matrix_1, const Matrix &matrix_2);
    //Штрассен
    friend Matrix shtrassen(const Matrix &A, const Matrix &B);
    //Виноград-Штрассен
    friend Matrix vinograd_shtrassen(const Matrix &A, const Matrix &B);
    //Умножение матрицы на число (слева)
    friend Matrix operator*(double a, const Matrix &matrix_1);
    //Умножение матрицы на число (справа)
    friend Matrix operator*(const Matrix &matrix_1, double a);
private:
    double *arr;
    int n;
    bool exist_condition;
};
