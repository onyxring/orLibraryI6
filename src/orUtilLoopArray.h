!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.11.17 orUtilLoopArray
! A template for extensions.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
!
!--------------------------------------------------------------------------------------
! Revision History
! 2024.11.17	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orUtilLoopArray_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUtil";
#include "orUtilArray";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orUtilLoopArray_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orUtilLoopArray_STAGE  < LIBRARY_STAGE);
   #undef      orUtilLoopArray_STAGE  ;
   Constant    orUtilLoopArray_STAGE  LIBRARY_STAGE;
   #ifdef      orUtilLoopArray_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orUtilLoopArray...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
constant repeatLast 2;

object _orLoopArray
   private _get[obj prop onceOnly method;
         if(self.isComplete(obj,prop)){ !here we handle the scenarios where we've already cycled through all of the items...
            switch(onceOnly){
               false: util.orArray.set(obj,prop,0,0); !clear the tracker
               repeatLast: return self._trackAndResolve(obj,prop, util.orArray.getSize(obj, prop)-2); ! -1, because the tracker elements increases array size by 1; another -1 because this is expecting a zero-based index
               default: return -1;
            }
         }
         return self._trackAndResolve(obj,prop, self.method(obj,prop,1));
      ]
   ,  _trackAndResolve [obj prop ndx !--ndx is zero-based index
               t;
            t=util.orArray.get(obj,prop,0) | util.orNum.pow(2,ndx); !--for the purposes of building a mask, we leave ndx as a zero-based number
            util.orArray.set(obj,prop,0,t);
            return util.orArray.get(obj,prop,ndx+1);!--ndx is zero-based. We add one, because the first element is the tracker
      ]
   ,  _countUnusedIndexes[obj prop
            t c mask trkr;
         trkr=util.orArray.get(obj,prop,0);
         mask=1;

         for(t=0:t<util.orArray.getSize(obj,prop)-1:t++){
            if(trkr&mask==0) c=c+1;
            mask=mask*2; !next bit
         }
         return c;
         !print util.orNum.pow(2,);
      ]
   ,  _getUnusedIndex[obj prop ndx
            mask trkr pos;
         trkr=util.orArray.get(obj,prop,0);
         mask=1;
         for(pos=0:ndx>0 && pos<util.orArray.getSize(obj,prop)-1:pos++){
            if(trkr&mask==0) ndx--;
            if(ndx==0)return pos;
            mask=mask*2; !next bit
         }
         return -1;
      ]
   ,  _getRandomUnusedIndex[obj prop
            r;
         r=random(self._countUnusedIndexes(obj,prop));
         return self._getUnusedIndex(obj, prop, r);
      ]
   with getNext[obj prop onceOnly;
         return self._get(obj, prop, onceOnly, _getUnusedIndex);
      ]
   ,  getAny[obj prop onceOnly;
         return self._get(obj, prop, onceOnly, _getRandomUnusedIndex);
      ]
   ,  isComplete[obj prop; return self._countUnusedIndexes(obj,prop)==0; ]
;

#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);

#endif; !--After Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
