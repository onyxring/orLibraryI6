!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.05.24 utParentChildRelationship
! Unit tests for the orParentChildRelationships extension.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
!
!--------------------------------------------------------------------------------------
! Revision History
! 2024.05.24	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        utParentChildRelationship_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orTest";
#include "orParentChildRelationship";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utParentChildRelationship_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utParentChildRelationship_STAGE  < LIBRARY_STAGE);
   #undef      utParentChildRelationship_STAGE  ;
   Constant    utParentChildRelationship_STAGE  LIBRARY_STAGE;
   #ifdef      utParentChildRelationship_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utParentChildRelationship...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);

#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);

#endif; !--After Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

    attribute under;
	attribute cover;

	#ifdef orParentChildRelationship_STAGE;

		orParentChildRelationship orPcrCover
				with childAttribute under,
					parentAttribute cover,
					preposition "under"
		;
		[PutUnderSub;  GenericInsertSub(##Insert, orPcrCover); ];
	#endif;

   	object orPcrRoom  "school hall" has light with description "As good a place as any.";

		object -> sink "sink" has  open container static with name 'sink', description "Free standing.";
		object -> -> book "book" with name 'book';

		object -> table "table" has  supporter static with name 'table', description "Round.";
		object -> -> lunchbox "lunchbox" with name 'lunchbox';

		object -> cubby "cubby" has cover supporter open container static with name 'cubby', description "Mounted on the wall.";

		object shoes "shoes" selfobj has pluralname with name 'shoes', description "a pair";

!----------------------------------------------------------------------------------------------------------------------------------------------------------
	   orTestCollection orPcrTests;
		  orTest ->  with setup[; location = orPcrRoom; move player to orPcrRoom;],
			command "look",
				assert[rsp;
					if(rsp.contains("sink (in which")==false) return "Standard container-only behavior failed.";
					if(rsp.contains("table (on which")==false) return "Standard supporter-only behavior failed.";
					if(rsp.contains("cubby (which is empty)")==false) return "Standard empty container behavior failed.";
					if(rsp.contains(") ,")||rsp.contains(")  ")) return "False space after closing parentheses.";
					if(rsp.contains("in which  is")) return "Extra space after WHICH.";
				];

		orTest ->  with setupCommands "take book" "take lunchbox" "put book in cubby" "put lunchbox on cubby",
			command "look",
		  	assert[rsp;
					if(rsp.contains("cubby (on which (in which are")) return "Standard library mishandling of container/supporter combo not addressed.";
					if(rsp.contains("; on which")==false) return "Correction for inline list of container/supporter contents not detected.";
					if(rsp.contains("in which  is")) return "Extra space after WHICH.";
				];
        orTest ->  with setup[;
				#ifdef orParentChildRelationship_STAGE;
					orPcrSupporter.includeContentsInExamine=false;
				#endif;
			],
		 	command "x cubby",
		  	assert[rsp;
				if(rsp.contains("is empty")) return "Full cubby shows as empty.";
				if(rsp.contains("In the cubby")==false) return "Does not display cubby's contained contents.";
				if(rsp.contains("On the cubby")) return "Displays supported contents during examination, when configured NOT to.";
		  	];
        orTest ->  with setup[;
				#ifdef orParentChildRelationship_STAGE;
					orPcrSupporter.includeContentsInExamine=false;
				#endif;
		],
		 	command "search cubby",
		  	assert[rsp;
				if(rsp.contains("is empty")) return "Full cubby shows as empty.";
				if(rsp.contains("In the cubby")==false) return "Search does not display cubby's contained contents";
				if(rsp.contains("On the cubby")==false) return "Search does not displays supported contents, when examine is configured NOT to.";
		  	];
		orTest ->  with setup[;
				#ifdef orParentChildRelationship_STAGE;
					orPcrSupporter.includeContentsInExamine=true;
				#endif;
			],
		 	command "x cubby",
		  	assert[rsp;
				if(rsp.contains("is empty")) return "Full cubby shows as empty.";
				if(rsp.contains("In the cubby")==false) return "Does not display cubby's contained contents.";
				if(rsp.contains("On the cubby")==false) return "Does not display supported contents during examination, when configured to.";
		  	];
        orTest ->  with setup[;
				#ifdef orParentChildRelationship_STAGE;
					orPcrSupporter.includeContentsInExamine=true;
				#endif;
			],
		 	command "search cubby",
		  	assert[rsp;
				if(rsp.contains("is empty")) return "Full cubby shows as empty.";
				if(rsp.contains("In the cubby")==false) return "Search does not display cubby's contained contents";
				if(rsp.contains("On the cubby")==false) return "Search does not displays supported contents, when examine configured to.";
		  	];


#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

	#ifdef orParentChildRelationship_STAGE;
		Extend 'put' * multiexcept 'under' noun              -> PutUnder;
	#endif;

#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
