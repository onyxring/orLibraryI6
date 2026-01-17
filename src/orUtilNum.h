!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.07.09 orNum
! Tools for managing property arrays.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! This extension, classified as a "utility", provides tooling for other extensions to
! use and provides no specific features by itself.
!
! It provides functions wrapped into helper objects.  Specifically:
!	orNum contains functions to manipulate property arrays.
!--------------------------------------------------------------------------------------
! Revision History
! 2024.07.09	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orUtilNum_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUtil";
#include "_orInfExt";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orUtilNum_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orUtilNum_STAGE  < LIBRARY_STAGE);
   #undef      orUtilNum_STAGE  ;
   Constant    orUtilNum_STAGE  LIBRARY_STAGE;
   #ifdef      orUtilNum_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orUtilNum...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
system_file;
#ifdef TARGET_ZCODE;
	constant orMinEnd $7fff;
	constant orUMinEnd $ffff;
	constant orMaxEnd $8000;
#ifnot;
	constant orMinEnd $7fffffff;
	constant orUMinEnd $ffffffff;
	constant orMaxEnd $80000000;
#endif;
	object _orNum
		with
			uIsGreaterThan[v1 v2; return (UnsignedCompare(v1, v2) == 1); ]
		,	uIsGreaterThanOrEqualTo[v1 v2; return (UnsignedCompare(v1, v2) == 1 or 0); ]
		,	uIsLessThan[v1 v2;  return (UnsignedCompare(v1, v2) == -1); ]
		,	uIsLessThanOrEqualTo[v1 v2;  return (UnsignedCompare(v1, v2) == -1 or 0); ]
		,	isBetween[v v1 v2 s;
				if(v<v2) {
					s=v1;
					v1=v2;
					v2=s;
				}
				if (v>=v1 && v<=v2) rtrue;
				rfalse;
			]
		,	uIsBetween[v v1 v2 s;
				if(self.uIsGreaterThan(v1,v2)) {
					s=v1;
					v1=v2;
					v2=s;
				}
				if (self.uIsGreaterThanOrEqualTo(v, v1) && self.uIsLessThanOrEqualTo(v,v2)) rtrue;
				rfalse;
			]
		,	pow[a b
					r;
				if(b==0) return 1;
				for(r=a,b=b-1:b>0:b--) r=r*a;
				return r;
			]
		,	getBit[val bit; return (val & util.orNum.pow(2,bit))~=0; ]
		,	isBitSet[val bit; return util.orNum.getBit(val,bit); ]
		,	setBitOn[val bit; return val | util.orNum.pow(2,bit); ]
		,	setBitOff[val bit; return val & (util.orNum.uMaxNumber - util.orNum.pow(2,bit)); ]
		,	xor[val1 val2; return (val1 | val2) & (~(val1 & val2)); ]
		,	max[val1 val2 val3 val4 val5
					retval;
				retval=val1; if(retval<val2) retval=val2;
				if(retval<val3) retval=val3;
				if(retval<val4) retval=val4;
				if(retval<val5) retval=val5;
				return retval;
			]
		,	min[val1 val2 val3 val4 val5
					retval;
				retval=val1;
				if(retval>val2) retval=val2;
				if(retval>val3) retval=val3;
				if(val3==orMinEnd) return retval;
				if(retval>val4) retval=val4;
				if(val4==orMinEnd) return retval;
				if(retval>val5) retval=val5;
				return retval;
			]
		,	umax[val1 val2 val3 val4 val5
					retval;
				retval=val1; if(self.uIsLessThan(retval,val2)) retval=val2;
				if(self.uIsLessThan(retval,val3)) retval=val3;
				if(self.uIsLessThan(retval,val4)) retval=val4;
				if(self.uIsLessThan(retval,val5)) retval=val5;
				return retval;
			]
		,	umin[val1 val2 val3 val4 val5
					retval;
				retval=val1;
				if(self.uIsGreaterThan(retval,val2)) retval=val2;
				if(self.uIsGreaterThan(retval,val3)) retval=val3;
				if(val3==orUMinEnd) return retval;
				if(self.uIsGreaterThan(retval,val4)) retval=val4;
				if(val4==orUMinEnd) return retval;
				if(self.uIsGreaterThan(retval,val5)) retval=val5;
				return retval;
			]
		,	maxNumber		$7fff
		,	uMaxNumber	$ffff
	;
	orInfExt with ext_initialise[; !because you can't #ifdef to change object members, outside of routine code.
		#ifdef TARGET_GLULX;
			_orNum.maxNumber=$7fffffff;
			_orNum.uMaxNumber=$ffffffff;
		#endif;
	];
	!These three lines are pulled from Roger Firth's Dump.h extension; modified only to account for glulx's expanded wordsize
	#ifdef TARGET_ZCODE;
		[hex x; print (hchar) (x & $FF00) / $100, (hchar) x & $00FF; ];
	#ifnot; !glulx
		[hex x; print (hchar) (x & $FF000000) / $1000000, (hchar) (x & $00FF0000)/$10000, (hchar) (x & $0000FF00) / $100, (hchar) x & $000000FF; ];
	#endif;
	[hchar x; print (hdigit) (x & $00F0) /$10, (hdigit) x & $000F; ];
	[hdigit x; if ((x = x%$10) < 10) print x; else print (char) x-10+'A'; ];

#endif; !--Before Parser

!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
