Injecting Services
========================================

This example shows the main purpose of this library, injecting services info services.

.. note::
   Injection rules are simple:

   * Service must have one constructor
   * Do not inject service using reference (library limitations, it might be fixed in future) 

.. literalinclude:: ../../Examples/InterfacesInjection.cpp
   :caption: Examples/InterfacesInjection
   :language: cpp

.. code-block:: console
   :caption: Output

   actionA, actionB executed.


