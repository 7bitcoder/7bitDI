Using Aliases
========================================

With the use of aliases, one service can be injected through its multiple base classes, also aliases can be chained.
In case of injecting multiple aliases all real services will be fetched.

.. warning::
   Using aliases is resource intensive, especially for injecting transient and multiple services, provider recursively
   traverses through aliases chain to find proper service. Mixing scoped and singleton aliases for same base type will
   lead to undefined behavior

.. literalinclude:: ../../Examples/Guides/ServiceAliases.cpp
   :caption: Examples/Guides/ServiceAliases
   :language: C++

.. code-block:: console
   :caption: Output

   actionA from top service, actionB from top service executed.
