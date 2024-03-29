Injecting Service Provider
========================================

ServiceProvider_ object can be injected and can be used to manually get (scoped/singletons) or create (transient) services.

.. _ServiceProvider: ../reference/di/serviceprovider.html

.. literalinclude:: ../../Examples/Guides/InjectingServiceProvider.cpp
   :caption: Examples/Guides/InjectingServiceProvider
   :language: C++

.. code-block:: console
   :caption: Output

   actionA, actionB executed.
