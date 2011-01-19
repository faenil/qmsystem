/*!
 * @file qmdisplaystate.cpp
 * @brief QmDisplayState

   <p>
   Copyright (C) 2009-2010 Nokia Corporation

   @author Antonio Aloisio <antonio.aloisio@nokia.com>
   @author Ilya Dogolazky <ilya.dogolazky@nokia.com>
   @author Timo Olkkonen <ext-timo.p.olkkonen@nokia.com>
   @author Ustun Ergenoglu <ext-ustun.ergenoglu@nokia.com>

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
#include "qmdisplaystate.h"
#include "qmdisplaystate_p.h"

namespace MeeGo {

QmDisplayState::QmDisplayState(QObject *parent)
              : QObject(parent) {
     MEEGO_INITIALIZE(QmDisplayState);

     connect(priv, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)),
             this, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)));
}

QmDisplayState::~QmDisplayState() {
    MEEGO_PRIVATE(QmDisplayState)

    disconnect(priv, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)),
               this, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)));

    MEEGO_UNINITIALIZE(QmDisplayState);
}

void QmDisplayState::connectNotify(const char *signal) {
    MEEGO_PRIVATE(QmDisplayState)

    /* QObject::connect needs to be thread-safe */
    priv->connectMutex.lock();

    if (QLatin1String(signal) == SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState))) {
        if (0 == priv->connectCount[SIGNAL_DISPLAY_STATE]) {
            #if HAVE_MCE
                QDBusConnection::systemBus().connect(MCE_SERVICE,
                                                     MCE_SIGNAL_PATH,
                                                     MCE_SIGNAL_IF,
                                                     MCE_DISPLAY_SIG,
                                                     priv,
                                                     SLOT(slotDisplayStateChanged(const QString&)));
            #endif
        }
        priv->connectCount[SIGNAL_DISPLAY_STATE]++;
    }

    priv->connectMutex.unlock();
}

void QmDisplayState::disconnectNotify(const char *signal) {
    MEEGO_PRIVATE(QmDisplayState)

    /* QObject::disconnect needs to be thread-safe */
    priv->connectMutex.lock();

    if (QLatin1String(signal) == SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState))) {
        priv->connectCount[SIGNAL_DISPLAY_STATE]--;

        if (0 == priv->connectCount[SIGNAL_DISPLAY_STATE]) {
            #if HAVE_MCE
                QDBusConnection::systemBus().disconnect(MCE_SERVICE,
                                                        MCE_SIGNAL_PATH,
                                                        MCE_SIGNAL_IF,
                                                        MCE_DISPLAY_SIG,
                                                        priv,
                                                        SLOT(slotDisplayStateChanged(const QString&)));
            #endif
        }
    }

    priv->connectMutex.unlock();
}

QmDisplayState::DisplayState QmDisplayState::get() const {
    QmDisplayState::DisplayState state = Off;

#if HAVE_MCE
    MEEGO_PRIVATE_CONST(QmDisplayState)

    QmIPCInterface *requestIf = priv->requestIf;
    QList<QVariant> results;
    QString stateStr;

    results = requestIf->get(MCE_DISPLAY_STATUS_GET);
    if (!results.isEmpty())
        stateStr = results[0].toString();

    if ( stateStr == DIMMED )
        state = Dimmed;
    else if ( stateStr == ON )
        state = On;
#endif

    return state;
}

bool QmDisplayState::set(QmDisplayState::DisplayState state) {
#if HAVE_MCE
    MEEGO_PRIVATE(QmDisplayState)

    QmIPCInterface *requestIf = priv->requestIf;
    QString method;

    switch (state){
        case Off:
            method = QString(MCE_DISPLAY_OFF_REQ);
            break;
        case Dimmed:
            method = QString(MCE_DISPLAY_DIM_REQ);
            break;
        case On:
            method = QString(MCE_DISPLAY_ON_REQ);
            break;
        default:
            return false;
    }

    requestIf->callAsynchronously(method);
    return true;
#else
    Q_UNUSED(state);
    return false;
#endif

}

