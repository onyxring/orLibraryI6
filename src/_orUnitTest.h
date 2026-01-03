!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.15 orUnitTest
! A framework for unit tests.  Used to issue text to the parser, then validate the
! response.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License:
!--------------------------------------------------------------------------------------
! Revision History
! 2025.12.15	Initial Creation
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
  Constant noTest -1;

  global testResult;    !a place to capture the results of the current running test
  global tmpCatch;      !a place to capture the results of the current single step of the current running test
  global currentTest 0; !the current test being run
  global success 0;     !number of total successes 
  global failed 0;      !number of total failures

  class orUnitTest 
    with  isComplete[;return (self.ptr>=self.getTotalTests());]
      ,   getNumTestsForProperty[prop; return util.orArray.getLength(self,prop)/2; ]
      ,   getPropFromIndex[i
              count; 
            
            count=self.getNumTestsForProperty(tests); 
            if(i<count) return tests;
            
            count=count+self.getNumTestsForProperty(tests1); 
            if(i<count) return tests1;
            
            count=count+self.getNumTestsForProperty(tests2); 
            if(i<count) return tests2;
            
            count=count+self.getNumTestsForProperty(tests3); 
            if(i<count) return tests3;

            count=count+self.getNumTestsForProperty(tests4); 
            if(i<count) return tests4;

            print "ERROR getPropFromIndex: parameter value (",i,") exceeds the total number of tests defined (",count,")";
            return -1;
          ]
      ,   getPropIndexFromIndex[i;
            if(i<self.getNumTestsForProperty(tests)) return i*2;
            i=i-self.getNumTestsForProperty(tests);
            
            if(i<self.getNumTestsForProperty(tests1)) return i*2;
            i=i-self.getNumTestsForProperty(tests1);

            if(i<self.getNumTestsForProperty(tests2)) return i*2;
            i=i-self.getNumTestsForProperty(tests2);

            if(i<self.getNumTestsForProperty(tests3)) return i*2;
            i=i-self.getNumTestsForProperty(tests3);

            if(i<self.getNumTestsForProperty(tests4)) return i*2;
            
            print "ERROR getPropIndexFromIndex: parameter exceeds the total number of tests defined by (",i-self.getNumTestsForProperty(tests4),")";
            return -1;
          ]
      
      ,   getTotalTests[count;
            count=util.orArray.getLength(self,tests)/2;
            count=count+util.orArray.getLength(self,tests1)/2;
            count=count+util.orArray.getLength(self,tests2)/2;
            count=count+util.orArray.getLength(self,tests3)/2;
            count=count+util.orArray.getLength(self,tests4)/2;
            return count;
          ]
      ,   runSetup[
            val;
          val=util.orArray.get(self,self.getPropFromIndex(self.ptr),self.getPropIndexFromIndex(self.ptr));
          if(metaclass(val)==routine) return val();
          return playerCommands.push(val);          
        ]
      , validate[str
            val retval;
          if(str.lower().contains("error",true)){
              print "^Found ERROR in response...";
              self.printResponse();
              rfalse;
          }
          self.curResult=str;
          val=util.orArray.get(self,self.getPropFromIndex(self.ptr),self.getPropIndexFromIndex(self.ptr)+1);
          if(val==noTest) return -1; !-- -1: neither pass nor fail, don't count this in the final tally
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
      , tests 0 0 
      , tests1 0 0 
      , tests2 0 0 
      , tests3 0 0 
      , tests4 0 0 
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
    with areUnitTestsRunning false
    , ext_initialise[; 
          tmpCatch=util.orStr.new(); !--these are never freed, since the opposite of initialize
          testResult=util.orStr.new(); !--   would be game shutdown (and that clears out everything)
          #ifdef orUnitTestAutoRun;
            playerCommands.pushCommand("utrun");
          #endif;
        ]  
      , ext_afterprompt[; !--this runs right before the status line is printed; before player input
          tmpCatch.release(); !--stop capturing so the status line isn't captured too.
          testResult.set(testResult.append(tmpCatch)); !--add the input this turn to our running memory, since restarting the capture after the status line will reset our capture buffer
        ]
      , ext_keyboardPrimitiveNotify[ c t; !--this runs just before player input
          if(_orTestHooks.areUnitTestsRunning==true && playerCommands.getLength()>0){
             tmpCatch.capture();              
          }
        ]
    ;

#endif; !--after Parser
!======================================================================================
! AFTER GRAMMAR
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);
        verb meta 'utrun' *-> utRun;
        verb meta 'utFinish' *-> utFinish;
        
        [utRunSub 
              t result;
              
            if(playerCommands.getLength()>0){ !--Normally, utRun occurs after the unit test commands have run; however, if one of the commands adds more commands to the playerCommands queue, then let's just ignore this well-intentioned utRun call and add a new one to the queue
              playerCommands.pushCommand("utRun", true, true);
              return;
            }
            _orTestHooks.areUnitTestsRunning=true;
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
            
            !--despite being an objectloop, this only runs agaist the first test then returns
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
          _orTestHooks.areUnitTestsRunning=false;
          tmpCatch.release(); 
          
          print "^----------------------------------------------------------------------------------";
          print "^Tests complete: ",success,"/",success+failed," succeeded; ",failed," FAILED.";
        ];

        
#endif; !--after Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
