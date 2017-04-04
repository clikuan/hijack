CC = gcc
LIBRARY_NAME = lcm.so
lib: lcm.c
	${CC} -o ${LIBRARY_NAME} -shared -fPIC $^ -ldl
clean:
	rm -f ${LIBRARY_NAME}
