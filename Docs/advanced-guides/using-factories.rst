Using Factories
========================================

Factory functor can be provided to manually create a service.
Functor should return unique_ptr and as an argument should optionally take other services ().
Functor scheme (Services...) -> std::unique_ptr
Where Services are pointners, unique pointners, references, vectors with pointners or unique pointners
        

.. literalinclude:: ../../Examples/Guides/FactoryFunctions.cpp
   :caption: Examples/Guides/FactoryFunctions
   :language: C++

.. code-block:: console
   :caption: Output

   Hello from service.
