Injected Utility Class
========================================

Library provides also Injected_ utility class.
This base class has inject() method that can be used to inject services in simple inline way, also
there are InjectedSingleton, InjectedScoped and InjectedTransient base classes that are inheriting
from Injected and Registered classes to combine these two features

.. note::
   Class should inherit Injected constructor with 'using Injected::Injected;' in public section

.. literalinclude:: ../../Examples/Guides/InjectedUtilityClass.cpp
   :caption: Examples/Guides/InjectedUtilityClass
   :language: C++

.. code-block:: console
   :caption: Output

   actionA, actionB executed.
