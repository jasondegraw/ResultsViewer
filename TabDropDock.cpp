/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <resultsviewer/TabDropDock.hpp>
#include <resultsviewer/TabBarDrag.hpp>
#include <resultsviewer/BrowserView.hpp>
#include <resultsviewer/PlotView.hpp>
#include <QApplication>


namespace resultsviewer{

TabDropDock::TabDropDock( QWidget* parent): QTabWidget(parent)
{
//  setAcceptDrops(true);
//  setAttribute(Qt::WA_Hover,true);
//  setMouseTracking(true);
  resultsviewer::TabBarDrag *tabBar = new resultsviewer::TabBarDrag(this);
  connect(tabBar, SIGNAL(signalPerformDrag()), this, SLOT(slotPerformDrag()));
  setTabBar(tabBar);
}


void TabDropDock::dropEvent(QDropEvent *evt)
{
/*  resultsviewer::BrowserView *bv = qobject_cast<resultsviewer::BrowserView *>(evt->source());
  resultsviewer::PlotView *pv = qobject_cast<resultsviewer::PlotView *>(evt->source());
  if (pv)
  {
    if (pv->parent() == 0)
    {
      addTab(pv,pv->windowTitle());
      setCurrentIndex(count()-1);
    }
  }
  if (bv)
  {
    if (bv->parent() == 0)
    {
      addTab(bv,bv->windowTitle());
      setCurrentIndex(count()-1);
    }
  }
*/
}

void TabDropDock::dragEnterEvent(QDragEnterEvent *e)
{
  e->accept();
}


/*
void TabDropDock::mousePressEvent(QMouseEvent *e)
{
  if (e->button() == Qt::LeftButton) m_startDragPos = e->pos();
}

void TabDropDock::mouseMoveEvent(QMouseEvent *e)
{
  if (e->buttons() & Qt::LeftButton)
  {
    int distance = (e->pos() - m_startDragPos).manhattanLength();
    if (distance >= QApplication::startDragDistance()) performDrag();
  }
}
*/
void TabDropDock::slotPerformDrag()
{
//  emit(signalFloatOrDockMe(this));
  QDrag *drag = new QDrag(this);
  QWidget *widget = currentWidget();
  if (widget)
  {
    WidgetMimeData *mimeData = new WidgetMimeData(widget);
    drag->setMimeData(mimeData);
    if (drag->exec(Qt::MoveAction) != Qt::MoveAction)
    {
      if (widget->parent() != 0)
      {
        widget->setParent(0);
        widget->setGeometry(QCursor::pos().x(),QCursor::pos().y(),width(),height());
        widget->show();
        widget->raise();
        widget->activateWindow();
//        removeTab(currentIndex());
      }
    }
  }
//  this->repaint();
}



};
