cflags := -Wall -std=c17 -Iinclude -Ilib
sobjs := build/arr.o build/list.o build/que.o build/vec.o
aobjs := build/algo/search.o
shared_sobjs := build/shared/arr.o build/shared/list.o build/shared/que.o build/shared/vec.o
shared_aobjs := build/shared/algo/search.o

init:
	mkdir -p build
	mkdir -p install
	mkdir -p build/algo
	mkdir -p build/test
	mkdir -p build/shared
	mkdir -p build/shared/algo
	python3 tools/pec/pec.py .ec include/ec.h _LJW_SEQLIB_EC_H_

build: build/libseqlib.a build/libseqlib.so
tbuild: build/libseqlib.a
test: build/test/algo build/test/arr build/test/list build/test/que build/test/vec
	./build/test/algo sort 72 44 11 23 55 66 77 88 99 72 23
	./build/test/algo search 1 2 5 33 46 79 80 83 92
	./build/test/arr 10 72 44 11 23 55 66 77 88 99 72
	./build/test/list 72 44 11 23 55 66
	./build/test/que 5 72 44 11 23 55 66
	./build/test/vec 10 72 44 11 23 55 66 77 88 99 72 23
install: build/libseqlib.a build/libseqlib.so build/libseqlib.so.a .sym
	mkdir -p install/include
	cp -r include/* install/include/
	cp $^ install/
	cd install && zip -r seqlib.zip include/ *.a *.so .sym

build/libseqlib.a: build/algo.o build/struct.o lib/aLocas/libaLocas.a lib/cmth/libcmth.a
	${LD} -r $^ -o build/libseqlib.o
	objcopy --localize-symbols=lib/aLocas/.sym build/libseqlib.o
	objcopy --localize-symbols=lib/cmth/.sym build/libseqlib.o
	${AR} rcs $@ build/libseqlib.o
build/libseqlib.so: build/shared/struct.o build/shared/algo.o lib/cmth/libcmth.so.a lib/aLocas/libaLocas.so.a
	${CC} -shared -Wl,--version-script=.map $^ -o $@
build/libseqlib.so.a: build/shared/struct.o build/shared/algo.o
	${AR} rcs $@ $^
build/test/%: test/%.c build/libseqlib.a
	${CC} ${cflags} $^ -o $@

build/algo.o: build/algo/search.o
	${LD} -r $^ -o $@
build/algo/%.o: src/algo/%.c
	${CC} ${cflags} $< -c -o $@
	nm -g $@ | awk '{print $$3}' >> .sym
build/struct.o: ${sobjs}
	${LD} -r $^ -o $@
build/%.o: src/%.c
	${CC} ${cflags} $< -c -o $@
	nm -g $@ | awk '{print $$3}' >> .sym
build/shared/struct.o: ${shared_sobjs}
	${LD} -r $^ -o $@
build/shared/algo.o: ${shared_aobjs}
	${LD} -r $^ -o $@
build/shared/%.o: src/%.c
	${CC} ${cflags} $< -fPIC -c -o $@
build/shared/algo/%.o: src/algo/%.c
	${CC} ${cflags} $< -fPIC -c -o $@

clean:
	rm -rf build
	rm .sym

.PHONY: build tbuild test install clean init build/lib/aLocas build/lib/cmth

