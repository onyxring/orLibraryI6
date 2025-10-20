!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.07.09 orUtil
!
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! This extension, classified as a "utility", provides tooling for other extensions to
! use and provides no specific features by itself.
!
!--------------------------------------------------------------------------------------
! Revision History
! 2024.07.09	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orUtil_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES

!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orUtil_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orUtil_STAGE  < LIBRARY_STAGE);
   #undef      orUtil_STAGE  ;
   Constant    orUtil_STAGE  LIBRARY_STAGE;
   #ifdef      orUtil_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orUtil..."; #endif;
!======================================================================================
! AFTER GRAMMAR
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
	object util with
#ifdef orUtilBuf_STAGE;  orBuf _orBuf,  #endif;
#ifdef orUtilChar_STAGE;  orChar _orChar,  #endif;
#ifdef orUtilStr_STAGE;  orStr _orStr,  #endif;
#ifdef orUtilMap_STAGE;  orMap _orMap,  #endif;
#ifdef orUtilNum_STAGE;  orNum _orNum,  #endif;
#ifdef orUtilParser_STAGE; orParser _orParser,  #endif;
#ifdef orUtilArray_STAGE; orArray  _orArray,  #endif;
#ifdef orUtilRef_STAGE;  orRef _orRef,  #endif;
#ifdef orUtilUi_STAGE; orUi _orUI,  #endif;
#ifdef orUtilLoopArray_STAGE; orLoopArray _orLoopArray,  #endif;
	;

#endif; !--AFTER Parser

!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
