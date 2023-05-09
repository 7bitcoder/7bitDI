Injecting Service Provider
========================================

Service Provider object can be injected and can be used to manually get (scoped/singletons) or create (transient) services.

.. literalinclude:: ../../Examples/SelfInjection.cpp
   :caption: Examples/SelfInjection
   :language: C++

.. code-block:: console
   :caption: Output

   actionA, actionB executed.


