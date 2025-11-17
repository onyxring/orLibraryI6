!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.07.30 orInfExt
! A simple helper which saves just a few characters of code, keeping you from actually
! having to place extension objects as children of the LibraryExtension object.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! Instead of doing this:
!   object extName LibraryExtension with ext_initialise[;...];
!
! You can do this...
!   orInfExt with ext_initialise[;...];
!--------------------------------------------------------------------------------------
! Revision History
! 2024.07.30	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orInfExt_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orHookInformLibrary";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orInfExt_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orInfExt_STAGE  < LIBRARY_STAGE);
   #undef      orInfExt_STAGE  ;
   Constant    orInfExt_STAGE  LIBRARY_STAGE;
   #ifdef      orInfExt_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orInfExt..."; #endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
    class orInfExt;
    !--this is automatically called by the _orInformLibaryHook extension
    [allocateInformExtensions obj;
        obj=true;
        while(obj~=0){
            obj=getFirstOrphanExtensionObject();
            if(obj~=0) move obj to LibraryExtensions;
        }
    ];
    [getFirstOrphanExtensionObject obj;
        obj=0;
        objectloop(obj ofclass orInfExt && parent(obj)~=LibraryExtensions) break;
        if(obj ofclass orInfExt) return obj;
        return 0;
    ];
#endif; !--Before Parser
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
