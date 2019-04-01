#!/usr/bin/python2
template = 'tempReducingRules[{}] = vector_string{{"{}"}};'

orginalRules = """
E' -> E
E -> E add T
E -> E sub T
E -> T
T -> T mul F
T -> T div F
T -> F
F -> num
F -> sub F
F -> lparn E rparn
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