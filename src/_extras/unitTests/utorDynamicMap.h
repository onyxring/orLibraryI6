!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! YYYY.MM.DD utorDynamicMap
! Unit Tests for the utorDynamicMap orLibrary extension.
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
default        utorDynamicMap_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orDynamicMap";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utorDynamicMap_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorDynamicMap_STAGE  < LIBRARY_STAGE);
   #undef      utorDynamicMap_STAGE  ;
   Constant    utorDynamicMap_STAGE  LIBRARY_STAGE;
   #ifdef      utorDynamicMap_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorDynamicMap...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   orDynamicMap with cant_go "The trees are too dense.", found_in wateredge forestmidst treasuretrove;

   orDynamicMapRoom wateredge "Edge of water" has light
   with s_to "Your boat is that way, but lets go explore.", 
   description "A small strip separating the south ocean from the surrounding forest.";

   orDynamicMapRoom forestmidst "Midst of Forest" has light
   with description "In the forest. Lots of trees.";

   orDynamicMapRoom treasuretrove "Pile of Treasure" has light
   with description "At last! The long-lost treasure!"; 

orUnitTest "utorDynamicMap" 
   with tests [; PlayerTo(wateredge, 3); ] noTest 
      "s" "boat is that way" 
      "e" "Midst of Forest" 
      "n" "Pile of Treasure"   
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
