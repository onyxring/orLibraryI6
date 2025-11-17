!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! YYYY.MM.DD utBetterChoice
! Unit tests for the orBetterChoice extension.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! Revision History
! YYYY.MM.DD	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        utBetterChoice_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orTest";
#include "orBetterChoice";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utBetterChoice_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utBetterChoice_STAGE  < LIBRARY_STAGE);
   #undef      utBetterChoice_STAGE  ;
   Constant    utBetterChoice_STAGE  LIBRARY_STAGE;
   #ifdef      utBetterChoice_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utBetterChoice...";#endif;
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
   object utBetterChoiceRoom "orBetterChoice Room" has light with description "As good a place as any";

   object -> dish "immovable candy dispensor" has supporter static with name 'candy' 'dispensor', add_to_scope handle;
		object chocolate "chocolate candy" dish with name 'chocolate' 'candy' has edible;
		object toy "plastic candy toy" dish with name 'plastic' 'candy' 'toy';
		object handle "candy dispensor's handle" has static with  name 'candy' 'dispensor^s' 'handle';

   orTestCollection orBetterChoiceTests;
		orTest ->  with setup[; move player to utBetterChoiceRoom;],
         command "take candy",
		 	assert[rsp;
		  		if(rsp.contains("Which do you mean")==false) return "Expected a disambiguation request between two items: chocoloate and plastic candy toy.";
		  		if(rsp.contains("handle"))  return "Disambiguation list includes items brought into scope (handle).";
		  		if(rsp.contains("dispensor"))  return "Disambiguation list includes static items (dispensor).";
		  	];
		orTest ->  with command "take all candy";
		orTest ->  with command "drop candy",
		 	assert[rsp;
		 		if(rsp.contains("Which do you mean")==false) return "Expected a disambiguation request between two items: chocoloate and plastic candy toy.";
		 		if(rsp.contains("handle") || rsp.contains("dispensor"))  return "Disambiguation list includes items not held.";
		 	];
		orTest ->  with setupCommands "drop all candy",
		 	command "eat candy",
		 	assert[rsp;
		 		if(rsp.contains("You eat the chocolate")==false) return "Expected correct resolution of the edible item.";
		 	];
#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