int QmDisplayState::getMaxDisplayBrightnessValue() {
    MEEGO_PRIVATE(QmDisplayState);

    GError* error = NULL;
    int ret = gconf_client_get_int(priv->gc, MAX_BRIGHTNESS_KEY, &error);
    if ( error ) {
        ret = -1;
        g_error_free (error);
    }
    return ret;
}

int QmDisplayState::getDisplayBrightnessValue() {
    MEEGO_PRIVATE(QmDisplayState);

    GError* error = NULL;
    int ret = gconf_client_get_int(priv->gc, BRIGHTNESS_KEY, &error);
    if ( error ) {
        ret = -1;
        g_error_free (error);
    }
    return ret;
}

int QmDisplayState::getDisplayBlankTimeout() {
    MEEGO_PRIVATE(QmDisplayState);

    GError* error = NULL;
    int ret = gconf_client_get_int(priv->gc, BLANK_TIMEOUT_KEY, &error);
    if ( error ) {
        ret = -1;
        g_error_free (error);
    }
    return ret;
}

int QmDisplayState::getDisplayDimTimeout() {
    MEEGO_PRIVATE(QmDisplayState);

    GError* error = NULL;
    int ret = gconf_client_get_int(priv->gc, DIM_TIMEOUT_KEY, &error);
    if ( error ) {
        ret = -1;
        g_error_free (error);
    }
    return ret;
}

bool QmDisplayState::getBlankingWhenCharging() {
    MEEGO_PRIVATE(QmDisplayState);

    GError* error = NULL;
    int val = gconf_client_get_int(priv->gc, BLANKING_CHARGING_KEY, &error);
    if ( error ) {
        g_error_free (error);
        return false;
    }

    // check if blanking is not inhibited during charging.
    bool ret = (val == 0);
    return ret;
}

void QmDisplayState::setDisplayBrightnessValue(int brightness) {
    MEEGO_PRIVATE(QmDisplayState);

    if ((1 > brightness) || (brightness > getMaxDisplayBrightnessValue())) {
        return;
    }

    gconf_client_set_int(priv->gc, BRIGHTNESS_KEY, brightness, NULL);
}

void QmDisplayState::setDisplayBlankTimeout(int timeout) {
    MEEGO_PRIVATE(QmDisplayState);

    gconf_client_set_int(priv->gc, BLANK_TIMEOUT_KEY, timeout, NULL);
}

void QmDisplayState::setDisplayDimTimeout(int timeout) {
    MEEGO_PRIVATE(QmDisplayState);

    GSList* list = NULL;
    GError* error = NULL;

    // Get the list of possible values for dimming timeout.
    list = gconf_client_get_list(priv->gc, POSSIBLE_DIM_LIST_KEY, GCONF_VALUE_INT, &error);
    if ( error ) {
        g_error_free(error);
        g_slist_free(list);
        return;
    }

    // Check if the timeout value is in the list of possible values.
    if (g_slist_index(list, (gconstpointer)timeout) == -1 ) {
        g_slist_free(list);
        return;
    }

    // Set the timeout value.
    gconf_client_set_int(priv->gc, DIM_TIMEOUT_KEY, timeout, NULL);
    g_slist_free(list);
}

void QmDisplayState::setBlankingWhenCharging(bool blanking) {
    MEEGO_PRIVATE(QmDisplayState);

    int b = (blanking?0:1);

    gconf_client_set_int(priv->gc, BLANKING_CHARGING_KEY, b, NULL);
}

bool QmDisplayState::setBlankingPause(void) {
#if HAVE_MCE
    MEEGO_PRIVATE_CONST(QmDisplayState)

    QmIPCInterface *requestIf = priv->requestIf;
    requestIf->callAsynchronously(MCE_PREVENT_BLANK_REQ);
    return true;
#else
    return false;
#endif
}

bool QmDisplayState::cancelBlankingPause(void) {
#if HAVE_MCE
    MEEGO_PRIVATE_CONST(QmDisplayState)

    QmIPCInterface *requestIf = priv->requestIf;
    requestIf->callAsynchronously(MCE_CANCEL_PREVENT_BLANK_REQ);
    return true;
#else
    return false;
#endif
}

} //MeeGo namespace
