#include "calculator.h"
#include <chrono>
#include <cmath>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QPixmap>
#include <QRect>
#include <fstream>

//QTextStream out(stdout);

Calc::Calc(QWidget *parent) : QMainWindow(parent){
    QAction *quit = new QAction("Выход", this);
    QAction *generation_ = new QAction("Сгенерировать матрицу", this);
    QMenu *menu;
    menu = menuBar()->addMenu("Меню");
    menu->addAction(generation_);
    connect(generation_, &QAction::triggered, this, &Calc::new_window);
    menu->addAction(quit);
    connect(quit, &QAction::triggered, qApp, QApplication::quit);

    QAction *how_to_use = new QAction("Формат файла", this);
    QMenu *menu_2;
    menu_2 = menuBar()->addMenu("Справка");
    menu_2->addAction(how_to_use);
    connect(how_to_use, &QAction::triggered, this, &Calc::info);

    filename_text = new QLabel("Введите название файла,\nв который вы хотите\nсохранить матрицу.", this);
    filename_text->setGeometry(350, 40, 200, 45);
    filename_text->setVisible(false);
    filename = new QLineEdit(this);
    filename->setGeometry(350, 95, 100, 35);
    filename->setVisible(false);
    filename->setReadOnly(true);
    saveButton = new QPushButton("Сохранить\nматрицу", this);
    saveButton->setGeometry(350, 145, 100, 35);
    saveButton->setVisible(false);
    connect(saveButton, &QPushButton::clicked, this, &Calc::save_result);

    result_text = new QLabel("", this);
    result_text->setGeometry(50, 20, 275, 100);

    size_1_label = new QLabel("", this);
    size_1_label->setGeometry(75, 250, 155, 50);
    firstStatus = new QLabel("Первая матрица:\nне загружена", this);
    firstStatus->setGeometry(75, 220, 155, 50);
    QPushButton *firstButton = new QPushButton("Загрузить\nпервую матрицу\nиз файла", this);
    firstButton->setGeometry(75, 300, 110, 50);
    connect(firstButton, &QPushButton::clicked, this, &Calc::load_first_matrix);

    size_2_label = new QLabel("", this);
    size_2_label->setGeometry(250, 250, 155, 50);
    secondStatus = new QLabel("Вторая матрица:\nне загружена", this);
    secondStatus->setGeometry(250, 220, 155, 50);
    QPushButton *secondButton = new QPushButton("Загрузить\nвторую матрицу\n из файла", this);
    secondButton->setGeometry(250, 300, 110, 50);
    connect(secondButton, &QPushButton::clicked, this, &Calc::load_second_matrix);

    QPushButton *thirdButton = new QPushButton("Умножить\nматрицы", this);
    thirdButton->setGeometry(425, 300, 110, 50);
    connect(thirdButton, &QPushButton::clicked, this, &Calc::multiply);

    QStringList variants = {"Классическое умножение", "Алгоритм Штрассена", "Алгоритм Винограда-Штрассена"};
    combo = new QComboBox(this);
    combo->addItems(variants);
    combo->setGeometry(QRect(50, 150, 200, 30));
}

void Calc::info(){
    QWidget *info_window = new QWidget();
    QLabel *size_info = new QLabel("Текстовый файл, в котором находится матрица, должен\nсодержать размер матрицы на первой строке."
                                   "Матрица\nдолжна располагаться, начиная со второй строки.", info_window);
    size_info->setGeometry(10, 10, 400, 50);
    QPixmap pixmap("./info_image.jpg");
    QLabel *img_label = new QLabel(info_window);
    img_label->setPixmap(pixmap);
    img_label->setGeometry(10, 60, 150, 150);
    info_window->show();
    info_window->setFixedSize(400, 220);
    info_window->setWindowTitle("Формат файла");
    info_window->setWindowIcon(QIcon("icon_for_app.png"));
}

void Calc::new_window(){
    nw = new QWidget();
    QLabel *size_text = new QLabel("Введите размер\nквадратной матрицы:", nw);
    size_text->setGeometry(10, 10, 150, 30);
    size_input = new QLineEdit(nw);
    size_input->setValidator(new QIntValidator(0, 10000, nw));
    size_input->setGeometry(10, 50, 130, 35);

    QLabel *file_text = new QLabel("Введите название\nфайла:", nw);
    file_text->setGeometry(170, 10, 150, 30);
    file_input = new QLineEdit(nw);
    file_input->setGeometry(170, 50, 130, 35);

    QPushButton *gen_button = new QPushButton("Сгенерировать", nw);
    gen_button->setGeometry(105, 100, 100, 50);
    connect(gen_button, &QPushButton::clicked, this, &Calc::gen_matrix);
    int height = this->height(), width = this->width();
    nw->move(width - nw->width() + 130, height);
    nw->show();
    nw->setWindowIcon(QIcon("icon_for_app.png"));
    nw->setFixedSize(320, 200);
    nw->setWindowTitle("Генерация матрицы");
}

