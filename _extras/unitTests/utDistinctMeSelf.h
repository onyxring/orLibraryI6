!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! YYYY.MM.DD utDistinctMeSelf
! Unit tests for the orDistinctMeSelf extension.
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
default        utDistinctMeSelf_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orTest";
#include "orDistinctMeSelf";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utDistinctMeSelf_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utDistinctMeSelf_STAGE  < LIBRARY_STAGE);
   #undef      utDistinctMeSelf_STAGE  ;
   Constant    utDistinctMeSelf_STAGE  LIBRARY_STAGE;
   #ifdef      utDistinctMeSelf_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utDistinctMeSelf...";#endif;
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
      object utDistinctMeSelfRoom "orDistinctMeSelf Room" has light with description "As good a place as any";

      object -> friar "Friar" has animate male
		with name 'friar',
			orders[;
				Bless:
					<Bless noun, friar>;
					rtrue;
			];

		orTestCollection orDistinctMeSelfTests;
		  orTest ->  with setup[; move player to utDistinctMeSelfRoom;],
         command "friar, bless me",
		  	assert[rsp;
		  		if(rsp.contains("a blessing upon you.")==false) return "Expected me resolution to the player.";
		  	];
		  orTest ->  with command "friar, bless self",
		  	assert[rsp;
		  		if(rsp.contains("a blessing upon himself.")==false) return "Expected self resolution to the friar.";
		  	];
		  orTest ->  with command "friar, bless yourself",
		  	assert[rsp;
		  		if(rsp.contains("a blessing upon himself.")==false) return "Expected yourself resolution to the friar.";
		  	];

#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);
   [BlessSub;
		CSubjectVerb(actor,0,0,"administer",0,"administers");
				print " a blessing upon ";
		if(actor==noun) OnesSelf(noun);
			else ThatOrThose(noun);
			".";
	];

	verb 'bless' * noun -> Bless;


#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
