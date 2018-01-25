all:	flotestgen

flotestgen:	flopoco_test_generator.c
	gcc -o $@ $< -lgmp -Wall

do:	
	./flotestgen input_sample.csv 5 10

clean:
	rm -rf flotestgen *.out *~