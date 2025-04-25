#Tests

1.arg count

- length not present
- no extra parameters allowed

  2.invalid length

- '<1'
- '> MAX_INT'
- not int

  3.len/cost input tests

- at least one
- no duplicates
- both integers >=1
- nothing but "num,"num" on line
- space after comma optional
- can input lots

  4.unusual situations

- length < any piece : len 10 20,20 -> 0 remainder 10

  5.no remainder

- no dupes: len 50, 2/4 6/4 -> 25@2 remainder: 0
- single len: len 50, 5/5 = 10@5 remainder:0
- multi pieces: len 30, 2/3 4/7 1/1 18/30 -> 7@4 1@2 remainder: 0

  6.remainder

- one piece: len 20 6/3 -> 3@6 remainder: 2
- mult piece: len 50 17/20 20/30 19/21 -> 2@20 remainder: 10


1.Arg count

- length not present
- no extra params

2.invalid rod length

- '<1'
- '> MAX_INT'
- not int

3.Evict from least recent

- Input rod length:
10
least_recently_used.c caching_provider(0x7fffeafef2a0 10)
2 @ 5 = 10
Remainder: 0
Value: 10
least_recently_used.cstatistics()
requests: 1
hits: 0
misses: 1
evictions: 0

20
least_recently_used.c caching_provider(0x7fffeafef2a0 20)
1 @ 17 = 20
Remainder: 3
Value: 20
least_recently_used.cstatistics()
requests: 2
hits: 0
misses: 2
evictions: 0

30
least_recently_used.c caching_provider(0x7fffeafef2a0 30)
1 @ 17 = 20
2 @ 4 = 8
1 @ 5 = 5
Remainder: 0
Value: 33
least_recently_used.cstatistics()
requests: 3
hits: 0
misses: 3
evictions: 0

40
least_recently_used.c caching_provider(0x7fffeafef2a0 40)
Evicting index 0
2 @ 17 = 40
1 @ 5 = 5
Remainder: 1
Value: 45
least_recently_used.cstatistics()
requests: 4
hits: 0
misses: 4
evictions: 1

4.Evict least frequent

-Input rod length:
10
least_frequently_used.c caching_provider(0x7fffc79e12a0 10)
2 @ 5 = 10
Remainder: 0
Value: 10
least_frequently_used.cstatistics()
requests: 1
hits: 0
misses: 1
evictions: 0

10
least_frequently_used.c caching_provider(0x7fffc79e12a0 10)
2 @ 5 = 10
Remainder: 0
Value: 10
least_frequently_used.cstatistics()
requests: 2
hits: 1
misses: 1
evictions: 0

20
least_frequently_used.c caching_provider(0x7fffc79e12a0 20)
1 @ 17 = 20
Remainder: 3
Value: 20
least_frequently_used.cstatistics()
requests: 3
hits: 1
misses: 2
evictions: 0

20
least_frequently_used.c caching_provider(0x7fffc79e12a0 20)
1 @ 17 = 20
Remainder: 3
Value: 20
least_frequently_used.cstatistics()
requests: 4
hits: 2
misses: 2
evictions: 0

30
least_frequently_used.c caching_provider(0x7fffc79e12a0 30)
1 @ 17 = 20
2 @ 4 = 8
1 @ 5 = 5
Remainder: 0
Value: 33
least_frequently_used.cstatistics()
requests: 5
hits: 2
misses: 3
evictions: 0

40
least_frequently_used.c caching_provider(0x7fffc79e12a0 40)
Evicting index 2
2 @ 17 = 40
1 @ 5 = 5
Remainder: 1
Value: 45
least_frequently_used.cstatistics()
requests: 6
hits: 2
misses: 4
evictions: 1
