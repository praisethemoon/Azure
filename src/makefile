azure: aconlin.o acorelib.o acoroutine.o adllin.o azure.o aerror.o\
	aexception.o agc.o agen.o alex.o alib.o alibio.o alibstr.o alibsys.o\
	alibmath.o aparse.o aregex.o atable.o autil.o avm.o main.o
	cc aconlin.o acorelib.o acoroutine.o adllin.o azure.o aerror.o\
	aexception.o agc.o agen.o alex.o alib.o alibio.o alibstr.o alibsys.o\
	alibmath.o aparse.o aregex.o atable.o autil.o avm.o main.o\
	-o azure -lm -ldl
aconlin.o: aconlin.c aconsole.h
	cc -c -ansi aconlin.c
acorelib.o: acorelib.c
	cc -c -ansi acorelib.c
acoroutine.o: acoroutine.c acoroutine.h
	cc -c -ansi acoroutine.c
adllin.o: adllin.c
	cc -c -ansi adllin.c
azure.o: azure.c azure.h
	cc -c -ansi azure.c
aerror.o: aerror.c aerror.h
	cc -c -ansi aerror.c
aexception.o: aexception.c aexception.h
	cc -c -ansi aexception.c
agc.o: agc.c agc.h
	cc -c -ansi agc.c
agen.o: agen.c agen.h
	cc -c -ansi agen.c
alex.o: alex.c alex.h
	cc -c -ansi alex.c
alib.o: alib.c alib.h
	cc -c -ansi alib.c
alibio.o: alibio.c
	cc -c -ansi alibio.c
alibmath.o: alibmath.c
	cc -c -ansi alibmath.c
alibstr.o: alibstr.c
	cc -c -ansi alibstr.c
alibsys.o: alibsys.c
	cc -c -ansi alibsys.c
aparse.o: aparse.c aparse.h
	cc -c -ansi aparse.c
aregex.o: aregex.c aregex.h
	cc -c -ansi aregex.c
atable.o: atable.c atable.h
	cc -c -ansi atable.c
autil.o: autil.c autil.h
	cc -c -ansi autil.c
avm.o: avm.c avm.h
	cc -c -ansi avm.c
main.o: main.c
	cc -c -ansi main.c
clean:
	rm *.o
