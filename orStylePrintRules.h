!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.03.09 orStylePrintRules
! Common print rules.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
! License: Public Domain
!--------------------------------------------------------------------------------------
! AutoDep  :	none
!--------------------------------------------------------------------------------------
! Print Rules defined in this module:
! italics		- print in emphasised
! strong		- print in bold font
! highlight		- print in reverse font (header)
!--------------------------------------------------------------------------------------
! Revision History
! 2001.08.13	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.03.09	Updated the module stucture to use features of the the 6/12 library
!				Added support for orString object
!======================================================================================
! INCLUDE DEPENDENCIES

!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orStylePrintRules_STAGE  0; 	#ifnot;
#ifndef        orStylePrintRules_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orStylePrintRules_STAGE  < LIBRARY_STAGE);
   #undef      orStylePrintRules_STAGE  ;
   Constant    orStylePrintRules_STAGE  LIBRARY_STAGE;
   #ifdef      orStylePrintRules_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orStylePrintRules...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	system_file;
	[italics o;
		style underline;
		print (string) o;
		style roman;
	];
	[strong o;
		style bold;
		print (string) o;
		style roman;
	];
	[highlight o;
		style reverse;
		print (string) o;
		style roman;
	];
! 	[stringarray o i;
! #ifdef orString_STAGE;
! 		if(o ofclass orString){
! 			o=o.primaryBuf;
! 		}
! #endif;
! 		if(metaclass(o)==String) {
! 			print (string)o;
! 		}
! 		else {
! 			for(i=0:i<(o-->0):i++) print (char) o->(i+WORDSIZE);
! 		}
! 	];
#endif; !--BEFORE_PARSER
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE