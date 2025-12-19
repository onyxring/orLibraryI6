!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! YYYY.MM.DD utAdjective
! Unit tests for the orAdjective extension.
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
default        utAdjective_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orAdjective";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utAdjective_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utAdjective_STAGE  < LIBRARY_STAGE);
   #undef      utAdjective_STAGE  ;
   Constant    utAdjective_STAGE  LIBRARY_STAGE;
   #ifdef      utAdjective_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utAdjective...";#endif;
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
   object utAdjectiveRoom "orAdjective Room" has light with description "As good a place as any";
         object -> glass "drinking glass" with name 'glass', adjective 'drinking';
         object -> looking_glass "looking glass" with name 'glass', adjective  'looking';
         object -> marble "glass marble" with name 'marble', adjective  'glass';
         object -> marble_table "marble table" with name 'table', adjective  'marble';

   orUnitTestCollection orAdjectiveTests "orAdjective";
		orUnitTest -> with setup[; move player to utAdjectiveRoom;]
      ,  command "x glass",
			assert[rsp;
				if(rsp.contains("Which do you mean")==false) return "Expected a disambiguation request between two items: looking glass and drinking glass.";
				if(rsp.contains("glass marble"))  return "Disambiguation list includes items with only adjectives (glass marble).";
			];
		orUnitTest -> with command "x looking glass",
			assert[rsp; if(rsp.contains("You see nothing special about the looking glass.")==false) return "Expected resolution to the looking glass.";];

		orUnitTest -> with command "x marble",
			assert[rsp; if(rsp.contains("You see nothing special about the glass marble.")==false) return "Expected resolution to the glass marble.";];

		orUnitTest -> with command "x table",
			assert[rsp; if(rsp.contains("You see nothing special about the marble table.")==false) return "Expected resolution to the marble table.";];

		orUnitTest -> with command "x drinking",
			setup[; #ifdef orAdjective; orAdjective.mode=PREFER_NOUN;#endif;],
			assert[rsp; if(rsp.contains("You see nothing special about the drinking glass.")==false) return "Expected resolution to the drinking glass.";];

		orUnitTest -> with command "x drinking",
			setup[; #ifdef orAdjective; orAdjective.mode=REQUIRE_NOUN; #endif;],
			assert[rsp; if(rsp.contains("You can't see any such thing.")==false) return "Expected no resolution from adjective only reference.";];

#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
