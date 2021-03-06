/************************************************************************
 * This file has been generated automatically from                      *
 *                                                                      *
 * src/server/qgsserverresponse.h                                       *
 *                                                                      *
 * Do not edit manually ! Edit header and run scripts/sipify.pl again   *
 ************************************************************************/







class QgsServerResponse
{

%TypeHeaderCode
#include "qgsserverresponse.h"
%End
  public:

    QgsServerResponse();
%Docstring
constructor
%End

    virtual ~QgsServerResponse();
%Docstring
destructor
%End

    virtual void setHeader( const QString &key, const QString &value ) = 0;
%Docstring
Set Header entry
Add Header entry to the response
Note that it is usually an error to set Header after data have been sent through the wire
%End

    virtual void removeHeader( const QString &key ) = 0;
%Docstring
Clear header
Undo a previous 'setHeader' call
%End

    virtual QString header( const QString &key ) const = 0;
%Docstring
Return the header value
%End

    virtual QMap<QString, QString> headers() const = 0;
%Docstring
Return the header value
%End

    virtual bool headersSent() const = 0;
%Docstring
Return true if the headers have alredy been sent
%End


    virtual void setStatusCode( int code ) = 0;
%Docstring
Set the http status code

:param code: HTTP status code value
%End

    virtual int statusCode() const = 0;
%Docstring
Return the http status code
%End

    virtual void sendError( int code,  const QString &message ) = 0;
%Docstring
Send error
This method delegates error handling at the server level. This is different
from calling setReturnCode() which let you return a specific response body.
Calling sendError() will end the transaction and any attempt to write data
or set headers will be an error.

:param code: HHTP return code value
:param message: An informative error message
%End

    virtual void write( const QString &data );
%Docstring
Write string
This is a convenient method that will write directly
to the underlying I/O device
%End

    virtual qint64 write( const QByteArray &byteArray );
%Docstring
Write chunk of data
This is a convenient method that will write directly
to the underlying I/O device

:return: the number of bytes that were actually written
%End



    virtual void write( const QgsServerException &ex );
%Docstring
Write server exception
%End

    virtual QIODevice *io() = 0;
%Docstring
Return the underlying QIODevice
%End

    virtual void finish() = 0;
%Docstring
Finish the response,  ending the transaction
%End

    virtual void flush() = 0;
%Docstring
Flushes the current output buffer to the network

'flush()' may be called multiple times. For HTTP transactions
headers will be written on the first call to 'flush()'.
%End

    virtual void clear() = 0;
%Docstring
Reset all headers and content for this response
%End

    virtual QByteArray data() const = 0;
%Docstring
Get the data written so far

This is implementation dependent: some implementations may not
give access to the underlying and return an empty array.

Note that each call to 'flush' may empty the buffer and in case
of streaming process you may get partial content
%End

    virtual void truncate() = 0;
%Docstring
Truncate data

Clear internal buffer
%End
};

/************************************************************************
 * This file has been generated automatically from                      *
 *                                                                      *
 * src/server/qgsserverresponse.h                                       *
 *                                                                      *
 * Do not edit manually ! Edit header and run scripts/sipify.pl again   *
 ************************************************************************/
