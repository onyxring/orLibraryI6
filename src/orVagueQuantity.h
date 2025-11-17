!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.08.10 orVagueQuantity
! This module allows multiple objects of the same class to be described with an vague
! adjective rather than a specific number.  For instance, "several gold coins" instead
! of the more exact "22 gold coins".
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------

!
!--------------------------------------------------------------------------------------
! Revision History
! 2024.08.10	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orVagueQuantity_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orInfExt";
#include "orUtilArray";
#include "_orHookWriteList";
#include "_orHookWriteAfterEntry";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orVagueQuantity_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orVagueQuantity_STAGE  < LIBRARY_STAGE);
   #undef      orVagueQuantity_STAGE  ;
   Constant    orVagueQuantity_STAGE  LIBRARY_STAGE;
   #ifdef      orVagueQuantity_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orVagueQuantity...";#endif;
!======================================================================================
! Before Parser
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
   attribute specificQuantity;
   property individual vagueQuantity;
#endif; !-before parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

   orInfExt orVagueQuantityController
      with  ext_sdIsOrAre[obj quantity
               idx;
            obj=self.getObj(obj);
            idx=self.findEntry(obj,quantity);

            if(idx<0) rfalse;

            if(util.orArray.getSize(obj,vagueQuantity) > idx+2 && util.orArray.get(obj,vagueQuantity,idx+2)==true) { !--if the forceSingular indicator was set then usurp the normal "are" behavior, and print "is"...
               if (_WAE_recurse_flag && (c_style & ENGLISH_BIT)) Tense(IS2__TX, WAS2__TX);
               rtrue;
            }
            rfalse;
         ],
         ext_wlrPluralMany[obj quantity
               idx;
            obj=self.getObj(obj,quantity);
            idx=self.findEntry(obj,quantity);
            if(idx<0) rfalse;

            print (string)util.orArray.get(obj, vagueQuantity, idx+1);
            return true;
         ],
         getObj[obj;
            if(obj provides vagueQuantity) return obj;
            return orVagueQuantityController;
         ],
         findEntry[obj quantity
               t ptr;
            if(parent(obj) has specificQuantity) return -1;
            if(obj has specificQuantity) return -1;

            ptr=-1;

            for(t=0:t<util.orArray.getSize(obj,vagueQuantity):t=t+2){
               if(util.orArray.get(obj,vagueQuantity,t)>quantity) break;
               ptr=t;

               if(util.orArray.getSize(obj,vagueQuantity)>t+2 && util.orArray.get(obj,vagueQuantity,t+2)<=1) t++;
            }

            return ptr;
         ],
         vagueQuantity 2 "a couple of" 3 "a few" 5 "several" 10 "numerous" 0 0 0 0 0 0 0 0 0 0; !default
#endif; !--After VERBLIB

!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
