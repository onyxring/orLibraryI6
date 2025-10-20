!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.04.14 orBuf
! Various utility functions for dealing with buffers.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
! License: Public Domain
!--------------------------------------------------------------------------------------
! This extension, classified as a "utility", provides tooling for other extensions to
! use and provides no specific game elements by itself.
!
! orBuf provides routines wrapped together in the orBuf member of the util object.
! These routines all support manipulation of buffers.  Buffers, also known as Inform
! "character tables," are character arrays, except for the first WORD_SIZE number of bytes,
! which contain the current length of the string which the buffer contains.
!
! Additionally, orBuf supports the related concept of "sized buffers" which have the
! following characteristics:
!
! References to sized buffers (the pointers to these) are offset to a position
! 2 + WORDSIZE bytes into the allocated array.  This means that variables pointing to
! sized buffers no longer point to the start of the array, but a few bytes in.
!
! With this offset in place, sized buffers are indistinguishable from normal buffers.
! For example:
!
!		buf-->0 is still the length of the string contained in the buffer
!		buf->WORDSIZE is still the first character of the string.
!
!	However, sized buffers also have the following charactersitics:
!
!		the two consecutive bytes stored at buf->(-WORDSIZE-2) contain the identifying
!  		values of $90 and $84.
!
!		buf-->(-1) contains the allocated size of the array usable by characters (the
!			original size of the array, minus WORDSIZE (for string length), minus
!			WORDSIZE (for array size), minus two bytes (the "sized buffer identifier").
!
!	Because sized arrays look like normal arrays; the routines in the orBuf object
!	can be used with either.
!
! Complementary extensions:
!
! TODO:
! *	orStringArray : If included, the additional property routines are added to the
!						orBuf object for dynamic memory managment and allocation.
!
!						Specifically:
!							.lock()
!							.free()
! 							.newFromLiteral()
!--------------------------------------------------------------------------------------
! Revision History
! 2024.04.14	Initial creation.
!======================================================================================
! Extension Framework management
#ifndef orExtensionFramework_STAGE;
default        orUtilBuf_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUtil";
#include "orUtilNum";
#include "orUtilChar";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orUtilBuf_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orUtilBuf_STAGE  < LIBRARY_STAGE);
   #undef      orUtilBuf_STAGE  ;
   Constant    orUtilBuf_STAGE  LIBRARY_STAGE;
   #ifdef      orUtilBuf_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orUtilBuf...";#endif;
!======================================================================================
! AFTER VERBLIB
!--------------------------------------------------------------------------------------
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
	constant orBUF_END -1;
	default orStringDefaultSize 500;
	!TODO: we've setup the concept of "sizedBuffers"; however we are not actually using these to enforce size, only to distinguish bufs from literals.  Need to add size enforcement to all of these.
	object _orBuf
	 private	_captureStreamValue
		,	_captureBuffer
	 	with set[buf stringVal;
				if(self.isSized(stringVal)) {
					self.setFromBuffer(buf, stringVal);
				}else	{
					self.setFromLiteral(buf, stringVal);
				}
				return buf;
			]
		,	setFromLiteral[buf stringVal len;
				if(len==0) len=self.getBestSize(buf);
				stringVal.print_to_array(buf, len); !check
				self.setLength(buf,self.getLength(buf)); !--pointless, but this keeps length setting in one place, helping with things like profiling and consistency checks
			]
		,	setFromBuffer[buf stringVal;
				self.copy(buf, stringVal, self.getLength(stringVal),0,0);
				self.setLength(buf,self.getLength(stringVal));
			]
		,	getSize[buf;
				if(self.isSized(buf)==false) return -1;
				return buf-->(-1);
			]
		,	getBestSize[buf;
				if(self.isSized(buf)==false) return orStringDefaultSize; !--may not be right, but if not passed in it'll be the best guess
				return buf-->(-1);
			]
		,	isSized[buf;
				#ifdef TARGET_ZCODE;
				if (util.orNum.uIsBetween(buf, $000E-->0, #array__start) == false) rfalse; !has to be a literal; skip out immediately so we don't try to read protected memory
				#endif;

				!TODO: GLULX supports the ability to assign types to memory objects, which is cleaner.  Consider doing this for GLULX-only code
				if(buf->(-WORDSIZE-2)==$90 && buf->(-WORDSIZE-1)==$84 ) rtrue;
				rfalse;
			]
		,	capture[buf size; size=size; !suppress unused warning 
				self._captureBuffer=buf;
				#ifndef TARGET_GLULX;
			        @output_stream 3 buf;	!capture output to an array
	            #ifnot;
					if(size==0) size=self.getSize(buf);
					if(size==-1) size=orStringDefaultSize;
                    self._captureStreamValue=glk_stream_get_current();
                    glk_stream_set_current(glk_stream_open_memory(self._captureBuffer+WORDSIZE, size, filemode_Write, 662));
	            #endif;
			]
		,	release[;
                #ifndef TARGET_GLULX;
                    @output_stream -3;		!release output back to screen
                #ifnot;
                    glk_stream_close(glk_stream_get_current(), gg_arguments);
                    glk_stream_set_current(self._captureStreamValue);
                    (self._captureBuffer-->0)=gg_arguments-->1;
                #endif;
				self.setLength(self._captureBuffer,self.getLength(self._captureBuffer)); !--pointless, but this keeps length setting in one place, helping with things like profiling and consistency checks
				return self._captureBuffer;
			]
		,	getLength [buf; return (buf-->0); ]
		,	setLength [buf newLength
		  			size;
				size = self.getSize(buf);
				if(size>0 && newLength > size) {
		  			#ifdef DEBUG; print "WARNING setLength: trying to set string length to greater than buffer size."; #endif;
		  			newLength=size; !--accommodate the size of the buffer
		  		}
		  		(buf-->0)=newLength;
		  	]
		,	print[buf len
					i c;
				if(len<=0) len=self.getLength(buf);
				for(i=0:i<len:i++) {
					c=self.getChar(buf,i);
					if(c==0) break; !--Shouldn't normally happen, but if theres been some sizing issues, its better to just finish on an embedded zero
					print (char)c;
				}
				return buf;
			]
        ,	equals[buf1 buf2 caseInsensitive
                    t;
				if(self.getLength(buf1)~=self.getLength(buf2)) rfalse; ! different sizes, so not equal
                for(t=WORDSIZE: t< self.getLength(buf1)+WORDSIZE :t++){
					if(caseInsensitive<= 0){
                        if(buf1->(t)~=buf2->(t)) {
							rfalse;
						}
					}else{
                        if(util.orChar.toLower(buf1->(t))~=util.orChar.toLower(buf2->(t))) {
							rfalse;
						}
					}
                }
				rtrue;
			]
        ,	getChar[buf pos; return buf->(pos+WORDSIZE);]
		,	setChar[buf pos val; buf->(pos+WORDSIZE)=val;]
		,	convertToSizedBuffer[b size;
				if(size==0) print "WARNING!!! util.orBuf.convertToSizedBuffer() has zero size specified.";
				if(self.isSized(b)) return b; !--already a sized
				b=b+(WORDSIZE+2);
				b-->(-1)=size-(WORDSIZE+WORDSIZE+2); !--the string length; the buffer length; and the 2 byte indicator
				b->(-WORDSIZE-1)=$84;
				b->(-WORDSIZE-2)=$90;
				!b->0=$90;
				!b->1=$84;
				!b-->1=size-(WORDSIZE+WORDSIZE+2); !--the string length; the buffer length; and the 2 byte indicator
				!return b+(WORDSIZE+2);
				return b;
			]
		,	copy[toBuf fromBuf numCharsToCopy toPos fromPos
                    frmPtr toPtr size;
				!TODO: is NULL the right value to use here?  If NULL is the same as zero, then this can all go away
				if(fromPos==NULL) fromPos=0;
				if(toPos==NULL) toPos=0;

				if (numCharsToCopy<0 || numCharsToCopy+fromPos > self.getLength(fromBuf)) numCharsToCopy=self.getLength(fromBuf)-fromPos; !--make sure we are not trying to read beyond the end of the source length
				size=self.getSize(toBuf);
				if(size>0 && numCharsToCopy+toPos > size) numCharsToCopy=size-toPos; !--make sure we are not trying to overflow the target buffer


				frmPtr=fromBuf+fromPos+WORDSIZE;
				toPtr=toBuf+toPos+WORDSIZE;
				! for(i=0:i <numCharsToCopy:i++)
                !     toBuf->(i+toPos+WORDSIZE)= fromBuf->(i+fromPos+WORDSIZE);

				!The following uses "native" interpreter methods to do the above which should be significantly faster
		#ifdef TARGET_ZCODE;
				@copy_table frmPtr toPtr numCharsToCopy;
		#ifnot;
				@mcopy numCharsToCopy frmPtr toPtr;
		#endif;
				self.setLength(toBuf,toPos+numCharsToCopy);

                return toBuf;
			]
        ,	mid[toBuf fromBuf fromPos numCharsToCopy
					maxLengthToCopy;
			!TODO
			!maxLengthToCopy = util.orNum.min(self.getLength(fromBuf)-fromPos, self.getLength(toBuf));
				maxLengthToCopy = self.getLength(fromBuf)-fromPos;
				if(numCharsToCopy==orBUF_END || numCharsToCopy>maxLengthToCopy) numCharsToCopy=maxLengthToCopy;
				self.copy(toBuf, fromBuf, numCharsToCopy, 0, fromPos);
				self.setLength(toBuf, numCharsToCopy);
				return toBuf;
			]
		,	left[toBuf fromBuf numCharsToCopy;
				self.copy(toBuf, fromBuf, numCharsToCopy, 0, 0);
				self.setLength(toBuf, numCharsToCopy);
				return toBuf;
			]
		,	right[toBuf fromBuf numCharsToCopy
						fromPos;
				fromPos=self.getLength(fromBuf)-numCharsToCopy;
				self.copy(toBuf, fromBuf, numCharsToCopy, 0, fromPos);
				self.setLength(toBuf, numCharsToCopy);
				return toBuf;

			]
		,   insert[toBuf fromBuf toPos charCount
						len;
				if(charCount<=0) charCount=self.getLength(fromBuf);
				len=self.getLength(toBuf);
				!--expand the target buffer to make room
				! for(i=self.getLength(toBuf):i>=toPos:i--){
				! 	toBuf->(WORDSIZE+i+charCount)=toBuf->(WORDSIZE+i);
				! }
				

				self.copy(toBuf, toBuf, len-toPos, toPos+charCount, toPos);
				
				! !--now fill in the inserted space
				! for(i=0:i<charCount:i++){
				! 	if(fromBuf==0)
				! 		toBuf->(WORDSIZE+i+toPos)=' ';
				! 	else
				! 		toBuf->(WORDSIZE+i+toPos)=fromBuf->(WORDSIZE+i);
				! }
				self.copy(toBuf, fromBuf, charCount, toPos);
				self.setLength(toBuf, len+charCount);
				return toBuf;
			]
		, 	prepend[toBuf fromBuf;
 				self.insert(toBuf,fromBuf, 0, self.getLength(fromBuf));
				return toBuf;
			]
		, 	append[toBuf fromBuf;
 				self.copy(toBuf,fromBuf,NULL, self.getLength(toBuf), 0);
				return toBuf;
			]
		,   delete[buf pos count
					len;

				len=self.getLength(buf);
				self.copy(buf, buf, len-count,pos, pos+count);
				self.setLength(buf, len-count);
				return buf;
			]
		,   replace[buf searchTextBuf replaceTextBuf
				pos searchLength;

				searchLength=self.getLength(searchTextBuf);
				pos = self.indexOf(buf,searchTextBuf);
				if(pos==-1) return buf;
				self.delete(buf, pos, searchLength);
				self.insert(buf, replaceTextBuf, pos);
				return buf;
			]
		,   replaceAll[buf searchTextBuf replaceTextBuf startingIndex;

				while(startingIndex ~=-1){
					self.replace(buf, searchTextBuf, replaceTextBuf);
					startingIndex=self.indexOf(buf, searchTextBuf, startingIndex);
				}
				return buf;
			]
		,	indexOf[buf searchTextBuf startingIndex
					o i;
				for(o=startingIndex: o<self.getLength(buf):o++){
					for(i=0: i<self.getLength(searchTextBuf):i++){
						if(buf->(o+i+WORDSIZE)~=searchTextBuf->(i+WORDSIZE)) break;
					}
					if(i==self.getLength(searchTextBuf)) return o;
				}
				return -1;
			]
		,	indexOfFirstTrue[buf routine startingIndex
					o c;
				for(o=startingIndex: o<self.getLength(buf):o++){
					c = buf->(o+WORDSIZE);
					if(routine(c)==true) return o;
				}
				return -1;
			]
		,	indexOfFirstFalse[buf routine startingIndex
					o c;
				if(startingIndex<0) startingIndex=0;
				if(startingIndex>self.getLength(buf)) return -1;
				for(o=startingIndex: o<self.getLength(buf):o++){
					c = buf->(o+WORDSIZE);
					if(routine(c)==false) return o;
				}
				return -1;
			]
		,	toUpper[buf
					t;
				for(t=WORDSIZE:t<=self.getLength(buf)+WORDSIZE:t++){
					buf->t=util.orChar.toUpper(buf->t);
				}
				return buf;
			]
		,	toLower[buf
					t;
				for(t=WORDSIZE:t<self.getLength(buf)+WORDSIZE:t++){
					buf->t=util.orChar.toLower(buf->t);
				}
				return buf;
			]
		,	reverse[buf !--With this approach, we do this without using another buffer
					t c;
				for(t=0:t<self.getLength(buf)/2:t++){
					c=buf->(WORDSIZE+t);
					buf->(WORDSIZE+t)=buf->(WORDSIZE+self.getLength(buf)-t-1);
					buf->(WORDSIZE+self.getLength(buf)-t-1)=c;
				}
				return buf;
			]
		,	trimLeft[buf;
				while(self.getLength(buf)>0 && self.getChar(buf,0)==' ') self.delete(0);
				return buf;
			]
		,	trimRight[buf;
				while(self.getLength(buf)>0 && self.getChar(buf,self.getLength(buf)-1)==' ') {
					self.delete(buf,self.getLength(buf)-1,1);
				}
				return buf;
			]
		,	trim[buf;
				return self.trimRight(self.trimLeft(buf));
			]
#ifdef orStringPool; !we dont require dynamic allocation, but if the feature is there already we can add some nice syntactical sugar...
		,	newFromLiteral[val retval;
				retval=orStringPool.allocateBuffer();
				self.setFromLiteral(retval,val);
				self.lock(retval);
				return retval;
			]
		,	lock[buf;
				orStringPool.allocateBuffer(buf);
				return buf;
			]
		,	free[buf; !add the current buffer to the pool, making it available for other allocation
				orStringPool.freeBuffer(buf);
				return buf;
			]

#endif;
	;
	#endif; !--AFTER VERBLIB
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE