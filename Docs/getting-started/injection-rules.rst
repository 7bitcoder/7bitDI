Injection Rules
========================================

Dependency injection mechanism relies havily on template metaprogramming and it has some limitations. The most important one is that we cannot inject service into service using reference.

General
---------------------
* Only one constructor should be defined for each service implementation
* If service is registered with interface and implementation, interface should have virtual descructor
* If multiple services are registered by the same interface, all should have the same life time (build method will throw exception)
* Only one service implementation can be registered (build method will throw exception)
* It is guaranteed that injected service wont be null

Injecting Services
---------------------
* Services cannot be injected by value: (T)
* Singleton/scoped services can be injected using one of:

  * Pointers: (T*)
  * Const pointner: (T* const)
  * Pointner to const object: (const T*)
  * Const pointner to const object: (const T* const)
* Transient services can be injected using std::unique_ptr: (unique_ptr<T>)
* Multiple services implementing specified interface can be injected using std::vector:

  * Transient (std::vector<std::unique_ptr<T>>)
  * Singleton/scoped (std::vector<T*>)

.. literalinclude:: ../../Examples/Rules.cpp
   :caption: Examples/Rules
   :language: C++


