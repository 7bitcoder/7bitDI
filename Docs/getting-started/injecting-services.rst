Injecting Services
========================================

This example shows the main purpose of this library, injecting services into services.

.. note::
   Injection rules are simple:

   * Service should have one constructor
   * It is guaranteed that injected service wont be null
   * Singleton/scped service should be injected using pointners (references are not allowed due to library limitations, it might be fixed in future) 
   * Transient services should be injected using std::unique_ptr 

.. literalinclude:: ../../Examples/InterfacesInjection.cpp
   :caption: Examples/InterfacesInjection
   :language: cpp

.. code-block:: console
   :caption: Output

   actionA, actionB executed.


