Using Factories
========================================

Factory functor can be provided to manually create a service.
Functor should return unique_ptr or raw object if the type is movable/copyable and should optionally take other services as arguments.

Functor scheme (Services...) -> std::unique_ptr< T> | T
where services are pointers, in-place objects (if an object is movable or copyable), unique pointers, references, vectors with pointers or unique pointers
        
.. literalinclude:: ../../Examples/Guides/FactoryFunctions.cpp
   :caption: Examples/Guides/FactoryFunctions
   :language: C++

.. code-block:: console
   :caption: Output

   Hello from service.
