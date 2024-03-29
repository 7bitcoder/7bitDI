Keyed Services
========================================

Multiple services with the same implementation can be registered using keyed services with addKeyed... ServiceCollection_
methods, similarly to normal services ServiceProvider_ has special Keyed methods to retrieve these services. Each keyed
service is created independently and has a different address, the default service can be retrieved or registered with an 
empty serviceKey = ""

.. _ServiceCollection: ../reference/di/servicecollection.html
.. _ServiceProvider: ../reference/di/serviceprovider.html

.. literalinclude:: ../../Examples/Guides/KeyedServices.cpp
   :caption: Examples/Guides/KeyedServices
   :language: C++

.. code-block:: console
   :caption: Output

   action, action, action executed.
