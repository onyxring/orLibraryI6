!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorNpcSkillMove
! Unit Tests for the utorNpcSkillMove orLibrary extension.
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
default        utorNpcSkillMove_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orNpcSkillMove";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utorNpcSkillMove_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorNpcSkillMove_STAGE  < LIBRARY_STAGE);
   #undef      utorNpcSkillMove_STAGE  ;
   Constant    utorNpcSkillMove_STAGE  LIBRARY_STAGE;
   #ifdef      utorNpcSkillMove_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorNpcSkillMove...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   
   object foyer "Foyer" has light
   with description "This is a clean, simple entry hall.  Exits lead southwest, southeast, and due south.",
   sw_to diningroom, s_to atrium, se_to study
   ;
   object diningroom "Dining Room" has light
   with description "This is really more of a breakfast nook. Exits lead northeast, southeast, and due east.",
   ne_to foyer, e_to atrium, se_to kitchen
   ;
   object kitchen "Kitchen" has light
   with description "The place where food is prepared. Exits lead northwest, northeast, and due north.",
   nw_to diningroom, n_to atrium, ne_to study
   ;
   object study "Study" has light
   with description "A library without books. Exits lead northwest, southwest, and due west.",
   nw_to foyer, w_to atrium, sw_to kitchen
   ;
   object atrium "Atrium" has light
   with description "Light filters down from above. Exits lead north, south, east, and west.",
   n_to foyer, e_to study, w_to diningroom, s_to kitchen
   ;


   orNpc maid_sm "maid" atrium has female 
      class orNpcSkillMove
      with name 'maid'
      ,  followTarget 0
      ,  priority 2
      ,  isEnabled false
      ;

   orNpc butler_sm "butler" atrium 
      class orNpcSkillMove
      with name 'butler'
      ,  priority 4
      ,  movePath foyer diningRoom study
      ,  isEnabled false;

orUnitTest "utorNpcSkillMove" 
   with tests [; maid_sm.isEnabled=true;
                  PlayerTo(atrium, 3); 
               ] noTest
               "z" "maid departs"   
               [;
                  move maid_sm to atrium; 
                  butler_sm.isEnabled=true; 
                  maid_sm.followTarget=butler_sm;
                  playerCommands.pushCommand("z");
               ] [;
                  if(self.assertContains("butler departs") && self.assertContains("maid departs")) rtrue;
                  rfalse;
               ]
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
