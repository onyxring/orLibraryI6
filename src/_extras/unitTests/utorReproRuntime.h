!---------------------------------------------------------------------------
! utorReproRuntime.h
! Reproducer for the Beguile stringRuntime equality bug, using raw orLib
! orString directly. If this passes when run via unitTestRunner.inf, the
! bug is in Beguile's emission scaffolding, not in orLib's pool semantics.
!---------------------------------------------------------------------------
#ifndef        orExtensionFramework_STAGE;
#ifndef        utorReproRuntime_STAGE;
default        utorReproRuntime_STAGE  0;
#include "_orUnitTest";
#include "orString";
#endif;
#ifnot;
#ifndef        utorReproRuntime_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorReproRuntime_STAGE  < LIBRARY_STAGE);
   #undef      utorReproRuntime_STAGE  ;
   Constant    utorReproRuntime_STAGE  LIBRARY_STAGE;
   #ifdef      utorReproRuntime_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorReproRuntime...";#endif;

#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

global g_reproTotal = 0;
global g_reproPassed = 0;

[ rAssertTrue name cond;
    g_reproTotal++;
    if(cond) {
        g_reproPassed++;
    } else {
        print "FAIL: ", (string)name, "^";
    }
];

! Mirrors Beguile stringRuntime.bgl's testEquality and surrounding tests.
orUnitTest "orStringReproRuntime"
    with tests [s o t u sa sb sc app;
        ! testAssignment
        s = util.orStr.new();
        s.set("hello");
        rAssertTrue("assign-literal", util.orStr.areEqual(s, "hello"));
        t = util.orStr.new();
        t.set(s);
        rAssertTrue("assign-string", util.orStr.areEqual(s, t));
        u = util.orStr.new();
        u.set("");
        rAssertTrue("assign-empty", u.isEmpty());
        s.free(); t.free(); u.free();

        ! testEquality — THE failing pattern
        s = util.orStr.new(); s.set("test");
        rAssertTrue("eq-literal-true",  util.orStr.areEqual(s, "test"));
        rAssertTrue("eq-literal-false", ~~util.orStr.areEqual(s, "other"));
        o = util.orStr.new(); o.set("test");
        rAssertTrue("eq-string-true",   util.orStr.areEqual(s, o));
        o.set("different");
        rAssertTrue("eq-string-false",  ~~util.orStr.areEqual(s, o));
        s.free(); o.free();

        ! testConcatenation
        sa = util.orStr.new(); sa.set("foo");
        sb = util.orStr.new(); sb.set("bar");
        sc = util.orStr.new(); sc.set(sa); sc.set(sc.append(sb));
        rAssertTrue("concat", util.orStr.areEqual(sc, "foobar"));
        sa.free(); sb.free(); sc.free();

        ! testAppendPrepend
        s = util.orStr.new(); s.set("abc");
        app = s.append("XYZ");
        rAssertTrue("append-string", util.orStr.areEqual(app, "abcXYZ"));
        s.free();

        ! Final score in the unit-test summary
        print "REPRO_RESULT: ", g_reproPassed, "/", g_reproTotal, " passed^";
    ]
    "REPRO_RESULT:"   ! orUnitTest expects an expected-output marker
;

#endif; !--AFTER_VERBLIB
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
