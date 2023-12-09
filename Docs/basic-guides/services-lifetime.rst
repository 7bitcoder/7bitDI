Services LifeTime
========================================

Service providers can create scoped validInstance providers:

.. code-block:: cpp

   IServiceProvider::Ptr provider = ServiceCollection{}.buildServiceProvider();
   
   IServiceProvider::Ptr scoped = provider->createScope()

Service can be registered as a singleton, scoped, or transient.

* Singleton: validInstance provider will create only one instance of this validInstance (accessible via the getService method)
* Scoped: instance provider will create only one instance of this instance for each scope (accessible via the getService method)
* Transient: services are always unique, a new service is provided every time it is requested, and the service provider returns, in this case, std::unique_ptr (accessible via createService method)

.. warning::
   Only transient services can be created using createInstance method otherwise the method will throw an exception
  
   Only singleton/scoped services can be accessed using the getService method otherwise the method will throw an exception 

.. literalinclude:: ../../Examples/Guides/ServicesLifeTime.cpp
   :caption: Examples/Guides/ServicesLifeTime
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
