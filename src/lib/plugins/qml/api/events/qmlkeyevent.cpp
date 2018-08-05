/* ============================================================
* Falkon - Qt web browser
* Copyright (C) 2018 Anmol Gautam <tarptaeya@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */
#include "qmlkeyevent.h"
#include <QQmlEngine>

QmlKeyEvent::QmlKeyEvent(QKeyEvent *keyEvent, QObject *parent)
    : QObject(parent)
    , m_keyEvent(keyEvent)
{
    delete keyEvent;
    QQmlEngine::setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

int QmlKeyEvent::count() const
{
    return m_keyEvent->count();
}

bool QmlKeyEvent::isAutoRepeat() const
{
    return m_keyEvent->isAutoRepeat();
}

int QmlKeyEvent::key() const
{
    return m_keyEvent->key();
}

int QmlKeyEvent::modifiers() const
{
    return (int)m_keyEvent->modifiers();
}

quint32 QmlKeyEvent::nativeModifiers() const
{
    return m_keyEvent->nativeModifiers();
}

quint32 QmlKeyEvent::nativeScanCode() const
{
    return m_keyEvent->nativeScanCode();
}

quint32 QmlKeyEvent::nativeVirtualKey() const
{
    return m_keyEvent->nativeVirtualKey();
}

QString QmlKeyEvent::text() const
{
    return m_keyEvent->text();
}