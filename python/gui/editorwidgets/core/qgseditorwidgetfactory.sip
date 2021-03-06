/***************************************************************************
    qgseditorwidgetfactory.sip
     --------------------------------------
    Date                 : 21.4.2013
    Copyright            : (C) 2013 Matthias Kuhn
    Email                : matthias at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/**
 * Every attribute editor widget needs a factory, which inherits this class
 */
class QgsEditorWidgetFactory
{
%TypeHeaderCode
#include <qgseditorwidgetfactory.h>
%End

  public:
    /**
     * Constructor
     *
     * @param name A human readable name for this widget type
     */
    QgsEditorWidgetFactory( const QString& name );

    virtual ~QgsEditorWidgetFactory();

    /**
     * Override this in your implementation.
     * Create a new editor widget wrapper. Call {@link QgsEditorWidgetRegistry::create()}
     * instead of calling this method directly.
     *
     * @param vl       The vector layer on which this widget will act
     * @param fieldIdx The field index on which this widget will act
     * @param editor   An editor widget if already existent. If NULL is provided, a new widget will be created.
     * @param parent   The parent for the wrapper class and any created widget.
     *
     * @return         A new widget wrapper
     */
    virtual QgsEditorWidgetWrapper* create( QgsVectorLayer* vl, int fieldIdx, QWidget* editor, QWidget* parent ) const = 0 /Factory/;

    virtual QgsSearchWidgetWrapper* createSearchWidget( QgsVectorLayer* vl, int fieldIdx, QWidget* parent ) const /Factory/;

    /**
     * Return The human readable identifier name of this widget type
     *
     * @return a name
     */
    QString name();

    /**
     * Override this in your implementation.
     * Create a new configuration widget for this widget type.
     *
     * @param vl       The layer for which the widget will be created
     * @param fieldIdx The field index for which the widget will be created
     * @param parent   The parent widget of the created config widget
     *
     * @return         A configuration widget
     */
    virtual QgsEditorConfigWidget* configWidget( QgsVectorLayer* vl, int fieldIdx, QWidget* parent ) const = 0 /Factory/;

    /**
     * Check if this editor widget type supports a certain field.
     *
     * @param vl        The layer
     * @param fieldIdx  The field index
     * @return          True if the type is supported for this field
     *
     * @see isFieldSupported( QgsVectorLayer* vl, ind fieldIdx )
     */
    bool supportsField( QgsVectorLayer* vl, int fieldIdx );

    /**
     * Returns a list of widget types which this editor widget supports.
     * Each widget type can have a priority value attached, the factory with the highest one
     * will be used.
     *
     * @return A map of widget type names and weight values
     * @note not available in Python bindings
     */
    //virtual QHash<const char*, int> supportedWidgetTypes();

    /**
     * This method allows disabling this editor widget type for a certain field.
     * By default, it returns 5 for every fields.
     * Reimplement this if you only support certain fields.
     *
     * Typical return values are:
     *   * 0: not supported
     *   * 5: maybe support (for example, Datetime support strings depending on their content)
     *   * 10: basic support (this is what returns TextEdit for example, since it supports everything in a crude way)
     *   * 20: specialised support
     *
     * @param vl
     * @param fieldIdx
     * @return 0 if the field is not supported or a bigger number if it can (the widget with the biggest number will be
     *      taken by default). The default implementation returns 5..
     *
     * @see supportsField( QgsVectorLayer* vl, fieldIdx )
     */
    virtual unsigned int fieldScore( const QgsVectorLayer* vl, int fieldIdx ) const;
};
