#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "sbgeometry.h"
#include "sbraytracing.h"

int main(int argc, char *argv[]) {
    Material green_rubber;
    green_rubber.setDiffuseColor(SBVector3(0.0f, 0.5f, 0.0f));
    green_rubber.spec_exponent = 100.0f;
    green_rubber.albedo = SBVector2(0.5f,  0.5f);
    green_rubber.reflection = 0.5f;
    Sphere sphere1(SBVector3(3.0f, 4.0f, 2.0f), 1.5f, green_rubber);

    Material blue_rubber;
    blue_rubber.setDiffuseColor(SBVector3(0.0f, 0.0f, 0.5f));
    blue_rubber.spec_exponent = 10.0f;
    blue_rubber.albedo = SBVector2(0.8f,  0.0f);
    blue_rubber.reflection = 0.0f;
    Sphere sphere2(SBVector3(2.0f, 1.0f, -2.0f), 1.0f, blue_rubber);

    Material red_rubber;
    red_rubber.setDiffuseColor(SBVector3(0.5f, 0.0f, 0.0f));
    red_rubber.spec_exponent = 10.0f;
    red_rubber.albedo = SBVector2(0.8f,  0.2f);
    red_rubber.reflection = 0.0f;
    Sphere sphere3(SBVector3(0.0f, 0.0f, 0.0f), 3.0f, red_rubber);

    Material yellow_rubber;
    yellow_rubber.setDiffuseColor(SBVector3(0.5f, 0.5f, 0.0f));
    yellow_rubber.spec_exponent = 50.0f;
    yellow_rubber.albedo = SBVector2(0.8f,  0.2f);
    yellow_rubber.reflection = 0.0f;
    Sphere sphere4(SBVector3(-1.5f, -1.5f, -2.0f), 0.7f, yellow_rubber);

    Material mirror;
    mirror.setDiffuseColor(SBVector3(0.0f, 0.0f, 0.0f));
    mirror.spec_exponent = 1000.0f;
    mirror.albedo = SBVector2(0.8f,  0.9f);
    mirror.reflection = 0.9f;
    Sphere sphere5(SBVector3(-1.0f, 3.0f, -2.0f), 0.7f, mirror);

    Light light1;
    light1.position = SBVector3(-10.0f, 10.0f, -18.0f);
    light1.bright = 1.7f;

    Light light2;
    light2.position = SBVector3(10.0f, 0.0f, -4.0f);
    light2.bright = 0.7f;

    Light light3;
    light3.position = SBVector3(0.0f, 10.0f, -4.0f);
    light3.bright = 1.0f;

    Scene scene1;
    scene1.spheres.push_back(sphere1);
    scene1.spheres.push_back(sphere2);
    scene1.spheres.push_back(sphere3);
    scene1.spheres.push_back(sphere4);
    scene1.spheres.push_back(sphere5);

    scene1.lights.push_back(light1);
    scene1.lights.push_back(light2);
    scene1.lights.push_back(light3);

    render(true, scene1, 1);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    /*SBLine line1(SBVector2(1, 2), SBVector2(6, 9));
    SBLine line2(SBVector2(2, 2), SBVector2(6, 9));
    int result = line1.isSegmentIntersect(line2);
    SBVector2 v_result = line1.lineIntersect(line2);
    qDebug() << result;
    qDebug() << v_result.getX() << v_result.getY();*/

    return app.exec();
}
