/* f4info.c   (c)Copyright Sequiter Software Inc., 1988-2001.  All rights reserved. */

#include "D4all.h"
#ifndef S4UNIX
   #ifdef __TURBOC__
      #pragma hdrstop
   #endif
#endif

FIELD4INFO *S4FUNCTION d4fieldInfo( DATA4 *data )
{
   FIELD4INFO *fieldInfo ;
   FIELD4 *field ;
   short int i ;

   #ifdef E4PARM_HIGH
      if ( data == 0 )
      {
         error4( 0, e4parm_null, E92201 ) ;
         return (FIELD4INFO *)0 ;
      }
   #endif

   if ( error4code( data->codeBase ) < 0 )
      return (FIELD4INFO *)0 ;

   fieldInfo = (FIELD4INFO *)u4allocFree( data->codeBase, ( (long)d4numFields( data ) + 1L ) * (long)sizeof( FIELD4INFO ) ) ;
   if ( fieldInfo == 0 )
      return (FIELD4INFO *)0 ;

   for ( i = 0 ; i < d4numFields( data ) ; i++ )
   {
      field = d4fieldJ( data, (short int)(i + 1) ) ;
      // AS Oct 27/03 - long field names support
      fieldInfo[i].name = field->longName ;
      if ( field->type == r5wstrLen )
         fieldInfo[i].len = field->len - 2 ;  // subtract 2 extra bytes to store field length which are added on a create
      else
         fieldInfo[i].len = field->len ;
      fieldInfo[i].dec = field->dec ;
      // AS 04/06/00 this is not correct because if a type can contain binary info, the type may be different
      if ( field->binary == 1 )  // set to '2' indicates 4 byte r4memo/r4gen, '1' indicates r4memoBin
      {
         switch( field->type )
         {
            case r4str:  // is binary character then
               fieldInfo[i].type = r4charBin ;
               break ;
            case r4memo:  // is binary memo then
               fieldInfo[i].type = r4memoBin ;
               break ;
            default:
               fieldInfo[i].type = (char)field->type ;
               break ;
         }
      }
      else
         fieldInfo[i].type = (char)field->type ;
      // AS Apr 9/03 - Was not registering if auto-timestamp or auto-increment
      if ( field->null )
         fieldInfo[i].nulls = r4null ;
      #ifdef S4CLIENT_OR_FOX  /* LY 2003/06/05 : auto*** members not declared for other index types */
         else if ( field->autoIncrement )
            fieldInfo[i].nulls = r4autoIncrement ;
         else if ( field->autoTimestamp )
            fieldInfo[i].nulls = r4autoTimestamp ;
      #endif
   }

   return fieldInfo ;
}
