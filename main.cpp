#include <QApplication>
#include "AppController.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    AppController controller(a);
    controller.run();

    return a.exec();
}
