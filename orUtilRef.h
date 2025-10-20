!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.07.09 orUtilRef
! Tools for using memory references
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! This extension, classified as a "utility", provides tooling for other extensions to
! use and provides no specific features by itself.
!
! It provides functions wrapped into helper objects.  Specifically:
!	orReference contains functions to access properties.
!--------------------------------------------------------------------------------------
! Revision History
! 2024.07.09	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orUtilRef_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUtil";
#include "orUtilNum";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orUtilRef_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orUtilRef_STAGE  < LIBRARY_STAGE);
   #undef      orUtilRef_STAGE  ;
   Constant    orUtilRef_STAGE  LIBRARY_STAGE;
   #ifdef      orUtilRef_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orUtilRef...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
object _orRef
	with isRoutine[r; if(metaclass(r)==routine) rtrue; rfalse; ]
	,	isString[r; if(metaclass(r)==string) rtrue;	]
	,	isObject[r; if(metaclass(r)==object) rtrue; rfalse; ]
    ,	isDictionaryWord[j;
			#Ifdef TARGET_ZCODE;
				!The following test was is copied straight out of the standard library, but doesn't always work.
				!	There are cases where a string literal will incorrectly return true for Z-code, because high memory is used
				!	for strings and overlaps the space where dictionary words are contained.

				!if (UnsignedCompare(j, HDR_DICTIONARY-->0) >= 0 && UnsignedCompare(j, HDR_HIGHMEMORY-->0) < 0)
				if (util.orNum.uIsBetween(j, HDR_DICTIONARY-->0, HDR_HIGHMEMORY-->0)==false) rfalse; !same as the above, but we only trust failure.  Its the false positives we can improve on.

				!This approach of converting from number to address, then back again, works some of the time, but not always.  It is
				!	entirely possible that the same address points to BOTH a string in high memory, AND to a dictionary
				!	word in readable memory.  So it's not reliable, and cannot be made reliable in Z-code.
				if(j==Dword__No(No__Dword(j))) rtrue;
				if(j==No__Dword(Dword__No(j))) rtrue;
			#Ifnot; ! TARGET_GLULX
				if (j->0 == $60) rtrue; !So much easier in GLULX, where data types are actually stored in memory with the type instance itself.
			#Endif; ! TARGET_
			rfalse;
		]
		!An enhanced version of the standard library's now unused valueOrRun() routine.  Traditionally used to resolve a property value
		!	regardless of whether it is an actual property or routine.
	,	resolveValue[obj prop
				parm1 parm2 parm3 parm4;
			if (util.orRef.isRoutine(obj)) return obj(prop, parm1, parm2, parm3, parm4);
			if (obj provides prop){
				if (util.orRef.isRoutine(obj.prop)) return obj.prop(parm1, parm2, parm3, parm4);
				return obj.prop; !value
			}
			return obj; !value
		]
	,	resolvePrint[obj prop
				parm1 parm2 parm3 parm4;
			if(obj==0 or -1) rfalse;
			if (util.orRef.isRoutine(obj)) return obj(prop, parm1, parm2, parm3, parm4);
			if (obj provides prop){
				if (util.orRef.isRoutine(obj.prop)) return obj.prop(parm1, parm2, parm3, parm4);
				print (string) obj.prop;
				rtrue;
			}
			print (string) obj; !value
			rtrue;
		]
	;
#endif; !--Before Parser

!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
