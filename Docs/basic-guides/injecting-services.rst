Injecting Services
========================================

This example shows the main purpose of this library, injecting services into services.

.. note::
   Injection rules are simple:

   * Service should have one constructor
   * It is guaranteed that injected service won`t be null
   * Singleton/scoped service should be injected using pointers (references are not allowed due to library limitations, it might be fixed in the future) 
   * Transient services should be injected using std::unique_ptr 

.. literalinclude:: ../../Examples/Guides/InjectingServices.cpp
   :caption: Examples/Guides/InjectingServices
   :language: C++

.. code-block:: console
   :caption: Output

   actionA, actionB executed.


