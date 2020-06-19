LCC?=lcc -v -Wa-l -Wl-m -Wl-j
MKROM?=$(LCC)
CC=$(LCC) -c $(CFLAGS)
CA=$(LCC) -c
EMU?=sameboy
ROM=main.gb

.PHONY:
build: $(ROM)

%.gb: %.rel
	$(MKROM) -o $@ $^

%.s: %.c
	$(CC) -S -o $@ $^

%.rel: %.s
	$(CA) -o $@ $^

run: $(ROM)
	$(EMU) $<

.PHONY:
clean:
	find . -type f -regex '.*.\(gb\|o\|rel\|sym\|map\|lst\|s\)' -delete