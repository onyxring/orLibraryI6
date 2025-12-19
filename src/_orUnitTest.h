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
  #include "orString";
  #include "orPlayerCommandQueue";
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
  default orStringDefaultSize	2000;  !we use big strings to capture and analyze game behavior
  global testResult;
  global tmpCatch;

  global currentTest 0;
  global success 0;
  global failed 0;

  class orUnitTest 
  with  isComplete[;return (self.ptr>=util.orArray.getSize(self, test));]
      , runSetup[
            val;
          val=util.orArray.get(self,setup,self.ptr);
          if(metaclass(val)==routine) return val();
          return playerCommands.push(val);          
        ]
      , validate[str
            val retval;
          self.curResult=str;
          val=util.orArray.get(self,test,self.ptr);
          if(val==0) return -1;
          if(metaclass(val)==routine) return val(str);
          
          val = util.orStr.new(val); !--ensure its a string 
          if(val.getChar(0)=='!') {
            retval = self.assertDoesNotContain(val.mid(1,999));
            val.free();
            return retval;
          }
          val.free();
          return self.assertContains(val);          
      ]
      , curResult 0
      , ptr 0
      , setup[;]
      , test[str; rtrue;]
      , assertContains[val; 
          print "^    Contains ~"; util.orStr.print(val); print "~: ";
          if(self.curResult.contains(val)) {
            print "pass.";
            rtrue;
          }
          print "FAILED.";
          self.printResponse();
          rfalse;
      ]
      , assertDoesNotContain[val; 
          print "^    Does NOT contain ~"; util.orStr.print(val); print "~: ";
          if(self.curResult.contains(val)==false) {
            print "pass.";
            rtrue;
          }
          print "FAILED.";
          self.printResponse();
          rfalse;
      ]
      , printResponse[;
        print "^-- -- -- -- -- -- -- -- -- -- -- -- -- -- --^";
        self.curResult.print();
        print "^-- -- -- -- -- -- -- -- -- -- -- -- -- -- --^";
      ]
     
;

#endif; !--before parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);

  object _orTestHooks LibraryExtensions 
    with ext_initialise[; 
          tmpCatch=util.orStr.new();
          testResult=util.orStr.new();
        ]  
      , ext_afterprompt[; !--this runs right before the status line is printed; before player input
          tmpCatch.release(); !--stop capturing so the status line isn't captured too.
          testResult.set(testResult.append(tmpCatch)); !--add the input this turn to our running memory, since restarting the capture after the status line will reset our capture buffer
        ]
      , ext_keyboardPrimitiveNotify[ c t; !--this runs just before player input
          if(playerCommands.getLength()>0){
            tmpCatch.capture(); 
          }
          rfalse;
        ]
    ;

#endif; !--after Parser
!======================================================================================
! AFTER GRAMMAR
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);
        verb meta 'utrun' *-> utRun;
        verb meta 'utFinish' *-> utFinish;
        
        [utRunSub 
              t count result;
            tmpCatch.release(); !--stop capturing output 
            testResult.set(testResult.append(tmpCatch)); !--add the output to the results accumulated so far

            if(currentTest~=0){ 
              
              result=currentTest.validate(testResult);
              switch(result){
                false:
                  failed++;
                true:
                  success++;
              }
              testResult.set("");
              currentTest.ptr++;
            }
            objectloop(t ofclass orUnitTest && t.isComplete()==false){
              currentTest=t;
              
              if(currentTest.ptr==0) print "^Running test ~",(name) currentTest,"~...";
              tmpCatch.capture();!--allowing setup to actually produce test case output
              t.runSetup();
              playerCommands.pushCommand("utRun", true, true);
              return;
            }
            playerCommands.pushCommand("utFinish", true, true);
        ];
        
        [utFinishSub ;
          tmpCatch.release(); 
          print "^----------------------------------------------------------------------------------";
          print "^Tests complete: ",success,"/",success+failed," succeeded; ",failed," FAILED.";
        ];
#endif; !--after Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
