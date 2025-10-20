!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! 2024.03.08 #orExtensionFramework.h
!!!	The Extension Management Framework for orLibrary.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
! This work is licensed under the Creative Commons Attribution-ShareAlike License
! (http://creativecommons.org/licenses/by-sa/1.0/).
!--------------------------------------------------------------------------------------
! This extension framework is the foundation upon which the orLibrary is built.  It
! contains the plumbing used by all orLibrary extensions to ensure extensions are
! automatically added in the right places, the right number of times and that
! dependencies are included appropriately.
!
! Typically, you do not need to include this file explicitly.  As long as you #include
! the #orParser, #orVerbLib, and #orGrammar wrappers in place of the parser, verblib,
! and grammar files, it will be pulled in for you.
!
! Registering extensions with this framework is done by adding entries to
! the #orExtensionRegistry.h file.
!--------------------------------------------------------------------------------------
system_file;
!======================================================================================
! orLibrary Versioning
!======================================================================================
default orLibVersion "2.0b";
default orLibDateStamp "2025.09.28";
!======================================================================================
default orXFErrorInclude "ERROR: orExtensionFramework extensions must only be #Included once, before including '#orParser'.";
!--Do some checking to make sure things are included in the right order; notify of detected sequencing issues.
#ifndef orExtensionFramework_STAGE; !-- this is the first time we've ever included this file
!-- Define default extensions here ----------------------------------------------------
    #include "_orHookBanner";
!--------------------------------------------------------------------------------------
    #ifndef VN_1633; message "WARNING: orExtensionFramework was developed against the Inform 6 compiler version 6.43. Although it might work with some previous versions, this has not been tested."; #endif;
    #ifndef LIBRARY_STAGE;          !--if parser has not yet been included
        Default BEFORE_PARSER 10;   !--temporarily define the stage constants, since modules reference them before they are actually defined
        Default AFTER_PARSER  20;
        Default AFTER_VERBLIB 30;
        Default AFTER_GRAMMAR 40;
        Default orExtensionFramework_STAGE BEFORE_PARSER; !--set the orLibrary stage to "BEFORE_PARSER", even though it hasn't been defined yet
        Default LIBRARY_STAGE BEFORE_PARSER; !--we set this so extensions will know the stage, even though parser hasn't been included yet to define these
       	#ifndef orExtensionFrameworkBrief; message "orExtensionFramework: pre-PARSER section."; #endif;
        #include "#orExtensionRegistry"; !--include all registered modules
        #ifndef orExtensionFrameworkBrief; message "----"; #endif;
        #undef BEFORE_PARSER;  !--now that extensions have used the stage constants, lets tear them down, so parser doesn't error out when it tries to define them officially
        #undef AFTER_PARSER;
        #undef AFTER_VERBLIB;
        #undef AFTER_GRAMMAR;
        #undef LIBRARY_STAGE;
    #ifnot; !--misconfig detected.
        message fatalerror "ERROR: orExtensionFramework was not included before parser. Using the #Parser, #Verblib, and #Grammar wrappers typically takes care of this for you.";
    #endif;
#ifnot; !-- included after parser, at least
    #ifndef LIBRARY_STAGE; !--misconfig detect.
        message "WARNING: orExtensionFramework was included multiple times before parser. You typically do not need to include #orExtensionFramework yourself.  Using the #orParser, #orVerblib, and #orGrammar wrappers typcially takes care of this for you.  Ignoring.";
    #ifnot;
            #iftrue(LIBRARY_STAGE == AFTER_PARSER); !--throw some warnings if we are using earlier versions of the standard library or compiler
                #Iftrue (LIBRARY_VERSION < 612); message "WARNING: orExtensionFramework was developed against the Inform 6 standard library version 6.12. Although it might work with some previous versions, this has not been tested."; #endif;
            #endif;
            #iftrue (orExtensionFramework_STAGE == LIBRARY_STAGE); !--misconfig detected
            message "WARNING:The orExtensionFramework has been included more than once between standard library inclusions.  You typically do not need to include #orExtensionFramework yourself.  Using the #orParser, #orVerblib, and #orGrammar wrappers typcially takes care of this for you.  Ignoring.";
        #ifnot; !-- Confirmed that we are not trying to include orExtensionFramework multiple times in the same place
            #iffalse (orExtensionFramework_STAGE == LIBRARY_STAGE -10); !--misconfig detected
                message fatalerror "ERROR: Missing an inclusion detected.  The orExtensionFramework must be included four times: 1. before parser, 2. after parser; 3. after verblib; 4. after grammar.  Using the #orParser, #orVerblib, and #orGrammar wrappers typcially takes care of this for you.";
            #ifnot; !--Confirmed that sequencing is all good
                !--We're good to go...
                #ifndef orExtensionFrameworkBrief;
                    #iftrue (LIBRARY_STAGE == AFTER_PARSER); message "orExtensionFramework: AFTER PARSER section.";  #endif;
                    #iftrue (LIBRARY_STAGE == AFTER_VERBLIB); message "orExtensionFramework: AFTER VERBLIB section.";  #endif;
                    #iftrue (LIBRARY_STAGE == AFTER_GRAMMAR); message "orExtensionFramework: AFTER GRAMMAR section.";  #endif;
                #endif;
                #include "#orExtensionRegistry"; !--include all registered modules
                #ifndef orExtensionFrameworkBrief;
                    message "----"; !--a little mark to separate extension messages from those generated in the game authors code
                #endif;
                #undef orExtensionFramework_STAGE; !--reset our pass tracker so we know what we've done
                Constant orExtensionFramework_STAGE = LIBRARY_STAGE;
            #endif;
        #endif;
    #endif;
#endif;