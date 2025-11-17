!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.03.26 orHookStandardLibrary
! Adds additional hooks for use by the standard library's LibraryExtensions object.
! No default behavior has been change beyond allowing extensions to be defined.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! This extension, classified as a "hook", provides tooling for other extensions to
! use and provides no specific features by itself.
!--------------------------------------------------------------------------------------
! Revision History
! 2024.03.23	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orHookStandardLibrary_STAGE 0;
!--------------------------------------------------------------------------------------
! INCLUDED DEPENDENCIES

!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orHookStandardLibrary_STAGE ; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orHookStandardLibrary_STAGE < LIBRARY_STAGE);
   #undef      orHookStandardLibrary_STAGE ;
   Constant    orHookStandardLibrary_STAGE LIBRARY_STAGE;
   #ifdef      orHookStandardLibrary_STAGE ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orHookStandardLibrary..."; #endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
    replace Identical _oldIdentical;
    replace DisplayStatus _oldDisplayStatus;
    replace NextWord _oldNextWord;
    replace NounWord _oldNounWord;
    replace ActionPrimitive _oldActionPrimitive;
    replace AfterRoutines _oldAfterRoutines;

    property individual ext_identical;
    property individual ext_displayStatus;
    property individual ext_nounword;
    property individual ext_nextword;
    property individual ext_beforeAction;
    property individual ext_afterAction;
    property individual ext_afterRoutines;

#endif; !--BEFORE_PARSER
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);

    [ ActionPrimitive;
        if(LibraryExtensions.RunWhile(ext_beforeAction, false)~=false) return;
        _oldActionPrimitive();
        LibraryExtensions.RunAll(ext_afterAction);
    ];
    [ AfterRoutines;
        if(LibraryExtensions.RunWhile(ext_afterRoutines, 0)~=0) rtrue;
        return _oldAfterRoutines();
    ];

    [ NounWord rv;
        rv=LibraryExtensions.RunWhile(ext_nounword, 0);
        if(rv ~= 0) return rv;
        return _oldNounWord();
    ];
    [ NextWord rv;
        rv=LibraryExtensions.RunWhile(ext_nextword, 0);
        if(rv ~= 0) return rv;
        return _oldNextWord();
    ];
    [ Identical o1 o2;
        if(LibraryExtensions.RunWhile(ext_identical, true, o1,o2) == false) rfalse; !--ext_identical return true if it cant tell them apart; let some other method try
        return _oldIdentical(o1, o2);
    ];

    [DisplayStatus;
        if(LibraryExtensions.RunWhile(ext_displayStatus, false) ~= false) rfalse; !--an extension can return true to abort the original display logic
        _oldDisplayStatus();

    ];

#endif; !--AFTER_Parser
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE'