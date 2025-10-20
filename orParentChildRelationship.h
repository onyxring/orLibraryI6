!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.05.14 orParentChildRelationship
! Reworks the container and supporter functionality of the standard library, allowing
! objects to be BOTH supporters and containers simultaneously.  It also provides
! hooks to allow new relations to be defined.  Potentially, "under", "behind",
! or "near", for example.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! TODO: Need to harden this!
!--------------------------------------------------------------------------------------
! Revision History
! 2001.10.06	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.05.02	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orParentChildRelationship_STAGE  0;
!======================================================================================
! INCLUDE DEPENDENCIES
#include "_orHookWriteAfterEntry";
#include "_orHookVerbs";
#include "orString";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orParentChildRelationship_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orParentChildRelationship_STAGE  < LIBRARY_STAGE);
   #undef      orParentChildRelationship_STAGE  ;
   Constant    orParentChildRelationship_STAGE  LIBRARY_STAGE;
   #ifdef      orParentChildRelationship_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orParentChildRelationship...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	attribute contained;
	attribute supported;

	replace RemoveSub _oldRemoveSub;
	replace InsertSub;
	replace DropSub _oldDropSub;
	replace PutOnSub;
	replace EnterSub _oldEnterSub;
	replace ExitSub _oldExitSub;
	replace VisibleContents;
	replace OpenSub _oldOpenSub;
	replace AttemptToTakeObject;
	replace SearchSub;
	replace SayWhatsOn;
	replace ObjectIsUntouchable;
	replace ScopeWithin;
#endif; !--Before Parser

