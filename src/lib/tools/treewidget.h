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
#ifndef BOOKMARKSTREEWIDGET_H
#define BOOKMARKSTREEWIDGET_H

#include "qzcommon.h"

#include <QTreeWidget>

class FALKON_EXPORT TreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit TreeWidget(QWidget* parent = nullptr);
    enum ItemShowMode { ItemsCollapsed = 0, ItemsExpanded = 1 };
    ItemShowMode defaultItemShowMode() { return m_showMode; }
    void setDefaultItemShowMode(ItemShowMode mode) { m_showMode = mode; }
    QList<QTreeWidgetItem*> allItems();

    bool appendToParentItem(const QString &parentText, QTreeWidgetItem* item);
    bool appendToParentItem(QTreeWidgetItem* parent, QTreeWidgetItem* item);
    bool prependToParentItem(const QString &parentText, QTreeWidgetItem* item);
    bool prependToParentItem(QTreeWidgetItem* parent, QTreeWidgetItem* item);

    void addTopLevelItem(QTreeWidgetItem* item);
    void addTopLevelItems(const QList<QTreeWidgetItem*> &items);
    void insertTopLevelItem(int index, QTreeWidgetItem* item);
    void insertTopLevelItems(int index, const QList<QTreeWidgetItem*> &items);

    void deleteItem(QTreeWidgetItem* item);
    void deleteItems(const QList<QTreeWidgetItem*> &items);

Q_SIGNALS:
    void itemControlClicked(QTreeWidgetItem* item);
    void itemMiddleButtonClicked(QTreeWidgetItem* item);

public Q_SLOTS:
    void filterString(const QString &string);
    void clear();

private Q_SLOTS:
    void sheduleRefresh();

private:
    void mousePressEvent(QMouseEvent* event) override;
    void iterateAllItems(QTreeWidgetItem* parent);

    bool m_refreshAllItemsNeeded;
    QList<QTreeWidgetItem*> m_allTreeItems;
    ItemShowMode m_showMode;
};

#endif // BOOKMARKSTREEWIDGET_H
