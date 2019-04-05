#!/usr/bin/python2
template = 'tempReducingRules[{}] = vector_string{{"{}"}};'

orginalRules = """
W' -> W
W -> while C begin S end
C -> lparn I rop I rparn
S -> P semi S
S -> ''
P -> id aop E
E -> I mop I
E -> I sub I
E -> I
I -> id
I -> num
I -> sub I
"""

orginalRules = filter(None, orginalRules.splitlines())
rules = orginalRules

rules = list(map(lambda x:filter(None, x.replace('->',' ').split(" ")), rules))

i = 0
while i<len(rules):
    print("// {}".format(orginalRules[i]))
    _ = '", "'.join(rules[i])
    print(template.format(i, _).replace('"\'\'"','""'))
    i += 1