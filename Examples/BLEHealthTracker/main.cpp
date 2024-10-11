#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.addImportPath(qApp->applicationDirPath() + "/qml"); //! Needed on desktop in development
    engine.addImportPath(":/"); //! If RESOURCE_PREFIX is /, this is mandatory for mobile
    engine.loadFromModule("BLEHealthTracker", "Main");

    return app.exec();
}
