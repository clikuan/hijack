CC = gcc
LIBRARY_NAME = hw2.so
lib: lcm.c
	${CC} -o ${LIBRARY_NAME} -shared -fPIC $^ -ldl
	strip -s ${LIBRARY_NAME}
clean:
	rm -f ${LIBRARY_NAME}
