!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! YYYY.MM.DD utPrefixSuffix
! A template for extensions.
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
default        utPrefixSuffix_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orTest";
#include "orPrefixSuffix";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utPrefixSuffix_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utPrefixSuffix_STAGE  < LIBRARY_STAGE);
   #undef      utPrefixSuffix_STAGE  ;
   Constant    utPrefixSuffix_STAGE  LIBRARY_STAGE;
   #ifdef      utPrefixSuffix_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utPrefixSuffix...";#endif;
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
      object utPrefixSuffixRoom "orPrefixSuffix Room" has light with description "As good a place as any";

      object -> mrsRobinson "Mrs. Robinson" has proper with description "Loved by Beatles.  And Jesus.",
			name 'mrs' 'robinson';

      orTestCollection orPrefixSuffixTests;
		  orTest ->  with setup[; move player to utPrefixSuffixRoom;],
         command "x mrs. robinson",
		  	assert[rsp;
		  		if(rsp.contains("And Jesus")==false) return "Expected description for mrsRobinson object.";
				if(rsp.contains("That's not a verb I recogni")) return "Expected secondary command, after full stop, to be removed.";
		  	];
#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
