bats = bats/bats-core/bin/bats

# Build
# -----

hw: hw.c
	gcc -o $@ $<

motzkin: motzkin.c
	gcc -o $@ $<

# Test
# ----

test: test-hw

test-hw: hw hw.bats
	$(bats) hw.bats
