!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.05.18 orHookWriteList
! Adds hooks into the WriteListR routine for use by the standard librarys
! LibraryExtensions object.  No functional changes have been made.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License:
!--------------------------------------------------------------------------------------
! This extension, classified as a "hook", provides tooling for other extensions to
! use and provides no specific features by itself.
!--------------------------------------------------------------------------------------
! Revision History
! 2024.08.10	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orHookWriteList_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDED DEPENDENCIES

!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orHookWriteList_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orHookWriteList_STAGE  < LIBRARY_STAGE);
   #undef      orHookWriteList_STAGE  ;
   Constant    orHookWriteList_STAGE  LIBRARY_STAGE;
   #ifdef      orHookWriteList_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orHookWriteList..."; #endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	replace WriteListR;
    property individual ext_wlrPluralMany;
#endif; !--Before Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   [ WriteListR o depth stack_pointer  classes_p sizes_p i j k k2 l m n q senc mr;
    if (depth > 0 && o == child(parent(o))) {
        SortOutList(o);
        o = child(parent(o));
    }
    for (::) {
        if (o == 0) rfalse;
        if (c_style & WORKFLAG_BIT && depth==0 && o hasnt workflag) {
            o = sibling(o);
            continue;
        }
        if (c_style & CONCEAL_BIT && (o has concealed || o has scenery)) {
            o = sibling(o);
            continue;
        }
        break;
    }
    classes_p = match_classes + stack_pointer;
    sizes_p   = match_list + stack_pointer;

    for (i=o,j=0 : i && (j+stack_pointer)<128 : i=NextEntry(i,depth),j++) {
        classes_p->j = 0;
        if (i.plural) k++;
    }

    if (c_style & ISARE_BIT) {
        if (j == 1 && o hasnt pluralname) Tense(IS__TX, WAS__TX);
        else                              Tense(ARE__TX, WERE__TX);
        if (c_style & NEWLINE_BIT)   print (string) COLON__TX, "^";
        else                         print (char) ' ';
        c_style = c_style - ISARE_BIT;
    }

    stack_pointer = stack_pointer+j+1;

    if (k < 2) jump EconomyVersion;   ! It takes two to plural
    n = 1;
    for (i=o,k=0 : k<j : i=NextEntry(i,depth),k++)
        if (classes_p->k == 0) {
            classes_p->k = n; sizes_p->n = 1;
            for (l=NextEntry(i,depth),m=k+1 : l && m<j : l=NextEntry(l,depth),m++)
                if (classes_p->m == 0 && i.plural && l.plural ~= 0) {
                    if (ListEqual(i, l) == 1) {
                        sizes_p->n = sizes_p->n + 1;
                        classes_p->m = n;
                    }
                }
            n++;
        }
    n--;

    for (i=1,j=o,k=0 : i<=n : i++,senc++) {
        while (((classes_p->k) ~= i) && ((classes_p->k) ~= -i)) {
            k++; j=NextEntry(j, depth);
        }
        m = sizes_p->i;
        if (j == 0) mr = 0;
        else {
            if (j.list_together ~= 0 or lt_value && metaclass(j.list_together) == Routine or String &&
                j.list_together == mr) senc--;
            mr = j.list_together;
        }
    }
    senc--;

    for (i=1,j=o,k=0,mr=0 : senc>=0 : i++,senc--) {
        while (((classes_p->k) ~= i) && ((classes_p->k) ~= -i)) {
            k++; j=NextEntry(j, depth);
        }
        if (j.list_together ~= 0 or lt_value) {
            if (j.list_together == mr) {
                senc++;
                jump Omit_FL2;
            }
            k2 = NextEntry(j, depth);
            if (k2 == 0 || k2.list_together ~= j.list_together) jump Omit_WL2;
            k2 = metaclass(j.list_together);
            if (k2 == Routine or String) {
                q = j; listing_size = 1; l = k; m = i;
                while (m < n && q.list_together == j.list_together) {
                    m++;
                    while (((classes_p->l) ~= m) && ((classes_p->l) ~= -m)) {
                        l++; q = NextEntry(q, depth);
                     }
                    if (q.list_together == j.list_together) listing_size++;
                }
                ! print " [", listing_size, "] ";
                if (listing_size == 1) jump Omit_WL2;
                if (c_style & INDENT_BIT) Print__Spaces(2*(depth+wlf_indent));
                if (k2 == String) {
                    q = 0;
                    for (l=0 : l<listing_size : l++) q = q+sizes_p->(l+i);

                    if(LibraryExtensions.RunWhile(ext_wlrPluralMany, false, j, q)==false) { !--new hook
                        EnglishNumber(q);
                    }
                    print " ";
                    print (string) j.list_together;
                    if (c_style & ENGLISH_BIT) print " (";
                    if (c_style & INDENT_BIT)  print (string) COLON__TX, "^";
                }
                q = c_style;
                if (k2 ~= String) {
                    inventory_stage = 1;
                    parser_one = j; parser_two = depth+wlf_indent;
                    if (RunRoutines(j, list_together) == 1) jump Omit__Sublist2;
                }

                #Ifdef TARGET_ZCODE;
                @push lt_value;    @push listing_together;    @push listing_size;
                #Ifnot; ! TARGET_GLULX;
                @copy lt_value sp; @copy listing_together sp; @copy listing_size sp;
                #Endif; ! TARGET_;

                lt_value = j.list_together; listing_together = j; wlf_indent++;
                WriteListR(j, depth, stack_pointer); wlf_indent--;

                #Ifdef TARGET_ZCODE;
                @pull listing_size; @pull listing_together; @pull lt_value;
                #Ifnot; ! TARGET_GLULX;
                @copy sp listing_size;
                @copy sp listing_together;
                @copy sp lt_value;
                #Endif; ! TARGET_;

                if (k2 == String) {
                    if (q & ENGLISH_BIT) print ")";
                }
                else {
                    inventory_stage = 2;
                    parser_one = j; parser_two = depth+wlf_indent;
                    RunRoutines(j, list_together);
                }

              .Omit__Sublist2;

                if (q & NEWLINE_BIT && c_style & NEWLINE_BIT == 0) new_line;
                c_style = q;
                mr = j.list_together;
                jump Omit_EL2;
            }
        }

      .Omit_WL2;

        if (WriteBeforeEntry(j, depth, 0, senc) == 1) jump Omit_FL2;
        if (sizes_p->i == 1) {
            if (c_style & NOARTICLE_BIT)  print (name) j;
            else {
                if (c_style & DEFART_BIT) print (the) j;
                else                      print (a) j;
            }
            if (c_style & ID_BIT)         print " (", j, ")";
        }
        else {
            if (c_style & DEFART_BIT) PrefaceByArticle(j, 1, sizes_p->i);

            if(LibraryExtensions.RunWhile(ext_wlrPluralMany, false, j, sizes_p->i)==false) { !--new hook
                print (number) sizes_p->i;
            }
            print " ";
            PrintOrRun(j, plural, 1);
        }
        if (sizes_p->i > 1 && j hasnt pluralname) {
            give j pluralname;
            WriteAfterEntry(j, depth, stack_pointer);
            give j ~pluralname;
        }
        else {
            WriteAfterEntry(j,depth,stack_pointer);
        }
      .Omit_EL2;

        if (c_style & ENGLISH_BIT) {
            if (senc == 1) print (SerialComma) i+senc, (string) AND__TX;
            if (senc > 1)  print (string) COMMA__TX;
        }
     .Omit_FL2;
    }
    rtrue;

  .EconomyVersion;

    n = j;
    for (i=1,j=o : i<=n : j=NextEntry(j,depth),i++,senc++) {
        if (j.list_together ~= 0 or lt_value && metaclass(j.list_together) == Routine or String &&
            j.list_together==mr) senc--;
        mr = j.list_together;
    }

    for (i=1,j=o,mr=0 : i<=senc : j=NextEntry(j,depth),i++) {
        if (j.list_together ~= 0 or lt_value) {
            if (j.list_together == mr) {
                i--;
                jump Omit_FL;
            }
            k = NextEntry(j, depth);
            if (k == 0 || k.list_together ~= j.list_together) jump Omit_WL;
            k = metaclass(j.list_together);
            if (k == Routine or String) {
                if (c_style & INDENT_BIT) Print__Spaces(2*(depth+wlf_indent));
                if (k == String) {
                    q = j; l = 0;
                    do {
                        q = NextEntry(q, depth); l++;
                    } until (q == 0 || q.list_together ~= j.list_together);
                    EnglishNumber(l); print " ";
                    print (string) j.list_together;
                    if (c_style & ENGLISH_BIT) print " (";
                    if (c_style & INDENT_BIT) print (string) COLON__TX, "^";
                }
                q = c_style;
                if (k ~= String) {
                    inventory_stage = 1;
                    parser_one = j; parser_two = depth+wlf_indent;
                    if (RunRoutines(j, list_together) == 1) jump Omit__Sublist;
                }

                #Ifdef TARGET_ZCODE;
                @push lt_value; @push listing_together; @push listing_size;
                #Ifnot; ! TARGET_GLULX;
                @copy lt_value sp; @copy listing_together sp; @copy listing_size sp;
                #Endif; ! TARGET_;

                lt_value = j.list_together; listing_together = j; wlf_indent++;
                WriteListR(j, depth, stack_pointer); wlf_indent--;

                #Ifdef TARGET_ZCODE;
                @pull listing_size; @pull listing_together; @pull lt_value;
                #Ifnot; ! TARGET_GLULX;
                @copy sp listing_size; @copy sp listing_together; @copy sp lt_value;
                #Endif; ! TARGET_;

                if (k == String) {
                    if (q & ENGLISH_BIT) print ")";
                }
                else {
                    inventory_stage = 2;
                    parser_one = j; parser_two = depth+wlf_indent;
                    RunRoutines(j, list_together);
                }

              .Omit__Sublist;

                if (q & NEWLINE_BIT && c_style & NEWLINE_BIT == 0) new_line;
                c_style = q;
                mr = j.list_together;
                jump Omit_EL;
            }
        }

      .Omit_WL;

        if (WriteBeforeEntry(j, depth, i, senc) == 1) jump Omit_FL;
        if (c_style & NOARTICLE_BIT)  print (name) j;
        else {
            if (c_style & DEFART_BIT) print (the) j;
            else                      print (a) j;
        }
        if (c_style & ID_BIT)         print " (", j, ")";
        WriteAfterEntry(j, depth, stack_pointer);

      .Omit_EL;

        if (c_style & ENGLISH_BIT) {
            if (i == senc-1) print (SerialComma) senc, (string) AND__TX;
            if (i < senc-1) print (string) COMMA__TX;
        }

  .Omit_FL;

    }
]; ! end of WriteListR
#endif; !--After AFTER_VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
