Services LifeTime
========================================

Service providers can create scoped service providers:

.. code-block:: cpp

   IServiceProvider::Ptr provider = ServiceCollection{}.buildServiceProvider();
   
   IServiceProvider::Ptr scoped = provider->createScope()

Service can be registered as singleton, scoped or transient.

* Singleton: service provider will create only one instance of this service (accessible via the getService method) 
* Scoped: service provider will create only one instance of this service for each scope (accessible via the getService method) 
* Transient: services are always unique, a new instance is provided every time it is requested, and the service provider returns, in this case, std::unique_ptr (accessible via createService method)

.. warning::
   Only transient services can be created using createService method otherwise the method will throw an exception 
  
   Only singleton/scoped services can be accessed using the getService method otherwise the method will throw an exception 

.. literalinclude:: ../../Examples/ServicesLifeTime.cpp
   :caption: Examples/ServicesLifeTime
   :language: C++

.. code-block:: console
   :caption: Output

   Singletons comparison
   rootProvider     == rootProvider:       1
   rootProvider     == scopedProvider:     1
   scopedProvider   == scopedProvider:     1

   Scoped comparison
   rootProvider     == rootProvider:       1
   rootProvider     == scopedProvider:     0
   scopedProvider   == scopedProvider:     1

   Transient comparison
   rootProvider     == rootProvider:       0
   rootProvider     == scopedProvider:     0
   scopedProvider   == scopedProvider:     0