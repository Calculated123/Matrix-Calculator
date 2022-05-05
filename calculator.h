#pragma once
#include <matrix.h>
#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QMainWindow>
#include <chrono>

class Calc : public QMainWindow{
    Q_OBJECT
public:
    Calc(QWidget *parent = 0);
private slots:
    void load_first_matrix();
    void load_second_matrix();
    void multiply();
    void save_result();
    void new_window();
    void gen_matrix();
    void info();
private:
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    QPushButton *saveButton;
    QWidget *nw;
    QLineEdit *filename, *size_input, *file_input;
    QComboBox *combo;
    QLabel *result_text, *filename_text;
    QLabel *firstStatus, *secondStatus, *size_1_label, *size_2_label;
    Matrix *first_matrix, *second_matrix, *third_matrix;
    bool first_cond = 0, second_cond = 0, third_matrix_exist = 0;
};
