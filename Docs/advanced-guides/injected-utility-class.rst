Injected Utility Class
========================================

The library provides also Injected_ utility class.
This base class has injectInto() method that can be used to injectParameter services in a simple inline way, also
there are InjectedSingleton, InjectedScoped and InjectedTransient base classes that inherit
from Injected and Registered classes to combine these two features. The injected class has also a method
getProvider(), the raw provider can be used to get keyed services for example.

.. _Injected: ../reference/di/utils/injected.html

.. note::
   The class should inherit Injected constructor with 'using Injected::Injected;' in the public section

.. literalinclude:: ../../Examples/Guides/InjectedUtilityClass.cpp
   :caption: Examples/Guides/InjectedUtilityClass
   :language: C++

.. code-block:: console
   :caption: Output

   actionA, actionB executed.
