#include "mainwindow.h"
#include <QApplication>
#include <memory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto game=std::unique_ptr<Game>(Game::instance());
    game->run();
    MainWindow w(std::move(game));
    w.show();
    return a.exec();
}
