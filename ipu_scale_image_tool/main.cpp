#include <QApplication>
#include <QQmlApplicationEngine>
#include "filecustomdialog.h"
#include "ipu-scale.h"
#include <QQmlContext>
#include <QQuickView>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

//    QQmlApplicationEngine engine;
//    engine.rootContext()->setContextProperty( "file_custom_dialog", new FileCustomDialog );
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") );

//    QQuickView view;
    QQuickView view;
    view.rootContext()->setContextProperty( "ipu_scale", new IPUScale );

    view.rootContext()->setContextProperty( "file_custom_dialog", new FileCustomDialog );

    view.setSource(QUrl("qrc:///main.qml"));
    view.show();
    return app.exec();
}
