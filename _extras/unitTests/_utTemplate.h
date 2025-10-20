!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! YYYY.MM.D6 <REPLACEWITHNAME>
! Unit Tests for the <REPLACEWITHNAME> orLibrary extension.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! Revision History
! 2024.05.06	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        <REPLACEWITHNAME>_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orTest";
#include "<REPLACEWITHNAME>";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        <REPLACEWITHNAME>_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       <REPLACEWITHNAME>_STAGE  < LIBRARY_STAGE);
   #undef      <REPLACEWITHNAME>_STAGE  ;
   Constant    <REPLACEWITHNAME>_STAGE  LIBRARY_STAGE;
   #ifdef      <REPLACEWITHNAME>_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   <REPLACEWITHNAME>...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object <REPLACEWITHNAME>Room "<REPLACEWITHNAME> Room" has light with description "As good a place as any";

   orTestCollection <REPLACEWITHNAME>Tests;
		  orTest ->  with setup[; move player to <REPLACEWITHNAME>Room;],
         command "",
		  	assert[rsp;
		  	];

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
