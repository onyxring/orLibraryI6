!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! YYYY.MM.DD <REPLACEWITHNAME>
! A template for extensions.
!--------------------------------------------------------------------------------------
! Created by <AUTHOR NAME>
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! This file is a template for creating extensions compatable with the
! orLibrary Extension Framework.
!
! To use:
!
!	1)	Determine a unique name with no spaces which will be used to identify this
!     programatically.
!	2)	Copy this file and name it that SAME as that unique name, plus the .h extension.
!	3)	Run a "search and replace" replacing the text "<REPLACEWITHNAME>" with that
!		unique name.
!	4)	Add the name to the orLibrary's _orExtensionRegistry.h file which registers it
!       with the framework.
!  5) #Include any requisite extensions, registered with the framework, in the
!        "INCLUDE DEPENDENCIES" section.
!  6)  Add your code to the appropriate placeholder sections, allocated with regard to
!       the standard library's inclusion.
!	7)	Replace this text with a description of what your extension does and how to
!        use it.
!--------------------------------------------------------------------------------------
! Revision History
! YYYY.MM.DD	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
#ifndef        <REPLACEWITHNAME>_STAGE;
Constant        <REPLACEWITHNAME>_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES

!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        <REPLACEWITHNAME>_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       <REPLACEWITHNAME>_STAGE  < LIBRARY_STAGE);
   #undef      <REPLACEWITHNAME>_STAGE  ;
   Constant    <REPLACEWITHNAME>_STAGE  LIBRARY_STAGE;
   #ifdef      <REPLACEWITHNAME>_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   <REPLACEWITHNAME>...";#endif;
   !--uncomment the follwing to make this a Glulx-only extension
   !#ifndef  TARGET_GLULX; message fatalerror "<REPLACEWITHNAME>: ERROR! This extension is specific to the Glulx platform and will not compile for Z-Code."; #endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);

#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);

#endif; !--After Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE


