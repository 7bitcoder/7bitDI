Register Utility Class
========================================

The library provides a simple template RegisterService_ utility class.
This base class can be used to automatically register class in service collection with the use of a specialized TRegisterer.
There are already created aliases RegisterSingleton_, RegisterScoped_, RegisterTransient_ that are registering services
in GlobalServices_ singleton.

.. note::
   The class should inherit Injected constructor with 'using Injected::Injected;' in the public section

.. _RegisterService: ../reference/di/utils/register.html
.. _RegisterSingleton: ../reference/di/utils/register.html
.. _RegisterScoped: ../reference/di/utils/register.html
.. _RegisterTransient: ../reference/di/utils/register.html
.. _GlobalServices: ../reference/di/utils/globalservices.html

.. literalinclude:: ../../Examples/Guides/RegisterUtilityClass.cpp
   :caption: Examples/Guides/RegisterUtilityClass
   :language: C++

.. code-block:: console
   :caption: Output

   actionA_, actionB executed.
