!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.05.11 orStr
! orStr provides a utility object for managing strings.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
! License: Public Domain
!--------------------------------------------------------------------------------------
! Revision History
! 2024.05.11	Initial creation
!--------------------------------------------------------------------------------------
! This extension, classified as a "utility", provides tooling for other extensions to
! use and supplies no specific story features by itself.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE	;
default        orUtilStr_STAGE  	0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUtil";
#include "orUtilRef";
#ifndef orString_STAGE; #include "orString"; #endif; !circular dependency
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orUtilStr_STAGE	; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orUtilStr_STAGE  	< LIBRARY_STAGE);
   #undef      orUtilStr_STAGE  	;
   Constant    orUtilStr_STAGE  	LIBRARY_STAGE;
   #ifdef      orUtilStr_STAGE  	; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orUtilStr...";#endif;

!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
	!a utility object with tools to manage strings, independent of actual orString objects
	object _orStr
		with new[unkStr !take a value and cast it as a string
				retval lck;
			lck=util.orStr.ambiguousLock(unkStr);  !encase an ephemeral is passed in, lock it
			retval=orStringPool.allocateString(); !to make sure it doesn't get reallocated
			if(unkStr~=0) retval.set(unkStr); !set the value on our newly allocated string
			util.orStr.ambiguousFree(unkStr, lck); !restore the unknown string type to its passed-in lock status
			return retval.lock(); !TODO: shouldn't need to call lock again; however some early testing showed that this was needed.  not sure if this is still the case or if a bug fix has addressed it, but leaving it in place for now until a deeper test can be created
		]
		, format[pattern param1 param2 param3
				retval;
			retval=orStringPool.allocateString();
			retval.set(retval.format(pattern, param1, param2, param3));
			return retval.free();
		]
		!--This routine returns the passed in value as a buffer, suitable for buffer routines to be applied against.
		!  This routine does NOT lock any buffers itself, but depending on the type of string object passed in, the returned value may
		!  or may not be locked.  As such, we refer to the returned value as an "ambiguous buffer".  Calling processes therefore should NOT try
		!  to lock or free the return value using normal lock and free techniques.
		,	toAmbiguousBuffer[unkStr
					retval;
				if(unkStr ofclass orString) return unkStr.getBuf();  !if parameter is orString, then use its buffer (which is probably already locked)
				!-must be a literal.  Let's convert it to buffer.
				retval=orStringPool.allocateBuffer();
				util.orBuf.set(retval,unkStr); !copy passed in string into buffer
				return orStringPool.freeBuffer(retval); !unlock this buffer, making it ephemeral.
			]
		! passed an ambiguous buffer, this routine will detect if it is a "free" buffer.  If so, it will lock it and return true; otherwise return false.
		! Calling processes should remember this return value and pass it to the ambiguousFree routine.  The intent for this routine pair is to ensure
		! all ambiguous buffers can be made safe, temporarily, and subsequently returned to their original state.
		,	ambiguousLock[buf retval;
				if(orStringPool.isFree(buf)) {
					orStringPool.reclaimBuffer(buf);
					orStringPool.reclaimString(buf);
					retval=true;
				}
				return retval;
			]
		,	ambiguousFree[buf val;
				if(val) { !was free, so restore unlocked status
					orStringPool.freeBuffer(buf);
					orStringPool.freeString(buf);
				}
			]

		! for testing orStr/buffer/literals, regardless of what they are
		,   areEqual[buf1 buf2 caseInsensitive;
				buf1=util.orStr.toAmbiguousBuffer(buf1);
				buf2=util.orStr.toAmbiguousBuffer(buf2);
				return util.orBuf.equals(buf1, buf2, caseInsensitive);
			]
		,	trim[buf
					str tmpStr;
				str=util.orStr.new(buf);
				tmpStr=str.trim();
				str.free();
				return tmpStr;
			]
		,	print[buf prop;  !--will print if passed 1. a literal packed string; 2. an orString object; 3. a character buffer; or 4. a "sized Buffer"
				if(prop~=0) buf=buf.prop; !if a property is provided, we assume the obj.prop syntax is meant to point us to a value to use, so resolve that as buf and continue

				if(buf provides print) {
					buf.print();  !orString or a string literal
					return;
				}
				if(util.orRef.isRoutine(buf)) return buf();
				util.orBuf.print(buf); !assume this is a character buffer of some kind
			]
		;

	#endif; !--AFTER PARSER
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
