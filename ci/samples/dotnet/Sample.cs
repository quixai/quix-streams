using System;
using Xunit;

using Mizux.CMakeSwig.Foo;
using Mizux.CMakeSwig.Bar;
using Mizux.CMakeSwig.FooBar;

namespace Mizux.Sample.Tests {
  public class FooTest {
    [Theory]
    [InlineData(false)]
    [InlineData(true)]
    public void IntegerTest(bool callGC) {
      // Instantiate Foo
      Foo obj = new Foo();

      if (callGC) {
        GC.Collect();
      }

      obj.SetInt(42);
      Assert.Equal(42, obj.GetInt());
    }

    [Theory]
    [InlineData(false)]
    [InlineData(true)]
    public void Integer64Test(bool callGC) {
      // Instantiate Foo
      Foo obj = new Foo();

      if (callGC) {
        GC.Collect();
      }

      long a = 2147483647;
      obj.SetInt64(a);
      Assert.Equal(a, obj.GetInt64());

      long b = 2147483648;
      obj.SetInt64(b);
      Assert.Equal(b, obj.GetInt64());
    }
  }

  public class BarTest {
    [Theory]
    [InlineData(false)]
    [InlineData(true)]
    public void IntegerTest(bool callGC) {
      // Instantiate Bar
      Bar obj = new Bar();

      if (callGC) {
        GC.Collect();
      }

      obj.SetInt(42);
      Assert.Equal(42, obj.GetInt());
    }

    [Theory]
    [InlineData(false)]
    [InlineData(true)]
    public void Integer64Test(bool callGC) {
      // Instantiate Bar
      Bar obj = new Bar();

      if (callGC) {
        GC.Collect();
      }

      long a = 2147483647;
      obj.SetInt64(a);
      Assert.Equal(a, obj.GetInt64());

      long b = 2147483648;
      obj.SetInt64(b);
      Assert.Equal(b, obj.GetInt64());
    }
  }

  public class FooBarTest {
    [Theory]
    [InlineData(false)]
    [InlineData(true)]
    public void IntegerTest(bool callGC) {
      // Instantiate FooBar
      FooBar obj = new FooBar();

      if (callGC) {
        GC.Collect();
      }

      obj.SetFooInt(42);
      obj.SetBarInt(97);
      Assert.Equal(42+97, obj.GetInt());
    }

    [Theory]
    [InlineData(false)]
    [InlineData(true)]
    public void Integer64Test(bool callGC) {
      // Instantiate FooBar
      FooBar obj = new FooBar();

      if (callGC) {
        GC.Collect();
      }

      long a = 1073741824;
      long b = 1073741825;
      obj.SetFooInt64(a);
      obj.SetBarInt64(b);
      Assert.Equal(2147483649, obj.GetInt64());
    }
  }
} // namespace Mizux.Sample.Tests
