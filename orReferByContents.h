!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.08.03 orReferByContents
! An object that can be refered to by the name of what it contains.  For instance a
! glass of water can be taken by the command "get water".
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
!
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.22	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.08.03	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orReferByContents_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES

!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orReferByContents_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orReferByContents_STAGE  < LIBRARY_STAGE);
   #undef      orReferByContents_STAGE  ;
   Constant    orReferByContents_STAGE  LIBRARY_STAGE;
   #ifdef      orReferByContents_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orReferByContents...";#endif;
!======================================================================================
! AFTER PARSER
! TODO: is the current approach with referActions and ignoreActions sufficient? consider...  put marbles on shelf (you put the bottle on the shelf); take marbles from table; take marbles from bottle
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
system_file;
class orReferByContents
		with	referActions ##take ##drop ##insert
		,		ignoreActions 0
		,		match_name[w o;
					if(WordInProperty(w,self,name)==true && self.discount(w,self)==false)
						rtrue;
					else{
						objectloop(o in self) {
							if(WordInProperty(w,o,name)==true && self.discount(w,o)==false)
								rtrue;
						}
					}
					rfalse;
				]
		,		discount[w o i;w=w;o=o;i=i;
	#ifdef ordjective_STAGE;
					if((o provides adjective)==false) rfalse;
					i=WordInProperty(w,o,adjective);
					return i;
	#ifnot;
					rfalse;
	#endif;
				]
		,		match_short_name[w swn o; w=w;o=o; swn=swn;
	#ifdef orRecogName_STAGE;
						if(MatchAgainstShortName(self,swn)==true && self.discount(w,self)==false)
							rtrue;
						else{
							objectloop(o in self) {
								if(MatchAgainstShortName(o,swn)==true && self.discount(w,o)==false)
									rtrue;
							}
						}
						rfalse;
	#ifnot;
						rfalse;
	#endif;
				]
		,		parse_name[w found count svdwn passedinwn;
					if(parser_action==##TheSame) return 0; !--don't try to differentiate between similar objects

					if (found_ttype==HELD_TOKEN && (self in actor)==false && self has static or scenery) return -1;
					if(self.&referActions~=0 && WordInProperty(action_to_be,self,referActions)==false) return -1;
					if(self.&ignoreActions~=0 && WordInProperty(action_to_be,self,ignoreActions)==true) return -1;
					passedinwn=wn;
					do{
						found=false;
						svdwn=wn;
						w=NextWord();
						if(w~=0) found=self.match_name(w);
						if(found==false || w==0) found=self.match_short_name(w, svdwn);
						if(found==true) count++;
					}until(found==false);
					if(count>0) return count;
					wn=passedinwn; !--restore the passed in wn
					return -1;
				]
	;
#endif; !--After Parser
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE