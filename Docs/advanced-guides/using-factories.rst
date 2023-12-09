Using Factories
========================================

Factory functor can be provided to manually create a service.
Functor should return unique_ptr and as an argument should optionally take other services ().
Functor scheme (Services...) -> std::unique_ptr
Where Services are pointers, unique pointers, references, vectors with pointers or unique pointers
        

.. literalinclude:: ../../Examples/Guides/FactoryFunctions.cpp
   :caption: Examples/Guides/FactoryFunctions
   :language: C++

.. code-block:: console
   :caption: Output

   Hello from service.
