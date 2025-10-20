!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.05.14 utNameable
! A template for extensions.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! Revision History
! 2024.05.14	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        utNameable_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orTest";
#include "orNameable";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utNameable_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utNameable_STAGE  < LIBRARY_STAGE);
   #undef      utNameable_STAGE  ;
   Constant    utNameable_STAGE  LIBRARY_STAGE;
   #ifdef      utNameable_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utNameable...";#endif;
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
  object utNameableRoom "orNameable Room" has light with description "As good a place as any";

  orNameable -> golem "golem" has animate with description "The creature looks interested in pretty things.",
      name 'golem';

  orNameable -> rose "rose" with description "Smells as sweet.",
      name 'rose';

  object -> brick "brick" with description "A brick.",
      name 'brick';

  orTestCollection orNameableTests;
		orTest -> with setup[; move player to utNameableRoom;]
      ,  command "name golem ~bob~",
			assert[rsp;
        if(rsp.contains("You begin to think of the golem as ~Bob.~")==false) return "Expected the golem to be named bob.";
			];
      orTest -> with command "x bob",
			assert[rsp;
			  if(rsp.contains("looks interested in pretty")==false) return "Expected description of golem.";
			];
      orTest -> with command "name rose bob",
			assert[rsp;
			  if(rsp.contains("You begin to think of the rose as ~Bob.~")==false) return "Expected the rose to be named bob.";
			];
      orTest -> with command "x bob",
			assert[rsp;
			  if(rsp.contains("Which do you mean")==false) return "Expected disambiguation request.";
			];
      orTest -> with command "name brick cube",
			assert[rsp;
        if(rsp.contains("cannot be named")==false) return "Expected ~cannot be named~ response.";
			];
      orTest -> with command "unname bob",
			assert[rsp;
			  if(rsp.contains("Which do you mean")==false) return "Expected disambiguation request.";
			];
      orTest -> with command "unname rose",
			assert[rsp;
        if(rsp.contains("You forget any name you may have given the rose.")==false) return "Expected rose to be unnamed.";
			];
      orTest -> with command "unname bob",
			assert[rsp;
			  if(rsp.contains("You forget any name you may have given the golem.")==false) return "Expected golem to be unnamed.";
			];
      orTest -> with command "unname brick",
			assert[rsp;
			if(rsp.contains("You forget any name you may have given the brick.")==false) return "Expected brick to be unnamed.";
			];
#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
