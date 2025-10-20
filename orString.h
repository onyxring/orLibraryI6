!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.05.11 orString
! orString wraps string buffers and exposes formalized objects to create dynamic text.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
! License: Public Domain
!--------------------------------------------------------------------------------------
! Revision History
! 2001.12.26	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.05.11	Refactored for the orLibrary version 2; refactored from original orLib's
!				orDynaString.
!--------------------------------------------------------------------------------------
! Complementary extensions:
! *	orPrint : If included, the orString.format() will also resolve print patterns.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE	;
default        orString_STAGE  	0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orUtilBuf";
!#ifndef orStringPool_STAGE; #include "_orStringPool"; #endif; !circular dependency
#ifndef orUtilStr_STAGE; #include "orUtilStr"; #endif; !circular dependency
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orString_STAGE	; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orString_STAGE  	< LIBRARY_STAGE);
   #undef      orString_STAGE  	;
   Constant    orString_STAGE  	LIBRARY_STAGE;
   #ifdef      orString_STAGE  	; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orString...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	default orStringDefaultSize	500;
	default orStringPoolReserve 10; !ephemeral, "unsafe" strings
	default orBufferPoolReserve (orStringPoolReserve+2); !need to allocate enough buffers to cover all declared strings, plus a bit more to cover actively used temporary orStrings

	constant orSTRING_END = orBUF_END; #ifdef orSTRING_END;#endif; !suppress warning

	class orString(orStringPoolReserve)
		private _underlyingBuffer 0
		with getLength [; return util.orBuf.getLength(self.getBuf());]
		,	getBuf[suppressAllocation; !-- returns the buffer underlying this object; allocating a new one if necessary (unless suppressAllocation is true)
				if(self._underlyingBuffer==0 && suppressAllocation==0) self._underlyingBuffer=orStringPool.allocateBuffer();
				return self._underlyingBuffer;
			]
		!-- information about the string routines
		,	getChar[pos; return self.getBuf()->(pos+WORDSIZE); ]
		,	isEmpty[;return util.orBuf.getLength(self.getBuf())==0;]

		!-- memory management routines
		,	clone[;
				return util.orStr.new(self);
			]
		,	isLocked[;
				return orStringPool.isLocked(self);
			]
		,	isBufferLocked[;
				return orStringPool.isLocked(self._underlyingBuffer);
			]
		,	lock[; !-- reclaim the lock on a string which was previously freed, assuming it hasnt been recycled
				orStringPool.reclaimString(self);
				return self;
			]
		,	free[; !dispose of this string, and its associated buffer, unlocking it by restoring it the pool and making it unsafe for most uses
				orStringPool.freeString(self);
				return self;
			]
		!-- routines modifying the string itself
		,	set[unkStr ab lckBuf lckSelf;
				lckSelf=util.orStr.ambiguousLock(self);
				ab=util.orStr.toAmbiguousBuffer(unkStr);
				lckBuf=util.orStr.ambiguousLock(ab);
				util.orBuf.set(self.getBuf(),ab);
				util.orStr.ambiguousFree(ab,lckBuf);
				util.orStr.ambiguousFree(self,lckSelf);
				return self;
			]
		,	capture[; util.orBuf.capture(self.getBuf()); return self;]
		,	release[; util.orBuf.release(); return self;]
		,	captureOutput[o r;
				self.capture();
				util.orStr.print(o,r);
				self.release();
				return self;
			]
		,	setLength [newLength; util.orBuf.setLength(self.getBuf(),newLength); return self;]
		,	setChar[pos val; self.getBuf()->(pos+WORDSIZE)=val; return self;]
		!-- functional routines
		,	print[; util.orBuf.print(self.getBuf()); return self; ]
		!-- search routines
		,	equals[altBuf caseInsensitive; return util.orBuf.equals(util.orStr.toAmbiguousBuffer(altBuf), self.getBuf(), caseInsensitive); ]
		,	equalsOneOf[buf1 buf2 buf3 buf4 caseInsensitive;
				if(buf1~=0 && util.orBuf.equals(util.orStr.toAmbiguousBuffer(buf1), self.getBuf(), caseInsensitive)==true) rtrue;
				if(buf2~=0 && util.orBuf.equals(util.orStr.toAmbiguousBuffer(buf2), self.getBuf(), caseInsensitive)==true) rtrue;
				if(buf3~=0 && util.orBuf.equals(util.orStr.toAmbiguousBuffer(buf3), self.getBuf(), caseInsensitive)==true) rtrue;
				if(buf4~=0 && util.orBuf.equals(util.orStr.toAmbiguousBuffer(buf4), self.getBuf(), caseInsensitive)==true) rtrue;
				rfalse;
			]
		,	equalsOneOfCaseInsensitive[buf1 buf2 buf3 buf4; return self.equalsOneOf(buf1, buf2, buf3, buf4, true); ]
		,	indexOf[searchText startingIndex;  return util.orBuf.indexOf(self.getBuf(), util.orStr.toAmbiguousBuffer(searchText), startingIndex);  ]
		,	numOccurances[searchText startingIndex count n;
				n=util.orBuf.indexOf(self.getBuf(), util.orStr.toAmbiguousBuffer(searchText), startingIndex);
				while(n>=0){
					count++;
					startingIndex=n;
					n=util.orBuf.indexOf(self.getBuf(), util.orStr.toAmbiguousBuffer(searchText), startingIndex+1);
				}
				return count;
			]
		,	contains[searchText; return self.indexOf(searchText)>-1; ]
		,	indexOfFirstFalse[routine startingIndex; return util.orBuf.indexOfFirstFalse(self.getBuf(), routine, startingIndex); ]
		,	indexOfFirstTrue[routine startingIndex; return util.orBuf.indexOfFirstTrue(self.getBuf(), routine, startingIndex); ]
		!-- Modifications, returning "free" strings
		,   insert[pos source count
					retval;
				retval=self.clone();
				util.orBuf.insert(retval.getBuf(), util.orStr.toAmbiguousBuffer(source), pos, count);
				return retval.free();
			]
		,   delete[pos count
					retval;
				retval=self.clone();
				util.orBuf.delete(retval.getBuf(),pos, count);
				return retval.free();
			]
		,   replace[searchText replaceText
					retval stl;
				retval=self.clone();
				searchText=util.orStr.toAmbiguousBuffer(searchText);
				stl=util.orStr.ambiguousLock(searchText);
				replaceText=util.orStr.toAmbiguousBuffer(replaceText);
				util.orBuf.replace(retval.getBuf(), searchText, replaceText);
				util.orStr.ambiguousFree(searchText, stl);
				return retval.free();
			]
		,   replaceAll[searchText replaceText
					retval stl;

				retval=self.clone();
				searchText=util.orStr.toAmbiguousBuffer(searchText);
				stl=util.orStr.ambiguousLock(searchText);
				replaceText=util.orStr.toAmbiguousBuffer(replaceText);
				util.orBuf.replaceAll(retval.getBuf(), searchText, replaceText);
				util.orStr.ambiguousFree(searchText, stl);
				return retval.free();
			]
		,	upper[retval;
				retval=self.clone();
				util.orBuf.toUpper(retval.getBuf());
				return retval.free();
			]
		,	append[text
					retval;
				retval=self.clone();
				util.orBuf.append(retval.getBuf(), util.orStr.toAmbiguousBuffer(text));
				return retval.free();
			]
		,	prepend[text
					retval;
				retval=self.clone();
				util.orBuf.prepend(retval.getBuf(), util.orStr.toAmbiguousBuffer(text));
				return retval.free();
			]
		,	lower[retval;
				retval=self.clone();
				util.orBuf.toLower(retval.getBuf(), self.getBuf());
				return retval.free();
			]
		,	reverse[retval;
				retval=self.clone();
				util.orBuf.reverse(retval.getBuf(), self.getBuf());
				return retval.free();
			]
		,	trimLeft[retval;
				retval=self.clone();
				util.orBuf.trimLeft(retval.getBuf(), self.getBuf());
				return retval.free();
			]
		,	trimRight[retval;
				retval=self.clone();
				util.orBuf.trimLeft(retval.getBuf(), self.getBuf());
				return retval.free();
			]
		,	trim[retval;
				retval=self.clone();
				util.orBuf.trim(retval.getBuf(), self.getBuf());
				return retval.free();
			]
		,	mid[start count retval;
				retval=util.orStr.new();
				util.orBuf.mid(retval.getBuf(), self.getBuf(), start, count);
				return retval.free();
			]
		,	left[count retval;
				retval=util.orStr.new();
				util.orBuf.left(retval.getBuf(), self.getBuf(), count);
				return retval.free();
			]
		,	right[count retval;
				retval=util.orStr.new();
				util.orBuf.right(retval.getBuf(), self.getBuf(), count);
				return retval.free();
			]
		,	format[pattern p1 p2 p3
					retval;
				retval=util.orStr.new();
				if(pattern==0) pattern="$0";
				pattern=util.orStr.new(pattern);
				pattern.set(pattern.replaceAll("$0",self));


#ifdef orPrint_STAGE;
				retval.capture();
				orPrint(pattern, p1, p2, p3);
				retval.release();
#ifnot;
				if(p1~=0) pattern.set(pattern.replaceAll("$1",p1));
				if(p2~=0) pattern.set(pattern.replaceAll("$2",p2));
				if(p3~=0) pattern.set(pattern.replaceAll("$3",p3));

				retval.set(pattern);
#endif;
				pattern.free();
				return retval.free();

			]
	;
	#endif; !--BEFORE PARSER
!======================================================================================
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

! orStringPool provides a pattern to reuse buffers and orStrings, somewhat working around
	! the fact memory cannot be dynamically allocated in the Z-machine
	object orStringPool
		private _allocateObj[type  !pick an arbitrary free object, lock it, and return it for use
				o retval;
			objectloop(o in orObjectHeap && o ofclass type) {
				retval=o;
				break;
			}
			if(retval==0) {
				print "[ERROR: Unable to allocate a new instance.]";
				return 0;
			}
			move retval to orAllocatedObjects;
		#ifdef orProfiler_STAGE;
			self.performProfiling();
		#endif;
			return retval;
		]
	,	_freeObj[obj; !take specific allocated object, and free it for later allocations
			if(obj==0) return 0;
			move obj to orObjectHeap;
			return obj;
		]
	,	_reclaimObj[obj; !take an specific free object, and lock it
			if(obj==0) return 0;
			move obj to orAllocatedObjects;
		#ifdef orProfiler_STAGE;
			self.performProfiling();
		#endif;
			return obj;
		]
		!find the allocated object wrapper for the given buffer
	,	_findAllocatedObjFromBuffer[b o;
			objectloop(o in orAllocatedObjects && o ofclass orBufferWrapper ) if(o.buf==b) return o;
			return 0;
		]
			!find the unallocated object wrapper for the given buffer
	,	_findAvailableObjFromBuffer[b o;
			objectloop(o in orObjectHeap && o ofclass orBufferWrapper ) if(o.buf==b) return o;
			return 0;
		]
	,	isManagedBuffer[b; !-is the buffer one of the ones we manage in our allocation/free pool?
			if(self._findAllocatedObjFromBuffer(b)==0) rfalse;
			rtrue;
		]
	,	_findObjFromBuffer[b retval;
			retval=self._findAllocatedObjFromBuffer(b);
			if(retval==0) retval=self._findAvailableObjFromBuffer(b);
			return retval;
		]
		!find the allocated object wrapper for the given buffer
	,	_findAllocatedObj[b o;
			objectloop(o in orAllocatedObjects) if(o==b) return o;
			return 0;
		]
		!find the unallocated object wrapper for the given buffer
	,	_findAvailableObj[b o;
			objectloop(o in orObjectHeap) if(o==b) return o;
			return 0;
		]
	with allocateString[retval; !pick an arbitrary free string, lockit, then initialize it with an arbitrary free buffer; return the string
			retval = self._allocateObj(orString);
			retval.getBuf(); !force initilization
 			return retval;
		]
	,	freeString[str; !take a specific allocated string, and free it for later use
			self.freeBuffer(self._findAllocatedObj(str));
			self._freeObj(self._findAllocatedObj(str));

			return str;
		]
	,	reclaimString[b retval; !take a previously freed string and and lock it again
			retval = self._reclaimObj(self._findAvailableObj(b));
			if(retval~=0) self.reclaimBuffer(retval.getBuf(true));
			return retval;
		]
	,	allocateBuffer[retval; !pick an arbitrary buffer, allocate it, and set its length to 0, returning it
			retval = self._allocateObj(orBufferWrapper).buf;
			util.orBuf.setLength(retval,0);
			return retval;
		]
	,	freeBuffer[b; !take a specific buffer and free it for later allocation
			self._freeObj(self._findAllocatedObjFromBuffer(b));
			return b;
		]
	,	reclaimBuffer[b retval;
			retval = self._reclaimObj(self._findAvailableObjFromBuffer(b));
			if(retval~=0) return retval.buf;
			return 0;
		]
	, 	isLocked[obj;
			if(self._findAllocatedObj(obj)) return true;
			if(self._findAllocatedObjFromBuffer(obj)) return true;
			return false;
		]
	,	isFree[obj;
			if(self._findAvailableObj(obj)) return true;
			if(self._findAvailableObjFromBuffer(obj)) return true;
			return false;
		]
#ifdef orProfiler_STAGE;
	,	performProfiling[
				c o;
			objectloop(o in orAllocatedObjects && o ofclass orString) c++;
			orProfiler.trackMax(maxStrAlloc,c);
			c=0;
			objectloop(o in orAllocatedObjects && o ofclass orBufferWrapper) c++;
			orProfiler.trackMax(maxBufAlloc,c);
		]
	, 	displayProfileSummary[;
			print "^^[";
			print "Max buffers used:",orProfiler.maxBufAlloc,"/", orBufferPoolReserve;
			print "; Max strings used:",orProfiler.maxStrAlloc,"/",orStringPoolReserve;
			print "; Max buf length used:",orProfiler.maxBufLenUsed,"/",orStringDefaultSize;
			print "]";
			self._consistencyCheck();
		]
	, 	_consistencyCheck[o i c;
			objectloop(o in orAllocatedObjects && o ofclass orString){
				c=0;
				objectloop(i in orAllocatedObjects && i ofclass orString && o~=i){
					if(o.getBuffer(true)==i.getBuffer(true)) c++;
				}
				if(c>0) print "WARNING! Detected buffer shared between two allocated orString objects";
			}
		]
	,	getCount[type o retval;
				objectloop(o in orObjectHeap && o ofclass type) retval++;
				return retval;
			]
	, 	displaySummary[;
			print "[Available buffers:",self.getCount(orBufferWrapper), "; strings:",self.getCount(orString),"]";
			self._consistencyCheck();
		]
	, 	shortSummary[msg;
			new_line;
			print (string) msg;
			print "[b:",self.getCount(orBufferWrapper), "/s:",self.getCount(orString),"]";
			self._consistencyCheck();
		]
#endif;
	;

	array __dsbuf buffer orStringDefaultSize*orBufferPoolReserve; !--a big buffer which we chop up and assign as individual blocks to the memory manager
	class orBufferWrapper(orBufferPoolReserve) with buf 0; ! wrapper for buffers as objects, so we can move them around the object tree

	orInfExt with ext_initialise[
				i obj;
			for(i=0:orBufferWrapper.remaining()>0:i++){
				obj=orBufferWrapper.create();
				obj.buf=__dsbuf+(i*orStringDefaultSize);
				obj.buf=util.orBuf.convertToSizedBuffer(obj.buf,orStringDefaultSize);
				move obj to orObjectHeap;
			}

			for(i=0:orString.remaining()>0:i++){
				obj=orString.create();
				obj.getBuf(); !lets make sure that we've associated a buffer to all strings: getBuf will allocate one if not previously allocated
				move obj to orObjectHeap;
			}
		];

	!placeholders to keep track of allocated and available objects and their LIFO requirements.
	object orAllocatedObjects;
	object orObjectHeap;
	#endif; !AFTER_VERBLIB
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
