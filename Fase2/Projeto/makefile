all: Monitor Simulador
Monitor: Monitor.o
	gcc -o Monitor Monitor.o
Simulador: Simulador.o
	gcc -o Simulador Simulador.o 
Monitor.o: Monitor.c
	gcc -c Monitor.c
Simulador.o: Simulador.c
	gcc -c Simulador.c

clean:
	rm *.o
	rm Simulador
	rm Monitor
	rm Relatorio
	

