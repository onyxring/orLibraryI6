!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2014.05.01 orUnitTest
! A framework for unit tests.  Used to issue text to the parser, then validate the
! response.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License:
!--------------------------------------------------------------------------------------
! Three concepts are important in this implementation:
! 1. The orUnitTest class.   Use this to define a single test: configure any prerequisites for a
!   test, specify game input which will automatically play out without a user having to
!   enter it, and routines to interrogate the games responses and check for expected
!   results, reporting results.
! 2. The orUnitTestCollection which groups orUnitTests together, plays them out in sequence, and
!   aggregates their results.
! 3. The orUnitTestManager object which coordinates multiple test collections.
!--------------------------------------------------------------------------------------
! Revision History
! 2024.05.01	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orUnitTest_STAGE   0;
!--------------------------------------------------------------------------------------
! INCLUDED DEPENDENCIES
#include "orUtilBuf";
#include "orString";
#include "_orHookStandardLibrary";
#include "_orInfExt";
!TODO: this is broken
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orUnitTest_STAGE   ; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orUnitTest_STAGE   < LIBRARY_STAGE);
   #undef      orUnitTest_STAGE   ;
   Constant    orUnitTest_STAGE   LIBRARY_STAGE;
   #ifdef      orUnitTest_STAGE   ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orUnitTest..."; #endif;
!======================================================================================
! BEFORE  PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
    constant orStringDefaultSize	1000;  !we use big strings to capture and analyze game behavior
    default orUnitTestAutoRun true;
