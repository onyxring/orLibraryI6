!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORNumberedContainer	[Z, GLULX]
!	This is an object that is designed to emulate a collection of openable, stationary containers 
!	(like lockers).  
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- - 
! License: 
! This work is licensed under the Creative Commons Attribution-ShareAlike License
! (http://creativecommons.org/licenses/by-sa/1.0/).
!
! In summary: you must credit the original author(s); if you alter, transform, or 
! build upon this software, you may distribute the SOURCE FORM of the resulting 
! work only under a license identical to this one. Note that the ShareAlike clause 
! does not affect the way in which you distribute the COMPILED FORM of works built upon
! this software. Copyright remains with the original author(s), from whom you
! must seek permission if you wish to vary any of the terms of this license.
! The author(s) would also welcome bug reports and enhancement suggestions.
!--------------------------------------------------------------------------------------
! AutoDep :	none
!--------------------------------------------------------------------------------------
! This object mimics a collection of openable objects and should have a plural short_name 
! which reflects this (such as "cubbies").  Additionally the property "singular_name" needs 
! to contain the name of an individual container (such as "cubby").  The properties start_numb
! and end_numb should hold values that define the range of container numbers (defaulted to 1 - 20)
!
!	sample use:
!
!		ORNumberedContainer -> SchoolLockers "lockers" 
!			with	name	"lockers" "locker"
!			,		singular_name "locker"
!			,		start_numb	20
!			,		end_numb	40
!			,		description [; print "This is a long line of lockers, numbered from 20 to 40.";
!							self.numbered_description();];
!
!	An object can be placed inside a specific locker with the following command:
!
!		SchoolLockers.add_item_to(sunglasses,33); !--put the sunglasses in locker #33
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORNumberedContainer";					!Constant USE_ORNumberedContainer; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORNumberedContainer; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.04.10	Initial Creation 
! 2002.02.08	Added GLULX support.
! 2002.02.26	Fit to new template.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORNumberedContainer...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORNumberedContainer forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORNumberedContainer has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORNumberedContainer_DONEREPLACE; constant ORNumberedContainer_DONEREPLACE; #ifdef ORNumberedContainer_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORNumberedContainer_DONEMESSAGE; constant ORNumberedContainer_DONEMESSAGE;  #ifdef ORNumberedContainer_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORNumberedContainer requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORNumberedContainer_DONECODE;constant ORNumberedContainer_DONECODE; #ifdef ORNumberedContainer_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	class ORNumberedContainer
		with	singular_name "<singular_name>"
		,		open_state		0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
		,		contained_obj	0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
		,		contained_in	0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 		
		,		description [;self.numbered_description();]
		,		isnumbervalid[n;
					if(n==0)n=self.refer_number;
					if(self.refer_number<self.start_numb || self.refer_number>self.end_numb) {
						print "There ",(string)IS__TX," no ",(string)self.singular_name," with that number.";
						rfalse;
					}
					rtrue;
				]
		,		numbered_description
				[total count t;
					print "Each ",(string)self.singular_name," ",(string)IS__TX," labled with a number beginning with ",self.start_numb," and ending with ",self.end_numb,". ";
					total=0;
					for(t=0:t<(self.#open_state/WORDSIZE):t++) if(self.&open_state-->t~=0) total++;
					if(total~=0) {
						print "The ";
						if(total==1) 
							print (string)self.singular_name;
						else 
							print (name)self;
						print " numbered ";
						for(t=0:t<(self.#open_state/WORDSIZE):t++) if(self.&open_state-->t~=0) {
							count++;
							if(count==total && total>1)print "and ";
							print self.&open_state-->t;
							if(count~=total && total>2)print ",";
							print " ";
						}
						if(total==1)print",(string)IS__TX,";
						else print (ig)ppf("were","are");
						print" open. ";
					}
				]
		,		start_numb 1
		,		end_numb 20
		,		refer_number 0 !--container number being referenced
		,		find_internal[obj t;
					for(t=0:t<(self.#contained_obj/WORDSIZE):t++){
						if(self.&contained_obj-->t==obj) return t;
					}
					return -1;
				]
		,		remove_item[obj j;
					j=self.find_internal(obj);
					if(j~=-1) self.&contained_obj-->j=self.&contained_in-->j=0;
					return;
				]
		,		add_item_to[obj num j;
					j=self.find_internal(0); !--find empty
					if(j==-1) {
						print "ERROR!!! ",(the)self," is unable to contain more objects.";
						return;
					}
					self.&contained_obj-->j=obj;
					self.&contained_in-->j=num;
					return;
				]
		,		before
				[o count total n;
					receive, open, close, Search: print_ret "There ",(isorare)self," many ",(name) self," here. Which one specifically?";
					LetGo: self.remove_item(noun);
					ReceiveNum:	if(self.isnumbervalid()==false) rtrue;
								if((wordinproperty(self.refer_number,self,open_state))==false) 
									print_ret "That specific ",(string) self.singular_name," ",(string)IS__TX," closed.";
								else
								{
									self.add_item_to(second,self.refer_number);
									move second to self;
									print_ret (ig)CIVerb(actor,"put","put")," ",(the) second," into the ",(string) self.singular_name," (number ",self.refer_number,").";
								}
					SearchNum:	if(self.isnumbervalid()==false) rtrue;
								if((wordinproperty(self.refer_number,self,open_state))==true) 
								{
									total=0;
									for(n=0:n<(self.#contained_obj/WORDSIZE):n++) if(self.&contained_in-->n==self.refer_number) total++;
									if(total==0) print_ret "There ",(string)IS__TX," nothing inside.";
									print "Inside the ",(string) self.singular_name," ",(string)IS__TX," ";
									count=0;
									for(n=0:n<(self.#contained_obj/WORDSIZE):n++) {
										if(self.&contained_in-->n==self.refer_number) {
											o=self.&contained_obj-->n;
											count++;
											if(count==total && total>1) print " and ";
											print (a)o;
											if(count<total) 
												print", ";
											else
												print".";
										
										}
									}
									return true;
								}
								else print_ret "That ",(string) self.singular_name," ",(string)IS__TX," closed.";
								
					OpenNum:	if(self.isnumbervalid()==false) rtrue;
								if((wordinproperty(self.refer_number,self,open_state))==true) 
									print_ret "That ",(string) self.singular_name," ",(string)IS__TX," already open.";
								else{
									for(n=0:n<(self.#open_state/WORDSIZE):n++){
										if(self.&open_state-->n~=0) continue;
										self.&open_state-->n=self.refer_number;
										print (ig)CIVerb(actor,"opened","open")," the ",(string) self.singular_name,". ";
										for(n=0:n<(self.#contained_in/WORDSIZE):n++) if(self.&contained_in-->n==self.refer_number) move self.&contained_obj-->n to self; !--nested for, okay to reuse n since exiting
										<<SearchNum noun>>;
									}
									printorrun(self,cannot_open_more_msg);
									rtrue;
								}
					CloseNum:	if(self.isnumbervalid()==false) rtrue;
								if((wordinproperty(self.refer_number,self,open_state))==false) 
									print_ret "That ",(string) self.singular_name," ",(string)IS__TX," already closed.";
								else
								{
									for(n=0:n<(self.#open_state/WORDSIZE):n++){
										if(self.&open_state-->n~=self.refer_number) continue;
										self.&open_state-->n=0;
										for(n=0:n<(self.#contained_in/WORDSIZE):n++) if(self.&contained_in-->n==self.refer_number) remove self.&contained_obj-->n;!--nested for, okay to reuse n since exiting
										print_ret (ig)CIVerb(actor,"closed","close")," the ",(string) self.singular_name,". ";
									}
								}
					
				]
		,		cannot_open_more_msg [;
						print_ret (ig)CICant(actor)," open any more ",(name)self,". Some ",(ig)ppf("needed","need")," to be closed first. ";
					]
		has		static scenery pluralname container open
	;
!======================================================================================
#endif; #ifnot; #ifndef ORNumberedContainer_DONEGRAMMAR; constant ORNumberedContainer_DONEGRAMMAR; #ifdef ORNumberedContainer_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
	!---- handle numbered objects ----------------------------------------------
	[NumberedObj o word n;
		o=noundomain(location,actor,NOUN_TOKEN);
		if(o==0 or 1) return -1;
		if((o ofclass ORNumberedContainer)==false) return -1;
		word=NextWord(); !get the next word and move on...
		if(word~='number' or 'numbered') wn--; !if the next word was not a word to ignore then backtrack
		n=trynumber(wn);
		if(n==-1000 or 0) return -1; !this may be a numbered object, but if we can't find the specifying number then move on
		o.refer_number=n;
		wn++;
		return o;
	];
	!---- numbered object open -------------------------------------------------
	Extend "open" first
		* NumberedObj			->	OpenNum
	;
	[OpenNumSub;
		print (The) noun," is not a numbered object.";	
	];
	!---- numbered object close -------------------------------------------------
	Extend "close" last
		* NumberedObj			->	CloseNum
	;
	[CloseNumSub;
		print (The) noun," is not a numbered object.";	
	];
	!---- numbered object search -------------------------------------------------
	Extend "examine" last
		* NumberedObj			->	SearchNum
	;
	Extend "look" last
		* "in"/"at" NumberedObj	->	SearchNum
	;
	[SearchNumSub;
		print (The) noun," is not a numbered object.";	
	];
	!---- numbered object insert into -------------------------------------------------
	Extend "insert" first
		* held "into"/"in" NumberedObj ->	ReceiveNum reverse 
		* "into"/"in" NumberedObj held ->	ReceiveNum 
	;
	Extend "put" first
		* held "into"/"in" NumberedObj ->	ReceiveNum reverse
		* "into"/"in" NumberedObj held ->	ReceiveNum 
	;
	!the noun should be the numbered container so that it can handle the before and after routiones and 
	!determine what to do based on the number
	[ReceiveNumSub;
		print (The) noun," is not a numbered object.";	
	];
!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================