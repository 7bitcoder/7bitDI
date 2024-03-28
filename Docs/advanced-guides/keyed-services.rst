Keyed Services
========================================

Multiple services with the same implementation can be registered using keyed services with
addKeyed... ServiceCollection methods, similarly to normal services ServiceProvider has special Keyed methods to retrieve these services.
Each keyed service is instantiated independently and has different address, default service can be retrieved with empty serviceKey = ""


.. literalinclude:: ../../Examples/Guides/KeyedServices.cpp
   :caption: Examples/Guides/KeyedServices
   :language: C++

.. code-block:: console
   :caption: Output

   action, action, action executed.
