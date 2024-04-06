Injected Utility Class
========================================

Library provides also Injected_ utility class.
This base class has inject() method that can be used to inject services in simple inline way, also
there are InjectedSingleton, InjectedScoped and InjectedTransient base classes that are inheriting
from Injected and Registered classes to combine these two features. Injected class has also method
getProvider(), raw provider can be used to get keyed services for example.

.. _Injected: ../reference/di/utils/injected.html

.. note::
   Class should inherit Injected constructor with 'using Injected::Injected;' in public section

.. literalinclude:: ../../Examples/Guides/InjectedUtilityClass.cpp
   :caption: Examples/Guides/InjectedUtilityClass
   :language: C++

.. code-block:: console
   :caption: Output

   actionA, actionB executed.