#iftrue (LIBRARY_STAGE == AFTER_PARSER);
	object _pcr_libEx LibraryExtensions
		!To be a little more flexible in practices established with the standard library's supporter/container approach,
		!lets assume that children, which do NOT have the childAttribute (e.g., supported, contained) but are in
		!parents which provide relationships, actually do have a relationship, even though it wasn't declared.  Prefer supporter, then container, followed by the first found relationship, in that order.
		with lm_r 0,
		! ext_initialise[o rcount c;
		! 	objectloop(o==o && (rcount=numPcrsProvidedBy(o))>0 ){ !for all containers in the game...
		! 		objectloop(c in o && getPcrFromChild(c) == 0){ !if they have children that are missing the appropriate relationship attribute
		! 			#ifdef DEBUG;
		! 			print "WARNING: object ", (name)c," has a relationship-providing parent (",(name)o,") but does not have the appropriate attribute.  Attempting to resolve...^";
		! 			#endif;
		! 			if(o has supporter)
		! 				give c supported;
		! 			else if (o has container)
		! 				give c contained;
		! 			else
		! 				getFirstPcrProvidedBy(o).applyTo(c); !if the parent provides only one relationship, then provide that
		! 		}
		! 	}
		! ],
		!a modification to a piece of the WriteAfterEntry routine
		ext_sdRecurseBit[o;
			if (c_style & RECURSE_BIT ~= 0 && _WAE_child_count>0){
				annotateContents(o);
				rtrue;
			}
			rfalse;
		],
		ext_examineSub[displayState didOutput;
			orSuppressNewlineAfterDescription = false;
			if (location == thedark) return L__M(##Examine, 1, noun); !too dark to see
			displayState=true; ! by default, display contents of relationship providing objects, such as containers and supporters
			if(noun.description~=0) { ! if there is  a description, then print it
				if(LibraryExtensions.RunWhile(ext_preDescription,false)~=false) return;
				displayState = PrintOrRun(noun, description,true); !return false to suppress displaying additional state (such as content or swichable)
				didOutput=true;
				LibraryExtensions.RunAll(ext_postDescription);
				if(orSuppressNewlineAfterDescription == false) new_line; !do the newline feed that normally goes after the description, here.
			}
			if (displayState > 0) {
				if(describeContents(noun, true)) didOutput=true;
				if (noun has switchable) {
					didOutput=true;
					L__M(##Examine, 3, noun);
				}
			}
			if(AfterRoutines()) didOutput=true;
			!--after we did all the checking, if we didn't print anything, inform the player there's nothing to see here and move along please...l
			if(didOutput==0) L__M(##Examine, 2, noun); !nothing special
			return true;
		],
		ext_messages[n x1 x2
				r p;
			x1=lm_o; x2=lm_s; n=lm_n;
			Look:	switch(n){
						4: print "^On ", (the) x1;
							WriteListFrom(child(x1),
								ENGLISH_BIT+RECURSE_BIT+PARTINV_BIT+TERSE_BIT+CONCEAL_BIT+ISARE_BIT + WORKFLAG_BIT);
							".";
					}
			GenericInsert: switch(n){
						1:  CSubjectVerb(actor,true,false,"need",0,"needs","needed");
							print " to be holding ", (the) x1, " before ", (theActor) actor;
							Tense(" can", " could");
							" put ", (ItOrThem) x1, " ",(nop) self.lm_r.sayPrep()," something else.";
						2:  CSubjectCant(actor,true,false); " put something ",(nop)self.lm_r.sayPrep()," itself.";
						3:  CSubjectCant(actor,true,false); " put things ",(nop)self.lm_r.sayPrep()," ",(the)x2,".";
						4:	print "There ";
								Tense("is", "was");
				            " no more room ", (nop)self.lm_r.sayPrep()," ",(the)x2, ".";
						5:  "Done.";
        				6:  CSubjectVerb(actor,false,false,"put",0,"puts","put"); " ", (the) x1, " ",(nop)self.lm_r.sayPrep()," ", (the)x2, ".";
					}

			Take: switch(n){
						4:  CSubjectWill(actor,true);
							print " have ";
							Tense("", "had ");
							"to get ",  (nop) inferPcr(x1,x2).sayFromPrep(), " ", (the) x1, " first.";
			}
			Miscellany: switch(n){
							58: p=inferPcr(x1,x2);
								if(p==0) return L__M(##Miscellany, 26, x1);
								print "(first taking ", (the) x1," ", (nop) p.sayFromPrep(), " ", (the) x2, ")";
					}
			Enter: switch(n){
						1:  print "But "; CSubjectIs(actor,true,true);
            				" already ", (nop) inferPcr(x1,x2).sayPrep(), (the) x1, ".";
						5:  CSubjectVerb(actor,false,false,"get",0,"gets","got");
									inferPcr(x1,x2).sayToPrep();
									 " ", (the) x1, ".";
						6:  "(getting ", (nop) inferPcr(x1,x2).sayFromPrep(), " ", (the) x1, ")";
						7:  r=inferPcr(x1,x2);
							if(r==0) "(entering ", (the) x1, ")";
							"(getting ",r.sayToPrep()," ", (the) x1, ")";
					}
			Exit: switch(n){
						3:  CSubjectVerb(actor,false,false,"get",0,"gets", "got");
							print " ";
							inferPcr(x1,x2).sayFromPrep(); " ", (the) x1, ".";
						4:  CSubjectIsnt(actor,true);
							print " ";
							print (nop)inferPcr(x1,x2).sayPrep(), " ", (the) x1, ".";
						5:  "(first getting ", (nop) inferPcr(x1,x2).sayFromPrep(),
							" ", (the) x1, ")";
					}
			Go: switch(n){
					  	1:	CSubjectWill(actor,true);
							Tense(" have", " have had");
							" to get ", (nop) inferPcr(x1,x2).sayFromPrep(), " ", (the) x1, " first.";
					}
			! Search: switch(n){
			! 			3: 	print "On ", (the) x1;
			! 				WriteListFrom(child(x1),
			! 					TERSE_BIT + WORKFLAG_BIT + ENGLISH_BIT + ISARE_BIT + CONCEAL_BIT + PARTINV_BIT + RECURSE_BIT );
			! 				print ".";
			! 				rtrue;
			! 			7: 	print "In ", (the) x1;
			! 				WriteListFrom(child(x1),
			! 					TERSE_BIT + WORKFLAG_BIT + ENGLISH_BIT + ISARE_BIT + CONCEAL_BIT + PARTINV_BIT + RECURSE_BIT );
			! 				print ".";
			! 				rtrue;
			! 			8: 	print "supporting ";
			! 				WriteListFrom(child(x1),
			! 					TERSE_BIT + WORKFLAG_BIT + ENGLISH_BIT + CONCEAL_BIT + PARTINV_BIT + RECURSE_BIT );
			! 				rtrue;
			! 			9: 	print "containing ";
			! 				WriteListFrom(child(x1),
			! 					TERSE_BIT + WORKFLAG_BIT + ENGLISH_BIT + CONCEAL_BIT + PARTINV_BIT + RECURSE_BIT );
			! 				rtrue;

			! 		}
			! Open:	if(n==4){
			! 			print (ig)CIVerb(actor,"opened","open"), " ",(the) x1, ", revealing ";
			! 			SetContainedChildrenWorkFlag(x1,true);
			! 			if(WriteListFrom(child(x1),
			! 		      ENGLISH_BIT + WORKFLAG_BIT + TERSE_BIT + CONCEAL_BIT)==0) "nothing.";
	        !           ".";
			! 		}

		];
#endif; !-after parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

!TODO: Replace all the text with L__M messages
!TODO: Ensure the child cannot have both contained and supported attributes at the same time
!TODO: SearchScope seems like its something which should be addressed too.

	! the class for defining relationships between a parent and child object
	class 	orParentChildRelationship
		with parentAttribute 0
		,	childAttribute 0
		,	preposition 0
		,	toPreposition 0
		,	fromPreposition 0
		,	includeContentsInExamine true
		,	sayPrep[; print (string)self.preposition; ]
		,	sayToPrep[;
				if(self.toPreposition==0)  print (string)self.sayPrep();
				print (string)self.toPreposition;
			]
		,	sayFromPrep[;
				if(self.fromPreposition==0)  print "from ",(string)self.sayPrep();
				print (string)self.fromPreposition;
			]
		,	isAppliedTo [c;
				if(isMissingPcr(c) && numPcrsProvidedBy(parent(c))==1 && self.isProvidedBy(parent(c))) return true;
				return c has self.childAttribute;
			]
		,	applyTo[c r;
				r=getPcrFromChild(c);
				if(r~=0 && r~=self) r.removeFrom(c);
				give c self.childAttribute;
		]
		,	prepareToApply[c p keepSilent; c=p=keepSilent; rtrue; ]
		,	isProvidedBy [o; if(o==0) return false; return o has self.parentAttribute; ]
		,	numVisibleChildrenWith[p c count;
				objectloop(c in p && c hasnt concealed or scenery && self.isAppliedTo(c)==true) count++;
				return count;
			]
		,	listChildrenWithPcr[p c;
				objectloop(c in p){
					if(self.isAppliedTo(c)==true)
						give c workflag;
					else
						give c ~workflag;
				}
				WriteListFrom(child(p),TERSE_BIT + WORKFLAG_BIT + ENGLISH_BIT +  CONCEAL_BIT + PARTINV_BIT + RECURSE_BIT + ISARE_BIT);
			],
			describeContentsWith[p;
				if(self.numVisibleChildrenWith(p)==0) rfalse;
				print "^";
				print (cap)self.preposition," ",(the) p;
				self.listChildrenWithPcr(p);
				print ".^";
				rtrue;
			]
		,	annotateContentsWith[p;
				if(self.numVisibleChildrenWith(p)==0) rfalse;
				print (string)self.preposition," ",(nop)util.orStr.trim(WHICH__TX).print();

				self.listChildrenWithPcr(p);
				rtrue;
			]
		,	removeFrom[c; give c ~self.childAttribute;]
	;

	! The two relationships provided by the standard library: container and supporter...
	orParentChildRelationship orPcrContainer
		with parentAttribute container
		,	childAttribute contained
		,	preposition "in"
		,	toPreposition "into"
		,	fromPreposition "out of"
		,	describeContentsWith[p;
				if(self.isProvidedBy(p)==false) return 0;
				if (p hasnt open or transparent) print_ret (The) p, " ", (IsOrAre) p, " closed.";
				if(self.numVisibleChildrenWith(p)==0) print_ret "Inside, ",(the) p, " ", (IsOrAre) p, " empty.";
				return self.orParentChildRelationship::describeContentsWith(p);
			]
		,	numVisibleChildrenWith[p c count;
				objectloop(c in p && c hasnt concealed or scenery && p has open or transparent && self.isAppliedTo(c)==true) count++;
				return count;
			]
		,	prepareToApply[c p keepSilent;
				if (p has container && p hasnt open && ImplicitOpen(c)){
            		if(keepSilent==false) L__M(##Insert, 3, second);
					return false;
				}
				return true;
			]
		,	isAppliedTo [c;
				if(parent(c) == 0) return false;
				if(isMissingPcr(c)==true && parent(c) has container && parent(c) hasnt supporter) return true;
				return self.orParentChildRelationship::isAppliedTo(c);
			];

	orParentChildRelationship orPcrSupporter
		with	parentAttribute supporter
		,		childAttribute supported
		,		includeContentsInExamine false !--default behavior of the standard library
		,		preposition "on"
		,		toPreposition "onto"
		,		fromPreposition "off"
		,		isAppliedTo [c;
					if(parent(c) == 0) return false;
					if(isMissingPcr(c)==true && parent(c) has supporter) return true;
					return self.orParentChildRelationship::isAppliedTo(c);
				];

	!just some syntactical sugar, since this condition is explicitly tested for numerous times in the library
	[isContained c; return orPcrContainer.isAppliedTo(c);];
	[isSupported c; return orPcrSupporter.isAppliedTo(c);];

	[getPcrFromChild c r;
			objectloop(r ofclass orParentChildRelationship && r.isAppliedTo(c)) return r;
			return 0;
	];
	[isMissingPcr c r;
		objectloop(r ofclass orParentChildRelationship && c has r.childAttribute) return false;
		return true;
	];
	[clearAllPcrsFromChild c r;
		objectloop(r ofclass orParentChildRelationship && r.isAppliedTo(c)) r.removeFrom(c);
	];
	!list the contents of a parent object, grouped by their relationships
	[describeContents p hideDesignatedPcrs c r count; c=c;
		objectloop(r ofclass orParentChildRelationship && r.numVisibleChildrenWith(p)>0){
			if(hideDesignatedPcrs==true && r.includeContentsInExamine==false) continue; !--support the default behavior of the standard library
			count = count + r.describeContentsWith(p);
		}
		return count;
	];
	!same, but in the short, parenthetical form used by the standard library when describing an object in the room description
	[annotateContents p
			r count;

		objectloop(r ofclass orParentChildRelationship && r.numVisibleChildrenWith(p)>0){
			if(count > 0)
				print ", ";
			else
				print " (";
			count = count + r.annotateContentsWith(p);
		}
		if(count>0) print ")";
		return count;
	];
	[numPcrsProvidedBy p r count;
		objectloop(r ofclass orParentChildRelationship && r.isProvidedBy(p)) count++;
		return count;
	];
	[getFirstPcrProvidedBy p r;
		objectloop(r ofclass orParentChildRelationship && r.isProvidedBy(p)) return r;
		return 0;
	];

	global _currentPcrContext =0;
	[ VisibleContents p count r;
		if(_currentPcrContext~=0) return _currentPcrContext.numVisibleChildrenWith(p); !--a bit of a hack; allowing us to limit the domain searched to specific types of containment without making deeper changes to the standard library's code
		objectloop(r ofclass orParentChildRelationship && r.isProvidedBy(p)) {
			count = count + r.numVisibleChildrenWith(p);
		}
		return count;
	];


	[ ObjectIsUntouchable item flag1 flag2 ancestor i c;
		! Determine if there's any barrier preventing the actor from moving
		! things to "item".  Return false if no barrier; otherwise print a
		! suitable message and return true.
		! If flag1 is set, do not print any message.
		! If flag2 is set, also apply Take/Remove restrictions.

		! If the item has been added to scope by something, it's first necessary
		! for that something to be touchable.

		ancestor = CommonAncestor(actor, item);
		if (ancestor == 0) {
			ancestor = item;
			while (ancestor && (i = ObjectScopedBySomething(ancestor)) == 0)
				ancestor = parent(ancestor);
			if (i) {
				if (ObjectIsUntouchable(i, flag1, flag2)) return;
				! An item immediately added to scope
			}
		}
		else

		! First, a barrier between the actor and the ancestor.  The actor
		! can only be in a sequence of enterable objects, and only closed
		! containers form a barrier.
		!--orlib: checking to see if the parent is a container is no longer enough; additionally, we must check the immediate child to see if it is actually contained.  If instead, it is supported by a parent that is both a supporter AND a container, then it is not automatically untouchable
		if (actor ~= ancestor) {
			i = parent(actor);
			c=actor; !--orlib: the actor is the child we are checking to see if it is contained
			while (i ~= ancestor) {
				if (i has container && i hasnt open && isContained(c)) { !--orlib: child is actually contained in the container
					if (flag1) rtrue;
					return L__M(##Take, 9, i, noun);
				}
				c=i; !--orlib: the current parent becomes the child for the next loop
				i = parent(i);
			}
		}

		! Second, a barrier between the item and the ancestor.  The item can
		! be carried by someone, part of a piece of machinery, in or on top
		! of something and so on.

		i = parent(item);
		c=item; !--orlib: item is the child we are checking to see if it is contained
		if (item ~= ancestor && i ~= player) {
			while (i ~= ancestor) {
				!if (flag2 && i hasnt container && i hasnt supporter) {
				if (flag2 && numPcrsProvidedBy(i)==0) { !isn't any form of a container, supporter, or customer...
					if (i has animate) {
						if (flag1) rtrue;
						return L__M(##Take, 6, i, noun);
					}
					if (i has transparent) {
						if (flag1) rtrue;
						return L__M(##Take, 7, i, noun);
					}
					if (flag1) rtrue;
					return L__M(##Take, 8, item, noun);
				}
				if (i has container && i hasnt open && isContained(c)) { !--orlib: child is actually contained in the container
					if (flag1) rtrue;
					return L__M(##Take, 9, i, noun);
				}
				c=i; !--orlib: the current parent becomes the child for the next loop
				i = parent(i);
			}
		}
		rfalse;
	];
	[ ScopeWithin domain nosearch context x y;
		if (domain==0) rtrue;
		!  Special rule: the directions (interpreted as the 12 walls of a room) are
		!  always in context.  (So, e.g., "examine north wall" is always legal.)
		!  (Unless we're parsing something like "all", because it would just slow
		!  things down then, or unless the context is "creature".)
		if (indef_mode==0 && domain==actors_location
			&& scope_reason==PARSING_REASON && context~=CREATURE_TOKEN)
				ScopeWithin(compass);
		!  Look through the objects in the domain, avoiding "objectloop" in case
		!  movements occur, e.g. when trying each_turn.
		x = child(domain);
		while (x ~= 0)
		{   y = sibling(x);
			if(x hasnt contained || domain has open || domain has transparent)  !orlib: TODO: we are obviously deciding to NOT search things in a box; but this wasn't done by the current standard library's version of this, so why are we here?
				ScopeWithin_O(x, nosearch, context);
			x = y;
		}
	];
	!orLib: very little change to this routine; however, the changes appears right in the middle, so replacing the entire thing is the only real option for now.
	[ AttemptToTakeObject item
		ancestor after_recipient i k;

		! Try to transfer the given item to the actor: return false
		! if successful, true if unsuccessful, printing a suitable message
		! in the latter case.
		! People cannot ordinarily be taken.
		if (item == actor) return L__M(##Take, 2, noun);
		if (item has animate) return L__M(##Take, 3, item);

		ancestor = CommonAncestor(actor, item);

		if (ancestor == 0) {
			i = ObjectScopedBySomething(item);
			if (i) ancestor = CommonAncestor(actor, i);
		}

		! Is the actor indirectly inside the item?
		if (ancestor == item) return L__M(##Take, 4, item);

		! Does the actor already directly contain the item?
		if (item in actor) return L__M(##Take, 5, item);

		! Can the actor touch the item, or is there (e.g.) a closed container
		! in the way?
		if (ObjectIsUntouchable(item, false, true)) rtrue;

		! The item is now known to be accessible.

		! Consult the immediate possessor of the item, if it's in a container
		! which the actor is not in.

		i = parent(item);

		!if (i && i ~= ancestor && (i has container or supporter)) { !--orlib: replace this one line
		if (i && i ~= ancestor && numPcrsProvidedBy(i)>0) { ! honor any containment, not just container/supporter
			after_recipient = i;
			k = action; action = ##LetGo;
			if (RunRoutines(i, before)) { action = k; rtrue; }
			action = k;
		}

		if (item has static)  return L__M(##Take, 11, item);
		if (item has scenery) return L__M(##Take, 10, item);

		! The item is now known to be available for taking.  Is the player
		! carrying too much?  If so, possibly juggle items into the rucksack
		! to make room.

		if (ObjectDoesNotFit(item, actor) ||
			LibraryExtensions.RunWhile(ext_objectdoesnotfit, false, item, actor)) return;
		if (AtFullCapacity(item, actor)) return L__M(##Take, 12, item);

		! Transfer the item.

		move item to actor; give item ~worn;

		clearAllPcrsFromChild(item); !orlib: added this one line, which clears any containment conditions

		! Send "after" message to the object letting go of the item, if any.
		if (after_recipient) {
			k = action; action = ##LetGo;
			if (RunRoutines(after_recipient, after)) { action = k; rtrue; }
			action = k;
		}
		rfalse;
	];

	[ SayWhatsOn p c f;
		if (p==parent(actor)) rfalse;
		objectloop(c in p){
			if(isSupported(c))
				give c workflag;
			else
				give c ~workflag;
		}

		if (f==0) rfalse;
		L__M(##Look, 4, p);
		objectloop(c in p) give c ~workflag;
		rtrue;
	];



	[ RemoveSub p;
		p = parent(noun);
		if(isContained(noun) && p hasnt open && ImplicitOpen(p)) return L__M(##Remove, 1, p); !special case for containers: try to open it
		if (numPcrsProvidedBy(p)==0) return L__M(##Remove, 4, noun);
		_oldRemoveSub();
	];

	[ GenericInsertSub receiveAction orPcr ancestor;
		_pcr_libEx.lm_r=orPcr;
		receive_action = receiveAction;
		if (second == d_obj || actor in second) <<Drop noun, actor>>;
		if (parent(noun) == second && orPcr.isAppliedTo(noun)) {
			return L__M(##Drop, 1, noun);
		}
		if (ImplicitTake(noun) && noun notin actor) return L__M(##GenericInsert, 1, noun);

		ancestor = CommonAncestor(noun, second);
		if (ancestor == noun) return L__M(##GenericInsert, 2, noun);
		if (ObjectIsUntouchable(second)) return;

		if (second ~= ancestor) {
			action = ##Receive;
			if (RunRoutines(second, before)) { action = receiveAction; return; }
			action = receiveAction;
			if(orPcr.prepareToApply(noun,second)==false) return;
		}
		if (orPcr.isProvidedBy(second)==false) return L__M(##GenericInsert, 3, noun, second);

		if (noun has worn && no_implicit_actions) return L__M(##Disrobe, 4, noun);
		if (noun has worn && ImplicitDisrobe(noun)) return;

		if (ObjectDoesNotFit(noun, second) ||
			LibraryExtensions.RunWhile(ext_objectdoesnotfit, false, noun, second)) return;
		if (AtFullCapacity(noun, second)) return L__M(##GenericInsert, 4, noun, second);

		move noun to second;
		orPcr.applyTo(noun);

		if (AfterRoutines()) return;

		if (second ~= ancestor) {
			action = ##Receive;
			if (RunRoutines(second, after)) { action = receiveAction; return; }
			action = receiveAction;
		}
		if (keep_silent) return;
		if (multiflag) return L__M(##GenericInsert, 5, noun, second);
		L__M(##GenericInsert, 6, noun, second);
	];
	[ InsertSub;
		GenericInsertSub(##Insert, orPcrContainer);
	];
	[ PutOnSub;  GenericInsertSub(##PutOn, orPcrSupporter); ];
	[ DropSub r;
		_oldDropSub();
		if(parent(noun)==parent(actor)) {
			r=getPcrFromChild(actor); !apply the same relationship that the actor has with the parent...

			if(r)
				r.applyTo(noun);
			else
				clearAllPcrsFromChild(noun);
		}

	];
	[ ExitSub loc r;
		loc=parent(actor);
		_oldExitSub();
		if(parent(actor)==parent(loc)) { !have we successfully moved from our previous location to the parent of our previous location
			r=getPcrFromChild(loc); !if so, apply the same relationship that our previous location has
			if(r)
				r.applyTo(actor);
			else
				clearAllPcrsFromChild(actor);
		}
	];
	[ EnterSub; !--for either supporters or containers...
		_oldEnterSub();
		if(noun==parent(actor)) { !have we successfully entered something?
			!if the parent provides both supporter and container, then prefer to interpret it as container, based on the language used here "enter".  Could create a "Get On" verb which prefers the supporter relationship
			if(noun has container)
				give actor contained;
			else {
				if(noun has supporter)
					give actor supported;
				else
					clearAllPcrsFromChild(actor);
			}
		}
	];

	[ OpenSub;
		_currentPcrContext=orPcrContainer; !--the VisibleContents call is all we need to adjust in OpenSub...
		_oldOpenSub();
		_currentPcrContext=0;
	];

	[ SearchSub f;
		if (location==thedark) return L__M(##Search,1,noun);
		if (ObjectIsUntouchable(noun)) return;
		if (numPcrsProvidedBy(noun)==0) return L__M(##Search,4,noun); !--find nothing of interest

		!--at this point we are guaranteed at least one form of containment..
		f = VisibleContents(noun);

		if(f==0){ !--no visible children, so which message?
				!if supported, we treat containment as the preferred intended choice, even though other forms of containment may also be possible
				if(noun has container){
					if(noun hasnt transparent && noun hasnt open) return L__M(##Search,5,noun); !--"can't see inside"
					return L__M(##Search,6,noun); !--"is empty"
				}
				if(noun has supporter) return L__M(##Search,2,noun);
				return L__M(##Search,4,noun); !a parent which is neither a supporter nor a container; so same message as not a container "find nothing of interest"
		}
		if (AfterRoutines()==1) rtrue;
		!--actually contains something
		DescribeContents(noun);
		print "^";
		rtrue;
	];


	[inferPcr p c r;
		 if(p==0 || c==0) return 0;
		 r = getPcrFromChild(c);
		 if(r == 0) r = getFirstPcrProvidedBy(p);
		 if(r == 0) r = orPcrSupporter; !--default to supporter if no other relationship is found
		 return r;
	];



#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
