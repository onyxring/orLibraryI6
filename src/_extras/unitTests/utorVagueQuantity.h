!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! YYYY.MM.DD utorVagueQuantity
! Unit Tests for the utorVagueQuantity orLibrary extension.
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
default        utorVagueQuantity_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orVagueQuantity";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utorVagueQuantity_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorVagueQuantity_STAGE  < LIBRARY_STAGE);
   #undef      utorVagueQuantity_STAGE  ;
   Constant    utorVagueQuantity_STAGE  LIBRARY_STAGE;
   #ifdef      utorVagueQuantity_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorVagueQuantity...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utorVagueQuantityStart "orVagueQuantity Room" has light with description "Where orVagueQuantity tests are run.";

class marble2 with name 'marble' 'marbles//p', short_name "marble",
   plural "marbles";

object -> bowl2 "bowl" with name 'bowl' has open container;
    
marble2 ->->; marble2 ->->; marble2 ->->; marble2 ->->; 
    marble2 ->->; marble2 ->->; marble2 ->->; marble2 ->->; 
    marble2 ->->; marble2 ->->; marble2 ->->; marble2 ->->;    


orUnitTest "utorVagueQuantity" 
   with tests [; PlayerTo(utorVagueQuantityStart, 3); ] noTest 
   "x bowl" "are numerous marbles."   
   "take 6 marbles. x bowl" "are several marbles."  
   "take 3 marbles. x bowl" "are a few marbles."  
   "take 1 marble. x bowl" "are a couple of marbles."  
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
