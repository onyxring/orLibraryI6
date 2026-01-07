!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorVagueQuantity
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

class fish with name 'fish',
   short_name "fish",
   plural "fish",
   vagueQuantity 4 "some" 6 "a school of" true 9 "far too many";

object -> bowl2 "bowl" with name 'bowl' has open container;    
    fish ->->; fish ->->; fish ->->; fish ->->; 
    fish ->->; fish ->->; fish ->->; fish ->->; 
    fish ->->; fish ->->; fish ->->; fish ->->;    

orUnitTest "utorVagueQuantity" 
   with tests [; PlayerTo(utorVagueQuantityStart, 3); ] noTest 
   "x bowl" "are far too many fish."   
   "take 5 fish" "Taken"
   "x bowl" "is a school of fish."  
   "take 3 fish. x bowl" "are some fish."  
   "take 1 fish. x bowl" "are three fish."  
   "drop all" noTest !--clear for other unit tests
   ;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
