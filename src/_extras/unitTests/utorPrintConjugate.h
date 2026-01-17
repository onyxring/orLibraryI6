!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorPrintConjugate
! Unit Tests for the utorPrintConjugate orLibrary extension.
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
default        utorPrintConjugate_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orPrintConjugate";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utorPrintConjugate_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorPrintConjugate_STAGE  < LIBRARY_STAGE);
   #undef      utorPrintConjugate_STAGE  ;
   Constant    utorPrintConjugate_STAGE  LIBRARY_STAGE;
   #ifdef      utorPrintConjugate_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorPrintConjugate...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utorPrintConjugateStart "orPrintConjugate Room" has light with description "Where orPrintConjugate tests are run.";
   object -> trolls "trolls" has animate pluralname;
   object -> troll "troll" has animate;
   object -> tool "tool";
   object -> don "Donald" has proper;

orUnitTest "utorPrintConjugate" 
   with tests [; PlayerTo(utorPrintConjugateStart, 3); ] noTest
      [;selfobj.narrative_tense=PRESENT_TENSE; noun=troll; orPrint("Every breath $the:noun(take), every move $the:noun(make), $iAm:player watching $meYouItThem:noun."); ] "Every breath the troll takes, every move the troll makes, you are watching him."
      [;selfobj.narrative_tense=PAST_TENSE; noun=trolls; orPrint("Every breath $the:noun(take), every move $the:noun(make), $iAm:player watching $meYouItThem:noun."); ] "Every breath the trolls took, every move the trolls made, you were watching them."
      [;selfobj.narrative_tense=PAST_TENSE;noun=tool; orPrint("$That:noun(would fix) nothing."); ] "That tool would have fixed nothing."
      [;selfobj.narrative_tense=PAST_TENSE;noun=tool; orPrint("$The:noun(should not go) to the store."); ] "The tool should not have gone to the store."
      [;selfobj.narrative_tense=PRESENT_TENSE; actor=troll; orPrint("$The:actor(couldn't swing) the axe."); ] "The troll couldn't swing the axe."
      [;selfobj.narrative_tense=PAST_TENSE; actor=don; orPrint("$The:actor(couldn't fly) over the fence."); ] "Donald couldn't have flown over the fence."
      [;selfobj.narrative_tense=PAST_TENSE; noun=troll; orPrint("$The:noun(would flork:forkus:florked:florkon) to the store."); ] "The troll would have florkon to the store."
      [;selfobj.narrative_tense=PRESENT_TENSE; noun=troll; second=don;  ThatOrThose(noun); print "->"; orPrint("$noun:that");] "->him" 
      [; noun=troll; ItOrThem(noun); print "->"; orPrint("$noun:me");] "->him"
      [; noun=troll; CSubjectVerb(noun,true, true, "fly",0, "flies","flew"); print "->"; orPrint("$the:noun(fly)");] "->the troll flies"
      [; CSubjectVerb(player,true, false, "fly",0, "flies","flew"); print "->"; orPrint("$I:player(fly)");] "->You fly"
      [; noun=troll; CTheyreOrThats(noun); print "->"; orPrint("$Im:noun");] "->He's"
      [; noun=troll; IsOrAre(noun); print "->"; orPrint("$noun:iAm");] "->is"
      [; noun=troll; CSubjectIs(noun); print "->"; orPrint("$TheIs:noun");] "->The troll is"
      [; noun=troll; CSubjectIsnt(noun); print "->"; orPrint("$TheIsnt:noun");] "->The troll isn't"
      [; noun=troll; CSubjectHas(noun); print "->"; orPrint("$The:noun(have)");] "->The troll has"
      [; noun=troll; CSubjectWill(noun); print "->"; orPrint("$The:noun(will)");] "->The troll will"
      [; noun=troll; CSubjectCan(noun); print "->"; orPrint("$The:noun(can)");] "->The troll can"
      [; noun=troll; CSubjectCant(noun); print "->"; orPrint("$The:noun(can't)");] "->The troll can't"
      [; noun=troll; CSubjectDont(noun); print "->"; orPrint("$The:noun(don't)");] "->The troll doesn't"
! !$XXXX "don't" "doesn't" "didn't" "not done"
      [; noun=troll; OnesSelf(noun); print "->"; orPrint("$self:noun");] "->himself"
      [; noun=troll; Possessive(noun); print "->"; orPrint("$noun:my");] "->his"
      [; noun=troll; PossessiveCaps(noun); print "->"; orPrint("$noun:My");] "->His"
      [; noun=troll; TheActor(actor); print "->"; orPrint("$i:actor");] "you->you"
      
      [; noun=troll; Tense("fly", "flew"); print "->"; orPrint("$tense(fly:flew)");] "?"
      
      [;selfobj.narrative_tense=PRESENT_TENSE;] noTest !cleanup
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
