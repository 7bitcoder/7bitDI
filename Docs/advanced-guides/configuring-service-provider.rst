Configuring Service Provider
========================================

ServiceProviderOptions struct can be used to configure service provider,
read comment documentation for details:

.. literalinclude:: ../../Include/SevenBit/DI/ServiceProviderOptions.hpp
   :caption: Include/SevenBit/DI/ServiceProviderOptions.hpp
   :language: C++

Pass the custom options to the ServiceCollection buildServiceProvider method to change produced
service provider behaviour

.. literalinclude:: ../../Examples/Guides/ConfiguredServiceProvider.cpp
   :caption: Examples/Guides/ConfiguredServiceProvider
   :language: C++

.. code-block:: console
   :caption: Output

   Hello from service.
