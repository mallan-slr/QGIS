/***************************************************************************
  qgslayertreelayer.h
  --------------------------------------
  Date                 : May 2014
  Copyright            : (C) 2014 by Martin Dobias
  Email                : wonder dot sk at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSLAYERTREELAYER_H
#define QGSLAYERTREELAYER_H

#include "qgis_core.h"
#include "qgslayertreenode.h"

class QgsMapLayer;

/** \ingroup core
 * Layer tree node points to a map layer.
 *
 * When using with existing QgsMapLayer instance, it is expected that the layer
 * has been registered in QgsProject earlier.
 *
 * The node can exist also without a valid instance of a layer (just ID). That
 * means the referenced layer does not need to be loaded in order to use it
 * in layer tree. In such case, the node will start listening to map layer
 * registry updates in expectation that the layer (identified by its ID) will
 * be loaded later.
 *
 * A map layer is supposed to be present in one layer tree just once. It is
 * however possible that temporarily a layer exists in one tree more than just
 * once, e.g. while reordering items with drag and drop.
 *
 * @note added in 2.4
 */
class CORE_EXPORT QgsLayerTreeLayer : public QgsLayerTreeNode
{
    Q_OBJECT
  public:
    explicit QgsLayerTreeLayer( QgsMapLayer* layer );
    QgsLayerTreeLayer( const QgsLayerTreeLayer& other );

    explicit QgsLayerTreeLayer( const QString& layerId, const QString& name = QString() );

    QString layerId() const { return mLayerId; }

    QgsMapLayer* layer() const { return mLayer; }

    //! Get layer's name
    //! @note added in 3.0
    QString name() const override;
    //! Set layer's name
    //! @note added in 3.0
    void setName( const QString& n ) override;

    static QgsLayerTreeLayer* readXml( QDomElement& element );
    virtual void writeXml( QDomElement& parentElement ) override;

    virtual QString dump() const override;

    virtual QgsLayerTreeLayer* clone() const override;

  protected slots:
    void registryLayersAdded( const QList<QgsMapLayer*>& layers );
    void registryLayersWillBeRemoved( const QStringList& layerIds );
    //! Emits a nameChanged() signal if layer's name has changed
    //! @note added in 3.0
    void layerNameChanged();

  signals:
    //! emitted when a previously unavailable layer got loaded
    void layerLoaded();
    //! emitted when a previously available layer got unloaded (from layer registry)
    //! @note added in 2.6
    void layerWillBeUnloaded();

  protected:
    void attachToLayer();

    QString mLayerId;
    QString mLayerName; // only used if layer does not exist
    QgsMapLayer* mLayer; // not owned! may be null
};



#endif // QGSLAYERTREELAYER_H
