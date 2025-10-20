!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.10.07 orTopicPool
! A template for extensions.
!--------------------------------------------------------------------------------------
! Created by <AUTHOR NAME>
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
!
!--------------------------------------------------------------------------------------
! Revision History
! 2024.10.07	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orTopicPool_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orDialogue";
#include "orUtilArray";
#include "_orInfExt";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orTopicPool_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orTopicPool_STAGE  < LIBRARY_STAGE);
   #undef      orTopicPool_STAGE  ;
   Constant    orTopicPool_STAGE  LIBRARY_STAGE;
   #ifdef      orTopicPool_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orTopicPool..."; #endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	default  orMaxTopicPoolSize 20;
	Array 	 playerDialoguePool table orMaxTopicPoolSize;

	property individual dialoguePool;
	property individual relatedTopics;
#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);

	[playerTakeNoticeOfTopic topic;
         if(util.orArray.find(playerDialoguePool, topic)==-1) util.orArray.insert(playerDialoguePool,topic);
    ];

	[imprintRelatedTopics top c1 c2
			t relTop;
		if(top provides relatedTopics){
			for(t=0:t<util.orArray.getLength(top,relatedTopics):t++){
				relTop=util.orArray.get(top,relatedTopics,t);

				if(relTop.isAppropriateFor(c1,c2)){
					if(c1==player){
						if(util.orArray.find(playerDialoguePool,relTop)==-1) util.orArray.prepend(playerDialoguePool,relTop);
					}
					else{
						if(c1 provides dialoguePool){
							if(util.orArray.find(c1, dialoguePool, relTop)==-1) util.orArray.prepend(c1,dialoguePool,relTop);
						}
					}
				}
				if(c2==0) continue;
				if(relTop.isAppropriateFor(c2,c1)){
					if(c2==player){
						if(util.orArray.find(playerDialoguePool,relTop)==-1) util.orArray.prepend(playerDialoguePool,relTop);
					}else {
						if(c2 provides dialoguePool){
							if(util.orArray.find(c2, dialoguePool, relTop)==-1) util.orArray.prepend(c2,dialoguePool,relTop);
						}
					}
				}
			}
		}
	];
	orInfExt
	 with ext_topicAsk[top act talkingTo;

		]
	,	ext_topicAsked[top act talkingTo;

			imprintRelatedTopics(top, act, talkingTo);

			if(act==player){
				util.orArray.removeValue(playerDialoguePool, top);
			}else{
				if(act provides dialoguePool) util.orArray.removeValue(act, dialoguePool, top);
			}

			rfalse;
		]
   ,	ext_topicTell[top act talkingTo;

		]
	,	ext_topicTold[top act talkingTo;

			imprintRelatedTopics(top, act, talkingTo);
			if(act==player){
				util.orArray.removeValue(playerDialoguePool, top);
			}else{
				if(act provides dialoguePool) util.orArray.removeValue(act, dialoguePool, top);
			}
			rfalse;
		]
	;

#endif; !--After Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
