!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.02.09 orGibberish
! Prints a human-pronouncable, random "word".
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
!
!--------------------------------------------------------------------------------------
! Revision History
! 2001.12.15	Initial Creation
! ...			   History from orLibrary v1 snipped away.
! 2025.02.09	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orGibberish_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES

!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orGibberish_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orGibberish_STAGE  < LIBRARY_STAGE);
   #undef      orGibberish_STAGE  ;
   Constant    orGibberish_STAGE  LIBRARY_STAGE;
   #ifdef      orGibberish_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orGibberish...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object orGibberish !---Print random pronouncable words
		private _phoneChoice 0 0 0 0 0 0
		,	_vowels		"a" "e" "i" "o" "u" "oo" "ou" "ae" "ei" "ia" "ee" !---vowels
		!---consonant phones------------------------------------------------------------
		,	_tenueCon	   "c" "k" "p" "qu" "t"			!---tenue mutes
		,	_muteCon		"b" "d" "g" "j" "ph" "th" "ch"	!---mediae & aspirates mutes
		,	_liquidCon	"l" "r"							!---liquids
		,	_frictiveCon	"f" "s" "z" "v"					!---frictives
		,	_genCon		"h" "m" "n" "w" "x" "y"			!---others
		,	_chooseVowel[; self._printOne(_vowels);]
		,	_chooseConsonant[disallowliquids value;
				if(disallowliquids)
					value=random(8)+6; !--ignore the first six liquid ending possibilities
				else
					value=random(15);
				!---5 simple rules to determine complex consonant phones:
				!---	a) a liquid can follow any mute						   (1,2)
				!---	b) a liquid can follow any frictive					   (3,4)
				!---	c) a liquid can follow an 's'						      (5)
				!---	d) a liquid following a tenue can follow an 's'	   (6)
				!---	e) a tenue can follow an 's'						      (7)
				!---but most of the time non-complex consonants are used	(8-15)
				switch(value){
					1, 2:	self._printOne(_tenueCon,_muteCon);			!--- 2/15 (probability)
							self._printOne(_liquidCon);
					3, 4:	self._printOne(_frictiveCon);					!--- 2/15
							self._printOne(_liquidCon);
					5:	print (string)"s"; self._printOne(_liquidCon);	!--- 1/15
					6:	print (string)"s"; self._printOne(_tenueCon);		!--- 1/15
						self._printOne(_liquidCon);
					7:	print (string)"s"; self._printOne(_tenueCon);		!--- 1/15
					default: self._printOne(_tenueCon,_liquidCon,			!--- 8/15
										_frictiveCon,_muteCon,_genCon);
				}]
		,	_add_PhoneChoice[o;
				self.&_phoneChoice-->0=(self.&_phoneChoice-->0)+1;
				self.&_phoneChoice-->(self.&_phoneChoice-->0)=o;]
		,	_printOne[one two three four five choice val;
				self.&_phoneChoice-->0=0;
				if(one)self._add_PhoneChoice(one);
				if(two)self._add_PhoneChoice(two);
				if(three)self._add_PhoneChoice(three);
				if(four)self._add_PhoneChoice(four);
				if(five)self._add_PhoneChoice(five);
				val=random(self.&_phoneChoice-->0);
				choice=self.&_phoneChoice-->(val);
				print (string)(self.&choice-->(random(self.#choice/WORDSIZE)-1));]
      with	printWord[num_vowels t;
				if(num_vowels==0)return;
				if(random(true,false)==true) self._chooseConsonant();
				for(t=0:t<num_vowels:t++){
					self._chooseVowel();
					if(t<(num_vowels-1)) self._chooseConsonant(); !--skip final
				}
				if(random(true,false)==true) self._chooseConsonant(true);]!--true: no liquid end
	;
#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
