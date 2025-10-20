!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.28 orDialogue
! This class provides the basis for ASK/TELL Topic-based conversations
! including learnable conversations as well as scripted conversations, and
! NPC conversations which do not repeat.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
!
!--------------------------------------------------------------------------------------
! Revision History
! 2001.10.01	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.09.04	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orDialogue_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orStylePrintRules";
#include "orUtilArray";
#include "_orTopic";
#include "_orHookParser";
#include "_orInfExt";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orDialogue_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orDialogue_STAGE  < LIBRARY_STAGE);
   #undef      orDialogue_STAGE  ;
   Constant    orDialogue_STAGE  LIBRARY_STAGE;
   #ifdef      orDialogue_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orDialogue..."; #endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	global playerInteractingWith;

	attribute learnable;
	property individual learnTopic;
	property individual interactingWith;
	property individual iDunno;
	property individual ext_topicAsk;
	property individual ext_topicAsked;
	property individual ext_topicTell;
	property individual ext_topicTold;
#endif;
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);
	orInfExt with ext_forceLookahead[; if(action_to_be==##sayTopic or ##unrecognizedTopicWords) rtrue; ] !--let's force look ahead resolution for the sayTopic and unrecognizedTopicWordsSub verbs.  This enables the chooseObject routine on the orTopic object to consider the subject when determining if it is in context.
	,	ext_beforeparsing[; consult_from=0;] !--I suspect it is a bug in †he standard library that consult_from doesnt get reset each turn, but we'll just fix it here.
	,	targetNpc 0	!we'll later use this to overwrite what the parser has determined is the second
	,	ext_unknownverb[;  !treat, "person, hello" as "say hello to person"
			if(actor ~= player) { 	!--the parser has determined this is a command to an NPC, but doesn't recognize the verb...
								!--we assume this is a collection of non-verb words that the player is attempting to say...
				self.targetNpc = actor; !--So let's undo the assumptions the parser has made; saving actor off to later be placed into the second variable
				actor = player; !--since this is not a command, we restore the acting character as the player
				verb_wordnum--; !--since we're not interpreting a word as a different verb, but inserting an implied verb, we back up the word number pointer to the first word considered for the remaining parser activities
				return 'say'; !-- return say
			}
		]
		!--We have multiple possible topics.  Normally, we let the parser takes care of this, but in the case of say verbs, the conversee isn't specified until after the topic (say hello to bob)
		!--this causes problems when determining which topics are in context for the conversee, because we don't know who s/he is.  We handle this problem here...
	,	ext_adjudicate[
			npc i j obj top matchedTopic;
		!--this logic only applies to grammars where the noun follows the topic, currently only "say, topic to character" forms.
		if(action_to_be~=##sayTopic or ##vagueSayTopic) rfalse;

		!--The parser will build a list of known topics, which match the names specified; however, two or more topics may have the
		!  same names, but be in context for different NPCs.  Therefore, we need to limit our list of topics down
		!  to those which are in context for the NPC we are talking to.

		!--First we try to determine who we are talking to...
		npc=self.targetNpc; !--if this is the result of a say command veiled as an order, then we know who we are talking to. So let's use that.
		if(npc==0 && advance_warning~=-1) npc=advance_warning; !--if we specified the npc after the topic, then it should be in the look ahead variable (advance_warning)
		if(npc==0) npc=ResolveActorTalkingTo(true); !--if undefined, lets assume who we were last talking to (making no alternative decisions if that doesn't work)...

		!--There are two ways to whittle the list of options down...
		if(npc) {  !--1. we know who we are talking to, so pick the first option in our list that works for them...
			!the logic here is simple: iterate through all the collected possible topics and choose the first one which is in context for our conversee...
			for(i=0: i<number_matched: i++) {
				top = match_list-->i;
				if(top == npc) return top; !--not a topic at all, but we are disambiguating the NPC we are talking to, so return it.
				matchedTopic=self.matchTopic(top, npc);
				if(matchedTopic) return matchedTopic;
				!otherwise loop
			}
		}
		else{ !--2. we don't know who we are speaking to, so pick the first option in our list that works for anyone nearby...

			for(i=0: i<number_matched: i++) {
				top = match_list-->i;

				objectloop(obj near actor){
					if(obj==actor) continue; !--skip over ourselves

					if(obj has animate or talkable) { !--object is an npc, so test this
						matchedTopic=self.matchTopic(top, obj);
						if(matchedTopic) {
							match_scores-->i = SCORE__IFGOOD;
							return matchedTopic;
						}
					}

					! Let's include characters who are added to the scope, but not actually present.  Currently we do NOT resolve thos which have been added via an add_to_scope routine, only property lists...
					if(obj provides add_to_scope){
						for(j=0:j<util.orArray.getLength(obj,add_to_scope):j++){
							if(util.orRef.isObject())
								matchedTopic=self.matchTopic(top, util.orArray.get(obj,add_to_scope,j));

							if(matchedTopic) {
								match_scores-->i = SCORE__IFGOOD;
								return matchedTopic;
							}
						}
					}
				}
			}
		}
		return false; !if none match, then we will return false to continue with the default parser behavior.
	]
	,	matchTopic[top npc !return the first topic in our list of options which matches the given npc
				j;
			if(npc hasnt animate or talkable) rfalse;
			if((top ofclass orTopic)==false) rfalse;
			if(top.isInContextFor(npc)){
				for(j=0: j<number_matched: j++) match_scores-->j = 0;
				number_matched=1;
				match_list-->0 = top; !--we will only return one topic, the first one which is in context for the NPC.
				dont_infer=true; !--we don't want to infer the topic, because we already know it is in context for the NPC.
				self.targetNpc=npc;
				return top;!--this topic is in context for the npc, so we will use it.
			}
			rfalse;
		]
	,	ext_playLoopRewriteParsedCommands[;
			if(self.targetNpc) { !--if we previously detected "NPC, topic", lets actually rewrite it for the parser into: SAY topic TO npc
				if(consult_from~=0){ !--did we detect words that didn't match a topic?
					action=inputobjs-->0=##unrecognizedTopicWords; !--lets redirect this to our error handler, disguised as a verb.

					!--TODO: the following line looks wrong.  inputobjs-->1 should be set to 2 according to my understanding of the parser, as it is in the subsequent code block...  comeback and test this.  It seems to work, but it might just be that some of the later code is deducing the npc when it should be explicitly set here.
					inputobjs-->1=1; !--tell the parser that we actually have both noun and second
					inputobjs-->2 = self.targetNpc; !--identify who we are talking to (this is used by both SAY and the unrecognizedTopicWords routines)
				}
				else{
					action=inputobjs-->0=##sayTopic; !--ensure this is SAY, instead of ORDER or whatever else might have been determined by the parser
					inputobjs-->1=2; !--tell the parser that we actually have both noun and second
				}
				inputobjs-->3 = self.targetNpc; !--identify who we are talking to (this is used by both SAY and the unrecognizedTopicWords routines)

				self.targetNpc=0; !zero this out for next loop
				rtrue; !--we did something, so don't do default behavior
			}
			rfalse;
		]
		!--we remove certain characters from the input, because we want to allow players to input them, but we don't want them to mess up parsing...
		,	ext_keyboardPrepInput[a_buf
				t triggerComma;

			!commas are a special case, since we allow for them in orders or conversations like: bob, hello
			if((WordValue(1)->#dict_par1) & DICT_VERB) triggerComma=true; !--if the first word is a verb, and therefore NOT a "bob, hello" command
																			! syntax, then we ignore commas, so phrases like: "say red, blue balloon"
																			! will be treated as "say red blue balloon"

			for(t=2: t<=(a_buf->1)+2: t++) {
				if(a_buf->t==0) break;
				if(a_buf->t==34 or 33 or 63) { !--convert quotes, exclamation points and question marks to spaces so we ignore them
					a_buf->t=32;
					triggerComma=true; !--incidentally, if any of these are found, also start removing commas, e.g.: say "hello, how are you?" turns into: say hello how are you
				}
				 if(a_buf->t==44){ !--comma found
				 	if(triggerComma) a_buf->t=32; !--we've determined that any commas to accept have already been accepted, so ignore them going forward
				 	else triggerComma=true; !--otherwise, set it up to replace the next comma
				 }
			}
		]
	;

	!the player has specified a topic, but not who to ask/tell/say it to.  We try to deduce who the player is speaking with to scope in possible topics to match
	[topicInVagueTarget top
			npc; !isolate information known by the target
		npc=ResolveActorTalkingTo();
		switch(scope_stage){
			1: return false;
			2: if(npc>0){
					objectloop(top ofclass orTopic && top.isKnownBy(npc)){
						PlaceInScope(top);
					}
				}
				rtrue;
			3: 	if(npc>0){
					if(npc provides iDunno)
					 	printorrun(npc,iDunno);
					else
						print (ig)CIVerb(npc,"did","do","does")," not appear to know the answer to that.^";
				}
				else print "I'm not sure who you want to talk to.^";
		}
	];
	[topicKnownByTarget  !isolate information known by the target of the conversation
			top npc;
		npc=inputobjs-->2;
		switch(scope_stage){
			1: rfalse;
			2: objectloop(top ofclass orTopic && top.isKnownBy(npc))
					PlaceInScope(top);
				rtrue;
			3: 	if(npc provides iDunno)
					printorrun(npc,iDunno);
				else
					print (ig)CIVerb(npc,"did","do","does")," not appear to know the answer to that.^";
		}
	];

	[topicKnownByActor  !isolate information known by the actor
			top npc;
		switch(scope_stage){
			1: 	rfalse;
			2: 	npc=inputobjs-->2;
				if(action_to_be==##sayTopic or ##vagueSayTopic)
					objectloop(top ofclass orTopic  && top.isTellable()==true  && top.isKnownBy(actor)) PlaceInScope(top); !--place all topics in scope for say; we'll handle this in adjudicate
				else
					objectloop(top ofclass orTopic  && top.isTellable()==true  && top.isKnownBy(actor) && top.isInContextFor(npc) ) PlaceInScope(top); !--if we already know who we are talking to, then lets ensure context now...

				etype=0;
				rtrue;
			3:  "I'm not sure what you want to say.";
		}
	];

	[askTopicSub
			topic talkingTo s;

		talkingTo=noun;
		topic=second;

		if(talkingTo==0 && topic == 0)	 {
			print "[To ask something of someone, use: ASK ";
			style underline;
			print "character ";
			style roman;
			print "ABOUT ";
			style underline;
			print "topic";
			style roman;
			print ".]^";
			meta=true;
			rtrue;
		};

		if (RunLife(actor,##askTopic)~=0) return;

		if(second==0 || (second provides quip)==false || second.quip==0) {
			print "You'll need to be more specific. What do you want to ask?^";
			!L__M(##Tell, 2); !TODO, is this correct?
 			return;
 		}
		if(noun==actor) {
			L__M(##Tell, 1);
			rtrue;
		}
		talkingTo=noun;
		topic=second;
		if(LibraryExtensions.RunUntil(ext_topicAsk, true, topic, actor, talkingTo)==true) return;

		if(actor provides interactingWith && metaclass(actor.interactingWith~=routine)) actor.interactingWith=talkingTo;

		if(actor==player) playerInteractingWith=talkingTo;

		!Ask
		if(util.orArray.get(topic,quip,1)~=-1){ !--if ask is different than tell
			s=util.orArray.get(topic,quip,0);
			if(util.orRef.isRoutine(s))
				s();
			else
				print (string)s;
			print "^^";
		}

		if(LibraryExtensions.RunUntil(ext_topicAsked, true, topic, actor, talkingTo)==true) return;

		<tellTopic actor topic,noun>;  !ask/tell are wrapped together.  Let the tellTopic sub handle both.

		rtrue;
	];

	[talkSub; print_ret "You fumble for words but find nothing to say."; ];
	[vagueTalkSub; print_ret "You mumble a few words to no one in particular."; ];

	[tellTopicSub
			topic talkingTo s;

 		talkingTo=noun;
		topic=second;

 		if(talkingTo==0 && topic == 0)	 {
			print "[To tell something to someone, use: TELL ";
			style underline;
			print "topic ";
			style roman;
			print "TO ";
			style underline;
			print "character";
			style roman;
			print ".]^";
			meta=true;
			rtrue;
		};

		if (RunLife(actor,##tellTopic)~=0) return;

		if(second==0 || (second provides quip)==false || second.quip==0) {
			print "You'll need to be more specific. What do you want to say?^";
			!L__M(##Tell, 2); !TODO, is this correct?
 			return;
 		}
		talkingTo=noun;
		topic=second;
		if(topic.isInContextFor(talkingTo)==false) {
			if(talkingTo provides iDunno)
				printorrun(talkingTo,iDunno);
			else {
				print (The)talkingTo," ";
				if(talkingTo has pluralname)
					print "don't";
				else
					print "doesn't";
				print " respond to that.^";
			}
			return;
		}

		if(LibraryExtensions.RunUntil(ext_topicTell, true, topic, actor, talkingTo)==true) return;
		if(actor provides interactingWith && metaclass(actor.interactingWith~=routine)) actor.interactingWith=talkingTo;
		if(util.orArray.getSize(second,quip)>1)
			s=util.orArray.get(topic,quip,1);
		else
			s=util.orArray.get(topic,quip,0);

		if(util.orRef.isRoutine(s))
			s();
		else
			print (string)s,"^";

		topic.recordTell(actor, talkingTo);
		if(topic has learnable && talkingTo provides learnTopic)
			talkingTo.learnTopic(topic);

		LibraryExtensions.RunAll(ext_topicTold, topic, actor, talkingTo);

	];

	[sayTopicSub
		    topic talkingTo;

 		talkingTo=second;
		topic=noun;

		if(talkingTo==0 && topic == 0)	 {
			print "[To say something to someone, use: SAY ";
			style underline;
			print "topic ";
			style roman;
			print "TO ";
			style underline;
			print "character";
			style roman;
			print ".]^";
			meta=true;
			rtrue;
		};

        if(topic.isInContextFor(talkingTo)==false) {
			if(talkingTo provides iDunno)
				printorrun(talkingTo,iDunno);
			else {
				print (The)talkingTo," ";
				if(talkingTo has pluralname)
					print "don't";
				else
					print "doesn't";
				print " respond to that.^";
			}
			return;
		}

        <tellTopic talkingTo topic>;
	];

	[resolveActorTalkingTo currentOnly
			obj o;
		if(actor==player && playerInteractingWith~=0) obj=playerInteractingWith;
		if(obj==0 && actor provides interactingWith) obj=valueorrun(actor,interactingWith);
		if(obj>0 && ScopeCeiling(obj)==ScopeCeiling(actor)) return obj; !--we're already talking to someone, lets assume them.

		!--we aren't having a conversation with someone already, so lets make an arbitrary decision on who we should talk to...
		if(currentOnly==false){
			objectloop(o in  location && o~=actor){ !-- o in location probably isnt' right
				if(o~=0 && o has animate or talkable) return o; !return first possible
			}
		}

		return 0;
	];
	[vagueAskTopicSub
			talkingTo;
		talkingTo=vagueError(ResolveActorTalkingTo());

		if(talkingTo){
			second=noun;
			noun=talkingTo;
			print "(Asking ",(the)talkingTo,")^";
			<<askTopic noun second>>;
		}
	];
	[vagueTellTopicSub
			talkingTo;
		talkingTo=vagueError(ResolveActorTalkingTo());

		if(talkingTo){
			second=noun;
			noun=talkingTo;
			print "(Telling ",(the)talkingTo,")^";
			<<tellTopic noun second>>;
		}
	];

	[vagueSayTopicSub
            talkingTo;

		talkingTo=vagueError(ResolveActorTalkingTo());

		if(talkingTo){
			print "(Speaking with ",(the)talkingTo,")^";
			<<sayTopic noun talkingTo>>;
		}
	];
	[vagueError res;
		if(res==0) print "There ",(ig)ppf("was","is","will be")," nothing to speak with.^";
		if(res<0) {
			print "You'll need to be more specific. Who ",(italics)"exactly"," did you want to speak with?^";
			res=0;
		}
		return res;
	];

	[unrecognizedTopicWordsSub q;
		wn=consult_from;
		q=NextWord();
		if(q=='who' or 'what' or 'when' or 'where' or 'why' or 'how') {
			print "[It looks like you are trying to ask ";
			if(noun) print(the)noun," ";
			print "a question. Try ASK ",(italics)"character"," ABOUT ", (italics)"topic", " instead.]^";
			meta=true;
			rtrue;
		}

		if(noun==0) "I'm not sure who you want to talk to.";
		if(noun==actor && actor==player) "You talk to yourself but don't answer.";

		if(noun provides iDunno)
			printorrun(noun,iDunno);
		else
			"That elicits no response from ",(the)noun,".";
	];

	!--we use this because 's' is used as a short form for 'say', but it conflicts with the standard library's use of 's' as a short form for 'south'
	[GoSouthSub;
		<<go s_obj>>;
	];
	!--this is a convenience method, usable in before routines to determine if the current action is a dialogue action without having to specify all the possible verbs
	[isDialogueAction act;
		if(act==##askTopic or ##tellTopic or ##sayTopic or ##talk or ##vagueTalk or ##vagueAskTopic or ##vagueTellTopic or ##unrecognizedTopicWords or ##tell  or ##ask) return true;
		return false;
	];

	extend 'ask' replace
		* 											-> askTopic !provides syntax for "say topic to character"
		* talkable 'about' scope=topicKnownByTarget -> askTopic
		* talkable scope=topicKnownByTarget			-> askTopic
		* creature 'about' scope=topicKnownByTarget -> askTopic
		* creature scope=topicKnownByTarget			-> askTopic
		* 'about' scope=topicInVagueTarget			-> vagueAskTopic
		* scope=topicInVagueTarget					-> vagueAskTopic
		* creature topic  							-> unrecognizedTopicWords
		* talkable topic  							-> unrecognizedTopicWords
		* topic 			 						-> unrecognizedTopicWords
	;

	verb 'a' ='ask';

	Extend 'tell' replace
		* 											-> tellTopic !provides syntax  to player
		* talkable 'about' scope=topicKnownByActor 	-> tellTopic
		* talkable scope=topicKnownByActor			-> tellTopic
		* creature 'about' scope=topicKnownByActor 	-> tellTopic
		* creature scope=topicKnownByActor			-> tellTopic
		* 'about' scope=topicKnownByActor 			-> vagueTellTopic
		* scope=topicKnownByActor 'to' talkable 	-> sayTopic
		* scope=topicKnownByActor 'to' animate  	-> sayTopic
		* scope=topicKnownByActor 					-> vagueSayTopic
		* creature topic 							-> unrecognizedTopicWords
		* talkable topic 							-> unrecognizedTopicWords
		* topic 			 						-> unrecognizedTopicWords
	;
	Extend only 'speak' replace
		* 											-> vagueTalk
		* 'to'/'with' talkable 						->Talk
		* 'to'/'with' creature 						->Talk
		* talkable 									->Talk
		* creature 									->Talk
	;
	Extend only 'shout' replace
		* 											-> vagueTalk
		* 'to'/'at' talkable 						->Talk
		* 'to'/'at' creature 						->Talk
		* talkable 									->Talk
		* creature 									->Talk
	;
	verb 'yell'
		* 											-> vagueTalk
		* 'to'/'at' talkable 						->Talk
		* 'to'/'at' creature 						->Talk
		* talkable 									->Talk
		* creature 									->Talk
	;
	Verb 'talk'
		* 											-> vagueTalk
		* 'to'/'with' talkable 						->Talk
		* 'to'/'with' creature 						->Talk
		* talkable 									->Talk
		* creature 									->Talk
	;
	Verb 't'
		* 											-> vagueTalk
		* 'to' creature 							->Talk
		* 'to' talkable 							->Talk
		* creature 									->Talk
		* talkable 									->Talk
		* creature 'about' scope=topicKnownByActor 	-> tellTopic
		* creature scope=topicKnownByActor			-> tellTopic
		* talkable 'about' scope=topicKnownByActor 	-> tellTopic
		* talkable scope=topicKnownByActor			-> tellTopic
		* scope=topicKnownByActor 					-> vagueTellTopic
		* 'about' scope=topicKnownByActor 			-> vagueTellTopic
		* scope=topicKnownByActor 'to' talkable 	-> sayTopic
		* scope=topicKnownByActor 'to' animate  	-> sayTopic
		* scope=topicKnownByActor 					-> vagueSayTopic
		* creature topic 							-> unrecognizedTopicWords
		* talkable topic 							-> unrecognizedTopicWords
		* topic 			 						-> unrecognizedTopicWords
	;
    extend 'say' replace
	 	* 											-> sayTopic !provides syntax for "say topic to character"
		* scope=topicKnownByActor 					-> vagueSayTopic
		* scope=topicKnownByActor 'to'/'in'/'into'/'at' talkable  	-> sayTopic
		* scope=topicKnownByActor 'to'/'at' creature 	-> sayTopic
		* topic 'to'/'in'/'into'/'at' talkable 					-> unrecognizedTopicWords
		* topic 'to'/'at' creature 						-> unrecognizedTopicWords
		* topic 			 						-> unrecognizedTopicWords
	 ;

	verb 's'
	 	*  											-> GoSouth
		* scope=topicKnownByActor 					-> vagueSayTopic
		* scope=topicKnownByActor 'to'/'in'/'into'/'at' talkable  	-> sayTopic
		* scope=topicKnownByActor 'to'/'at' creature 	-> sayTopic
		* topic 'to'/'in'/'into'/'at' talkable 					-> unrecognizedTopicWords
		* topic 'to'/'at' creature 						-> unrecognizedTopicWords
		* topic 			 						-> unrecognizedTopicWords
	;


	! Extend 'answer' first
	! 	* 'to' creature  scope=topicKnownByActor 	-> AnswerTopic reverse
	! 	* 'to' creature  topic					-> Answer reverse
	! 	* scope=topicKnownByActor 'to' creature		-> AnswerTopic
	! ;
#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
