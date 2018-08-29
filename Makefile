OBJ =  main.o ./b_kv/src/b_kv.o ./b_kv/port/b_kv_port.o
INCLUDE = -I ./b_kv/inc/ -I ./b_kv/port/


TEST: $(OBJ)
	gcc -o $@ $^

%.o: %.c
	gcc  $(INCLUDE) -c $< -o $@

clean:
	rm -rf *.o TEST ./b_kv/src/*.o ./b_kv/port/*.o
	