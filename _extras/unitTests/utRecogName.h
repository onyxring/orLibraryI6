!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.05.06 utRecogName
! Unit Tests for the orRecogName orLibrary extension.
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
default        utRecogName_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orTest";
#include "orRecogName";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utRecogName_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utRecogName_STAGE  < LIBRARY_STAGE);
   #undef      utRecogName_STAGE  ;
   Constant    utRecogName_STAGE  LIBRARY_STAGE;
   #ifdef      utRecogName_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utRecogName...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);

#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);

#endif; !--After Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utRecogNameRoom "orRecogName Room" has light with description "As good a place as any";

   object -> browndog "great brown dog"
   with description "Happy and brown.",
      name 'dog' 'brown' 'great';

   object -> SmellyCat "dirty smelly cat"
   with description "It stinks.";

   orTestCollection orRecogNameTests;
		  orTest ->  with setup[; move player to utRecogNameRoom;],
         command "x cat",
		  	assert[rsp;
		  		if(rsp.contains("It stinks.")==false) return "Expected resolution to SmellyCat object.";
		  	];

		  orTest ->  with command "x dog",
		  	assert[rsp;
		  		if(rsp.contains("Happy and brown.")==false) return "Expected resolution to browndog object.";
		  	];

#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
