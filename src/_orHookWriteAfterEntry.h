!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.05.18 orHookWriteAfterEntry
! Adds hooks into the WriteAfterEntry routine for use by the standard librarys
! LibraryExtensions object.  No functional changes have been made.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License:
!--------------------------------------------------------------------------------------
! This extension, classified as a "hook", provides tooling for other extensions to
! use and provides no specific features by itself.
!
! It replaces the large and bulky original version of WriteAfterEntry with a
! smaller version that does little more than make calls to routines that are
! pieces of the original WriteAfterEntry code.
!
! These smaller routines can more selectively be replaced by other modules.
!--------------------------------------------------------------------------------------
! Revision History
! 2003.02.21	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.05.18	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orHookWriteAfterEntry_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDED DEPENDENCIES

!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orHookWriteAfterEntry_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orHookWriteAfterEntry_STAGE  < LIBRARY_STAGE);
   #undef      orHookWriteAfterEntry_STAGE  ;
   Constant    orHookWriteAfterEntry_STAGE  LIBRARY_STAGE;
   #ifdef      orHookWriteAfterEntry_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orHookWriteAfterEntry..."; #endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	replace WriteAfterEntry;
	global _WAE_recurse_flag=0; !-
	global _WAE_parenth_flag=0; !-
	global _WAE_child_count=0;  !- flag3
	global _WAE_eldest_child=0; !-
    property individual ext_sdPartInvBit;
    property individual ext_sdFullInvBit;
    property individual ext_sdConcealBit;
    property individual ext_sdAlwaysBit;
    property individual ext_sdRecurseBit;
    property individual ext_sdIsOrAre;
    property individual ext_sdNewLineBit;
    property individual ext_sdDoList;
