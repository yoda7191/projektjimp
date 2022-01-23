CC_O = gcc -o
CC_I = gcc
DEL = rm -r *.o

test: main.o splines.o points.o wielomianbaza4.o aproksymator_na_bazie.o gaus/libge.a
	$(CC_O) polytest main.o splines.o points.o wielomianbaza4.o -L gaus -l ge -lm 
	$(CC_O) aproxtest main.o splines.o points.o aproksymator_na_bazie.o -L gaus -l ge 
	$(DEL)
	./polytest -s polytest.txt -p test/dane.1 -g plotpolytest -f 5.1 -t 5.7 -n 300
	./aproxtest -s aproxtest.txt -p test/dane.1 -g plotaproxtest -f 5.1 -t 5.7 -n 300

poly: main.o splines.o points.o wielomianbaza4.o gaus/libge.a
	$(CC_O) poly main.o splines.o points.o wielomianbaza4.o -L gaus -l ge -lm -g
	$(DEL)

aprox: main.o splines.o points.o aproksymator_na_bazie.o gaus/libge.a
	$(CC_O) aprox main.o splines.o points.o aproksymator_na_bazie.o -L gaus -l ge -g
	$(DEL)

intrp: main.o splines.o points.o interpolator.o gaus/libge.a
	$(CC_O) intrp main.o splines.o points.o interpolator.o -L gaus -l ge -g
	$(DEL)

prosta: main.o splines.o points.o prosta.o
	$(CC_O) prosta main.o splines.o points.o prosta.o -g
	$(DEL)

wielomianbaza4.o: makespl.h points.h gaus/piv_ge_solver.h 
	$(CC_I) gaus -c wielomianbaza4.c -lm -g

aproksymator_na_bazie.o: makespl.h points.h gaus/piv_ge_solver.h
	$(CC_I) gaus -c aproksymator_na_bazie.c -g

interpolator.o: makespl.h points.h gaus/piv_ge_solver.h -g
	$(CC_I) gaus -c interpolator.c -g

main.o:
	$(CC_I) -c main.c -g

splines.o:
	$(CC_I) -c splines.c -g 

points.o:
	$(CC_I) -c points.c -g 


