LCC?=lcc -v -Wa-l -Wl-m -Wl-j
MKROM?=$(LCC)
CC=$(LCC) -c $(CFLAGS)
CA=$(LCC) -c
EMU?=sameboy
ROM=main.gb

.PHONY:
build: $(ROM)

main.gb: main.rel scanlines.rel
	$(MKROM) -o $@ $^

%.gb: %.rel
	$(MKROM) -o $@ $^


%.rel: %.s
	$(CA) -o $@ $^

.SECONDARY:
%.s: %.c
	$(CC) -S -o $@ $^

.PHONY:
run: $(ROM)
	$(EMU) $<

.PHONY:
clean:
	find . -type f -regex '.*.\(gb\|o\|rel\|sym\|map\|lst\|s\)' -delete