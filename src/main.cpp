#include <QGuiApplication>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QString>
#include <QTranslator>
#include <QUrl>

#include "models/word_counter.h"
#include "models/word_counter_model.h"

auto main(int argc, char **argv) -> int {
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app{argc, argv};

    app.setApplicationDisplayName(QGuiApplication::translate("main", "Word Counter"));
    app.setOrganizationDomain(QStringLiteral("ovfireandvoid"));
    app.setOrganizationName(QStringLiteral("ovfireandvoid"));

    QTranslator app_translator;

    if (app_translator.load(QLocale::system(), {}, {}, QStringLiteral(":/translations"))) {
        app.installTranslator(&app_translator);
    } else {
        Q_ASSERT(app_translator.load(QStringLiteral("en"), QStringLiteral(":/translations")));
    }

    app.installTranslator(&app_translator);

    qmlRegisterModule("WordCounter", 1, 0);
    qmlRegisterType<WordCounter>("WordCounter", 1, 0, "WordCounter");
    qmlRegisterType<WordCounterModel>("WordCounter", 1, 0, "WordCounterModel");
    qmlProtectModule("WordCounter", 1);

    QQmlApplicationEngine app_engine{QUrl{QStringLiteral("qrc:/main.qml")}};

    if (app_engine.rootObjects().empty()) {
        return EXIT_FAILURE;
    }

    return app.exec();
}
