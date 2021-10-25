#!/usr/bin/env python3
'''Test API'''

import unittest
import cmakeswig.Transport.pyTransport as transport
import cmakeswig.FooBar.pyFooBar as foobar


class TestFoo(unittest.TestCase):
    # def test_foo_method(self):
    #     f = transport.Transport()
    #     print(f'class Foo: ${dir(f)}')

    # def test_foo_pdr(self):
    #     f = transport.Transport()
    #     pdr = f.getPDR()
    #     dbl = pdr.doubleVal(0)
    #     self.assertEqual(dbl, 0.)
    #     print(f'class Foo: pdr ${dir(pdr)}')

    # def test_foo_numerics(self):
    #     f = transport.Transport()
    #     pdr = f.getPDR()
    #     dbl = pdr.getNumerics()
    #     print(dbl[0])
    #     print(f'class numeric values: pdr ${dir(dbl)}')
    #     self.assertEqual(dbl[0], 0.)

    def test_map(self):
        f = transport.Transport()
        pdr = f.getPDR()
        dbl = pdr.getMap()
        # print(dbl["val1"])
        # print(f'class numeric values: pdr ${dir(dbl)}')
        print(f'dbl: pdr ${dbl}')
        # self.assertEqual(dbl["val1"], 0.)
        

if __name__ == '__main__':
    unittest.main(verbosity=2)
