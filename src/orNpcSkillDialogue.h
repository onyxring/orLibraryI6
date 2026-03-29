!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.09.02 orNPCSkillDialogue
! A base class and controlling daemon for NPCs.
!--------------------------------------------------------------------------------------
! Created by Jin Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
!
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.08	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.09.02	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
#ifndef        orXNSorNPCSkillDialogue_STAGETAGE;
Constant        orNPCSkillDialogue_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orNpc";
#include "orDialogue";
#include "_orTopicPool";
!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        orNPCSkillDialogue_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orNPCSkillDialogue_STAGE  < LIBRARY_STAGE);
   #undef      orNPCSkillDialogue_STAGE  ;
   Constant    orNPCSkillDialogue_STAGE  LIBRARY_STAGE;
   #ifdef      orNPCSkillDialogue_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orNPCSkillDialogue...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
   global orDialogueAutoAnswer false;
#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);

   class orNPCSkillDialogue
      with dialoguePool 0 0 0 0 0
      , pendingQuestion 0
      , learnTopic[topic;
         if(util.orArray.find(topic,knownBy,self)==-1)
            util.orArray.append(topic,knownBy,self);
      ]
      , takeNoticeOfTopic[topic;
         if((self provides dialoguePool)==false) return;
         if(util.orArray.find(self, dialoguePool, topic)==-1) util.orArray.insert(self,dialoguePool,0,topic);
      ]
   ;
#endif; !--After Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   orNpcSkill _orNPCSkillDialogue
      with appliesToCharacter[npc; if(npc ofclass orNPCSkillDialogue) rtrue; rfalse;]
		,  canPerform[npc
               i count;
            for(i=0:i<util.orArray.getLength(npc, dialoguePool):i++){
               if(util.orArray.get(npc, dialoguePool).isAppropriateFor(npc, player)) 
                  count++;
            }
            return (count>0);
         ]
      ,  perform[npc 
               r;

            r=util.orArray.get(npc, dialoguePool,random(util.orArray.getLength(npc, dialoguePool))-1);
            util.orArray.removeValue(npc, dialoguePool,r);
            new_line;
            if(r.isKnownBy(npc))
               <tellTopic player r, npc>;
            else
               <askTopic player r, npc>;

         ]
	;

object _handleAutoAnswer LibraryExtensions
with priority 15 !--run after other stuff has had a chance to run
,  ext_topicAsked[top actor talkingTo; !--support AutoAnswer being turned off
      if(top.isResponseOnly()) rfalse; !--regardless of the auto answer being turned of, we will still auto answer topics which are designated as response only (via ORDIA_RESPONSE_ONLY)
      if(talkingTo==player && orDialogueAutoAnswer==false && actor provides pendingQuestion) {
         actor.pendingQuestion=top;
         rtrue;
      }
      rfalse;
];

#endif; !--After VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);
!======================================================================================
! AFTER GRAMMAR
[AnswerQuestionSub pq;
   if(noun provides pendingQuestion && noun.pendingQuestion~=0){
      pq=noun.pendingQuestion;
      noun.pendingQuestion=0;
      <<tellTopic noun pq>>;
   }
   else{
      "There doesn't seem to be any open questions.";
   }
];

Extend 'answer' first 
   *	creature -> AnswerQuestion;
#endif;
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
