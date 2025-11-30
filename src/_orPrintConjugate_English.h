!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.11.25 orPrintConjugate (English)
! Conjugation logic for the 250-ish commonly used irregular verbs in 
! the English language (US Dialect).
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! This includes a list of common irregular verbs in the English language, 
! American dialect.  Not a complete list at all.
!--------------------------------------------------------------------------------------
! Revision History
! 2025.11.25	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orPrintConjugate_English_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES

!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orPrintConjugate_English_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orPrintConjugate_English_STAGE  < LIBRARY_STAGE);
   #undef      orPrintConjugate_English_STAGE  ;
   Constant    orPrintConjugate_English_STAGE  LIBRARY_STAGE;
   #ifdef      orPrintConjugate_English_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orPrintConjugate_English...";#endif;
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
constant entrySize 5;
   [ orPrintConjugate ptrn vrb;
		if(ptrn.parametersString.isEmpty()) return;
		print " ";
		if(printIrregularVerbConjugation(ptrn)) return;
		vrb=ptrn.getParamEphemeral(0);
		__orPrintConjugateEnglish(ptrn.contextObject, vrb, ptrn);
		vrb.free();
	];
	[ __orPrintConjugateEnglish obj vrb ptrn !isPerfectTense !--TODO: handle isPerfectTense
			didPrintIrregular endingChar lookupForm;
		
		endingChar=vrb.getCharFromRight(0); !--get the last character once, to minimize repeat calls to this routine
		! if(isPerfectTense){
		! 	lookupForm=ptrn.getParamEphemeral(3);
		! 	if(lookupForm==0) lookupForm=ptrn.getParamEphemeral(2);
		! 	if(lookupForm~=-1 && lookupForm.isEmpty()==false) return lookupForm.print();
		! 	if(vrb.right(1).equals("e")) 
		! 		vrb.append("d").print();
		! 	else 
		! 		vrb.append("ed").print();
		! 	vrb.free();
		! 	return;
		! }
		!--past tense
		if(getNarrativeTense()== PAST_TENSE){
			lookupForm=ptrn.getParamEphemeral(2); !--zero indexed.  3rd parameter is past tense form.
			if(lookupForm~=-1 && lookupForm.isEmpty()==false) return lookupForm.print();
			
			if(endingChar=='y' && util.orChar.isConsonant(vrb.getCharFromRight(1))) return vrb.left(vrb.getLength()-1).append("ied").print(); !--cry = cried
			if(endingChar=='e') return vrb.append("d").print(); !--hope = hoped
			
			!--adding "ed" -------
			vrb.print();
			!--for words ending in a consonant - vowl - consonant pattern, double the last letter before adding "ed" (unless the last letter is w,x, or y)
			if(endingChar~='w' or 'x' or 'y' && util.orChar.isConsonant(endingChar) && util.orChar.isVowel(vrb.getCharFromRight(1)) && util.orChar.isConsonant(vrb.getCharFromRight(2)))
				print (char)endingChar; !--hop = hopped
			print "ed"; 
		}
		if(getNarrativeTense()== PRESENT_TENSE){
			!--3rd person singular
			if (self.contextObject hasnt pluralname && (self.contextObject~=player || getNarrativePerson()==THIRD_PERSON )) { 
				lookupForm=ptrn.getParamEphemeral(1); !--zero indexed. 2nd parameter is 3rd person singular, present tense form
				if(lookupForm~=-1 && lookupForm.isEmpty()==false) return lookupForm.print();

				if(endingChar=='y' && util.orChar.isConsonant(vrb.getCharFromRight(1)))  {
					return vrb.left(vrb.getLength()-1).append("ies").print(); !fly=flies
				}
				if(endingChar=='s') return vrb.append("es").print(); !kiss=kisses
				
				return vrb.append("s").print(); !--default: hide=hides
			}
			!--1st, 2nd, or 3rd person plural
			else{
				vrb.print(); !--the first pattern matches the passed in verb pattern
				return;
			}
		}
		
	 ];
! A simple FNV-0 hashing implementation, used to speed lookups.
[hashString str i hsh;
   str=util.orStr.new(str); 
   for(i=0:i<str.getLength():i++){
      hsh=hsh*$1b3;
      hsh=util.orNum.xor(hsh,util.orChar.toLower(str.getChar(i)));
   }
   hsh=hsh & $ffff;
   str.free();
   return hsh;
];

