!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! YYYY.MM.DD utorDialogue
! Unit Tests for the utorDialogue orLibrary extension.
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
default        utorDialogue_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orDialogue";

!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utorDialogue_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorDialogue_STAGE  < LIBRARY_STAGE);
   #undef      utorDialogue_STAGE  ;
   Constant    utorDialogue_STAGE  LIBRARY_STAGE;
   #ifdef      utorDialogue_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorDialogue...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utorDialogueStart "orDialogue Room" has light with description "Where orDialogue tests are run.";
   
      object -> magicMirror "magic mirror" has talkable 
	      with name 'magic' 'mirror', 
            description[; if(tBeauty.hasBeenTold())  "Etched in the 	glass is a taunting shape, seemingly a mustache.";
            "It reflects back the truth. Verbally."; 
         ];

      object -> servant "servant" has animate with name 'servant', 
         description "Waiting quietly to attend to your needs",
         orders[;
               AskTopic: <AskTopic noun second, self>;
         ];
      
      orTopic tBestNumber with knownBy magicMirror
      ,  name 'best' 'number'
      ,  dissemTrack 0 0
      ,  quip "~What's the best number?~"
         [teller talkingTo;
            if(self.hasBeenToldTo(talkingTo)){
            print "~I told you already...~ the mirror says.";
               }
            "~Two is the best. It's the only prime number that is even.~";
         ];

   orTopic tMom with knownBy selfObj, name 'mom',
      context magicMirror,
      quip "~You know,~ you say to... That's YOUR mom.~";
   
   orTopic tMoon with name 'moon' 'luna',
      knownBy orDIA_ALL,
      quip "~Luna's magic is subtle, but powerful.~";

   orTopic topicsKnownByMirror with knownBy magicMirror;
      orTopic -> tBeauty with name 'handsome' 'beauty',
         quip "~Mirror, Mirror, in my hand, who's most handsome in this land?~" 
            "~Your mom,~ the mirror replies. ~Seriously, her mustache is quite remarkable.~";

      orTopic -> tPoisonBananas  with name 'monkey' 'problem',
         quip "~How can I get rid of this little monkey problem I have? ...~";


orUnitTest "utorDialogue" 
   with tests [; PlayerTo(utorDialogueStart, 3); ] noTest
               "x mirror" "reflects back the truth"
               "ask mirror about beauty" "her mustache is quite remarkable"   
               "x mirror" "is a taunting shape"
               "tell mirror about mom" "That's YOUR mom"   
               "tell mirror about beauty" "no response"   
               "ask mirror about mom" "no response"   
               "ask mirror about number" "????"   
   ,  tests1   "ask mirror about number" "????"   
               "servant, ask mirror about number" "????"   
               "servant, ask mirror about number" "????"   
               "a beauty" "her mustache is quite remarkable"   
               "t mom" "YOUR mom"   
               "say mom to mirror" "YOUR mom"   
               "s mom" "YOUR mom"   
               "mirror, mom" "YOUR mom"
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
