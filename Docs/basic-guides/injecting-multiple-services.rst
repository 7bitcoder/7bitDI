Injecting Multiple Services
========================================

Multiple services can inherit one interface and can be injected using a vector.

.. Note::
   Service objects are ordered in vectors using registration order.
   The last registered service of base type is used when resolving one service

   Injection rules are simple:

   * It is guaranteed that vectors won't contain null pointners
   * Singleton/scoped services should be injected using std::vector<pointners> 
   * Transient services should be injected using std::vector<std::unique_ptr> 


.. literalinclude:: ../../Examples/InjectingMultipleServices.cpp
   :caption: Examples/InjectingMultipleServices
   :language: C++

.. code-block:: console
   :caption: Output

   work all: work A done! work B done! work C done!
   single work: work C done!