[ printIrregularVerbConjugation  ptrn isPerfectTense !--TODO: handle isPerfectTense
      entryPos pos isThirdPerson; 
   entryPos=findVerbEntryPos(ptrn.getParamEphemeral(0));
   if(entryPos==-1) return false;

   pos=(entryPos*entrySize)+1; !+1 to advance past the hash key

   !--TODO: handle perfect tense
   !past tense
   if(player provides narrative_tense && player.narrative_tense == PAST_TENSE){
		pos=pos+2; 
	}
	else{ !--present tense
      !--3rd person singular...
      if (self.contextObject~=player ||  (self.contextObject==player &&  player provides narrative_voice && player.narrative_voice == 3)) pos=pos+1;  
      !--other present tense, no change.
   }
   util.orArray.get(_irregularVerbs, pos).print();
   return true;
];

!perform a binary search against the array, returs the number of the element in the array 
[findVerbEntryPos str hsh lo hi mid el count;
   hsh=hashString(str);
   lo=0;
   hi=(util.orArray.getSize(_irregularVerbs)/entrySize)-1; !last element 
   while(lo<=hi){
      mid=lo+(hi-lo)/2;
      el=util.orArray.get(_irregularVerbs, mid*entrySize); 
      if(el==hsh) { !found!
         while(hsh==util.orArray.get(_irregularVerbs, (mid-1)*entrySize)) mid--; !its possible that a hash table can have multiple elements with the same hash.  So let's back up to the first and start comparing them one by one.
         while(hsh==util.orArray.get(_irregularVerbs, (mid)*entrySize)){ !--go through them all
            if(util.orStr.areEqual(str, util.orArray.get(_irregularVerbs, (mid*entrySize)+1) ,true)) return mid; !--if we match the word, return the element number
            mid++;
         } 
         return -1; !--we matched the hash, but the strings don't match
      }
      if(util.orNum.uIsLessThan(el, hsh))
         lo=mid+1;
      else 
         hi=mid-1;
   }  
   return -1; !--not match for the hash
];
      
