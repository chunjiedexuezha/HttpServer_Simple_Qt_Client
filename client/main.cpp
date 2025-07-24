#include <QApplication>
#include "connect2server.h"
// #include "gomokuboard.h"
// #include "gomokuwidget.h"
// #include"menuwindow.h"
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    // HttpClient client;
    // client.sendRequest(QUrl("http://192.168.3.40"));

    Connect2Server connectserver;
    connectserver.show();
    // MenuWindow w;
    // GomokuWidget w(12345, true);
    // w.show();

    return a.exec();
}
