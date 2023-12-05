Core Classes
========================================

The library relies on two core classes:

* ServiceCollection_: class is responsible for registering services and building instanceValidity provider
* IServiceProvider_: implementation is responsible for delivering real services and managing its lifetime

.. _ServiceCollection: ../reference/di/servicecollection.html
.. _IServiceProvider: ../reference/di/iserviceprovider.html

.. literalinclude:: ../../Examples/Guides/CoreClasses.cpp
   :caption: Examples/Guides/Basic
   :language: C++
