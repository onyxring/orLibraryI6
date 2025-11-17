!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.04.17 orPrint
! Inline printing using $notation.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License:
!--------------------------------------------------------------------------------------
! Revision History
! 2024.04.17	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orPrint_STAGE  0 ;
!--------------------------------------------------------------------------------------
! INCLUDED DEPENDENCIES
#include "orString";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orPrint_STAGE	; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orPrint_STAGE  	< LIBRARY_STAGE);
   #undef      orPrint_STAGE  	;
   Constant    orPrint_STAGE  	LIBRARY_STAGE;
   #ifdef      orPrint_STAGE  	; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orPrint...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	!replace PrintOrRun;
#endif; !BEFORE PARSER
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
	!special includes, not typical for the Extension Managment Framework, to load  language-specific print rules
	#ifdef LIBRARY_ENGLISH; #ifndef orPrintPatterns_English_STAGE;
		#ifndef orExtensionFrameworkBrief; message "      (English detected. Including English print patterns.)"; #endif;
		Default orPronoun_English_STAGE BEFORE_PARSER;
		#include "_orPronoun_English"; !pronoun is a requirement of the english print patters.  Give it a chance to catchup with the current compiler stage
		Default orPrintPatterns_English_STAGE BEFORE_PARSER;
		#include "_orPrintPatterns_English"; !also include let the English print patterns extension catch up, since it wasn't included manually
	#endif; #endif;

	!encoding constants used when escaping special characters...
	constant DS_ENCODED "___:1"; !dollar sign
	constant OP_ENCODED "___:2"; !open parentheses
	constant CP_ENCODED "___:3"; !close parentheses
	constant CL_ENCODED "___:4"; !colon

	[orPrint txt p1 p2 p3 p4;  orPrintEngine.parseAndPrint(txt, p1, p2, p3, p4); rtrue;];
	! orprint("$TheActor $is giving $theNoun to $theSecond, who $is accepting $itNoun.") !note, the first $is references Actor, since that was the last noun printed; the second refrences the second for the same reason.
	! orprint("$TheActor $actor:gave|give|gives|will-give $bold$theNoun$normal to $theSecond, who $is accepting $itNoun.") !note, the first $is references Actor, since that was the last noun printed; the second refrences the second for the same reason.

	object orPrintEngine
		with parametersString 0 	!parameters part of the text, not including parentheses
		,	patternName 0		!the rule name, parsed from the tokenString
		,	contextObject 0	!the resolved object, derived from the tokenString
		,	objSpecifiedFirst 0
		,	userParam1, userParam2, userParam3, userParam4 !any user-supplied parameters, passed to the orPrint routine
		,	parseAndPrint[fullText p1 p2 p3 p4
					startPtr endPtr currentPatternString;


				fullText=util.orStr.new(fullText);
				if(fullText.getLength()==0) return fullText.free();

				self.parametersString=util.orStr.new();
				self.patternName=util.orStr.new();

				!orStringPool.displaySummary(); !27

				!new_line;orStringPool.displaySummary();
				!self.encodeEscapedCharacters(fullText);

				!new_line;orStringPool.displaySummary();
				fullText.set(self.encodeEscapedCharacters(fullText));

				!new_line;orStringPool.displaySummary();

				!assign the userdefined parameters so they can be accessed in rules
				self.userParam1=p1;
				self.userParam2=p2;
				self.userParam3=p3;
				self.userParam4=p4;
				!now we loop through the text, printing regular text and print patterns one by one as we find them...

				endPtr=fullText.indexOf("$"); !get the pos of the end of "plain text" (start of the first $token pattern)

				while (endPtr>-1){
					self.decodeEscapedCharacters(fullText.mid(startPtr,endPtr-startPtr)).print();  !print the plain text leading up to the token, decoding it first
					startPtr=endPtr+1; !position the token start just after the token marker ($)
					endPtr=self.indexOfPatternEnd(fullText, startPtr); !position the end pointer to the end of the pattern
					currentPatternString=self.decodeEscapedCharacters(fullText.mid(startPtr,endPtr-startPtr)).lock(); !lock the ephemeral
					self.printPattern(currentPatternString); !print the translated token
					currentPatternString.free(); !release the ephemeral
					startPtr=endPtr;!move the next char pointer to the end of the just-printed text
					if(fullText.getChar(startPtr)==';') startPtr++; !if the pattern ends with the "dont-print character, a semicolon, then skip past it..."
					endPtr=fullText.indexOf("$",startPtr); !find the next token start
				}

				self.decodeEscapedCharacters(fullText.mid(startPtr, orSTRING_END)).print();  !print everything after the last token

				!cleanUp
				fullText.free();
				self.patternName.free();
				self.parametersString.free();
			]
		,	encodeEscapedCharacters[str;
				return str.replaceAll("$$", DS_ENCODED)
							.replaceAll("$)",CP_ENCODED)
							.replaceAll("$(",OP_ENCODED)
							.replaceAll("$:",CL_ENCODED);
			]
		,	decodeEscapedCharacters[str; ! retval tmp;
				return str.replaceAll(DS_ENCODED,"$")
							.replaceAll(CP_ENCODED,")")
							.replaceAll(OP_ENCODED,"(")
							.replaceAll(CL_ENCODED,":");
			]
		,	indexOfPatternEnd[str start
					retval sp;

				retval=str.indexOfFirstFalse(self.isAcceptablePatternChar, start); !works for simple patterns

				sp=str.indexOf("(",start);
				if(sp~=-1 && sp<=retval){ !if an open paren was found within our pattern string, then we need to expand it, since parameters may contain additional characters not normally allowed in patterns.
					retval=str.indexOf(")",start)+1; !unlike non-applicable characters which we implicitly use to mark the end of a token, but exclude, we want to INCLUDE the close parentheses...
				}
				if(retval==-1) return str.getLength(); !if no ending characters were identified, then assume we extend to the end of the string
				return retval;
			]
		,	isAcceptablePatternChar[c; !is the character one that we allow to define token patterns, upto and including the open parentheses (but not the close)?
				if(util.orChar.isAlphaNumeric(c)) rtrue;
				if(c=='_' or ':' or '(') rtrue;
				rfalse;
			]
		,	printPattern[pattern
					pr retval;

				self.parsePattern(pattern);
				objectloop(pr ofclass orPrintPattern && pr ~= _oprDefault && retval==false){
					!note:we are just asigning these references; since these are safe strings, the printRule versions will be safe as well
					pr.patternName=self.patternName;
					pr.contextObject=self.contextObject;
					pr.objSpecifiedFirst=self.objSpecifiedFirst;
					pr.parametersString=self.parametersString;
					retval=pr.runRule();
				}
				if(retval==false) {
					_oprDefault.patternName=self.patternName;
					_oprDefault.contextObject=self.contextObject;
					_oprDefault.objSpecifiedFirst=self.objSpecifiedFirst;
					_oprDefault.parametersString=self.parametersString;

					retval=_oprDefault.runRule();
				}
				if(retval==false) pattern.print();

			]
		,	parsePattern[pattern
					tmpTokenString  ptr;

				self.parametersString.set(self.getParameterString(pattern)); !keep the the parameter string for later

				tmpTokenString=self.getTokenString(pattern).lock(); !we parse the token string for more immediately interesting things

				ptr=tmpTokenString.indexOf(":");
				self.objSpecifiedFirst=false;

				if(ptr==-1){ !only one token specified...
					self.contextObject=self.tokenToObject(tmpTokenString); !--is it an object?

					if(self.contextObject <= 0)  !not an object
						self.patternName.set(tmpTokenString); !set that token as the pattern name and move on
					else{
						self.patternName.set("default"); !since only one token and it WAS an object, we must be printing it. !--todo: should this be the default rule?
						self.objSpecifiedFirst=true; 	!this has no bearing on the default behavior of single token rules; but set it anyway for consistency
					}
				}
				else{ !more than one token..

					self.contextObject=self.tokenToObject(tmpTokenString.left(ptr));
					if(self.contextObject > 0) { !if the first token is an object...
						self.objSpecifiedFirst=true;
						self.patternName.set(tmpTokenString.mid(ptr+1, orSTRING_END)); !set the pattern to the last half (+1 advance beyond the colon)
					}
					else{!otherwise, we must be specifying the object second...
						self.objSpecifiedFirst=false;
						self.patternName.set(tmpTokenString.left(ptr)); !set the pattern to the last half...
						self.contextObject=self.tokenToObject(tmpTokenString.mid(ptr+1, orSTRING_END));
					}
				}

				tmpTokenString.free();
			]
		,	getTokenString[pattern sp;
				sp=pattern.indexOf("(");
				if(sp==-1)  return pattern.clone().free(); !clone().free()... this routine is expected to produce an ephemeral which the calling process may or may not lock, then subsequently free.  If we returned pattern alone, which is locked, the process might unlock it in err
				return pattern.left(sp); !this is ephemeral
			]
		,	getParameterString[pattern
					sp ep;

				sp=pattern.indexOf("(");

				if(sp==-1) return util.orStr.new("").free();
				ep=pattern.indexOf(")");

				return pattern.mid(sp,ep-sp).replaceAll("(","").replaceAll(")","");
			]

		,	tokenToObject[txt
					retval amlk;
				amlk=util.orStr.ambiguousLock(txt);

				!if(txt.isEmpty()) retval = -1; !--special meaning: fill in with a default object based on rule type
				if(txt.equals("actor",true)) retval = actor;
				if(txt.equals("player",true)) retval = player;
				if(txt.equals("noun",true)) retval = noun;
				if(txt.equals("second",true)) retval = second;
				if(txt.equals("1")) retval = orPrintEngine.userParam1;
				if(txt.equals("2")) retval = orPrintEngine.userParam2;
				if(txt.equals("3")) retval = orPrintEngine.userParam3;
				if(txt.equals("4")) retval = orPrintEngine.userParam4;

				util.orStr.ambiguousFree(txt, amlk);
				return retval;
			]
		,	printDefaultRule[obj;
				if(metaclass(obj)==routine) return obj();
				if(obj ofclass orString || metaclass(obj)==string) return util.orStr.print(obj);
				if(metaclass(obj)==object) print (name)obj;
				else print obj;  !--this will just print the number
			]
	;

	class orPrintPattern with runRule[; ]
	,	patternName 0
	,	parametersString 0
	,	contextObject 0
	,	objSpecifiedFirst 0
	,	getParam[n;
			if(self.numParams()<=n) return 0;
			return self._getTokenFromString(n, orPrintEngine.parametersString);
		]
	,	numParams[;
			if(orPrintEngine.parametersString.isEmpty())return 0;
			return orPrintEngine.parametersString.numOccurances(":")+1;
		]
	,	_getTokenFromString[n list t pos posEnd;
			for(pos=-1, t=0:t<n:t++) pos=list.indexOf(":",pos); !pos to the starting seperator in question (zero should leave pos at -1, the imaginary first seperator that precedes the list of tokens)
			posEnd=list.indexOf(":",pos+1); !posEnd to the next seperator, following pos

			if(posEnd==-1) posEnd=list.getLength()+1;  !there isnt one, so pull in the rest of the string and add 1, so posEnd points to an imaginary last seperator following the list of tokens, is the same manner as the above (we adjust this down next)

			posEnd--; ! don't include our ending token (imaginary or otherwise)

			if(posEnd<0) { !in the case of the colon starts the string (an empty first token, such as ":text")
				pos=posEnd=-1; !this works below: pos+1==0 (skipping our first imaginary token); posEnd-pos==0 (-1-1)
			}

			return list.mid(pos+1, posEnd-pos);
		];

	 orPrintPattern _oprDefault
		with runRule[;

			if(self.patternName.equalsOneOf("bold", "b")) {
						style bold;
					rtrue;
				}
				if(self.patternName.equalsOneOf("underline", "italics", "i")) {
						style underline;
					rtrue;
				}
				if(self.patternName.equalsOneOf("reverse", "r")) {
						style reverse;
					rtrue;
				}

				if(self.patternName.equalsOneOf("roman", "normal", "n")) {
						style roman;
					rtrue;
				}

				if(self.contextObject~=0){
					if(self.patternName.equals("default")) {orPrintEngine.printDefaultRule(self.contextObject); rtrue;}

					if(self.patternName.equals("the")) {print (the)self.contextObject;rtrue;}
					if(self.patternName.equals("The")) {print (The)self.contextObject;rtrue;}
					if(self.patternName.equals("a")) {print (a)self.contextObject;rtrue;}
					if(self.patternName.equals("A")) {print (A)self.contextObject;rtrue;}
					if(self.patternName.equals("name")) {print (name)self.contextObject;rtrue;}
					if(self.patternName.equals("numb")) {print self.contextObject;rtrue;}
					if(self.patternName.equals("numbText")) {print (LanguageNumber)self.contextObject;rtrue;}
				}

				if(self.numParams()>0){
					if(self.patternName.equalsOneOf("upper","up")) { self.parametersString.upper().print(); rtrue;}
					if(self.patternName.equalsOneOf("lower","lo")) { self.parametersString.lower().print(); rtrue;}

				}

				rfalse; !we could satisfy no rules
			];
!TODO: make PrintOrRun a feature in the standardLibary hooks extension
! 			[ PrintOrRun obj prop flag;
! 				if (obj.#prop > WORDSIZE) return RunRoutines(obj, prop);
! 				if (obj.prop == NULL) rfalse;
! 				switch (metaclass(obj.prop)) {
! 				Class, Object, nothing:
! 					if(obj.prop ofclass orString) {
! 						obj.prop.print();
! #ifdef orPrint_STAGE;
! 						orPrint(obj.prop);
! #ifnot;
! 						obj.prop.print();
! #endif;
! 					}
! 					return RunTimeError(2,obj,prop);
! 				String:
! #ifdef orPrint_STAGE;
! 					orPrint(obj.prop);
! #ifnot;
! 					print (string) obj.prop;
! #endif;
! 					if (flag == 0) new_line;
! 					rtrue;
! 				Routine:
! 					return RunRoutines(obj, prop);
! 				}
! 			];

#endif; !--After PARSER
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