#endif;
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);

    orString _orUnitTestResponse; !the string object used to capture test responses

    !--the hooks into the standard library.
    orInfExt with  ext_displayStatus[; !--each time a turn is complete, give control to the Testing Framework to see what needs to be done
                orUnitTestManager.preInput();
                return false;
            ],
            ext_keyboardPrimitive[a_buffer a_table; !--we launch where we would normally getting user input
              if(orUnitTestManager.heartBeat(a_buffer, a_table)) return true; !--this will launch a test if there are any to play out.  If there are, then exit out, not providing the user the chance for input
              return false;
            ],
            ext_initialise[c;
              objectloop(c ofclass orUnitTestCollection) {
                #iftrue  (orUnitTestAutoRun == true);
                  c.queueAll();
                #endif;
              }
            ]
    ;

    object orUnitTestManager
      with  _currentCollection 0
      ,     successes 0
      ,     failures 0
      ,     briefMode 0
      !,     isSessionInProgress[; return self._currentCollection>0;]
      ,   preInput[t;
              t=self.getCurrentTest();

              if(t~=0 && t.state~=orUnitTestPerformingSetup) t.closeOut(); !if we are not looping through setup commands, then lets close this test out.

              if(self.areAllTestsComplete()) return;
              if(self._currentCollection && self._currentCollection.areAllTestsComplete()==true) self._currentCollection.printSummary();

              self.incrementCollection(); !if we just closed out the last test in the collection, move on to the next collection
              t=self.getCurrentTest(); !next test
              if(t==0) return; !no tests left

              if(t.state==orUnitTestReadyToRun) {
                if(t.hasUnplayedSetupCommands())
                  self.cPrint("^-- Setup...");
                else
                  self.cPrint("^-- Running Test...");
              }

              if(t.state==orUnitTestPerformingSetup && t.hasUnplayedSetupCommands()==false){
                print self.cPrint("^-- Running Test...");  !We've just finished performing a setup, now moving on to the test portion of our show...
              }

            ]
      ,     cPrint[t1 t2 t3;
                if(orUnitTestManager.briefMode==false) {
                  print (string)t1;
                  if(t2) print (string)t2;
                  if(t3) print (string)t3;
                }
            ]
      ,     heartBeat[a_buffer a_table;
              self.incrementCollection(); !has no effect if the current collection is still open
              if(self._currentCollection==0) return false;
              return self._currentCollection.heartbeat(a_buffer, a_table);
            ]
      ,     incrementCollection[c;
              objectloop(c ofclass orUnitTestCollection && c.areAllTestsComplete()==false) {
                if(self._currentCollection~=c){
                  self.cPrint("^========================================");
                  if(orUnitTestManager.briefMode==false)print "^== Collection: ~",(name)c,"~^----------------------------------";
                }
                self._currentCollection=c;
                return;
              }
              self._currentCollection=0; !--no collections left

              print "^=====================================^== TOTAL SUMMARY: ", self.successes," success(es); ",self.failures," failure(s)","^=====================================";
              new_line;

            ]
      ,     getCurrentTest[;
              if(self._currentCollection~=0) return self._currentCollection.getCurrentTest();
              return 0;
            ]

      ,     areAllTestsComplete[tc;
              objectloop(tc ofclass orUnitTestCollection && tc.areAllTestsComplete()==false) return false;
              return true;
            ]

    ;

    class orUnitTestCollection
      with  _currentTest 0
        , successes 0
        , failures 0
        , printSummary[;
            orUnitTestManager.successes=orUnitTestManager.successes+self.successes;
            orUnitTestManager.failures=orUnitTestManager.failures+self.failures;
            if(orUnitTestManager.briefMode==false) print "^-- Results: ",self.successes," success(es); ",self.failures," failure(s)";
          ]
        , areAllTestsComplete[t;
            objectloop(t in self && t ofclass orUnitTest && t.isPendingOrActive()==true) return false;
            return true;
          ]
         , getCurrentTest[
             t;
               if(self._currentTest~=0 && self._currentTest.isPendingOrActive()) return self._currentTest;
               objectloop(t in self && t ofclass orUnitTest && t.isPendingOrActive()) {
                 self._currentTest=t;
                 return t; !return the first found
               }
               return 0;
           ]
        , heartbeat[a_buffer a_table
                max t cmd;
              t=self.getCurrentTest();
              t.collection=self;

              max=a_buffer->0; !capture the max buffersize, usually 120 bytes, to restore it

              if(t.setup~=0) t.setup();

              cmd=t.getNextSetupCommand();
              if(cmd==0){
                cmd=t.command;
                t.state=orUnitTestPerformingCommand;
              }else{
                t.state=orUnitTestPerformingSetup;
              }

              util.orBuf.set(a_buffer, cmd);
              a_buffer->0=max;  !--orBuf actually writes the length in a single two bytes value in big endian order; but the user input buffer uses two single byte values.  byte zero is max buff size and byte one is actual length, which works out in this case.
              Tokenise__(a_buffer,a_table);

             if(t.state~=orUnitTestPerformingSetup) {
                if(orUnitTestManager.briefMode==false){
                  style bold;
                  util.orBuf.print(cmd);
                  style roman;
                  new_line;
                }
                _orUnitTestResponse.capture();
              }else{
                 if(orUnitTestManager.briefMode==false){
                  util.orBuf.print(cmd);
                  new_line;
                 }
              }
              return t;
          ]
        , queueAll[i;  objectloop(i in self && i ofclass orUnitTest) i.queue(); ]
        !, reportResults[; ]
    ;

    constant orUnitTestNotQueued         0;
    constant orUnitTestReadyToRun        1;
    constant orUnitTestPerformingSetup   2;
    constant orUnitTestPerformingCommand 3;
    constant orUnitTestComplete          4;

    class orUnitTest
      with  state orUnitTestNotQueued
      ,     setup 0
      ,     setupCommandPtr 0
      ,     setupCommands 0
      ,     command 0
      ,     collection 0
      ,     result -1
      ,     isPendingOrActive [;
              if(self.state==orUnitTestNotQueued) return false;
              if(self.state==orUnitTestComplete) return false;
              return true;
            ]
      ,     queue[;
              self.state=orUnitTestReadyToRun;
              self.setupCommandPtr=0;
              self.result=-1;
            ]
      ,     hasUnplayedSetupCommands[;
              if(self.&setupCommands-->0==0 || self.setupCommandPtr>=self.#setupCommands/WORDSIZE) return false;
              return true;
            ]
      ,     getNextSetupCommand[retval;
              if(self.hasUnplayedSetupCommands()==false) return 0;
              retval= self.&setupCommands-->self.setupCommandPtr;
              self.setupCommandPtr++;
              return retval;
            ]
      ,     closeOut[;
                _orUnitTestResponse.release();  !close out the capture
                if(orUnitTestManager.briefMode==false)_orUnitTestResponse.print(); !and print all of our input
                self.result=self.assess(_orUnitTestResponse); !and assess the output
                self.state=orUnitTestComplete;
          ]
      ,     assess[rsp;
              self.result=self.assert(rsp);
              if(orUnitTestManager.briefMode==false) "^RESULT: ";
              if(self.result>1){
                  if(orUnitTestManager.briefMode==false){
                    style bold;
                    print "Failure";
                    print". ";
                    print (string)self.result;

                    style roman;
                }
                 self.collection.failures++;
              }
              else{
                 if(orUnitTestManager.briefMode==false){
                  style bold;
                  print "Success";
                  style roman;
                  }
                 self.collection.successes++;
              }
              if(orUnitTestManager.briefMode==false) print "^----------------------------------";

              return self.result;
            ]
      ,     assert [;]
    ;


#endif; !--after Parser
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
