/* ============================================================
* Falkon - Qt web browser
* Copyright (C) 2010-2017 David Rosca <nowrep@gmail.com>
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
#ifndef BOOKMARKSTOOLBAR_H
#define BOOKMARKSTOOLBAR_H

#include <QWidget>

#include "qzcommon.h"

class QHBoxLayout;
class QTimer;

class BrowserWindow;
class Bookmarks;
class BookmarkItem;
class BookmarksToolbarButton;

class FALKON_EXPORT BookmarksToolbar : public QWidget
{
    Q_OBJECT
public:
    explicit BookmarksToolbar(BrowserWindow* window, QWidget* parent = nullptr);

private Q_SLOTS:
    void contextMenuRequested(const QPoint &pos);

    void refresh();
    void bookmarksChanged();
    void showOnlyIconsChanged(bool state);
    void showOnlyTextChanged(bool state);

    void openBookmarkInNewTab();
    void openBookmarkInNewWindow();
    void openBookmarkInNewPrivateWindow();
    void createNewFolder();
    void editBookmark();
    void deleteBookmark();

private:
    void clear();
    void addItem(BookmarkItem* item);
    BookmarksToolbarButton* buttonAt(const QPoint &pos);

    QSize minimumSizeHint() const override;

    void dropEvent(QDropEvent* e) override;
    void dragEnterEvent(QDragEnterEvent* e) override;
    void dragMoveEvent(QDragMoveEvent* e) override;
    void dragLeaveEvent(QDragLeaveEvent* e) override;
    void paintEvent(QPaintEvent* p) override;

    BrowserWindow* m_window;
    Bookmarks* m_bookmarks;
    BookmarkItem* m_clickedBookmark;
    QHBoxLayout* m_layout;
    QTimer* m_updateTimer;
    QAction* m_actShowOnlyIcons = nullptr;
    QAction* m_actShowOnlyText = nullptr;

    int m_dropRow;
    QPoint m_dropPos;
    void clearDropIndicator();
};

#endif // BOOKMARKSTOOLBAR_H
