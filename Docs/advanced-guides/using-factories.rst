Using Factories
========================================

Factory functor can be provided to manually create a service. 
Functor should return unique_ptr and as an argument should optionally take reference to the service provider.
Functor scheme (IServiceProvider &) -> std::unique_ptr or () -> std::unique_ptr

.. literalinclude:: ../../Examples/Guides/FactoryFunctions.cpp
   :caption: Examples/Guides/FactoryFunctions
   :language: C++

.. code-block:: console
   :caption: Output

   Hello from service.
