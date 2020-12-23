PROGS = quadratic_test sum_array_test find_max_test sort_test fib_iter_test fib_rec_test strlen_test
OBJS = quadratic_c.o quadratic_s.o sum_array_c.o sum_array_s.o find_max_c.o find_max_s.o sort_c.o sort_s.o fib_iter_c.o fib_iter_s.o fib_rec_c.o fib_rec_s.o strlen_c.o strlen_s.o

CFLAGS = -g

%.o : %.c
	gcc ${CFLAGS} -c -o $@ $<

%.o : %.s
	as -o $@ $<


all : ${PROGS}

quadratic_test : quadratic_test.c quadratic_c.o quadratic_s.o
	gcc ${CFLAGS} -o $@ $^

sum_array_test : sum_array_test.c sum_array_c.o sum_array_s.o
	gcc ${CFLAGS} -o $@ $^

find_max_test : find_max_test.c find_max_s.o find_max_c.o
	gcc ${CFLAGS} -o $@ $^

sort_test : sort_test.c sort_s.o sort_c.o
	gcc ${CFLAGS} -o $@ $^

fib_iter_test : fib_iter_test.c fib_iter_c.o fib_iter_s.o
	gcc ${CFLAGS} -o $@ $^

fib_rec_test : fib_rec_test.c fib_rec_c.o fib_rec_s.o
	gcc ${CFLAGS} -o $@ $^

strlen_test : strlen_test.c strlen_s.o strlen_c.o
	gcc ${CFLAGS} -o $@ $^


clean :
	rm -rf ${PROGS} ${OBJS} *~

test : 
	@echo Building test files...
	@make -s
	@echo ===================Tests Start===========================
	@echo ===================Quadratic Test========================
	@./quadratic_test
	@echo ===================Sum Array Test========================
	@./sum_array_test
	@echo ===================Find Max Test=========================
	@./find_max_test
	@echo ===================Simple Sort Test======================
	@./sort_test
	@echo ===================Fib Iter Test=========================
	@./fib_iter_test
	@echo ===================Fib Rec Test==========================
	@./fib_rec_test
	@echo ===================Strlen Test===========================
	@./strlen_test
	@echo ===================Tests End=============================
	@make -s clean
