!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorStateDescriptors
! Unit Tests for the utorStateDescriptors orLibrary extension.
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
#ifndef        utorStateDescriptors_STAGE;
default        utorStateDescriptors_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orStateDescriptors";
!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        utorStateDescriptors_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorStateDescriptors_STAGE  < LIBRARY_STAGE);
   #undef      utorStateDescriptors_STAGE  ;
   Constant    utorStateDescriptors_STAGE  LIBRARY_STAGE;
   #ifdef      utorStateDescriptors_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorStateDescriptors...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
    attribute blessed;
    object utorStateDescriptorsStart "orStateDescriptors Room" with description "Where orStateDescriptors tests are run.";

    object -> box "magic box" with name 'magic' 'box',
    description "The box is impossible to ignore." 
    has container openable light blessed;

    class marbleSD
	with	name 'marble' 'marbles//p',
			short_name "marble",
			plural "marbles";

    Object bowlSD "bowl" utorStateDescriptorsStart
	with	name 'bowl'
	has		open container;

        MarbleSD "" bowlSD;
        MarbleSD "" bowlSD;
        MarbleSD "" bowlSD;
        MarbleSD "" bowlSD;
        MarbleSD "" bowlSD;
        MarbleSD "" bowlSD;

    [sdBlessed obj suppress;
        if(suppress==false && obj has blessed) 
            print "glowing with purity";
        return obj has blessed;
    ];

    orUnitTest "utorStateDescriptors" 
        with tests [; 
            orStateDescriptors.addRoomDescriptionHandler(sdBlessed);
            orStateDescriptors.addInventoryHandler(sdBlessed);
            PlayerTo(utorStateDescriptorsStart, 3); 
            playerCommands.pushCommand("look");
            playerCommands.pushCommand("take box");
        ]  "(closed, providing light and glowing with purity)" 
        "i" "(closed, providing light and glowing with purity)"  
        "l" "!(which is empty)"
        [; 
            orStateDescriptors.removeRoomDescriptionHandler(sdLight);
            playerCommands.pushCommand("drop box");
        ] noTest 
        "look" "(closed and glowing with purity)"   
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
