#include "MainWindow.h"
#include <QApplication>

int main( int argc, char** argv ) {
    QApplication app( argc, argv );
    app.setOrganizationName( "STech" );
    app.setApplicationName( "Estlcam Werkzeuge" );
    MainWindow main;
    main.show();
    return app.exec();
}
