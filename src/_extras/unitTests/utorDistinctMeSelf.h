!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! YYYY.MM.DD utorDistinctMeSelf
! Unit Tests for the utorDistinctMeSelf orLibrary extension.
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
default        utorDistinctMeSelf_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orDistinctMeSelf";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utorDistinctMeSelf_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorDistinctMeSelf_STAGE  < LIBRARY_STAGE);
   #undef      utorDistinctMeSelf_STAGE  ;
   Constant    utorDistinctMeSelf_STAGE  LIBRARY_STAGE;
   #ifdef      utorDistinctMeSelf_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorDistinctMeSelf...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utorDistinctMeSelfStart "orDistinctMeSelf Room" has light with description "Where orDistinctMeSelf tests are run.";

    object -> friar "friar" has animate male
      with name 'friar', 
      orders[; 
            Bless: 
               <Bless noun, friar>;
            rtrue;
      ];

[BlessSub; 
   CSubjectVerb(actor,0,0,"administer",0,"administers");
    print " a blessing upon ";
    if(actor==noun) OnesSelf(noun); 
    else ThatOrThose(noun);
    ".";
];

orUnitTest "utorDistinctMeSelf" 
   with tests [; PlayerTo(utorDistinctMeSelfStart, 3); ] noTest
               "friar, bless me" "upon you"   
               "friar, bless self" "upon himself"   
               "friar, bless yourself" "upon himself"      
               "friar, bless friar" "upon himself"   
;
#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);
   verb 'bless' * noun -> bless;
#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
