!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorFirstImpressions
! Unit Tests for the utorFirstImpressions orLibrary extension.
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
#ifndef        utorFirstImpressions_STAGE;
default        utorFirstImpressions_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orFirstImpressions";
!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        utorFirstImpressions_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorFirstImpressions_STAGE  < LIBRARY_STAGE);
   #undef      utorFirstImpressions_STAGE  ;
   Constant    utorFirstImpressions_STAGE  LIBRARY_STAGE;
   #ifdef      utorFirstImpressions_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorFirstImpressions...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utorFirstImpressionsStart "orFirstImpressions Room" has light with description "Where orFirstImpressions tests are run.";

object candlestick "candlestick" utorFirstImpressionsStart
   with name 'candlestick',
   description "It was copper, or perhaps brass, polished to a
      dull gleam. Even free of a candle it was
      heavy and hard enough to make a formidable
      weapon.",
   preImpression "A sense of unease settled on me as I
      looked at the candlestick.",
   postImpression "I hefted it, calculating the force it would
      exert across the back of someone's head. Definitely
      lethal.";

orUnitTest "orFirstImpressions" 
   with tests [; PlayerTo(utorFirstImpressionsStart, 3); ] noTest
      "x candlestick" "Definitely lethal."
      "x candlestick" "!Definitely lethal."
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
