#!/usr/bin/python2

grammar = """
0	s2	 	 	 	 	 	 	 	 	 	 	 	 	 	1	 	 	 	 	 
1	 	 	 	 	 	 	 	 	 	 	 	 	acc	 	 	 	 	 	 	 
2	 	 	 	s4	 	 	 	 	 	 	 	 	 	 	 	3	 	 	 	 
3	 	s5	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 
4	 	 	 	 	 	 	 	s7	 	 	s9	s8	 	 	 	 	 	 	 	6
5	 	 	r4	 	 	 	 	s12	 	 	 	 	 	 	 	 	10	11	 	 
6	 	 	 	 	s13	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 
7	 	 	 	 	r9	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 
8	 	 	 	 	r10	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 
9	 	 	 	 	 	 	 	s7	 	 	s9	s8	 	 	 	 	 	 	 	14
10	 	 	s15	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 
11	 	 	 	 	 	 	s16	 	 	 	 	 	 	 	 	 	 	 	 	 
12	 	 	 	 	 	 	 	 	s17	 	 	 	 	 	 	 	 	 	 	 
13	 	 	 	 	 	 	 	s19	 	 	s21	s20	 	 	 	 	 	 	 	18
14	 	 	 	 	r11	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 
15	 	 	 	 	 	 	 	 	 	 	 	 	r1	 	 	 	 	 	 	 
16	 	 	r4	 	 	 	 	s12	 	 	 	 	 	 	 	 	22	11	 	 
17	 	 	 	 	 	 	 	s25	 	 	s27	s26	 	 	 	 	 	 	23	24
18	 	 	 	 	 	s28	 	 	 	 	 	 	 	 	 	 	 	 	 	 
19	 	 	 	 	 	r9	 	 	 	 	 	 	 	 	 	 	 	 	 	 
20	 	 	 	 	 	r10	 	 	 	 	 	 	 	 	 	 	 	 	 	 
21	 	 	 	 	 	 	 	s19	 	 	s21	s20	 	 	 	 	 	 	 	29
22	 	 	r3	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 
23	 	 	 	 	 	 	r5	 	 	 	 	 	 	 	 	 	 	 	 	 
24	 	 	 	 	 	 	r8	 	 	s30	s31	 	 	 	 	 	 	 	 	 
25	 	 	 	 	 	 	r9	 	 	r9	r9	 	 	 	 	 	 	 	 	 
26	 	 	 	 	 	 	r10	 	 	r10	r10	 	 	 	 	 	 	 	 	 
27	 	 	 	 	 	 	 	s25	 	 	s27	s26	 	 	 	 	 	 	 	32
28	 	r2	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 
29	 	 	 	 	 	r11	 	 	 	 	 	 	 	 	 	 	 	 	 	 
30	 	 	 	 	 	 	 	s34	 	 	s36	s35	 	 	 	 	 	 	 	33
31	 	 	 	 	 	 	 	s34	 	 	s36	s35	 	 	 	 	 	 	 	37
32	 	 	 	 	 	 	r11	 	 	r11	r11	 	 	 	 	 	 	 	 	 
33	 	 	 	 	 	 	r6	 	 	 	 	 	 	 	 	 	 	 	 	 
34	 	 	 	 	 	 	r9	 	 	 	 	 	 	 	 	 	 	 	 	 
35	 	 	 	 	 	 	r10	 	 	 	 	 	 	 	 	 	 	 	 	 
36	 	 	 	 	 	 	 	s34	 	 	s36	s35	 	 	 	 	 	 	 	38
37	 	 	 	 	 	 	r7	 	 	 	 	 	 	 	 	 	 	 	 	 
38	 	 	 	 	 	 	r11	 	 	 	 	 	 	 	 	 	 	 	 	 
"""

grammar = filter(None, grammar.splitlines())
grammar = list(map(lambda x:x.split("\t"), grammar))
dimension = "[{}][{}]".format(len(grammar), len(grammar[0]))

grammar = str(grammar)

grammar = grammar.replace("],","],\n")
grammar = grammar.replace("[","{").replace("]","}")
grammar = grammar.replace("'",'"')

print(grammar)
print(dimension)