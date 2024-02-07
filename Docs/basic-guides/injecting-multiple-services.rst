Injecting Multiple Services
========================================

Multiple services can inherit one interface and can be injected using a vector.

.. Note::
   Service objects are ordered in vectors using registration order.
   The last registered service with the same base type is used when resolving one service

   Injection rules are simple:

   * It is guaranteed that vectors won't contain null pointers 
   * Singleton/scoped services should be injected using std::vector<pointers>
   * Transient services should be injected using std::vector<std::unique_ptr> 


.. literalinclude:: ../../Examples/Guides/InjectingMultipleServices.cpp
   :caption: Examples/Guides/InjectingMultipleServices
   :language: C++

.. code-block:: console
   :caption: Output

   work all: work A done! work B done! work C done!
   single work: work C done!
