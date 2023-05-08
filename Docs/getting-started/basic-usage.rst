Basic Usage
========================================

This is very trivial usage of this library 
(you could just create object inline using constructor)
but is shows the main idea behind project.

* Service collection class is responsible for registering services and building service provider
* Service provider is responsible for delivering real services and managing its lifetime

.. literalinclude:: ../../Examples/Basic.cpp
   :caption: Examples/Basic
   :language: cpp


.. code-block:: console
   :caption: Output

   Hello from service.



