#include "calculator.h"
#include <QApplication>
#include <QWidget>
#include <QIcon>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    Calc window;

    window.setFixedSize(600, 400);
    window.setWindowTitle("Матричный калькулятор");
    window.setWindowIcon(QIcon("icon_for_app.png"));
    window.show();
    return app.exec();
}
