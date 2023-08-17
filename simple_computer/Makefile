TARGET_MAIN = ./test/display
TARGET_TEST = ./test/myReadkey_test
TARGET_LIB1 = ./mySimpleComputer/libmySimpleComputer.a
TARGET_LIB2 = ./myTerm/libmyTerm.a
TARGET_LIB3 = ./myBigChars/libmyBigChars.a
TARGET_LIB4 = ./myReadkey/libmyReadkey.a
CC = gcc
CFLAGS = -Wall -Wextra -O0 -g -I./mySimpleComputer -I./myTerm -I./myBigChars -I./myReadkey
.PHONY : clean

$(TARGET_MAIN) : ./test/display.o $(TARGET_LIB1) $(TARGET_LIB2) $(TARGET_LIB3) $(TARGET_LIB4) $(TARGET_TEST)
	$(CC) $(CFLAGS) -o $@ ./test/display.o -L./mySimpleComputer -lmySimpleComputer -L./myTerm -lmyTerm -L./myBigChars -lmyBigChars -L./myReadkey -lmyReadkey

./test/display.o : ./test/display.c
	$(CC) -c $(CFLAGS) -o $@ $^


$(TARGET_TEST) : ./test/myReadkey_test.o
	$(CC) $(CFLAGS) -o $@ $^ -L./myReadkey -lmyReadkey -L./myTerm -lmyTerm -L./mySimpleComputer -lmySimpleComputer

./test/myReadkey_test.o : ./test/myReadkey_test.c
	$(CC) -c $(CFLAGS) -o $@ $^


$(TARGET_LIB1) : ./mySimpleComputer/libmySimpleComputer.o
	ar rcs $@ $^

./mySimpleComputer/libmySimpleComputer.o : ./mySimpleComputer/mySimpleComputer.c
	$(CC) -c $(CFLAGS) -o $@ $^


$(TARGET_LIB2) : ./myTerm/libmyTerm.o
	ar rcs $@ $^	

./myTerm/libmyTerm.o : ./myTerm/myTerm.c
	$(CC) -c $(CFLAGS) -o $@ $^


$(TARGET_LIB3) : ./myBigChars/libmyBigChars.o
	ar rcs $@ $^	

./myBigChars/libmyBigChars.o : ./myBigChars/myBigChars.c
	$(CC) -c $(CFLAGS) -o $@ $^ -L./myTerm -lmyTerm


$(TARGET_LIB4) : ./myReadkey/libmyReadkey.o
	ar rcs $@ $^	

./myReadkey/libmyReadkey.o : ./myReadkey/myReadkey.c
	$(CC) -c $(CFLAGS) -o $@ $^


run : $(TARGET_MAIN) $(TARGET_TEST)
	$(TARGET_TEST)

run_display : $(TARGET_MAIN) $(TARGET_LIB1) $(TARGET_LIB2) $(TARGET_LIB3) $(TARGET_LIB4)
	./test/display

clean :
	rm $(TARGET_MAIN) $(TARGET_TEST) $(TARGET_LIB1) $(TARGET_LIB2) $(TARGET_LIB3) $(TARGET_LIB4) ./mySimpleComputer/libmySimpleComputer.o ./myTerm/libmyTerm.o ./myBigChars/libmyBigChars.o ./myReadkey/libmyReadkey.o ./test/myReadkey_test.o ./test/display.o 