#include <QtScript>
#include <QtCore>

#include "mongocursorprototype.h"

using soft::mongo::Cursor;

MongoCursorPrototype :: MongoCursorPrototype (QObject *parent)
   : QObject (parent)
{}

MongoCursorPrototype :: ~MongoCursorPrototype ()
{}

Cursor *MongoCursorPrototype :: self() const
{
   Cursor *cursorUri = qscriptvalue_cast<Cursor*>(thisObject());
   if (!cursorUri) {
      context()->throwError (QString (tr ("Cannot instanciate MongoCursor")));
   }
   return cursorUri;
}

bool MongoCursorPrototype :: more() const
{
   return self()->more();
}

bool MongoCursorPrototype :: error() const
{
   return self()->error();
}

bool MongoCursorPrototype :: isAlive() const
{
   return self()->isAlive();
}

QScriptValue MongoCursorPrototype :: next()
{
   return engine()->toScriptValue (self()->next());
}

QScriptValue MongoCursorPrototype :: clone()
{
   return engine()->toScriptValue (self()->clone());
}

QScriptValue MongoCursorPrototype :: current()
{
   return engine()->toScriptValue (self()->current());   
}

