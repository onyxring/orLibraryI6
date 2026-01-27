!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.04.14 orChar
! Various utility functions for dealing with Characters.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
! License:
!--------------------------------------------------------------------------------------
! This extension, classified as a "utility", provides tooling for other extensions to
! use and provides no specific features by itself.
!
! It provides functions focused on characters (letters, digits, and symbols rather than
! story characters), wrapped into the orChar object.
!--------------------------------------------------------------------------------------
! Revision History
! 2024.04.14	Initial creation.
!======================================================================================
! Extension Framework management
#ifndef orExtensionFramework_STAGE;
#ifndef orUtilChar_STAGE;
default        orUtilChar_STAGE  0;
!======================================================================================
! INCLUDE DEPENDENCIES
#include "_orUtil";

!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        orUtilChar_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orUtilChar_STAGE  < LIBRARY_STAGE);
   #undef      orUtilChar_STAGE  ;
   Constant    orUtilChar_STAGE  LIBRARY_STAGE;
   #ifdef      orUtilChar_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orUtilChar...";#endif;

!======================================================================================
! BEFORE PARSER
!--------------------------------------------------------------------------------------
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	array __vowels string "aeiouAEIOU@:a@:o@:uo@:eo@:io@:y@:A@:O@:U@:E@:I@'a@'e@'i@'o@'u@'y@'A@'E@'I@'O@'U@'Y@`a@`e@`i@`o@`u@`A@`E@`I@`O@`U@^a@^e@^i@^o@^u@^A@^E@^I@^O@^U@oa@oA@~a@~n@~o@~A@~N@~O@ae@AE@et@Et@/o@/O";
#endif;
!======================================================================================
! AFTER VERBLIB
!--------------------------------------------------------------------------------------
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

	object _orChar
		with isAlpha[c;
				if(self.isLower(c)) rtrue;
				if(self.isUpper(c)) rtrue;
				rfalse;
			]
		,	isNumeric[c;
				if(c>='0' && c<='9') rtrue; rfalse;
			]
		,	isAlphaNumeric[c;
				if(self.isAlpha(c)==true) rtrue;
				if(self.isNumeric(c)==true) rtrue;
				rfalse;
			]
		,	isVowel[c
					t;
				for(t=1:t<=__vowels->0:t++) if(c==__vowels->t) rtrue;
				rfalse;
			]
		,	isConsonant[c;
				if(self.isVowel(c)==true) rfalse;
				if(self.isAlpha(c)==false) rfalse;
				rtrue;
			]
		,	isLower[c;
				if(c>='a' && c<='z') rtrue; 
				
				if((c>=155 && c<=157) || (c>=164 && c<=166)) rtrue; !diaeresis +3 (except for 166 which does not convert)

				if(c>=169 && c<=174) rtrue; !acute +6
				if(c>=181 && c<=185) rtrue; !grave +5
				if(c>=191 && c<=195) rtrue; !circumflex +5
				if(c==201) rtrue; ! +1 angstrom
				if(c==203) rtrue; ! +1 slashed o				
				if(c>=205 && c<=207) rtrue; !tilde +3
				if(c==211) rtrue; ! +1 ae
				if(c==213) rtrue; ! +1 cedilla
				if(c==215) rtrue; ! +2 th
				if(c==216) rtrue; ! +2 eth
				if(c==220) rtrue; ! +1 oe

				rfalse;
			]
		,	isUpper[c;
				if(c>='A' && c<='Z') rtrue; 
				if((c>=158 && c<=159) || (c>=167 && c<=168)) rtrue; !diaeresis
				if(c>=175 && c<=180) rtrue; !acute 
				if(c>=186 && c<=190) rtrue; !grave 
				if(c>=196 && c<=200) rtrue; !circumflex 
				if(c==202) rtrue; ! angstrom
				if(c==204) rtrue; ! slashed o				
				if(c>=208 && c<=210) rtrue; !tilde 
				if(c==212) rtrue; ! ae
				if(c==214) rtrue; ! cedilla
				if(c==217) rtrue; ! th
				if(c==218) rtrue; ! eth
				if(c==221) rtrue; ! oe

				rfalse;
			]
		,	toUpper[c;
				if(self.isLower(c)==false) return c;			
				if(c<155) return UpperCase(c);
				
				if((c>=155 && c<=157) || (c>=164 && c<=165) || (c>=205 && c<=207) ) return c+3; !diaeresis, tilde 			
				if(c>=169 && c<=174) return c+6; !acute 
				if((c>=181 && c<=185) || (c>=191 && c<=195)) return c+5; !circumflex, grave 			
				if(c==215 or 216) return c+2;  ! th, eth
				if(c==201 or 203 or 211 or 213 or 220) return c+1; !--angrstrom, slashed o, ae, cedilla, oe
				return c;
			]
		,	toLower[c;
				if(self.isUpper(c)==false) return c; 
				if(c<155) return LowerCase(c);
				
				if((c>=158 && c<=160) || (c>=167 && c<=168) || (c>=208 && c<=210) ) return c-3; !diaeresis, tilde 			
				if(c>=175 && c<=180) return c-6; !acute 
				if((c>=186 && c<=190) || (c>=196 && c<=200)) return c-5; !circumflex, grave 			
				if(c==217 or 218) return c-2;  ! th, eth
				if(c==202 or 204 or 212 or 214 or 221) return c-1; !--angrstrom, slashed o, ae, cedilla, oe
				return c;
			]
	;
	#endif; !--AFTER VERBLIB
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE


