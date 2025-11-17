!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.07.09 orArray
! Tools for managing property arrays.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! This extension, classified as a "utility", provides tooling for other extensions to
! use and provides no specific game elements by itself.
!
! orArray provides routines wrapped together in the orArray member of the util object.
! These routines all support manipulation of WORD arrays, either TABLE arrays or
! property arrays.  All routines are built with zero-based indexing, regardless of the
! underlying array type.
!--------------------------------------------------------------------------------------
! Revision History
! 2024.07.09	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orUtilArray_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUtil";

!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orUtilArray_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orUtilArray_STAGE  < LIBRARY_STAGE);
   #undef      orUtilArray_STAGE  ;
   Constant    orUtilArray_STAGE  LIBRARY_STAGE;
   #ifdef      orUtilArray_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orUtilArray...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
constant orArrayNoProp = 0;
object _orArray
		with clear[obj prop !--remove everything from the list
					t;
				if(metaclass(obj)~=object && prop ~= orArrayNoProp)
					prop = orArrayNoProp;

				for(t=0:t<self.getSize(obj, prop):t++) self.set(obj,prop,t,0);
			]
		, 	getSize[obj prop;
				if(metaclass(obj)==object){
					return obj.#prop/WORDSIZE;
				}else{
					return obj-->0;
				}
			]
		,	getLength[obj prop
					t sz;

				if(metaclass(obj)~=object && prop ~= orArrayNoProp)
					prop = orArrayNoProp;

				sz=self.getSize(obj,prop);

				for(t=0:t<sz:t++){
					if(util.orArray.get(obj,prop,t)==0) return t;
				}
				return sz;
			]
		!TODO: test this
		! ,	countWhere[obj prop condition !--count all members where the passed in condition is true
		! 			t sz count;

		! 		if(metaclass(obj)~=object && prop ~= orArrayNoProp) {
		! 			condition=prop;
		! 			prop = orArrayNoProp;
		! 		}

		! 		sz=self.getSize(obj,prop);

		! 		for(t=0:t<sz:t++){
		! 			if(condition(obj,prop,get(obj, prop, t))) count++;
		! 		}
		! 		return count;
		! 	]
		,	get[obj prop n;

				if(metaclass(obj)~=object && prop ~= orArrayNoProp) {
					n=prop;
					prop = orArrayNoProp;
				}

				if(util.orArray.getSize(obj,prop)==0) "Error (orArray): ",(property)prop," array is empty.";
				if(n>=util.orArray.getSize(obj, prop)) return -1; !"Error (orArray): get index (",n,") out of range. (",util.orArray.getSize(obj,prop)," item max)^";
				if(metaclass(obj)==object){
					return obj.&prop-->n;
				}else{
					return obj-->(n+1);
				}
			]
		,	set[obj prop n val;
				if(metaclass(obj)~=object && prop ~= orArrayNoProp) {
					val=n;
					n=prop;
					prop = orArrayNoProp;
				}

				if(util.orArray.getSize(obj, prop)==0) "Error (orArray): ",(property)prop," array is empty.";

				if(n>=util.orArray.getSize(obj, prop)) "Error (orArray): ",(property)prop," set index (",n,") out of range. (",util.orArray.getSize(obj, prop)," item max)^";
				if(metaclass(obj)==object){
					obj.&prop-->n=val;
				}else{
					return obj-->(n+1)=val;
				}
			]
		,	find[obj prop val
					t;
				if(metaclass(obj)~=object && prop ~= orArrayNoProp) {
					val=prop;
					prop = orArrayNoProp;
				}

				for(t=0:t<self.getSize(obj,prop):t++){
					if(self.get(obj,prop,t)==val) return t;
				}

				return -1;
			]
		!TODO: test this
		! ,	findWhere[obj prop condition !--the first index where the passed in condition is true
		! 			t sz;

		! 		if(metaclass(obj)~=object && prop ~= orArrayNoProp) {
		! 			condition=prop;
		! 			prop = orArrayNoProp;
		! 		}

		! 		sz=self.getSize(obj,prop);

		! 		for(t=0:t<sz:t++){
		! 			if(condition(obj,prop,get(obj, prop, t))) return t;
		! 		}
		! 		return -1;
		! 	]
		,	remove[obj prop pos  !--eliminate a position and shift everything forward
					t sz;
				if(metaclass(obj)~=object && prop ~= orArrayNoProp) {
					pos=prop;
					prop = orArrayNoProp;
				}
				sz=self.getSize(obj,prop)-1;
				for(t=pos:t<sz:t++)
					self.set(obj,prop,t, self.get(obj,prop,t+1));

				self.set(obj,prop,sz,0);
			]
		, 	removeValue[obj prop val  !--Locate an option and remove it
					pos;
				if(metaclass(obj)~=object && prop ~= orArrayNoProp) {
					val=prop;
					prop = orArrayNoProp;
				}

				pos=util.orArray.find(obj, prop,val);

				while(pos~=-1) {
					util.orArray.remove(obj, prop, pos);
					pos=util.orArray.find(obj, prop,val);
				}
			]
		!TODO: test this
		! , 	removeWhere[obj prop condition  !--remove all entries where the passed in condition is true
		! 			pos numRemoved;

		! 		if(metaclass(obj)~=object && prop ~= orArrayNoProp) {
		! 			condition=prop;
		! 			prop = orArrayNoProp;
		! 		}

		! 		pos=util.orArray.findWhere(obj, prop,condition);

		! 		while(pos~=-1) {
		! 			numRemoved++;
		! 			util.orArray.remove(obj, prop, pos);
		! 			pos=util.orArray.findWhere(obj, prop,condition);
		! 		}
		! 		return numRemoved;
		! 	]
		,	insert[obj prop pos val  !--add an option and associated value to the first position
					 t;
				if(metaclass(obj)~=object && prop ~= orArrayNoProp) {
					pos=val;
					val=prop;
					prop = orArrayNoProp;
				}

				for(t=self.getSize(obj,prop)-1:t>pos:t--) self.set(obj,prop,t,self.get(obj,prop,t-1)); ! expand out for more space
				self.set(obj,prop,pos,val);
				rtrue;
			]
		,	append[obj prop val;
				if(self.getSize(obj,prop)==self.getLength(obj,prop)){
					"Error (orArray): ",(property)prop," array is out of space.  Cannot append new value.";
				}
				self.insert(obj,prop, self.getLength(obj,prop), val);
				rtrue;
			]
		,	prepend[obj prop val;
				if(self.getSize(obj,prop)==self.getLength(obj,prop)){
					"Error (orArray): ",(property)prop," array is out of space.  Cannot prepend new value.";
				}
				self.insert(obj,prop, 0, val);
				rtrue;
			]
	;
#ifdef DEBUG;
	[debugDumpArray obj prop
			t sz o;

		if(obj==0) return;
		print "^dump:";
		!print (name)obj,"(",obj,")";

		sz= util.orArray.getLength(obj,prop);
		if(sz>100) return;

		for(t=0:t<sz:t++){
			print ":",t,"-> ";
			o= util.orArray.get(obj,prop,t);
			print o;
			if(util.orRef.isObject(o))	print "(",(name) o,")";

		}
	];
#endif;

#endif; !--Before Parser

!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
