Injecting Multiple Services
========================================

Multiple services can inherit one interface and can be injected using vector.

.. Note::
   Service objects are ordered in vector using registration order.
   last registered serivice of base type is used when resolving one service

.. literalinclude:: ../../Examples/InterfacesMultiple.cpp
   :caption: Examples/InterfacesMultiple
   :language: cpp

.. code-block:: console
   :caption: Output

   work A done! work B done! work C done!


