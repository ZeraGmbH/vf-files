#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qInfo("ZENUX automount started");

    return a.exec();
}