/**
 * @file usbmode.cpp
 * @brief QmUSBMode tests

   <p>
   Copyright (C) 2009-2010 Nokia Corporation

   @author Timo Olkkonen <ext-timo.p.olkkonen@nokia.com>

   This file is part of SystemSW QtAPI.

   SystemSW QtAPI is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License
   version 2.1 as published by the Free Software Foundation.

   SystemSW QtAPI is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with SystemSW QtAPI.  If not, see <http://www.gnu.org/licenses/>.
   </p>
 */
#include <QObject>
#include <qmusbmode.h>
#include <QTest>
#include <QDebug>

using namespace MeeGo;

class TestClass : public QObject
{
    Q_OBJECT

public slots:
    void modeChanged(MeeGo::QmUSBMode::Mode mode) {
        qDebug() << "Received a modeChanged signal: " << mode;
        currentMode = mode;
        signalReceived = true;
    }

private:
    QmUSBMode *mode;
    QmUSBMode::Mode currentMode;
    bool signalReceived;

    void setGetMode(QmUSBMode::Mode newMode) {
        qDebug() << "Settings mode to: " << newMode;
        QVERIFY(mode->setMode(newMode));
        QTest::qWait(1000);
        QCOMPARE(currentMode, newMode);
        QCOMPARE(currentMode, mode->getMode());
    }

    void setGetDefaultMode(QmUSBMode::Mode newMode) {
        QVERIFY(mode->setDefaultMode(newMode));
        QCOMPARE(newMode, mode->getDefaultMode());

    }

private slots:
    void initTestCase() {
        mode = new QmUSBMode();
        QVERIFY(mode);
        QVERIFY(connect(mode, SIGNAL(modeChanged(MeeGo::QmUSBMode::Mode)),
                        this, SLOT(modeChanged(MeeGo::QmUSBMode::Mode))));
    }

    void testSetGetModes() {
        setGetMode(QmUSBMode::MassStorage);
        setGetMode(QmUSBMode::ChargingOnly);
        setGetMode(QmUSBMode::OviSuite);
    }

    void testSetGetDefaultModes() {
        setGetDefaultMode(QmUSBMode::MassStorage);
        setGetDefaultMode(QmUSBMode::ChargingOnly);
        setGetDefaultMode(QmUSBMode::OviSuite);
        setGetDefaultMode(QmUSBMode::Ask);
    }

    void cleanupTestCase() {
        delete mode;
    }
};

QTEST_MAIN(TestClass)
#include "usbmode.moc"
