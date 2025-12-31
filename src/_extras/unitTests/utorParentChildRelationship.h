!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorParentChildRelationship
! Unit Tests for the utorParentChildRelationship orLibrary extension.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! Revision History
! 2024.05.06	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        utorParentChildRelationship_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orParentChildRelationship";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utorParentChildRelationship_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorParentChildRelationship_STAGE  < LIBRARY_STAGE);
   #undef      utorParentChildRelationship_STAGE  ;
   Constant    utorParentChildRelationship_STAGE  LIBRARY_STAGE;
   #ifdef      utorParentChildRelationship_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorParentChildRelationship...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

   !--- section 1: Listing Children During Examination
   object utorParentChildRelationshipStart "orParentChildRelationship Room" has light 
      with description "Where orParentChildRelationship tests are run."
      ,  w_to pcrRoom2;

   object -> bowl_pcr "bowl" has open container with name 'bowl', description "Can contain things.";
   object -> -> apple "apple" with name 'apple';  

   object -> table_pcr "table" has supporter with name 'table'; 
   object -> -> book_pcr "book" with name 'book';

   !--- section 1b: Suppressing Container Listings by Parent
   object pcrRoom2 "second pcrRoom" has light with description "pcr room 2", w_to pcrRoom3; 
      object -> mysteriousBasin "basin" has open container 
      with name 'basin', 
      description[; print "The basin is mysterious, its contents unclear."; rfalse;];

      object -> -> orange_pcr "orange" with name 'orange';

      object -> bowl2_pcr "bowl" has open container with name 'bowl',
      description "Can contain things.";

      object -> -> apple_pcr "apple" with name 'apple';
   
   !--- section 2: Simultaneous Containers/Supporters
   object pcrRoom3 "third pcrRoom" has light with description "pcr room 3"; 
      object -> cubby "cubby" with name 'cubby' has supporter open container; 
         object -> -> book2_pcr "book" with name 'book'; 
         object -> -> lunchbox "lunchbox" with name 'lunchbox'; 

   !--- section 3: Creating New Relationships
      attribute under;
      attribute cover;

      orParentChildRelationship orPcrCover
         with 	childAttribute under
         ,  parentAttribute cover
         ,  preposition "under"
      ;
      
      object shoes "shoes" with name 'shoes' has pluralname under; 
   !----------------------------------------------------------
   orUnitTest "utorParentChildRelationship" 
      with tests [; 
         PlayerTo(utorParentChildRelationshipStart, 3); 
         orPcrSupporter.includeContentsInExamine=true; 
         orPcrContainer.includeContentsInExamine=false; 
      ] noTest
      "x table" "is a book."
      "x bowl" "!is an apple."
      [; 
         orPcrContainer.includeContentsInExamine=true; 
         playerCommands.pushCommand("w");
         playerCommands.pushCommand("x bowl"); !TODO? should this be deleted?
         playerCommands.pushCommand("x basin");
      ] "is mysterious"
      "w" "!(on which (in which"
      "take all from cubby. put book on cubby. insert lunchbox into cubby. x cubby" [val retval; 
                                                                                       retval=retval+self.assertContains("On the cubby is a book");
                                                                                       retval=retval+self.assertContains("In the cubby is a lunchbox");
                                                                                       return retval==2;
                                                                                    ]
      [; give cubby cover; 
         move shoes to cubby; 
         playerCommands.pushCommand("look");
      ] "under which are some shoes"
      "put book under cubby" "You put the book under the cubby."
   ;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
