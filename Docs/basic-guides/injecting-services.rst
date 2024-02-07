Injecting Services
========================================

This example shows the main purpose of this library, injecting services into services.

.. note::
   Injection rules are simple:

   * Service should have one constructor
   * Singleton/scoped service should be injected using pointers or references
   * Transient services should be injected using std::unique_ptr or directly by type if an object is movable or copyable

.. literalinclude:: ../../Examples/Guides/InjectingServices.cpp
   :caption: Examples/Guides/InjectingServices
   :language: C++

.. code-block:: console
   :caption: Output

   actionA, actionB executed.
