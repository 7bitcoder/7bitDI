Services LifeTime
========================================

Service provider can create scoped service provider:

.. code-block:: cpp

   IServiceProvider::Ptr provider = ServiceCollection{}.buildServiceProvider();
   
   IServiceProvider::Ptr scoped = provider->createScope()

Service can be registered as singleton, scoped or transient.

* Singleton: service provider will create only one instance of this service (accesable via getService method) 
* Scoped: service provider will create only one instance of this service for each scope (accesable via getService method) 
* Transient: services are always unique, a new instance is provided every time its requested, service provider returns in this case std::unique_ptr (accesable via createService method)

.. warning::
   Only transient services can be created using createService method otherwise method will throw exception 
  
   Only singleton/scoped services can be accessed using getService method otherwise method will throw exception 

.. literalinclude:: ../../Examples/LifeTime.cpp
   :caption: Examples/LifeTime
   :language: C++

Singleton service address is always the same.
Scoped service address is same in given provider scope.
Transient service address is always different.

.. literalinclude:: ../../Examples/LifeTimeExplanation.cpp
   :caption: Examples/LifeTimeExplanation
   :language: C++

.. code-block:: console
   :caption: Possible Output

   root provider: 
         16SingletonService pointer: 0x1005040b0
         13ScopedService pointer: 0x1005043e0
         16TransientService pointer: 0x100504460

         16SingletonService pointer: 0x1005040b0
         13ScopedService pointer: 0x1005043e0
         16TransientService pointer: 0x1006040a0

   scope1 provider: 
         16SingletonService pointer: 0x1005040b0
         13ScopedService pointer: 0x100404130
         16TransientService pointer: 0x100304580

         16SingletonService pointer: 0x1005040b0
         13ScopedService pointer: 0x100404130
         16TransientService pointer: 0x100304460

   scope2 provider: 
         16SingletonService pointer: 0x1005040b0
         13ScopedService pointer: 0x1004040d0
         16TransientService pointer: 0x100404390

         16SingletonService pointer: 0x1005040b0
         13ScopedService pointer: 0x1004040d0
         16TransientService pointer: 0x100604460