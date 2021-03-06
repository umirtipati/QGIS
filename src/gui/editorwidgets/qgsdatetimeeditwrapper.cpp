/***************************************************************************
    qgsdatetimeeditwrapper.cpp
     --------------------------------------
    Date                 : 03.2014
    Copyright            : (C) 2014 Denis Rouzaud
    Email                : denis.rouzaud@gmail.com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsdatetimeeditwrapper.h"
#include "qgsdatetimeeditfactory.h"
#include "qgsmessagelog.h"
#include "qgslogger.h"
#include "qgsdatetimeedit.h"
#include "qgsdatetimeeditconfig.h"
#include "qgsdatetimefieldformatter.h"

#include <QDateTimeEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QTextCharFormat>
#include <QCalendarWidget>

QgsDateTimeEditWrapper::QgsDateTimeEditWrapper( QgsVectorLayer *vl, int fieldIdx, QWidget *editor, QWidget *parent )
  : QgsEditorWidgetWrapper( vl, fieldIdx, editor, parent )

{
}

QWidget *QgsDateTimeEditWrapper::createWidget( QWidget *parent )
{
  QgsDateTimeEdit *widget = new QgsDateTimeEdit( parent );
  widget->setDateTime( QDateTime::currentDateTime() );
  return widget;
}

void QgsDateTimeEditWrapper::initWidget( QWidget *editor )
{
  QgsDateTimeEdit *qgsEditor = dynamic_cast<QgsDateTimeEdit *>( editor );
  if ( qgsEditor )
  {
    mQgsDateTimeEdit = qgsEditor;
  }
  // assign the Qt editor also if the QGIS editor has been previously assigned
  // this avoids testing each time which widget to use
  // the QGIS editor must be used for non-virtual methods (dateTime, setDateTime)
  QDateTimeEdit *qtEditor = dynamic_cast<QDateTimeEdit *>( editor );
  if ( qtEditor )
  {
    mQDateTimeEdit = qtEditor;
  }

  if ( !mQDateTimeEdit )
  {
    QgsDebugMsg( "Date/time edit widget could not be initialized because provided widget is not a QDateTimeEdit." );
    QgsMessageLog::logMessage( QStringLiteral( "Date/time edit widget could not be initialized because provided widget is not a QDateTimeEdit." ), QStringLiteral( "UI forms" ), QgsMessageLog::WARNING );
    return;
  }

  const QString displayFormat = config( QStringLiteral( "display_format" ), QgsDateTimeFieldFormatter::defaultFormat( field().type() ) ).toString();
  mQDateTimeEdit->setDisplayFormat( displayFormat );

  const bool calendar = config( QStringLiteral( "calendar_popup" ), true ).toBool();
  if ( calendar != mQDateTimeEdit->calendarPopup() )
  {
    mQDateTimeEdit->setCalendarPopup( calendar );
  }
  if ( calendar && mQDateTimeEdit->calendarWidget() )
  {
    // highlight today's date
    QTextCharFormat todayFormat;
    todayFormat.setBackground( QColor( 160, 180, 200 ) );
    mQDateTimeEdit->calendarWidget()->setDateTextFormat( QDate::currentDate(), todayFormat );
  }

  const bool allowNull = config( QStringLiteral( "allow_null" ), true ).toBool();
  if ( mQgsDateTimeEdit )
  {
    mQgsDateTimeEdit->setAllowNull( allowNull );
  }
  else
  {
    QgsApplication::messageLog()->logMessage( tr( "The usual date/time widget QDateTimeEdit cannot be configured to allow NULL values. "
        "For that the QGIS custom widget QgsDateTimeEdit needs to be used." ),
        QStringLiteral( "field widgets" ) );
  }

  if ( mQgsDateTimeEdit )
  {
    connect( mQgsDateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, &QgsDateTimeEditWrapper::dateTimeChanged );
  }
  else
  {
    connect( mQDateTimeEdit, &QDateTimeEdit::dateTimeChanged, this,  &QgsDateTimeEditWrapper::dateTimeChanged );
  }
}

bool QgsDateTimeEditWrapper::valid() const
{
  return mQgsDateTimeEdit || mQDateTimeEdit;
}

void QgsDateTimeEditWrapper::showIndeterminateState()
{
  if ( mQgsDateTimeEdit )
    mQgsDateTimeEdit->setEmpty();
}

void QgsDateTimeEditWrapper::dateTimeChanged( const QDateTime &dateTime )
{
  const QString fieldFormat = config( QStringLiteral( "field_format" ), QgsDateTimeFieldFormatter::defaultFormat( field().type() ) ).toString();
  emit valueChanged( dateTime.toString( fieldFormat ) );
}

QVariant QgsDateTimeEditWrapper::value() const
{
  if ( !mQDateTimeEdit )
    return QVariant( field().type() );

  if ( field().type() == QVariant::DateTime )
  {
    if ( mQgsDateTimeEdit )
    {
      return mQgsDateTimeEdit->dateTime();
    }
    else
    {
      return mQDateTimeEdit->dateTime();
    }
  }

  const QString fieldFormat = config( QStringLiteral( "field_format" ), QgsDateTimeFieldFormatter::defaultFormat( field().type() ) ).toString();

  if ( mQgsDateTimeEdit )
  {
    return mQgsDateTimeEdit->dateTime().toString( fieldFormat );
  }
  else
  {
    return mQDateTimeEdit->dateTime().toString( fieldFormat );
  }
}

void QgsDateTimeEditWrapper::setValue( const QVariant &value )
{
  if ( !mQDateTimeEdit )
    return;

  const QString fieldFormat = config( QStringLiteral( "field_format" ), QgsDateTimeFieldFormatter::defaultFormat( field().type() ) ).toString();
  const QDateTime date = field().type() == QVariant::DateTime ? value.toDateTime() : QDateTime::fromString( value.toString(), fieldFormat );

  if ( mQgsDateTimeEdit )
  {
    mQgsDateTimeEdit->setDateTime( date );
  }
  else
  {
    mQDateTimeEdit->setDateTime( date );
  }
}

void QgsDateTimeEditWrapper::setEnabled( bool enabled )
{
  if ( !mQDateTimeEdit )
    return;

  mQDateTimeEdit->setEnabled( enabled );
}
