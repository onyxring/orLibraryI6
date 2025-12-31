!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorNpcSkillDialogue
! Unit Tests for the utorNpcSkillDialogue orLibrary extension.
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
default        utorNpcSkillDialogue_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orNpcSkillDialogue";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utorNpcSkillDialogue_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorNpcSkillDialogue_STAGE  < LIBRARY_STAGE);
   #undef      utorNpcSkillDialogue_STAGE  ;
   Constant    utorNpcSkillDialogue_STAGE  LIBRARY_STAGE;
   #ifdef      utorNpcSkillDialogue_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorNpcSkillDialogue...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utorNpcSkillDialogueStart "orNpcSkillDialogue Room" has light with description "Where orNpcSkillDialogue tests are run.";

     orNpc -> medusa "woman" has female
      class orNpcSkillDialogue
      with name 'woman' 'medusa' 'gorgon' 'redhead'
      , 	description "Dressed in a toga with a dozen red, rubber snakes intertwined with her long red curls.  They bounce around her face as she moves.";

         orTopic medusa_costume "Medusa costume"  
         with quip "~I've always been fascinated with Greek mythology.~ She glances at her reflection in the mirror. ~My roommate helped me make this.  She's a theatrical major and gets a lot of experience working with costumes.~"
         
         ,  knownBy medusa 
         ,  context selfObj
         ,  relatedTopics dracula_costume; ! theater_history; 

         orTopic dracula_costume "Dracula costume"
            with quip "~You chose Dracula?~ she asks, arching an eyebrow."	"~Dracula was always my favorite monster,~ you say, glancing at your reflection."
            ,  name	'dracula' 'costume'
            ,  knownBy	selfObj
            ,  context	medusa
            ,  relatedTopics medusa_costume dracula_frost;
         
         orTopic -> dracula_frost "frost"
            with quip "~You like Frost, then?~" 
                  "You smile.  ~My mother used to stay up late and read poetry to me to me as a child.  'The Road Not Taken' stands out most in my memories.~"
            ,  knownBy	selfObj
            ,  context	medusa
            ,	name 'robert' 'frost';

orUnitTest "utorNpcSkillDialogue" 
   with tests [; PlayerTo(utorNpcSkillDialogueStart, 3); medusa.takeNoticeOfTopic(medusa_costume); ] noTest
               "z" "fascinated with Greek mythology"
               "z" "chose Dracula?"   
               [; orDialogueAutoAnswer=true; playerCommands.pushCommand("answer medusa");] [val retval; 
                                                                                             retval=retval+self.assertContains("Dracula was always my favorite");
                                                                                             retval=retval+self.assertContains("My mother used to stay up late");
                                                                                             return retval==2;
                                                                                          ]              
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
