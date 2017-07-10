CC = cc
#CPP = cc -E
LINK = $(CC)
INCS = -I .
LIBS = -pthread -lrt
CFLAGS = -pipe -O -W -Wall -Wpointer-arith -Wno-unused-parameter -Werror -g
OBJS = ../objs


all:$(OBJS)/daemon.o $(OBJS)/worker.o $(OBJS)/log.o $(OBJS)/dlib.o $(OBJS)/dstring.o $(OBJS)/dsignal.o $(OBJS)/dprocess.o $(OBJS)/dsystem.o

	#mydaemond
	$(LINK) -o $(OBJS)/mydaemond $(OBJS)/daemon.o $(OBJS)/log.o $(OBJS)/dlib.o $(OBJS)/dstring.o $(OBJS)/dsignal.o   \
			$(OBJS)/dprocess.o $(OBJS)/dsystem.o $(LIBS)
	#client
	$(LINK) -o $(OBJS)/worker    $(OBJS)/worker.o $(OBJS)/log.o $(OBJS)/dlib.o $(OBJS)/dstring.o $(OBJS)/dsignal.o   \
			$(OBJS)/dprocess.o $(OBJS)/dsystem.o $(LIBS)


$(OBJS)/daemon.o:daemon.c
	$(CC) -c $(CFLAGS) $(INCS) daemon.c  	-o $(OBJS)/daemon.o
$(OBJS)/worker.o:worker.c
	$(CC) -c $(CFLAGS) $(INCS) worker.c  	-o $(OBJS)/worker.o
$(OBJS)/log.o:log.c
	$(CC) -c $(CFLAGS) $(INCS) log.c     	-o $(OBJS)/log.o
$(OBJS)/dlib.o:dlib.c
	$(CC) -c $(CFLAGS) $(INCS) dlib.c    	-o $(OBJS)/dlib.o
$(OBJS)/dstring.o:dstring.c
	$(CC) -c $(CFLAGS) $(INCS) dstring.c 	-o $(OBJS)/dstring.o
$(OBJS)/dsignal.o:dsignal.c
	$(CC) -c $(CFLAGS) $(INCS) dsignal.c 	-o $(OBJS)/dsignal.o
$(OBJS)/dprocess.o:dprocess.c
	$(CC) -c $(CFLAGS) $(INCS) dprocess.c 	-o $(OBJS)/dprocess.o
$(OBJS)/dsystem.o:dsystem.c
	$(CC) -c $(CFLAGS) $(INCS) dsystem.c  	-o $(OBJS)/dsystem.o

.PHONE:clean
clean:
	rm -rf $(OBJS)

