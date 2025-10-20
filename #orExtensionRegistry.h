!--------------------------------------------------------------------------------------
! orExtensionRegistry.h
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
! The list of "registered" extensions currently managed by the orExtensionFramework.
! This file shouldn't be modified. To add personal extensions to the framework, your 
! put your extension in your own folder, add a file named "#orExtensionRegistryStub.h" 
! to it that folder, and ensure that directory folder is listed *before* the orLibrary 
! path in the include_path setting.  New extensions must be in the format required by 
! the framework (see #orExtensionFramework.h for a template) and may be "registered" 
! by adding a line for it using the same format as below.  
!--------------------------------------------------------------------------------------
#ifdef orActionQueue_STAGE; #include "_orActionQueue"; #endif;
#ifdef orAdjective_STAGE; #include "orAdjective"; #endif;
#ifdef orBackdrop_STAGE; #include "orBackdrop"; #endif;
#ifdef orBetterChoice_STAGE; #include "orBetterChoice"; #endif;
#ifdef orCantGoOdd_STAGE; #include "orCantGoOdd"; #endif;
#ifdef orCenter_STAGE; #include "orCenter"; #endif;
#ifdef orDeque_STAGE; #include "_orDeque"; #endif;
#ifdef orDialogue_STAGE; #include "orDialogue"; #endif;
#ifdef orDistinctMeSelf_STAGE; #include "orDistinctMeSelf"; #endif;
#ifdef orDistinctRead_STAGE; #include "orDistinctRead"; #endif;
#ifdef orDoor_STAGE; #include "orDoor"; #endif;
#ifdef orExits_STAGE; #include "orExits"; #endif;
#ifdef orFirstImpressions_STAGE; #include "orFirstImpressions"; #endif;
#ifdef orGibberish_STAGE; #include "orGibberish"; #endif;
#ifdef orGotoLocation_STAGE; #include "orGotoLocation"; #endif;
#ifdef orInfExt_STAGE; #include "_orInfExt"; #endif;
#ifdef orImplicitCommands_STAGE; #include "orImplicitCommands"; #endif;
#ifdef orHookBanner_STAGE; #include "_orHookBanner"; #endif;
#ifdef orHookDrawStatusLine_STAGE; #include "_orHookDrawStatusLine"; #endif;
#ifdef orHookInformLibrary_STAGE; #include "_orHookInformLibrary"; #endif;
#ifdef orHookKeyboard_STAGE; #include "_orHookKeyboard"; #endif;
#ifdef orHookParser_STAGE; #include "_orHookParser"; #endif;
#ifdef orHookParseToken_STAGE; #include "_orHookParseToken"; #endif;
#ifdef orHookVerbs_STAGE; #include "_orHookVerbs"; #endif;
#ifdef orHookWriteAfterEntry_STAGE; #include "_orHookWriteAfterEntry"; #endif;
#ifdef orHookWriteList_STAGE; #include "_orHookWriteList"; #endif;
#ifdef orMenu_STAGE; #include "orMenu"; #endif;
#ifdef orMenuTopics_STAGE; #include "orMenuTopics"; #endif;
#ifdef orNameable_STAGE; #include "orNameable"; #endif;
#ifdef orNpc_STAGE; #include "orNpc"; #endif;
#ifdef orNpcSkillDialogue_STAGE; #include "orNpcSkillDialogue"; #endif;
#ifdef orNpcSkillMove_STAGE; #include "orNpcSkillMove"; #endif;
#ifdef orParentChildRelationship_STAGE; #include "orParentChildRelationship"; #endif;
#ifdef orPlayerCommandQueue_STAGE; #include "orPlayerCommandQueue"; #endif;
#ifdef orPrefixSuffix_STAGE; #include "orPrefixSuffix"; #endif;
#ifdef orPrint_STAGE; #include "orPrint"; #endif;
#ifdef orPrintPatterns_English_STAGE; #include "_orPrintPatterns_English"; #endif;
#ifdef orPronoun_English_STAGE; #include "_orPronoun_English"; #endif;
#ifdef orRecogName_STAGE; #include "orRecogName"; #endif;
#ifdef orReferByContents_STAGE; #include "orReferByContents"; #endif;
#ifdef orRemoteLook_STAGE; #include "orRemoteLook"; #endif;
#ifdef orReview_STAGE; #include "orReview"; #endif;
#ifdef orRoutineList_STAGE; #include "_orRoutineList"; #endif;
#ifdef orStateDescriptors_STAGE; #include "orStateDescriptors"; #endif;
#ifdef orStatusLine_STAGE; #include "orStatusLine"; #endif;
#ifdef orString_STAGE; #include "orString"; #endif;
#ifdef orStylePrintRules_STAGE; #include "orStylePrintRules"; #endif;
#ifdef orHookStandardLibrary_stage; #include "_orHookStandardLibrary"; #endif;
#ifdef orTopic_STAGE; #include "_orTopic"; #endif;
#ifdef orTopicPool_STAGE; #include "_orTopicPool"; #endif;
#ifdef orDialogueMenu_STAGE; #include "orDialogueMenu"; #endif;
#ifdef orTrackSeen_STAGE; #include "_orTrackSeen"; #endif;
#ifdef orTransition_STAGE; #include "orTransition"; #endif;
#ifdef orUniqueMultiMessage_STAGE; #include "orUniqueMultiMessage"; #endif;
#ifdef orUnitTest_STAGE; #include "_orUnitTest"; #endif;
#ifdef orUtil_STAGE; #include "_orUtil"; #endif;
#ifdef orUtilArray_STAGE; #include "orUtilArray"; #endif;
#ifdef orUtilBuf_STAGE; #include "orUtilBuf"; #endif;
#ifdef orUtilChar_STAGE; #include "orUtilChar"; #endif;
#ifdef orUtilLoopArray_STAGE; #include "orUtilLoopArray"; #endif;
#ifdef orUtilMap_STAGE; #include "orUtilMap"; #endif;
#ifdef orUtilMapPathFinder_STAGE; #include "orUtilMapPathFinder"; #endif;
#ifdef orUtilNum_STAGE; #include "orUtilNum"; #endif;
#ifdef orUtilParser_STAGE; #include "orUtilParser"; #endif;
#ifdef orUtilRef_STAGE; #include "orUtilRef"; #endif;
#ifdef orUtilStr_STAGE; #include "orUtilStr"; #endif;
#ifdef orUtilUI_STAGE; #include "orUtilUi"; #endif;
#ifdef orVagueQuantity_STAGE; #include "orVagueQuantity"; #endif;

#include "#orExtensionRegistryStub"; !--pull in personal extenstions, if defined.  If not, this will reference the blank file in this folder.