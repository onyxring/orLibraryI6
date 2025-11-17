!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.07.04 orMap
! orMap provides a utility object for managing world maps.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! This extension, classified as a "utility", provides tooling for other extensions to
! use and provides no specific features by itself.
!
! It provides functions wrapped into a utility object, specifically:
!  orMap contains functions to interrogate the world map.
!--------------------------------------------------------------------------------------
! Revision History
! 2024.07.04	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orUtilMap_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUtil";
#include "orUtilRef";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orUtilMap_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orUtilMap_STAGE  < LIBRARY_STAGE);
   #undef      orUtilMap_STAGE  ;
   Constant    orUtilMap_STAGE  LIBRARY_STAGE;
   #ifdef      orUtilMap_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orUtilMap...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
   object _orMap with
      reverseDirProp[d;
         switch(d){
         	in_to: d=out_to;
            out_to: d=in_to;
            u_to:d=d_to;
            d_to:d=u_to;
            n_to: d=s_to;
            s_to: d=n_to;
            e_to: d=w_to;
            w_to: d=e_to;
            nw_to: d=se_to;
            se_to: d=nw_to;
            ne_to: d=sw_to;
            sw_to: d=ne_to;
         }
         return d;
      ],
      reverseDirObj[d;
         switch(d){
            in_obj: d=out_obj;
            out_obj: d=in_obj;
            u_obj:d=d_obj;
            d_obj:d=u_obj;
            n_obj: d=s_obj;
            s_obj: d=n_obj;
            e_obj: d=w_obj;
            w_obj: d=e_obj;
            nw_obj: d=se_obj;
            se_obj: d=nw_obj;
            ne_obj: d=sw_obj;
            sw_obj: d=ne_obj;
         }
         return d;
      ]
      ,	getDirectionToAdjacentObject[start adjacentRoom
					d o; !---given an adjoining room, return the dir_obj the npc needs to travel to get there
 				start=self.locationOf(start);
            adjacentRoom=self.locationOf(adjacentRoom);
            objectloop(d in compass) { !---d is the direction object
 					o=start.(d.door_dir);
					if(o~=0 && (o==adjacentRoom ||(o provides door_to && util.orRef.resolveValue(o,door_to)==adjacentRoom))) return d;
 				}
				return 0;!not an adjacent room
 			]
		,  locationOf[a
               b;
            if(a==0) return 0;
            b = parent(a);

            while(b~=0){
               a=b;
               b=parent(a);
            }
            return a;
         ]
      , pathFinder _pathStub
   ;
   object _pathStub with determinePath[; #ifdef Debug; print "WARNING! Path features are not enabled.  Include ~_orMapPath~ to enable."; #endif; return -2;];


#endif; !--Before Parser
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
