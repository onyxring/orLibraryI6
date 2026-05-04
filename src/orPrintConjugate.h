!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.11.25 orPrintConjugate (English)
! Include noun-verb conjugation logic in orPrint.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! This a helper exension.  It detects the language and includes the appropriate 
! language-specific conjugation extension.  
!
! At the moment, only English is supported.
!--------------------------------------------------------------------------------------
! Revision History
! 2025.11.25	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
#ifndef        orPrintConjugate_STAGE;
Constant        orPrintConjugate_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orPrint";
#include "orUtilArray"; !needed for the conjugation routines (because they aren't 
                        !     included until after the LDF has set the languge value, 
                        !     there are limitations on what they can and cannot 
                        !     include)
!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        orPrintConjugate_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orPrintConjugate_STAGE  < LIBRARY_STAGE);
   #undef      orPrintConjugate_STAGE  ;
   Constant    orPrintConjugate_STAGE  LIBRARY_STAGE;
   #ifdef      orPrintConjugate_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orPrintConjugate...";#endif;
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
	!detect current language and load language-specific conjugation functionality...
	#ifdef LIBRARY_ENGLISH; #ifndef orPrintConjugate_English_STAGE; 
            constant orPrintConjugate_English_STAGE BEFORE_PARSER; #include "_orPrintConjugate_English"; 
	#endif; #endif;
      
#endif; !--After Parser
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE


