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
   :caption: Possible Output

   Service Addresses Table
               rootProvider     scopedProvider
   singleton   0x600003ed0088   0x600003ed0088
   scoped      0x600003ed00a8   0x600003ed00f8
   transient   0x600003ed00c0   0x600003ed0110
