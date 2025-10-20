!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.03.11 orNameable
! Gives an object the ability to be named and then be referred to by that name.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! AutoDep :	None.
!--------------------------------------------------------------------------------------
! To create a nameable object, simple derive it from orNameable:
!
!	orNameable golem "Golem"
!		has	animate
!		with description "The creature looks dirty and pathetic."
!		,	name 'golem'
!	;
!
! And during game play you can name the golem with a command similar to:
!
!	>name golem "bob"
!
! And subsequently refer to the object by that name:
!
!	>x bob
!	The creature looks dirty and pathetic.
!
! Complementary extensions:
!	* orRecogName : if included, this module will continue to match user text against
!						an object's printed name.
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.12	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.03.11	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orNameable_STAGE  0;
!======================================================================================
! INCLUDE DEPENDENCIES
!#include "orPronoun";
#include "_orInfExt";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orNameable_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orNameable_STAGE  < LIBRARY_STAGE);
   #undef      orNameable_STAGE  ;
   Constant    orNameable_STAGE  LIBRARY_STAGE;
   #ifdef      orNameable_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orNameable...";#endif;
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);

	global named_wn;
	global namedArticle;
	#ifndef MAX_SIZE; constant MAX_SIZE=9;#endif;

	system_file;
	class orNameable
		with	custom_name	0 0 0 0 0 0 0 0 0 0 !--up to 20 characters in length (we're allocating WORDs here, to support memory tied to the object instance, but will use them as characters)
		,	_afterDescription false !--used to passthrough the following short_name (used with recursion)
		,	short_name[; if(self._afterDescription==false){
							self._afterDescription=true;
							print (name)self;
							if(self.&custom_name->0~=0){
								print " (named ~";
								self._customName();
								print "~)";
							}
							self._afterDescription=false;
							rtrue;
						}
				]
		,	customArticle 0
		,	before[;
				Name:	if(noun==self) {
							self._setName(named_wn);
							rtrue;
						}
			]
		,	parse_name[w t count o swn add length;
				length=WordLength(wn);
				if(length>MAX_SIZE) length=MAX_SIZE;
				if(parser_action==##TheSame){
					if(parser_two provides custom_name){
						for(t=0:t<parser_one.#custom_name:t++) if(parser_one.&custom_name->t~=parser_two.&custom_name->t) return -2; !--different
					}
					return 0; !cant distinguish between the two using the custom_name
				}
				while(true){
					swn=wn;
					w=NextWord();
					t=WordInProperty(w,self,name);
	#ifdef orRecogName_STAGE;
					if(t<1 || w==0) t=MatchAgainstShortName(self,swn);
	#endif;
					if(t<1){
						add=WordAddress(swn);
						t=true;
						for(o=0:o<length:o++){
							if(add->o~=self.&custom_name->o){
								t=false;
								break;
							}
						}
					}
					if(t<1) {
						if(count>0) return count;
						return -1;
					}
					count++;
				}
			]
		,	_customName[t temp;
				switch(self.customArticle){
					'a':print "a ";
					'an': print "an ";
					'the': print "the ";
				}
				for(t=0:t<self.#custom_name:t++){
					temp=(self.&custom_name->t);
					if(temp==0) continue;
					if(self.customArticle==0 && t==0) temp=temp-32;
					print (char)temp;
				}
			]
		,	_setName[addr len t;
				if(named_wn==0) rfalse;
				addr=WordAddress(named_wn);
				len=WordLength(named_wn);
				if(len==0) rfalse;
				if(len>self.#custom_name)len=self.#custom_name;
				for(t=0:t<self.#custom_name:t++) self.&custom_name->t=0; !-zero out name first
				for(t=0:t<len:t++) self.&custom_name->t=addr->t; !--copy new name in
				self.customArticle=namedArticle;
				L__M(##Name,1,self);
				rtrue;
			]
	;
	orInfExt with ext_messages[n x1 x2; x1=lm_o; x2=lm_s; n=lm_n;
			Name:
				switch(n){
					1 :	x1._afterDescription=true;
						print (ig)CIVerb(actor,"began","begin")," to think of ",(the)x1," as ";
						if(x1.customArticle==0) print "~";
						x1._customName();
						print".";
						if(namedArticle==0)print"~^";
						x1._afterDescription=false;
					2:	!bkt_o();
						print (The)x1," cannot be named.^";
						!bkt_c();
					3:	!bkt_o();
						print (ig)CIVerb(actor,"forgot","forget")," any name ",(I)actor," may have given ",(the)x1,".^";
						!bkt_c();
				}
		];

#endif; !--AFTER PARSER
!======================================================================================
! AFTER GRAMMAR
!-- isolate a single word, possibly quoted (name knife "stinger")
!-- and possibly beginning with an article (refer to the knife as a sword)
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);
	[single_word addr;
		named_wn=wn++;
		namedArticle=0;
		if(WordAddress(named_wn)->0=='"')named_wn=wn++; !--strip out quotes
		addr=WordAddress(named_wn);
		if(WordLength(named_wn)==3 && addr->0=='t' && addr->1=='h' && addr->2=='e') namedArticle='the';
		if(WordLength(named_wn)==2 && addr->0=='a' && addr->1=='n') namedArticle='an';
		if(WordLength(named_wn)==1 && addr->0=='a') namedArticle='a';
		if(namedArticle~=0) named_wn=wn++; !--strip out article
		if(WordAddress(named_wn+1)->0=='"') wn++;
		rtrue;
	];

	[NameSub; 	L__M(##Name,2);];
	[UnnameSub;
		if(noun provides custom_name) noun.custom_name=0;
		L__M(##Name,3);
	];

	Verb 'name'
		* noun single_word->Name
		* noun 'as'/'to' single_word ->Name
	;
	Verb 'unname'
		* noun -> Unname
	;
	Verb 'refer'
		* 'to' noun 'as' single_word ->Name
	;
	verb 'dub' = 'name';
	verb 'call' = 'name';
#endif; !--AFTER GRAMMAR
!--------------------------------------------------------------------------------------
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE

