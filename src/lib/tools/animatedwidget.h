/* ============================================================
* Falkon - Qt web browser
* Copyright (C) 2010-2014  David Rosca <nowrep@gmail.com>
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
#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QWidget>
#include <QTimeLine>

#include "qzcommon.h"

class FALKON_EXPORT AnimatedWidget : public QWidget
{
    Q_OBJECT

public:
    enum Direction { Down, Up };

    explicit AnimatedWidget(const Direction &direction = Down, int duration = 300, QWidget* parent = nullptr);

    QWidget* widget() { return m_widget; }

public Q_SLOTS:
    void hide();
    void startAnimation();

private Q_SLOTS:
    void animateFrame(int frame);

private:
    void resizeEvent(QResizeEvent* e) override;

    Direction m_direction;
    QTimeLine m_timeLine;
    qreal m_stepHeight;
    qreal m_stepY;
    int m_startY;

    QWidget* m_widget;
};

#endif // NOTIFICATION_H
