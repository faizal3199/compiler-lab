#!/usr/bin/python2

grammar = """
0	 	s5	 	 	s4	s6	 	 	 	1	2	3
1	s7	s8	 	 	 	 	 	acc	 	 	 	 
2	r3	r3	s9	s10	 	 	 	r3	 	 	 	 
3	r6	r6	r6	r6	 	 	 	r6	 	 	 	 
4	r7	r7	r7	r7	 	 	 	r7	 	 	 	 
5	 	s5	 	 	s4	s6	 	 	 	 	 	11
6	 	s16	 	 	s15	s17	 	 	 	12	13	14
7	 	s5	 	 	s4	s6	 	 	 	 	18	3
8	 	s5	 	 	s4	s6	 	 	 	 	19	3
9	 	s5	 	 	s4	s6	 	 	 	 	 	20
10	 	s5	 	 	s4	s6	 	 	 	 	 	21
11	r8	r8	r8	r8	 	 	 	r8	 	 	 	 
12	s23	s24	 	 	 	 	s22	 	 	 	 	 
13	r3	r3	s25	s26	 	 	r3	 	 	 	 	 
14	r6	r6	r6	r6	 	 	r6	 	 	 	 	 
15	r7	r7	r7	r7	 	 	r7	 	 	 	 	 
16	 	s16	 	 	s15	s17	 	 	 	 	 	27
17	 	s16	 	 	s15	s17	 	 	 	28	13	14
18	r1	r1	s9	s10	 	 	 	r1	 	 	 	 
19	r2	r2	s9	s10	 	 	 	r2	 	 	 	 
20	r4	r4	r4	r4	 	 	 	r4	 	 	 	 
21	r5	r5	r5	r5	 	 	 	r5	 	 	 	 
22	r9	r9	r9	r9	 	 	 	r9	 	 	 	 
23	 	s16	 	 	s15	s17	 	 	 	 	29	14
24	 	s16	 	 	s15	s17	 	 	 	 	30	14
25	 	s16	 	 	s15	s17	 	 	 	 	 	31
26	 	s16	 	 	s15	s17	 	 	 	 	 	32
27	r8	r8	r8	r8	 	 	r8	 	 	 	 	 
28	s23	s24	 	 	 	 	s33	 	 	 	 	 
29	r1	r1	s25	s26	 	 	r1	 	 	 	 	 
30	r2	r2	s25	s26	 	 	r2	 	 	 	 	 
31	r4	r4	r4	r4	 	 	r4	 	 	 	 	 
32	r5	r5	r5	r5	 	 	r5	 	 	 	 	 
33	r9	r9	r9	r9	 	 	r9	 	 	 	 	 
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