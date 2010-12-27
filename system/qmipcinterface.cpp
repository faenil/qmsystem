/*!
 * @file qmipcinterface.cpp
 * @brief QmIPCInterface

   <p>
   Copyright (C) 2009-2010 Nokia Corporation

   @author Antonio Aloisio <antonio.aloisio@nokia.com>
   @author Timo Olkkonen <ext-timo.p.olkkonen@nokia.com>
   @author Matias Muhonen <ext-matias.muhonen@nokia.com>

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
#include "qmipcinterface_p.h"

namespace MeeGo {

QmIPCInterface::QmIPCInterface(const char* service,
                               const char* path,
                               const char* interface,
                               QObject *parent)
              :  QDBusAbstractInterface(service,
                                        path,
                                        interface,
                                        QDBusConnection::systemBus(),
                                        parent) {
}

QmIPCInterface::~QmIPCInterface(){
}

bool QmIPCInterface::callSynchronously(const QString& method,
                                       const QVariant& arg1,
                                       const QVariant& arg2 ) {
    QDBusMessage msg = call(method, arg1, arg2);
    if (msg.type() == QDBusMessage::ReplyMessage) {
        return true;
    }
    return false;
}

QDBusPendingCall QmIPCInterface::callAsynchronously(const QString& method,
                                        const QVariant& arg1,
                                        const QVariant& arg2 ) {
    return asyncCall(method, arg1, arg2);
}

QList<QVariant> QmIPCInterface::get(const QString& method,
                                    const QVariant& arg1,
                                    const QVariant& arg2) {
    QList<QVariant> results;
    QDBusMessage msg = call(method, arg1, arg2);
    if (msg.type() == QDBusMessage::ReplyMessage) {
        results  = msg.arguments();
    }
    return results;
}

bool QmIPCInterface::connect(const QString & name, QObject * receiver, const char * slot){
     return connection().connect(service(), path(), interface(), name, receiver, slot);
}

} // Namespace MeeGo