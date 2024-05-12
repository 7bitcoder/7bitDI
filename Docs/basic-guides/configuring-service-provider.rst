Configuring Service Provider
========================================

ServiceProviderOptions_ struct can be used to configure service provider,
read comment documentation for details:

.. _ServiceProviderOptions: ../reference/di/serviceprovideroptions.html

.. literalinclude:: ../../Include/SevenBit/DI/ServiceProviderOptions.hpp
   :caption: Include/SevenBit/DI/ServiceProviderOptions.hpp
   :language: C++

Pass the custom options to the ServiceCollection buildServiceProvider method to change the produced
service provider behavior

.. literalinclude:: ../../Examples/Guides/ConfiguredServiceProvider.cpp
   :caption: Examples/Guides/ConfiguredServiceProvider
   :language: C++

.. code-block:: console
   :caption: Output

   Hello from service.
