Injected Utility Class
========================================

Library Provides simple template Injected utility class:

.. literalinclude:: ../../Include/SevenBit/DI/Injected.hpp
   :caption: Include/SevenBit/DI/Injected.hpp
   :language: C++

This class can be used as a base class for service implementation to inject services in simple inline way:

.. note::
   Class should inherit Injected constructor with 'using Injected::Injected;' in public section

.. literalinclude:: ../../Examples/Guides/InjectedUtilityClass.cpp
   :caption: Examples/Guides/InjectedUtilityClass
   :language: C++

.. code-block:: console
   :caption: Output

   actionA, actionB executed.