#endif; !--Before Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);
    [WriteAfterEntry o depth stack_p p recurse_flag parenth_flag eldest_child child_count combo i j retval;
	    !--two things: 1) we need to expose local variables to the LibraryExtensions handlers, so we make these global
        !              2) this routine gets called recursively which means those globals may be changed before we finish processing,
        !                 something not intended by the original code.
        !   So we save whatever the current state of the globals is into locals...
        eldest_child=_WAE_eldest_child;	recurse_flag=_WAE_recurse_flag;	parenth_flag=_WAE_parenth_flag;	child_count=_WAE_child_count;
        !   ...and reset the globals to nothing, after this runs and we reach the end, we reset the globals to their original
        !   values, just as though they were locals from the calling code.
        _WAE_eldest_child=0;_WAE_recurse_flag=0;_WAE_parenth_flag=0;_WAE_child_count=0;
        !--all code from the orginal version of this routine has been modified to use the global variables instead of the locals

        inventory_stage = 2;

        retval = LibraryExtensions.RunWhile(ext_sdPartInvBit, false,o); !--return true to interrupt standard library's default behavior
        if(retval == false && c_style & PARTINV_BIT){
            if (o.invent && RunRoutines(o, invent))
                if (c_style & NEWLINE_BIT) ""; else rtrue;

            combo = 0;
            if (o has light && location hasnt light) combo=combo+1;
            if (o has container && o hasnt open)     combo=combo+2;
            if ((o has container && (o has open || o has transparent))) {
                objectloop(i in o) {
                    if (i hasnt concealed && i hasnt scenery) {
                        j = true; break;
                    }
                }
                if (~~j) combo=combo+4;
            }
            if (combo) L__M(##ListMiscellany, combo, o);
        }   ! end of PARTINV_BIT processing

        retval = LibraryExtensions.RunWhile(ext_sdFullInvBit, false, o); !--return true to interrupt standard library's default behavior
        if(retval == false && c_style & FULLINV_BIT) {
            if (o.invent && RunRoutines(o, invent)){
                if (c_style & NEWLINE_BIT) "";
                else rtrue;
            }

            if (o has light && o has worn) { L__M(##ListMiscellany, 8, o);  _WAE_parenth_flag = true; }
            else {
                if (o has light)           { L__M(##ListMiscellany, 9, o);  _WAE_parenth_flag = true; }
                if (o has worn)            { L__M(##ListMiscellany, 10, o); _WAE_parenth_flag = true; }
            }

            if (o has container)
                if (o has openable) {
                    if (_WAE_parenth_flag) print (string) AND__TX;
                    else              L__M(##ListMiscellany, 11, o);
                    if (o has open)
                        if (child(o)) L__M(##ListMiscellany, 12, o);
                        else          L__M(##ListMiscellany, 13, o);
                    else
                        if (o has lockable && o has locked) L__M(##ListMiscellany, 15, o);
                        else                                L__M(##ListMiscellany, 14, o);
                    _WAE_parenth_flag = true;
                }
                else
                    if (child(o)==0 && o has transparent)
                        if (_WAE_parenth_flag) L__M(##ListMiscellany, 16, o);
                        else              L__M(##ListMiscellany, 17, o);

            if (_WAE_parenth_flag) print ")";
        }   ! end of FULLINV_BIT processing

        retval = LibraryExtensions.RunWhile(ext_sdConcealBit, false, o); !--return true to interrupt standard library's default behavior
        if(retval == false){
            if(c_style & CONCEAL_BIT) {
                _WAE_child_count = 0;
                objectloop (p in o)
                    if (p hasnt concealed && p hasnt scenery) { _WAE_child_count++; _WAE_eldest_child = p; }
            }
            else { _WAE_child_count = children(o); _WAE_eldest_child = child(o);}
        }

		retval = LibraryExtensions.RunWhile(ext_sdAlwaysBit, false, o); !--return true to interrupt standard library's default behavior
        if(retval == false){
            if (_WAE_child_count && (c_style & ALWAYS_BIT)) {
                if (c_style & ENGLISH_BIT) L__M(##ListMiscellany, 18, o);
                    _WAE_recurse_flag=true;
            }
        }

		retval = LibraryExtensions.RunWhile(ext_sdRecurseBit, false, o); !--return true to interrupt standard library's default behavior
        if(retval == false){
            if (_WAE_child_count && (c_style & RECURSE_BIT)) {
                if (o has supporter) {
                    if (c_style & ENGLISH_BIT) {
                        if (c_style & TERSE_BIT) L__M(##ListMiscellany, 19, o);
                        else                     L__M(##ListMiscellany, 20, o);
                        if (o has animate)       print (string) WHOM__TX;
                        else                     print (string) WHICH__TX;
                    }
                    _WAE_recurse_flag = true;
                }
                if (o has container && (o has open || o has transparent)) {
                    if (c_style & ENGLISH_BIT) {
                        if (c_style & TERSE_BIT) L__M(##ListMiscellany, 21, o);
                        else                     L__M(##ListMiscellany, 22, o);
                        if (o has animate)       print (string) WHOM__TX;
                        else                     print (string) WHICH__TX;
                        }
                    _WAE_recurse_flag = true;
                }
            }
        }
        retval = LibraryExtensions.RunWhile(ext_sdIsOrAre, false, o, _WAE_child_count); !--return true to interrupt standard library's default behavior
        if(retval == false){
            if (_WAE_recurse_flag && (c_style & ENGLISH_BIT)){
                if (_WAE_child_count > 1 || _WAE_eldest_child has pluralname) Tense(ARE2__TX, WERE2__TX);
                else Tense(IS2__TX, WAS2__TX);
	        }
        }
        retval = LibraryExtensions.RunWhile(ext_sdNewLineBit, false, o); !--return true to interrupt standard library's default behavior
        if(retval == false){
            if (c_style & NEWLINE_BIT ~= 0) new_line;
        }
		retval = LibraryExtensions.RunWhile(ext_sdDoList, false, o); !--return true to interrupt standard library's default behavior
        if(retval == false){
            if (_WAE_recurse_flag) {
                o = child(o);
                #Ifdef TARGET_ZCODE;
                @push lt_value; @push listing_together; @push listing_size;
                #Ifnot; ! TARGET_GLULX;
                @copy lt_value sp; @copy listing_together sp; @copy listing_size sp;
                #Endif;
                lt_value = 0;   listing_together = 0;   listing_size = 0;
                WriteListR(o, depth+1, stack_p);
                #Ifdef TARGET_ZCODE;
                @pull listing_size; @pull listing_together; @pull lt_value;
                #Ifnot; ! TARGET_GLULX;
                @copy sp listing_size; @copy sp listing_together; @copy sp lt_value;
                #Endif;
                if (c_style & TERSE_BIT) print ")";
            }
        }
    ! reset the globals to the original they had when this routine was first called.
	_WAE_eldest_child=eldest_child;_WAE_recurse_flag=recurse_flag;_WAE_parenth_flag=parenth_flag;_WAE_child_count=child_count;
];
#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
