#include "MainWindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application metadata
    QApplication::setApplicationName("BASL Designer");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("Broken Arrow Software");
    QApplication::setOrganizationDomain("brokenarrowsoftware.com");
    
    // Use Fusion style for better cross-platform look
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
