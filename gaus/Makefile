libge: matrix.o pivot.o piv_ge_solver.o
	ar rvs libge.a $^

matrix.o: matrix.h 
	cc -c matrix.c -g
pivot.o: matrix.h
	cc -c pivot.c -g
piv_ge_solver.o: piv_ge_solver.h matrix.h
	cc -c piv_ge_solver.c -g


.PHONY: clean

clean:
	-rm *.o libge.a
