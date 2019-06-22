PUBLICH = macros.h list.h
PRIVATEH = privatestructs.h
SCHEDULE = schedule.c schedule.h

.PHONY: default
default: clear app

.PHONY: clear
clear:
	clear; clear
	
.PHONY: clean
clean:
	rm -f *.o
	rm -f *.gch

app: cpu.o cpuinit.o  schedule.o
	gcc -o app cpuinit.o cpu.o schedule.o

cpu.o: cpu.c $(SCHEDULE) $(PUBLICH) $(PRIVATEH)
	gcc -c cpu.c

cpuinit.ot: cpuinit.c $(SCHEDULE) $(PUBLICH) $(PRIVATEH)
	gcc -c cpuinit.c

schedule.o: $(SCHEDULE) $(PUBLICH)
	gcc -c schedule.c
