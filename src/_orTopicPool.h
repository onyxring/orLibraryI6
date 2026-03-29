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
#ifndef        orTopicPool_STAGE;
Constant        orTopicPool_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orDialogue";
#include "orUtilArray";
#include "_orInfExt";
#include "_orTopic";
!--------------------------------------------------------------------------------------
#endif;
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
	Constant allTopics 1;
	default  orMaxTopicPoolSize 20;
	Array 	 playerDialoguePool table orMaxTopicPoolSize;
 
	property individual dialoguePool;
	property individual relatedTopics;
	property individual excludedTopics;
#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);

	[playerTakeNoticeOfTopic topic;
         if(util.orArray.find(playerDialoguePool, topic)==-1 && topic.isTellable()) util.orArray.insert(playerDialoguePool,0,topic);		 
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
			! removeExcludedTopics(top, act, talkingTo);
			! imprintRelatedTopics(top, act, talkingTo);
			! rfalse;
		]
   ,	ext_topicTell[top act talkingTo;

		]
	,	ext_topicTold[top act talkingTo;
			removeExcludedTopics(top, act, talkingTo);
			imprintRelatedTopics(top, act, talkingTo);
			rfalse;
		]
	;

	[removeExcludedTopics top act talkingTo
			t relTop;

		util.orArray.removeValue(playerDialoguePool, top);
		if(act provides dialoguePool) util.orArray.removeValue(act, dialoguePool, top);			
		if(talkingTo provides dialoguePool) util.orArray.removeValue(talkingTo, dialoguePool, top);			

		if(top provides excludedTopics){
			for(t=0:t<util.orArray.getLength(top,excludedTopics):t++){
				relTop=util.orArray.get(top,excludedTopics,t);
				if(relTop==allTopics){
					if(act==player) util.orArray.clear(playerDialoguePool);
					if(act provides dialoguePool) util.orArray.clear(act,dialoguePool,relTop);	
					break;
				}
				
				if(act==player) util.orArray.remove(playerDialoguePool,relTop);
				if(act provides dialoguePool) util.orArray.remove(act,dialoguePool,relTop);
				
				if(talkingTo==player) util.orArray.remove(playerDialoguePool,relTop);
				if(talkingTo provides dialoguePool) util.orArray.remove(talkingTo,dialoguePool,relTop);
				
			}
		}
	];
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