array _irregularVerbs table [
   $0054 "get" "gets" "got" "gotten"
   $00E5 "leave" "leaves" "left" 0
   $0583 "slay" "slays" "slew" "slain"
   $0722 "learn" "learns" "learned" 0
   $0971 "know" "knows" "knew" "known"
   $099F "teach" "teaches" "taught" 0
   $0A13 "misunderstand" "misunderstands" "misunderstood" 0
   $0B43 "spoil" "spoils" "spoiled" 0
   $0C18 "knit" "knits" "knit" 0
   $0C98 "dig" "digs" "dug" 0
   $0CFD "mishear" "mishears" "misheard" 0
   $0F6D "rebuild" "rebuilds" "rebuilt" 0
   $10A9 "steal" "steals" "stole" "stolen"
   $1130 "wake" "wakes" "woke" "woken"
   $12C0 "split" "splits" "split" 0
   $1388 "throw" "throws" "threw" "thrown"
   $15FE "ring" "rings" "rang" "rung"
   $1663 "relay" "relays" "relaid" 0
   $17E7 "stink" "stinks" "stank" "stunk"
   $17EB "sting" "stings" "stung" 0
   $18AB "put" "puts" "put" 0
   $1C02 "plead" "pleads" "pleaded" 0
   $1E90 "uphold" "upholds" "upheld" 0
   $1FAE "ride" "rides" "rode" "ridden"
   $2060 "stick" "sticks" "stuck" 0
   $2065 "shine" "shines" "shone" 0
   $20BF "oversleep" "oversleeps" "overslept" 0
   $2497 "mislead" "misleads" "misled" 0
   $25D3 "fall" "falls" "fell" "fallen"
   $26D1 "housebreak" "housebreaks" "housebroke" "housebroken"
   $2725 "quit" "quits" "quit" 0
   $27E5 "overdo" "overdoes" "overdid" "overdone"
   $28DD "meet" "meets" "met" 0
   $2A60 "build" "builds" "built" 0
   $2AA4 "swim" "swims" "swam" "swum"
   $2BDB "overtake" "overtakes" "overtook" "overtaken"
   $2C7D "forswear" "forswears" "forswore" "forsworn"
   $2D3A "pay" "pays" "paid" 0
   $2DDF "oversee" "oversees" "oversaw" "overseen"
   $2F1B "mean" "means" "meant" 0
   $3087 "unwind" "unwinds" "unwound" 0
   $30B4 "unbind" "unbinds" "unbound" 0
   $32CE "understand" "understands" "understood" 0
   $336E "make" "makes" "made" 0
   $3384 "overpay" "overpays" "overpaid" 0
   $342E "recast" "recasts" "recast" 0
   $3439 "upset" "upsets" "upset" 0
   $34FA "melt" "melts" "melted" "molten"
   $3615 "mislay" "mislays" "mislaid" 0
   $3631 "bend" "bends" "bent" 0
   $3827 "forecast" "forecasts" "forecast" 0
   $387A "bear" "bears" "bore" "born"
   $387C "beat" "beats" "beat" "beaten"
   $38D8 "seek" "seeks" "sought" 0
   $3CDA "undo" "undoes" "undid" "undone"
   $3D96 "rebind" "rebinds" "rebound" 0
   $3E04 "rive" "rives" "rived" "riven"
   $3E06 "shut" "shuts" "shut" 0
   $3E37 "run" "runs" "ran" 0
   $3F67 "spoonfeed" "spoonfeeds" "spoonfed" 0
   $40A7 "misdeal" "misdeals" "misdealt" 0
   $40EE "eat" "eats" "ate" "eaten"
   $422B "shrink" "shrinks" "shrank" "shrunk"
   $431F "rise" "rises" "rose" "risen"
   $438C "dwell" "dwells" "dwelled" "dwelled"
   $44F4 "sell" "sells" "sold" 0
   $46DA "overhear" "overhears" "overheard" 0
   $4720 "overeat" "overeats" "overate" "overeaten"
   $479A "send" "sends" "sent" 0
   $4874 "smite" "smites" "smote" "smitten"
   $4B96 "overhang" "overhangs" "overhung" 0
   $4CAE "dive" "dives" "dove" "dived"
   $4CC4 "cleave" "cleaves" "clove" "cloven"
   $4DAF "outdo" "outdoes" "outdid" "outdone"
   $5078 "wreak" "wreaks" "wrought" 0
   $549F "misspend" "misspends" "misspent" 0
   $5662 "prove" "proves" "proved" "proven"
   $580D "misspell" "misspells" "misspelled" 0
   $5974 "sit" "sits" "sat" 0
   $59C6 "shed" "sheds" "shed" 0
   $59FD "rethink" "rethinks" "rethought" 0
   $5AC9 "forsake" "forsakes" "forsook" "forsaken"
   $5B35 "fit" "fits" "fit" 0
   $5CC9 "smell" "smells" "smelled" 0
   $5D09 "stave" "staves" "staved" 0
   $5FCE "thrust" "thrusts" "thrust" 0
   $6023 "fly" "flies" "flew" "flown"
   $602D "sunburn" "sunburns" "sunburned" 0
   $6314 "forego" "foregoes" "forewent" "foregone"
   $6345 "sow" "sows" "sowed" "sown"
   $63A3 "show" "shows" "showed" "shown"
   $63B1 "shoe" "shoes" "shod" 0
   $6585 "shave" "shaves" "shaved" "shaven"
   $665F "undertake" "undertakes" "undertook" "undertaken"
   $66AA "forgive" "forgives" "forgave" "forgiven"
   $66E1 "say" "says" "said" 0
   $66EF "saw" "saws" "sawed" "sawn"
   $67C9 "heave" "heaves" "hove" 0
   $67FE "forbid" "forbids" "forbade" "forbidden"
   $69B2 "deal" "deals" "dealt" 0
   $6B59 "wring" "wrings" "wrung" 0
   $6D01 "see" "sees" "saw" "seen"
   $6D10 "set" "sets" "set" 0
   $6D13 "sew" "sews" "sewed" "sewn"
   $6D89 "rid" "rids" "rid" 0
   $712E "drink" "drinks" "drank" "drunk"
   $7257 "slide" "slides" "slid" 0
   $7301 "rewind" "rewinds" "rewound" 0
   $7327 "let" "lets" "let" 0
   $7357 "hit" "hits" "hit" 0
   $74B0 "overlay" "overlays" "overlaid" 0
   $7515 "write" "writes" "wrote" "written"
   $76B5 "shoot" "shoots" "shot" 0
   $77D8 "outsell" "outsells" "outsold" 0
   $7983 "choose" "chooses" "chose" "chosen"
   $79A2 "swing" "swings" "swung" 0
   $7A2B "keep" "keeps" "kept" 0
   $7A66 "lay" "lays" "laid" 0
   $7B1F "swell" "swells" "swelled" "swollen"
   $7C03 "sling" "slings" "slung" 0
   $7C0F "slink" "slinks" "slunk" 0
   $8458 "come" "comes" "came" "come"
   $85A3 "lose" "loses" "lost" 0
   $85C9 "dream" "dreams" "dreamed" "dreamed"
   $862B "shear" "shears" "shore" "shorn"
   $8650 "stand" "stands" "stood" 0
   $865C "gild" "gilds" "gilded" 0
   $8728 "sweep" "sweeps" "swept" 0
   $87D2 "lie" "lies" "lay" "lain"
   $87D7 "cost" "costs" "cost" 0
   $87D9 "tell" "tells" "told" 0
   $87F0 "hew" "hews" "hewed" "hewn"
   $8A2C "miscast" "miscasts" "miscast" 0
   $8B68 "flee" "flees" "fled" 0
   $8CBC "tear" "tears" "tore" "torn"
   $8E56 "swear" "swears" "swore" "sworn"
   $9365 "misread" "misreads" "misread" 0
   $93B8 "withstand" "withstands" "withstood" 0
   $93CD "bet" "bets" "bet" 0
   $968B "chide" "chides" "chid" "chidden"
   $9742 "shake" "shakes" "shook" "shaken"
   $9753 "give" "gives" "gave" "given"
   $97ED "outgrow" "outgrows" "outgrew" "outgrown"
   $9A68 "drive" "drives" "drove" "driven"
   $9ABC "light" "lights" "lit" 0
   $9ABE "withdraw" "withdraws" "withdrew" "withdrawn"
   $9CA7 "forbear" "forbears" "forebore" "forborne"
   $9D12 "proofread" "proofreads" "proofread" 0
   $9DA2 "spill" "spills" "spilt" 0
   $9DF5 "beseech" "beseeches" "besought" 0
   $9E41 "waylay" "waylays" "waylaid" 0
   $9F5F "become" "becomes" "became" "become"
   $A030 "rewrite" "rewrites" "rewrote" "rewritten"
   $A070 "tread" "treads" "trod" "trodden"
   $A138 "mistake" "mistakes" "mistook" "mistaken"
   $A296 "draw" "draws" "drew" "drawn"
   $A2D1 "remake" "remakes" "remade" 0
   $A58B "creep" "creeps" "crept" 0
   $A7F8 "think" "thinks" "thought" 0
   $A983 "do" "does" "did" "done"
   $A9D9 "freeze" "freezes" "froze" "frozen"
   $AC7B "foretell" "foretells" "foretold" 0
   $AE90 "buy" "buys" "bought" 0
   $AF6A "go" "goes" "went" "gone"
   $B095 "preset" "presets" "preset" 0
   $B10F "burn" "burns" "burned" "burned"
   $B1D3 "overshoot" "overshoots" "overshot" 0
   $B2CA "weave" "weaves" "wove" "woven"
   $B367 "begin" "begins" "began" "begun"
   $B37A "dare" "dares" "dared" 0
   $B409 "outshine" "outshines" "outshone" 0
   $B511 "wear" "wears" "wore" "worn"
   $B596 "browbeat" "browbeats" "browbeat" "browbeaten"
   $B5EA "grind" "grinds" "ground" 0
   $B6EC "input" "inputs" "input" 0
   $B7DC "undergo" "undergoes" "underwent" "undergone"
   $B7E2 "spit" "spits" "spat" 0
   $B7F8 "spin" "spins" "spun" 0
   $BC5F "weep" "weeps" "wept" 0
   $BC69 "strew" "strews" "strewed" "strewn"
   $BD86 "leap" "leaps" "leaped" 0
   $BD92 "lead" "leads" "led" 0
   $BD98 "lean" "leans" "leaned" 0
   $BDF2 "rerun" "reruns" "reran" "rerun"
   $C2AB "hurt" "hurts" "hurt" 0
   $C3E1 "rend" "rends" "rent" 0
   $C55C "read" "reads" "read" 0
   $C584 "wet" "wets" "wet" 0
   $C594 "wed" "weds" "wed" 0
   $C61B "underwrite" "underwrites" "underwrote" "underwritten"
   $C678 "thrive" "thrives" "thrived" "thriven"
   $C72E "overthrow" "overthrows" "overthrew" "overthrown"
   $C7BF "hold" "holds" "held" 0
   $CB91 "awake" "awakes" "awoke" "awoken"
   $CC01 "sleep" "sleeps" "slept" 0
   $CC37 "withhold" "withholds" "withheld" 0
   $CC49 "strive" "strives" "strove" "striven"
   $CDD8 "redo" "redoes" "redid" "redone"
   $CE8B "resell" "resells" "resold" 0
   $D0C3 "lend" "lends" "lent" 0
   $D0EA "wind" "winds" "wound" 0
   $D181 "inlay" "inlays" "inlaid" 0
   $D442 "fling" "flings" "flung" 0
   $D537 "repay" "repays" "repaid" 0
   $D589 "overrun" "overruns" "overran" "overridden"
   $D754 "feed" "feeds" "fed" 0
   $D75C "feel" "feels" "felt" 0
   $DA3A "win" "wins" "won" 0
   $DBE5 "reset" "resets" "reset" 0
   $DC05 "outbid" "outbids" "outbid" 0
   $DC32 "have" "has" "had" 0
   $DC79 "clothe" "clothes" "clothed" "clothed"
   $DD01 "take" "takes" "took" "taken"
   $DD99 "grow" "grows" "grew" "grown"
   $E01F "spring" "springs" "sprang" "sprung"
   $E05B "grave" "graves" "graved" "graven"
   $E1C0 "hide" "hides" "hid" "hidden"
   $E1E2 "burst" "bursts" "burst" 0
   $E1F0 "floodlight" "floodlights" "floodlit" 0
   $E233 "cling" "clings" "clung" 0
   $E2CE "oversell" "oversells" "oversold" 0
   $E612 "retell" "retells" "retold" 0
   $E6F0 "partake" "partakes" "partook" "partaken"
   $E76B "kneel" "kneels" "knelt" 0
   $E988 "hang" "hangs" "hung" 0
   $EAA7 "stride" "strides" "strode" "stridden"
   $EB39 "hamstring" "hamstrings" "hamstrung" 0
   $EBA0 "hear" "hears" "heard" 0
   $EC18 "spend" "spends" "spent" 0
   $ECBD "catch" "catches" "caught" 0
   $EF8E "spell" "spells" "spelt" 0
   $EFBE "strike" "strikes" "struck" 0
   $F0B9 "find" "finds" "found" 0
   $F0FF "mow" "mows" "mowed" "mown"
   $F115 "spread" "spreads" "spread" 0
   $F310 "unbend" "unbends" "unbent" 0
   $F38D "forerun" "foreruns" "foreran" "forerun"
   $F413 "forget" "forgets" "forgot" "forgotten"
   $F493 "string" "strings" "strung" 0
   $F581 "sink" "sinks" "sank" "sunk"
   $F58D "sing" "sings" "sang" "sung"
   $F80A "speak" "speaks" "spoke" "spoken"
   $F816 "slit" "slits" "slit" 0
   $F950 "cut" "cuts" "cut" 0
   $FAD5 "beset" "besets" "beset" 0
   $FBBA "fight" "fights" "fought" 0
   $FCBB "outrun" "outruns" "outran" 0
   $FD56 "overcome" "overcomes" "overcame" "overcome"
   $FE42 "arise" "arises" "arose" "arisen"
   $FED3 "foresee" "foresees" "foresaw" "foreseen"
   $FF49 "speed" "speeds" "sped" 0
];

#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE


