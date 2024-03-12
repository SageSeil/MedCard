#include "formautorization.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FormAutorization w;
    w.show();
    return a.exec();
}
