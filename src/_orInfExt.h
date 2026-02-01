!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.07.30 orInfExt
! A helper which saves just a few characters of code, keeping you from actually
! having to place extension objects as children of the LibraryExtension object, and 
! also sorts extension objects in the LibraryExtensions based on priority.
!
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
#ifndef        orInfExt_STAGE;
default        orInfExt_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orHookInformLibrary";
#ifndef orUtilArray_STAGE; #include "orUtilArray"; #endif;
!--------------------------------------------------------------------------------------
#endif;
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
    class orInfExt with priority 10;
    !--this is automatically called by the _orInformLibaryHook extension
    [allocateInformExtensions obj;
        obj=true;
        while(obj~=0){
            obj=getFirstOrphanExtensionObject();
            if(obj~=0) move obj to LibraryExtensions;
        }
        sortLibraryExtensionsByPriority();
    ];
    [getFirstOrphanExtensionObject obj;
        obj=0;
        objectloop(obj ofclass orInfExt && parent(obj)~=LibraryExtensions) break;
        if(obj ofclass orInfExt) return obj;
        return 0;
    ];
    
    array usedPriorities table 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0;
    [sortLibraryExtensionsByPriority 
            obj pri sib o;
        objectloop(obj in LibraryExtensions){
            if(obj provides priority)
                pri=valueOrRun(obj,priority);
            else   
                pri=10;
            
            if(util.orArray.find(usedPriorities,pri)==-1) {
                util.orArray.insert(usedPriorities,orArrayNoProp,0,pri);
            }
        }
        util.orArray.sort(usedPriorities,numDescending); !--sort decending, so higher numbers are inserted first lower numbers last
                                                         !  pushing down the higher priority values.  The net effect is that lower 
                                                         !  priority values are executed first when resolved by the LibraryExtension object

        for(o=0:o<util.orArray.getLength(usedPriorities):o++){
            pri=util.orArray.get(usedPriorities,o);
            obj=child(LibraryExtensions);
            while(obj~=0){
                sib=sibling(obj);
                if( (obj provides priority && valueOrRun(obj,priority)==pri) ||
                    ((obj provides priority)==false && pri==10) ){
                        move obj to LibraryExtensions;
                    }
                    obj=sib;               
            }
        }        
        
        return;
    ];
#endif; !--Before Parser
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