void Calc::gen_matrix(){
    if(size_input->text().isEmpty()){
        QMessageBox::warning(nw, "Ошибка", "Вы не ввели размер матрицы.");
        return;
    }
    if((size_input->text()).toInt() == 0){
        QMessageBox::warning(nw, "Ошибка", "Размер матрицы не может быть нулевым.");
        return;
    }
    if(file_input->text().isEmpty()){
        QMessageBox::warning(nw, "Ошибка", "Вы не ввели название файла.");
        return;
    }
    QString dir = QFileDialog::getExistingDirectory(this, tr("Выберите папку"),
                                                 QDir::currentPath(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if(dir.isEmpty()){
        QMessageBox::warning(this, "Ошибка", "Вы не выбрали папку.");
        return;
    }
    Matrix *temp_gen = new Matrix((size_input->text()).toInt(), true, true);
    temp_gen->to_file((file_input->text()).toStdString(), dir.toStdString());
    QMessageBox::information(this, "Успех", "Матрица сгенерирована и загружена в файл.");
    delete temp_gen;
}

void Calc::save_result(){
    if(!third_matrix_exist){
        QMessageBox::warning(this, "Ошибка", "Вы не умножили матрицы");
        return;
    }
    if(filename->text().isEmpty()){
        QMessageBox::warning(this, "Ошибка", "Вы не ввели название файла");
        return;
    }
    QString dir = QFileDialog::getExistingDirectory(this, tr("Выберите папку"),
                                                 QDir::currentPath(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if(dir.isEmpty()){
        QMessageBox::warning(this, "Ошибка", "Вы не выбрали папку.");
        return;
    }
    third_matrix->to_file((filename->displayText()).toStdString(), dir.toStdString());
    int size_m = first_matrix->get_size();
    std::chrono::duration<double> diff = end - start;
    std::ofstream fout(filename->displayText().toStdString() + "_multiplying_time.txt");
    fout << "Две квадратные матрицы размера " << size_m << "x" << size_m << " были умножены за " << diff.count() << " секунд.";
    fout.close();
    QMessageBox::information(this, "Успех", "Матрица сохранена в файл");
}

void Calc::multiply(){
    result_text->setText("");
    if(first_cond == 0 && second_cond == 0){
        QMessageBox::warning(this, "Ошибка", "Вы не загрузили матрицы");
        return;
    }
    else if(first_cond == 0 && second_cond == 1){
        QMessageBox::warning(this, "Ошибка", "Вы не загрузили первую матрицу");
        return;
    }
    else if(first_cond == 1 && second_cond == 0){
        QMessageBox::warning(this, "Ошибка", "Вы не загрузили вторую матрицу");
        return;
    }
    else{
        if(first_matrix->get_size() != second_matrix->get_size()){
            QMessageBox::warning(this, "Ошибка", "Матрицы не совпадают по размеру");
            return;
        }
        if((first_matrix->get_exist()) == 0 || second_matrix->get_exist() == 0){
            QMessageBox::warning(this, "Ошибка", "Ошибка");
            return;
        }
        if(first_matrix->get_exist() && second_matrix->get_exist()){
            result_text->setText("Начинается умножение...");
            //https://www.cplusplus.com/reference/chrono/
            if(combo->currentIndex() == 0){
                start = std::chrono::steady_clock::now();
                Matrix temp = (*first_matrix) * (*second_matrix);
                end = std::chrono::steady_clock::now();
                third_matrix = new Matrix(temp);
            }
            int old_size = first_matrix->get_size();
            if(old_size & (old_size - 1)){
                first_matrix->resize(pow(2, ceil(log2(old_size))));
                second_matrix->resize(pow(2, ceil(log2(old_size))));
            }
            if(combo->currentIndex() == 1){
                start = std::chrono::steady_clock::now();
                Matrix temp = shtrassen((*first_matrix), (*second_matrix));
                end = std::chrono::steady_clock::now();
                temp.resize(old_size);
                third_matrix = new Matrix(temp);
            }
            if(combo->currentIndex() == 2){
                start = std::chrono::steady_clock::now();
                Matrix temp_2 = vinograd_shtrassen((*first_matrix), (*second_matrix));
                end = std::chrono::steady_clock::now();
                temp_2.resize(old_size);
                third_matrix = new Matrix(temp_2);
            }
            first_matrix->resize(old_size); second_matrix->resize(old_size);
            QString temp_time, temp = result_text->text();
            std::chrono::duration<double> diff = end - start;
            temp_time.setNum(diff.count());
            temp.append("\nУмножение завершено за " + temp_time + " секунд.");
            result_text->setText(temp);
            third_matrix_exist = 1;
            filename_text->setVisible(true);
            filename->setVisible(true);
            saveButton->setVisible(true);
            filename->setReadOnly(false);
            }
        }
    }

void Calc::load_first_matrix(){
    firstStatus->setText("Первая матрица:\nпроисходит загрузка...");
    QString filename_1 = QFileDialog::getOpenFileName(
                this,
                "Выберите текстовый файл",
                QDir::currentPath(),
                "Txt files (*.txt) ;; All files (*.*)");
    if(filename_1 == ""){
        firstStatus->setText("Первая матрица:\nНе был выбран файл");
        size_1_label->setText("");
        first_cond = 0;
    }
    else{
        first_matrix = new Matrix(filename_1.toStdString());
        QString temp;
        size_1_label->setText("Размер матрицы: " + temp.setNum(first_matrix->get_size()));
        firstStatus->setText("Первая матрица:\nуспешно загружена");
        first_cond = 1;
    }
};
void Calc::load_second_matrix(){
    secondStatus->setText("Вторая матрица:\nпроисходит загрузка...");
    QString filename_2 = QFileDialog::getOpenFileName(
                this,
                "Выберите текстовый файл",
                QDir::currentPath(),
                "Txt files (*.txt) ;; All files (*.*)");
    if(filename_2 == ""){
        secondStatus->setText("Вторая матрица:\nНе был выбран файл");
        size_2_label->setText("");
        second_cond = 0;
    }
    else{
        second_matrix = new Matrix(filename_2.toStdString());
        QString temp;
        size_2_label->setText("Размер матрицы: " + temp.setNum(second_matrix->get_size()));
        secondStatus->setText("Вторая матрица:\nуспешно загружена");
        second_cond = 1;
    }
}
