#!/usr/bin/env python3
'''Test API'''

import cmakeswig.Foo.pyTransport as foo
import cmakeswig.FooBar.pyFooBar as foobar


'''Test Transport'''
print(f'pyTransport: ${dir(foo)}')
foo.fooHello(1)
foo.fooHello(2147483647) # max int
foo.fooHello(2147483647+1) # max int + 1

f = foo.Foo()
print(f'class Foo: ${dir(f)}')
f.hello(1)
f.hello(2147483647)
f.hello(2147483647+1)


'''Test Bar'''
print(f'pyBar: ${dir(bar)}')
bar.barHello(1)
bar.barHello(2147483647) # max int
bar.barHello(2147483647+1) # max int + 1

b = bar.Bar()
print(f'class Bar: ${dir(b)}')
b.hello(1)
b.hello(2147483647)
b.hello(2147483647+1)


'''Test FooBar'''
print(f'pyFooBar: ${dir(foobar)}')
foobar.foobarHello(1)
foobar.foobarHello(2147483647) # max int
foobar.foobarHello(2147483647+1) # max int + 1

fb = foobar.FooBar()
print(f'class FooBar: ${dir(fb)}')
fb.hello(1)
fb.hello(2147483647)
fb.hello(2147483647+1)
