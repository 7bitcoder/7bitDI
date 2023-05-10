Injecting Multiple Services
========================================

Multiple services can inherit one interface and can be injected using vector.

.. Note::
   Service objects are ordered in vector using registration order.
   Last registered serivice of base type is used when resolving one service

   Injection rules are simple:

   * It is guaranteed that vectors wont contain null pointners
   * Singleton/scped services should be injected using std::vector<pointners> 
   * Transient services should be injected using std::vector<std::unique_ptr> 


.. literalinclude:: ../../Examples/InjectingMultipleServices.cpp
   :caption: Examples/InjectingMultipleServices
   :language: C++

.. code-block:: console
   :caption: Output

   work all: work A done! work B done! work C done!
   single work: work C done!


