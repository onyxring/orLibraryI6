!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.04.09 orDeque
! A class for encapsulating the queues and stack data structure concepts.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!--------------------------------------------------------------------------------------
! License: Public Domain
!--------------------------------------------------------------------------------------
! This extension, classified as a "utility", provides tooling for other extensions to
! use and provides no specific features by itself.
!
! A class which wraps one or more property arrays, exposing the methods to treat them
! as lists, stacks, or queues.
!--------------------------------------------------------------------------------------
! Revision History
! 2004.02.28	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.04.10	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orDeque_STAGE 0;
!--------------------------------------------------------------------------------------
! INCLUDED DEPENDENCIES
#include "orUtilArray";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orDeque_STAGE	; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orDeque_STAGE < LIBRARY_STAGE);
   #undef      orDeque_STAGE ;
   Constant    orDeque_STAGE LIBRARY_STAGE;
   #ifdef      orDeque_STAGE ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orDeque..."; #endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	property individual arrayPropName;

	!TODO: manually track length.  it's likely zero is a valid entry in this case.
	class orDeque
		private _getProp[prop; if(prop==0) return self.arrayPropName; return prop;]
		!,	_insert[pos newOption prop; prop=self._getProp(prop); return util.orArray.insert(self, prop, pos, newOption);  ]
		!,	_get[n prop; prop=self._getProp(prop); return util.orArray.get(self,prop, n); ]
		!,	_set[n val prop; prop=self._getProp(prop); return util.orArray.set(self,prop, n, val); ]
		!,	remove[number prop; prop=self._getProp(prop); return util.orArray.remove(self, prop, number); ]
		with getLength[prop;
				prop=self._getProp(prop);
				return util.orArray.getLength(self,prop);
			]
		,	clear[prop; prop=self._getProp(prop); return util.orArray.clear(self, prop);  ]
 		!,	find[option prop; prop=self._getProp(prop);  return util.orArray.find(self,prop,Option); ]
		!,	removeValue[option prop; prop=self._getProp(prop); return util.orArray.removeValue(self, prop, option); ]
		,	enqueue[newOption prop; prop=self._getProp(prop); !--add an option to the end (to the first unused position)
				prop=self._getProp(prop);
				!self._insert(self.getLength(prop), newOption, prop);
				util.orArray.insert(self, prop, self.getLength(prop), newOption);
			]
		,	dequeue[prop; return self.pop(prop);] !--remove an option from the front (same as pop)
		,	push[newOption prop; !--add an option to the front
				prop=self._getProp(prop);
				!self._insert(0, newOption,prop);
				util.orArray.insert(self, prop, 0, newOption);
			]
		,	peek[prop !--look at an option in the front, withough dequeuing it
					retval;
				prop=self._getProp(prop);
				retval=util.orArray.get(self,prop, 0); !self._get(0,prop);
				return retval;
			]
		,	pop[prop !--remove an option from the front (same as dequeue)
					retval;
				prop=self._getProp(prop);
				!retval=self._get(0,prop);
				retval=util.orArray.get(self,prop, 0);
				util.orArray.remove(self, prop, 0);
				return retval;
			]
		,	popEnd[prop !--remove an option from the end;
					end retval;
				prop=self._getProp(prop);
				end=self.getLength(prop);
				if(end<1) return 0;
				retval=util.orArray.get(self,prop, end-1); !; !self._get(end-1,prop);
				!self.remove(end-1, prop);
				util.orArray.remove(self, prop, end-1);
				return retval;
			]
		,	peekEnd[prop !--look at an option from the end, without removing it
					end retval;
				prop=self._getProp(prop);
				end=self.getLength(prop);
				if(end<1) return 0;
				retval=retval=util.orArray.get(self,prop, end-1); !self._get(end-1,prop);
				return retval;
			]
		! ,	getRandom[prop
		! 			r;
		! 		prop=self._getProp(prop);
		! 		r=random(self.getLength(prop))-1;
		! 		return self.get(r,prop);
		! 	]
		! ,	swap[s f prop
		! 			t;
		! 		prop=self._getProp(prop);
		! 		t=self.get(s, prop);
		! 		self.set(s, self.get(f, prop), prop);
		! 		self.set(f,t,prop);
		! 	]
		! ,	randomize[prop
		! 			t;
		! 		prop=self._getProp(prop);
		! 		for(t=0:t<self.getLength(prop)*3:t++){
		! 			self.swap(random(self.getLength(prop)-1), random(self.getLength(prop))-1,prop);
		! 		}
		! 	]
	;
#endif; !--Before Parser
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE

