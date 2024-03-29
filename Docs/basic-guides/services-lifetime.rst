Services LifeTime
========================================

Service providers can create scoped service providers:

.. code-block:: cpp

   ServiceProvider provider = ServiceCollection{}.buildServiceProvider();
   
   ServiceProvider scoped = provider.createScope()

.. warning::
   Root/main service provider cannot be destroyed before its children scoped providers this can lead to undefined behavior

Service can be registered as a singleton, scoped, or transient.

* Singleton: service provider will create only one instance of this service (accessible via the getService method)
* Scoped: instance provider will create only one instance of this instance for each scope (accessible via the getService method)
* Transient: services are always unique, a new service is provided every time it is requested (accessible via createService or createInstanceInPlace method)

.. warning::
   Only transient services can be created using createInstance/createInstanceInPlace method otherwise the method will throw an exception
  
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
