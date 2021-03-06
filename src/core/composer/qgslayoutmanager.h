/***************************************************************************
    qgslayoutmanager.h
    ------------------
    Date                 : January 2017
    Copyright            : (C) 2017 Nyall Dawson
    Email                : nyall dot dawson at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSLAYOUTMANAGER_H
#define QGSLAYOUTMANAGER_H

#include "qgis_core.h"
#include "qgis.h"
#include "qgscomposition.h"
#include "qgslayout.h"
#include <QObject>

class QgsProject;

/**
 * \ingroup core
 * \class QgsLayoutManager
 * \since QGIS 3.0
 *
 * \brief Manages storage of a set of layouts.
 *
 * QgsLayoutManager handles the storage, serializing and deserializing
 * of QgsLayouts. Usually this class is not constructed directly, but
 * rather accessed through a QgsProject via QgsProject::layoutManager().
 *
 * QgsLayoutManager retains ownership of all the layouts contained
 * in the manager.
 */

class CORE_EXPORT QgsLayoutManager : public QObject
{
    Q_OBJECT

  public:

    /**
     * Constructor for QgsLayoutManager. The project will become the parent object for this
     * manager.
     */
    explicit QgsLayoutManager( QgsProject *project SIP_TRANSFERTHIS = nullptr );

    ~QgsLayoutManager() override;

    /**
     * Adds a composition to the manager. Ownership of the composition is transferred to the manager.
     * Returns true if the addition was successful, or false if the composition could not be added (eg
     * as a result of a duplicate composition name).
     * \see removeComposition()
     * \see compositionAdded()
     */
    bool addComposition( QgsComposition *composition SIP_TRANSFER );

    /**
     * Adds a \a layout to the manager. Ownership of the layout is transferred to the manager.
     * Returns true if the addition was successful, or false if the layout could not be added (eg
     * as a result of a duplicate layout name).
     * \see removeLayout()
     * \see layoutAdded()
     */
    bool addLayout( QgsLayout *layout SIP_TRANSFER );

    /**
     * Removes a composition from the manager. The composition is deleted.
     * Returns true if the removal was successful, or false if the removal failed (eg as a result
     * of removing a composition which is not contained in the manager).
     * \see addComposition()
     * \see compositionRemoved()
     * \see compositionAboutToBeRemoved()
     * \see clear()
     */
    bool removeComposition( QgsComposition *composition );

    /**
     * Removes a \a layout from the manager. The layout is deleted.
     * Returns true if the removal was successful, or false if the removal failed (eg as a result
     * of removing a layout which is not contained in the manager).
     * \see addLayout()
     * \see layoutRemoved()
     * \see layoutAboutToBeRemoved()
     * \see clear()
     */
    bool removeLayout( QgsLayout *layout );

    /**
     * Removes and deletes all layouts from the manager.
     * \see removeLayout()
     */
    void clear();

    /**
     * Returns a list of all compositions contained in the manager.
     */
    QList< QgsComposition * > compositions() const;

    /**
     * Returns a list of all layouts contained in the manager.
     */
    QList< QgsLayout * > layouts() const;

    /**
     * Returns the composition with a matching name, or nullptr if no matching compositions
     * were found.
     */
    QgsComposition *compositionByName( const QString &name ) const;

    /**
     * Returns the layout with a matching name, or nullptr if no matching layouts
     * were found.
     */
    QgsLayout *layoutByName( const QString &name ) const;

    /**
     * Reads the manager's state from a DOM element, restoring all layouts
     * present in the XML document.
     * \see writeXml()
     */
    bool readXml( const QDomElement &element, const QDomDocument &doc );

    /**
     * Returns a DOM element representing the state of the manager.
     * \see readXml()
     */
    QDomElement writeXml( QDomDocument &doc ) const;

    /**
     * Saves the composition with matching \a name in template format.
     * Returns true if save was successful.
     */
    bool saveAsTemplate( const QString &name, QDomDocument &doc ) const;

    /**
     * Duplicates an existing composition from the manager. The new
     * composition will automatically be stored in the manager.
     * Returns new composition if duplication was successful.
     */
    QgsComposition *duplicateComposition( const QString &name, const QString &newName );

    /**
     * Duplicates an existing \a layout from the manager. The new
     * layout will automatically be stored in the manager.
     * Returns new the layout if duplication was successful.
     */
    QgsLayout *duplicateLayout( const QgsLayout *layout, const QString &newName );

    /**
     * Generates a unique title for a new composition, which does not
     * clash with any already contained by the manager.
     */
    QString generateUniqueComposerTitle() const;

    /**
     * Generates a unique title for a new layout, which does not
     * clash with any already contained by the manager.
     */
    QString generateUniqueTitle() const;

  signals:

    //! Emitted when a composition is about to be added to the manager
    void compositionAboutToBeAdded( const QString &name );

    //! Emitted when a layout is about to be added to the manager
    void layoutAboutToBeAdded( const QString &name );

    //! Emitted when a composition has been added to the manager
    void compositionAdded( const QString &name );

    //! Emitted when a layout has been added to the manager
    void layoutAdded( const QString &name );

    //! Emitted when a composition was removed from the manager
    void compositionRemoved( const QString &name );

    //! Emitted when a layout was removed from the manager
    void layoutRemoved( const QString &name );

    //! Emitted when a composition is about to be removed from the manager
    void compositionAboutToBeRemoved( const QString &name );

    //! Emitted when a layout is about to be removed from the manager
    void layoutAboutToBeRemoved( const QString &name );

    //! Emitted when a composition is renamed
    void compositionRenamed( QgsComposition *composition, const QString &newName );

    //! Emitted when a layout is renamed
    void layoutRenamed( QgsLayout *layout, const QString &newName );

  private:

    QgsProject *mProject = nullptr;

    QList< QgsComposition * > mCompositions;
    QList< QgsLayout * > mLayouts;

    QgsComposition *createCompositionFromXml( const QDomElement &element, const QDomDocument &doc ) const;

};

#endif // QGSLAYOUTMANAGER_H
