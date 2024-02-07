Using Aliases
========================================

With the use of aliases, one service can be injected through its multiple base classes, also aliases can be chained.
If multiple aliases are registered with the same base class only last will be used.


.. literalinclude:: ../../Examples/Guides/ServiceAliases.cpp
   :caption: Examples/Guides/ServiceAliases
   :language: C++

.. code-block:: console
   :caption: Output

   actionA from top service, actionB from top service executed.
