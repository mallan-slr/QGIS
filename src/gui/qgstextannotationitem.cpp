/***************************************************************************
                              qgstextannotationitem.cpp
                              ------------------------
  begin                : February 9, 2010
  copyright            : (C) 2010 by Marco Hugentobler
  email                : marco dot hugentobler at hugis dot net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgstextannotationitem.h"
#include <QDomDocument>
#include <QPainter>

QgsTextAnnotationItem::QgsTextAnnotationItem( QgsMapCanvas* canvas ): QgsAnnotationItem( canvas ), mDocument( new QTextDocument( QObject::tr( "" ) ) )
{
  mDocument->setUseDesignMetrics( true );
}

QgsTextAnnotationItem::~QgsTextAnnotationItem()
{
  delete mDocument;
}

QTextDocument* QgsTextAnnotationItem::document() const
{
  if ( !mDocument )
  {
    return nullptr;
  }

  return mDocument->clone();
}

void QgsTextAnnotationItem::setDocument( const QTextDocument* doc )
{
  delete mDocument;
  mDocument = doc->clone();
}

void QgsTextAnnotationItem::paint( QPainter * painter )
{
  if ( !painter || !mDocument )
  {
    return;
  }

  drawFrame( painter );
  if ( mMapPositionFixed )
  {
    drawMarkerSymbol( painter );
  }
  double frameWidth = mFrameBorderWidth;
  mDocument->setTextWidth( mFrameSize.width() );

  painter->save();
  painter->translate( mOffsetFromReferencePoint.x() + frameWidth / 2.0,
                      mOffsetFromReferencePoint.y() + frameWidth / 2.0 );

  QRectF clipRect = QRectF( 0, 0, mFrameSize.width() - frameWidth / 2.0, mFrameSize.height() - frameWidth / 2.0 );
  if ( painter->hasClipping() )
  {
    //QTextDocument::drawContents will draw text outside of the painter's clip region
    //when it is passed a clip rectangle. So, we need to intersect it with the
    //painter's clip region to prevent text drawn outside clipped region (e.g., outside composer maps, see #10400)
    clipRect = clipRect.intersected( painter->clipRegion().boundingRect() );
  }
  //draw text document
  mDocument->drawContents( painter, clipRect );
  painter->restore();
  if ( isSelected() )
  {
    drawSelectionBoxes( painter );
  }
}

void QgsTextAnnotationItem::writeXml( QDomDocument& doc ) const
{
  QDomElement documentElem = doc.documentElement();
  if ( documentElem.isNull() )
  {
    return;
  }
  QDomElement annotationElem = doc.createElement( QStringLiteral( "TextAnnotationItem" ) );
  if ( mDocument )
  {
    annotationElem.setAttribute( QStringLiteral( "document" ), mDocument->toHtml() );
  }
  _writeXml( doc, annotationElem );
  documentElem.appendChild( annotationElem );
}

void QgsTextAnnotationItem::readXml( const QDomDocument& doc, const QDomElement& itemElem )
{
  delete mDocument;
  mDocument = new QTextDocument;
  mDocument->setHtml( itemElem.attribute( QStringLiteral( "document" ), QObject::tr( "" ) ) );
  QDomElement annotationElem = itemElem.firstChildElement( QStringLiteral( "AnnotationItem" ) );
  if ( !annotationElem.isNull() )
  {
    _readXml( doc, annotationElem );
  }
}
