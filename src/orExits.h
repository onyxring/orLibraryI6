!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.07.09 ORExits
! Implements the "Exits" verb functionality.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! Implements the "Exits" verb as well as a standalone routine for which does the
! same thing DescribeExits().
!--------------------------------------------------------------------------------------
! Revision History
! 2003.02.01	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.07.09	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orExits_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES

!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orExits_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orExits_STAGE  < LIBRARY_STAGE);
   #undef      orExits_STAGE  ;
   Constant    orExits_STAGE  LIBRARY_STAGE;
   #ifdef      orExits_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orExits..."; #endif;
!======================================================================================
! After Verblib
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
	property individual exitsText;
!TODO: convert text to a default message L__M...
	[DescribeExits qualifier ignoreexits_text count d save;
		if(parent(actor) provides exitsText && parent(actor).exitsText~=0 && ignoreexits_text==false){
			printorrun(parent(actor),exitsText,true); !--true, suppress new_line
			return;
		}
		count=0;
		objectloop(d in compass) if(metaclass(location.(d.door_dir))==object && location.(d.door_dir) hasnt concealed) count++;
		if(count==0) {
			print "There are no obvious exits from here.";
			return;
		}
		print "The only exit";
		if(count > 1) print"s";
		print " ";
		if(qualifier~=0) print (string)qualifier;

		if(count > 1)
			print "lay";
		else
			print "lies";
		save=count;
		objectloop(d in compass) if(metaclass(location.(d.door_dir))==object && location.(d.door_dir) hasnt concealed) {
			if(count<save && save>2) print ",";
			if(count==1 && save > 1) print " and";
			print" ",(LanguageDirection)(d.door_dir);
			count--;
		}
		print ".";
	];
#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

	Verb 'exits' * -> Exits;
	[ExitsSub;
		if(RunRoutines(actors_location,before)~=0) rtrue;
		DescribeExits();
		print "^";
	];

#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE